
#define TIEMPO_LIMITE 3.0f
#define LoRa_IP 'S'
#define RANDOM_SENDER 0.5f

#include "DefaultApplication.hpp"

#include "LoRaNetwork.hpp"
#include "SerialListenerState.hpp"
#include "SerialDelivery.hpp"

#include "ame_LoRa.hpp"

#include "LoRaCommands.hpp"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

using namespace ame;

Application* app;
LoRaNetwork* lora;
SerialListenerState<Note>* listener;
float tiempo = 0;
Note cliente_1;
Note cliente_2;

void setup() {
  Serial.begin(9600);
  pinMode(OLED_RST, OUTPUT);

  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA RECEIVER ");
  display.display();
  
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

  if (tiempo > TIEMPO_LIMITE) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("tiempo sin respuesta");
    display.setCursor(0, 20);
    display.setTextSize(2);
    display.print(tiempo);
    display.display();
  }else{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print(cliente_1);
    display.setCursor(0, 20);
    display.setTextSize(2);
    display.print(cliente_2);
    display.display();
  }
  ame_Debuging(ame_Log_EndLoop, "loop");
}

void LoRaMessage(Note* message) {
  tiempo = 0;

  char port = message->get(0);
  message->remove(0,3);

  if(port == 'A'){
    message->insertLocalArrayPointer(0, "C2-");
    cliente_1 = *message;
  }
  if(port == 'B'){
    message->insertLocalArrayPointer(0, "C3-");
    cliente_2 = *message;
  }
  Serial.println("LoRaMessage");
  Serial.println(*message);

}


//