#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"
#include <SPI.h>
#include <Adafruit_PN532.h>

rgb_lcd lcd;
Servo myservo;

#define PN532_IRQ   (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

byte pinButton = 6;
byte pinServo = 5;
byte speakerPin = 4;
String msj;
bool sistemaRemotoHabilitado = false;

//Producción
String claves[6];
bool inicio = false;

byte contadorErrores=0;

char inChar = ' ';
String string = " ";

bool alarma = false;
bool garaje = false;
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

  nfc.begin();
  nfc.SAMConfig();
  
  guardarClaves();
  printEstado();
  attachInterrupt(pinButton, abrirPuerta, FALLING);
  iniciarSistema();
}

void loop(){
}

//Guardar clave por primera vez
void guardarClaves(){
   inicio = true;
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.setRGB(0,255,0);
   msj = "Sistema iniciado";
   lcd.print(msj);
   Serial.println(msj);
   delay(1000);
   lcd.setCursor(0, 0);
   
}

//Editar clave
void editarClave(String clave, char tipoClave,byte largo){
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
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Clave guardada");
    delay(150);
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
           if(validarClaveRemota(2,stringClaveRemota)){
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
     
     if(validarClaveRemota(1, claveSerial)){
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


bool cambiarEstadoRemoto(String key, char tipoClave){
 byte posicion;
 int pos;
    switch(tipoClave){
       case 'A': 
          if(alarma){
            msj = "Alarma ya activa";
          }else{
            if(validarClaveRemota(3, key)){
                alarma = true;
                if(garaje){
                  for (pos = 0; pos <= 70; pos += 1) {
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
            if(validarClaveRemota(4, key)){
                alarma = false; 
                Serial.println(msj);
                msj = "Desactivando";
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
              if(validarClaveRemota(5, key)){
                msj = "Abriendo";
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.setRGB(0,0,255);
                lcd.print(msj);
                Serial.println(msj);
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
              if(validarClaveRemota(6, key)){
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
              }else{
              return false; 
             }
            }
          }
       break;
       default:
         msj = "Caracter no valido";
         return false;
    }
    printEstado();
    Serial.println(msj);
    return true;
}

//Activar el sistema
void iniciarSistema(){
    Serial.print("Bienvenido al sistema remoto de SKY_CQRTY \nPor favor digite su clave de acceso remota para acceder: ");
    while(1){
      sistemaRemoto();
      if(aperturaPuerta){
        Serial.println("\nLa puerta ha sido abierta");
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
          sistemaBloqueado();
        }
    }
  }
}

bool validarClaveRemota(int posicion, String clave){
  Serial.println("\nValidar clave");
  if(clave==lectura(posicion)){      
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
  lcd.print("Clave Maestra");
  Serial.println("Digite la clave maestra remota");
  byte i =0; 
  String clave = ""; 
  while(1){
      playNote('d', 200);
      delay(150);
      if(clave.length()<10){
          inChar = Serial.read();
          if(inChar == 'A' || inChar == 'B' || inChar == 'C' || inChar == 'D' || inChar == '1' || inChar == '2' 
          || inChar == '3' || inChar == '4' || inChar == '5' || inChar == '6' || inChar == '7' || inChar == '8' 
          || inChar == '9' || inChar == '0' || inChar == '*' || inChar == '#'){
               Serial.print('*');
               clave +=inChar;
          }
        }else{
         if(validarClaveRemota(1,clave)){
              Serial.println("Sistema desbloqueado");
              estadoSistema = false;
              sistemaRemotoHabilitado=true;
              contadorErrores = 0;
              printEstado();
              return;            
         }else{
             Serial.println("Error, vuelva a intentar");
             stringClaveRemota ="";
             cantidadClaveRemota =0;
         }
      }
    }
}


//Mostrar estado actual
void printEstado(){
  lcd.clear();
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


String lectura(int bloque) {
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16];                         // Array to store block data during reads

  uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {

    if (uidLength == 4)
    {
      success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4*bloque, 1, keyuniversal);
      success = nfc.mifareclassic_ReadDataBlock((4*bloque)+1, data);
      String clave = (char*)(data);
      if(bloque == 1 || bloque == 2){
        return clave.substring(0, 10);
      } else {
        return clave.substring(0, 6);
      }
      
    }
  }
}
