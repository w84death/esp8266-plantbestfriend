#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <FS.h>

const char* ssid = "P1X_2.4GHz"; //Enter SSID
const char* password = "dawajneta"; //Enter Password

ESP8266WebServer server(80); 

String getContentType(String filename);
bool handleFileRead(String path);

void setup(void){
    Serial.begin(115200);
    delay(1000);
    Serial.println('\n');
    delay(1000);
    Serial.println("----------------------------------------");
    Serial.println("WELCOME TO THE < PLANT BEST FRIEND >");
    Serial.println("----------------------------------------");
    delay(1000);
    
    Serial.println("> Connecting to WiFi [");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print("*");
    }
    Serial.print("]\n");
    Serial.println("> WiFi connection Successful");

    Serial.println("----------------------------------------");    
    Serial.println("> The IP Address of ESP8266 Module is: ");
    Serial.print("> ");
    Serial.print(WiFi.localIP());
    Serial.println("----------------------------------------");

    SPIFFS.begin();
    server.on("/", handleRoot);
     server.onNotFound([]() {
      if (!handleFileRead(server.uri()))
        server.send(404, "text/plain", "404: Not Found");
    });
    server.begin();
    Serial.println("> HTTP server started");
}

void loop(void){
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "Hello world!");
}


String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".gif")) return "image/gif";
  return "text/plain";
}

bool handleFileRead(String path) {
  Serial.println("> HandleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
