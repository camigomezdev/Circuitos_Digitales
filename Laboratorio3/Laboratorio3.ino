#include <Keypad.h>
#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"
#include <TimerOne.h>

rgb_lcd lcd;
Servo myservo;
byte pinButton = 0;
byte pinServo = 3;
byte speakerPin = 4;
String msj;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 8, 7, 6, 5 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 12, 11, 10, 9}; 
// Create the Keypad
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Producción
String claves[6];
bool inicio = false;

byte contadorErrores=0;

char inChar = ' ';
String string = " ";

bool alarma = false;
bool garaje = false;
bool sistemaRemotoHabilitado = false;
bool primera = true;
bool aperturaPuerta = false;
bool estadoSistema= false;
bool claveRemota = false;
char claveValidar = ' ';
int cantidadClaveRemota = 0;
String stringClaveRemota = "";

void setup()
{
  pinMode(speakerPin, OUTPUT);
  myservo.attach(pinServo);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);
  lcd.scrollDisplayLeft();
  guardarClaves();
  attachInterrupt(pinButton, abrirPuerta, FALLING);
  iniciarSistema();
  
}

void loop(){
  iniciarSistema();
}

//Guardar clave por primera vez
void guardarClaves(){
   lcd.setCursor(6, 0);
   msj = "Bienvenido";
   lcd.print(msj);
   lcd.setCursor(6, 1);
   msj = "Sistema de seguridad SKY_CQRTY";
   lcd.print(msj);
   scroll(msj.length()+15);
   delay(1000);

   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "Guardar claves";
   lcd.print(msj);
   
   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "Clave maestra:";
   lcd.print(msj);
   editarClave('*', 10);

   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "Clave remota:";
   lcd.print(msj);
   editarClave('#', 10);

   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "ACTIVAR alarma:";
   lcd.print(msj);
   editarClave('A', 6);

   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "DESACTIVAR alarma:";
   lcd.print(msj);
   editarClave('B', 6);

   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "ABRIR garaje:";
   lcd.print(msj);
   editarClave('C', 6);

   lcd.clear();
   lcd.setCursor(0, 0);
   msj = "CERRAR garaje:";
   lcd.print(msj);
   editarClave('D', 6);
   
   inicio = true;
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.setRGB(0,255,0);
   msj = "Sistema iniciado";
   lcd.print(msj);
   delay(1000);
}

//Scroll de la pantalla
void scroll(int largo){
   for (int positionCounter = 0; positionCounter < largo; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(200);
  }  
}

//Editar clave
void editarClave(char tipoClave,byte largo){
   byte i = 0;
    bool continuar = false;
    String clave = "";
    if(inicio){
        lcd.clear();
        lcd.setCursor(0, 0);
        msj = "Favor ingrese clave maestra para editar: ";
        lcd.print(msj);
        delay(150);
        scroll(msj.length()-1);
        continuar = validarClave(4, 10);
      if(continuar){
        lcd.clear();
        lcd.setCursor(0, 0);
        msj = "Nueva clave:";
        lcd.print(msj);
      }
    }else{
      continuar = true;
    }
    if(continuar){
      clave = "";
      i=0;
      while(i<largo){
          char key = kpd.getKey();
          if(key)  // Check for a valid key.
          {
              lcd.setCursor(i+2, 1);
              lcd.print('*');
              clave = clave + key;
              i++;
          }
       }
        switch(tipoClave){
           case 'A': 
            claves[0] = clave;
            break;
           case 'B':
            claves[1] = clave;
           break;
           case 'C':
            claves[2] = clave;
           break;
           case 'D':
            claves[3] = clave;
           break;
           case '*':
            claves[4] = clave;
           break;
           case '#':
            claves[5] = clave;
           break;
           default:
            
            break;
        }
        delay(150);
        lcd.setRGB(0,255,0);
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Clave guardada");
        delay(1000);
      }
}

void sistemaLocal(){
      printEstado();
      lcd.setCursor(0,1);
      char key = kpd.getKey();
      if(key)  // Check for a valid key.
      {
          if(key == '#'){
            if(!alarma){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Editar clave: ");
              lcd.setCursor(0,1);
              lcd.print("(A/B/C/D): ");
              key = kpd.getKey();
              while(!key){
                key = kpd.getKey();
                //lcd.print(key);
              }
            editarClave(key, 6);
            }else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Desactive alarma"); 
            }
          }else{
            lcd.setCursor(0,1);
            lcd.print(key);
            cambiarEstado(key);
          }
      }
}

