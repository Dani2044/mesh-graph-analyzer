#ifndef __CARA_H__
#define __CARA_H__

#include <vector>
#include "vertice.h"

class Cara {
private:
    std::vector<Vertice> vertices;  // Almacena directamente objetos Vertice

public:
    // Constructor por defecto
    Cara();

    // Constructor con parámetros
    Cara(const std::vector<Vertice>& vertices);

    // Getters
    const std::vector<Vertice>& getVertices() const; // Cambiado a referencia constante
    int getNumeroVertices() const;

    // Setters
    void setVertices(const std::vector<Vertice>& vertices);

    // Método para calcular el área de la cara
    double calcularArea() const;

    // Método para agregar un vértice
    void agregarVertice(const Vertice& vertice);
};

#endif // __CARA_H__