
#ifndef RadioNetwork_hpp
#define RadioNetwork_hpp
#define RadioNetwork_AVAILABLE

#define radio_UDP '%'
#define radio_TCP '&'

#include "SerialNetwork.hpp"
#include "Class.hpp"
#include "Note.hpp"
#include "ByteArray.hpp"
#include "MessageDelivery.hpp"
#include "LinkedList.hpp"

#include <SPI.h>
#include <LoRa.h>

#ifdef RadioNetwork_LogApp
	#include "ame_Logger_config.hpp"
	#include "ame_Logger.hpp"
	
	#define RadioMethodLog(location,method,type,mns) ame_Log(nullptr,location,"RadioNetwork",method,type,mns)
	#define RadioNetworkLog(location,method,type,mns) ame_Log(this,location,"RadioNetwork",method,type,mns)
	#define const_RadioNetworkLog(location,method,type,mns) 
	#define StaticRadioNetworkLog(pointer,location,method,type,mns) ame_Log(pointer,location,"RadioNetwork",method,type,mns)
#else
	#ifdef RadioNetwork_LogDebugApp
		#include "ame_Logger_config.hpp"
		#include "ame_Logger.hpp"
		
		#define RadioMethodLog(location,method,type,mns) ame_LogDebug(nullptr,location,"RadioNetwork",method,type)
		#define RadioNetworkLog(location,method,type,mns) ame_LogDebug(this,location,"RadioNetwork",method,type)
		#define const_RadioNetworkLog(location,method,type,mns) 
		#define StaticRadioNetworkLog(pointer,location,method,type,mns) ame_LogDebug(pointer,location,"RadioNetwork",method,type)
	#else
		#define RadioMethodLog(location,method,type,mns) 
		#define RadioNetworkLog(location,method,type,mns) 
		#define const_RadioNetworkLog(location,method,type,mns) 
		#define StaticRadioNetworkLog(pointer,location,method,type,mns) 
	#endif
#endif

namespace ame{
	
	/*
	*	ip has 1 bytes
	*/
	
struct RadioSendingMessage{
	char uuid;
	char command;
	char ip;
	Note message;

	RadioSendingMessage(){}

	RadioSendingMessage(const RadioSendingMessage& c_message){
		uuid = c_message.uuid;
		command = c_message.command;
		ip = c_message.ip;
		message = c_message.message;
	}

	void operator=(const RadioSendingMessage& c_message){
		uuid = c_message.uuid;
		command = c_message.command;
		ip = c_message.ip;
		message = c_message.message;
	}

	bool operator==(const RadioSendingMessage& c_message){
		return 	command == c_message.command &&
				uuid == c_message.uuid &&
				ip == c_message.ip &&
				message == c_message.message;
	}

	bool operator!=(const RadioSendingMessage& c_message){
		return 	command != c_message.command &&
				uuid != c_message.uuid &&
				ip != c_message.ip &&
				message != c_message.message;
	}
};
	
struct RadioMessage{
	char uuid;
	char command;
	char size;
	char sender_ip;
	char reciever_ip;
	Note* message = nullptr;
};

Note createRadioMessage(char a_start, char a_uuid, char a_command, char a_sender_ip, char a_reciever_ip, Note a_message, char a_end){// <$*412hola>
	RadioMethodLog(ame_Log_StartMethod, "createRadioMessage", "println", "");
	
	Note i_length = Note((char)a_message.length());
	Note i_note = Note(a_start) + Note(a_uuid) + Note(a_command) + i_length + Note(a_sender_ip) + Note(a_reciever_ip) + a_message + Note(a_end);
	
	RadioMethodLog(ame_Log_EndMethod, "createRadioMessage", "println", "");
	return i_note;
}

bool isRadioMessage(char a_start, char a_end, Note a_message){//<$*112x>
	RadioMethodLog(ame_Log_StartMethod, "isRadioMessage", "println", "");
	if(a_message.isEmpty()){
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "a_message.isEmpty()");
		return false;
	}
	if(a_message.length() < 8){
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "a_message.length() < 8");
		return false;
	}
	if(a_message.charAt(0) != a_start && a_message.charAt(a_message.length() - 1) != a_end){
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "a_message.charAt(0) != a_start && a_message.charAt(a_message.length() - 1) != a_end");
		return false;
	}
	
	int size = a_message[3];
	
	if(size + 7 != a_message.length()){
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "size + 7 ");
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", size + 7);
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "a_message.length()");
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", a_message.length());
		return false;
	}
	
	RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "");
	return true;
}

