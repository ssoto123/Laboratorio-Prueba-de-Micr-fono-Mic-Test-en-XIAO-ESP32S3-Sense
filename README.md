# 🎙️ Laboratorio: Prueba de Micrófono (Mic Test) en XIAO ESP32S3 Sense

**👨‍🏫 Profesor:** Ing. Saúl Isaí Soto Ortiz (Titular A)  
**🏫 Institución:** Instituto Tecnológico Superior del Occidente del Estado de Hidalgo (ITSOEH)  
**🎓 Asignatura / Club:** Internet of Things (IoT) & Networking  

Este repositorio contiene la práctica fundamental para activar, configurar y leer datos desde el micrófono PDM integrado en la placa **Seeed Studio XIAO ESP32S3 Sense**. Se utiliza la interfaz I2S (Inter-IC Sound) actualizada para capturar el sonido ambiental, filtrarlo digitalmente y graficarlo utilizando los núcleos más recientes de ESP32.

---

## 📋 Requisitos Previos

Antes de comenzar la práctica, los estudiantes deben asegurarse de cumplir con los siguientes puntos:

1. **Hardware:**
   * Placa base **XIAO ESP32S3**.
   * Placa de expansión **Sense** (la que contiene la cámara y el micrófono) correctamente ensamblada ("clic" de conexión).
   * Cable USB-C con soporte para transferencia de datos.
2. **Software:**
   * Arduino IDE (Versión 2.3.x recomendada).
   * Soporte de placas ESP32 instalado en el Gestor de Tarjetas (`esp32 by Espressif Systems` **versión 3.3.10 o superior**).
3. **Configuración en Arduino IDE:**
   * **Placa:** `XIAO_ESP32S3`.
   * **PSRAM:** `OPI PSRAM` (¡Importante para evitar desbordamientos de memoria!).

---

## 🛠️ Desarrollo (Paso a Paso)

Vamos a configurar nuestra placa para que se convierta en una "oreja digital". Sigue estos pasos de forma ordenada:

### Paso 1: Entender los pines
El micrófono de tu placa está soldado directamente y se comunica usando un protocolo especial llamado **PDM (Pulse-Density Modulation)** a través de I2S. Utiliza internamente dos pines:
* **Pin 42 (CLK):** Es como el director de orquesta que marca el ritmo al que viajan los datos (reloj).
* **Pin 41 (DATA):** Es el cable principal (o tubería) por donde viaja la información acústica.

### Paso 2: El Código (Filtrado de Señal y Compuerta de Ruido)
El código de esta práctica (`mic_test_xiao.ino`) procesa la señal en tiempo real para limpiarla usando tres técnicas:
1. **Filtro DC (Eliminar Falso Cero):** Resta el voltaje base del micrófono para centrar la onda.
2. **Compuerta de Ruido (Noise Gate):** Ignora el ruido eléctrico microscópico de la placa silenciando valores muy bajos.
3. **Anclas Visuales:** Evita que el Trazador Serie haga un "zoom" exagerado al ruido de fondo.

### Paso 3: Cargar y Ejecutar
1. Abre el código en un nuevo sketch de Arduino IDE.
2. Conecta tu placa al puerto USB de tu computadora.
3. Presiona el botón **Subir** (la flecha hacia la derecha).
4. Ve al menú y abre el **Trazador Serie** (*Herramientas > Trazador Serie*).
5. **¡PASO CRÍTICO!** Ajusta los baudios en la esquina inferior derecha del Trazador a **115200 baudios**.

---

## ⚠️ Solución de Problemas Comunes (Troubleshooting)

**Error:** La gráfica muestra líneas sin sentido o ruido gigante y descontrolado.
* **Solución 1 (Baudios):** Verifica la velocidad en la esquina inferior derecha del Trazador Serie. Debe decir exactamente `115200 baud`. Si está en `9600 baud`, el IDE interpreta los datos como ruido basura.
* **Solución 2 (Ruido Ambiental):** Si la gráfica sigue temblando mucho en silencio, incrementa la variable `UMBRAL_RUIDO` en el código (ej. de 150 a 300) para volver al micrófono un poco "más sordo" ante el ruido de fondo.

**Error:** `Compilation error: ESP_I2S.h: No such file or directory`
* **Solución:** Tienes una versión muy antigua del núcleo ESP32. Ve a *Herramientas > Placa > Gestor de tarjetas*, busca `esp32` y actualiza el paquete *esp32 by Espressif Systems* a la versión 3.x.x.

---

## 📊 Resultados Esperados

Al abrir el Trazador Serie (a 115200 baudios), observarás tres líneas (etiquetadas como Value 1, Value 2 y Value 3):
1. **Value 1 y Value 2:** Dos líneas rectas horizontales en los topes superior e inferior de la pantalla. Son nuestros "muros" de contención visual.
2. **Value 3:** Una línea en el centro (valor **0**). En silencio absoluto, esta línea debe ser **completamente plana**.
3. Si hablas o silbas cerca del orificio metálico del micrófono, verás cómo la línea central (Value 3) explota en picos y valles formando **ondas sonoras limpias** dentro de los muros.

---

## 🧠 Explicación de los Resultados

**¿Por qué tuvimos que agregar una "Compuerta de Ruido"?**
* **El Piso de Ruido (Noise Floor):** Los componentes electrónicos no son perfectos. La propia electricidad fluyendo por la placa genera micro-alteraciones. Además, el micrófono capta el ventilador de la PC o las vibraciones de la mesa de laboratorio. 
* **La Compuerta (Noise Gate):** Funciona como el cadenero de una discoteca: si la onda de sonido no tiene la suficiente fuerza (volumen) para superar el umbral configurado (ej. 150 puntos), la convierte en un 0 absoluto. Solo cuando hablas fuerte, la onda logra pasar.

**¿Por qué salen 3 valores ("Value 1", "Value 2", "Value 3") en la gráfica?**
*Esta es la técnica de las "Anclas Visuales":*
Imagina que el Trazador Serie de Arduino es como un camarógrafo aficionado que tiene una cámara con un **zoom automático incontrolable**. Si tú te quedas en silencio, en la "habitación" de la señal solo queda una microscópica pelusa de polvo (el ruido eléctrico residual). El camarógrafo, al no tener nada más interesante que grabar, le hace un súper zoom a esa pelusa hasta que parece un meteorito llenando la pantalla. Esto hace que una señal de ruido insignificante parezca una onda inmensa y caótica.

Para controlar a este camarógrafo, en nuestro código hicimos un truco: le enviamos tres datos al mismo tiempo separados por comas (`-3000, 3000, onda_limpia`). Arduino interpreta cada valor separado por coma como una línea distinta:
*   **Value 1 (-3000) y Value 2 (3000):** Al enviarlos constantemente, funcionan como el piso y el techo del escenario.
*   **Value 3 (onda_limpia):** Es el sonido de tu voz rebotando en el centro.

Al ver el piso y el techo estáticos, el "camarógrafo" (el Trazador Serie) se ve obligado a "alejar la toma" permanentemente para que ambos quepan en la pantalla. Así engañamos al auto-zoom: el escenario se estabiliza, el silencio absoluto por fin se ve como una línea plana real y tu voz tiene un espacio definido para dibujarse correctamente sin marear al espectador.
