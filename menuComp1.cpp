// Implementación de operaciones básicas

#include "menu.h"

// Verificar si un objeto se encuentra cargado en memoria
bool Menu::objetoCargado(const std::string& nombre_objeto) {
    for (int i = 0; i < objetosCargados.size(); i++) {
        if (objetosCargados[i].getNombre() == nombre_objeto) {
            return true;
        }
    }
    return false;
}

// Salir del programa
void Menu::salirPrograma() {
    std::cout << "Saliendo del programa..." << std::endl;
    exit(0);
}

// Función para eliminar espacios en blanco y caracteres especiales al inicio y al final
void Menu::trim(std::string &s) {
    // Eliminar espacios y caracteres especiales al principio
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
        return !std::isspace(c) && c != '\n' && c != '\t' && c != '\r';
    }));
    // Eliminar espacios y caracteres especiales al final
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {
        return !std::isspace(c) && c != '\n' && c != '\t' && c != '\r';
    }).base(), s.end());
}

// Cargar objeto en memoria
void Menu::cargarObjeto(const std::string& nombre_archivo) {
    std::ifstream file(nombre_archivo);
    if (!file.is_open()) {
        std::cout << "(Archivo no existe) El archivo " << nombre_archivo << " no existe o es ilegible." << std::endl;
        return;
    }

    std::string mesh_name;
    int n_vertices;
    std::vector<Vertice> vertices;
    std::vector<Cara> caras;
    std::string line;

    if (std::getline(file, mesh_name)) {
        trim(mesh_name);
        if (objetoCargado(mesh_name)) {
            std::cout << "(Objeto ya existe) El objeto " << mesh_name << " ya ha sido cargado en memoria." << std::endl;
            return;
        }
    } else {
        std::cout << "(Archivo vacío o incompleto) El archivo " << nombre_archivo << " no contiene un objeto 3D válido." << std::endl;
        return;
    }

    if (std::getline(file, line)) {
        trim(line);
        try {
            n_vertices = std::stoi(line);
        } catch (...) {
            std::cout << "(Formato inválido) El número de vértices no es válido en el archivo " << nombre_archivo << "." << std::endl;
            return;
        }
    } else {
        std::cout << "(Archivo vacío o incompleto) El archivo " << nombre_archivo << " no contiene un objeto 3D válido." << std::endl;
        return;
    }

    for (int i = 0; i < n_vertices; ++i) {
        if (std::getline(file, line)) {
            trim(line);
            std::stringstream ss(line);
            double x, y, z;
            if (!(ss >> x >> y >> z)) {
                std::cout << "(Formato inválido) Las coordenadas de los vértices no son válidas en el archivo " << nombre_archivo << "." << std::endl;
                return;
            }
            vertices.push_back(Vertice(x, y, z));
        } else {
            std::cout << "(Archivo vacío o incompleto) El archivo " << nombre_archivo << " no contiene un objeto 3D válido." << std::endl;
            return;
        }
    }

    while (std::getline(file, line)) {
        trim(line);
        if (line == "-1") break;
        std::stringstream ss(line);
        int num_vertices;
        if (!(ss >> num_vertices) || num_vertices <= 0) {
            std::cout << "(Formato inválido) El número de vértices de la cara no es válido en el archivo " << nombre_archivo << "." << std::endl;
            return;
        }

        std::vector<int> indices(num_vertices);
        for (int i = 0; i < num_vertices; ++i) {
            if (!(ss >> indices[i])) {
                std::cout << "(Formato inválido) Los índices de los vértices no son válidos en el archivo " << nombre_archivo << "." << std::endl;
                return;
            }
        }

        Cara cara;
        for (int index : indices) {
            if (index >= 0 && index < vertices.size()) {
                cara.agregarVertice(vertices[index]);
            } else {
                std::cout << "(Índice inválido) El índice del vértice en la cara es inválido en el archivo " << nombre_archivo << "." << std::endl;
                return;
            }
        }
        caras.push_back(cara);
    }

    Objeto objeto(mesh_name);
    for (const Cara& cara : caras) {
        objeto.agregarCara(cara);
    }
    objeto.setVertices(vertices);
    objetosCargados.push_back(objeto);

    std::cout << "(Resultado exitoso) El objeto " << mesh_name << " ha sido cargado exitosamente desde el archivo " << nombre_archivo << "." << std::endl;
}

