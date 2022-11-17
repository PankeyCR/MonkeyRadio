
#define WIFI_IP "192.168.1.150"
#define WIFI_MAC { 0x10, 0x84, 0x2C, 0x80, 0, 2 }
#define LoRa_IP '1'
#define LoRa_CLIENT_IP '2'
#define LoRa_UUID '$'

//#define WIFI_IP "192.168.1.151"
//#define WIFI_MAC { 0x10, 0x84, 0x2C, 0x80, 0, 3 }
//#define LoRa_IP '2'
//#define LoRa_CLIENT_IP '1'
//#define LoRa_UUID '-'

#include "SerialState.hpp"

#include "WIFISerialServer.hpp"
#include "LoRaSerialServer.hpp"

#include "DefaultSerialPort.hpp"
#include "WIFISerialPort.hpp"
#include "LoRaSerialPort.hpp"

#include "DefaultServerProtocol.hpp"
#include "DefaultPortProtocol.hpp"

#include "LoRaNetwork.hpp"
#include "SerialListenerState.hpp"
#include "SerialDelivery.hpp"

#include "ame_WIFI.hpp"
#include "ame_LoRa.hpp"

#include "LoRaCommands.hpp"

using namespace ame;

LoRaNetwork* lora;
SerialState* serialState;
SerialListenerState<Note>* listener;

long prevNow = 0;

void setup() {
  Serial.begin(9600);

  uint8_t mac[6] = WIFI_MAC;

  initializeWIFI( WIFI_IP,                //ip
                  "192.168.1.1" ,         //gateway
                  "192.168.1.1" ,         //subnet
                  "255, 255, 255, 0" ,    //dns
                  mac ,                   //mac
                  "pokemon" ,       //router name
                  "tania1919");            //router password

                  //router name MERCUSYS_6541 - pokemon
                  //router password 57924106 - tania1919

  haltUntilWIFIConnection();

  initializeLoRa( CR_BAND );  //BAND

  haltUntilLoRaConnection();

  serialState = new SerialState();

  serialState->setIP(WIFI_IP);
  serialState->addSerialServer(new WIFISerialServer(55), new DefaultServerProtocol<DefaultPortProtocol>());
  serialState->addSerialPort(new DefaultSerialPort(&Serial, "usb"), new DefaultPortProtocol());

  lora = new LoRaNetwork();
  setLoRaState(lora);

  lora->setLoraIp(LoRa_IP);
  lora->setRandomTime(0.01f);

  listener = new SerialListenerState<Note>();
  listener->addListener(LoRaMessage);
  listener->addListener(LoRaCommands);

  SerialDelivery* delivery = new SerialDelivery(listener);

  serialState->setDelivery(delivery);
  lora->setDelivery(delivery);

  serialState->initializeState();
  lora->initializeState();
}

void loop() {
  ame_Debuging(ame_Log_StartLoop, "loop");
  long Now = millis();
  float l_time = ((float)(Now - prevNow)) / 1000.0f;
  serialState->updateState(l_time);
  lora->updateState(l_time);
  listener->updateState(l_time);
  prevNow = Now;
  ame_Debuging(ame_Log_EndLoop, "loop");
}

void LoRaMessage(Note* message) {
  if(message == nullptr){
    Serial.println("message == nullptr");
    return;
  }
  Note mns = *message;
  Serial.println("LoRaMessage");
  Serial.println(mns);
  if (mns == "sendLoraStart") {
    Serial.println("sendind to lora network");
    lora->sendRadioByIp(LoRa_UUID, LoRa_CLIENT_IP, "lora send");
  }
  if (mns == "lora send") {
    Serial.println("broadcast ethernet");
    serialState->instantSend("Mensaje lora recivido");
  }
}


//