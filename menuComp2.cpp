// Implementación de operaciones relacionadas con vértices más cercanos 

#include "menu.h"
#include "KDTree.h"
#include <sstream>
#include <iomanip> 

// Encuentra el vértice más cercano en un objeto específico
void Menu::verticeCercanoObjeto(double px, double py, double pz, std::string& nombre_objeto) {
    // Verificar si el objeto está cargado
    if (!objetoCargado(nombre_objeto)) {
        std::cout << "(Objeto no existe) El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }
    // Buscar el objeto en la lista de objetos cargados
    Objeto* objetoEncontrado = nullptr;
    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombre_objeto) {
            objetoEncontrado = &objetosCargados[i];
            break;
        }
    }
    
    // Se crea un KD-Tree con los vértices del objeto
    KDTree kdTree;
    for (const Cara& cara : objetoEncontrado->getCaras()) {
        for (const Vertice& vertice : cara.getVertices()) {
            kdTree.insertar(vertice, nombre_objeto);  
        }
    }
    
    // Se define el punto objetivo
    Vertice puntoObjetivo(px, py, pz);

    // Buscar el nodo más cercano usando el KD-Tree
    NodoKDTree* nodoMasCercano = kdTree.encontrarMasCercano(puntoObjetivo);

    if (nodoMasCercano) {
        // Obtener el vértice más cercano
        Vertice verticeMasCercano = nodoMasCercano->getPunto();

        // Calcular la distancia entre el punto objetivo y el vértice más cercano
        double distanciaMinima = verticeMasCercano.distancia(puntoObjetivo);

        // Calcular indice del vértice
        int indiceVertice = -1;
        const std::vector<Vertice>& verticesObjeto = objetoEncontrado->getVertices();
        for (int i = 0; i < verticesObjeto.size(); ++i) {
            if (verticesObjeto[i] == verticeMasCercano) {
                indiceVertice = i;
                break;
            }
        }

        // Mostrar el vértice más cercano y su distancia
        std::cout << "(Resultado exitoso) El vértice " << indiceVertice 
                  << " (" << verticeMasCercano.getX() << ", " << verticeMasCercano.getY() 
                  << ", " << verticeMasCercano.getZ() << ") del objeto " << nombre_objeto
                  << " es el más cercano al punto (" << px << ", " << py << ", " << pz 
                  << "), a una distancia de " << distanciaMinima << ".\n";
    } else {
        std::cout << "(Error) No se encontró un vértice cercano.\n";
    }
}

//Indica el vertice mas cercano a un punto dado de todos los objetos cargados en memoria 
void Menu::verticeCercanoTodosObjetos(double px, double py, double pz) {
    if (objetosCargados.empty()) {
        std::cout << "Ningún objeto ha sido cargado en memoria." << std::endl;
        return;
    }
    
    KDTree kdTree;
    std::vector<std::string> objetosMasCercanos;
    double distanciaMinima = std::numeric_limits<double>::max();

    // Insertar los vértices de todos los objetos en el KD-Tree
    for (const Objeto& objeto : objetosCargados) {
        for (const Cara& cara : objeto.getCaras()) {
            for (const Vertice& vertice : cara.getVertices()) {
                kdTree.insertar(vertice, objeto.getNombre());
            }
        }
    }

    // Definir el punto objetivo
    Vertice puntoObjetivo(px, py, pz);

    // Buscar el nodo más cercano usando el KD-Tree
    NodoKDTree* nodoCercano = kdTree.encontrarMasCercano(puntoObjetivo);

    if (nodoCercano) {
        Vertice verticeMasCercano = nodoCercano->getPunto();
        objetosMasCercanos = nodoCercano->getNombresObjetos();

        // Calcular la distancia entre el punto objetivo y el vértice más cercano
        distanciaMinima = verticeMasCercano.distancia(puntoObjetivo);

        // Calcular índice del vértice más cercano
        int indiceVertice = -1;
        for (const Objeto& objeto : objetosCargados) {
            const std::vector<Vertice>& verticesObjeto = objeto.getVertices();
            for (int j = 0; j < verticesObjeto.size(); ++j) {
                if (verticesObjeto[j] == verticeMasCercano) {
                    indiceVertice = j;
                    break;
                }
            }
            if (indiceVertice != -1) break;
        }

        // Si el vértice pertenece a un solo objeto
        if (objetosMasCercanos.size() == 1) {
            std::cout << "(Resultado exitoso) El vértice " << indiceVertice
                      << " (" << verticeMasCercano.getX() << ", " << verticeMasCercano.getY() 
                      << ", " << verticeMasCercano.getZ() << ") del objeto " << objetosMasCercanos[0]
                      << " es el más cercano al punto (" << px << ", " << py << ", " << pz 
                      << "), a una distancia de " << distanciaMinima << ".\n";
        }
        // Si el vértice pertenece a más de un objeto
        else {
            std::cout << "(Resultado exitoso) El vértice " << indiceVertice
                      << " (" << verticeMasCercano.getX() << ", " << verticeMasCercano.getY() 
                      << ", " << verticeMasCercano.getZ() << ") de los objetos ";

            for (size_t i = 0; i < objetosMasCercanos.size(); ++i) {
                std::cout << objetosMasCercanos[i];
                if (i < objetosMasCercanos.size() - 1) {
                    std::cout << ", ";
                }
            }

            std::cout << " es el más cercano al punto (" << px << ", " << py << ", " << pz 
                      << "), a una distancia de " << distanciaMinima << ".\n";
        }
    } else {
        std::cout << "(Error) No se encontró un vértice cercano.\n";
    }
}

