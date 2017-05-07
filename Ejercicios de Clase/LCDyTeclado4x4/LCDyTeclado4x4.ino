#include <Keypad.h>
#include <Wire.h>
#include "rgb_lcd.h"
#define ledpin 13
#include <Servo.h>

Servo myservo;

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 8, 7, 6, 5 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 12, 11, 10, 9};
// Touch
byte pinTouch = 3;
bool bandera = false;

// Create the Keypad
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void precionoTouch();

void setup()
{
  pinMode(ledpin, OUTPUT);
  pinMode(pinTouch,INPUT);
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);

  // Print a message to the LCD.
  lcd.print("Escribe");
  attachInterrupt(digitalPinToInterrupt(pinTouch), precionoTouch, RISING);
  //attachInterrupt(1, precionoTouch, RISING);
  delay(1000);
}

void loop()
{
  char key = kpd.getKey();
  if (key) // Check for a valid key.
  {
    lcd.setCursor(5, 2);
    lcd.print(key);
    Serial.println(key);
  }
  if(bandera){
     lcd.setCursor(6, 2);
     lcd.print(key);
     bandera = false;
  }
 

}

void precionoTouch() {
  bandera = true;
}

