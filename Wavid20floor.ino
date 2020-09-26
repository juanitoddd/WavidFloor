
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>

// FLOOR ESP
#define ESP_ID 99
/************************* FastLED Consts *********************************/

#define NUM_LEDS 167
#define DATA_PIN 2

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

CRGB leds[NUM_LEDS];

/************************* UDP  *********************************/

#define OF_UDP_IP    "192.168.0.111"
#define OF_UDP_PORT  5000

/************************* WiFi Access Point *********************************/

#define WLAN_SSID  "juanddd"
#define WLAN_PASS  "manonegra"
#define UDP_PORT   4422

/************************* Adafruit.io Setup *********************************/

static const uint8_t _D0   = 16;
static const uint8_t _D1   = 5;
static const uint8_t _D2   = 4;
static const uint8_t _D3   = 0;
static const uint8_t _D4   = 2;
static const uint8_t _D5   = 14;
static const uint8_t _D6   = 12;
static const uint8_t _D7   = 13;
static const uint8_t _D8   = 15;
static const uint8_t _D9   = 3;
static const uint8_t _D10  = 1;

int pixels = 150;

String scene = "blinkSlow";

// Set your Static IP address

// ---- CHANGE THIS FOR EVERY ESP !!!!
IPAddress local_IP(192, 168, 0, ESP_ID );
// ------------------------------ !!!!
IPAddress gateway(192, 168, 0, 111);
IPAddress subnet(255, 255, 255, 0);


/************************* UDP Setup *********************************/

WiFiUDP UDP;
// Commands comming from OF

char command[25];
char reply[16];

String IP;
String ID; // IP Address + Light Type


int ledState = LOW;

void setup() {

  Serial.begin(115200);
  // delay(10);

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  // digitalWrite(LED_BUILTIN, HIGH);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  IP = WiFi.localIP().toString();
  Serial.println("IP address: "); 
  Serial.println(IP);

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);

  Serial.println("Set-up Finish");
  // digitalWrite(LED_BUILTIN, HIGH);
}


// the loop function runs over and over again forever
void loop() {

  int packetSize = UDP.parsePacket();
  
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize);
    int len = UDP.read(command, 25);
    if (len > 0)
    {
      command[len] = '\0';
    }
    Serial.print("Packet received: ");
    Serial.println(command);
    /************************* UDP Actions *********************************/

    if(strcmp(command, "hello") == 0){
      
      UDP.beginPacket(OF_UDP_IP, OF_UDP_PORT);
      ID.toCharArray(reply, 16);
      UDP.write(reply);
      Serial.print("Reply: ");
      Serial.println(reply);
      UDP.endPacket();
      
    } else if(strcmp(command, "who") == 0) {
      
      UDP.beginPacket(OF_UDP_IP, OF_UDP_PORT);
      ID.toCharArray(reply, 16);
      UDP.write(reply);
      Serial.print("Reply: ");
      Serial.println(reply);
      UDP.endPacket();
    }
  }
  // Serial.print("Command: ");
  // Serial.println(command);
  /************************* Debug Actions *********************************/

  /************************* Light Actions *********************************/
  
  if(strcmp(command, "on") == 0){
    
    _on();
    
  } else if(strcmp(command, "off") == 0) {
    
    _off();

  } else {
    
    _blink();
  }
}

void _off() {                
  digitalWrite(LED_BUILTIN, LOW);
}

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;   

void _blink() {
    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > interval) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;   
 
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW)
        ledState = HIGH;
      else
        ledState = LOW;
 
      // set the LED with the ledState of the variable:
      digitalWrite(LED_BUILTIN, ledState);
    }
}

void _on() {                
  digitalWrite(LED_BUILTIN, HIGH);   
}