// Identifica los vértices del objeto más cercanos a los de su caja envolvente
void Menu::verticesCercanosCaja(std::string& nombre_objeto) {
    // Verificar si el objeto está cargado
    if (!objetoCargado(nombre_objeto)) {
        std::cout << "(Objeto no existe) El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }

    bool cajaCreada = false;
    std::string nombreCaja = "env_" + nombre_objeto;

    // Si la caja envolvente no está creada
    if (!objetoCargado(nombreCaja)) {
        calcularCajaEnvolvente(nombre_objeto);
        cajaCreada = true;
    }

    // Obtener el objeto de la caja envolvente
    Objeto* cajaEnvolvente = nullptr;
    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombreCaja) {
            cajaEnvolvente = &objetosCargados[i];
            break;
        }
    }

    // Verificar que la caja envolvente se haya encontrado
    if (!cajaEnvolvente) {
        std::cout << "(Error) No se pudo encontrar o crear la caja envolvente." << std::endl;
        return;
    }

    // Crear un vector para almacenar los vértices del objeto original y sus índices
    std::vector<Vertice> verticesUnicos;
    std::vector<int> indicesVertices;

    Objeto* objetoOriginal = nullptr;
    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombre_objeto) {
            objetoOriginal = &objetosCargados[i];
            break;
        }
    }

    // Insertar los vértices únicos del objeto en los vectores
    for (const Cara& cara : objetoOriginal->getCaras()) {
        for (const Vertice& vertice : cara.getVertices()) {
            auto it = std::find(verticesUnicos.begin(), verticesUnicos.end(), vertice);
            if (it == verticesUnicos.end()) {
                verticesUnicos.push_back(vertice);
                indicesVertices.push_back(verticesUnicos.size() - 1);
            } else {
                indicesVertices.push_back(std::distance(verticesUnicos.begin(), it));
            }
        }
    }

    // Declarar la variable esquinas
    std::vector<Vertice> esquinas;

    // Buscar vértices más cercanos para cada esquina de la caja envolvente
    for (const Cara& cara : cajaEnvolvente->getCaras()) {
        for (const Vertice& vertice : cara.getVertices()) {
            bool yaExiste = false;
            for (const Vertice& esquina : esquinas) {
                if (esquina == vertice) { 
                    yaExiste = true;
                    break;
                }
            }
            if (!yaExiste) {
                esquinas.push_back(vertice);
            }
        }
    }

    // Mejorar la alineación de los títulos
    std::cout << "(Resultado exitoso) Los vertices del objeto " << nombre_objeto
              << " más cercanos a las esquinas de su caja envolvente son:\n";
    std::cout << std::setw(10) << "Esquina"
              << std::setw(20) << "Vertice"
              << std::setw(15) << "Distancia" << "\n";
    std::cout << "-------------------------------------------\n";

    // Buscar el vértice más cercano a cada esquina
    for (int i = 0; i < esquinas.size(); ++i) {
        Vertice esquina = esquinas[i];
        double minDistancia = std::numeric_limits<double>::max();
        int indiceVerticeMasCercano = -1;
        Vertice verticeMasCercano;

        // Comparar con cada vértice único para encontrar el más cercano
        for (int j = 0; j < verticesUnicos.size(); ++j) {
            double distancia = verticesUnicos[j].distancia(esquina);
            if (distancia < minDistancia) {
                minDistancia = distancia;
                verticeMasCercano = verticesUnicos[j];
                indiceVerticeMasCercano = j;
            }
        }

        // Imprimir resultados con alineación mejorada
        std::cout << std::setw(10) << i + 1 << " ("
                  << std::setw(6) << esquina.getX() << ", "
                  << std::setw(6) << esquina.getY() << ", "
                  << std::setw(6) << esquina.getZ() << ")    "
                  << std::setw(2) << indiceVerticeMasCercano << " ("
                  << std::setw(6) << verticeMasCercano.getX() << ", "
                  << std::setw(6) << verticeMasCercano.getY() << ", "
                  << std::setw(6) << verticeMasCercano.getZ() << ")    "
                  << std::setw(8) << minDistancia << "\n";
    }

    // Eliminar la caja envolvente si fue creada en este proceso
    std::stringstream ss;
    std::streambuf* coutBuf = std::cout.rdbuf(); 
    std::cout.rdbuf(ss.rdbuf());
    if (cajaCreada) {
        descargarObjeto(nombreCaja);
    }
    std::cout.rdbuf(coutBuf); 
}
