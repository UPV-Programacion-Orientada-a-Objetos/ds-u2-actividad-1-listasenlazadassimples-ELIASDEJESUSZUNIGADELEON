#include <iostream>
#include <cstdlib>
#include "SensorBase.h"
#include "ListaSensor.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"

using namespace std;

// Variables globales para prueba (luego se usara lista de gestion)
SensorTemperatura* sensorTemp = nullptr;
SensorPresion* sensorPres = nullptr;

void mostrarMenu() {
    cout << "\n========================================" << endl;
    cout << "  Sistema IoT - Monitoreo Polimorfico  " << endl;
    cout << "========================================" << endl;
    cout << "1. Crear Sensor de Temperatura" << endl;
    cout << "2. Crear Sensor de Presion" << endl;
    cout << "3. Registrar Lectura Manual" << endl;
    cout << "4. Leer Datos desde ESP32 (Serial)" << endl;
    cout << "5. Procesar Todos los Sensores" << endl;
    cout << "6. Mostrar Estado de Sensores" << endl;
    cout << "7. Salir y Liberar Memoria" << endl;
    cout << "========================================" << endl;
    cout << "Opcion: ";
}

void crearSensorTemperatura() {
    cout << "\n[LOG] Creando Sensor de Temperatura..." << endl;
    
    if (sensorTemp != nullptr) {
        cout << "[WARNING] Ya existe un sensor de temperatura. Eliminando el anterior..." << endl;
        delete sensorTemp;
    }
    
    sensorTemp = new SensorTemperatura("T-001");
    cout << "[SUCCESS] Sensor de temperatura creado exitosamente" << endl;
}

void crearSensorPresion() {
    cout << "\n[LOG] Creando Sensor de Presion..." << endl;
    
    if (sensorPres != nullptr) {
        cout << "[WARNING] Ya existe un sensor de presion. Eliminando el anterior..." << endl;
        delete sensorPres;
    }
    
    sensorPres = new SensorPresion("P-105");
    cout << "[SUCCESS] Sensor de presion creado exitosamente" << endl;
}

void registrarLectura() {
    cout << "\n[LOG] Registrando Lectura Manual..." << endl;
    
    int tipoSensor;
    cout << "Selecciona tipo de sensor:" << endl;
    cout << "1. Temperatura" << endl;
    cout << "2. Presion" << endl;
    cout << "Opcion: ";
    cin >> tipoSensor;
    
    if (tipoSensor == 1) {
        if (sensorTemp == nullptr) {
            cout << "[ERROR] No existe sensor de temperatura. Crealo primero (opcion 1)" << endl;
            return;
        }
        
        float valor;
        cout << "Ingresa temperatura (C): ";
        cin >> valor;
        sensorTemp->agregarLectura(valor);
        
    } else if (tipoSensor == 2) {
        if (sensorPres == nullptr) {
            cout << "[ERROR] No existe sensor de presion. Crealo primero (opcion 2)" << endl;
            return;
        }
        
        int valor;
        cout << "Ingresa presion (hPa): ";
        cin >> valor;
        sensorPres->agregarLectura(valor);
        
    } else {
        cout << "[ERROR] Opcion invalida" << endl;
    }
}

void leerDesdeSerial() {
    cout << "\nLeyendo datos desde ESP32..." << endl;
    cout << "Implementar lectura serial" << endl;
    //Abrir puerto COM9 (o el que corresponda)
    //Parsear lineas: "T-001,23.5" o "P-105,1013"
    //Crear sensores dinamicamente o agregar lecturas
}

void procesarSensores() {
    cout << "\n[LOG] Procesando todos los sensores..." << endl;
    
    bool hayAlgunSensor = false;
    
    if (sensorTemp != nullptr) {
        sensorTemp->procesarLectura();
        hayAlgunSensor = true;
    }
    
    if (sensorPres != nullptr) {
        sensorPres->procesarLectura();
        hayAlgunSensor = true;
    }
    
    if (!hayAlgunSensor) {
        cout << "[WARNING] No hay sensores creados aun" << endl;
    }
}

void mostrarEstado() {
    cout << "\n[LOG] Estado actual de los sensores:" << endl;
    
    bool hayAlgunSensor = false;
    
    if (sensorTemp != nullptr) {
        sensorTemp->mostrarEstado();
        hayAlgunSensor = true;
    }
    
    if (sensorPres != nullptr) {
        sensorPres->mostrarEstado();
        hayAlgunSensor = true;
    }
    
    if (!hayAlgunSensor) {
        cout << "[WARNING] No hay sensores creados aun" << endl;
    }
}

void liberarMemoria() {
    cout << "\n[LOG] Liberando memoria..." << endl;
    
    if (sensorTemp != nullptr) {
        delete sensorTemp;
        sensorTemp = nullptr;
    }
    
    if (sensorPres != nullptr) {
        delete sensorPres;
        sensorPres = nullptr;
    }
    
    cout << "[SUCCESS] Memoria liberada correctamente" << endl;
}

void pruebaListaGenerica() {
    cout << "\n=== PRUEBA: Lista Generica con Float ===" << endl;
    ListaSensor<float> listaTemp;
    listaTemp.insertar(23.5f);
    listaTemp.insertar(24.1f);
    listaTemp.insertar(22.8f);
    listaTemp.mostrar();
    cout << "Promedio: " << listaTemp.calcularPromedio() << endl;
    cout << "Maximo: " << listaTemp.obtenerMaximo() << endl;
    cout << "Minimo: " << listaTemp.obtenerMinimo() << endl;
    
    cout << "\n=== PRUEBA: Lista Generica con Int ===" << endl;
    ListaSensor<int> listaPresion;
    listaPresion.insertar(1013);
    listaPresion.insertar(1015);
    listaPresion.insertar(1012);
    listaPresion.mostrar();
    cout << "Promedio: " << listaPresion.calcularPromedio() << endl;
    cout << "Maximo: " << listaPresion.obtenerMaximo() << endl;
    cout << "Minimo: " << listaPresion.obtenerMinimo() << endl;
}

int main() {
    int opcion = 0;
    bool continuar = true;
    
    cout << "\n=== Sistema IoT de Monitoreo Polimorfico ===" << endl;
    cout << "Version 4 - SensorTemperatura + SensorPresion funcionando" << endl;
    cout << "Sistema inicializado correctamente." << endl;
    
    // Prueba inicial comentada (ya funcionan las clases)
    // pruebaListaGenerica();
    // cout << "\n--- Presiona ENTER para continuar al menu ---" << endl;
    // cin.get();

    //Crear instancia de ListaGestion (lista de SensorBase*)
    // ListaGestion* listaGlobal = new ListaGestion();
    
    while (continuar) {
        mostrarMenu();
        cin >> opcion;
        
        // Limpiar buffer de entrada
        cin.ignore(1000, '\n');
        
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
                cout << "\nCerrando sistema..." << endl;
                liberarMemoria();
                continuar = false;
                break;
            default:
                cout << "\nOpcion invalida. Intente de nuevo." << endl;
        }
    }

    cout << "\nSistema cerrado correctamente." << endl;
    return 0;
}
