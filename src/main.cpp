#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "config/config.h"
#include "package/package.h"
#include "state/states.h"

#define STACK_SIZE  4096

SemaphoreHandle_t xSemaphore; 

void create_taskStates(void);
void create_taskListen(void);
void vTaskListen( void * pvParameters );
void vTaskStates( void * pvParameters );

void setup(void){
    Serial.begin(9600);
    while (!Serial.availableForWrite()) {
      delay(10);
    }
    
    config_Init();
    xSemaphore = xSemaphoreCreateBinary(); 
    if (xSemaphore == NULL) {
        // Error al crear el semáforo
    }
    #ifdef NODE_LORA
      create_taskStates();
    #endif
    create_taskListen();
}

void loop(void){
}

void create_taskStates(void){
    xTaskCreatePinnedToCore(vTaskStates,
                "vTaskStates",
                STACK_SIZE,
                NULL,
                1,
                NULL,
                0);
}

void create_taskListen(void){
    xTaskCreatePinnedToCore(vTaskListen,
                            "vTaskListen",
                            STACK_SIZE,
                            NULL,
                            1,
                            NULL,
                            1);
}

void vTaskListen( void * pvParameters ){
    while(1){
        // Se espera a que el semáforo binario esté liberado
        #ifdef NODE_LORA
          if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
              // Cuando se libera el semáforo, se ejecuta el código de escucha
              if(recievePackage()) readPackage();
          }
        #elif GATEWAY_LORA
          if(recievePackage()) readPackage();
        #endif
    }
}

void vTaskStates( void * pvParameters ){
    while(1){
        switchStates();
        // Se libera el semáforo binario para permitir que la tarea de escucha comience a trabajar
        xSemaphoreGive(xSemaphore);
        // Se introduce un pequeño retardo para evitar la reentrancia inmediata
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}