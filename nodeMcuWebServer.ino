#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "data.h"
#include <ArduinoJson.h>


#define TIMEOUT 40 //secs
//#define STATICIP

int relay = D4;

const char* nodeHostname = "Switch Node";
/*const char* ssid = "Node";
const char* password = "";*/

boolean state = false; //state of switch
ESP8266WebServer server(80);

#ifdef STATICIP
IPAddress ip(192, 168, 1, 250); //Requested static IP address for the ESP
IPAddress router(192, 168, 1, 1); // IP address for the Wifi router
IPAddress netmask(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 250);
#endif

IPAddress  apGateway(192,168,3,1);
IPAddress  apIP(192,168,3,4); //Ip address as an AP
IPAddress  apNetmask(255, 255, 255, 0);

void loadIndexHTML() {
    sendFile (200 , "text/html", data_indexHTML, sizeof(data_indexHTML));
}
void loadFunctionsJS() {
    sendFile(200, "application/javascript", data_js_functionsJS, sizeof(data_js_functionsJS));
}
void loadStyle() {
    sendFile (200, "text/css", data_styleCSS, sizeof(data_styleCSS));
}

void switchJS() {
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
    } else if (req == "connect") {
        //add known network
        String ssid = root["ssid"];
        String pass = root["pass"];
        File f = SPIFFS.open("/last_connected.txt", "w");
        if(!f) {
            Serial.println("Error opening file");
            server.send(200, "text/json", "{\"req\": \"connect\",\"status\": \"failed\",\"error\":\"Couldnt open or create file.Try again later\"}");
        } else {
            f.println(ssid);
            f.println(pass);
            f.close();
            server.send(200, "text/json", "{\"req\": \"connect\",\"status\": \"success\",\"error\":\"\"}");
            Serial.println("ESP Restarting now");
            ESP.restart();
        }
    }else if (req == "apconf") {
        //add known network
        String ssid = root["ssid"];
        String pass = root["pass"];
        File f = SPIFFS.open("/apconfig.txt", "w");
        if(!f) {
            Serial.println("Error opening file");
            server.send(200, "text/json", "{\"req\": \"apconf\",\"status\": \"failed\",\"error\":\"Couldnt open or create file.Try again later\"}");
        }else {
            f.println(ssid);
            f.println(pass);
            f.close();
            server.send(200, "text/json", "{\"req\": \"apconf\",\"status\": \"success\",\"error\":\"\"}");
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

    WiFi.hostname(nodeHostname);
    Serial.println("Opening SPIFFS.");
    bool result = SPIFFS.begin();
    Serial.println("SPIFFS opened: " + result);
    File f = SPIFFS.open("/last_connected.txt", "r");
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
    
    String szSSID = "";
    String szPass = "";
    File f = SPIFFS.open("/apconfig.txt", "r");
    if(!f) {
        Serial.println("apconfig file open failed. Setting default values");
        szSSID = "Node";
        szPass = "";
    } else {
        szSSID = f.readStringUntil('\n');
        szSSID.replace("\n","");
        szSSID.replace("\t","");
        szSSID.replace("\r","");

        szPass = f.readStringUntil('\n');
        szPass.replace("\n","");
        szPass.replace("\t","");
        szPass.replace("\r","");
        f.close();

        /*
        for (int i = 0; i < strlen(szSSID.c_str()); ++i) {
          Serial.printf("%02x ", szSSID.c_str()[i]);
        }
        Serial.println("");*/
    }

    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);
    //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(apIP, apGateway, apNetmask) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP(nodeHostname) ? "Ready" : "Failed!");

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());

    WiFi.softAP((const char*)szSSID.c_str(),(const char*) szPass.c_str());
}

bool connectTo(String ssid,String password) {
    int i=0;
    WiFi.disconnect();

    WiFi.mode(WIFI_STA);
    // Connect to WiFi network

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
