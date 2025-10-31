#ifndef LISTASENSOR_H
#define LISTASENSOR_H

#include <iostream>

template <typename T>
struct Nodo {
    T dato;              ///< Valor almacenado en el nodo
    Nodo<T>* siguiente;  ///< Puntero al siguiente nodo en la lista

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;  ///< Puntero al primer nodo de la lista
    int tamanio;      ///< Contador de elementos en la lista

public:
    ListaSensor() : cabeza(nullptr), tamanio(0) {
        std::cout << "Lista generica creada" << std::endl;
    }

    ~ListaSensor() {
        std::cout << "Liberando lista con " << tamanio << " nodos..." << std::endl;
        liberarMemoria();
    }

    void insertar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        
        if (cabeza == nullptr) {
            // Lista vacia: nuevo nodo es la cabeza
            cabeza = nuevoNodo;
        } else {
            // Recorrer hasta el ultimo nodo
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
        
        tamanio++;
        std::cout << "Dato agregado: " << valor << " (Total: " << tamanio << ")" << std::endl;
    }

    int obtenerTamanio() const {
        return tamanio;
    }

    void mostrar() const {
        if (cabeza == nullptr) {
            std::cout << "  [Lista vacia]" << std::endl;
            return;
        }

        Nodo<T>* actual = cabeza;
        std::cout << "  Lecturas [" << tamanio << "]: ";
        while (actual != nullptr) {
            std::cout << actual->dato;
            if (actual->siguiente != nullptr) {
                std::cout << " -> ";
            }
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }

    T calcularPromedio() const {
        if (cabeza == nullptr || tamanio == 0) {
            return static_cast<T>(0);
        }

        T suma = 0;
        Nodo<T>* actual = cabeza;
        
        while (actual != nullptr) {
            suma += actual->dato;
            actual = actual->siguiente;
        }

        return suma / tamanio;
    }

    T obtenerMaximo() const {
        if (cabeza == nullptr) {
            return static_cast<T>(0);
        }

        T maximo = cabeza->dato;
        Nodo<T>* actual = cabeza->siguiente;

        while (actual != nullptr) {
            if (actual->dato > maximo) {
                maximo = actual->dato;
            }
            actual = actual->siguiente;
        }

        return maximo;
    }

    T obtenerMinimo() const {
        if (cabeza == nullptr) {
            return static_cast<T>(0);
        }

        T minimo = cabeza->dato;
        Nodo<T>* actual = cabeza->siguiente;

        while (actual != nullptr) {
            if (actual->dato < minimo) {
                minimo = actual->dato;
            }
            actual = actual->siguiente;
        }

        return minimo;
    }

    void liberarMemoria() {
        Nodo<T>* actual = cabeza;
        
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        
        cabeza = nullptr;
        tamanio = 0;
    }

    Nodo<T>* getCabeza() const {
        return cabeza;
    }
};

#endif // LISTASENSOR_H
