#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include "NodoKDTree.h"

class KDTree {
private:
    NodoKDTree* raiz;

    // Método auxiliar para insertar un nodo en el árbol
    NodoKDTree* insertarRecursivo(NodoKDTree* nodo, const Vertice& punto, const std::string& nombreObjeto, int profundidad);

    // Método auxiliar para encontrar el nodo más cercano
    NodoKDTree* encontrarMasCercanoRecursivo(NodoKDTree* nodo, const Vertice& punto, int profundidad, NodoKDTree* mejorNodo);

public:
    // Constructor
    KDTree();

    // Insertar un nuevo punto en el árbol
    void insertar(const Vertice& punto, const std::string& nombreObjeto);

    // Encontrar el nodo más cercano a un punto dado
    NodoKDTree* encontrarMasCercano(const Vertice& punto);

    // Destructor
    ~KDTree();
};

#endif // __KD_TREE_H__

