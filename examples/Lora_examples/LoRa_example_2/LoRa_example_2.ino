
#include "DefaultApplication.hpp"
#include "LoRaNetwork.hpp"
#include "SerialState.hpp"

//#include "EthernetSerialServer.hpp"
#include "WIFISerialServer.hpp"
#include "DefaultServerProtocol.hpp"
#include "DefaultPortProtocol.hpp"
#include "SerialDelivery.hpp"
#include "Note.hpp"
#include "System.hpp"

#include "ame_WIFI.hpp"

using namespace ame;

Application* app;

void setup() {
  Serial.begin(9600);

  initializeWIFI( "192.168.1.150",        //ip
                  "192.168.1.1" ,         //gateway
                  "192.168.1.1" ,         //subnet
                  "255, 255, 255, 0" ,    //dns
                  "MERCUSYS_6541" ,       //router name
                  "57924106");            //router password

  haltUntilWIFIConnection();
                  
  app = new DefaultApplication();
  
  LoRaNetwork* lora = app->getStateManager()->addState(new LoRaNetwork());
  SerialState* serialState = app->getStateManager()->addState(new SerialState());

  serialState->setDelivery(new SerialDelivery());

//  lora->addSerialServer(new LoraSerialServer(55), new DefaultServerProtocol());
//  lora->addSerialServer(new EthernetSerialServer(55), new DefaultServerProtocol());
  serialState->addSerialServer(new WIFISerialServer(55), new DefaultServerProtocol<DefaultPortProtocol>());
  
  SerialListenerState<Note>* listener = app->getStateManager()->addState(new SerialListenerState<Note>());
  listener->addListener(LoRaMessage);
}

void loop() {
  ame_Debuging(ame_Log_StartLoop, "loop");
  app->update();
  ame_Debuging(ame_Log_EndLoop, "loop");
}

void LoRaMessage(Note* message){
}