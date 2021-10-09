#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 

const char* ssid = "P1X_2.4GHz"; //Enter SSID
const char* password = "dawajneta"; //Enter Password

ESP8266WebServer server(80); 

void handleRoot();
void handleNotFound();

void setup(void){
    Serial.begin(115200);
    Serial.println('\n');
    Serial.println("----------------------------------------");
    Serial.println("WELCOME TO THE < PLANT BEST FRIEND >");
    Serial.println("----------------------------------------");
    WiFi.begin(ssid, password);
    
    
    Serial.println("> Connecting to WiFi [");
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print("*");
    }
    Serial.print("]\n");
    Serial.println("> WiFi connection Successful");
    Serial.println("> The IP Address of ESP8266 Module is: ");
    Serial.print("> ");
    Serial.print(WiFi.localIP());
    Serial.println("----------------------------------------");

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("> HTTP server started");
}

void loop(void){
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "Hello world!");
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}
