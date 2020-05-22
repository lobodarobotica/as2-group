#include <Arduino.h>
#include "header.h"

void setup()
{

  Serial.begin(9600);

  time_0 = millis();

  log_init();

  nexInit();

  n_Salvar.attachPush(n_SalvarPushCallback);

  while (true)
  {

    String config_1 = Log.read_file("/config.json");
    Log.delete_buffer();
    delay(5000);

    Serial.println("Estou Aqui 0");
    Serial.println(config_1);

    StaticJsonDocument<200> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, config_1);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.println(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    const char *ssid = doc["wifi"]["ssid"];
    const char *password = doc["wifi"]["password"];
    uint32_t Range = doc["range"];
    if (ssid == "" || password == "")
    {
      break;
    }
    n_Range.setValue(Range);

    Serial.println("Estou Aqui");
    Serial.println(password);
    Serial.println(ssid);
    Serial.println(Range);

    //n_SetUser.setText(ssid);

    connect_wifi();

    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi conectado.");
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connect_mqtt();

    delay(5000);

    break;
  }

  //dbSerialPrintln("setup done");
  bt0.attachPop(bt0PopCallback, &bt0);
  Log.delete_file("data.json");
  Serial.print("Hello World");
  //connect_wifi();

  //client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);

  //reconnect();

  adc.begin();
  // Adjust gain to two (2.048V range) to get maximum resolution for 4-20mA range
  adc.setGain(GAIN_TWO);
}

void loop()
{

  time_0 = millis();
  while (WiFi.status() != WL_CONNECTED || !Connection.mqtt_Connected())
  {

    if (millis() - time_0 >= 10000)
    {
      connect_wifi();
      connect_mqtt();

      if ((WiFi.status() == WL_CONNECTED) & (Connection.mqtt_Connected()))
      {

        String data_recovered = Log.read_file("/data.json");
        Serial.println(data_recovered);

        char *ch_prt = (char *)calloc(data_recovered.length() + 1, sizeof(char *));
        strcpy(ch_prt, data_recovered.c_str());

        Connection.mqtt_Publish("teste", ch_prt);
        delay(5000);

        //deletando arquivo
        Log.delete_file("/data.json");

        //Saindo do laço
        Serial.println("Aplicando break");
        break;
      }
      time_0 = millis();
    }

    magical();
  }
  magical2();
  client.loop();
}

void magical()
{
  nexLoop(nex_listen_list);

  if (millis() - previousMillis >= maxTime)
  {
    previousMillis = millis();
    //Faz a leitura do sinal 4 a 20ma
    Sinal(); //pega os valores do Termopar e então manda para o NEXTION

    data_value["data"]["timestamp"] = random(155555555, 157777777);
    data_value["data"]["value"] = random(0, 4096);

    String data_1 = data_value["data"];
    Serial.println(data_1);

    char myOutput[512];

    serializeJson(data_value, myOutput);

    delay(500);
    Log.append_file("/data.json", myOutput);
    delay(500);

    habilita_CH();
  }
}

void magical2()
{

  nexLoop(nex_listen_list);

  if (millis() - previousMillis >= maxTime)
  {
    previousMillis = millis();
    Sinal();
    habilita_CH();
  }

  if (millis() - previousMillis2 >= maxTime2)
  {
    previousMillis2 = millis();
    Serial.println("Estou Aqui");
    publish("teste");
    wifi_img();
  }
}

void connect_wifi()
{
  Serial.println(ssid);
  Serial.println(password);
  WiFi.begin(ssid, password);

  int time_1 = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    if (millis() - time_1 >= 20000)
    {
      time_1 = millis();
      break;
    }
  }
  Serial.println("Wifi Connected");
}

void log_init()
{
  Log.begin();
  Log.list_dir("/");
}

void bt0PopCallback(void *ptr)
{

  bt0.getValue(&dual_state);
}

void habilita_CH()
{

  bt0.getValue(&dual_state);
  if (dual_state == 1)
  {
    HAB_CH1 = 1;
  }
  else
  {
    HAB_CH1 = 0;
    cor.setValue(0);
    n_CH1.setValue(0);
    Temp.setValue(0);
  }
}

