#include <iostream>
#include <cstdlib>
#include <cstring>
#include "SensorBase.h"
#include "ListaSensor.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaGestion.h"
#include "SerialReader.h"

// Lista global polimorfica para gestionar todos los sensores
ListaGestion* listaGlobal = nullptr;

// Lector serial global
SerialReader* lectorSerial = nullptr;

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
    
    if (lectorSerial == nullptr) {
        lectorSerial = new SerialReader();
    }
    
    if (!lectorSerial->estaConectado()) {
        // Detectar puerto automaticamente segun el sistema operativo
#ifdef _WIN32
        const char* puerto = "COM9";  // Windows
#elif __linux__
        const char* puerto = "/dev/ttyUSB0";  // Linux (hardware real)
        // Nota: En WSL, el acceso a USB requiere USBIPD (ver WSL_USB_SETUP.md)
#else
        const char* puerto = "UNKNOWN";
#endif
        
        std::cout << "[INFO] Intentando conectar al puerto: " << puerto << std::endl;
        
        if (!lectorSerial->conectar(puerto)) {
            std::cout << "[ERROR] No se pudo conectar al ESP32" << std::endl;
            std::cout << "[HINT] Verifica que el ESP32 este conectado y enviando datos" << std::endl;
            return;
        }
    }
    
    std::cout << "[SERIAL] Esperando datos del ESP32..." << std::endl;
    std::cout << "[INFO] Formato esperado: 'T-001,23.5' o 'P-105,1013'" << std::endl;
    std::cout << "[INFO] Presiona Ctrl+C para cancelar" << std::endl;
    
    char linea[256];
    int lecturasRecibidas = 0;
    int intentos = 0;
    const int MAX_INTENTOS = 50;  // 5 segundos aprox
    
    while (intentos < MAX_INTENTOS && lecturasRecibidas < 10) {
        if (lectorSerial->leerLinea(linea, sizeof(linea))) {
            std::cout << "[RECV] " << linea << std::endl;
            
            // Parsear: "T-001,23.5" o "P-105,1013"
            char id[50];
            char valorStr[50];
            
            char* coma = strchr(linea, ',');
            if (coma != nullptr) {
                int idLen = coma - linea;
                strncpy(id, linea, idLen);
                id[idLen] = '\0';
                strcpy(valorStr, coma + 1);
                
                // Determinar tipo de sensor por el prefijo
                if (id[0] == 'T' || id[0] == 't') {
                    // Sensor de temperatura
                    SensorBase* sensor = listaGlobal->buscarSensor(id);
                    
                    if (sensor == nullptr) {
                        // Crear sensor automaticamente
                        std::cout << "[AUTO] Creando sensor de temperatura: " << id << std::endl;
                        SensorTemperatura* nuevoSensor = new SensorTemperatura(id);
                        listaGlobal->agregarSensor(nuevoSensor);
                        sensor = nuevoSensor;
                    }
                    
                    SensorTemperatura* sensorTemp = dynamic_cast<SensorTemperatura*>(sensor);
                    if (sensorTemp != nullptr) {
                        float valor = atof(valorStr);
                        sensorTemp->agregarLectura(valor);
                        lecturasRecibidas++;
                    }
                    
                } else if (id[0] == 'P' || id[0] == 'p') {
                    // Sensor de presion
                    SensorBase* sensor = listaGlobal->buscarSensor(id);
                    
                    if (sensor == nullptr) {
                        // Crear sensor automaticamente
                        std::cout << "[AUTO] Creando sensor de presion: " << id << std::endl;
                        SensorPresion* nuevoSensor = new SensorPresion(id);
                        listaGlobal->agregarSensor(nuevoSensor);
                        sensor = nuevoSensor;
                    }
                    
                    SensorPresion* sensorPres = dynamic_cast<SensorPresion*>(sensor);
                    if (sensorPres != nullptr) {
                        int valor = atoi(valorStr);
                        sensorPres->agregarLectura(valor);
                        lecturasRecibidas++;
                    }
                }
            }
        }
        intentos++;
    }
    
    if (lecturasRecibidas > 0) {
        std::cout << "[SUCCESS] Se recibieron " << lecturasRecibidas << " lecturas del ESP32" << std::endl;
    } else {
        std::cout << "[WARNING] No se recibieron datos del ESP32" << std::endl;
        std::cout << "[HINT] Verifica que el ESP32 este enviando datos por serial" << std::endl;
    }
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
    
    if (lectorSerial != nullptr) {
        delete lectorSerial;
        lectorSerial = nullptr;
    }
    
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
