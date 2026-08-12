# 🎙️ Laboratorio: Prueba de Micrófono (Mic Test) en XIAO ESP32S3 Sense

**👨‍🏫 Profesor:** Ing. Saúl Isaí Soto Ortiz (Titular A)  
**🏫 Institución:** Instituto Tecnológico Superior del Occidente del Estado de Hidalgo (ITSOEH)  
**🎓 Asignatura / Club:** Internet of Things (IoT) & Networking  

Este repositorio contiene la práctica fundamental para activar, configurar y leer datos desde el micrófono PDM integrado en la placa **Seeed Studio XIAO ESP32S3 Sense**. Se utiliza la interfaz I2S (Inter-IC Sound) para capturar el sonido ambiental y graficarlo.

---

## 📋 Requisitos Previos

Antes de comenzar la práctica, los estudiantes deben asegurarse de cumplir con los siguientes puntos:

1. **Hardware:**
   * Placa base **XIAO ESP32S3**.
   * Placa de expansión **Sense** (la que contiene la cámara y el micrófono) correctamente ensamblada ("clic" de conexión).
   * Cable USB-C con soporte para transferencia de datos.
2. **Software:**
   * Arduino IDE (Versión 2.3.x recomendada).
   * Soporte de placas ESP32 instalado en el Gestor de Tarjetas (`esp32 by Espressif Systems`).
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

### Paso 2: El Código
El código de esta práctica (`mic_test_xiao.ino`) está diseñado bajo la analogía de un "Traductor". El procedimiento se divide en tres fases principales:
1. **Contratar al Traductor:** Incluimos la librería `<I2S.h>`.
2. **Asignarle una oficina:** Configuramos los pines 41 y 42.
3. **Ponerlo a trabajar:** Iniciamos la lectura en bucle (`loop()`) filtrando y eliminando el "ruido blanco" matemático para obtener una onda acústica limpia.

### Paso 3: Cargar y Ejecutar
1. Abre el código en un nuevo sketch de Arduino IDE.
2. Conecta tu placa al puerto USB de tu computadora.
3. Presiona el botón **Subir** (la flecha hacia la derecha).
4. En lugar de usar el Monitor Serie normal, ve al menú y abre el **Trazador Serie** (*Herramientas > Trazador Serie* o *Serial Plotter*).
5. Ajusta los baudios en la esquina del Trazador a **115200 baudios**.

---

## 📊 Resultados Esperados

Al abrir el Trazador Serie, observarás lo siguiente:
1. Cuando el entorno esté en absoluto silencio, la gráfica mostrará una línea plana cerca del centro.
2. Si empiezas a hablar, aplaudir, o soplar suavemente hacia el micrófono en la placa Sense (el orificio metálico pequeño), verás cómo la línea plana explota en picos y valles continuos, formando **ondas sonoras**.
3. A mayor volumen de tu voz, más altos serán los "picos de las montañas" gráficos en la pantalla.

---

## 🧠 Explicación de los Resultados

**¿Por qué vemos montañas en la pantalla cuando hablamos?**

* **El Micrófono Físico:** En la placa Sense hay un micrófono MEMS digital. Cuando las ondas de presión de tu voz chocan contra él, su membrana interna vibra microscópicamente (exactamente igual que un tímpano humano).
* **El Traductor PDM a I2S:** El micrófono toma esas vibraciones físicas y las convierte en una rapidísima ráfaga de pulsos eléctricos digitales (0s y 1s).
* **La Librería (I2S):** Toma esa lluvia de pulsos eléctricos y los promedia **16,000 veces por segundo** (`16000 Hz`), agrupándolos y convirtiéndolos en números enteros tradicionales. 
* **La Gráfica:** Arduino IDE recibe estos números (valores positivos cuando hay alta presión acústica, y negativos en baja presión) y los une con una línea en el trazador en tiempo real. 

El resultado que ves en pantalla es la **representación digital matemática de tu voz**. Esta es exactamente la misma tecnología y principio base que utilizan los dispositivos del Internet de las Cosas y los asistentes virtuales (Alexa, Siri) para captar el audio del ambiente antes de procesarlo mediante Inteligencia Artificial.
