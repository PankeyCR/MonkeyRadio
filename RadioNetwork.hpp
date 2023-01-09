
#ifndef RadioNetwork_hpp
#define RadioNetwork_hpp
#define RadioNetwork_AVAILABLE

#define radio_NULL '\0'	//

#define radio_UDP 'A'	//port to port udp message
#define radio_UDPB 'B'	//no confirmation broadcast

#define radio_TCP 'C'	//port to port tcp message
#define radio_TCPB 'D'	//port to port tcp message and broadcast

#define radio_SC 'E'	//port to port single confirmation
#define radio_SCB 'F'	//port to port single confirmation and broadcast

#define radio_NRSC 'G'	//port to port single confirmation with no responce
#define radio_NRSCB 'H'	//port to port single confirmation and broadcast with no responce

#define radio_confirmation_TCP 'P'
#define radio_confirmation_TCPB 'Q'

#define radio_confirmation_SC 'R'
#define radio_confirmation_SCB 'S'

#define radio_confirmation_SCNR 'T'
#define radio_confirmation_SCNRB 'U'

#include "SerialNetwork.hpp"
#include "Class.hpp"
#include "Note.hpp"
#include "NoteHelper.hpp"
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
	
	Note i_length = Note((char)a_message.getPosition());
	
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "length");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_message.getPosition());
	
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "note_length");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", i_length);

	Note i_message_format = Note(a_start) + Note(a_uuid) + Note(a_command) + i_length + Note(a_sender_ip) + Note(a_reciever_ip) + a_message + Note(a_end);
	
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_start");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_start);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_uuid");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_uuid);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_command");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_command);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_sender_ip");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_sender_ip);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_reciever_ip");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_reciever_ip);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_message");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_message);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "a_end");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", a_end);
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", "i_message_format");
	RadioMethodLog(ame_Log_Statement, "createRadioMessage", "println", i_message_format);
	RadioMethodLog(ame_Log_EndMethod, "createRadioMessage", "println", "");
	return i_message_format;
}

bool isRadioMessage(char a_start, Note a_message, char a_end){//<$*112x>
	RadioMethodLog(ame_Log_StartMethod, "isRadioMessage", "println", "");
	if(a_message.isEmpty()){
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", "a_message.isEmpty()");
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "return false");
		return false;
	}
	if(a_message.getPosition() < 8){
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", "a_message.length() < 8");
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "return false");
		return false;
	}
	if(a_message.get(0) != a_start && a_message.getLastChar() != a_end){
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", "a_message.charAt(0) != a_start && a_message.charAt(a_message.length() - 1) != a_end");
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "return false");
		return false;
	}
	
	int i_size = a_message[3];
	RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", "encoded size:");
	RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", i_size);
	
	if(i_size + 7 != a_message.getPosition()){
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", "size + 7 ");
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", i_size + 7);
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", "a_message.length()");
		RadioMethodLog(ame_Log_Statement, "isRadioMessage", "println", a_message.length());
		RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "return false");
		return false;
	}
	
	RadioMethodLog(ame_Log_EndMethod, "isRadioMessage", "println", "return true");
	return true;
}

//use only inside if isRadioMessage
RadioMessage getRadioMessage(Note a_message){//<$*112x>
	RadioMethodLog(ame_Log_StartMethod, "getRadioMessage", "println", "");
	RadioMessage i_message;
	
	i_message.uuid = a_message[1];
	i_message.command = a_message[2];
	i_message.size = a_message[3];
	i_message.sender_ip = a_message[4];
	i_message.reciever_ip = a_message[5];
	
	i_message.message = new Note();
	
	for(int x = 6; x < a_message.getPosition() - 1; x++){
		char f_char = a_message.get(x);
		i_message.message->addLocalValue(f_char);
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

bool isRadioResponce(char a_start, Note a_message, char a_end){//<$/1>
	RadioMethodLog(ame_Log_StartMethod, "isRadioResponce", "println", "");
	if(a_message.isEmpty()){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_message.isEmpty()");
		return false;
	}
	if(a_message.getPosition() != 5){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_message.length() < 8");
		return false;
	}
	if(a_message.get(0) != a_start && a_message.get(4) != a_end){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_message.charAt(0) != a_start && a_message.charAt(a_message.length() - 1) != a_end");
		return false;
	}
	
	RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "");
	return true;
}

bool isRadioResponce(char a_start, char a_command, Note a_message, char a_end){//<$/1>
	RadioMethodLog(ame_Log_StartMethod, "isRadioResponce", "println", "");
	if(!isRadioResponce(a_start, a_message, a_end)){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "");
		return false;
	}
	
	int i_command = a_message[2];
	
	if(i_command != a_command){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "i_command");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", i_command);
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "a_command");
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", a_command);
		return false;
	}
	
	RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "");
	return true;
}

