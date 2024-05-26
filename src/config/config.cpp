#include "config.h"

extern const uint8_t GATEWAY_ID;
SSD1306 display (0x3c, OLED_SDA, OLED_SCL);
#ifdef GATEWAY_LORA
  WiFiClient client;
#endif
uint32_t chipID = 0;
byte myBoardID;

int config_Init(void){
    Serial.println("");
    setupID();
    #ifdef GATEWAY_LORA
      setupWiFi();
      updateTime();
    #endif
    if(setupDisplay() == ERRNO) return -1;
    if(setupLORA() == ERRNO) return -1;
    #ifdef NODE_LORA
      if(setupTimers() == ERRNO) return -1;
      setupDHT();
    #endif

    #ifdef NODE_LORA
      Serial.printf("* LoRa Node... ID: 0x%2X\r\n", myBoardID);
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0, "LORA NODE");
      display.drawString(0, 12, "ID: 0x" + String(myBoardID, HEX));
      display.display();
    #else 
      Serial.printf("* LoRa Gateway.. ID: 0x%2X\r\n", myBoardID);
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0, "LORA GATEWAY");
      display.drawString(0, 12, "ID: 0x" + String(myBoardID, HEX));
      display.display();
    #endif
    return 0;
}

/*
  ESTABLECER ID DEL DISPOSITIVO
  Haciendo uso de la dirección MAC de cada placa
*/
void setupID(void){
    Serial.print("* Creating Device ID...");
    for(int i=0; i<17; i=i+8) {
      chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    myBoardID = (byte) chipID;
    Serial.printf(" Chip ID: %d... ID device: 0x%2X \r\n", chipID, myBoardID);
    delay(1000);
}

/*
  INICIALIZACION DEL DISPLAY
*/
int setupDisplay (void){
    if (OLED_RST != NOT_A_PIN){
      pinMode(OLED_RST, OUTPUT);
      digitalWrite(OLED_RST, LOW);
      delay(20);
      digitalWrite(OLED_RST, HIGH);
    }
    Serial.print("* Initializing Display...");
    // initialize OLED
    Wire.begin(OLED_SDA, OLED_SCL);
    if (!display.init()){ // Address 0x3C for 128x32
      Serial.println((" --- Error: SSD1306 allocation failed"));
      return -1;
    }
    Serial.println(" Display OK");
    return 0;
}

/*
  INICIALIZACION PARAMETROS LoRa
*/
int setupLORA (void){
    Serial.print("* Initializing LoRa...");
    //SPI LoRa pins
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    //setup LoRa transceiver module
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(BAND)) {
      Serial.println(" --- Error: Starting LoRa failed!");
      return -1;
    }
    LoRa.setTxPower(TXPOWER);
    // Text serial monitor and display
    Serial.println(" LoRa OK");
    return 0;
}

/*
  INICIALIZACION DEL WiFi
  Solo en caso de que el dispositivo sea etiquetado como Gateway se hará uso del WiFi
*/
void setupWiFi(void){
    #ifdef GATEWAY_LORA
      Serial.print("* Initializing WiFi...");
      /*Connection to the WiFi Network*/
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
      }
      // Text serial monitor and display
      Serial.println(" WiFi OK");
    #endif
}
