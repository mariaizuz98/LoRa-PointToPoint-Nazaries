#include "package.h"

extern byte myBoardID;
extern SSD1306 display;
extern hw_timer_t *sendTimer;
extern hw_timer_t *responseTimer;
extern const uint8_t GATEWAY_ID;

byte senderID;          // sender address
byte receiverID;          // recipient address
byte incomingMsgID;     // incoming msg ID
String incomingMsg;     // data sensor
int incomingTempT , incomingHumidity = 0;
bool receiveMsg = false;
bool recieveACK = false;

void sendPackage(byte destID, byte msgID, char* msg){
    // send packet
    LoRa.beginPacket();
    LoRa.write(myBoardID);
    LoRa.write(destID);
    LoRa.write(msgID);
    LoRa.print(msg);  // send the high byte
    LoRa.endPacket();

    Serial.printf(" ··· Message LoRa send... \r\n");
    Serial.printf("Sender: 0x%2X |  Destination: 0x%2X  |  Message ID: %d  |  Message: %s  \r\n", 
                    myBoardID, destID, msgID, msg);
}

bool recievePackage (void){
    if(LoRa.parsePacket() != 0)     receiveMsg = true;

    return receiveMsg;
}

void readPackage(void){
    // read packet header bytes:
    senderID = LoRa.read();                             // sender address
    receiverID = LoRa.read();                           // recipient/gateway address
    incomingMsgID = LoRa.read();                        // incoming msg ID
    incomingMsg = "";

    while (LoRa.available()) {
        incomingMsg += (char)LoRa.read();
    }
    if(receiverID == myBoardID){
        Serial.println(" ··· Message LoRa receive...");
        Serial.printf("Sender: 0x%2X |  Destination: 0x%2X  |  Message ID: %d  |  Message: %s  |  RSSI:  %d  | SNR:  %.2f  \r\n", 
                        senderID, receiverID, incomingMsgID, incomingMsg, LoRa.packetRssi(), LoRa.packetSnr());
        identifyActionLoRa(incomingMsgID);
    }
}

void identifyActionLoRa(byte msgID){
    switch (msgID){
        case DATA:
            if(receiverID == myBoardID){
                sendPackage(senderID, ACK, "");
                #ifdef GATEWAY_LORA
                    sendDataToCloud();
                #endif
            }
            break;
        case ACK:
            if(receiverID == myBoardID){
                recieveACK = true;
            }
            break;
        default:
            break;
    }
}
void sendDataToCloud(void){
    parseSensorData(strdup(incomingMsg.c_str()));
    if (WiFi.status() == WL_CONNECTED) {
        // JSON creation
        newJSON(incomingTempT, incomingHumidity);
        // Encryption data
        encryptData();
        // Post Function
        SendPost();
        delay(500);
    } else {
        Serial.println("");
        Serial.println("Failed to connect!");
        setupWiFi();
    }
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
