#include "timer.h"

//#define TIME_SEND_MSG 300000000         // alarma cada 15 min (valor en microsegundos)
#define TIME_SEND_MSG 300000000         // alarma cada 5 min (valor en microsegundos) PARA PRUEBAS
#define TIME_RESPONSE_MSG 120000000     // alarma cada 2 min (valor en microsegundos)

hw_timer_t *sendTimer = NULL;
hw_timer_t *responseTimer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

bool sendLoRa = false;
bool sendLoRaAgain = false;

void IRAM_ATTR onSendTimer(void){
    sendLoRa = true;    
}

void IRAM_ATTR onResponseTimer(void){
    timerWrite(sendTimer,0);
    sendLoRaAgain = true;
}


int setupTimers(void){
    // Timer1 para la interrupción del envio de mensajes mediante LoRa (cada 15 min)
    Serial.print("* Initializing Timers...");
    sendTimer = timerBegin(0, 80, true);
    if (sendTimer == NULL) {
        Serial.println(" --- Error (sendTimer): timer not created");
        return -1;
    }
    timerAttachInterrupt(sendTimer, &onSendTimer, true); // adjuntar la función de interrupción
    timerAlarmWrite(sendTimer, TIME_SEND_MSG, true); 
    timerAlarmEnable(sendTimer); // habilitar la alarma

    // Timer2 para la interrupción de vuelta a enviar (cada 2 min)
    responseTimer = timerBegin(1, 80, true); 
    if (responseTimer == NULL) {
        Serial.println(" --- Error (responseTimer): timer not created");
        return -1;
    }
    timerAttachInterrupt(responseTimer, &onResponseTimer, true); // adjuntar la función de interrupción
    timerAlarmWrite(responseTimer, TIME_RESPONSE_MSG, true); 
    Serial.println(" Timers OK");
    return 0;
}
