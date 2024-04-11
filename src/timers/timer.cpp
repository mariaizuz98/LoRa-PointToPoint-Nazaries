#include "timer.h"

hw_timer_t *sendTimer = NULL;
hw_timer_t *responseTimer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

bool sendLoRa = false;
bool sendLoRaAgain = false;

void IRAM_ATTR onSendTimer(void){
    portENTER_CRITICAL(&timerMux);
    sendLoRa = true;  
    portEXIT_CRITICAL(&timerMux);  
}

void IRAM_ATTR onResponseTimer(void){
    portENTER_CRITICAL(&timerMux);
    sendLoRaAgain = true;
    portEXIT_CRITICAL(&timerMux);
}


int setupTimers(void){
    // Timer1 para la interrupción del envio de mensajes mediante LoRa (cada 15 min)
    Serial.print("* Initializing Timers...");
    sendTimer = timerBegin(0, 8000, true);
    if (sendTimer == NULL) {
        Serial.println(" --- Error (sendTimer): timer not created");
        return -1;
    }
    timerAttachInterrupt(sendTimer, &onSendTimer, true); // adjuntar la función de interrupción
    timerAlarmWrite(sendTimer, TIME_SEND_MSG, false); 
    timerAlarmEnable(sendTimer); // habilitar la alarma

    // Timer2 para la interrupción de vuelta a enviar (cada 2 min)
    responseTimer = timerBegin(1, 80, true); 
    if (responseTimer == NULL) {
        Serial.println(" --- Error (responseTimer): timer not created");
        return -1;
    }
    timerAttachInterrupt(responseTimer, &onResponseTimer, true); // adjuntar la función de interrupción
    timerAlarmWrite(responseTimer, TIME_RESPONSE_MSG, false); 
    Serial.println(" Timers OK");
    return 0;
}
