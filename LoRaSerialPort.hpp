
#ifndef LoRaSerialPort_hpp
#define LoRaSerialPort_hpp
#define LoRaSerialPort_AVAILABLE 

#include "cppObject.hpp"

#ifdef ame_ArduinoIDE
	#include "Arduino.h"
	#include "Printable.h"
	#include "IPAddress.h"
#endif

#include "SerialPort.hpp"
#include "cppObject.hpp"

#include <SPI.h>
#include <LoRa.h>

#ifdef LoRaSerialPort_LogApp
	#include "ame_Logger_config.hpp"
	#include "ame_Logger.hpp"
	
	#define LoRaSerialPortLog(location,method,type,mns) ame_Log(this,location,"LoRaSerialPort",method,type,mns)
#else
	#ifdef LoRaSerialPort_LogDebugApp
		#include "ame_Logger_config.hpp"
		#include "ame_Logger.hpp"
		
		#define LoRaSerialPortLog(location,method,type,mns) ame_LogDebug(this,location,"LoRaSerialPort",method,type)
	#else
		#define LoRaSerialPortLog(location,method,type,mns) 
	#endif
#endif

namespace ame{

class LoRaSerialPort : public SerialPort{	
    public:
	LoRaSerialPort(){
		LoRaSerialPortLog(ame_Log_StartMethod, "Contructor",  "println", "");
	}
	LoRaSerialPort(Note c_ip){
		this->m_ip = c_ip;
		LoRaSerialPortLog(ame_Log_StartMethod, "Contructor",  "println", name);
	}
	LoRaSerialPort(Note c_name, Note c_ip){
		this->m_name = c_name;
		this->m_ip = c_ip;
		LoRaSerialPortLog(ame_Log_StartMethod, "Contructor",  "println", name);
	}

	virtual ~LoRaSerialPort(){
		LoRaSerialPortLog(ame_Log_StartMethod, "Destructor",  "println", "");
		if(connected()){
			stop();
		}
	}
	
	virtual int available(){
		LoRaSerialPortLog(ame_Log_StartMethod, "available",  "println", "");
		int i_available = LoRa.available();
		if(i_available <= 0){
			LoRa.parsePacket();
		}
		return LoRa.available();
	}
	
	virtual int read(){
		LoRaSerialPortLog(ame_Log_StartMethod, "read",  "println", "");
		return LoRa.read();
	}
	
	virtual size_t write(uint8_t chr){
		LoRaSerialPortLog(ame_Log_StartMethod, "write",  "println", String(chr));
		LoRa.beginPacket();
		size_t i_size = LoRa.write(chr);
		LoRa.endPacket();
		return i_size;
	}

	virtual void flush(){
		LoRaSerialPortLog(ame_Log_StartMethod, "flush",  "println", "");
		LoRa.flush();
	}

	#if defined(cppObject_AVAILABLE) && defined(cppObjectClass_AVAILABLE) && defined(Class_AVAILABLE)
	virtual cppObjectClass* getClass(){return Class<LoRaSerialPort>::getClass();}
	virtual bool instanceof(cppObjectClass* cls){
		return cls == Class<LoRaSerialPort>::getClass() || SerialPort::instanceof(cls);
	}
	#endif

	protected:
};

}

#endif