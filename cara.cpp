#include "cara.h"
#include <cmath>
#include <cstdlib>
// Constructor por defecto
Cara::Cara() : vertices() {}

// Constructor con parámetros
Cara::Cara(const std::vector<Vertice>& vertices) : vertices(vertices) {}

// Getter para obtener los vértices
const std::vector<Vertice>& Cara::getVertices() const {
    return vertices;
}

// Getter para obtener el número de vértices
int Cara::getNumeroVertices() const {
    return vertices.size();
}

// Setter para establecer los vértices
void Cara::setVertices(const std::vector<Vertice>& vertices) {
    this->vertices = vertices;
}

// Implementación del método calcularArea
double Cara::calcularArea() const {
    int numVertices = vertices.size();
    if (numVertices < 3) {
        // Un polígono debe tener al menos 3 vértices
        exit(-1);
    }

    double area = 0.0;

    // Fórmula de Gauss para el área del polígono
    for (int i = 0; i < numVertices; ++i) {
        int j = (i + 1) % numVertices;
        area += vertices[i].getX() * vertices[j].getY();
        area -= vertices[j].getX() * vertices[i].getY();
    }

    return std::fabs(area) / 2.0;
}

// Implementación del método agregarVertice
void Cara::agregarVertice(const Vertice& vertice) {
    vertices.push_back(vertice);
}