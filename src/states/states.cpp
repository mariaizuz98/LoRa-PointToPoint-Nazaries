#include "states.h"

enum states {
    READY,
    LINKED,
    WAITING,
} state;

char measure[256]; // Arreglo estático para almacenar la medida

extern hw_timer_t *sendTimer, *responseTimer;
extern bool sendLoRa, sendLoRaAgain, recieveACK;

void switchStates(void){
    switch (state) {
        case READY:
            state = LINKED;
            break;
        case LINKED:
            if(sendLoRa){
                char* dataMeasure = readSensorDHT();
                strncpy(measure, dataMeasure, sizeof(measure) - 1);
                measure[sizeof(measure) - 1] = '\0';
                sendPackage(GATEWAY, DATA, measure);
                sendLoRa = false; 
                state = WAITING;
            }
            break;
        case WAITING:
            if(sendLoRaAgain){
                Serial.println(" --> Se envia de nuevo el paquete con los datos.\n");
                sendPackage(GATEWAY, DATA, measure);
                sendLoRaAgain = false;
            } else if(recieveACK){
                timerStop(responseTimer);
                timerStart(sendTimer);
                recieveACK = false;
                state = LINKED;
            }
            break;
        default:
            break;
    }
}
