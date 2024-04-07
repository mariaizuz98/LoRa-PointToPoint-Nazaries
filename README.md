# LoRa-Node-V2
Código en VSCode - PlatformIO del nodo/gateway para el PFG

#### Nota: El código se esta implementado para que pueda ser usado tanto para Nodo como Gateway. Solo sería necesario modificar el archivo Platformio.ini para elegir el tipo.

### Platformio.ini
Uso de librerías y placas
### config.h y config.cpp 
Inicialización de los recursos de la placa de desarrollo y establecer un ID
### packeage.h y packeage.cpp
Envio de paquete de datos y recepción del paquete
### main.cpp
Programa general. Este programa incluye dos hilos. Controlados por un semaforo para sincronizar las tareas. 
- Hilo escucha (vTaskListen): este hilo es utilizado tanto por los nodos como por le gateway. Hilo encargado de estar leer el paquete enviado mediante LoRa.
- Hilo estados (vTaskStates): solo lo utilizan los nodos. Hilo encargado de hacer el cambio de estados. En este ejemplo no se hace uso de los estados ya que se trata de un ejemplo básico que prueba una funcionalidad punto a punto. 
  