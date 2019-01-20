#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <stdlib.h>

#define BUF_LEN 64

String input;
bool inComplete = false;
int ID = 0;

void setup() {
  Serial.begin(115200);
  input.reserve(64);
}

void loop() {

  if(inComplete) {
    if (input.startsWith("printRoutingTable")){
      delay(1);
    } else if (input.startsWith("setID")){
      ID = atoi(parseID(input));
      Serial.print(String(ID) + '\n');
    } else if (input.startsWith("printID")){
      Serial.print(String(ID) + '\n');
    }
    
    input = "";
    inComplete = false;
    Serial.flush();
  }
}

void serialEvent() {
  while(Serial.available()) {
    char in = (char)Serial.read();
    input += in;

    if (in == '\n') {
      inComplete = true;
    }
  }
}

// because there's no split function for String objects...
char *parseID(String str)
{
  char tmp[BUF_LEN];

  for(int i = 0; i < str.length(); i++){
    tmp[i] = str.charAt(i);
  }
  char *id = strtok(tmp, " ");
  id = strtok(NULL, " ");
  return id;
}

/*

print routing table
set id
get id
*/
