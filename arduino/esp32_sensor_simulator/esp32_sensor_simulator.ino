// ===== CONFIGURACION =====
#define INTERVALO_LECTURA 2000  // Milisegundos entre lecturas (2 segundos)
#define NUM_SENSORES_TEMP 3     // Cantidad de sensores de temperatura
#define NUM_SENSORES_PRES 2     // Cantidad de sensores de presion

// Rangos de valores simulados
#define TEMP_MIN 18.0
#define TEMP_MAX 30.0
#define PRESION_MIN 1000
#define PRESION_MAX 1020

// ===== VARIABLES GLOBALES =====
unsigned long ultimaLectura = 0;

float randomFloat(float minVal, float maxVal) {
  return minVal + (random(0, 10000) / 10000.0) * (maxVal - minVal);
}

void setup() {
  // Inicializar comunicacion serial a 115200 baudios
  Serial.begin(115200);
  
  // Esperar a que se establezca la conexion serial
  delay(1000);
  
  Serial.println("========================================");
  Serial.println("  ESP32 - Simulador de Sensores IoT   ");
  Serial.println("========================================");
  Serial.println("Sistema iniciado correctamente");
  Serial.println("Enviando datos cada 2 segundos...");
  Serial.println("Formato: ID,VALOR");
  Serial.println("  Temperatura: T-XXX,##.#");
  Serial.println("  Presion:     P-XXX,####");
  Serial.println("========================================");
  Serial.println();
  
  // Inicializar semilla aleatoria
  randomSeed(analogRead(0));
  
  // LED integrado para indicar actividad
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned long tiempoActual = millis();
  
  // Verificar si es tiempo de enviar nueva lectura
  if (tiempoActual - ultimaLectura >= INTERVALO_LECTURA) {
    ultimaLectura = tiempoActual;
    
    // Parpadear LED para indicar transmision
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Enviar lecturas de sensores de temperatura
    for (int i = 1; i <= NUM_SENSORES_TEMP; i++) {
      float temperatura = randomFloat(TEMP_MIN, TEMP_MAX);
      
      // Formato: T-001,23.5
      Serial.print("T-");
      if (i < 10) Serial.print("00");
      else if (i < 100) Serial.print("0");
      Serial.print(i);
      Serial.print(",");
      Serial.println(temperatura, 1);  // 1 decimal
      
      delay(100);  // Pequeña pausa entre sensores
    }
    
    // Enviar lecturas de sensores de presion
    for (int i = 1; i <= NUM_SENSORES_PRES; i++) {
      int presion = random(PRESION_MIN, PRESION_MAX + 1);
      
      // Formato: P-105,1013
      Serial.print("P-");
      if (i < 10) Serial.print("00");
      else if (i < 100) Serial.print("0");
      Serial.print(100 + i);  // IDs de presion empiezan en 101
      Serial.print(",");
      Serial.println(presion);
      
      delay(100);
    }
    
    // Linea separadora para facilitar lectura
    Serial.println("---");
    
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  // Pequeña pausa para no saturar el CPU
  delay(10);
}

