
#include "ame_Enviroment.hpp"
#include "LoRaNetwork.hpp"

using namespace ame;

void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println("//////////////////////////////////////////start");
  Note responce = createRadioResponce('<','$','/','>');//<$/>
  Serial.println(responce);
  
  if(!isRadioResponce('/','<','>',responce)){
    return;
  }
  
  Serial.println("is a radio responce");
  
  RadioResponce radio = getRadioResponce(responce);
  Serial.println("uuid");
  Serial.println(radio.uuid);
  Serial.println("command");
  Serial.println(radio.command);
  if(radio.uuid == '$' && radio.command == '/'){
    Serial.println("all good");
  }
  Serial.println("//////////////////////////////////////////end");
}




//