RadioMessage getRadioMessage(Note a_message){//<$*112x>
	RadioMethodLog(ame_Log_StartMethod, "getRadioMessage", "println", "");
	RadioMessage i_message;
	
	i_message.uuid = a_message[1];
	i_message.command = a_message[2];
	i_message.size = a_message[3];
	i_message.sender_ip = a_message[4];
	i_message.reciever_ip = a_message[5];
	
	i_message.message = new Note();
	
	for(int x = 6; x < a_message.length() - 1; x++){
		char f_char = a_message.charAt(x);
		*i_message.message += f_char;
	}
	
	RadioMethodLog(ame_Log_EndMethod, "getRadioMessage", "println", "");
	return i_message;
}
	
struct RadioResponce{
	char uuid;
	char command;
	char ip_origin;
};

Note createRadioResponce(char a_start, char a_uuid, char a_command, char a_ip_origin, char a_end){//<$/1>
	RadioMethodLog(ame_Log_StartMethod, "createRadioResponce", "println", "");
	
	Note i_note = Note(a_start) + Note(a_uuid) + Note(a_command) + Note(a_ip_origin) + Note(a_end);
	
	RadioMethodLog(ame_Log_EndMethod, "createRadioResponce", "println", "");
	return i_note;
}

bool isRadioResponce(char a_uuid, char a_command, char a_ip_origin, char a_start, char a_end, Note a_message){//<$/1>
	RadioMethodLog(ame_Log_StartMethod, "isRadioResponce", "println", "");
	if(a_message.isEmpty()){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_message.isEmpty()");
		return false;
	}
	if(a_message.length() != 5){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_message.length() < 8");
		return false;
	}
	if(a_message.charAt(0) != a_start && a_message.charAt(4) != a_end){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_message.charAt(0) != a_start && a_message.charAt(a_message.length() - 1) != a_end");
		return false;
	}
	
	int i_uuid = a_message[1];
	int i_command = a_message[2];
	int i_ip = a_message[3];
	
	if(i_command != a_command || i_uuid != a_uuid || i_ip != a_ip_origin){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "i_command");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", i_command);
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_command");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", a_command);
		
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "i_uuid");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", i_uuid);
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_uuid");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", a_uuid);
		
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "i_ip");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", i_ip);
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_ip_origin");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", a_ip_origin);
		return false;
	}
	
	RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "");
	return true;
}

RadioResponce getRadioResponce(Note a_message){//<$/1>
	RadioMethodLog(ame_Log_StartMethod, "getRadioResponce", "println", "");
	RadioResponce i_responce;
	
	i_responce.uuid = a_message[1];
	i_responce.command = a_message[2];
	i_responce.ip_origin = a_message[3];
	
	RadioMethodLog(ame_Log_EndMethod, "getRadioResponce", "println", "");
	return i_responce;
}

class RadioNetwork : public SerialNetwork {	
    public:
		RadioNetwork(){
			RadioNetworkLog(ame_Log_StartMethod, "Constructor", "println", "");
			RadioNetworkLog(ame_Log_EndMethod, "Constructor", "println", "");
		}
		virtual ~RadioNetwork(){
			RadioNetworkLog(ame_Log_StartMethod, "Destructor", "println", "");
			RadioNetworkLog(ame_Log_EndMethod, "Destructor", "println", "");
		}
		
		virtual void setLoraIp(char a_ip){
			RadioNetworkLog(ame_Log_StartMethod, "setLoraIp", "println", "");
			m_lora_ip = a_ip;
			RadioNetworkLog(ame_Log_EndMethod, "setLoraIp", "println", "");
		}

		virtual void sendRadio(char a_uuid, char a_ip, Note a_message){
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "");
			
			this->sendRadio(a_uuid, radio_UDP, a_ip, a_message);

