#include <iostream>
#include <cstdlib>

using namespace std;

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
    cout << "\nCreando Sensor de Temperatura..." << endl;
    cout << "Implementar clase SensorTemperatura" << endl;
    //Crear instancia de SensorTemperatura
    //Agregar a ListaGestion
}

void crearSensorPresion() {
    cout << "\nCreando Sensor de Presion..." << endl;
    cout << "Implementar clase SensorPresion" << endl;
    // Crear instancia de SensorPresion
    // Agregar a ListaGestion
}

void registrarLectura() {
    cout << "\nRegistrando Lectura Manual..." << endl;
    cout << "Implementar metodo agregarLectura()" << endl;
    //Pedir ID del sensor
    //Pedir valor de lectura
    //Llamar a sensor->agregarLectura()
}

void leerDesdeSerial() {
    cout << "\nLeyendo datos desde ESP32..." << endl;
    cout << "Implementar lectura serial" << endl;
    //Abrir puerto COM9 (o el que corresponda)
    //Parsear lineas: "T-001,23.5" o "P-105,1013"
    //Crear sensores dinamicamente o agregar lecturas
}

void procesarSensores() {
    cout << "\nProcesando todos los sensores..." << endl;
    cout << "Implementar procesamiento polimorfico" << endl;
    //Iterar sobre ListaGestion
    //Llamar a sensor->procesarLectura() para cada uno
}

void mostrarEstado() {
    cout << "\nEstado actual de los sensores:" << endl;
    cout << "Implementar metodo mostrarEstado()" << endl;
    //Iterar y mostrar info de cada sensor
    //Mostrar numero de lecturas almacenadas
}

void liberarMemoria() {
    cout << "\nLiberando memoria..." << endl;
    cout << "Implementar destructores" << endl;
    //Eliminar todos los nodos de las listas
    //Eliminar todos los sensores de ListaGestion
}

int main() {
    int opcion = 0;
    bool continuar = true;
    
    cout << "\n=== Sistema IoT de Monitoreo Polimorfico ===" << endl;
    cout << "Version 2 - Estructura del Main con Menu" << endl;
    cout << "Sistema inicializado correctamente." << endl;

    // Crear instancia de ListaGestion (lista de SensorBase*)
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
                leerDesdSerial();
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
