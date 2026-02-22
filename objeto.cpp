#include "objeto.h"
#include <sstream>
#include <limits>

// Constructor por defecto
Objeto::Objeto() {}

// Constructor con nombre
Objeto::Objeto(const std::string& nombre) : nombre(nombre) {}

// Constructor con parámetros
Objeto::Objeto(const std::string& nombre, const std::vector<Vertice>& vertices, const std::vector<Cara>& caras)
    : nombre(nombre), verticesTotales(vertices), caras(caras) {}

// Getters
std::string Objeto::getNombre() const {
    return nombre;
}

const std::vector<Cara>& Objeto::getCaras() const {
    return caras;
}

const std::vector<Vertice>& Objeto::getVertices() const {
    return verticesTotales;
}

// Setters
void Objeto::setNombre(const std::string& nombre) {
    this->nombre = nombre;
}

void Objeto::setCaras(const std::vector<Cara>& caras) {
    this->caras = caras;
}

void Objeto::setVertices(const std::vector<Vertice>& vertices) {
    this->verticesTotales = vertices;
}

// Métodos para agregar elementos
void Objeto::agregarCara(const Cara& cara) {
    caras.push_back(cara);
}

// Métodos de manipulación
void Objeto::calcularCajaEnvolvente(double& xmin, double& ymin, double& zmin, double& xmax, double& ymax, double& zmax) const {
    xmin = ymin = zmin = std::numeric_limits<double>::max();
    xmax = ymax = zmax = std::numeric_limits<double>::lowest();

    for (const Cara& cara : caras) {
        for (const Vertice& vertice : cara.getVertices()) {
            if (vertice.getX() < xmin) xmin = vertice.getX();
            if (vertice.getY() < ymin) ymin = vertice.getY();
            if (vertice.getZ() < zmin) zmin = vertice.getZ();
            if (vertice.getX() > xmax) xmax = vertice.getX();
            if (vertice.getY() > ymax) ymax = vertice.getY();
            if (vertice.getZ() > zmax) zmax = vertice.getZ();
        }
    }
}

// Método para obtener una representación en cadena
std::string Objeto::toString() const {
    std::ostringstream flujo;
    std::vector<Vertice> vertices_unicos;
    int num_aristas = 0;

    for (int i = 0; i < caras.size(); ++i) {
        const std::vector<Vertice>& vertices_cara = caras[i].getVertices();
        for (int j = 0; j < vertices_cara.size(); ++j) {
            bool vertice_unico = true;
            for (int k = 0; k < vertices_unicos.size(); ++k) {
                if (vertices_unicos[k] == vertices_cara[j]) {
                    vertice_unico = false;
                    break;
                }
            }

            if (vertice_unico) {
                vertices_unicos.push_back(vertices_cara[j]);
            }

            Vertice vertice_actual = vertices_cara[j];
            Vertice vertice_siguiente = vertices_cara[(j + 1) % vertices_cara.size()];
            bool arista_duplicada = false;

            for (int m = 0; m < i; ++m) {
                const std::vector<Vertice>& vertices_cara_anterior = caras[m].getVertices();
                for (int n = 0; n < vertices_cara_anterior.size(); ++n) {
                    Vertice v1_anterior = vertices_cara_anterior[n];
                    Vertice v2_anterior = vertices_cara_anterior[(n + 1) % vertices_cara_anterior.size()];

                    if ((vertice_actual == v1_anterior && vertice_siguiente == v2_anterior) ||
                        (vertice_actual == v2_anterior && vertice_siguiente == v1_anterior)) {
                        arista_duplicada = true;
                        break;
                    }
                }

                if (arista_duplicada) {
                    break;
                }
            }

            if (!arista_duplicada) {
                num_aristas++;
            }
        }
    }

    flujo << nombre << " contiene " << vertices_unicos.size() << " vertices, "
          << num_aristas << " aristas y " << caras.size() << " caras.";

    return flujo.str();
}