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
int numero;
bool estado = true;
String inString = "";    // string to hold input

int acumulador[5];
int pos=0;

char car;

char buscarCaracter(int acumulador[5], int pos);

struct Caracter
{  
  int cantidad;
  int vector[5];
  char caract;
};

int dit = 6;
int dash = dit*4;

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
Caracter z = {4, {dash,dash,dit,dit}, 'z'};

Caracter diccionario[26] = {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z};

void setup() {
  // Open serial communications and wait for port to open:
    
    
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("\n\nString toInt():");
  Serial.println();
}

void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char
      // and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string,
    // then the string's value:
    if (inChar == '\n') {
        //Serial.print("Value:");
        //Serial.println(inString.toInt());
        numero = inString.toInt();
       
        if(!estado){
           if(numero == dit){
           }
           else if(numero == dash){
              car = buscarCaracter(acumulador, pos);
              Serial.print(car); 
              pos = 0;
           }else if(numero == dit*5){
              car = buscarCaracter(acumulador, pos);
              Serial.print(car);
              Serial.print('_');
              pos = 0;
           }
        }else{
              acumulador[pos] = numero;
              pos += 1;
        }
        estado = !estado;
        inString = "";
      }
     
    }
  }



char buscarCaracter(int acumulador[5], int pos){
   int i = 0;
   for(i=0; i<26; i+=1){
     if(pos == diccionario[i].cantidad){
        int j = 0;
        for(j=0; j<pos; j+=1){
          if(diccionario[i].vector[j] != acumulador[j]){
            break;
          }
          if(pos==j+1){
            return diccionario[i].caract;
          }
        } 
     }
   }
   return 'N';
};
