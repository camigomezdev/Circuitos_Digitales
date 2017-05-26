/*
          **************************************************
                       Autor: Gonzalo Matarrubia
          **************************************************             
                       Proyecto: 1er tutorial NFC
                           para Geeky Theory
          **************************************************
          Descripción: Este sketch utiliza el número de 
          identificación único de cada etiqueta NFC para
          "fichar" la entrada o salida de una persona en
          un supuesto puesto de trabajo, etc.
          **************************************************
*/
//Incluimos la librería de la shield NFC
#include <Adafruit_PN532.h>

//Macros
#define SCK         13
#define MOSI        11
#define SS            10
#define MISO        12

#define miTarjeta 254883858
#define fuera 0
#define dentro 1

//Creamos un objeto NFC de clase PN532
Adafruit_PN532 NFC(SCK,MISO,MOSI,SS);

//Creamos variables globales
int persona = fuera;
void setup () {
     Serial.begin(9600);
     if(!Serial) { delay(100); }
     
     //Configuramos el NCF
     NFC.begin();
     NFC.SAMConfig();
}

void loop () {
     /*
     Leemos constantemente el número de identificación mientras no haya
     ninguna etiqueta NFC que leer id valdrá 0
     */
     uint32_t id = NFC.readPassiveTargetID(PN532_MIFARE_ISO14443A);

     Serial.println(id);
     
     if( miTarjeta == id) {
          //Comprobamos si la persona estaba fuera o dentro
          if ( persona == fuera ){
               Serial.println("Bienvenido");
               persona=dentro;
               delay(2000);
          }
          else {
               Serial.println("Hasta luego");
               persona=fuera;
               delay(2000);
          }
     }
     
     delay(50);
}
