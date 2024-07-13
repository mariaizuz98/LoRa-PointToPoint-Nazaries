# LoRa-PointToPoint-Nazaries
Código en VSCode - PlatformIO del nodo/gateway para el PFG de María Izuzquiza Fernandez
Curso académico 2023/2024

#### Nota: El código se esta implementado para que pueda ser usado tanto para Nodo como Gateway. Solo sería necesario modificar el archivo Platformio.ini para elegir el tipo.

### Platformio.ini
Uso de librerías y placas de desarrollo
### main.cpp
Programa general. Este programa incluye dos hilos. Se ejecutan en paralelo. 
- Hilo escucha (vTaskListen): este hilo es utilizado tanto por los nodos como por le gateway. Hilo encargado de estar leer el paquete enviado mediante LoRa.
- Hilo estados (vTaskStates): solo lo utilizan los nodos. Hilo encargado de hacer el cambio de estados. 
    * Rama master: En este ejemplo no se hace uso de los estados ya que se trata de un ejemplo básico que prueba una funcionalidad punto a punto. 
    * Rama periodic_send: se añade la funcionalidad del envio de paquetes periodico (cada 5min en las pruebas). Además se añade el envio de un ACK cuando se recibe un mensaje de tipo DATA.
### Carpeta config 
Inicialización de los recursos de la placa de desarrollo y establecimiento de un ID único.
Recursos utilizados:
| Gateway  | Nodo |
| ------------- | ------------- |
| ID  | ID  |
| WiFI  | -  |
| LCD  | LCD  |
| LoRa  | LoRa  |
| -  | DHT11  |
### Carpeta package
Envio de paquete de datos y recepción del paquetes
Estructura de paquetes LoRa:
| ID propio (1 byte)  | ID destino (1 byte) | ID mensaje (1 byte)  | mensaje (variable) |
| ------------- | ------------- |
### Carpeta secrets
Archivo de ejemplo para las creedenciales de la conexión WiFi
### Carpeta sensors
Sensor DHT11
### Carpeta states
Estados del sistemas (READY, LINKED, WAITING)
- READY: para esta prueba no se implementa
- LINKED: envío periódico de medidas del sensor
- WAITING: estado de espera hasta recibir confirmación de recepción
### Carpeta timers
Temporizadores de envio y reenvio de paquetes. Para las pruebas realizadas los tiempos son los siguientes:
- Envio de paquetes cada 2 minutos
- Temporizador de reenvío: 1 min (no se recibie ACK (confirmación de recepción))
#### Nota: este tiempo es parametrizable.

