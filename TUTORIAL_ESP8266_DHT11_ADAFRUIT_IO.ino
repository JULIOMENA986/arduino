#include "DHT.h"

/********* Config. comunicacion ************/

#include "config.h"

/******** Example Starts Here ***********/

#define DHTPIN D4
const int PIR = D7;//movimiento
const int alarma=D5;//una madre que hace ruido
const int Trigger = D5;   //Pin digital 2 para el Trigger del sensor
const int Echo = D6;   //Pin digital 3 para el Echo del sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

AdafruitIO_Feed *temperatura = io.feed("Temperatura1");
AdafruitIO_Feed *humedad = io.feed("Humendad");
AdafruitIO_Feed *movimiento = io.feed("Movimiento");
AdafruitIO_Feed *distancia = io.feed("Ultrasonico1");

void setup() {
  pinMode(PIR,INPUT);
  pinMode(alarma, OUTPUT);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  //
  Serial.begin(9600);
  while(! Serial);
  Serial.print("Conectando a Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  
  dht.begin();
}

void loop() {
  delay(10000);
  
  io.run();
  long tiempo; 
  long d; 
  float h = dht.readHumidity();
  int value = digitalRead(PIR);
  float t = dht.readTemperature();
  if(value!=0)
  {
    tone(alarma,300);
    Serial.print(" alarma:  ");
    Serial.print(" alarma activa ");
    Serial.println();
    
  }
  else
  {
    noTone(alarma);
  }
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  tiempo = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = tiempo/59;             //escalamos el tiempo a una distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(d);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  Serial.print("temperatura: ");
  Serial.print(t);
  Serial.print("°C");
  Serial.print(" humedad: ");
  Serial.println(h);
  Serial.print(" movimiento: ");
  Serial.println(value);
  temperatura->save(t);
  humedad->save(h);
  movimiento->save(value);
  distancia->save(d);
}