void sistemaRemoto(){
  if(!sistemaRemotoHabilitado){
      if(cantidadClaveRemota<10){
        inChar = Serial.read();
        if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '1' || inChar == '2' 
        || inChar == '3' || inChar == '4' || inChar == '5' || inChar == '6' || inChar == '7' || inChar == '8' 
        || inChar == '9' || inChar == '0' || inChar == '*' || inChar == '#'){
             Serial.print('*');
             stringClaveRemota +=inChar;
             cantidadClaveRemota +=1;
         }
        }else{
           if(validarClaveRemota(5,stringClaveRemota)){
              Serial.println("Sistema Remoto Habilitado");
              Serial.println("A: Activar Alarma \nB: Desactivar Alarma \nC: Abrir Garaje \nD: Cerrar garaje \n#: Editar");
              sistemaRemotoHabilitado=true;
           }else{
             Serial.println("Error, vuelva a intentar");
             stringClaveRemota ="";
             cantidadClaveRemota =0;
           }
        }
  }else{
     if (Serial.available()){
      //Lectura de caracteres   
      inChar = Serial.read();
          if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '#' ){
          Serial.print(inChar);
          leerClave(inChar);
       }
      }
  }
 }

void leerClave(char tipoClave){
  String claveSerial = "";
 byte cantidad = 0;
  if(tipoClave == '#'){
    if(alarma){
      Serial.println("Debe desactivar la alarma");
    }else{
      Serial.println("Digite la clave maestra remota para editar: ");
      while(cantidad<10){
        inChar = Serial.read();
        if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '1' || inChar == '2' 
        || inChar == '3' || inChar == '4' || inChar == '5' || inChar == '6' || inChar == '7' || inChar == '8' 
        || inChar == '9' || inChar == '0' || inChar == '*' || inChar == '#'){
             Serial.print('*');
             claveSerial +=inChar;
             cantidad +=1;
         }
     }
     
     if(validarClaveRemota(5, claveSerial)){
        claveSerial = "";
        cantidad =0;
        Serial.println("Indique la clave que desea editar y digite la nueva clave: ");
        Serial.println("A: Activar Alarma \nB: Desactivar Alarma \nC: Abrir Garaje \nD: Cerrar garaje");
        inChar = Serial.read();
        while(inChar != 'A' && inChar != 'B' && inChar != 'C' && inChar != 'D'){
           inChar = Serial.read();
           tipoClave = inChar;
        }
        Serial.print(tipoClave);
        while(cantidad<10){
          inChar = Serial.read();
          if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '1' || inChar == '2' 
          || inChar == '3' || inChar == '4' || inChar == '5' || inChar == '6' || inChar == '7' || inChar == '8' 
          || inChar == '9' || inChar == '0' || inChar == '*' || inChar == '#'){
               Serial.print('*');
               claveSerial +=inChar;
               cantidad +=1;
           }
        }
        switch(tipoClave){
           case 'A': 
            claves[0] = claveSerial;
            Serial.println("Clave editada exitosamente");
            break;
           case 'B':
            claves[1] = claveSerial;
            Serial.println("Clave editada exitosamente");
           break;
           case 'C':
            claves[2] = claveSerial;
            Serial.println("Clave editada exitosamente");
           break;
           case 'D':
            claves[3] = claveSerial;
            Serial.println("Clave editada exitosamente");
           break;
           default:
            Serial.println("Error");
            break;
        }
        
     }else{
        return;
     }
  }
 }else{
    while(cantidad<6){
    inChar = Serial.read();
    if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '1' || inChar == '2' 
    || inChar == '3' || inChar == '4' || inChar == '5' || inChar == '6' || inChar == '7' || inChar == '8' 
    || inChar == '9' || inChar == '0' || inChar == '*' || inChar == '#'){
         Serial.print('*');
         claveSerial +=inChar;
         cantidad +=1;
     }
    }
    cambiarEstadoRemoto(claveSerial, tipoClave);
  }
}

