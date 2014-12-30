#include <EEPROM.h>
#include <Ports.h>
#include <JeeLib.h>
#include <OneWire.h>

int nodeid = 16;

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

OneWire  ds(4);  
byte addr[8]; // Only have one sensor attached

void findSensor() {
  ds.reset_search();
  
  if ( ds.search(addr)) {
    Serial.println("Found sensor");
  } else {
    Serial.println("Failed to find sensor");
  }
}

float readTemp() { 
  byte i;
  byte present = 0;
  byte data[12];
  float celsius, fahrenheit;
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  Sleepy::loseSomeTime(1000);
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
 
  int16_t raw = (data[1] << 8) | data[0];
  raw = raw << 3; // 9 bit resolution default
  if (data[7] == 0x10) {
     // "count remain" gives full 12 bit resolution
     raw = (raw & 0xFFF0) + 12 - data[6];
  }
  
  celsius = (float)raw / 16.0;
  return celsius;

}

void setup () {
  nodeid = EEPROM.read(0);
  Serial.begin(57600);
  Serial.println("\nTemperature");
  Serial.print("Node id: ");
  Serial.println(nodeid);
  
  rf12_initialize(nodeid, RF12_868MHZ);
  rf12_sleep(RF12_SLEEP);
  findSensor();
}

void loop () {
  float temp = readTemp();
  int tempi = temp*100;
//  Serial.print("Temperature = ");
//  Serial.println(tempi);
  
  int high = tempi/256;
  int low = tempi % 256;
  
   unsigned char buf[4];
   buf[0] = nodeid; // our node id
   buf[1] = 0; // msg type
   buf[2] = high;
   buf[3] = low;

   rf12_sleep(RF12_WAKEUP);   
   
   while (!rf12_canSend()) {
     rf12_recvDone();
   }

   rf12_sendStart(0, buf, sizeof buf);
   rf12_sendWait(2); // standby
   Sleepy::loseSomeTime(60  * 1000);  


}
