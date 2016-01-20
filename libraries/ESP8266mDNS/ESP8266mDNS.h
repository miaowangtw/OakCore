/*
ESP8266 Multicast DNS (port of CC3000 Multicast DNS library)
Version 1.1
Copyright (c) 2013 Tony DiCola (tony@tonydicola.com)
ESP8266 port (c) 2015 Ivan Grokhotkov (ivan@esp8266.com)

This is a simple implementation of multicast DNS query support for an Arduino
running on ESP8266 chip. Only support for resolving address queries is currently
implemented.

Requirements:
- ESP8266WiFi library

Usage:
- Include the ESP8266 Multicast DNS library in the sketch.
- Call the begin method in the sketch's setup and provide a domain name (without
  the '.local' suffix, i.e. just provide 'foo' to resolve 'foo.local'), and the
  Adafruit CC3000 class instance.  Optionally provide a time to live (in seconds)
  for the DNS record--the default is 1 hour.
- Call the update method in each iteration of the sketch's loop function.

License (MIT license):
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/
#ifndef ESP8266MDNS_H
#define ESP8266MDNS_H

#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

//this should be defined at build time
#ifndef ARDUINO_BOARD
#define ARDUINO_BOARD "generic"
#endif

class UdpContext;

struct MDNSService {
  MDNSService* _next;
  char _name[32];
  char _proto[3];
  uint16_t _port;
};

class MDNSResponder {
public:
  MDNSResponder();
  ~MDNSResponder();
  bool begin(const char* hostName);
  //for compatibility
  bool begin(const char* hostName, IPAddress ip, uint32_t ttl=120){
    return begin(hostName);
  }
  void update();

  void addService(char *service, char *proto, uint16_t port);
  void addService(const char *service, const char *proto, uint16_t port){
    addService((char *)service, (char *)proto, port);
  }
  void addService(String service, String proto, uint16_t port){
    addService(service.c_str(), proto.c_str(), port);
  }
  
  void enableArduino(uint16_t port, bool auth=false);

private:
  struct MDNSService * _services;
  UdpContext* _conn;
  char _hostName[128];
  bool _arduinoAuth;

  uint32_t _getOurIp();
  uint16_t _getServicePort(char *service, char *proto);
  void _parsePacket();
  void _reply(uint8_t replyMask, char * service, char *proto, uint16_t port);
};

extern MDNSResponder MDNS;

#endif //ESP8266MDNS_H