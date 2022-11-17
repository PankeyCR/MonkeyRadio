
#ifndef LoRaSerialServer_hpp
#define LoRaSerialServer_hpp
#define LoRaSerialServer_AVAILABLE 

#include "SerialNetwork.hpp"

#ifdef ame_ArduinoIDE
	#include "Arduino.h"
	#include "IPAddress.h"
#endif

#include "LoRaSerialPort.hpp"

#include <SPI.h>
#include <LoRa.h>

#ifdef LoRaSerialServer_LogApp
	#include "ame_Logger_config.hpp"
	#include "ame_Logger.hpp"
	
	#define LoRaSerialServerLog(location,method,type,mns) ame_Log(this,location,"LoRaSerialServer",method,type,mns)
	#define const_LoRaSerialServerLog(location,method,type,mns) 
	#define StaticLoRaSerialServerLog(pointer,location,method,type,mns) ame_Log(pointer,location,"LoRaSerialServer",method,type,mns)
#else
	#ifdef LoRaSerialServer_LogDebugApp
		#include "ame_Logger_config.hpp"
		#include "ame_Logger.hpp"
		
		#define LoRaSerialServerLog(location,method,type,mns) ame_LogDebug(this,location,"LoRaSerialServer",method,type)
		#define const_LoRaSerialServerLog(location,method,type,mns) 
		#define StaticLoRaSerialServerLog(pointer,location,method,type,mns) ame_LogDebug(pointer,location,"LoRaSerialServer",method,type)
	#else
		#define LoRaSerialServerLog(location,method,type,mns) 
		#define const_LoRaSerialServerLog(location,method,type,mns) 
		#define StaticLoRaSerialServerLog(pointer,location,method,type,mns) 
	#endif
#endif
namespace ame{

class LoRaSerialServer : public SerialServer{	
    public:
    LoRaSerialServer(){
		LoRaSerialServerLog(ame_Log_Statement, "Contructor",  "println", "port = 80");
		m_port = 80;
    }
    LoRaSerialServer(int c_port){
		LoRaSerialServerLog(ame_Log_Statement, "Contructor",  "println", "");
		m_port = c_port;
    }
	
	virtual ~LoRaSerialServer(){
		LoRaSerialServerLog(ame_Log_Statement, "Destructor",  "println", "");
	}
	
	SerialPort* accept(){
		LoRaSerialServerLog(ame_Log_Statement, "accept",  "println", "");
		return nullptr;
	} 
	
	SerialPort* available(){
		LoRaSerialServerLog(ame_Log_Statement, "available",  "println", "");\
		return nullptr;
	}
	
	#if defined(cppObject_AVAILABLE) && defined(cppObjectClass_AVAILABLE) && defined(Class_AVAILABLE)
	virtual cppObjectClass* getClass(){return Class<LoRaSerialServer>::getClass();}
	virtual bool instanceof(cppObjectClass* cls){
		return cls == Class<LoRaSerialServer>::getClass() || SerialServer::instanceof(cls);
	}
	#endif

protected:
};

}

#endif