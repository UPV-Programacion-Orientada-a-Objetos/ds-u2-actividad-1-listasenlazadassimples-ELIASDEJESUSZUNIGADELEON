#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <iostream>
#include <cstring>

class SensorBase {
protected:
    char nombre[50];  ///< Identificador unico del sensor (ej: "T-001", "P-105")

public:
    SensorBase(const char* nombreSensor) {
        strncpy(nombre, nombreSensor, 49);
        nombre[49] = '\0';  // Asegurar terminacion nula
    }

    virtual ~SensorBase() {
        std::cout << "Liberando sensor: " << nombre << std::endl;
    }

    const char* getNombre() const {
        return nombre;
    }

    virtual void procesarLectura() = 0;

    virtual void mostrarEstado() const = 0;

    virtual int obtenerNumLecturas() const = 0;
};

#endif // SENSORBASE_H