//Activar el sistema
void iniciarSistema(){
    Serial.print("Bienvenido al sistema remoto de SKY_CQRTY \nPor favor digite su clave de acceso remota para acceder: ");
    while(1){
      sistemaLocal();
      sistemaRemoto();
      if(aperturaPuerta){
        playNote('a', 400);
        if(!alarma){
        playNote('a', 400);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Puerta Abierta");
        aperturaPuerta = false;
        delay(1000);
      }
      else{
        playNote('a', 400);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Clave de acceso:");
        Timer1.initialize(10000000);
        Timer1.attachInterrupt(tiempoDesbloqueo);
        if(cambiarEstado('B')){
          aperturaPuerta=false;
        }
      
      }
      
    }
  }
}

//Mostrar estado actual
void printEstado(){
  lcd.setCursor(0,0);
  if(alarma){
      lcd.setRGB(0,255,0);
      msj = "Alarma: Activa  ";
  }else{
      lcd.setRGB(111,111,111);
      msj = "Alarma: Inactiva";
  }
  lcd.print(msj);
}

bool cambiarEstado(char tipoClave){
 byte posicion;
 int pos;
    switch(tipoClave){
       case 'A': 
          if(alarma){
            msj = "Alarma ya activa";
          }else{
            if(validarClave(0, 6)){
                alarma = true;
                if(garaje){
                  for (pos = 0; pos <= 70; pos += 1) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    myservo.write(pos);              
                    delay(15);                       // waits 15ms for the servo to reach the position
                  }
                  garaje = false;
                }
                msj = "Activando";
            }else{
              return false; 
             }
          }
        break;
       case 'B':
          if(!alarma){
            msj = "Alarma desactiva";
          }else{
            if(validarClave(1, 6)){
                alarma = false; 
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(msj);
                msj = "Desactivando";
                Timer1.stop();
                Timer1.detachInterrupt();
            }else{
              return false; 
             }
          }
       break;
       case 'C':
          if(alarma){
            msj = "Desactive alarma";
          }else{
            if(garaje){
              msj = "Garaje abierto";
            }else{
              if(validarClave(2, 6)){
                msj = "Abriendo";
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.setRGB(0,0,255);
                lcd.print(msj);
                 for (pos = 70; pos >= 0; pos -= 1) {
                  myservo.write(pos);
                  delay(15);                      
                }
                garaje = true; 
              }else{
              return false; 
             }
            }
          }
       break;
       case 'D':
         if(alarma){
            msj = "Desactive alarma";
          }else{
            if(!garaje){
              msj = "Garaje cerrado";
            }else{
              if(validarClave(3, 6)){
                msj = "Cerrando";
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.setRGB(0,0,255);
                lcd.print(msj);
                for (pos = 0; pos <= 70; pos += 1) { // goes from 0 degrees to 90 degrees
                  // in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);                       // waits 15ms for the servo to reach the position
                }
                garaje = false; 
              }else{
              return false; 
             }
            }
          }
       break;
       default:
        msj = "Caracter no valido";
        return;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(msj);
    delay(1000);
    return true;
}


bool cambiarEstadoRemoto(String key, char tipoClave){
 byte posicion;
 int pos;
    switch(tipoClave){
       case 'A': 
          if(alarma){
            msj = "Alarma ya activa";
          }else{
            if(validarClaveRemota(0, key)){
                alarma = true;
                if(garaje){
                  for (pos = 0; pos <= 70; pos += 1) {
                    // in steps of 1 degree
                    myservo.write(pos); 
                    delay(15);                    
                  }
                   garaje = false;
                }
                msj = "Activando";
            }else{
              return false; 
             }
          }
        break;
       case 'B':
          if(!alarma){
            msj = "Alarma desactiva";
          }else{
            if(validarClaveRemota(1, key)){
                alarma = false; 
                Serial.println(msj);
                msj = "Desactivando";
                Timer1.stop();
                Timer1.detachInterrupt();
            }else{
              return false; 
             }
          }
       break;
       case 'C':
          if(alarma){
            msj = "Desactive alarma";
          }else{
            if(garaje){
              msj = "Garaje abierto";
            }else{
              if(validarClaveRemota(2, key)){
                msj = "Abriendo";
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.setRGB(0,0,255);
                lcd.print(msj);
                Serial.println(msj);
                for (pos = 70; pos >= 0; pos -= 1) { // goes from 0 degrees to 90 degrees
                  // in steps of 1 degree
                  myservo.write(pos);
                  delay(15);                       // waits 15ms for the servo to reach the position
                }
                garaje = true;
                return;
              }else{
              return false; 
             }
            }
          }
       break;
       case 'D':
         if(alarma){
            msj = "Desactive alarma";
          }else{
            if(!garaje){
              msj = "Garaje cerrado";
            }else{
              if(validarClaveRemota(3, key)){
                msj = "Cerrando";
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.setRGB(0,0,255);
                lcd.print(msj);
                Serial.println(msj);
                for (pos = 0; pos <= 70; pos += 1) {
                  myservo.write(pos);             
                  delay(15);                     
                }
                garaje = false; 
                return;
              }else{
              return false; 
             }
            }
          }
       break;
       default:
        msj = "Caracter no valido";
        return;
    }
    Serial.println(msj);
    return true;
}

