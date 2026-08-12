/*
 * 🎙️ Prueba de Micrófono (Mic Test) - XIAO ESP32S3 Sense
 * 
 * 👨‍🏫 Autor: Ing. Saúl Isaí Soto Ortiz
 * 🏫 Institución: Instituto Tecnológico Superior del Occidente del Estado de Hidalgo (ITSOEH)
 * 🎓 Asignatura: Internet of Things (IoT) & Networking
 * 
 * Descripción: Este código activa el micrófono integrado en la placa
 * XIAO ESP32S3 Sense y envía las lecturas de sonido al Trazador Serie.
 */

// 📦 LIBRERÍA NECESARIA
// Imagina que esta librería es un "traductor" que le enseña a la placa ESP32 
// a entender el idioma especial (llamado I2S) que habla el micrófono.
#include <I2S.h>

// Creamos un objeto de la clase I2S. 
// Piensa en él como nuestro "empleado" encargado exclusivamente de escuchar al micrófono.
I2SClass I2S;

void setup() {
  // 1️⃣ ENCENDER LA COMUNICACIÓN CON LA PC (PARA VER LOS DATOS)
  // Abrimos la línea de comunicación con la computadora a una velocidad rápida (115200 baudios).
  Serial.begin(115200);
  while (!Serial) {
    ; // Nos quedamos aquí atrapados esperando hasta que la PC y la placa logren conectarse.
  }

  // 2️⃣ CONFIGURAR LOS PINES DEL MICRÓFONO
  // El micrófono PDM del XIAO usa dos pines específicos que vienen así de fábrica:
  // Pin 42 = El "metrónomo" (Reloj/CLK) que marca el ritmo exacto de la escucha.
  // Pin 41 = La "tubería" por donde viajan los datos del audio (DATA).
  I2S.setPinsPdmRx(42, 41);

  // 3️⃣ ARRANCAR EL MICRÓFONO (INICIAR I2S)
  // Aquí le damos las reglas del juego a nuestro micrófono:
  // - Modo PDM de recepción (Modo de escucha activa).
  // - 16000 muestras por segundo (Su velocidad de captura / 16KHz).
  // - 16-bits de calidad por cada muestra (Resolución profunda de los datos).
  // - Modo Mono (Solo tiene un micrófono, así que es 1 canal de audio, no estéreo).
  if (!I2S.begin(I2S_MODE_PDM_RX, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    Serial.println("❌ ¡Error! El micrófono no pudo iniciar. Revisa tu placa Sense.");
    while (1); // Si falla, nos quedamos atrapados en un bucle infinito por seguridad.
  }
}

void loop() {
  // 4️⃣ ESCUCHAR CONSTANTEMENTE EL ENTORNO
  // Le pedimos al empleado (I2S) que nos pase una sola "muestra" (un milisegundo de sonido)
  int sample = I2S.read();
  
  // 5️⃣ FILTRAR EL RUIDO BASURA Y MOSTRAR EL RESULTADO
  // A veces el micrófono reporta valores vacíos como 0, -1 o 1 por matemáticas internas,
  // incluso si estás en silencio absoluto. Los ignoramos para no ensuciar nuestra gráfica.
  if (sample && sample != -1 && sample != 1) {
    // Enviamos el valor final. 
    // Al abrir el "Trazador Serie" en Arduino, estos números se dibujarán como ondas sonoras.
    Serial.println(sample);
  }
}
