/*
 * 🎙️ Prueba de Micrófono (Mic Test) PRO - XIAO ESP32S3 Sense
 * 
 * 👨‍🏫 Autor: Ing. Saúl Isaí Soto Ortiz
 * 🏫 Institución: Instituto Tecnológico Superior del Occidente del Estado de Hidalgo (ITSOEH)
 * 🎓 Asignatura: Internet of Things (IoT) & Networking
 * 
 * Descripción: Este código activa el micrófono PDM de la placa, elimina el 
 * voltaje base (DC offset), aplica una compuerta de ruido para el silencio absoluto 
 * y estabiliza visualmente el Trazador Serie.
 */

#include <ESP_I2S.h>

I2SClass I2S;

// Variables para nuestro filtro de señal
float promedio_ruido = 0; 
int contador_muestras = 0; 

// 🛑 COMPUERTA DE RUIDO (NOISE GATE)
// Todo sonido que esté por debajo de este valor será considerado "Silencio Absoluto".
// Si en tu salón de clases hay mucho ruido de fondo, sube este número (ej. 200 o 300).
const int UMBRAL_RUIDO = 150; 

void setup() {
  // 1️⃣ INICIAR COMUNICACIÓN A ALTA VELOCIDAD (Asegúrate de poner 115200 en el Trazador)
  Serial.begin(115200);
  while (!Serial) { ; }

  // 2️⃣ CONFIGURAR LOS PINES DEL MICRÓFONO
  I2S.setPinsPdmRx(42, 41);

  // 3️⃣ INICIAR EL MICRÓFONO (16KHz, 16-bits, Mono)
  if (!I2S.begin(I2S_MODE_PDM_RX, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    Serial.println("❌ Error al iniciar el micrófono.");
    while (1); 
  }
}

void loop() {
  // 4️⃣ LEER UNA MUESTRA DE SONIDO
  int muestra_cruda = I2S.read();
  
  if (muestra_cruda != 0 && muestra_cruda != -1 && muestra_cruda != 1) {
    
    // 5️⃣ FILTRO MATEMÁTICO (Eliminar el Falso Cero / DC Offset)
    promedio_ruido = (0.99 * promedio_ruido) + (0.01 * muestra_cruda);
    int onda_limpia = muestra_cruda - (int)promedio_ruido;
    
    // 6️⃣ COMPUERTA DE RUIDO (Silenciar el piso de ruido eléctrico)
    // Utilizamos abs() para evaluar tanto los picos positivos como los negativos.
    if (abs(onda_limpia) < UMBRAL_RUIDO) {
      onda_limpia = 0; // Lo forzamos a silencio absoluto
    }
    
    // 7️⃣ CONTROL DE VELOCIDAD Y ESTABILIZADOR VISUAL
    // Graficamos 1 de cada 10 muestras para que no parpadee tan rápido
    contador_muestras++;
    if (contador_muestras % 10 == 0) {
      
      // TRUCO: Enviamos límites fijos (-3000 y 3000) antes de la onda real.
      // Esto evita que el Trazador Serie haga un "zoom" exagerado en la pantalla.
      Serial.print("-3000, 3000, ");
      
      // Finalmente, enviamos la onda de nuestra voz
      Serial.println(onda_limpia); 
    }
  }
}
