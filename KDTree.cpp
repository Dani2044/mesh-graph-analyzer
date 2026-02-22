#include "KDTree.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include <stdexcept>

// Constructor
KDTree::KDTree() : raiz(nullptr) {}

// Destructor
KDTree::~KDTree() {
    delete raiz;
}

// Función para insertar un punto en el árbol de forma recursiva
NodoKDTree* KDTree::insertarRecursivo(NodoKDTree* nodo, const Vertice& punto, const std::string& nombreObjeto, int profundidad) {
    if (nodo == nullptr) {
        return new NodoKDTree(punto, nombreObjeto, profundidad);
    }

    if (nodo->getPunto() == punto) {
        nodo->agregarNombreObjeto(nombreObjeto);
        return nodo;
    }

    int eje = profundidad % 3;
    if ((eje == 0 && punto.getX() < nodo->getPunto().getX()) ||
        (eje == 1 && punto.getY() < nodo->getPunto().getY()) ||
        (eje == 2 && punto.getZ() < nodo->getPunto().getZ())) {
        nodo->setIzquierda(insertarRecursivo(nodo->getIzquierda(), punto, nombreObjeto, profundidad + 1));
    } else {
        nodo->setDerecha(insertarRecursivo(nodo->getDerecha(), punto, nombreObjeto, profundidad + 1));
    }

    return nodo;
}

// Insertar un nuevo punto en el árbol
void KDTree::insertar(const Vertice& punto, const std::string& nombreObjeto) {
    raiz = insertarRecursivo(raiz, punto, nombreObjeto, 0);
}

// Método para encontrar el nodo más cercano de forma recursiva
NodoKDTree* KDTree::encontrarMasCercanoRecursivo(NodoKDTree* nodo, const Vertice& punto, int profundidad, NodoKDTree* mejorNodo) {
    if (nodo == nullptr) {
        return mejorNodo;
    }

    double distanciaActual = nodo->getPunto().distancia(punto);
    double distanciaMejor = mejorNodo->getPunto().distancia(punto);

    if (distanciaActual < distanciaMejor) {
        mejorNodo = nodo;
    }

    int eje = profundidad % 3;
    NodoKDTree* proximoNodo = nullptr;
    NodoKDTree* otroNodo = nullptr;

    if ((eje == 0 && punto.getX() < nodo->getPunto().getX()) ||
        (eje == 1 && punto.getY() < nodo->getPunto().getY()) ||
        (eje == 2 && punto.getZ() < nodo->getPunto().getZ())) {
        proximoNodo = nodo->getIzquierda();
        otroNodo = nodo->getDerecha();
    } else {
        proximoNodo = nodo->getDerecha();
        otroNodo = nodo->getIzquierda();
    }

    mejorNodo = encontrarMasCercanoRecursivo(proximoNodo, punto, profundidad + 1, mejorNodo);

    double distanciaEje = (eje == 0 ? std::abs(punto.getX() - nodo->getPunto().getX()) :
                          (eje == 1 ? std::abs(punto.getY() - nodo->getPunto().getY()) :
                                      std::abs(punto.getZ() - nodo->getPunto().getZ())));
    if (distanciaEje < mejorNodo->getPunto().distancia(punto)) {
        mejorNodo = encontrarMasCercanoRecursivo(otroNodo, punto, profundidad + 1, mejorNodo);
    }

    return mejorNodo;
}

// Encontrar el nodo más cercano a un punto dado
NodoKDTree* KDTree::encontrarMasCercano(const Vertice& punto) {
    if (raiz == nullptr) {
        throw std::runtime_error("El árbol está vacío.");
    }
    return encontrarMasCercanoRecursivo(raiz, punto, 0, raiz);
}

