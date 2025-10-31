#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <iostream>
#include <cstring>

// Detectar sistema operativo en tiempo de compilacion
#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #include <windows.h>
#elif __linux__
    #define PLATFORM_LINUX
    #include <fcntl.h>
    #include <unistd.h>
    #include <termios.h>
#endif

class SerialReader {
private:
#ifdef PLATFORM_WINDOWS
    HANDLE hSerial;           ///< Handle del puerto serial en Windows
#elif defined(PLATFORM_LINUX)
    int serialPort;           ///< File descriptor en Linux
#endif
    bool conectado;           ///< Estado de la conexion
    char buffer[256];         ///< Buffer para almacenar datos recibidos

public:
    SerialReader() : conectado(false) {
#ifdef PLATFORM_WINDOWS
        hSerial = INVALID_HANDLE_VALUE;
#elif defined(PLATFORM_LINUX)
        serialPort = -1;
#endif
        std::cout << "[CONSTRUCTOR] SerialReader creado" << std::endl;
#ifdef PLATFORM_WINDOWS
        std::cout << "[INFO] Sistema detectado: Windows" << std::endl;
#elif defined(PLATFORM_LINUX)
        std::cout << "[INFO] Sistema detectado: Linux" << std::endl;
#endif
    }
    ~SerialReader() {
        desconectar();
        std::cout << "[DESTRUCTOR] SerialReader liberado" << std::endl;
    }

    bool conectar(const char* puerto) {
#ifdef PLATFORM_WINDOWS
        std::cout << "[SERIAL] Conectando a " << puerto << " (Windows)..." << std::endl;
        
        hSerial = CreateFileA(
            puerto,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cout << "[ERROR] No se pudo abrir el puerto " << puerto << std::endl;
            std::cout << "[HINT] Verifica que el ESP32 este conectado y el driver instalado" << std::endl;
            return false;
        }

        // Configurar parametros del puerto serial
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(hSerial, &dcbSerialParams)) {
            std::cout << "[ERROR] Error al obtener configuracion del puerto" << std::endl;
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE;
            return false;
        }

        dcbSerialParams.BaudRate = CBR_115200;  // 115200 baudios
        dcbSerialParams.ByteSize = 8;           // 8 bits de datos
        dcbSerialParams.StopBits = ONESTOPBIT;  // 1 bit de parada
        dcbSerialParams.Parity = NOPARITY;      // Sin paridad

        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cout << "[ERROR] Error al configurar puerto serial" << std::endl;
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE;
            return false;
        }

        // Configurar timeouts
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        
        if (!SetCommTimeouts(hSerial, &timeouts)) {
            std::cout << "[ERROR] Error al configurar timeouts" << std::endl;
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE;
            return false;
        }

        conectado = true;
        std::cout << "[SUCCESS] Puerto serial abierto: " << puerto << std::endl;
        return true;

#elif defined(PLATFORM_LINUX)
        std::cout << "[SERIAL] Conectando a " << puerto << " (Linux)..." << std::endl;
        
        serialPort = open(puerto, O_RDWR | O_NOCTTY);
        
        if (serialPort < 0) {
            std::cout << "[ERROR] No se pudo abrir el puerto " << puerto << std::endl;
            std::cout << "[HINT] Verifica permisos: sudo chmod 666 " << puerto << std::endl;
            return false;
        }

        // Configurar puerto serial en Linux
        struct termios tty;
        if (tcgetattr(serialPort, &tty) != 0) {
            std::cout << "[ERROR] Error al obtener atributos del puerto" << std::endl;
            close(serialPort);
            serialPort = -1;
            return false;
        }

        // Configurar velocidad 115200 baudios
        cfsetospeed(&tty, B115200);
        cfsetispeed(&tty, B115200);

        // 8N1 (8 bits, sin paridad, 1 bit de parada)
        tty.c_cflag &= ~PARENB;        // Sin paridad
        tty.c_cflag &= ~CSTOPB;        // 1 bit de parada
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;            // 8 bits
        tty.c_cflag &= ~CRTSCTS;       // Sin control de flujo hardware
        tty.c_cflag |= CREAD | CLOCAL; // Habilitar lectura

        tty.c_lflag &= ~ICANON;        // Modo no canonico
        tty.c_lflag &= ~ECHO;          // Sin eco
        tty.c_lflag &= ~ISIG;          // Sin senales

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Sin control de flujo software
        tty.c_oflag &= ~OPOST;         // Sin procesamiento de salida

        // Timeouts
        tty.c_cc[VTIME] = 10;    // Timeout 1 segundo
        tty.c_cc[VMIN] = 0;      // Lectura no bloqueante

        if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
            std::cout << "[ERROR] Error al configurar puerto serial" << std::endl;
            close(serialPort);
            serialPort = -1;
            return false;
        }

        conectado = true;
        std::cout << "[SUCCESS] Puerto serial abierto: " << puerto << std::endl;
        return true;
#else
        std::cout << "[ERROR] Sistema operativo no soportado" << std::endl;
        return false;
#endif
    }

    bool leerLinea(char* linea, int maxLen) {
        if (!conectado) {
            return false;
        }

#ifdef PLATFORM_WINDOWS
        DWORD bytesLeidos;
        char c;
        int index = 0;

        while (index < maxLen - 1) {
            if (ReadFile(hSerial, &c, 1, &bytesLeidos, nullptr)) {
                if (bytesLeidos > 0) {
                    if (c == '\n') {
                        linea[index] = '\0';
                        return index > 0;
                    } else if (c != '\r') {
                        linea[index++] = c;
                    }
                }
            } else {
                break;
            }
        }
        linea[index] = '\0';
        return false;

#elif defined(PLATFORM_LINUX)
        int bytesLeidos = read(serialPort, buffer, sizeof(buffer) - 1);
        
        if (bytesLeidos > 0) {
            buffer[bytesLeidos] = '\0';
            
            // Buscar salto de linea
            char* newline = strchr(buffer, '\n');
            if (newline != nullptr) {
                int len = newline - buffer;
                if (len < maxLen) {
                    strncpy(linea, buffer, len);
                    linea[len] = '\0';
                    return true;
                }
            }
        }
        return false;
#else
        return false;
#endif
    }

    void desconectar() {
        if (!conectado) {
            return;
        }

#ifdef PLATFORM_WINDOWS
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE;
        }
#elif defined(PLATFORM_LINUX)
        if (serialPort >= 0) {
            close(serialPort);
            serialPort = -1;
        }
#endif
        conectado = false;
        std::cout << "[SERIAL] Puerto cerrado" << std::endl;
    }

    bool estaConectado() const {
        return conectado;
    }
};

#endif // SERIALREADER_H