bool validarClaveRemota(int posicion, String clave){
  Serial.println("\nValidar clave");
  if(clave==claves[posicion]){      
      contadorErrores = 0;
      playNote('b', 100);
      delay(100/3);
      playNote('a', 300);
      return true;
  }else{
     contadorErrores += 1;
     playNote('c', 400);
     msj = "Clave incorrecta";
     Serial.println(msj);
     msj = "Error #" + String(contadorErrores);
     Serial.println(msj);
     if(contadorErrores == 3){
        sistemaBloqueado();
     }
     return false;
  }
}

//Validar clave
bool validarClave(int posicion,byte largo){
  String clave = "";
 byte i = 0;
  while(i<largo){
          char key = kpd.getKey();
          if(key)  // Check for a valid key.
          {
              lcd.setCursor(i+2, 1);
              lcd.print('*');
              clave = clave + key;
              i++;
          }
          if(estadoSistema && largo!=10){
            sistemaBloqueado();
            return;
          }
  }
  
  if(clave==claves[posicion]){
      contadorErrores = 0;
      playNote('b', 100);
      delay(100/3);
      playNote('a', 300);
      return true;
  }else{
     delay(200);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.setRGB(255,0,0);
     contadorErrores += 1;
     playNote('c', 400);
     msj = "Clave incorrecta";
     lcd.print(msj);
     lcd.setCursor(0,1);
     msj = "Error #" + String(contadorErrores);
     lcd.print(msj);
     delay(1000);
     lcd.clear();
     if(contadorErrores == 3){
        sistemaBloqueado();
     }
     return false;
  }
}

void sistemaBloqueado(){
  lcd.clear();
  lcd.setRGB(255,0,0);
  lcd.setCursor(5,0);
  lcd.print("Sistema");
  lcd.setCursor(4,1);
  lcd.print("Bloqueado");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Clave Maestra: ");
 byte i =0; 
  String clave = ""; 
  while(1){
      playNote('d', 200);
      
      if(cantidadClaveRemota<10){
          inChar = Serial.read();
          if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '1' || inChar == '2' 
          || inChar == '3' || inChar == '4' || inChar == '5' || inChar == '6' || inChar == '7' || inChar == '8' 
          || inChar == '9' || inChar == '0' || inChar == '*' || inChar == '#'){
               Serial.print('*');
               stringClaveRemota +=inChar;
               cantidadClaveRemota +=1;
          }
        }else{
         if(validarClaveRemota(5,stringClaveRemota)){
              Serial.println("Sistema desbloqueado");
              estadoSistema = false;
              sistemaRemotoHabilitado=true;
              contadorErrores = 0;
              return;            
         }else{
             Serial.println("Error, vuelva a intentar");
             stringClaveRemota ="";
             cantidadClaveRemota =0;
         }
      }
      
      if(i<10){
          char key = kpd.getKey();
          if(key)  // Check for a valid key.
          {
              lcd.setCursor(i+2, 1);
              lcd.print('*');
              clave = clave + key;
              i++;
          }
      }else{
           if(validarClaveRemota(4,clave)){
               lcd.clear();
               lcd.setCursor(0, 0);
               lcd.setRGB(0,255,0);
               msj = "Sistema iniciado";
               lcd.print(msj);
               delay(1000);
               contadorErrores = 0;
             }else{
                 Serial.println("Error, vuelva a intentar");
                 clave ="";
                 i=0;
                 estadoSistema = false;
                 return;
             }
      }
    }
}

void playNote(char note,byte duration) {
    char names[] = {'a', 'b', 'c', 'd'};
   byte tones[] = { 37, 110, 2093, 4699};

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}
void playTone(int tone,byte duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

void abrirPuerta(){
  aperturaPuerta = true;
}

void tiempoDesbloqueo(){ 
  if(primera){
    primera = false;
  }else{
    estadoSistema = true;
    Timer1.stop();
  }
}

