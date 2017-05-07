#include <TimerOne.h>

// leds a usar
byte bcd0 = A3;
byte bcd1 = A2;
byte bcd2 = A1;
byte bcd3 = A0;
byte btn1 = 2;
byte btn2 = 3;

byte led0 = 7;
byte led1 = 8;
byte led2 = 9;
byte led3 = 10;
byte led4 = 11;
byte led5 = 12;
byte led6 = 13;


int puntaje1 = 0;
int puntaje2 = 0;
int diferencia;

int tiempoDeJuego;
int x = 3; // tiempo para iniciar el juego
int auxTiempoJuego;
char inChar;
String string="";

long sec = 1000000;


// Declaración de métodos
void decimalToBCD(int x);
void botonUnoPush();
void botonDosPush();
void apagarLeds();
void diferenciaMetodo();
void timerElapsed();
void leerTiempo();

void setup() {
  pinMode(bcd0, OUTPUT);
  pinMode(bcd1, OUTPUT);
  pinMode(bcd2, OUTPUT);
  pinMode(bcd3, OUTPUT);
  
  pinMode(led0,INPUT);
  pinMode(led1,INPUT);
  pinMode(led2,INPUT);
  pinMode(led3,INPUT);
  pinMode(led4,INPUT);
  pinMode(led5,INPUT);
  pinMode(led6,INPUT);
  
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.print("Ingrese el tiempo de juego en segundos entre 3 y 9: ");
  
}

void loop() {
  leerTiempo();
}

void leerTiempo(){
  if (Serial.available()){
    //Lectura de caracteres   
    inChar = Serial.read();
    //Suma de caracteres en variable string
    string+=inChar;
     //Imprime la variable con los caracteres acumulados hasta el "\n"   
     if (inChar=='\n'){
        tiempoDeJuego = string.toInt();
        if(tiempoDeJuego>2 && tiempoDeJuego<10){
          Serial.print("Tiempo: ");
          Serial.println(string);
          auxTiempoJuego = tiempoDeJuego;
          Timer1.initialize(sec);
          Timer1.attachInterrupt(timerElapsed);
        } else { 
          Serial.println("Intente nuevamente");
          Serial.print("Ingrese el tiempo de juego en segundos entre 3 y 9: ");
        }
        string=""; 
    }
  }
}

void timerElapsed(){ 
  if(x>0){
    Serial.println(x);
    decimalToBCD(x);
    x=x-1;
    if(x==0){
      Serial.println("GO!!");
    }
  } else {
    if (tiempoDeJuego == auxTiempoJuego){
     attachInterrupt(digitalPinToInterrupt(btn1), botonUnoPush, FALLING);
     attachInterrupt(digitalPinToInterrupt(btn2), botonDosPush, FALLING);
    }
    if(tiempoDeJuego>=0){
      decimalToBCD(tiempoDeJuego);
      tiempoDeJuego--;
      if(tiempoDeJuego == -1){
        detachInterrupt(digitalPinToInterrupt(btn1));
        detachInterrupt(digitalPinToInterrupt(btn2));
        Serial.println("Fin del juego");
        Serial.print("Puntaje jugador 1: ");
        Serial.println(puntaje1);
        Serial.print("Puntaje jugador 2: ");
        Serial.println(puntaje2);
        if(puntaje1>puntaje2){
          Serial.println("El ganador es el jugador 1 :D");
        } else if(puntaje2>puntaje1){
          Serial.println("El ganador es el jugador 2 :D");
        } else {
          Serial.println("EMPATE!!!");
        }
      }
    }
  }
}

void botonUnoPush(){
  puntaje1++;
  diferenciaMetodo();
}

void botonDosPush(){
  puntaje2++;
  diferenciaMetodo();
}

void diferenciaMetodo(){
  diferencia = puntaje2-puntaje1;
  if(diferencia == 0){
      digitalWrite(led0, HIGH);
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      digitalWrite(led3,LOW);
      digitalWrite(led4,LOW);
      digitalWrite(led5,LOW);
      digitalWrite(led6,LOW);
    } else if(diferencia>0){
      if(diferencia<=3){
        digitalWrite(led4,HIGH);
        digitalWrite(led0,LOW);
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led5,LOW);
        digitalWrite(led6,LOW);
      } else if(diferencia <= 5){
        digitalWrite(led5,HIGH);
        digitalWrite(led0,LOW);
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led4,LOW);
        digitalWrite(led6,LOW);
      } else if(diferencia>5){
        digitalWrite(led6,HIGH);
        digitalWrite(led0,LOW);
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led4,LOW);
        digitalWrite(led5,LOW);
      }
  } else if(diferencia<0){
       if(diferencia>=-3){
        digitalWrite(led4,LOW);
        digitalWrite(led0,LOW);
        digitalWrite(led1,HIGH);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        digitalWrite(led5,LOW);
        digitalWrite(led6,LOW);
      } else if(diferencia >= -5){
        digitalWrite(led5,LOW);
        digitalWrite(led0,LOW);
        digitalWrite(led1,LOW);
        digitalWrite(led2,HIGH);
        digitalWrite(led3,LOW);
        digitalWrite(led4,LOW);
        digitalWrite(led6,LOW);
      } else if(diferencia<-5){
        digitalWrite(led6,LOW);
        digitalWrite(led0,LOW);
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,HIGH);
        digitalWrite(led4,LOW);
        digitalWrite(led5,LOW);
      }  
  }
}

void decimalToBCD(int x){
   switch (x) {
     case 0:
        digitalWrite(bcd0,LOW);
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,LOW);
       break;
     case 1:
        digitalWrite(bcd0,HIGH);;
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,LOW);
       break;
     case 2:
        digitalWrite(bcd0,LOW);
        digitalWrite(bcd1,HIGH);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,LOW);
       break;
     case 3:
        digitalWrite(bcd0,HIGH);;
        digitalWrite(bcd1,HIGH);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,LOW);
       break;
     case 4:
        digitalWrite(bcd0,LOW);
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,HIGH);
        digitalWrite(bcd3,LOW);
       break;
     case 5:
        digitalWrite(bcd0,HIGH);;
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,HIGH);
        digitalWrite(bcd3,LOW);
       break;
     case 6:
        digitalWrite(bcd0,LOW);
        digitalWrite(bcd1,HIGH);
        digitalWrite(bcd2,HIGH);
        digitalWrite(bcd3,LOW);
       break;
     case 7:
        digitalWrite(bcd0,HIGH);;
        digitalWrite(bcd1,HIGH);
        digitalWrite(bcd2,HIGH);
        digitalWrite(bcd3,LOW);  
       break;  
     case 8:
        digitalWrite(bcd0,LOW);
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,HIGH);
       break;
     case 9:
        digitalWrite(bcd0,HIGH);;
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,HIGH);  
       break;
     default:
        digitalWrite(bcd0,LOW);
        digitalWrite(bcd1,LOW);
        digitalWrite(bcd2,LOW);
        digitalWrite(bcd3,LOW);
     break;
  }
}
