/*
 * ProxyGambit (GSM portion) -
 *   provides a serial terminal to a Raspberry Pi over a
 *   reverse TCP shell through a 2G GSM connection (SIM card)
 *
 * Hardware requirements:
 *   Raspberry Pi (or other serialized device)
 *   Arduino (ATmega328)
 *   Adafruit FONA (GSM board)
 *   SIM card (2G/GPRS enabled)
 *   Resistors (~1.6k and ~3.3k)
 *   3.4-4V or LiPo battery
 *   USB WiFi adapter for RPi
 *   SD card for RPi
 *
 * by Samy Kamkar
 * http://samy.pl
 */

// CHANGE YOUR TCP_PORT AND TCP_HOST TO CONNECT OUT TO YOUR MACHINE
// From your machine, you can then run:
//  nc -vlp 1337
#define TCP_PORT 1337
#define TCP_HOST "samy.pl"
#define TCP_HOST_MAXLEN 50

#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

char tcpserver[TCP_HOST_MAXLEN+1];

// This is to handle the absence of software serial on platforms
// like the Arduino Due. Modify this code if you are using different
// hardware serial port, or if you are using a non-avr platform
// that supports software serial.
#ifdef __AVR__
  #include <SoftwareSerial.h>
  SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
  SoftwareSerial *fonaSerial = &fonaSS;
#else
  HardwareSerial *fonaSerial = &Serial1;
#endif

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup() {
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
//    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
//  Serial.println(F("FONA is OK"));

  fona.enableGPRS(true);

  // Print SIM card IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
//    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }
  
  strncpy(tcpserver, TCP_HOST, TCP_HOST_MAXLEN);

  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  //fona.setGPRSNetworkSettings(F("your APN"), F("your username"), F("your password"));
  Serial.begin(115200);
}

#define MAXBUFLEN 80
uint8_t rbyte;
char buf[MAXBUFLEN];
bool conn;
void loop() {
  int j = 0;

  // reconnect if disconnected
  if (!fona.TCPconnected())
  {
    conn = fona.TCPconnect(tcpserver, TCP_PORT);
//    Serial.println(conn ? "success" : "FAIL");
    if (!conn)
    {
//      Serial.println("Waiting 1 second to try again");
      delay(1000);
    }
  }
  
  else
  {
    //Serial.println("connected");
    // tcp (user) data to read?
    while (fona.TCPavailable())
    {
 //     Serial.println("tcp avail");
      // send to raspi (and serial)
      fona.TCPread((uint8_t*)buf, MAXBUFLEN);
      Serial.print((char*)buf);
    }
    
    // raspi data?
    while (Serial.available())
    {
      j = 0;
      while (Serial.available() && j < MAXBUFLEN-2)
      {
        buf[j++] = Serial.read();
//        rbyte = Serial.read();
//        sprintf(buf+j, "%02X", rbyte);
//        j += 2;
      }
      buf[j] = '\0';
      fona.TCPsend((char*)buf, j);
    }
  }
}