			RadioNetworkLog(ame_Log_EndMethod, "sendRadio", "println", "");
		}

		virtual void sendRadio(char a_uuid, int a_command, char a_ip, Note a_message){
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "");

			RadioSendingMessage i_message;

			i_message.uuid = a_uuid;
			i_message.command = a_command;
			i_message.ip = a_ip;
			i_message.message = a_message;
			
			this->m_send.add(i_message);
			
			RadioNetworkLog(ame_Log_EndMethod, "sendRadio", "println", "");
		}

		virtual void sendRadio(RadioSendingMessage a_message){
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "");
			
			this->m_send.add(a_message);

			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "uuid");
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", a_message.uuid);
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "a_command");
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", a_message.command);
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "ip");
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", a_message.ip);
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", "message");
			RadioNetworkLog(ame_Log_StartMethod, "sendRadio", "println", a_message.message);
			RadioNetworkLog(ame_Log_EndMethod, "sendRadio", "println", "");
		}

		virtual void setRandomTime(float a_random){
			RadioNetworkLog(ame_Log_StartMethod, "setRandomTime", "println", "");
			this->randomTime = a_random;
			RadioNetworkLog(ame_Log_EndMethod, "setRandomTime", "println", "");
		}
		
		virtual float checkTimeOut(float a_tpc){
			//RadioNetworkLog(ame_Log_StartMethod, "checkTimeOut", "println", "");
			m_send_time += a_tpc;
			m_resend_time += a_tpc;
			if(!m_readyToSend && m_resend_time > m_time_limit_resend){
				m_readyToSend = true;
				if(m_sendingsTry > 4){
					/////////////////////////////////////////////////////////////////////ERROR
					RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", "m_sendingsTry > 4");
					m_send.removeDeleteByPosition(0);

					m_sendingsTry = 0;
				}
			}
			//RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", "m_send_time");
			//RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", m_send_time);
			//RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", "");
			return m_send_time;
		}
		
		virtual Note* readRadio(){
			//RadioNetworkLog(ame_Log_StartMethod, "readRadio", "println", "");
			int packetSize = LoRa.parsePacket();
			if (packetSize) {
				RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", "packetSize");
				RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", packetSize);
				String LoRaData;
				while (LoRa.available()) {
					LoRaData = LoRa.readString();
				}
				RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", "return note");
				return new Note(LoRaData);
			}
			//RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", "return nullptr");
			return nullptr;
		}
		
		virtual bool isReadingRadio(){
			//RadioNetworkLog(ame_Log_StartMethod, "isReadingRadio", "println", "");
			//RadioNetworkLog(ame_Log_EndMethod, "isReadingRadio", "println", "");
			return false;
		}
		
		virtual void sendMessage(){//<c4xxxxhola>
			//RadioNetworkLog(ame_Log_StartMethod, "sendMessage", "println", "");
			m_send_time = 0.0f;

			if(m_send.isEmpty()){
				//RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "");
				return;
			}
			if(!m_readyToSend){
				return;
			}
			
			m_resend_time = 0;
			m_readyToSend = false;
			m_sendingsTry++;
			
			RadioSendingMessage i_sending_message = *m_send.getByPosition(0);
			
			char i_uuid = i_sending_message.uuid;
			char i_command = i_sending_message.command;
			char i_ip = i_sending_message.ip;
			Note i_mns = i_sending_message.message;
			
			
			Note i_mns_format = createRadioMessage(m_start_char, i_uuid, i_command, m_lora_ip, i_ip, i_mns, m_end_char);
			
			LoRa.beginPacket();
			LoRa.print(i_mns_format);
			LoRa.endPacket();
			
			if(i_command == radio_UDP){
				m_send.removeDeleteByPosition(0);
				m_readyToSend = true;
				m_sendingsTry = 0;
			}
			
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "i_mns");
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", i_mns);
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "i_ip");
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", i_ip);
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "i_uuid");
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", i_uuid);
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "i_mns_format");
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", i_mns_format);
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "");
		}
		
		virtual bool isMessageAConfirmation(Note* a_message){//<$/1>
			RadioNetworkLog(ame_Log_StartMethod, "isMessageAConfirmation", "println", "");
			if(a_message == nullptr || m_send.isEmpty()){
				RadioNetworkLog(ame_Log_EndMethod, "isMessageAConfirmation", "println", "a_message == nullptr");
				return false;
			}
			RadioSendingMessage i_sending_message = *m_send.getByPosition(0);
			char i_uuid = i_sending_message.uuid;

			if(!isRadioResponce(i_uuid, m_send_message_confirmation, m_lora_ip, m_start_char, m_end_char, *a_message)){//<$/1>
				return false;
			}
			
			m_send.removeDeleteByPosition(0);
			m_resend_time = 0;
			return true;
		}
		
		virtual bool isNewPort(Note* a_message){
			RadioNetworkLog(ame_Log_StartMethod, "isNewPort", "println", "");
			RadioNetworkLog(ame_Log_EndMethod, "isNewPort", "println", "");
			return false;
		}
		
		virtual bool sendConfirmation(char a_uuid, char a_ip_origin){
			RadioNetworkLog(ame_Log_StartMethod, "sendConfirmation", "println", "");
			
			Note i_mns_format = createRadioResponce(m_start_char, a_uuid, m_send_message_confirmation, a_ip_origin, m_end_char);
			
			LoRa.beginPacket();
			LoRa.print(i_mns_format);
			LoRa.endPacket();
			
			RadioNetworkLog(ame_Log_EndMethod, "sendConfirmation", "println", "i_mns_format");
			RadioNetworkLog(ame_Log_EndMethod, "sendConfirmation", "println", i_mns_format);
			RadioNetworkLog(ame_Log_EndMethod, "sendConfirmation", "println", "");
			return true;
		}

		virtual void resetSendingTime(){
			m_send_time = 0.0f;
		}
		
		virtual void updateState(float tpc){// <c4xxxxhola>		<cxxxx>
			float time = this->checkTimeOut(tpc);
			
			Note* u_message = this->readRadio();

			if(this->isReadingRadio()){
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "this->isReadingRadio()");
				return;
			}

			if(u_message == nullptr){
				if(randomTime < time){
					//RadioNetworkLog(ame_Log_Statement, "updateState", "println", "u_message == nullptr && randomTime < time");
					this->sendMessage();
				}
				return;
			}

			this->resetSendingTime();

			if(this->isMessageAConfirmation(u_message) || this->isNewPort(u_message)/* || this->isNewPort(u_message)*/){
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "this->isMessageAConfirmation(u_message) ||");
				m_readyToSend = true;
				delete u_message;
				return;
			}

			if(!isRadioMessage(m_start_char, m_end_char, *u_message)){
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "!isRadioMessage");
				return;
			}
			
			RadioMessage u_radio = getRadioMessage(*u_message);
			
			delete u_message;

			char u_uuid = u_radio.uuid;
			char u_sender_ip = u_radio.sender_ip;
			char u_reciever_ip = u_radio.reciever_ip;

			if(u_reciever_ip != m_lora_ip){
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "not my ip");
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "My ip");
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", m_lora_ip);
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "u_reciever_ip");
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", u_reciever_ip);
				return;
			}
			
			this->sendConfirmation(u_uuid, u_sender_ip);

			this->DeliverMessage(u_radio.message);
			
			RadioNetworkLog(ame_Log_Statement, "updateState", "println", "end update state");
		}

		virtual bool isReadyToSend(){
			return m_send.isEmpty();
		}
			
		#if defined(cppObject_AVAILABLE) && defined(cppObjectClass_AVAILABLE) && defined(Class_AVAILABLE)
		virtual cppObjectClass* getClass(){
			return Class<RadioNetwork>::getClass();
		}
		
		virtual bool instanceof(cppObjectClass* cls){
			return cls == Class<RadioNetwork>::getClass()
			#if defined(AppState_AVAILABLE)
			|| AppState::instanceof(cls)
			#endif
			;
		}
		#endif
		
	protected:
		virtual Note getByteIp(Note a_ip){
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "");
			if(!isIP(a_ip)){
				RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "!isIP(a_ip)");
				return "";
			}
			char ip_1 = toIPAddress(a_ip, 0);
			char ip_2 = toIPAddress(a_ip, 1);
			char ip_3 = toIPAddress(a_ip, 2);
			char ip_4 = toIPAddress(a_ip, 3);

			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "ip_1");
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", (byte)ip_1);
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "ip_2");
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", (byte)ip_2);
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "ip_3");
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", (byte)ip_3);
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "ip_4");
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", (byte)ip_4);

			Note i_ip = Note(ip_1) + Note(ip_2) + Note(ip_3) + Note(ip_4);

			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "i_ip");
			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", i_ip);

			RadioNetworkLog(ame_Log_EndMethod, "getByteIp", "println", "");
			return i_ip;
		}
		virtual Note getByteIp(){
			return this->getByteIp(this->getIP());
		}

	public:
		PrimitiveList<RadioSendingMessage> m_send;
		float randomTime = 0.01f;
		float m_send_time = 0.0f;
		float m_resend_time = 0.0f;
		float m_time_limit_resend = 1.0f;
		
		char m_start_char = '<';
		char m_end_char = '>';
		
		char m_send_message_confirmation = '/';
		
		bool m_readyToSend = true;
		int m_sendingsTry = 0;
		
		char m_lora_ip = '0';
};

}

#endif