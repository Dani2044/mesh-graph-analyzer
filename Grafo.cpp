#include "Grafo.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>

// Constructor que crea la matriz de adyacencia a partir de un objeto
Grafo::Grafo(Objeto& objeto) {
    vertices = objeto.getVertices();
    int n = vertices.size();

    matrizAdyacencia.assign(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    const std::vector<Cara>& caras = objeto.getCaras();
    for (const Cara& cara : caras) {
        const std::vector<Vertice>& verticesCara = cara.getVertices();
        int numVertices = verticesCara.size();

        for (int i = 0; i < numVertices; ++i) {
            int indiceOrigen = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), verticesCara[i]));
            int indiceDestino = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), verticesCara[(i + 1) % numVertices]));

            if (!aristaExiste(indiceOrigen, indiceDestino)) {
                double costo = verticesCara[i].distancia(verticesCara[(i + 1) % numVertices]);
                matrizAdyacencia[indiceOrigen][indiceDestino] = costo;
                matrizAdyacencia[indiceDestino][indiceOrigen] = costo;
            }
        }
    }
}

// Getter: Obtiene el número de vértices
int Grafo::getNumVertices() const {
    return vertices.size();
}

// Getter: Obtiene la lista de vértices
const std::vector<Vertice>& Grafo::getVertices() const {
    return vertices;
}

// Getter: Obtiene la matriz de adyacencia
const std::vector<std::vector<double>>& Grafo::getMatrizAdyacencia() const {
    return matrizAdyacencia;
}

// Método para verificar si un vértice ya existe en el grafo
bool Grafo::verticeExiste(const Vertice& vertice) const {
    return std::find(vertices.begin(), vertices.end(), vertice) != vertices.end();
}

// Método para verificar si una arista ya existe en el grafo
bool Grafo::aristaExiste(int origen, int destino) const {
    return origen >= 0 && destino >= 0 &&
           origen < matrizAdyacencia.size() && destino < matrizAdyacencia.size() &&
           matrizAdyacencia[origen][destino] != std::numeric_limits<double>::infinity();
}

// Método para agregar un vértice al grafo
void Grafo::agregarVertice(const Vertice& vertice) {
    if (!verticeExiste(vertice)) {
        vertices.push_back(vertice);
        int tam = vertices.size();
        matrizAdyacencia.resize(tam);
        for (int i = 0; i < tam; ++i) {
            matrizAdyacencia[i].resize(tam, std::numeric_limits<double>::infinity());
        }
    }
}

// Método para agregar una arista entre dos vértices
void Grafo::agregarArista(int origen, int destino) {
    double distancia = vertices[origen].distancia(vertices[destino]);
    matrizAdyacencia[origen][destino] = distancia;
    matrizAdyacencia[destino][origen] = distancia;
}


// Implementación del algoritmo de Dijkstra
std::vector<double> Grafo::Dijsktra(int origen) {
    int n = vertices.size();
    std::vector<double> distancias(n, std::numeric_limits<double>::infinity());
    std::vector<bool> visitado(n, false);
    distancias[origen] = 0.0;

    for (int i = 0; i < n; ++i) {
        int nodoActual = -1;
        double distanciaMinima = std::numeric_limits<double>::infinity();

        for (int j = 0; j < n; ++j) {
            if (!visitado[j] && distancias[j] < distanciaMinima) {
                distanciaMinima = distancias[j];
                nodoActual = j;
            }
        }

        if (nodoActual == -1) break;
        visitado[nodoActual] = true;

        for (int vecino = 0; vecino < n; ++vecino) {
            if (matrizAdyacencia[nodoActual][vecino] != std::numeric_limits<double>::infinity() && !visitado[vecino]) {
                double nuevaDistancia = distancias[nodoActual] + matrizAdyacencia[nodoActual][vecino];
                if (nuevaDistancia < distancias[vecino]) {
                    distancias[vecino] = nuevaDistancia;
                }
            }
        }
    }

    return distancias;
}

// Método para calcular el centroide del grafo
Vertice Grafo::calcularCentro() {
    double sumaX = 0.0;
    double sumaY = 0.0;
    double sumaZ = 0.0;
    int n = vertices.size();

    // Calcular las sumas de las coordenadas
    for (const Vertice& vertice : vertices) {
        sumaX += vertice.getX();
        sumaY += vertice.getY();
        sumaZ += vertice.getZ();
    }

    // Calcular el promedio de las coordenadas
    double centroideX = sumaX / n;
    double centroideY = sumaY / n;
    double centroideZ = sumaZ / n;

    // Crear y retornar el vértice centroide
    return Vertice(centroideX, centroideY, centroideZ);
}


// Método para obtener la ruta
std::vector<int> Grafo::obtenerRuta(int origen, int destino) {
    int n = vertices.size();
    std::vector<double> distancias(n, std::numeric_limits<double>::infinity());
    std::vector<int> predecesor(n, -1);
    std::vector<bool> visitado(n, false);
    distancias[origen] = 0.0;

    for (int i = 0; i < n; ++i) {
        int nodoActual = -1;
        double distanciaMinima = std::numeric_limits<double>::infinity();

        for (int j = 0; j < n; ++j) {
            if (!visitado[j] && distancias[j] < distanciaMinima) {
                distanciaMinima = distancias[j];
                nodoActual = j;
            }
        }

        if (nodoActual == -1) break;
        visitado[nodoActual] = true;

        for (int vecino = 0; vecino < n; ++vecino) {
            if (matrizAdyacencia[nodoActual][vecino] != std::numeric_limits<double>::infinity() && !visitado[vecino]) {
                double nuevaDistancia = distancias[nodoActual] + matrizAdyacencia[nodoActual][vecino];
                if (nuevaDistancia < distancias[vecino]) {
                    distancias[vecino] = nuevaDistancia;
                    predecesor[vecino] = nodoActual;
                }
            }
        }
    }

    std::vector<int> ruta;
    for (int at = destino; at != -1; at = predecesor[at]) {
        ruta.push_back(at);
    }
    std::reverse(ruta.begin(), ruta.end());

    if (ruta.front() != origen) {
        ruta.clear();
    }

    return ruta;
}

// Método para obtener el índice de un vértice
int Grafo::obtenerIndiceVertice(const Vertice& vertice) const {
    auto it = std::find(vertices.begin(), vertices.end(), vertice);
    if (it != vertices.end()) {
        return std::distance(vertices.begin(), it);
    }
    return -1;
}
