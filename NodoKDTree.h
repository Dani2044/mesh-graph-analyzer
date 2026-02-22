#ifndef __NODO_KD_TREE_H__
#define __NODO_KD_TREE_H__

#include "vertice.h"
#include <vector>
#include <string>

class NodoKDTree {
private:
    Vertice punto; 
    std::vector<std::string> nombresObjetos; 
    NodoKDTree* izquierda; 
    NodoKDTree* derecha;   
    int profundidad;       

public:
    // Constructor por defecto
    NodoKDTree();

    // Constructor con parámetros
    NodoKDTree(const Vertice& punto, const std::string& nombreObjeto, int profundidad);

    // Getters y setters
    Vertice getPunto() const;
    void setPunto(const Vertice& punto); 
    NodoKDTree* getIzquierda() const;
    void setIzquierda(NodoKDTree* nodo);
    NodoKDTree* getDerecha() const;
    void setDerecha(NodoKDTree* nodo); 
    int getProfundidad() const;
    void setProfundidad(int profundidad);

    // Obtener los nombres de los objetos
    std::vector<std::string> getNombresObjetos() const;

    // Agregar un nuevo nombre de objeto al nodo
    void agregarNombreObjeto(const std::string& nombreObjeto);

    // Sobrecarga del operador ==
    bool operator==(const NodoKDTree& otro) const;

    // Destruir el nodo (destructor)
    ~NodoKDTree();
};

#endif // __NODO_KD_TREE_H__

