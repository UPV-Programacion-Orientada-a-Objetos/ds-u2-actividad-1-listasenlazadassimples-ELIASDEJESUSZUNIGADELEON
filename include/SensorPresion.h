#ifndef SENSORPRESION_H
#define SENSORPRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>

class SensorPresion : public SensorBase {
private:
    ListaSensor<int>* lecturas;  ///< Lista enlazada de lecturas de presion

public:

    SensorPresion(const char* nombreSensor) : SensorBase(nombreSensor) {
        lecturas = new ListaSensor<int>();
        std::cout << "SensorPresion creado: " << nombre << std::endl;
    }

    ~SensorPresion() override {
        std::cout << "Liberando SensorPresion: " << nombre << std::endl;
        delete lecturas;
    }

    void agregarLectura(int valor) {
        lecturas->insertar(valor);
        std::cout << "[SENSOR " << nombre << "] Presion registrada: " 
                  << valor << " hPa" << std::endl;
    }

    void procesarLectura() override {
        int numLecturas = lecturas->obtenerTamanio();
        
        if (numLecturas == 0) {
            std::cout << "[SENSOR " << nombre << "] No hay lecturas para procesar" << std::endl;
            return;
        }

        int promedio = lecturas->calcularPromedio();
        int maximo = lecturas->obtenerMaximo();
        int minimo = lecturas->obtenerMinimo();

        std::cout << "\n--- Procesamiento de " << nombre << " ---" << std::endl;
        std::cout << "Tipo: PRESION" << std::endl;
        std::cout << "Lecturas totales: " << numLecturas << std::endl;
        std::cout << "Promedio: " << promedio << " hPa" << std::endl;
        std::cout << "Maximo: " << maximo << " hPa" << std::endl;
        std::cout << "Minimo: " << minimo << " hPa" << std::endl;
        std::cout << "-----------------------------------\n" << std::endl;
    }

    void mostrarEstado() const override {
        std::cout << "\nSensor: " << nombre << std::endl;
        std::cout << "         Tipo: Presion (int)" << std::endl;
        std::cout << "         Lecturas almacenadas: " << lecturas->obtenerTamanio() << std::endl;
        
        if (lecturas->obtenerTamanio() > 0) {
            lecturas->mostrar();
        } else {
            std::cout << "         [Sin lecturas]" << std::endl;
        }
    }

    /**
     * @brief Obtiene el numero de lecturas almacenadas
     * @return int Cantidad de lecturas
     */
    int obtenerNumLecturas() const override {
        return lecturas->obtenerTamanio();
    }
};

#endif // SENSORPRESION_H
