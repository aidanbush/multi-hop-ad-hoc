#include <EEPROM.h>
#include <RH_ASK.h>
#include <SPI.h>
#include "packet.h"

RH_ASK driver;

unsigned long next;
int id = 1;
int addr = 0;

  
void setup() {
    
     EEPROM.write(addr, id);
     int value = EEPROM.read(addr);
    
  
    Serial.begin(115200); // Debugging onlyM
    if (!driver.init())
         Serial.println("init failed");
    Serial.println("setup done");
    next = millis();
}

void loop() {
  
  
    if (next < millis()) {
        next = millis() + 1000;
        const char *msg = "Hello World!";
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        Serial.println("sent");
    }

    uint8_t buf[24];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      int i;
      // Message with a good checksum received, dump it.
      Serial.print("Message: ");
      Serial.println((char*)buf);         
    }
    
}
