
//#define CAMION_C2
#define CAMION_C3
//#define CAMION_T3S3

#define TIEMPO_LIMITE 0.3f

#define LoRa_SERVER_IP '0'

#define LoRa_UUID '$'

#ifdef CAMION_C2
  #define LoRa_IP 'A'
  #define LoRa_CLIENT_IP '1'
  #define CORRECCION_X 0.01f
  #define CORRECCION_Y 0.015f
  #define CORRECCION_Z 0.02f
  #define RADIO 0.75f /2.0f
  #define RANDOM_SENDER 0.5f
#endif

#ifdef CAMION_C3
  #define LoRa_IP 'B'
  #define LoRa_CLIENT_IP '2'
  #define CORRECCION_X 0.28f
  #define CORRECCION_Y 0.01f
  #define CORRECCION_Z 0.01f
  #define RADIO 1.02f /2.0f
  #define RANDOM_SENDER 0.1f
#endif

#ifdef CAMION_T3S3
  #define LoRa_IP 'C'
  #define LoRa_CLIENT_IP '3'
  #define CORRECCION_X 0.08f
  #define CORRECCION_Y 0.04f
  #define CORRECCION_Z 0.01f
  #define RADIO 1.02f /2.0f
  #define RANDOM_SENDER 0.15f
#endif

#include "DefaultApplication.hpp"

#include "LoRaNetwork.hpp"
#include "SerialListenerState.hpp"
#include "SerialDelivery.hpp"

#include "ame_LoRa.hpp"

#include "LoRaCommands.hpp"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
float tiempo = 0.0f;

using namespace ame;

Application* app;
LoRaNetwork* lora;
SerialListenerState<Note>* listener;

void setup() {
  Serial.begin(9600);
  
  mpu.begin();
  
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);

  initializeLoRa( CR_BAND );  //BAND: CR_BAND EU_BAND

  haltUntilLoRaConnection();

  app = new DefaultApplication();

  lora = app->getStateManager()->addState(new LoRaNetwork());
  setLoRaState(lora);

  lora->setLoraIp(LoRa_IP);
  lora->setRandomTime(RANDOM_SENDER);

  listener = app->getStateManager()->addState(new SerialListenerState<Note>());
  listener->addListener(LoRaMessage);
  listener->addListener(LoRaCommands);

  SerialDelivery* delivery = new SerialDelivery(listener);
  lora->setDelivery(delivery);
}

void loop() {
  ame_Debuging(ame_Log_StartLoop, "loop");
  app->update();

  float tpc = app->getStateManager()->tpc();

  tiempo += tpc;

  if(tiempo > TIEMPO_LIMITE && lora->isReadyToSend()){
    tiempo = 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    float giro_x = g.gyro.x + CORRECCION_X;
    float giro_y = g.gyro.y + CORRECCION_Y;
    float giro_z = g.gyro.z + CORRECCION_Z;
    
    //calculo de la magnitud de la velocidad angular
    float giro = sqrt((giro_x * giro_x) + (giro_y * giro_y) + (giro_z * giro_z));
    
    //calcula la velocidad lineal como un producto de la velocidad angular* el radio de la rueda delantera
    //el 3.6 corresponde a la conversion de velocidad en m/s a km/h
    float velocidad_km_h = giro * RADIO * 3.6;

    if(velocidad_km_h < 0.05f){
      return;
    }
  
    Note velocidad = Note("v.") + Note(abs(velocidad_km_h));
  
    lora->sendRadio(LoRa_UUID, radio_TCP, LoRa_CLIENT_IP, velocidad);
    lora->sendRadio(LoRa_UUID, radio_UDP, LoRa_SERVER_IP, velocidad);

    Serial.println(velocidad);
    ame_Debuging(ame_Log_Statement, "sending message");
  }
  
  ame_Debuging(ame_Log_EndLoop, "loop");
}

void LoRaMessage(Note* message) {
  Note mns = *message;
  Serial.println("LoRaMessage");
  Serial.println(mns);
  if (mns == "sendLoraStart") {
    Serial.println("sendind to lora network");
    lora->sendRadio(LoRa_UUID, LoRa_CLIENT_IP, "lora send");
  }
}


//