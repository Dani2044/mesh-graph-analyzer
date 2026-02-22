#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <vector>
#include "vertice.h"
#include "objeto.h"

class Grafo {
private:
    std::vector<Vertice> vertices; // Lista de vértices únicos
    std::vector<std::vector<double>> matrizAdyacencia; // Matriz de adyacencia con costos

public:
  // Constructor que crea el grafo a partir de un objeto
  Grafo(Objeto& objeto);

  // Getters
  int getNumVertices() const; // Obtiene el número de vértices
  const std::vector<Vertice>& getVertices() const; // Obtiene la lista de vértices
  const std::vector<std::vector<double>>& getMatrizAdyacencia() const; // Obtiene la matriz de adyacencia
    
  // Métodos para gestionar vértices y aristas
  void agregarVertice(const Vertice& vertice); // Agrega un vértice al grafo
  void agregarArista(int origen, int destino); // Agrega una arista al grafo

  // Métodos de búsqueda y cálculo
  bool verticeExiste(const Vertice& vertice) const;  // Verifica si un vértice ya existe
  bool aristaExiste(int origen, int destino) const; // Verifica si una arista ya existe
  int obtenerIndiceVertice(const Vertice& vertice) const; // Obtiene el índice de un vértice

  // Algoritmos de ruta más corta
  std::vector<double> Dijsktra(int origen); // Calcula las distancias mínimas desde un origen
  std::vector<int> obtenerRuta(int origen, int destino); // Reconstruye la ruta más corta

  // Cálculo del centro del grafo
  Vertice calcularCentro(); // Calcula el centroide del grafo
};

#endif // __GRAFO_H__
