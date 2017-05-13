// Demo for Grove - Starter V2.0
// Author: Loovee  2013-3-10
#include <TimerOne.h>
// Uses the Grove - Button to control the Grove - LED.
// Connect the Grove - Button to the socket marked D3
// Connect the Grove - LED to D7

void timerElapsed();
void abrirPuerta();

// Defines the pins to which the button and LED are connected.
byte pinButton = 3;
byte pinLed    = 13;
boolean primera = true;
String inputString = "";
boolean stringComplete = false;

void setup()
{
   Serial.begin(9600);
   inputString.reserve(100);
   
    // Configure the button's pin for input signals.
    pinMode(pinButton, INPUT);

    // Configure the LED's pin for output.
    pinMode(pinLed, OUTPUT);
    attachInterrupt(0, abrirPuerta, FALLING);
    
}

void loop()
{
  if (stringComplete) {
    inputString.trim();
    Serial.println(inputString);
    if(inputString == "123"){
      primera = true;
      Serial.println("desbloqueo exitoso");
      digitalWrite(pinLed, LOW);
    }else{
      Serial.println("fallo de desbloqueo");
    }
    stringComplete = false;
    inputString = "";
  }
}

void timerElapsed(){ 
  if(primera){
    primera = false;
  }else{
    Serial.println("fallo de acceso");
    Serial.println("Ingrese clave de desbloqueo: ");
    digitalWrite(pinLed, HIGH);
    Timer1.stop();
    Timer1.detachInterrupt();
  }
}

void abrirPuerta(){
  Serial.println("Ingrese clave de acceso: ");
  Timer1.initialize(10000000);
   Timer1.attachInterrupt(timerElapsed);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

