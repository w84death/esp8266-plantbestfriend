#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <FS.h>
#include <DHT.h>

const char* ssid = "P1X_2.4GHz"; //Enter SSID
const char* password = "dawajneta"; //Enter Password

#define DHTPIN 5     
#define DHTTYPE    DHT22
DHT dht(DHTPIN, DHTTYPE);

float sens_temp = 0.0;
float sens_humi = 0.0;
float sens_mois = 0.0;

const char* html_temp = "%TEMP%";
const char* html_humi = "%HUMI%";
const char* html_mois = "%MOIS%";
const char* html_status_color = "%SCOLOR%";
const char* html_status_icon = "%SICON%";
const char* html_status_msg = "%SMSG%";
const char* html_status_time = "%TIMESTAMP%";

const char* icon_ok = "👍";
const char* icon_water = "🌊";
const char* icon_hot = "🔥";
const char* icon_cold = "☃";

ESP8266WebServer server(80);

String getContentType(String filename);
bool handleFileRead(String path);
void updateSensorsReadings();

void setup(void){
    Serial.begin(115200);
    Serial.println('\n');
    delay(1000);
    Serial.println("----------------------------------------");
    Serial.println("  WELCOME TO THE < PLANT BEST FRIEND >  ");
    Serial.println("----------------------------------------");
    delay(1000);
    
    Serial.print("> Connecting to WiFi [");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print("*");
    }
    Serial.print("]\n");
    Serial.println("> WiFi connection Successful");

    Serial.println("----------------------------------------");    
    Serial.print("> The IP Address of ESP8266 Module is: ");
    Serial.println(WiFi.localIP());
    Serial.println("----------------------------------------");

    SPIFFS.begin();
    server.onNotFound([]() {
      if (!handleFileRead(server.uri()))
        server.send(404, "text/plain", "404: Not Found");
    });
    server.begin();
    
    server.on("/temp", []() {
        server.send(200, "text/plain", String(sens_temp, DEC));
    });
    server.on("/humi", []() {
        server.send(200, "text/plain", String(sens_humi, DEC));
    });
    server.on("/mois", []() {
        server.send(200, "text/plain", String(sens_mois, DEC));
    });
    server.on("/update", []() {
      updateSensorsReadings();
      server.send(200, "text/plain", "OK");
    });
    
    Serial.println("> HTTP server started");

    Serial.println("----------------------------------------");
    Serial.println("> Updating sensor readings...");
    updateSensorsReadings();
    Serial.println("----------------------------------------");
}

void loop(void){
  server.handleClient();
}

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".gif")) return "image/gif";
  return "text/plain";
}

bool handleFileRead(String path) {
  Serial.println("\t-> Serving file: " + path);
  if (path.endsWith("/")) {
    path += "index.html";
    updateSensorsReadings();
  }
  String contentType = getContentType(path);
  String indexData;
   
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    
    return true;
  }
  Serial.println("\t-> File Not Found");
  return false;
}

void updateSensorsReadings() {
  sens_temp = dht.readTemperature();
  sens_humi = dht.readHumidity();
  sens_mois = 0.0;
  Serial.println("> Sensors reading: ");
  Serial.println("\t-> Temperature: " + String(sens_temp, DEC) + "°C");
  Serial.println("\t-> Humidity: " + String(sens_humi, DEC) + "%");
  Serial.println("\t-> Moisure: " + String(sens_mois, DEC) + "%");
}
