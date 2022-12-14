
#ifndef LoRaNetwork_hpp
#define LoRaNetwork_hpp
#define LoRaNetwork_AVAILABLE

#include "RadioNetwork.hpp"
#include "Class.hpp"
#include "Note.hpp"
#include "ByteArray.hpp"
#include "MessageDelivery.hpp"

#include <SPI.h>
#include <LoRa.h>

#ifdef LoRaNetwork_LogApp
	#include "ame_Logger_config.hpp"
	#include "ame_Logger.hpp"
	
	#define LoRaNetworkLog(location,method,type,mns) ame_Log(this,location,"LoRaNetwork",method,type,mns)
	#define const_LoRaNetworkLog(location,method,type,mns) 
	#define StaticLoRaNetworkLog(pointer,location,method,type,mns) ame_Log(pointer,location,"LoRaNetwork",method,type,mns)
#else
	#ifdef LoRaNetwork_LogDebugApp
		#include "ame_Logger_config.hpp"
		#include "ame_Logger.hpp"
		
		#define LoRaNetworkLog(location,method,type,mns) ame_LogDebug(this,location,"LoRaNetwork",method,type)
		#define const_LoRaNetworkLog(location,method,type,mns) 
		#define StaticLoRaNetworkLog(pointer,location,method,type,mns) ame_LogDebug(pointer,location,"LoRaNetwork",method,type)
	#else
		#define LoRaNetworkLog(location,method,type,mns) 
		#define const_LoRaNetworkLog(location,method,type,mns) 
		#define StaticLoRaNetworkLog(pointer,location,method,type,mns) 
	#endif
#endif

namespace ame{

class LoRaNetwork : public RadioNetwork {	
    public:
		LoRaNetwork(){}
		virtual ~LoRaNetwork(){}
			
		#if defined(cppObject_AVAILABLE) && defined(cppObjectClass_AVAILABLE) && defined(Class_AVAILABLE)
		virtual cppObjectClass* getClass(){
			return Class<LoRaNetwork>::classType;
		}
		
		virtual bool instanceof(cppObjectClass* cls){
			return cls == Class<LoRaNetwork>::getClass()
			#if defined(RadioNetwork_AVAILABLE)
			|| RadioNetwork::instanceof(cls)
			#endif
			;
		}
		#endif
		
	protected:
};

}

#endif