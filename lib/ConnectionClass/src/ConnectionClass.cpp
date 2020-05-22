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
#include "ConnectionClass.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char *SSID_Class;
const char *PASS_Class;
char *ID_MQTT_Class;
const char *TOPIC_PUBLISH_Class;
const char *TOPIC_SUBSCRIBE_Class;
uint16_t BROKER_PORT_Class;
char BROKER_MQTT_Class_by_host[20];
const char *BROKER_MQTT_Class;

String mac;

WiFiClient wifi_Client;
PubSubClient MQTT(wifi_Client);

void ConnectionClass::set_server_by_host(uint16_t BROKER_PORT, const char *BROKER_MQTT)
{
	ip_by_host(BROKER_MQTT);
	BROKER_PORT_Class = BROKER_PORT;
	MQTT.setServer(BROKER_MQTT_Class_by_host, BROKER_PORT_Class);
}
void ConnectionClass::set_server(uint16_t BROKER_PORT, const char *BROKER_MQTT)
{
	BROKER_PORT_Class = BROKER_PORT;
	BROKER_MQTT_Class = BROKER_MQTT;
	MQTT.setServer(BROKER_MQTT_Class, BROKER_PORT_Class);
}
void ConnectionClass::set_topic(const char *TOPIC_SUBSCRIBE)
{
	TOPIC_SUBSCRIBE_Class = TOPIC_SUBSCRIBE;
}
void ConnectionClass::subscribe_topic()
{
	MQTT.subscribe(TOPIC_SUBSCRIBE_Class);
}
void ConnectionClass::ota()
{
	ArduinoOTA.setPassword((const char *)"positiva_password");

	ArduinoOTA
		.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
				type = "sketch";
			else
				type = "filesystem";
		})

		.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR)
				Serial.println("Auth Failed");
			else if (error == OTA_BEGIN_ERROR)
				Serial.println("Begin Failed");
			else if (error == OTA_CONNECT_ERROR)
				Serial.println("Connect Failed");
			else if (error == OTA_RECEIVE_ERROR)
				Serial.println("Receive Failed");
			else if (error == OTA_END_ERROR)
				Serial.println("End Failed");
		});

	ArduinoOTA.begin();
	Serial.println(WiFi.localIP());
}
void ConnectionClass::handle()
{
	ArduinoOTA.handle();
}
void ConnectionClass::wifi_connect(const char *SSID, const char *PASS)
{
	SSID_Class = SSID;
	PASS_Class = PASS;

	while (WiFi.status() != WL_CONNECTED)
	{
		WiFi.begin(SSID_Class, PASS_Class);
		Serial.print(".");
		delay(5000);
	}
}
int ConnectionClass::wifi_status(){
	return WiFi.status();
}
void ConnectionClass::mqtt_Connect()
{
	if (!MQTT.connected())
	{
		Serial.print("Connecting to Broker MQTT: ");
		Serial.println(BROKER_MQTT_Class);
		if (!MQTT.connect(ID_MQTT_Class))
		{
			Serial.println("Connect fail");
			delay(1000);
		}
		else
		{

			Serial.println("Connected");
			delay(1000);
		}
	}
}
bool ConnectionClass::mqtt_Connected()
{
	return MQTT.connected();
}
void ConnectionClass::mqtt_Loop()
{
	MQTT.loop();
}
int ConnectionClass::mqtt_state(){
	return MQTT.state();
}
void ConnectionClass::mqtt_Publish(const char *TOPIC_PUBLISH, char *payload)
{
	TOPIC_PUBLISH_Class = TOPIC_PUBLISH;
	MQTT.publish(TOPIC_PUBLISH_Class, payload);
}
void ConnectionClass::func_recebe(void (*func)(char *topic, byte *payload, unsigned int length))
{
	MQTT.setCallback(func);
}
String ConnectionClass::ip()
{
	String _ip;
	_ip = WiFi.localIP().toString();
	return _ip;
}
String ConnectionClass::func_mac()
{

	mac = WiFi.macAddress();
	ID_MQTT_Class = (char *)malloc(sizeof(char) * mac.length() + 1);
	for (int i = 0; i < mac.length(); i++)
	{
		ID_MQTT_Class[i] = mac.charAt(i);
	}
	ID_MQTT_Class[mac.length()] = '\0';

	return mac;
}
void ConnectionClass::mac_to_topic()
{
	TOPIC_SUBSCRIBE_Class = ID_MQTT_Class;
}
void ConnectionClass::ip_by_host(const char *host_name)
{
	mdns_init();

	struct ip4_addr addr;
	addr.addr = 0;

	esp_err_t err = mdns_query_a(host_name, 2000, &addr);
	if (err)
	{
		if (err == ESP_ERR_NOT_FOUND)
		{
			printf("Host was not found!");
			return;
		}
		printf("Query Failed");
		return;
	}
	sprintf(BROKER_MQTT_Class_by_host, "%d.%d.%d.%d", IP2STR(&addr));
}
