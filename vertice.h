#ifndef __VERTICE_H__
#define __VERTICE_H__

class Vertice {
private:
    double x, y, z; // Coordenadas del vértice

public:
    // Constructor por defecto
    Vertice();

    // Constructor con parámetros
    Vertice(double x, double y, double z);

    // Getters
    double getX() const;
    double getY() const;
    double getZ() const;

    // Setters
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    // Método para calcular la distancia al vértice dado
    double distancia(const Vertice& otro) const;

    // Sobrecarga del operador ==
    bool operator==(const Vertice& otro) const;
};

#endif // __VERTICE_H__

// eof - vertice.h