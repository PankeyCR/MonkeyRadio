
#include "LoRaNetwork.hpp"
#include "SerialListenerState.hpp"
#include "SerialDelivery.hpp"

using namespace ame;

LoRaNetwork* lora;
SerialListenerState<Note>* listener;

void setup() {
  Serial.begin(9600);
  
  lora = new LoRaNetwork();
  
  listener = new SerialListenerState<Note>();
  listener->addListener(LoRaMessage);

  lora->setDelivery(new SerialDelivery(listener));

  lora->initializeState();
}

void loop() {
  ame_Debuging(ame_Log_StartLoop, "loop");
  lora->updateState(0);
  listener->updateState(0);
  ame_Debuging(ame_Log_EndLoop, "loop");
}

void LoRaMessage(Note* message){
  
}


//