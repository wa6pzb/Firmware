/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "yourAP";
const char* password = "12345678";

// NETWORK: Static IP details...
IPAddress ip(192, 168, 1, 101);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


ESP8266WebServer server(80);

String webPage = "";
String Scounter = "0";
String Smillis = "";
int buttonPin = 0;     // the pin number of the pushbutton pin
int counter = 0;
int gpio13Led = 13;
int gpio12Relay = 12;

unsigned long previousMillis = 0;
const long interval = 1000;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup(void) {

  
  webPage += "<h1>SONOFF Web Server</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";

  // preparing GPIOs

  pinMode(buttonPin, INPUT);
  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio12Relay, HIGH);

  Serial.begin(115200);
  delay(5000);

  // Static IP Setup Info
  WiFi.config(ip, gateway, subnet);
  
  WiFi.begin(ssid, password);
  Serial.println("");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });

  server.on("/on", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(gpio13Led, LOW);
    digitalWrite(gpio12Relay, HIGH);
    delay(1000);
  });

  server.on("/off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(gpio13Led, HIGH);
    digitalWrite(gpio12Relay, LOW);
    delay(1000);
  });

  server.on("/status", []() {
    server.send(200, "text/html", Scounter);
  });


  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    // increment
    counter = counter + 1;
    Scounter = String(counter);
    delay(100);
    }
    else
    {
    Scounter = String(counter);
    }
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    counter = counter - 1;
    Smillis = String(currentMillis);
  }
  if (counter < 0) {
  counter = 0;
}
server.handleClient();
}