void n_SalvarPushCallback(void *ptr)
{

  Serial.print("Esta no N_Salvar");
  Serial.println("//");

  memset(ssid, 0, sizeof(ssid));         // Limpa o buffer antes de realizar a leitura
  n_SetUser.getText(ssid, sizeof(ssid)); // Char para enviar para tela
  SetUser = atof(ssid);

  memset(password, 0, sizeof(password));          // Limpa o buffer antes de realizar a leitura
  n_SetSenha.getText(password, sizeof(password)); // Char para enviar para tela
  SetSenha = atof(password);

  if (n_SetUser.getText(ssid, sizeof(ssid)))
  {
    StaticJsonDocument<512> payload;

    payload["wifi"]["ssid"] = ssid;
    payload["wifi"]["password"] = password;

    n_Range.getValue(&Range);
    n_Range.setValue(Range);

    Serial.println("Range0:");
    Serial.println(Range);
    payload["range"] = Range;
    char myOutput[512];
    serializeJson(payload, myOutput);
    Log.write_file("/config.json", myOutput);
    delay(2000);
    Log.read_file("/config.json");
    Log.delete_buffer();

    connect_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connect_mqtt();
  }
}

void wifi_img()
{

  if (client.connected())
  {
    n_WifiStatus.setText("WIFI CONECTADO!");
  }
  else
  {
    n_WifiStatus.setText("WIFI DESCONECTADO!");
  }
}

void connect_mqtt()
{
  Connection.func_mac();
  Serial.println("Estou aqui 4");

  Connection.mac_to_topic();

  delay(100);
  Connection.set_server(1883, "mqtt.vuker.com.br");
  Serial.println("Estou aqui 1");

  long time_1 = millis();
  while (!Connection.mqtt_Connected())
  {
    if (millis() - time_1 >= 20000)
    {
      time_1 = millis();
      break;
    }

    Connection.mqtt_Connect();
    Connection.subscribe_topic();
    Connection.set_topic(MQTT_SERIAL_RECEIVER_CH);
    Connection.subscribe_topic();
  }
  Connection.mqtt_Publish("teste", "oi");
}

void callback(char *topic, byte *payload, unsigned int length)
{

  String msg;

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }
}

void VerificaConexoesWiFIEMQTT(void)
{
  if (!client.connected())
  {
    connect_mqtt();
  }
}

void Sinal()
{

  Serial.print(adc.read4to20mA(channel), 6);
  Serial.println("mA");

  if (HAB_CH1 == 1)
  {

    n_Range.getValue(&Range);
    n_Range.setValue(Range);
    tempe = ((adc.read4to20mA(channel)) * (Range) / 10);

    Temp.setValue(tempe);
    n_Range.setValue(Range);

    number = ((adc.read4to20mA(channel)) * 100);

    cor.setValue(number);
    n_CH1.setValue(number);
  }
}

void EnviaEstadoOutputMQTT(void)
{
  static unsigned long tempo;
  String corrente = String((adc.read4to20mA(channel)) * (Range) / 1000, 3);

  if (HAB_CH1 == 1)
  {

    if ((EnvioHabilita == 1) && ((millis() - tempo) > EnvioIntervalo))
    {

      String SensoresMsg = "{"; //*{"p0v01r0001x00":11,"p0v01r0002x01":22,"DataHora":00/00/00,00:00:00}

      SensoresMsg += "\"Sensor 1\":";
      SensoresMsg += corrente; // Valor do sensor 1
      SensoresMsg += ",";

      SensoresMsg += "}";

      int status = client.publish(TOPIC_PUB_Sensores, SensoresMsg.c_str());
      Serial.println(SensoresMsg);
      Serial.println("Estado: " + String(status)); // Status 1 para sucesso ou 0 para erro

      tempo = millis();
    }
  }
}

void publish(const char *topic)
{

  StaticJsonDocument<512> payload;

  String corrente = String((adc.read4to20mA(channel)) * (Range) / 1000, 3);

  payload["data"]["timestamp"] = random(155555555, 157777777);
  payload["data"]["value"] = random(0, 4096);

  char myOutput[512];
  serializeJson(payload, myOutput);

  Connection.mqtt_Publish(topic, myOutput);
}

void recebePacote(char *topic, byte *payload, unsigned int length)
{
  String RecebidoMsg;

  for (int i = 0; i < length; i++)
  { // Obtem a string do payload recebido
    char c = (char)payload[i];
    RecebidoMsg += c;
  }

  if (String(topic) == TOPIC_SUB_Suporte)
  { // Verifica se recebeu algo em TOPIC_SUB_Suporte
    if (RecebidoMsg == "curto")
    {
      EnvioIntervalo = 3000;
    }
    if (RecebidoMsg == "longo")
    {
      EnvioIntervalo = 15000;
    }
    if (RecebidoMsg == "on")
    {
      EnvioHabilita = 1;
    }
    if (RecebidoMsg == "off")
    {
      EnvioHabilita = 0;
    }

    Serial.print("Recebido: ");
    Serial.println(RecebidoMsg);
  }
}

void n_zerarPushCallback(void *ptr)
{
  gatilho = 1;
}
