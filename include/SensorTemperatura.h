#ifndef SENSORTEMPERATURA_H
#define SENSORTEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <iomanip>

class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float>* lecturas;  ///< Lista enlazada de lecturas de temperatura

public:

    SensorTemperatura(const char* nombreSensor) : SensorBase(nombreSensor) {
        lecturas = new ListaSensor<float>();
        std::cout << "[CONSTRUCTOR] SensorTemperatura creado: " << nombre << std::endl;
    }

    ~SensorTemperatura() override {
        std::cout << "[DESTRUCTOR] Liberando SensorTemperatura: " << nombre << std::endl;
        delete lecturas;
    }

    void agregarLectura(float valor) {
        lecturas->insertar(valor);
        std::cout << "[SENSOR " << nombre << "] Temperatura registrada: " 
                  << std::fixed << std::setprecision(1) << valor << " C" << std::endl;
    }

    void procesarLectura() override {
        int numLecturas = lecturas->obtenerTamanio();
        
        if (numLecturas == 0) {
            std::cout << "[SENSOR " << nombre << "] No hay lecturas para procesar" << std::endl;
            return;
        }

        float promedio = lecturas->calcularPromedio();
        float maximo = lecturas->obtenerMaximo();
        float minimo = lecturas->obtenerMinimo();

        std::cout << "\n--- Procesamiento de " << nombre << " ---" << std::endl;
        std::cout << "Tipo: TEMPERATURA" << std::endl;
        std::cout << "Lecturas totales: " << numLecturas << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Promedio: " << promedio << " C" << std::endl;
        std::cout << "Maximo: " << maximo << " C" << std::endl;
        std::cout << "Minimo: " << minimo << " C" << std::endl;
        std::cout << "-----------------------------------\n" << std::endl;
    }

    void mostrarEstado() const override {
        std::cout << "\n[ESTADO] Sensor: " << nombre << std::endl;
        std::cout << "         Tipo: Temperatura (float)" << std::endl;
        std::cout << "         Lecturas almacenadas: " << lecturas->obtenerTamanio() << std::endl;
        
        if (lecturas->obtenerTamanio() > 0) {
            lecturas->mostrar();
        } else {
            std::cout << "         [Sin lecturas]" << std::endl;
        }
    }

    int obtenerNumLecturas() const override {
        return lecturas->obtenerTamanio();
    }
};

#endif // SENSORTEMPERATURA_H
