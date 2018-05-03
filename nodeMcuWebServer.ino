#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "data.h"
#include <ArduinoJson.h>


<<<<<<< HEAD
#define TIMEOUT 40

int relay = D4;
=======
const char* ssid = "your ssid here";
const char* password = "your password here";

int relay = D4;
String ipaddr = "***.***.***.***";
>>>>>>> 3eeefce814e5c6cfd3033fcbb88c66475ee768c6

boolean state = false;
ESP8266WebServer server(80);

//#define STATICIP
<<<<<<< HEAD
#ifdef STATICIP
=======
>>>>>>> 3eeefce814e5c6cfd3033fcbb88c66475ee768c6
IPAddress ip(192, 168, 1, 250); //Requested static IP address for the ESP
IPAddress router(192, 168, 1, 1); // IP address for the Wifi router
IPAddress netmask(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 250);
<<<<<<< HEAD
#endif

IPAddress  apGateway(192,168,3,1);
IPAddress  apIP(192,168,3,4);
IPAddress  apNetmask(255, 255, 255, 0);

void loadIndexHTML() {
    Serial.println("loadIndexHTML");
    sendFile (200 , "text/html", data_indexHTML, sizeof(data_indexHTML));
}

void loadFunctionsJS() {
    Serial.println("loadFunctionsJS");
    sendFile(200, "application/javascript", data_js_functionsJS, sizeof(data_js_functionsJS));
}
void loadStyle() {
    Serial.println("loadStyle");
    sendFile (200, "text/css", data_styleCSS, sizeof(data_styleCSS));
}

void switchJS() {

    Serial.println("switchJS");
    StaticJsonBuffer<200> newBuffer;
    JsonObject& root = newBuffer.parseObject(server.arg("plain"));
    Serial.println(server.arg("plain"));
    if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
    }
    String req = root["req"];
    Serial.println(req);
    if (req == "check") {
        Serial.println("just a check");
        if (state) {
            server.send(200, "text/json", "{\"req\": \"check\",\"status\": \"true\",\"error\":\"\"}");
        } else {
            server.send(200, "text/json", "{\"req\": \"check\",\"status\": \"false\",\"error\":\"\"}");
        }
        return;
    } else if (req == "trigger") {
        boolean command = root["status"];
        state = command;
        server.send(200, "text/json", "{\"req\": \"trigger\",\"status\": \"success\",\"error\":\"\"}");
    } else if (req == "network_status") {
        String tmp = (WiFi.status() != WL_CONNECTED)?"offline":"online";
        server.send(200, "text/json", "{\"req\": \"network_status\",\"status\": \""+tmp+"\",\"error\":\"\"}");
    } else if (req == "add") {
        //add known network
        String ssid = root["ssid"];
        String pass = root["pass"];
        File f = SPIFFS.open("/last_connected.txt", "w");
        if(!f) {
            Serial.println("Error opening file");
            server.send(200, "text/json", "{\"req\": \"add\",\"status\": \"failed\",\"error\":\"Couldnt open or create file.Try again later\"}");
        } else {
            f.println(ssid);
            f.println(pass);
            f.close();
            server.send(200, "text/json", "{\"req\": \"add\",\"status\": \"success\",\"error\":\"\"}");
            Serial.println("ESP Restarting now");
            ESP.restart();
        }
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


void setup() {
    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);
    Serial.begin(115200);
    WiFi.hostname("node");

    bool result = SPIFFS.begin();
    Serial.println("SPIFFS opened: " + result);
    File f = SPIFFS.open("/last_connected.txt", "r+");
    if(!f) {
        Serial.println("Last connected file open failed.");
        accessPoint();
    } else {
        String szSSID = f.readStringUntil('\n');
        szSSID.replace("\n","");
        szSSID.replace("\t","");
        szSSID.replace("\r","");

        String szPass = f.readStringUntil('\n');
        szPass.replace("\n","");
        szPass.replace("\t","");
        szPass.replace("\r","");
        f.close();
        Serial.println("Tryng to connect to last connected network");
        /*
        for (int i = 0; i < strlen(szSSID.c_str()); ++i) {
          Serial.printf("%02x ", szSSID.c_str()[i]);
        }
        Serial.println("");*/
        if(!connectTo(szSSID,szPass)) {
            accessPoint();
        }
    }


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
        server.sendHeader("Location", "/",true); //Redirect to our html web page
        server.send(302, "text/plane","Page not Found. Redirecting to main page");
        //server.send(404, "text/plain", "FileNotFound");
    });

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.println(WiFi.localIP());
    digitalWrite(relay, HIGH);

}

void accessPoint() {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);
    //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(apIP, apGateway, apNetmask) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP("ESPTEST") ? "Ready" : "Failed!");

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());

    WiFi.softAP("node");
}

bool connectTo(String ssid,String password) {
    int i=0;
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

    WiFi.begin((const char*)ssid.c_str(),(const char*) password.c_str());

    for(int i=0; (WiFi.status() != WL_CONNECTED && i < TIMEOUT); i++) {
        delay(500);
        Serial.print(".");
    }

    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("Timed out. Caused by slow network or invalid ssid/password");
        return false;
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

    return true;
}

void loop() {
    server.handleClient();
}
=======

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(115200);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  WiFi.hostname("Test"); //set up a hostname
  
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
  ipaddr = String(WiFi.localIP());
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
  if (!ipaddr.equals(String(WiFi.localIP()))) {
    Serial.println(WiFi.localIP());
    ipaddr = String(WiFi.localIP());
  }
}
>>>>>>> 3eeefce814e5c6cfd3033fcbb88c66475ee768c6
