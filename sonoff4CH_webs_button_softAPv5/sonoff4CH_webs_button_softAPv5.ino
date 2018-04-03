#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "YourSSID";
/* Password must be at least 8 characters long or it will set no password at all */
const char *password = "12345678";

/* HTML symbols */
String state_on = "&#9899";
String state_off = "&#9898";

String state1 = "";
String state2 = "";
String state3 = "";
String state4 = "";



ESP8266WebServer server(80);

String webPage = "";

/* Specific GPIO details for the SONOFF 4CH */


int gpioSLed = 13;   //Status LED
int gpioRelay1 = 12;
int gpioRelay2 = 5;
int gpioRelay3 = 4;
int gpioRelay4 = 15;

int gpioButton1 = 0;
int gpioButton2 = 9;
int gpioButton3 = 10;
int gpioButton4 = 14;

int button1_state = HIGH;      // the current state of the button1
int button2_state = HIGH;      // the current state of the button2
int button3_state = HIGH;      // the current state of the button3
int button4_state = HIGH;      // the current state of the button4

int button1_reading;           // the current button1 reading from the input pin
int button2_reading;           // the current button2 reading from the input pin
int button3_reading;           // the current button3 reading from the input pin
int button4_reading;           // the current button4 reading from the input pin

int button1_previous = LOW;    // the previous button1 reading from the input pin
int button2_previous = LOW;    // the previous button2 reading from the input pin
int button3_previous = LOW;    // the previous button3 reading from the input pin
int button4_previous = LOW;    // the previous button4 reading from the input pin

long time1 = 0;         // the last time the output 1 was toggled
long time2 = 0;         // the last time the output 2 was toggled
long time3 = 0;         // the last time the output 3 was toggled
long time4 = 0;         // the last time the output 4 was toggled

long debounce = 200;   // the debounce time, increase if the output flickers



/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup(void) {
  /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
  webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  // basic webpage
  webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
  webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
  webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
  webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
  webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";

  // preparing GPIOs

  pinMode(gpioSLed, OUTPUT);
  digitalWrite(gpioSLed, HIGH);

  pinMode(gpioRelay1, OUTPUT);
  digitalWrite(gpioRelay1, LOW);

  pinMode(gpioRelay2, OUTPUT);
  digitalWrite(gpioRelay2, LOW);

  pinMode(gpioRelay3, OUTPUT);
  digitalWrite(gpioRelay3, LOW);

  pinMode(gpioRelay4, OUTPUT);
  digitalWrite(gpioRelay4, LOW);

  pinMode(gpioButton1, INPUT);
  pinMode(gpioButton2, INPUT);
  pinMode(gpioButton3, INPUT);
  pinMode(gpioButton4, INPUT);


  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Turn on Status LED showing device is ready now
  digitalWrite(gpioSLed, LOW);

  // server.on("/", handleRoot);
  server.on("/", []() {
    String webPage = "";
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
  });

  /* REALY 1 */

  server.on("/on_1", []() {
    String webPage = "";
    state1 = state_on;
    button1_state = HIGH;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay1, HIGH);
    delay(1000);
  });

  server.on("/off_1", []() {
    String webPage = "";
    state1 = state_off;
    button1_state = LOW;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay1, LOW);
    delay(1000);
  });

  /* REALY 2 */

  server.on("/on_2", []() {
    String webPage = "";
    state2 = state_on;
    button2_state = HIGH;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay2, HIGH);
    delay(1000);
  });

  server.on("/off_2", []() {
    String webPage = "";
    state2 = state_off;
    button2_state = LOW;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay2, LOW);
    delay(1000);
  });

  /* REALY 3 */

  server.on("/on_3", []() {
    String webPage = "";
    state3 = state_on;
    button3_state = HIGH;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay3, HIGH);
    delay(1000);
  });

  server.on("/off_3", []() {
    String webPage = "";
    state3 = state_off;
    button3_state = LOW;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay3, LOW);
    delay(1000);
  });

  /* REALY 4 */

  server.on("/on_4", []() {
    String webPage = "";
    state4 = state_on;
    button4_state = HIGH;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay4, HIGH);
    delay(1000);
  });

  server.on("/off_4", []() {
    String webPage = "";
    state4 = state_off;
    button4_state = LOW;
    /* meta tag for mobile device scaling - https://stackoverflow.com/questions/27855154/website-elements-and-font-are-too-small-in-mobiles */
    webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    // basic webpage
    webPage += "<h1>Relay 1 " + state1 + "</h1><p><a href=\"on_1\"><button>ON</button></a>&nbsp;<a href=\"off_1\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 2 " + state2 + "</h1><p><a href=\"on_2\"><button>ON</button></a>&nbsp;<a href=\"off_2\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 3 " + state3 + "</h1><p><a href=\"on_3\"><button>ON</button></a>&nbsp;<a href=\"off_3\"><button>OFF</button></a></p>";
    webPage += "<h1>Relay 4 " + state4 + "</h1><p><a href=\"on_4\"><button>ON</button></a>&nbsp;<a href=\"off_4\"><button>OFF</button></a></p>";
    webPage += "<h4>-----</h4><p><a href=\"/\"><button>REFRESH</button></a></p>";
    server.send(200, "text/html", webPage);
    digitalWrite(gpioRelay4, LOW);
    delay(1000);
  });


  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  {
    // Button 1
    button1_reading = digitalRead(gpioButton1);

    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (button1_reading == HIGH && button1_previous == LOW && millis() - time1 > debounce)
    {
      if (button1_state == HIGH)
      {
        button1_state = LOW;
        state1 = state_off;
      }
      else
      {
        button1_state = HIGH;
        state1 = state_on;
      }
      time1 = millis();
    }

    digitalWrite(gpioRelay1, button1_state);

    button1_previous = button1_reading;
  }

{
    // Button 2
    button2_reading = digitalRead(gpioButton2);

    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (button2_reading == HIGH && button2_previous == LOW && millis() - time2 > debounce)
    {
      if (button2_state == HIGH)
      {
        button2_state = LOW;
        state2 = state_off;
      }
      else
      {
        button2_state = HIGH;
        state2 = state_on;
      }
      time2 = millis();
    }

    digitalWrite(gpioRelay2, button2_state);

    button2_previous = button2_reading;
  }

{
    // Button 3
    button3_reading = digitalRead(gpioButton3);

    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (button3_reading == HIGH && button3_previous == LOW && millis() - time3 > debounce)
    {
      if (button3_state == HIGH)
      {
        button3_state = LOW;
        state3 = state_off;
      }
      else
      {
        button3_state = HIGH;
        state3 = state_on;
      }
      time3 = millis();
    }

    digitalWrite(gpioRelay3, button3_state);

    button3_previous = button3_reading;
  }

{
    // Button 4
    button4_reading = digitalRead(gpioButton4);

    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (button4_reading == HIGH && button4_previous == LOW && millis() - time4 > debounce)
    {
      if (button4_state == HIGH)
      {
        button4_state = LOW;
        state4 = state_off;
      }
      else
      {
        button4_state = HIGH;
        state4 = state_on;
      }
      time4 = millis();
    }

    digitalWrite(gpioRelay4, button4_state);

    button4_previous = button4_reading;
  }  
  server.handleClient();
}
