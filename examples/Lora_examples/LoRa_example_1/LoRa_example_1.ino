
#include "DefaultApplication.hpp"
#include "SerialPost.hpp"
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
SerialPost* post;

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

  post = app->getStateManager()->addState(new SerialPost());
  
  LoRaNetwork* lora = post->addNetwork("LoRa", new LoRaNetwork());
  SerialState* serialState = post->addNetwork("Serial", new SerialState());

  serialState->setDelivery(new SerialDelivery());

//  lora->addSerialServer(new LoraSerialServer(55), new DefaultServerProtocol());
//  lora->addSerialServer(new EthernetSerialServer(55), new DefaultServerProtocol());
  serialState->addSerialServer(new WIFISerialServer(55), new DefaultServerProtocol<DefaultPortProtocol>());
  
  post->addDelivery("LoRa", LoRaMessage);
  post->addDelivery("Serial", LoRaMessage);
  post->send("tuanis");
}

void loop() {
  ame_Debuging(ame_Log_StartLoop, "loop");
  app->update();
  ame_Debuging(ame_Log_EndLoop, "loop");
}

void LoRaMessage(Message* message){
  Note text = message->text();
  System::console.println("LoRaMessage");
  System::console.println(text);
  if(text == "tuanis"){
    post->send("Su Madre");
  }
  if(text == "Su Madre"){
    post->send("La tuya");
  }
  if(text == "La tuya"){
    post->send("tomela");
  }
  if(text == "tomela"){
    post->send("tuanis");
  }
}