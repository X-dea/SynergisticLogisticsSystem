#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin(1);
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  char a[4];
  Wire.requestFrom(1, 4);    // request 6 bytes from slave device #2

  while (Wire.available())   // slave may send less than requested
  {
    for(int x=0;x<=3;x++)
    {
      a[x] = Wire.read(); // receive a byte as character
    }
    Serial.print(a);         // print the character
  }

  delay(500);
}
