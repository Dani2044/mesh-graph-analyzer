#ifndef __OBJETO_H__
#define __OBJETO_H__

#include <string>
#include <vector>
#include <limits>
#include "vertice.h"
#include "cara.h"

class Objeto {
private:
    std::string nombre;
    std::vector<Cara> caras;
    std::vector<Vertice> verticesTotales;

public:
    // Constructor por defecto
    Objeto();

    // Constructor con nombre
    Objeto(const std::string& nombre);

    // Constructor con parámetros
    Objeto(const std::string& nombre, const std::vector<Vertice>& vertices, const std::vector<Cara>& caras);

    // Getters
    std::string getNombre() const;
    const std::vector<Cara>& getCaras() const;
    const std::vector<Vertice>& getVertices() const;

    // Setters
    void setNombre(const std::string& nombre);
    void setCaras(const std::vector<Cara>& caras);
    void setVertices(const std::vector<Vertice>& vertices);

    // Métodos para agregar elementos
    void agregarCara(const Cara& cara);

    // Métodos de manipulación
    void calcularCajaEnvolvente(double& xmin, double& ymin, double& zmin, double& xmax, double& ymax, double& zmax) const;

    // Método para obtener una representación en cadena
    std::string toString() const;
};

#endif // __OBJETO_H__