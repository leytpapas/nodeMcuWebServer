#ifndef data_h
#define data_h

#include "progMEMData.h"

#define bufSize 6000
int bufc = 0; //buffer counter;

char data_websiteBuffer[bufSize];

extern ESP8266WebServer server;



/*  sendHeader must be called first,
  then copy every string (or substring, when generating json files) with sendToBuffer
  and if everything is done call sendBuffer.
*/
void sendBuffer() {
  if (bufc > 0) {
    server.sendContent_P(data_websiteBuffer, bufc);
    bufc = 0;
  }
}

void sendToBuffer(String str) {
  size_t len = str.length();
  if (bufc + len > bufSize) {
    server.sendContent_P(data_websiteBuffer, bufc);
    bufc = 0;
  }
  memcpy(data_websiteBuffer + bufc, str.c_str(), len);
  bufc += len;
}

void sendHeader(int code, String type, size_t _size) {
  Serial.println(_size);
  server.setContentLength(_size);
  server.send(code, type, "");
}

void sendFile(int code, String type, const char* adr, size_t len) {
  sendHeader(code, type, len);
  
    int runs = len/bufSize;
    if(len%bufSize > 0) runs++;
    for (int i = 0; i < runs; i++){
      int _len = bufSize;
      for(int h=0;h<bufSize;h++){
        if(h+i*bufSize >= len){
          _len = h;
          break;
        }
        else data_websiteBuffer[h] = (char)pgm_read_byte_near(adr + h+i*bufSize);
      }
    }
  server.sendContent_P(adr, len);
  sendBuffer();
}
#endif
