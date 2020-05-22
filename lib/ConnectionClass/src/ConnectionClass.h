/*********************************************************************************************************
**********************************************************************************************************			
								Library: ConnectionClass0												**
								Organizacao: Engetech													**
								Autor: Eng. Mateus Domingues Dias										**
								Email: mateusxdias@gmail.com											**
								Release: 10																**
								Data do release 14/02/2019												**
**********************************************************************************************************/
/*********************************************************************************************************/

#ifndef _ConnectionClass_h
#define _ConnectionClass_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class ConnectionClass
{
 protected:

 public:
	 void set_server_by_host( uint16_t BROKER_PORT, const char *BROKER_MQTT);
	 void set_server(uint16_t BROKER_PORT, const char *BROKER_MQTT);
	 void ota();
	 void handle();
	 void wifi_connect(const char* SSID, const char* PASS);
	 void mqtt_Connect();
	 bool mqtt_Connected();
	 void mqtt_Loop();
	 int mqtt_state();
	 void mqtt_Publish(const char* TOPIC_PUBLISH, char *payload);
	 void func_recebe(void(*func)(char* topic, byte* payload, unsigned int length));
	 void mac_to_topic();
	 void ip_by_host(const char * host_name);
	 void set_topic(const char *TOPIC_SUBSCRIBE);
	 void subscribe_topic();
	 int wifi_status();

	 String ip();
	 String func_mac();
};
#endif
