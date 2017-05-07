#include <TimerOne.h>
long dit = 100000;
bool estado = false;
int contador = 0;
long vx[100];
int iVector;
long dash = dit*4;
int led13 = 13;
bool variable = false;
struct Caracter
{  
  int cantidad;
  long vector[5];
  char caract;
};

Caracter diccionario[36] = 
  {{2, {dit,dash}, 'a'},
  {4, {dash,dit,dit,dit}, 'b'},
  {4, {dash,dit,dash,dit}, 'c'},
  {3, {dash,dit,dit}, 'd'},
  {1, {dit}, 'e'},
  {4, {dit,dit,dash,dit}, 'f'},
  {3, {dash,dash,dit}, 'g'},
  {4, {dit,dit,dit,dit}, 'h'},
  {2, {dit,dit}, 'i'},
  {4, {dit,dash,dash,dash}, 'j'},
  {3, {dash,dit,dash}, 'k'},
  {4, {dit,dash,dit,dit}, 'l'},
  {2, {dash,dash}, 'm'},
  {2, {dash,dit}, 'n'},
  {3, {dash,dash,dash}, 'o'},
  {3, {dit,dash,dash,dit}, 'p'},
  {4, {dash,dash,dit,dash}, 'q'},
  {3, {dit,dash,dit}, 'r'},
  {3, {dit,dit,dit}, 's'},
  {1, {dash}, 't'},
  {3, {dit,dit,dash}, 'u'},
  {4, {dit,dit,dit,dash}, 'v'},
  {3, {dit,dash,dash}, 'w'},
  {4, {dash,dit,dit,dash}, 'x'},
  {4, {dash,dit,dash,dash}, 'y'},
  {4, {dash,dash,dit,dit}, 'z'},
  {5, {dash,dash,dash,dash,dash}, '0'},
  {5, {dit,dash,dash,dash,dash}, '1'},
  {5, {dit,dit,dash,dash,dash}, '2'},
  {5, {dit,dit,dit,dash,dash}, '3'},
  {5, {dit,dit,dit,dit,dash}, '4'},
  {5, {dit,dit,dit,dit,dit}, '5'},
  {5, {dash,dit,dit,dit,dit}, '6'},
  {5, {dash,dash,dit,dit,dit}, '7'},
  {5, {dash,dash,dash,dit,dit}, '8'}, 
  {5, {dash,dash,dash,dash,dit}, '9'}};

//Declaracion de variables
char inChar;
String string="";
int cuantos=0;
void traduccion(String palabra);
void cambioEstado();
void setup() 
{
  pinMode(13, OUTPUT);   
  Serial.begin(9600);
  
  Timer1.initialize(dit);
  Timer1.attachInterrupt(timerIsr);
}

void loop()
{
  if (Serial.available()){
  //Lectura de caracteres   
   inChar = Serial.read();
   //Suma de caracteres en variable string
   string+=inChar;
   
   //Imprime la variable con los caracteres acumulados hasta el "\n"   
   if (inChar=='\n'){
      Serial.print("Lectura: ");
      Serial.println(string);
      //Borra la variable string para almacenar nuevos datos
      traduccion(string);
      string=""; 
   }
  }
}

void timerIsr()
{
  cambioEstado();
  if(!estado){
    digitalWrite(led13,LOW);  
  }else{
    digitalWrite(led13,HIGH);  
  }
  
}

void cambioEstado(){
  long x = dit*contador;
  if(vx[iVector]==x & variable & iVector<cuantos){
    estado = !estado;
    iVector = iVector+1;
    contador=0;
  }
  if(iVector==cuantos){
    estado = false;
  }
  contador = contador+1;
}

void traduccion(String palabra){
  variable = true;
  int x = palabra.length()-2;
  int i = 0;
  char caract;
  int j;
  int k;
  int vec[5];
  int c = 0;
  iVector =0;
  for(i=0; i<x; i+=1){
    caract = palabra[i];
    j = 0;
    if (caract == ' '){
        vx[c]=dit*5;
        c = c+1;
    }else{
    for(j=0; j<26; j+=1){
      if(diccionario[j].caract==caract){
        k=0;
        for(k=0; k<diccionario[j].cantidad; k+=1){
            vx[c]=diccionario[j].vector[k];
            c = c+1;
            if(k == diccionario[j].cantidad-1){
              break;
            }
            vx[c]=dit;
            c = c+1;
        }
        if(palabra[i+1]!=' '){
          vx[c]=dash;
          c = c+1;
        }
      }
     }
    }
  }
  contador=0;
  cuantos=c;
  estado = !estado;
}
