#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "data.h"
#include <ArduinoJson.h>


const char* ssid = "*****";
const char* password = "******";

int relay = D4;

boolean state = false;
ESP8266WebServer server(80);

//#define STATICIP
IPAddress ip(192, 168, 1, 250); //Requested static IP address for the ESP
IPAddress router(192, 168, 1, 1); // IP address for the Wifi router
IPAddress netmask(255, 255, 255, 0);

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(115200);

  WiFi.hostname("Outlet"); //The displayed name on the routers tables
  WiFi.disconnect();

  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

#ifdef STATICIP
  WiFi.config(ip, router, netmask);
  Serial.println("Got static");
#endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  //HTML
  server.on ("/", loadIndexHTML);
  server.on ("/index.html", loadIndexHTML);
  //CSS
  server.on ("/style.css", loadStyle);
  //JS
  server.on ("/js/functions.js", loadFunctionsJS);
  //JSON
  server.on ("/switch.json", switchJS);

  server.onNotFound([]() {
    server.send(404, "text/plain", "FileNotFound");
  });

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  WiFi.localIP();
  digitalWrite(relay, HIGH);

}

void loadFunctionsJS() {
  sendFile(200, "application/javascript", data_js_functionsJS, sizeof(data_js_functionsJS));
}
void loadStyle() {
  sendFile (200, "text/css", data_styleCSS, sizeof(data_styleCSS));
}
void loadIndexHTML() {
  sendFile (200 , "text/html", data_indexHTML, sizeof(data_indexHTML));
}

void switchJS() {
  StaticJsonBuffer<200> newBuffer;
  JsonObject& root = newBuffer.parseObject(server.arg("plain"));
  Serial.println(server.arg("plain"));
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  boolean checkStatus = root["checkStatus"];
  if (checkStatus == true) {
    Serial.println("just a check");
    if (state) {
      server.send(200, "text/json", "true");
    } else {
      server.send(200, "text/json", "false");
    }
  } else {
    boolean command = root["command"];
    state = command;
    server.send(200, "text/json", "true");
    Serial.println(checkStatus);
    Serial.println("Not a check");
  }
  if (state)
    digitalWrite(relay, LOW);
  else
    digitalWrite(relay, HIGH);
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void loop() {
  server.handleClient();
}
