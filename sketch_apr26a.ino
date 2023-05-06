#include <Wire.h>//se llama la libreria Wire que nos ayudara a comunicarnos con el dispositivos de I2C
#include <LiquidCrystal_I2C.h>//se instala la libreria LiquidCrystal_I2C para trabajar con el i2c del lcd junto con el Módulo serial I2C
#include <DallasTemperature.h>//se instala y llama la libreria DallasTemperature para poder utilizar el sensor de temperatura
#include <RTClib.h>//se instala y llama la libreria RTClib para poder utilizar el modulo RTC

#define lcdAddress 0x27  //se indica la dirección que toma el modulo i2c
#define filas 2//se indica que numero de filas que tiene el lcd que son dos para despues utilizarlo
#define columnas 16//se indica el numero de columnas que tiene el lcd que son 16 
LiquidCrystal_I2C pantallalcd(lcdAddress, columnas, filas);//se llama la libreria LiquidCrystal_I2C se le indica el nombre de pantallalcd se le indica la dirección del lcd,la variable columnas y filas

OneWire Nauta(4);//En la libreria OneWire se le asigna en que pin se va a estar realizando la comunicacion
DallasTemperature SENSORT(&Nauta);//se llama la libreria de DallasTemperature y se crea el SENSORT que va a hacer lo que  la varible nauta

RTC_DS3231 THC;//crea objeto del tipo RTC_DS3231

#define buzzer 3//se indica el pin 3 al buzzer
#define salida_BUZ() pinMode(buzzer,OUTPUT)//se define la variable salida_BUZ y se le indica que va a suplentar el pin mode
#define encender_BUZ() tone(buzzer,200)//se define la variable encender_BUZ y se le indica que va a suplentar el tone
#define apagar_BUZ() noTone(buzzer)//se define la variable apagar_BUZ y se le indica que va a suplentar al noTone

#define bomba_A 2//se indica el pin 2 a la bomba_A
#define salida_B() pinMode(bomba_A,OUTPUT)//se define la variable salida_B y se le indica que va a suplentar al pinMode
#define apagar_B() digitalWrite(bomba_A,HIGH)//se define la variable apagar_B y se le indica que va a suplentar al digitalWrite
#define encender_B() digitalWrite(bomba_A,LOW)//se define la variable encender_B y se le indica que va a suplentar al digitalWrite

void setup() {
  pantallalcd.init();//se indica que la pantallalcd que en eso se iniciara
  pantallalcd.backlight();//se indica que la pantallalcd para su iluminación
  SENSORT.begin();//se inicializa el SENSORT
  THC.begin();//se inicializa el THC
  THC.adjust(DateTime(__DATE__, __TIME__));//funcion que permite establecer fecha y horario 
  salida_BUZ();//se indica la salida_BUZZ
  salida_B();//se indica la salida_B

}

void loop() {
  SENSORT.requestTemperatures();//Envia una solicitud al sensor para ver que temperatura nos devolvera el sensor
  float temperatura = SENSORT.getTempFByIndex(0);//asignamos a la variable flotante "temperatura" la lectura en el sensor
  pantallalcd.setCursor(0,0);//se dise que en la columna 0 se imprima lo siguiente y que se coloque en la fila 0
  pantallalcd.print(temperatura);//se imprima en la lcd temperatura
  pantallalcd.print("C ");//se imprima en el lcd C 
  
  DateTime fecha = THC.now();//funcion que devuelve fecha y horario en formato
  pantallalcd.print(  fecha.hour());//funcion que obtiene la hora de la fecha completa
  pantallalcd.print(":");//caracter dos puntos como separador
  pantallalcd.print(fecha.minute());//funcion que obtiene los minutos de la fecha completa
  pantallalcd.print(":");//caracter dos puntos como separador
  pantallalcd.print(fecha.second());//funcion que obtiene los segundos de la fecha completa
  
  delay(100);

  if (fecha.hour() == 19 && fecha.minute() == 59 && fecha.second() == 0){ //se  indica que a esa hora, ese minuto y ese segundo haga lo siguiente
    encender_BUZ();//se encienda el buzzer
    pantallalcd.setCursor(0,1);//se coloque en en la columna cero y se imprima en la fila
    pantallalcd.print("   REGAR EN 5s");//se implima REGAR EN 5s
    delay(5000);//se crea el delay para los 5s que se realizara
    pantallalcd.clear();//luego de que pase el tiempo se limpie
    
  } 
  if (fecha.hour() == 19 && fecha.minute() == 59 && fecha.second() == 5) {///se  indica que a esa hora, ese minuto y ese segundo haga lo siguiente
  apagar_BUZ();//se apgue el buzzer
  pantallalcd.setCursor(0,1);//se coloque en en la columna cero y se imprima en la fila
  pantallalcd.print("   REGAR GOOOD   ");//se imprima regar goood
  encender_B();//se encienda la bomba de agua
  delay(20000);//se crea el delay para los 20s que se realiza
  pantallalcd.clear();//lugo que pase el tiempo se limpie
  } else{//si no 
    apagar_B();//que la bomba este apagada
  } 
}
