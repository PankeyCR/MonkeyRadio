
#ifndef LoRaCommands_hpp
#define LoRaCommands_hpp
#define LoRaCommands_AVAILABLE

#include "Note.hpp"
#include "LoRaNetwork.hpp"
#include "MemoryRam.hpp"
#include "System.hpp"

#ifdef LoRaCommands_LogApp
	#include "ame_Logger_config.hpp"
	#include "ame_Logger.hpp"
	
	#define LoRaCommandsLog(location,method,type,mns) ame_Log(this,location,"LoRaCommands",method,type,mns)
	#define const_LoRaCommandsLog(location,method,type,mns) 
	#define StaticLoRaCommandsLog(pointer,location,method,type,mns) ame_Log(pointer,location,"LoRaCommands",method,type,mns)
#else
	#ifdef LoRaCommands_LogDebugApp
		#include "ame_Logger_config.hpp"
		#include "ame_Logger.hpp"
		
		#define LoRaCommandsLog(location,method,type,mns) ame_LogDebug(this,location,"LoRaCommands",method,type)
		#define const_LoRaCommandsLog(location,method,type,mns) 
		#define StaticLoRaCommandsLog(pointer,location,method,type,mns) ame_LogDebug(pointer,location,"LoRaCommands",method,type)
	#else
		#define LoRaCommandsLog(location,method,type,mns) 
		#define const_LoRaCommandsLog(location,method,type,mns) 
		#define StaticLoRaCommandsLog(pointer,location,method,type,mns) 
	#endif
#endif

namespace ame{

LoRaNetwork* g_loraState = nullptr;

void setLoRaState(LoRaNetwork* a_state){
	g_loraState = a_state;
}

void LoRaCommands(Note* message){
	if(message == nullptr || g_loraState == nullptr) {
		System::console.println("message == nullptr || g_loraState == nullptr");
		return;
	}
	
	System::console.println("LoRaCommands");
	
	PrimitiveList<Note> commands = splitNote(*message, ' ');

	if(commands.getPosition() == 1){
		Note command_0 = commands[0];

		if (command_0 == "getLoraSendingMessages") {
			for(int x = 0; x < g_loraState->m_send.getPosition(); x++){
				RadioSendingMessage* f_message = g_loraState->m_send.getByPosition(x);
				Note f_sending;
				f_sending += '<';
				f_sending += f_message->uuid;
				f_sending += '.';
				f_sending += f_message->command;
				f_sending += '.';
				f_sending += f_message->ip;
				f_sending += '.';
				f_sending += f_message->message;
				f_sending += '>';
				//g_serialState->instantSend(f_sending);
			}
		}
	}

	if(commands.getPosition() == 2){
		Note command_0 = commands[0];
		Note command_1 = commands[1];
	}

	if(commands.getPosition() == 5){
		Note command_0 = commands[0];
		Note command_1 = commands[1];
		Note command_2 = commands[2];
		Note command_3 = commands[3];
		Note command_4 = commands[4];

		if(command_1.length() == 1 || command_2.length() == 1 || command_3.length() == 1){
			if (command_0 == "sendLora") {
				g_loraState->sendRadio(command_1[0], command_2[0], command_3[0], command_4);
			}
		}
	}
/*
	if (mns == "get send message") {
		Serial.print("m_send_message: ");
		for(int x = 0; x < g_loraState->m_send.getPosition(); x++){
			// Serial.print(*g_loraState->m_send_message.getByPosition(x));
			// Serial.print(" - ");
		}
			Serial.println();
	}

	if (mns == "reset ip message") {
		Serial.println("resetdelte lists: ");
		g_loraState->m_send.resetDelete();
	}


	if (mns == "get randomTime") {
		Serial.print("randomTime: ");
		Serial.println(g_loraState->randomTime);
	}

	if (mns == "get send time") {
		Serial.print("m_send_time: ");
		Serial.println(g_loraState->m_send_time);
	}

	if (mns == "get time limit") {
		Serial.print("m_time_limit_resend: ");
		Serial.println(g_loraState->m_time_limit_resend);
	}

	if (mns == "get start char") {
		Serial.print("m_start_char: ");
		Serial.println(g_loraState->m_start_char);
	}

	if (mns == "get end char") {
		Serial.print("m_end_char: ");
		Serial.println(g_loraState->m_end_char);
	}

	if (mns == "get send command") {
		Serial.print("m_send_message_commnad: ");
		Serial.println(g_loraState->m_send_message_commnad);
	}

	if (mns == "get confirmation") {
		Serial.print("m_send_message_confirmation: ");
		Serial.println(g_loraState->m_send_message_confirmation);
	}

	if (mns == "get readyToSend") {
		Serial.print("m_readyToSend: ");
		Serial.println(g_loraState->m_readyToSend);
	}

	if (mns == "set readyToSend high") {
		Serial.println("m_readyToSend high: ");
		g_loraState->m_readyToSend = true;
	}

	if (mns == "set readyToSend low") {
		Serial.println("m_readyToSend low: ");
		g_loraState->m_readyToSend = false;
	}

	if (mns == "get sendingsTry") {
		Serial.print("m_sendingsTry: ");
		Serial.println(g_loraState->m_sendingsTry);
	}

	if (mns == "reset sendingsTry") {
		Serial.println("reseting: ");
		g_loraState->m_sendingsTry = 0;
	}*/
}

}

#endif