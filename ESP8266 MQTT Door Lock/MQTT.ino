#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int relay = 0;
const int ledr = 12; //red led
const int ledy = 13; //yellow led
const int ledg = 14; //green led
const int button = 15;
int bstate = 0; //button state

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WiFi 
const char *ssid = "J71"; // Enter your WiFi name
const char *password = "orchard@1234-1";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "project/door";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  //Set software serial baud to 115200;
  Serial.begin(115200);
  pinMode(relay , OUTPUT);
  pinMode(ledr , OUTPUT);
  pinMode(ledy , OUTPUT);
  pinMode(ledg , OUTPUT);
  pinMode(button , INPUT);
  digitalWrite(relay , LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay(); 
  
//connecting to a WiFi network
  wificonnect();
}


void callback(char *topic, byte *payload, unsigned int length) 
{
  display.clearDisplay();
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 20);
  display.print(" OPEN");
  display.display();  
  display.clearDisplay();
  
  if((char)payload[0] == '1')
  {
    Serial.println("1");
    digitalWrite(relay , HIGH);
    digitalWrite(ledg , HIGH);
    digitalWrite(ledr , LOW);
    delay(5000);
    digitalWrite(relay , LOW);
    digitalWrite(ledg , LOW);
    digitalWrite(ledr , HIGH);
  }
  else
  {
    Serial.print("Test Input");
  }
  
  Serial.println();
  Serial.println("-----------------------");

  display.display();
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(10, 20);
  display.println("LOCKED");
  display.display();
}


void loop() 
{
  // Reading button state to instant open the door
  bstate = digitalRead(button);

  if(bstate == HIGH)
  {
    Serial.println("button");
    digitalWrite(relay , HIGH);
    digitalWrite(ledg , HIGH);
    digitalWrite(ledr , LOW);
    delay(5000);
    digitalWrite(relay , LOW);
    digitalWrite(ledg , LOW);
    digitalWrite(ledr , HIGH);
  }
  else
  {
    client.loop();  
  }

  // Try to connect to wifi for read value
  if (WiFi.status() != WL_CONNECTED)
  {
    wificonnect();
  }
  else
  {
    client.loop();
  }

  // If Connect , then try to connect to MQTT Server
  if (!client.connected()) 
  {
    mqtt();
  }
  else
  {
    client.loop();
  }   
}


// Connect to wifi function
void wificonnect()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
    digitalWrite(ledr , LOW);
    digitalWrite(ledy , HIGH);
    digitalWrite(ledg , LOW);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println("Connecting....");
    display.display();
  }
  Serial.println("Connected to the WiFi network");
  digitalWrite(ledy , LOW);
  digitalWrite(ledg , LOW);
  digitalWrite(ledr , HIGH);
  mqtt();
}


// Connect to MQTT Server
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
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println("Reconnecting..");
    display.display();
    String client_id = "esp8266-clientmain-";
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
  display.clearDisplay();
  display.display();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.print("LOCKED");
  display.display();
    
  //publish and subscribe
  //client.publish(topic, "test");
  client.subscribe(topic);  
}
