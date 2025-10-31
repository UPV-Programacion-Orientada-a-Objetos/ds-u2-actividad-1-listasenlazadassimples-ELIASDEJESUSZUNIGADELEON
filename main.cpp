#include <iostream>
#include <cstdlib>
#include "SensorBase.h"
#include "ListaSensor.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaGestion.h"

// Lista global polimorfica para gestionar todos los sensores
ListaGestion* listaGlobal = nullptr;

void mostrarMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Sistema IoT - Monitoreo Polimorfico  " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Crear Sensor de Temperatura" << std::endl;
    std::cout << "2. Crear Sensor de Presion" << std::endl;
    std::cout << "3. Registrar Lectura Manual" << std::endl;
    std::cout << "4. Leer Datos desde ESP32 (Serial)" << std::endl;
    std::cout << "5. Procesar Todos los Sensores" << std::endl;
    std::cout << "6. Mostrar Estado de Sensores" << std::endl;
    std::cout << "7. Salir y Liberar Memoria" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Opcion: ";
}

void crearSensorTemperatura() {
    std::cout << "\n[LOG] Creando Sensor de Temperatura..." << std::endl;
    
    std::cout << "Ingresa el ID del sensor (ej: T-001): ";
    char id[50];
    std::cin >> id;
    
    // Verificar si ya existe
    if (listaGlobal->buscarSensor(id) != nullptr) {
        std::cout << "[WARNING] Ya existe un sensor con ese ID" << std::endl;
        return;
    }
    
    SensorTemperatura* nuevoSensor = new SensorTemperatura(id);
    listaGlobal->agregarSensor(nuevoSensor);
    std::cout << "[SUCCESS] Sensor de temperatura creado exitosamente" << std::endl;
}

void crearSensorPresion() {
    std::cout << "\n[LOG] Creando Sensor de Presion..." << std::endl;
    
    std::cout << "Ingresa el ID del sensor (ej: P-105): ";
    char id[50];
    std::cin >> id;
    
    // Verificar si ya existe
    if (listaGlobal->buscarSensor(id) != nullptr) {
        std::cout << "[WARNING] Ya existe un sensor con ese ID" << std::endl;
        return;
    }
    
    SensorPresion* nuevoSensor = new SensorPresion(id);
    listaGlobal->agregarSensor(nuevoSensor);
    std::cout << "[SUCCESS] Sensor de presion creado exitosamente" << std::endl;
}

void registrarLectura() {
    std::cout << "\n[LOG] Registrando Lectura Manual..." << std::endl;
    
    if (listaGlobal->getCantidad() == 0) {
        std::cout << "[ERROR] No hay sensores creados. Crea uno primero." << std::endl;
        return;
    }
    
    std::cout << "Ingresa el ID del sensor: ";
    char id[50];
    std::cin >> id;
    
    SensorBase* sensor = listaGlobal->buscarSensor(id);
    
    if (sensor == nullptr) {
        std::cout << "[ERROR] Sensor no encontrado: " << id << std::endl;
        return;
    }
    
    // Determinar tipo y agregar lectura (downcasting seguro)
    SensorTemperatura* sensorTemp = dynamic_cast<SensorTemperatura*>(sensor);
    if (sensorTemp != nullptr) {
        float valor;
        std::cout << "Ingresa temperatura (C): ";
        std::cin >> valor;
        sensorTemp->agregarLectura(valor);
        return;
    }
    
    SensorPresion* sensorPres = dynamic_cast<SensorPresion*>(sensor);
    if (sensorPres != nullptr) {
        int valor;
        std::cout << "Ingresa presion (hPa): ";
        std::cin >> valor;
        sensorPres->agregarLectura(valor);
        return;
    }
}

void leerDesdeSerial() {
    std::cout << "\n[LOG] Leyendo datos desde ESP32..." << std::endl;
    std::cout << "[PENDING] Implementar lectura serial" << std::endl;
    //Abrir puerto COM9 (o el que corresponda)
    //Parsear lineas: "T-001,23.5" o "P-105,1013"
    //Crear sensores dinamicamente o agregar lecturas
}

void procesarSensores() {
    std::cout << "\n[LOG] Procesando todos los sensores polimorficamente..." << std::endl;
    listaGlobal->procesarTodos();
}

void mostrarEstado() {
    std::cout << "\n[LOG] Estado actual del sistema:" << std::endl;
    listaGlobal->mostrarTodos();
}

void liberarMemoria() {
    std::cout << "\n[LOG] Liberando toda la memoria del sistema..." << std::endl;
    
    if (listaGlobal != nullptr) {
        delete listaGlobal;
        listaGlobal = nullptr;
    }
    
    std::cout << "[SUCCESS] Memoria liberada correctamente" << std::endl;
}

void pruebaListaGenerica() {
    std::cout << "\n=== PRUEBA: Lista Generica con Float ===" << std::endl;
    ListaSensor<float> listaTemp;
    listaTemp.insertar(23.5f);
    listaTemp.insertar(24.1f);
    listaTemp.insertar(22.8f);
    listaTemp.mostrar();
    std::cout << "Promedio: " << listaTemp.calcularPromedio() << std::endl;
    std::cout << "Maximo: " << listaTemp.obtenerMaximo() << std::endl;
    std::cout << "Minimo: " << listaTemp.obtenerMinimo() << std::endl;
    
    std::cout << "\n=== PRUEBA: Lista Generica con Int ===" << std::endl;
    ListaSensor<int> listaPresion;
    listaPresion.insertar(1013);
    listaPresion.insertar(1015);
    listaPresion.insertar(1012);
    listaPresion.mostrar();
    std::cout << "Promedio: " << listaPresion.calcularPromedio() << std::endl;
    std::cout << "Maximo: " << listaPresion.obtenerMaximo() << std::endl;
    std::cout << "Minimo: " << listaPresion.obtenerMinimo() << std::endl;
}

int main() {
    int opcion = 0;
    bool continuar = true;
    
    std::cout << "\n=== Sistema IoT de Monitoreo Polimorfico ===" << std::endl;
    std::cout << "Version 5 - ListaGestion Polimorfica Implementada" << std::endl;
    std::cout << "Sistema inicializado correctamente." << std::endl;
    
    // Inicializar lista de gestion polimorfica
    listaGlobal = new ListaGestion();
    
    // Prueba inicial comentada (ya funcionan las clases)
    // pruebaListaGenerica();
    // std::cout << "\n--- Presiona ENTER para continuar al menu ---" << std::endl;
    // std::cin.get();

    //Crear instancia de ListaGestion (lista de SensorBase*)
    
    while (continuar) {
        mostrarMenu();
        std::cin >> opcion;
        
        // Limpiar buffer de entrada
        std::cin.ignore(1000, '\n');
        
        switch (opcion) {
            case 1:
                crearSensorTemperatura();
                break;
            case 2:
                crearSensorPresion();
                break;
            case 3:
                registrarLectura();
                break;
            case 4:
                leerDesdeSerial();
                break;
            case 5:
                procesarSensores();
                break;
            case 6:
                mostrarEstado();
                break;
            case 7:
                std::cout << "\n[SYSTEM] Cerrando sistema..." << std::endl;
                liberarMemoria();
                continuar = false;
                break;
            default:
                std::cout << "\n[ERROR] Opcion invalida. Intente de nuevo." << std::endl;
        }
    }

    std::cout << "\n[SYSTEM] Sistema cerrado correctamente." << std::endl;
    return 0;
}
