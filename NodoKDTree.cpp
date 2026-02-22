#include "NodoKDTree.h"
#include <algorithm>

// Constructor por defecto
NodoKDTree::NodoKDTree() : izquierda(nullptr), derecha(nullptr), profundidad(0) {}

// Constructor con parámetros
NodoKDTree::NodoKDTree(const Vertice& punto, const std::string& nombreObjeto, int profundidad) 
    : punto(punto), izquierda(nullptr), derecha(nullptr), profundidad(profundidad) {
    nombresObjetos.push_back(nombreObjeto);
}

// Getters
Vertice NodoKDTree::getPunto() const {
    return punto;
}

NodoKDTree* NodoKDTree::getIzquierda() const {
    return izquierda;
}

NodoKDTree* NodoKDTree::getDerecha() const {
    return derecha;
}

int NodoKDTree::getProfundidad() const {
    return profundidad;
}

std::vector<std::string> NodoKDTree::getNombresObjetos() const {
    return nombresObjetos;
}

// Setters
void NodoKDTree::setPunto(const Vertice& punto) {
    this->punto = punto;
}

void NodoKDTree::setIzquierda(NodoKDTree* nodo) {
    izquierda = nodo;
}

void NodoKDTree::setDerecha(NodoKDTree* nodo) {
    derecha = nodo;
}

void NodoKDTree::setProfundidad(int profundidad) {
    this->profundidad = profundidad;
}

// Agregar un nuevo nombre de objeto
void NodoKDTree::agregarNombreObjeto(const std::string& nombreObjeto) {
    if (std::find(nombresObjetos.begin(), nombresObjetos.end(), nombreObjeto) == nombresObjetos.end()) {
        nombresObjetos.push_back(nombreObjeto);
    }
}

// Sobrecarga del operador ==
bool NodoKDTree::operator==(const NodoKDTree& otro) const {
    return (punto == otro.getPunto()); 
}

// Destructor
NodoKDTree::~NodoKDTree() {
    delete izquierda;
    delete derecha;
}

