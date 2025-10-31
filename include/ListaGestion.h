#ifndef LISTAGESTION_H
#define LISTAGESTION_H

#include "SensorBase.h"
#include <iostream>
#include <cstring>

struct NodoGestion {
    SensorBase* sensor;        ///< Puntero polimorfico al sensor
    NodoGestion* siguiente;    ///< Puntero al siguiente nodo
    NodoGestion(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

class ListaGestion {
private:
    NodoGestion* cabeza;  ///< Primer nodo de la lista
    int cantidad;         ///< Numero de sensores en la lista

public:

    ListaGestion() : cabeza(nullptr), cantidad(0) {
        std::cout << "[CONSTRUCTOR] ListaGestion creada" << std::endl;
    }

    ~ListaGestion() {
        std::cout << "[DESTRUCTOR] Liberando ListaGestion con " << cantidad << " sensores..." << std::endl;
        liberarTodo();
    }

    void agregarSensor(SensorBase* sensor) {
        NodoGestion* nuevoNodo = new NodoGestion(sensor);
        
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            NodoGestion* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
        
        cantidad++;
        std::cout << "[LISTA] Sensor agregado: " << sensor->getNombre() 
                  << " (Total: " << cantidad << ")" << std::endl;
    }

    SensorBase* buscarSensor(const char* nombre) {
        NodoGestion* actual = cabeza;
        
        while (actual != nullptr) {
            if (strcmp(actual->sensor->getNombre(), nombre) == 0) {
                return actual->sensor;
            }
            actual = actual->siguiente;
        }
        
        return nullptr;
    }

    void procesarTodos() {
        if (cabeza == nullptr) {
            std::cout << "[WARNING] No hay sensores para procesar" << std::endl;
            return;
        }

        std::cout << "\n[PROCESAMIENTO POLIMORFICO] Iniciando..." << std::endl;
        std::cout << "============================================" << std::endl;
        
        NodoGestion* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->procesarLectura();  // Llamada polimorfica
            actual = actual->siguiente;
        }
        
        std::cout << "============================================" << std::endl;
    }

    void mostrarTodos() const {
        if (cabeza == nullptr) {
            std::cout << "[WARNING] No hay sensores registrados" << std::endl;
            return;
        }

        std::cout << "\n========== ESTADO DE TODOS LOS SENSORES ==========" << std::endl;
        std::cout << "Total de sensores: " << cantidad << std::endl;
        
        NodoGestion* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->mostrarEstado();  // Llamada polimorfica
            actual = actual->siguiente;
        }
        
        std::cout << "==================================================\n" << std::endl;
    }

    int getCantidad() const {
        return cantidad;
    }

    void liberarTodo() {
        NodoGestion* actual = cabeza;
        
        while (actual != nullptr) {
            NodoGestion* siguiente = actual->siguiente;
            delete actual->sensor;  // Llama al destructor virtual
            delete actual;          // Libera el nodo
            actual = siguiente;
        }
        
        cabeza = nullptr;
        cantidad = 0;
    }
};

#endif // LISTAGESTION_H
