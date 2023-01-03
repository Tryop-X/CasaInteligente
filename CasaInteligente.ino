
/****************************************
 * Incluir Librerias
 ****************************************/
#include <WiFi.h>                                       // Libreria para WiFi
#include <PubSubClient.h>                               // LIbreria para Mqtt
#include <DallasTemperature.h>
#include <OneWire.h>

//*** Credenciales de la red *************
#define WIFISSID "HUGO - 1"                                   // Nombre de la red wifi
#define PASSWORD "72842321"                                   // Contraseña de la red wifi
#define TOKEN "BBFF-jN7Xh7fcOD14eT4Ej51njUemmlJhF3"           // Token de Ubidots
#define MQTT_CLIENT_NAME "esp32-Grupo5"                       // Nombre del dispositivo Mqtt

/****************************************
 * Definición de constantes


 
 ****************************************/
//VARIABLES PARA EL SENSOR DE TEMPERATURA

#define BS18B20 27 
OneWire oneWire(BS18B20);
DallasTemperature Sensor(&oneWire);
float valorLectura;

//----------------------------------------


#define VARIABLE_LABEL "pot"        // Nombre de la variable
#define DEVICE_LABEL "esp32_led"    // Nombre del dispositivo en ubidots
#define SENSOR 36                   // Set the GPIO36 as SENSOR
#define LED_1 23
#define LED_2 22
#define LED_3 21
#define LED_4 19
#define LED_5 18
#define LED_6 5
#define LED_7 4

#define BTN 15

int valorBoton = 0;


char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char topicSubscribe[150];
char str_sensor[10];
char valorAlarma[10];
char valorTemp2[10];

// Variable de timer usando millis()
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

/****************************************
 * Funciones Auxiliares
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

// Función para leer un dato que llega desde Ubidots al ESP32
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje llegada [");
  Serial.print(topic);
  Serial.print("] ");  
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  char valorHabitacion = topic[28];
  Serial.print("valor habitacion: ");
  Serial.println(valorHabitacion);
  Serial.print("estado: ");
  Serial.println((char)payload[10]);

  switch (valorHabitacion) {  
    case '1':
      if ((char)payload[10]=='1'){
        digitalWrite(LED_1, HIGH);        // Encendio de led        
      }
      else if ((char)payload[10]=='0'){
        digitalWrite(LED_1, LOW);         // Apagado de led
      }  
      break;
    case '2':
      if ((char)payload[10]=='1'){
        digitalWrite(LED_2, HIGH);        // Encendio de led        
      }
      else if ((char)payload[10]=='0'){
        digitalWrite(LED_2, LOW);         // Apagado de led
      }  
      break;
    case '3':
      if ((char)payload[10]=='1'){
        digitalWrite(LED_3, HIGH);        // Encendio de led        
      }
      else if ((char)payload[10]=='0'){
        digitalWrite(LED_3, LOW);         // Apagado de led
      }  
      break;    
    case '4':
      if ((char)payload[10]=='1'){
        digitalWrite(LED_4, HIGH);        // Encendio de led        
      }
      else if ((char)payload[10]=='0'){
        digitalWrite(LED_4, LOW);         // Apagado de led
      }  
      break;
    case '5':
      if ((char)payload[10]=='1'){
        digitalWrite(LED_5, HIGH);        // Encendio de led        
      }
      else if ((char)payload[10]=='0'){
        digitalWrite(LED_5, LOW);         // Apagado de led
      }  
      break;
        case '6':
        // Encender diodo led
        if ((char)payload[10]=='1'){
          digitalWrite(LED_6, HIGH);        // Encendio de led        
        }
        else if ((char)payload[10]=='0'){
          digitalWrite(LED_6, LOW);         // Apagado de led
        }  
        break;
      case '7':
      // Encender diodo led
      if ((char)payload[10]=='1'){
        digitalWrite(LED_7, HIGH);        // Encendio de led        
      }
      else if ((char)payload[10]=='0'){
        digitalWrite(LED_7, LOW);         // Apagado de led
      }  
      break;     
  }
}

void reconnect() {
  
  // Loop para reconexión MQTT
  while (!client.connected()) {
    Serial.println("Intentando conectarse...");
    
    // Conectandose
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Conectado");
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_1");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);
   
      sprintf(topicSubscribe, "%s", ""); // Cleans the topicSubscribe   
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_2");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);

      sprintf(topicSubscribe, "%s", ""); // Cleans the topicSubscribe   
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_3");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);


      sprintf(topicSubscribe, "%s", ""); // Cleans the topicSubscribe   
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_4");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);

      
      sprintf(topicSubscribe, "%s", ""); // Cleans the topicSubscribe   
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_5");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);
      
      sprintf(topicSubscribe, "%s", ""); // Cleans the topicSubscribe   
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_6");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);
      
      sprintf(topicSubscribe, "%s", ""); // Cleans the topicSubscribe   
      sprintf(topicSubscribe, "/v1.6/devices/%s/%s", DEVICE_LABEL, "led_7");   // Creando topic de suscripción
      client.subscribe(topicSubscribe);

    } else {
      Serial.print("Falla, rc=");
      Serial.print(client.state());
      Serial.println(" Espera 2 segundos");
      delay(2000);
    }
  }
}

/****************************************
 * Funciones principales
 ****************************************/
void setup() {

  Sensor.begin();



  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_7, OUTPUT);
  pinMode(BTN, INPUT);

  Serial.println();
  Serial.print("Conectandose a la red Wifi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());      // Mostrando la IP asignada
  
  client.setServer(mqttBroker, 1883);  // Conectando a servidor Ubidots por MQTT
  client.setCallback(callback);        // LLamada a función callback
  
}

void loop() {

  // Si pierde conexión , vuelve a conestarse
  if (!client.connected()) {
    reconnect();
  }
  if((millis() - lastTime)> timerDelay){
    lastTime = millis();
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);


    sprintf(payload, "%s", ""); // Cleans the payload
    sprintf(payload, "{\"%s\":", VARIABLE_LABEL);      // Agregar label de variable  
    // Lectura de potenciometro
    float sensor = analogRead(SENSOR); 
    Serial.print("sensor:");
    Serial.println(sensor*500/1024);
    dtostrf(sensor*500/4023, 5, 2, str_sensor);  
    
    sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor);  // Agregar valor
    client.publish(topic, payload);            
    
    
    sprintf(payload, "%s", ""); // Cleans the payload
    sprintf(payload, "{\"%s\":", "alarma");      // Agregar label de variable

    valorBoton = digitalRead(BTN);
    dtostrf(valorBoton, 1, 0, valorAlarma);
    Serial.print("valor del boton:");
    Serial.println(valorBoton);

    sprintf(payload, "%s {\"value\": %s}}", payload, valorAlarma);  // Agregar valor
    client.publish(topic, payload);  

        
    sprintf(payload, "%s", ""); // Cleans the payload
    sprintf(payload, "{\"%s\":", "term2");      // Agregar label de variable

    Sensor.requestTemperatures();
    valorLectura = Sensor.getTempCByIndex(0);
    Serial.println(valorLectura);
    dtostrf(valorLectura, 3, 2, valorTemp2);    

    sprintf(payload, "%s {\"value\": %s}}", payload, valorTemp2);  // Agregar valor
    client.publish(topic, payload);    
  }
  
  client.loop();
   
}
