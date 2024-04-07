#include "states.h"

enum states {
    STANDBY,
    READY,
    LINKED,
    WAITING,
} state;

bool firt_time = false;
char* measure = new char[256]; // Allocate memory for measure

extern void create_taskListen(void);
extern hw_timer_t *responseTimer;
extern bool sendLoRa, sendLoRaAgain;

void switchStates(void){
    switch (state) {
        case STANDBY:
            // if(config_Init() != ERRNO) state = READY;
            // if(config_Init() != ERRNO) state = LINKED;
            state = LINKED;
            firt_time = true;
            break; 
        case READY:
            // timerAlarmEnable(responseTimer);
            // if(sendLoRaAgain){
            //     sendPackage(BROADCAST, REQUEST_CONNECT_MSG, "");
            //     sendLoRaAgain = false;
            // } else if (recievePackage()){
            //     state = LINKED;
            // }
            break;
        case LINKED:
            if(firt_time){
                measure = readSensorDHT();
                delay(500);
                sendPackage(GATEWAY, DATA, measure);
                firt_time = false;
                timerAlarmEnable(responseTimer); 
                timerWrite(responseTimer, 0);
            } 
            // else if(sendLoRa){
            //     measure = readSensorDHT();
            //     delay(500);
            //     sendPackage(GATEWAY, DATA, measure);
            //     sendLoRa = false;
            //     timerAlarmEnable(responseTimer); 
            //     timerWrite(responseTimer, 0);
            // } else if(sendLoRaAgain){
            //     sendPackage(GATEWAY, DATA, measure);
            //     sendLoRaAgain = false;
            // }
            break;
        case WAITING:
            break;
        default:
            break;
    }

}