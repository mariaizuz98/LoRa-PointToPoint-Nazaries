# LoRa-PointToPoint-Nazaries
Código en VSCode - PlatformIO del nodo/gateway para el PFG de María Izuzquiza Fernandez
Curso académico 2023/2024

#### Nota: El código se esta implementado para que pueda ser usado tanto para Nodo como Gateway. Solo sería necesario modificar el archivo Platformio.ini para elegir el tipo.

### Platformio.ini
Uso de librerías y placas
### main.cpp
Programa general. Este programa incluye dos hilos. Se ejecutan en paralelo. 
- Hilo escucha (vTaskListen): este hilo es utilizado tanto por los nodos como por le gateway. Hilo encargado de estar leer el paquete enviado mediante LoRa.
- Hilo estados (vTaskStates): solo lo utilizan los nodos. Hilo encargado de hacer el cambio de estados. 
    * Rama master: En este ejemplo no se hace uso de los estados ya que se trata de un ejemplo básico que prueba una funcionalidad punto a punto. 
    * Rama periodic_send: se añade la funcionalidad del envio de paquetes periodico (cada 5min en las pruebas). Ademñas se añade el envio de un ACK cuando se recibe un mensaje de tipo DATA.
### Carpeta config 
Inicialización de los recursos de la placa de desarrollo y establecer un ID
### Carpeta package
Envio de paquete de datos y recepción del paquete
### Carpeta sensors
Sensor DHT11 y DHT22
### Carpeta states
### Carpeta timers
Temporizadores de envio y reenvio de paquetes. Envio de paquetes cada 15  minutos, en caso de no recibir ACK (confirmación de recepción) se reenvía a los 2 minutos
#### Nota: este tiempo es parametrizable.
### Carpeta secrets
Claves del WiFi para la subida de datos a la plataforma Nazaríes IoT