bool isRadioResponce(char a_start, char a_uuid, char a_command, char a_ip_origin, Note a_message, char a_end){//<$/1>
	RadioMethodLog(ame_Log_StartMethod, "isRadioResponce", "println", "");
	if(!isRadioResponce(a_start, a_message, a_end)){
		RadioMethodLog(ame_Log_EndMethod, "isRadioResponce", "println", "");
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
			RadioNetworkLog(ame_Log_StartMethod, "checkTimeOut", "println", "");
			m_send_time += a_tpc;
			m_resend_time += a_tpc;
			if(!m_readyToSend && m_resend_time > m_time_limit_resend){
				m_readyToSend = true;
				if(m_sendingsTry >= m_sending_limit){
					/////////////////////////////////////////////////////////////////////ERROR
					RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", "m_sendingsTry > 4");
					m_send.removeDeleteByPosition(0);

					m_sendingsTry = 0;
				}
			}
			RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", "m_send_time");
			RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", m_send_time);
			RadioNetworkLog(ame_Log_EndMethod, "checkTimeOut", "println", "");
			return m_send_time;
		}
		
		virtual Note* readRadio(){
			RadioNetworkLog(ame_Log_StartMethod, "readRadio", "println", "");
			int packetSize = LoRa.parsePacket();
			if (packetSize) {
				RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", "packetSize");
				RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", packetSize);
				String LoRaData;
				while (LoRa.available()) {
					LoRaData = LoRa.readString();
				}
				RadioNetworkLog(ame_Log_Statement, "readRadio", "println", "Note:");
				RadioNetworkLog(ame_Log_Statement, "readRadio", "println", LoRaData);
				RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", "return note");
				return new Note(LoRaData);
			}
			RadioNetworkLog(ame_Log_EndMethod, "readRadio", "println", "return nullptr");
			return nullptr;
		}
		
		virtual bool isReadingRadio(){
			RadioNetworkLog(ame_Log_StartMethod, "isReadingRadio", "println", "");
			RadioNetworkLog(ame_Log_EndMethod, "isReadingRadio", "println", "");
			return false;
		}
		
		virtual void sendMessage(){//<c4xxxxhola>
			RadioNetworkLog(ame_Log_StartMethod, "sendMessage", "println", "");

			if(m_send.isEmpty()){
				RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "m_send.isEmpty()");
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
			
			
			Note i_message_format = createRadioMessage(m_start_char, i_uuid, i_command, m_lora_ip, i_ip, i_mns, m_end_char);
			
			LoRa.beginPacket();
			LoRa.print(i_message_format);
			LoRa.endPacket();
			
			if(i_command == radio_UDP || i_command == radio_UDPB){
				m_send.removeDeleteByPosition(0);
				m_readyToSend = true;
				m_sending_limit = 0;
			}
			if(i_command == radio_TCP || i_command == radio_TCPB){
				m_sending_limit = 4;
			}
			if(	i_command == radio_SC || i_command == radio_SCB ||
				i_command == radio_NRSC || i_command == radio_NRSCB){
				m_sending_limit = 1;
			}
			
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "m_start_char");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", m_start_char);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "i_uuid");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", i_uuid);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "i_command");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", i_command);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "m_lora_ip");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", m_lora_ip);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "i_ip");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", i_ip);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "i_mns");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", i_mns);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "m_end_char");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", m_end_char);
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", "i_message_format");
			RadioNetworkLog(ame_Log_Statement, "sendMessage", "println", i_message_format);
			RadioNetworkLog(ame_Log_EndMethod, "sendMessage", "println", "");
		}
		
		virtual bool isMessageAConfirmation(Note* a_message){//<$/1>
			RadioNetworkLog(ame_Log_StartMethod, "isMessageAConfirmation", "println", "");
			if(a_message == nullptr || m_send.isEmpty()){
				RadioNetworkLog(ame_Log_EndMethod, "isMessageAConfirmation", "println", "a_message == nullptr");
				return false;
			}
			RadioSendingMessage i_sending_message = *m_send.getByPosition(0);

			if(!isRadioResponce(m_start_char, *a_message, m_end_char)){//<$/1>
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
		
		virtual void sendConfirmationByCommand(char a_uuid, char a_command, char a_ip_origin, char a_reciever_ip){
			RadioNetworkLog(ame_Log_StartMethod, "sendConfirmationByCommand", "println", "");
			RadioNetworkLog(ame_Log_Statement, "sendConfirmationByCommand", "println", "not my ip");
			RadioNetworkLog(ame_Log_Statement, "sendConfirmationByCommand", "println", "My ip");
			RadioNetworkLog(ame_Log_Statement, "sendConfirmationByCommand", "println", m_lora_ip);
			RadioNetworkLog(ame_Log_Statement, "sendConfirmationByCommand", "println", "u_reciever_ip");
			RadioNetworkLog(ame_Log_Statement, "sendConfirmationByCommand", "println", a_reciever_ip);

			if(a_reciever_ip != m_lora_ip){
				return;
			}
			
			if(	a_command == radio_TCP || a_command == radio_TCPB || 
				a_command == radio_SC || a_command == radio_SCB ||
				a_command == radio_NRSC || a_command == radio_NRSCB){
				this->sendConfirmation(a_uuid, a_ip_origin);
			}

			RadioNetworkLog(ame_Log_EndMethod, "sendConfirmationByCommand", "println", "");
		}
		
		virtual void DeliverMessageByCommand(char a_uuid, char a_command, char a_ip_origin, char a_reciever_ip, Note* a_message){
			RadioNetworkLog(ame_Log_StartMethod, "DeliverMessageByCommand", "println", "");
			RadioNetworkLog(ame_Log_Statement, "DeliverMessageByCommand", "println", "not my ip");
			RadioNetworkLog(ame_Log_Statement, "DeliverMessageByCommand", "println", "My ip");
			RadioNetworkLog(ame_Log_Statement, "DeliverMessageByCommand", "println", m_lora_ip);
			RadioNetworkLog(ame_Log_Statement, "DeliverMessageByCommand", "println", "u_reciever_ip");
			RadioNetworkLog(ame_Log_Statement, "DeliverMessageByCommand", "println", a_reciever_ip);

			if(a_reciever_ip != m_lora_ip){
				if(	a_command == radio_UDPB || a_command == radio_TCPB || a_command == radio_SCB || a_command == radio_NRSCB){
					this->DeliverMessage(a_message);
				}
				return;
			}
			
			this->DeliverMessage(a_message);

			RadioNetworkLog(ame_Log_EndMethod, "DeliverMessageByCommand", "println", "");
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
					RadioNetworkLog(ame_Log_Statement, "updateState", "println", "u_message == nullptr && randomTime < time");
					this->sendMessage();
					this->resetSendingTime();
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

			if(!isRadioMessage(m_start_char, *u_message, m_end_char)){
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "!isRadioMessage");
				return;
			}
			
			RadioMessage u_radio = getRadioMessage(*u_message);
			
			delete u_message;

			char u_uuid = u_radio.uuid;
			char u_command = u_radio.command;
			char u_sender_ip = u_radio.sender_ip;
			char u_reciever_ip = u_radio.reciever_ip;
			Note* u_rm_message = u_radio.message;

			if(u_rm_message == nullptr){
				RadioNetworkLog(ame_Log_Statement, "updateState", "println", "u_rm_message == nullptr");
				return;
			}

			RadioNetworkLog(ame_Log_Statement, "updateState", "println", "Decoded Message:");
			RadioNetworkLog(ame_Log_Statement, "updateState", "println", *u_rm_message);

			this->sendConfirmationByCommand(u_uuid, u_command, u_sender_ip, u_reciever_ip);
 
			this->DeliverMessageByCommand(u_uuid, u_command, u_sender_ip, u_reciever_ip, u_rm_message);
			
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
		int m_sending_limit = 0;
		
		char m_lora_ip = '0';
};

}

#endif