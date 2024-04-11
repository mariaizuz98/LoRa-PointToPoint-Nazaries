#include "states.h"

enum states {
    STANDBY,
    READY,
    LINKED,
    WAITING,
} state;

char measure[256]; // Arreglo estático para almacenar la medida

extern hw_timer_t *sendTimer, *responseTimer;
extern portMUX_TYPE timerMux;
extern bool sendLoRa, sendLoRaAgain, recieveACK;

void switchStates(void){
    switch (state) {
        case STANDBY:
            state = LINKED;
            break; 
        case READY:
            // Implementa la lógica de estado READY si es necesario
            break;
        case LINKED:
            portENTER_CRITICAL(&timerMux);
            if(sendLoRa){
                char* dataMeasure = readSensorDHT();
                strncpy(measure, dataMeasure, sizeof(measure) - 1);
                measure[sizeof(measure) - 1] = '\0';
                sendPackage(GATEWAY, DATA, measure);
                sendLoRa = false;
                timerStop(sendTimer);
                timerWrite(sendTimer, 0);
                timerAlarmEnable(responseTimer); 
                Serial.println("Hola 1");
                state = WAITING;
            }
            portEXIT_CRITICAL(&timerMux);  
            break;
        case WAITING:
            portENTER_CRITICAL(&timerMux);
            if(sendLoRaAgain){
                sendPackage(GATEWAY, DATA, measure);
                timerStop(responseTimer);
                timerWrite(responseTimer, 0);
                timerAlarmEnable(responseTimer); 
                Serial.println("Hola 2");
                sendLoRaAgain = false;
            } else if(recieveACK){
                timerStop(sendTimer);
                timerAlarmEnable(sendTimer); 
                recieveACK = false;
                state = LINKED;
            }
            portEXIT_CRITICAL(&timerMux);
            break;
        default:
            break;
    }
}
