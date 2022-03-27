#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
SoftwareSerial NodeMCU(D2, D1);

char auth[] = "rV0Cd0DpRgWnU9vA8eoWHUDBQkSJdivE";
//char ssid[] = "mkt";
//char pass[] = "123456789";

char ssid[] = "Redmi";
char pass[] = "1sampai5";

int value;
BlynkTimer timer;

int lampu1 = D5;
int lampu2 = D6;
int lampu3 = D7;
int lampu4 = D8;
int pompa = D3;

void read_sensor() {
  char kod;
  float valu;
  if (NodeMCU.available()) {
    kod = NodeMCU.read();
    if (kod == 'p') {
      delay(10);
      valu = NodeMCU.read();
      Serial.print(kod);
      Serial.println(valu);
      Blynk.virtualWrite(V0, valu);
    } else if (kod == 'l') {
      delay(10);
      valu = NodeMCU.read();
      Serial.print(kod);
      Serial.println(valu);
      Blynk.virtualWrite(V2, valu);
    } else if (kod == 's') {
      delay(10);
      valu = NodeMCU.read();
      Serial.print(kod);
      Serial.println(valu);
      Blynk.virtualWrite(V1, valu);
    }
  }
}

BLYNK_READ(  ){
  digitalWrite(lampu1, LOW);
  digitalWrite(lampu2, LOW);
  digitalWrite(lampu3, LOW);
  digitalWrite(lampu4, LOW);
  digitalWrite(pompa, LOW);
  delay(10000);
  digitalWrite(lampu1, HIGH);
  digitalWrite(lampu2, HIGH);
  digitalWrite(lampu3, HIGH);
  digitalWrite(lampu4, HIGH);
  digitalWrite(pompa, HIGH);
  delay(10000);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  NodeMCU.begin(9600);

  pinMode(lampu1, OUTPUT);
  pinMode(lampu2, OUTPUT);
  pinMode(lampu3, OUTPUT);
  pinMode(lampu4, OUTPUT);
  pinMode(pompa, OUTPUT);
  //timer.setInterval(1000L, read_ph);
  //timer.setInterval(1000L, read_suhu);
  //timer.setInterval(1000L, read_lux);
  timer.setInterval(1000L, read_sensor);
}

void loop() {
  
  Blynk.run();
  timer.run();
}
