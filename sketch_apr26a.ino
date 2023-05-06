#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <RTClib.h>

#define lcdAddress 0x27  
#define filas 2
#define columnas 16
LiquidCrystal_I2C pantallalcd(lcdAddress, columnas, filas);

OneWire ourWire(4);
DallasTemperature sensor(&ourWire);

RTC_DS3231 THC;

#define buzzer 3
#define salida_BUZ() pinMode(buzzer,OUTPUT)
#define encender_BUZ() tone(buzzer,1000)
#define apagar_BUZ() noTone(buzzer)

#define bomba 2
#define salida_B() pinMode(bomba,OUTPUT)
#define apagar_B() digitalWrite(bomba,HIGH)
#define encender_B() digitalWrite(bomba,LOW)

void setup() {
  pantallalcd.init();
  pantallalcd.backlight();
  sensor.begin();
  THC.begin();
  THC.adjust(DateTime(__DATE__, __TIME__));
  salida_BUZ();
  salida_B();

}

void loop() {
  sensor.requestTemperatures();
  float temperatura = sensor.getTempFByIndex(0);
  pantallalcd.setCursor(0,0);  
  pantallalcd.print(temperatura);
  pantallalcd.print("C ");
  
  DateTime fecha = THC.now();
  pantallalcd.print(  fecha.hour());
  pantallalcd.print(":");				
  pantallalcd.print(fecha.minute());
  pantallalcd.print(":");
  pantallalcd.print(fecha.second());
  
  delay(100);

  if (fecha.hour() == 19 && fecha.minute() == 59 && fecha.second() == 0){    
    encender_BUZ();
    pantallalcd.setCursor(0,1);
    pantallalcd.print("   REGAR EN 5s");
    delay(5000);
    pantallalcd.clear();
    
  } 
  if (fecha.hour() == 19 && fecha.minute() == 59 && fecha.second() == 5) {
  apagar_BUZ();
  pantallalcd.setCursor(0,1);
  pantallalcd.print("   REGAR GOOOD   ");
  encender_B();
  delay(20000);
  pantallalcd.clear();
  } else{
    apagar_B();
  } 
}