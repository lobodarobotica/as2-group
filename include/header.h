
//---------------------------BIBLIOTECAS----------------------------//
#include <WiFi.h>
#include <PubSubClient.h>
#include <LogClass.h>

#include <ConnectionClass.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Nanoshield_ADC.h>

#include "Nextion.h"
#include "NexText.h"
#include "SoftwareSerial.h"
//#include <stdlib.h>

//---------------------------BIBLIOTECAS----------------------------//

//---------------------------Class----------------------------//

LogClass Log;
WiFiClient espClient;
PubSubClient client(espClient);
Nanoshield_ADC adc;
ConnectionClass Connection;
//---------------------------Class--------------------------------//

//---------------------------FUNCTIONS----------------------------//

void magical();

void magical2();

void connect_wifi();

void log_init();

void bt0PopCallback(void *ptr);

void habilita_CH();

void n_SalvarPushCallback(void *ptr);

void wifi_img();

void setup_wifi();

void connect_mqtt();

void callback(char *topic, byte *payload, unsigned int length);

void VerificaConexoesWiFIEMQTT(void);

void Sinal();

void EnviaEstadoOutputMQTT(void);

void publish(const char *topic);

void recebePacote(char *topic, byte *payload, unsigned int length);

void n_zerarPushCallback(void *ptr);

//---------------------------MQTT----------------------------//

//---------------------------Variaveis----------------------------//

long time_0;
StaticJsonDocument<512> data_value;
StaticJsonDocument<200> doc;
//WIFI
char SetSenha;
char SetUser;

char buffer[50] = {0};
char ssid[50] = "SHARELINK";
char password[50] = "12345678";
char WifiStatus[50] = {0};

//MQTT
const char *mqtt_server = "mqtt.vuker.com.br";
int mqtt_port = 1883;

//#define MQTT_USER ""
//#define MQTT_PASSWORD ""
#define MQTT_SERIAL_PUBLISH_CH "/icircuit/ESP32/serialdata/tx"
#define MQTT_SERIAL_RECEIVER_CH "/icircuit/ESP32/serialdata/rx"
#define TOPIC_PUB_Sensores "ESP32/CAFE/Temperatura" // Topico publicado
#define TOPIC_SUB_Suporte "ESP32/CAFE/suporte"      // Topico assinado

//4a20
int channel = 0;

//Data
int gatilho = 0;
long SensoresIntervalo = 6000;
long EnvioIntervalo = 5000;
int EnvioHabilita = 1;

//---------------------------IHM----------------------------//

float tempe = 0;
uint32_t number;
uint32_t Range;
uint32_t HAB_CH1;
uint32_t dual_state;

NexPage page0 = NexPage(0, 0, "page0"); // Setup
NexPage page1 = NexPage(1, 0, "page1"); // Setup
NexPage page2 = NexPage(2, 0, "page2"); // Setup
NexButton n_Salvar = NexButton(0, 1, "BSalvar");
NexDSButton bt0 = NexDSButton(2, 5, "bt0");
NexText n_SetSenha = NexText(0, 8, "t3");
NexText n_SetUser = NexText(0, 6, "t1");
NexText n_WifiStatus = NexText(0, 10, "WifiStatus");
NexNumber cor = NexNumber(1, 9, "COR1");
NexNumber Temp = NexNumber(1, 8, "TEMP1");
NexNumber n_Range = NexNumber(1, 12, "x2");
NexNumber n_CH1 = NexNumber(2, 7, "CH1");

NexPicture p0 = NexPicture(0, 11, "p0");
NexPicture p1 = NexPicture(0, 9, "p1");

//variáveis de controle de tempo, para funcionar como um cronômetro
long previousMillis = 0;
const int maxTime = 5000;

long previousMillis2 = 0;
const int maxTime2 = 5000;

char dual_bt0[10] = {0};

NexTouch *nex_listen_list[] = {
    &n_Salvar,
    &bt0,
    NULL};

//---------------------------IHM----------------------------//