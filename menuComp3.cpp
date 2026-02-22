#include "menu.h"
#include "Grafo.h"
#include <iostream>
#include <vector>
#include <limits>

// Encuentra la ruta más corta entre dos vértices de un objeto
void Menu::rutaCortaVertices(int v1, int v2, std::string& nombre_objeto) {
    if (!objetoCargado(nombre_objeto)) {
        std::cout << "El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }

    Objeto obj;

    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombre_objeto)
            obj = objetosCargados[i];
    }

    Grafo grafo = Grafo(obj);
    int numVertices = grafo.getNumVertices();

    if (v1 == v2) {
        std::cout << "Los índices de los vértices dados son iguales." << std::endl;
        return;
    }

    if (v1 < 0 || v2 < 0 || v1 >= numVertices || v2 >= numVertices) {
        std::cout << "Algunos de los índices de vértices están fuera de rango para el objeto " << nombre_objeto << "." << std::endl;
        return;
    }

    std::vector<double> distancias = grafo.Dijsktra(v1);
    std::vector<int> ruta = grafo.obtenerRuta(v1, v2);

    if (distancias[v2] == std::numeric_limits<double>::infinity()) {
        std::cout << "No existe una ruta que conecte los vértices " << v1 << " y " << v2 << "." << std::endl;
        return;
    }

    std::cout << "La ruta más corta que conecta los vértices " << v1 << " y " << v2 << " del objeto " << nombre_objeto
              << " pasa por: ";
    for (size_t i = 0; i < ruta.size(); ++i) {
        std::cout << ruta[i];
        if (i < ruta.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "; con una longitud de " << distancias[v2] << "." << std::endl;
}

// Método para calcular la ruta más corta entre un vértice y el centro del objeto
void Menu::rutaCortaCentro(int v1, std::string& nombre_objeto) {
    if (!objetoCargado(nombre_objeto)) {
        std::cout << "El objeto " << nombre_objeto << " no ha sido cargado en memoria." << std::endl;
        return;
    }
    Objeto obj;
    for (int i = 0; i < objetosCargados.size(); ++i) {
        if (objetosCargados[i].getNombre() == nombre_objeto) {
            obj = objetosCargados[i];
            break;
        }
    }
    Grafo grafo = Grafo(obj);
    int numVertices = grafo.getNumVertices();
    if (v1 < 0 || v1 >= numVertices) {
        std::cout << "El índice del vértice está fuera de rango para el objeto " << nombre_objeto << "." << std::endl;
        return;
    }

    Vertice centroide = grafo.calcularCentro();
    std::vector<double> distancias = grafo.Dijsktra(v1);
    int indiceCercano = -1;
    double distanciaMinima = std::numeric_limits<double>::max();
    for (int i = 0; i < grafo.getNumVertices(); ++i) {
        double distancia = centroide.distancia(grafo.getVertices()[i]);
        if (distancia < distanciaMinima) {
            distanciaMinima = distancia;
            indiceCercano = i;
        }
    }
    if (indiceCercano == -1) {
        std::cout << "No se pudo encontrar un vértice cercano al centroide." << std::endl;
        return;
    }

    std::vector<int> ruta = grafo.obtenerRuta(v1, indiceCercano);

    double distanciaTotal = 0.0;
    for (int i = 0; i < ruta.size() - 1; ++i) {
        distanciaTotal += grafo.getVertices()[ruta[i]].distancia(grafo.getVertices()[ruta[i+1]]);
    }
    
    distanciaTotal += grafo.getVertices()[ruta[ruta.size() - 1]].distancia(centroide);

    std::cout << "La ruta más corta que conecta el vértice " << v1 << " con el centro del objeto " << nombre_objeto
              << ", ubicado en (" << centroide.getX() << ", " << centroide.getY() << ", " << centroide.getZ() << "), pasa por: ";
    for (int i = 0; i < ruta.size(); ++i) {
        std::cout << ruta[i];
        if (i < ruta.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ", centroide; con una longitud de " << distanciaTotal << "." << std::endl;
}