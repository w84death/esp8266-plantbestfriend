/**
 * ESP8266 PLANT BEST FRIEND
 * -------------------------
 * IoT Hardware & Software
 * 
 * by Krzysztof Krystian Jankowski // P1X 
 * https://p1x.in / https://krzysztofjankowski.com
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <FS.h>
#include <DHT.h>

const char* ssid = "P1X_2.4GHz";
const char* password = "dawajneta";

#define SOILPIN 0
#define SOILMIN 250
#define SOILMAX 600

#define DHTPIN 2
#define DHTTYPE DHT11
#define DHTTWEAK 15

#define WEBPORT 80

float sens_temp = 0.0;
float sens_humi = 0.0;
float sens_mois = 0.0;
long rssi = 0;

String getContentType(String filename);
bool handleFileRead(String path);
void updateSensorsReadings();
void updateRSSI();


DHT dht(DHTPIN, DHTTYPE, DHTTWEAK);
ESP8266WebServer server(WEBPORT);

void setup(void){
    Serial.begin(115200);
    Serial.println('\n');
    delay(1000);
    Serial.println("----------------------------------------");
    Serial.println("         STRAIGHT FROM THE POT...       ");
    Serial.println("          < PLANT BEST FRIEND >         ");
    Serial.println("----------------------------------------");
    delay(1000);
    Serial.println('\n');
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
    Serial.print("> The IP Address: ");
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
    server.on("/rssi", []() {
        updateRSSI();
        server.send(200, "text/plain", String(rssi, DEC));
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
  else if (filename.endsWith(".svg")) return "image/svg+xml ";
  return "text/plain";
}

bool handleFileRead(String path) {
  Serial.println("\t-> Serving file: " + path);
  if (path.endsWith("/")) path += "index.html";
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
  delay(100);
  
  sens_humi = dht.readHumidity();
  delay(100);
  
  sens_mois = 100 - map(analogRead(SOILPIN), SOILMIN, SOILMAX, 0, 100);

  Serial.println("> Sensors reading: ");
  Serial.println("\t-> Temperature: " + String(sens_temp, DEC) + "°C");
  Serial.println("\t-> Humidity: " + String(sens_humi, DEC) + "%");
  Serial.println("\t-> Moisture: " + String(sens_mois, DEC) + "%");
}

void updateRSSI() {
  rssi = WiFi.RSSI();
  Serial.print("> Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
