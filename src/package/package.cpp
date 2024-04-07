#include "package.h"

extern byte boardID;
extern SSD1306 display;
extern hw_timer_t *sendTimer;
extern hw_timer_t *responseTimer;
extern const uint8_t GATEWAY_ID;

byte senderID;          // sender address
byte incomingMsgID;     // incoming msg ID
int incomingTempT , incomingHumidity = 0;

void sendPackage(byte destID, byte msgID, char* msg){
    #ifdef NODE_LORA
        // send packet
        LoRa.beginPacket();
        LoRa.write(boardID);
        LoRa.write(destID);
        LoRa.write(msgID);
        LoRa.print(msg);  // send the high byte
        LoRa.endPacket();

        Serial.printf(" ··· Message LoRa send... \r\n");
        Serial.printf("Sender: 0x%2X |  Destination: 0x%2X  |  Message ID: 0x%2X  |  Message: %s  \r\n", 
                    boardID, destID, msgID, msg);
    #endif
}

bool recievePackage (void){
    bool receiveMsg = false;
    if(LoRa.parsePacket() != 0)     receiveMsg = true;
    else                            receiveMsg = false;

    return receiveMsg;
}

void readPackage(void){
    int receiverID;          // recipient address
    String incomingMsg;
    
    // read packet header bytes:
    senderID = LoRa.read();                             // sender address
    receiverID = LoRa.read();                           // recipient/gateway address
    incomingMsgID = LoRa.read();                        // incoming msg ID
    incomingMsg = "";

    while (LoRa.available()) {
        incomingMsg += (char)LoRa.read();
    }

    Serial.println(" ··· Message LoRa receive...");

    Serial.printf("Sender: 0x%2X |  Destination: 0x%2X  |  Message ID: %d  |  Message: %s  |  RSSI:  %d  | SNR:  %.2f  \r\n", 
                   senderID, receiverID, incomingMsgID, incomingMsg, LoRa.packetRssi(), LoRa.packetSnr());

    parseSensorData(strdup(incomingMsg.c_str()));
    #ifdef GATEWAY_LORA
        if (WiFi.status() == WL_CONNECTED) {
            // JSON creation
            newJSON(incomingTempT, incomingHumidity);
            // Encryption data
            encryptData();
            // Post Function
            SendPost();
        } else {
            Serial.println("");
            Serial.println("Failed to connect!");
            setupWiFi();
         }
    #endif
}


void parseSensorData(const char *sensorData) {
    char* token;
    char dataCopy[strlen(sensorData) + 1];
    strcpy(dataCopy, sensorData);

    // Firts token (temperature Celsius)
    token = strtok(dataCopy, ";");
    if (token != NULL) {
        incomingTempT = atoi(token); // Convert token to int 
    } else {
        Serial.println(" --- Error: Could not get temperature\r\n");
        incomingTempT = 0;
        return;
    }
    // Second token (humidity)
    token = strtok(NULL, ";");
    if (token != NULL) {
        incomingHumidity = atof(token); // Convert token to int 
    } else {
        Serial.println(" --- Error: Could not get humidity\r\n");
        incomingHumidity = 0;
        return;
    }
}
