# Ranokau - Automatización de Microclimas Urbanos

Bienvenido al repositorio de **Ranokau**, una iniciativa que busca acercar el cultivo directo a los entornos urbanos mediante la automatización de microclimas. Nuestro objetivo es facilitar la producción de alimentos frescos y saludables en espacios reducidos, aprovechando tecnologías de sensorización y control inteligente.

## Descripción del Proyecto

En Ranokau, creemos que la ciudad puede convertirse en un espacio productivo gracias a la integración de la tecnología. Este repositorio contiene el código fuente en C++ para la gestión de sensores y actuadores en entornos de cultivo urbano automatizado. Nuestro enfoque se centra en:

- **Sensores ambientales:** Temperatura, humedad, luminosidad y calidad del aire.
- **Controladores y actuadores:** Ventilación, riego, iluminación y calefacción.
- **Comunicación inalámbrica:** Uso de dispositivos ESP32 con conectividad Wi-Fi para la transmisión de datos.
- **Integración vía MQTT:** Intercambio de información entre sensores y servidores para la monitorización y el control remoto.

El objetivo es crear un sistema modular y escalable que permita gestionar microclimas de forma autónoma, adaptándose a las necesidades específicas de cada cultivo y ubicación.

## Características

- **Código en C++ optimizado** para dispositivos con recursos limitados.
- **Compatibilidad con ESP32:** Un microcontrolador potente y versátil ideal para IoT.
- **Soporte para MQTT:** Un protocolo ligero para la comunicación máquina a máquina.
- **Fácil integración:** Diseñado para extender con nuevos sensores, actuadores y lógicas de control.
- **Escalable:** Se puede desplegar desde pequeñas instalaciones en balcón hasta proyectos urbanos a gran escala.

## Requerimientos

- **Hardware:**
  - Un dispositivo ESP32
  - Sensores (DHT22 u otros para temperatura/humedad, fotocélulas para luz, MQ-135 u otros para calidad del aire)
  - Actuadores (bombas de agua, relés para luces, ventiladores)
- **Software:**
  - [PlatformIO](https://platformio.org/) o [Arduino IDE](https://www.arduino.cc/) con soporte para ESP32
  - Biblioteca cliente MQTT (por ejemplo: `PubSubClient`)
  - Biblioteca para manejo de sensores (por ejemplo: `Adafruit_Sensor`, `DHTesp`)

## Instalación

1. **Clonar el repositorio:**  
   ```bash
   git clone https://github.com/tu_usuario/ranokau.git
   cd ranokau