// Listar objetos cargados en memoria
void Menu::listarObjetosCargados() {
    if (objetosCargados.empty()) {
        std::cout << "Ningún objeto ha sido cargado en memoria." << std::endl;
        return;
    }

    std::cout << "Hay " << objetosCargados.size() << " objetos en memoria:" << std::endl;
    for (const Objeto& objeto : objetosCargados) {
        std::cout << objeto.toString() << std::endl;
    }
}

// Crear caja de ajuste para un objeto determinado
void Menu::calcularCajaEnvolvente(const std::string& nombre_objeto) {
    Objeto* objetoEncontrado = nullptr;
    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombre_objeto) {
            objetoEncontrado = &objetosCargados[i];
            break;
        }
    }

    if (!objetoEncontrado) {
        std::cout << "(Objeto no existe) El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }

    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double zmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    double zmax = std::numeric_limits<double>::lowest();

    for (const Cara& cara : objetoEncontrado->getCaras()) {
        for (const Vertice& vertice : cara.getVertices()) {
            double x = vertice.getX();
            double y = vertice.getY();
            double z = vertice.getZ();

            if (x < xmin) xmin = x;
            if (y < ymin) ymin = y;
            if (z < zmin) zmin = z;
            if (x > xmax) xmax = x;
            if (y > ymax) ymax = y;
            if (z > zmax) zmax = z;
        }
    }

    std::string nombreCaja = "env_" + nombre_objeto;
    int contador = 1;
    while (objetoCargado(nombreCaja)) {
        std::ostringstream oss;
        oss << "env_" << nombre_objeto << contador;
        nombreCaja = oss.str();
        ++contador;
    }

    Objeto cajaEnvolvente(nombreCaja);

    std::vector<Vertice> vertices = {
        Vertice(xmin, ymin, zmin),
        Vertice(xmin, ymin, zmax),
        Vertice(xmin, ymax, zmin),
        Vertice(xmin, ymax, zmax),
        Vertice(xmax, ymin, zmin),
        Vertice(xmax, ymin, zmax),
        Vertice(xmax, ymax, zmin),
        Vertice(xmax, ymax, zmax)
    };

    std::vector<Cara> caras = {
        Cara({vertices[0], vertices[1], vertices[3], vertices[2]}),
        Cara({vertices[4], vertices[5], vertices[7], vertices[6]}),
        Cara({vertices[0], vertices[1], vertices[5], vertices[4]}),
        Cara({vertices[1], vertices[3], vertices[7], vertices[5]}),
        Cara({vertices[3], vertices[2], vertices[6], vertices[7]}),
        Cara({vertices[2], vertices[0], vertices[4], vertices[6]})
    };

    cajaEnvolvente.setCaras(caras);

    objetosCargados.push_back(cajaEnvolvente);

    std::cout << "(Resultado exitoso) La caja envolvente del objeto " << nombre_objeto
              << " se ha generado con el nombre " << nombreCaja
              << " y se ha agregado a los objetos en memoria." << std::endl;
}

// Crear caja de ajuste global
void Menu::crearCajaDeAjusteGlobal() {
    if (objetosCargados.empty()) {
        std::cout << "(Memoria vacía) Ningún objeto ha sido cargado en memoria." << std::endl;
        return;
    }

    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double zmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    double zmax = std::numeric_limits<double>::lowest();

    for (const Objeto& objeto : objetosCargados) {
        for (const Cara& cara : objeto.getCaras()) {
            for (const Vertice& vertice : cara.getVertices()) {
                double x = vertice.getX();
                double y = vertice.getY();
                double z = vertice.getZ();

                if (x < xmin) xmin = x;
                if (y < ymin) ymin = y;
                if (z < zmin) zmin = z;
                if (x > xmax) xmax = x;
                if (y > ymax) ymax = y;
                if (z > zmax) zmax = z;
            }
        }
    }

    Objeto cajaGlobal("env_global");

    std::vector<Vertice> vertices = {
        Vertice(xmin, ymin, zmin),
        Vertice(xmin, ymin, zmax),
        Vertice(xmin, ymax, zmin),
        Vertice(xmin, ymax, zmax),
        Vertice(xmax, ymin, zmin),
        Vertice(xmax, ymin, zmax),
        Vertice(xmax, ymax, zmin),
        Vertice(xmax, ymax, zmax)
    };

    std::vector<Cara> caras = {
        Cara({vertices[0], vertices[1], vertices[3], vertices[2]}),
        Cara({vertices[4], vertices[5], vertices[7], vertices[6]}),
        Cara({vertices[0], vertices[1], vertices[5], vertices[4]}),
        Cara({vertices[1], vertices[3], vertices[7], vertices[5]}),
        Cara({vertices[3], vertices[2], vertices[6], vertices[7]}),
        Cara({vertices[2], vertices[0], vertices[4], vertices[6]})
    };

    cajaGlobal.setCaras(caras);

    objetosCargados.push_back(cajaGlobal);

    std::cout << "(Resultado exitoso) La caja de ajuste global se ha generado con el nombre env_global "
              << "y se ha agregado a los objetos en memoria." << std::endl;
}


