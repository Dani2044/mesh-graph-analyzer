#include "vertice.h"
#include <cmath>

// Constructor por defecto
Vertice::Vertice() : x(0), y(0), z(0) {}

// Constructor con parámetros
Vertice::Vertice(double x, double y, double z) : x(x), y(y), z(z) {}

// Getters
double Vertice::getX() const {
    return x;
}

double Vertice::getY() const {
    return y;
}

double Vertice::getZ() const {
    return z;
}

// Setters
void Vertice::setX(double x) {
    this->x = x;
}

void Vertice::setY(double y) {
    this->y = y;
}

void Vertice::setZ(double z) {
    this->z = z;
}

// Método para calcular la distancia al vértice dado
double Vertice::distancia(const Vertice& otro) const {
    double dx = otro.getX() - x;
    double dy = otro.getY() - y;
    double dz = otro.getZ() - z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Sobrecarga del operador ==
bool Vertice::operator==(const Vertice& otro) const {
    return (x == otro.x && y == otro.y && z == otro.z);
}