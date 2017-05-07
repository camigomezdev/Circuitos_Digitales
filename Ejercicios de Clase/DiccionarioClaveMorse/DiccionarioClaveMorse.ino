/*
  String to Integer conversion

 Reads a serial input string until it sees a newline, then converts
 the string to a number if the characters are digits.

 The circuit:
 No external components needed.

 created 29 Nov 2010
 by Tom Igoe

 This example code is in the public domain.
 */
int led13 = 13;

int numero;
bool estado = true;
String inString = "";    // string to hold input

int acumulador[5];
int pos=0;

char car;
int dit = 100;
int dash = dit*4;


struct Caracter
{  
  int cantidad;
  int vector[5];
  char caract;
};

Caracter a = {2, {dit,dash}, 'a'};
Caracter b = {4, {dash,dit,dit,dit}, 'b'};
Caracter c = {4, {dash,dit,dash,dit}, 'c'};
Caracter d = {3, {dash,dit,dit}, 'd'};
Caracter e = {1, {dit}, 'e'};
Caracter f = {4, {dit,dit,dash,dit}, 'f'};
Caracter g = {3, {dash,dash,dit}, 'g'};
Caracter h = {4, {dit,dit,dit,dit}, 'h'};
Caracter i = {2, {dit,dit}, 'i'};
Caracter j = {4, {dit,dash,dash,dash}, 'j'};
Caracter k = {3, {dash,dit,dash}, 'k'};
Caracter l = {4, {dit,dash,dit,dit}, 'l'};
Caracter m = {2, {dash,dash}, 'm'};
Caracter n = {2, {dash,dit}, 'n'};
Caracter o = {3, {dash,dash,dash}, 'o'};
Caracter p = {3, {dit,dash,dash,dit}, 'p'};
Caracter q = {4, {dash,dash,dit,dash}, 'q'};
Caracter r = {3, {dit,dash,dit}, 'r'};
Caracter s = {3, {dit,dit,dit}, 's'};
Caracter t = {1, {dash}, 't'};
Caracter u = {3, {dit,dit,dash}, 'u'};
Caracter v = {4, {dit,dit,dit,dash}, 'v'};
Caracter w = {3, {dit,dash,dash}, 'w'};
Caracter x = {4, {dash,dit,dit,dash}, 'x'};
Caracter y = {4, {dash,dit,dash,dash}, 'y'};
Caracter n0 = {5, {dash,dash,dash,dash,dash}, '0'};
Caracter n1 = {5, {dit,dash,dash,dash,dash}, '1'};
Caracter n2 = {5, {dit,dit,dash,dash,dash}, '2'};
Caracter n3 = {5, {dit,dit,dit,dash,dash}, '3'};
Caracter n4 = {5, {dit,dit,dit,dit,dash}, '4'};
Caracter n5 = {5, {dit,dit,dit,dit,dit}, '5'};
Caracter n6 = {5, {dash,dit,dit,dit,dit}, '6'};
Caracter n7 = {5, {dash,dash,dit,dit,dit}, '7'};
Caracter n8 = {5, {dash,dash,dash,dit,dit}, '8'};
Caracter n9 = {5, {dash,dash,dash,dash,dit}, '9'};

Caracter diccionario[26] = {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z};

//Declaracion de variables
char inChar;
String string="";

void traduccion(String palabra);

//Configuracion de puerto serial y reservacion de variable string
void setup() {
  Serial.begin(9600);
  string.reserve(200);
  pinMode(led13, OUTPUT);
}

void loop(){
  if (Serial.available()){
//Lectura de caracteres   
   inChar = Serial.read();
//Suma de caracteres en variable string
   string+=inChar;
   
//Imprime la variable con los caracteres acumulados hasta la ","   
   if (inChar=='\n'){
    Serial.print("Lectura: ");
    Serial.println(string);
//Borra la variable string para almacenar nuevos datos
    traduccion(string);
    string=""; 
   }
  }
}

void traduccion(String palabra){
  int x = palabra.length()-2;
  int i = 0;
  char caract;
  int j;
  int k;
  int vec[5];
  for(i=0; i<x; i+=1){
    caract = palabra[i];
    j = 0;
    if (caract == ' '){
        Serial.print(dit*5);
        Serial.println(" down ");
        digitalWrite(led13,LOW);
        delay(dit*5);
    }else{
    for(j=0; j<26; j+=1){
      if(diccionario[j].caract==caract){
        k=0;
        for(k=0; k<diccionario[j].cantidad; k+=1){
            Serial.print(diccionario[j].vector[k]);
            Serial.print(" Up ");
            digitalWrite(led13,HIGH);
            delay(diccionario[j].vector[k]);
            if(k == diccionario[j].cantidad-1){
              break;
            }
            Serial.print(dit);
            Serial.print(" down ");
            digitalWrite(led13,LOW);
            delay(dit);
        }
        Serial.print(dash);
        Serial.println(" down ");
        digitalWrite(led13,LOW);    
        delay(dash);
      }
     }
    }
  }
  return;
}



