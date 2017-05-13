/*

  Originally for an English final, actually going to be used for a lighting solution around my house.

  Run and debugged on an ESP8266-01 and Adafruit FTDI Friend

  Pin connections are as follows:

  GPIO0 --> Red LED transistor
  GPIO1 --> Green LED transistor
  GPIO2 --> Blue LED transistor
  GND --> Common Ground
  Vcc --> 3.3VDC, NOT 5V
  CHIP_EN --> 3.3VDC
  

*/

#define DEBUG 0 //Debug variable for compiling

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Wifi AP data
const char *ssid = "ESP8266";
const char *password = "password123"; //All the security!

//Start the webserver "server" on port 80
ESP8266WebServer server(80);

//Pin assignments
int rLed = 0;
int gLed = 1;
int bLed = 2;

//Placeholders for values
int r;
int g;
int b;

//HTML responses (and a placeholder)
String form = "<h1>LED Picker v0.1</h1> <form action='led'  oninput='gValue.value = g.valueAsNumber; rValue.value = r.valueAsNumber; bValue.value = b.valueAsNumber'> <div>R:<input id='r' type='range' min='0' max='255' step='1' name='r' value='0'/><output name='rValue' for='r' >0</output></div> <div>G:<input id='g' type='range' min='0' max='255' step='1' name='g' value='0'/><output name='gValue' for='g' >0</output></div> <div>B:<input id='b' type='range' min='0' max='255' step='1' name='b' value='0'/><output name='bValue' for='b' >0</output></div> <input id='submit' type='submit'> </form>";
String response;


void setup() {
  int i;
  for (i = 0; i < 2; i++) { //Assign all pins as outputs
    pinMode(i, OUTPUT);
    analogWrite(i, 0); //Turns off the LED as well
  }
  analogWriteRange(255); //Decreases the analog range from 1023 to 255, to make it easier for the interface -> analog output

#if DEBUG //Any blocks of code like this are for debugging and are either added or removed pre-compile
  Serial.begin(115200); //Start serial
  delay(10);
  Serial.println("\r\n");
  Serial.println("Configuring AP/server...");
#endif

  WiFi.softAP(ssid, password); //Start AP
  IPAddress myIP = WiFi.softAPIP(); //Make a IP variable and assign it the IP of the server
  server.on("/", handleRoot); //Configure the server to handle '/' to handleRoot
  server.on("/led", handleLed); //Configure the server to handle '/led/' to the handleLed function
  server.on("/tea", teapot); //Why not?
  server.begin(); //Start the server

#if DEBUG
  Serial.print("HTTP server started on ");
  Serial.println(myIP);
  Serial.println("\r\n");
  delay(30);
#endif

}


void handleRoot() { //Code for the root handling
  server.send(200, "text/html", form); //References the form string defined above for prettyness
}


void handleLed() { //Code for parsing input and the response after the form is submitted
  r = server.arg("r").toInt(); //Converts each argument in the request to an integer and passes it to a global variable
  g = server.arg("g").toInt();
  b = server.arg("b").toInt();

#if DEBUG
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  Serial.println("Setting values and sending 200 response");
#endif

  analogWrite(rLed, r); //Outputs the value to the correct pins
  analogWrite(gLed, g);
  analogWrite(bLed, b);

  response = "<div height=\'100\' width=\'100\' style=\'background-color: rgb("; //First define of the response
  
  if (r == 0 && g == 0 && b == 0) { //If the request is 0,0,0, say that the leds are turned off
    response += ")\'><h1>LEDs turned off";
  }
  
  else { //Otherwise render a view that has the color of the leds as the background and the RGB code above it
    response += r; //This set of values changes the color of the background using background-color
    response += ",";
    response += g;  //Optimisation in general probably sucks here
    response += ",";
    response += b;
    response += ")\'><h1>Color changed to ";
    response += r; //Actual text output here
    response += ", ";
    response += g;
    response += ", ";
    response += b;
  }
  response += "</h1> <div><a href=\'/\'> Return to home</a></div>"; //Add link back to '/'

  server.send(200, "text/html", response); //Send response back to client
}

void teapot() { 
  server.send(418, "text/plain", "Error 418: Attempted to brew coffee with a teapot"); // If you are curious: https://tools.ietf.org/html/rfc2324#section-2.3.2
} //That RFC is one of many April Fools jokes by ITEF; all of which are very humorous.


void loop() {
  server.handleClient(); //Forever handle the client
}

