#include <LiquidCrystal_I2C.h>        // Memanggil library LCD
#include <OneWire.h>                  //Library suhu
#include <DallasTemperature.h>        //Library Suhu
#include <SoftwareSerial.h>
#include <BH1750.h>                   //Library Lux Meter
#include <Wire.h>                     //Library I2C 

#define ONE_WIRE_BUS 2  // Menempatkan PIN hasil pembacaan sensor DS18B20 pada PIN 2. 
//Disebut One Wire karena kita bisa menempatkan sensor DS18B20 lain pada PIN yang sama
LiquidCrystal_I2C lcd(0x27, 16, 2);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);
SoftwareSerial ArduinoMega (13, 12);    //Untuk Komunikasi dengan NODEMCU

BH1750 lightMeter;

float suhuDS18B20;

//=============parameter for PH=============
const int ph_Pin  = A0;
int Po = 0;
float PH_step;
int nilai_analog_PH;
double TeganganPh;

void setup(void)
{
  //==============LCD I2C===============
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SMKN 1 PURWOSARI");
  lcd.setCursor(3, 1);
  lcd.print("HIDROPONIK");
  lcd.clear();
  //delay(1000);
  Serial.begin(9600);     //Menginisiasikan setup kecepatan komunikasi
  ArduinoMega.begin(9600);

  //Sensor One-Wire DS18B20
  sensor.begin();      //Sensor One-Wire DS18B20

  //LUX METER
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));

  //PH SENSOR
  pinMode (ph_Pin, INPUT);

}

void loop(void)
{
  //==================MEMBACA VOID========================
  while (!ArduinoMega.available()) {

    ph_meter();           //membaca void ph_meter
    lux_meter();          //membaca void lux_meter
    suhu_meter();         //membaca void suhu_meter
  }
  //read_serial();          //read_serial();

}

//==================SENSOR SUHU========================
void suhu_meter() {
  //sensor.setResolution(9);        // Sebelum melakukan pengukuran, atur resolusinya
  //sensor.setResolution(10);
  //sensor.setResolution(11);
  sensor.setResolution(12);
  sensor.requestTemperatures();     // Perintah konversi suhu
  suhuDS18B20 = sensor.getTempCByIndex(0);  //Membaca data suhu dari sensor #0 dan mengkonversikannya ke nilai Celsius
  // suhuDS18B20 = (suhuDS18B20*9/5) + 32;
  // suhuDS18B20 = suhuDS18B20 = 273.15;
  //Serial.println(suhuDS18B20, 1);       //Presisi 1 digit
  //Serial.println(suhuDS18B20, 2);       //Presisi 2 digit
  //Serial.println(suhuDS18B20, 3);       //Presisi 3 digit
  Serial.println(suhuDS18B20, 4);         //Presisi 4 digit
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Suhu : ");
  //lcd.setCursor(7, 1);
  lcd.print(suhuDS18B20, 1);    //menunjukkan berapa angka dibelakang koma
  //lcd.print((char)223);
  lcd.print(" C");
  //delay(2000);
  int suhuair = suhuDS18B20;
  ArduinoMega.write('s');
  delay(10);
  ArduinoMega.write(suhuair);
  Serial.println(suhuair);
  delay(50);                     //delay 1 detik (1000 miliseconds)
}

//==================SENSOR PH========================
void ph_meter() {

  //untuk kalibrasi
  float ph4 = 3.19;
  float ph7 = 2.62 ;

  nilai_analog_PH = analogRead(ph_Pin);
  Serial.print("Nilai ADC Ph: ");
  Serial.println(nilai_analog_PH);
  TeganganPh = 5 / 1024.0 * nilai_analog_PH;
  Serial.print("TeganganPh: ");
  Serial.println(TeganganPh, 3);

  PH_step = (ph4 - ph7) / 3;
  Po = 7.00 + ((ph7 - TeganganPh) / PH_step);     //Po = 7.00 + ((teganganPh7 - TeganganPh) / PhStep);
  lcd.setCursor(0, 0);
  lcd.print("PH   : ");
  lcd.println(Po, 1);
  //delay(2000);
  Serial.print("PH : ");
  Serial.println(Po, 1 );
  int pe = Po;
  ArduinoMega.write('p'); //MENGIRIM DATA P KE NODEMCU
  delay(10);
  ArduinoMega.write(pe);
  delay(1000);         //delay 1 menit (1000 miliseconds)



  /*
     PH step = (tegangan PH4 – tegangan PH7) / (PH7 - PH4)
               = (3.19 – 2.62)/(7-4)
               = 0.57/3
               = 0.19
  */
}

//==================SENSOR INTENSITAS CAHAYA========================
void lux_meter() {
  float lux = lightMeter.readLightLevel();
  lcd.setCursor(0, 0);
  lcd.print("Lux  : ");
  lcd.println(lux);
  //lcd.print("lux");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(1500);
  //Serial.print("Light: ");
  //Serial.print(lux);
  //Serial.println(" lx");
  int lx = lux;
  ArduinoMega.write('l');
  delay(10);
  ArduinoMega.write(lx);
  delay(50);
}
