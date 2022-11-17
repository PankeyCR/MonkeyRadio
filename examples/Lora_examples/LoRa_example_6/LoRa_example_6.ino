
#include "ame_Enviroment.hpp"
#include "LoRaNetwork.hpp"

using namespace ame;

void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println("//////////////////////////////////////////start");
  Note message = createRadioMessage('<','$','*','0','1',"hola",'>');//<$*401hola>
  Serial.println(message);
  
  if(!isRadioMessage('<','>',message)){
    return;
  }
  
  Serial.println("is a radio message");
  
  RadioMessage radio = getRadioMessage(message);
  Serial.println("uuid");
  Serial.println(radio.uuid);
  Serial.println("command");
  Serial.println(radio.command);
  Serial.println("size");
  Serial.println(radio.size);
  Serial.println("sender_ip");
  Serial.println(radio.sender_ip);
  Serial.println("reciever_ip");
  Serial.println(radio.reciever_ip);
  if(radio.message != nullptr){
    Serial.println("message");
    Serial.println(*radio.message);
  }
  if(radio.uuid == '$' && radio.command == '*' && radio.size == 4 && radio.sender_ip == '0' && radio.reciever_ip == '1'){
    Serial.println("all good");
  }
  Serial.println("//////////////////////////////////////////end");
}




//