
#ifndef CONFIGURATION_ame_LoRa_hpp
#define CONFIGURATION_ame_LoRa_hpp

	#include "ame_Enviroment.hpp"

	#if defined(DISABLE_ame_LoRa)
		#define ame_LoRa_hpp
	#endif
#endif

#ifndef ame_LoRa_hpp
#define ame_LoRa_hpp
#define ame_LoRa_AVAILABLE 

#include "Note.hpp"
#include "NoteHelper.hpp"

#ifdef ame_ArduinoIDE
	#include "Arduino.h"
#endif

#include <SPI.h>
#include <LoRa.h>

#ifdef ame_LoRa_LogApp
	#include "ame_Logger_config.hpp"
	#include "ame_Logger.hpp"
	
	#define ame_LoRaLog(location,method,type,mns) ame_Log(nullptr,location,"ame_LoRa",method,type,mns)
#else
	#ifdef ame_LoRa_LogDebugApp
		#include "ame_Logger_config.hpp"
		#include "ame_Logger.hpp"
		
		#define ame_LoRaLog(location,method,type,mns) ame_LogDebug(nullptr,location,"ame_LoRa",method,type)
	#else
		#define ame_LoRaLog(location,method,type,mns) 
	#endif
#endif

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define CR_BAND 915E6
#define A_BAND 433E6
#define EU_BAND 866E6
#define NA_BAND 915E6

namespace ame{

bool g_lora_lan = false;
Note g_lora_name;
Note g_lora_pass;
Note g_lora_ip = "100.100.1.1";

#ifdef ame_ArduinoIDE

void initializeLoRaHardware(Note a_ip, int a_sck, int a_miso, int a_mosi, int a_ss, int a_rst, int a_dio0, Note a_name, Note a_password, long a_frequency){
	ame_LoRaLog(ame_Log_StartMethod, "initializeLoRa",  "println", "");
	
	g_lora_name = a_name;
	g_lora_pass = a_password;
	g_lora_ip = a_ip;

	SPI.begin(a_sck, a_miso, a_mosi, a_ss);
	LoRa.setPins(a_ss, a_rst, a_dio0);

	if (!LoRa.begin(a_frequency)) {
		ame_LoRaLog(ame_Log_StartMethod, "haltUntilLoraConnection",  "println", "Starting LoRa failed!");
	}else{
		g_lora_lan = true;
	}
	ame_LoRaLog(ame_Log_EndMethod, "initializeLoRa",  "println", "");
}

void initializeLoRa(Note a_ip, Note a_name, Note a_password, long a_frequency){
	
  	int i_sck = 5;
  	int i_miso = 19;
  	int i_mosi = 27;
  	int i_ss = 18;
  	int i_rst = 14;
  	int i_dio0 = 26;

	initializeLoRaHardware(a_ip, i_sck, i_miso, i_mosi, i_ss, i_rst, i_dio0, a_name, a_password, a_frequency);
}

void initializeLoRa(long a_frequency){
	Note i_ip = "100.100.1.1";
	Note i_name = "free";
	Note i_password = "free";
	
  	int i_sck = 5;
  	int i_miso = 19;
  	int i_mosi = 27;
  	int i_ss = 18;
  	int i_rst = 14;
  	int i_dio0 = 26;

	initializeLoRaHardware(i_ip, i_sck, i_miso, i_mosi, i_ss, i_rst, i_dio0, i_name, i_password, a_frequency);
}

void haltUntilLoRaConnection(){
	ame_LoRaLog(ame_Log_StartMethod, "haltUntilLoraConnection",  "println", "");
	while (!g_lora_lan) {
		ame_LoRaLog(ame_Log_StartMethod, "haltUntilLoraConnection",  "println", ".");
		delay(1000);
	}
	ame_LoRaLog(ame_Log_EndMethod, "haltUntilLoraConnection",  "println", "");
}

bool hasLoRaLAN(){
	return g_lora_lan;
}

void printLoRa(Stream* port){
	if(port == nullptr){
		return;
	}
	if(hasLoRaLAN()){
		port->println("LoRa connected");
	}else{
		port->println("LoRa disconnected");
	}
}
#endif

}

#endif