// Elimina un objeto
void Menu::descargarObjeto(std::string& nombre_objeto) {
    auto it = std::find_if(objetosCargados.begin(), objetosCargados.end(),
        [&nombre_objeto](const Objeto& obj) { return obj.getNombre() == nombre_objeto; });

    if (it == objetosCargados.end()) {
        std::cout << "(Objeto no existe) El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }

    objetosCargados.erase(it);
    std::cout << "(Resultado exitoso) El objeto " << nombre_objeto << " ha sido eliminado de la memoria de trabajo." << std::endl;
}

// Guardar Objeto
void Menu::guardarObjetoEnArchivo(const std::string& nombre_objeto, const std::string& nombre_archivo) {
    // Buscar el objeto por nombre
    Objeto* objetoEncontrado = nullptr;
    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombre_objeto) {
            objetoEncontrado = &objetosCargados[i];
            break;
        }
    }

    if (!objetoEncontrado) {
        // Objeto no encontrado
        std::cout << "(Objeto no existe) El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }

    // Objeto encontrado, escribir en el archivo
    const Objeto& objeto = *objetoEncontrado;
    std::ofstream archivo(nombre_archivo);

    if (!archivo) {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
        return;
    }

    // Escribir el nombre del objeto
    archivo << objeto.getNombre() << std::endl;

    // Crear un vector de vértices únicos
    std::vector<Vertice> verticesUnicos;
    for (int i = 0; i < objeto.getCaras().size(); ++i) {
        const Cara& cara = objeto.getCaras()[i];
        for (int j = 0; j < cara.getNumeroVertices(); ++j) {
            const Vertice& verticeCara = cara.getVertices()[j];
            bool encontrado = false;
            for (int k = 0; k < verticesUnicos.size(); ++k) {
                if (verticesUnicos[k] == verticeCara) {
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) {
                verticesUnicos.push_back(verticeCara);
            }
        }
    }

    // Escribir el número de vértices
    int numVertices = verticesUnicos.size();
    archivo << numVertices << std::endl;

    // Escribir los vértices
    for (int i = 0; i < numVertices; ++i) {
        const Vertice& vertice = verticesUnicos[i];
        archivo << vertice.getX() << ' ' << vertice.getY() << ' ' << vertice.getZ() << std::endl;
    }

    // Escribir las caras
    for (int i = 0; i < objeto.getCaras().size(); ++i) {
        const Cara& cara = objeto.getCaras()[i];
        int numVerticesCara = cara.getNumeroVertices();
        archivo << numVerticesCara;
        for (int j = 0; j < numVerticesCara; ++j) {
            const Vertice& verticeCara = cara.getVertices()[j];
            // Encontrar el índice del vértice en verticesUnicos
            int indice = -1;
            for (int k = 0; k < numVertices; ++k) {
                if (verticesUnicos[k] == verticeCara) {
                    indice = k;
                    break;
                }
            }
            archivo << ' ' << indice;
        }
        archivo << std::endl;
    }

    // Escribir el final del archivo
    archivo << "-1" << std::endl;
    archivo.close();

    // Mensaje de éxito
    std::cout << "(Resultado exitoso) La información del objeto " << nombre_objeto
              << " ha sido guardada exitosamente en el archivo " << nombre_archivo << "." << std::endl;
}