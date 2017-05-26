#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <string.h>

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;
int clave;
char otherString[20] = "";
const char * input;

void setup(void) {

  Serial.begin(115200);
  Serial.println("--------Inicio de sistema--------");
  Serial.println("Coloque la tarjeta en el lector y dejela hay por todo el proceso");
  Serial.println("El sistema solo recibira claves de una longitu igual a 10 para las claves maestras y de 6 para las demás");
  Serial.println("Por favor ingrese la clave maestra local:");
  inputString.reserve(200);
  clave =0;
  nfc.begin();

  // configure board to read RFID tags
  nfc.SAMConfig();

}

void loop(void) {

  if (stringComplete) {
    Serial.println(inputString);
    inputString = "       "+inputString;
    inputString.toCharArray(otherString, inputString.length() + 1);
    clave += 1;
    if (clave == 1) {
      escritura(otherString, clave);
      Serial.println(otherString);
      Serial.println("Por favor ingrese la clave maestra remota:");
    }
    if (clave == 2) {
      escritura(otherString, clave);
      Serial.println(otherString);
      Serial.println("Por favor ingrese la clave para activar alarma:");
    }
    if (clave == 3) {
      escritura(otherString, clave);
      Serial.println(otherString);
      Serial.println("Por favor ingrese la clave para desactivar la alarma:");
    }
    if (clave == 4) {
      escritura(otherString, clave);
      Serial.println(otherString);
      Serial.println("Por favor ingrese la clave para abrir el garage:");
    }
    if (clave == 5) {
      escritura(otherString, clave);
      Serial.println(otherString);
      Serial.println("Por favor ingrese la clave para cerrar el garage:");
    }
    if (clave == 6) {
      escritura(otherString, clave);
      Serial.println(otherString);
      Serial.println("Finalizo el ingreso de claves");
    }
    
    inputString = "";
    stringComplete = false;
    delay(1000);
  }

}

void escritura(char * url, int posicion) {

  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  bool authenticated = false;               // Flag to indicate if the sector is authenticated

  // Use the default NDEF keys (these would have have set by mifareclassic_formatndef.pde!)
  uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };


  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  while (!success) {
    Serial.println("No se encontro la tarjeta, favor verificar que este colocada en el lector");
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  }

  success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4 * posicion, 0, keyb);
  if (!success)
  {
    Serial.println("Fallo de autenticacion");
    return;
  }

  Serial.println("Acceso consedido en el sector");


  if (strlen(url) > 30)
  {
    // The length is also checked in the WriteNDEFURI function, but lets
    // warn users here just in case they change the value and it's bigger
    // than it should be
    Serial.println("clave demasiado larga");
    return;
  }



  // URI is within size limits ... write it to the card and report success/failure
  success = nfc.mifareclassic_WriteNDEFURI(posicion, "", url);
  if (success)
  {
    Serial.println("Guardando en tarjeta");
    Serial.println("");
  }
  else
  {
    Serial.println("Fallo en el guardado");
  }


  // Wait a bit before trying again
  Serial.println("terminado");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

