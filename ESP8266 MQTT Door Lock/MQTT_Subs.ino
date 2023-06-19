#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int ledr = 12;
const int ledy = 13;
const int ledg = 14;
const int button = 15;
int bstate = 0;

// WiFi
const char *ssid = "J71"; // Enter your WiFi name
const char *password = "orchard@1234-1";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "tristan/pintu";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//void(* resetFunc) (void) = 0;

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);
  pinMode(ledr , OUTPUT);
  pinMode(ledy , OUTPUT);
  pinMode(ledg , OUTPUT);
  pinMode(button , INPUT);
  
  // connecting to a WiFi network
  connectwifi();
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  
  if((char)payload[0] == '1')
  {
    Serial.print("1");
    digitalWrite(ledg , HIGH);
    digitalWrite(ledr , LOW);
    delay(5000);
    digitalWrite(ledg , LOW);
    digitalWrite(ledr , HIGH);
  }
  else
  {
    Serial.print("Test Input");
  }  
  Serial.println();
  Serial.println("-----------------------");
}

void loop() 
{
  bstate = digitalRead(button);

  if(bstate == HIGH)
  {
    client.publish(topic, "1");
    delay(500);
  }
  else
  {
    client.loop();
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    connectwifi();
  }
  else
  {
    client.loop();
  }
  
  if(!client.connected()) 
  {
    mqtt();
  }
  else
  {
    client.loop();  
  }
}

void connectwifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
    digitalWrite(ledy , HIGH);
  }
  Serial.println("Connected to the WiFi network");
  digitalWrite(ledy , LOW);
  digitalWrite(ledg , LOW);
  digitalWrite(ledr , HIGH);
  mqtt();
}

void mqtt()
{
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) 
  {
    digitalWrite(ledr , LOW);
    digitalWrite(ledy , HIGH);
    digitalWrite(ledg , LOW);

    String client_id = "esp8266-client2-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) 
    {
      Serial.println("Public emqx mqtt broker connected");
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  digitalWrite(ledr , HIGH);
  digitalWrite(ledy , LOW);
  digitalWrite(ledg , LOW);
  // publish and subscribe
  client.subscribe(topic);
}
