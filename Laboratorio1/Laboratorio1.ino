#include <TimerOne.h>
//variables Globales
long dit = 60000;     //Guarda el valor del DIT
long dash = dit*4;     //Valor del DASH con respecto al DIT
bool estado = false;   //Guarda el estado del led, asi sabemos si debe estar encendido o apagado
int contador = 0;      //Cuanta cuanto tiempo ha sido en su estado actual de la variable estado
long vx[100];          //Vector donde se guarda el mensaje a enviar
int cuantos=0;         //Número de posiciones del vector del mensaje a leer
int iVector;           //Posición del vector actual
bool variable = false; //Variable para saber si ya se leyó una palabra
char inChar;           //Caracter para leer 
String string="";      //Palabra leida    
bool bandera = false;       //Que hago? TRUE = Escribir FALSE = Leer


//Para la lectura del fotoResistor
long tiempo;
long tiempo1;
bool estado2 = true;
long acumulador[5];
int pos=0;


//LEDs a utilizar
int led13 = 13;
int lectura = 2;    
int leer = 3;
int led6 = 6;

//Estructura
struct Caracter
{  
  int cantidad;
  long vector[5];
  char caract;
};

//Diccionario
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
  {4, {dit,dash,dash,dit}, 'p'},
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

//Declaracion de metodos
void traduccion(String palabra);
void cambioEstado();
void leerFotoresistencia();
char buscarCaracter(long acumulador[5], int pos);
void quehacer();
void leerPalabra();

void setup() 
{
  pinMode(led13, OUTPUT);
  pinMode(led6, OUTPUT);
  //pinMode(lectura, INPUT);   
  //pinMode(leer, INPUT);   
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(leer), quehacer, CHANGE);
  if(!bandera){
    Timer1.initialize(dit);
    Timer1.attachInterrupt(timerIsr);
  }else if(bandera){
    Timer1.initialize(50000000);
    attachInterrupt(digitalPinToInterrupt(lectura), leerFotoresistencia, CHANGE);
  }
}

void loop(){
  leerPalabra();
  }

void leerPalabra(){
  if (Serial.available()){
  //Lectura de caracteres   
  inChar = Serial.read();
  //Suma de caracteres en variable string
  string+=inChar;
   
   //Imprime la variable con los caracteres acumulados hasta el "\n"   
   if (inChar=='\n'){
      digitalWrite(led6,HIGH);
      delay(1000);
      Serial.print("Lectura: ");
      Serial.println(string);
      //Borra la variable string para almacenar nuevos datos
      traduccion(string);
      string=""; 
   }
  }
}

void quehacer(){
  Serial.println("HOLA");
  bandera = !bandera;
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
    variable = false;
    estado = false;
    iVector =0;
    digitalWrite(led6,LOW);
  }
  contador = contador+1;
}

void leerFotoresistencia(){
    tiempo = Timer1.read();
    Timer1.restart();
    tiempo1 = (tiempo+10000)/(dit);
    tiempo1 = tiempo1*dit;
    if(!estado2){
       if(tiempo1 == dash){
          inChar = buscarCaracter(acumulador, pos);
          Serial.print(inChar); 
          pos = 0;
       }else if(tiempo1 == dit*5){
          inChar = buscarCaracter(acumulador, pos);
          Serial.print(inChar);
          Serial.print('_');
          pos = 0;
       }
    }else{
        if(tiempo1<dit*5){
          acumulador[pos] = tiempo1;
          pos += 1;
        }else{
          estado2 = !estado2;
        }
    }
    estado2 = !estado2;
}
char buscarCaracter(long acumulador[5], int pos){
   int i = 0;
   for(i=0; i<36; i+=1){
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

void traduccion(String palabra){
  int i = 0;
  int j;
  int k;
  variable = true;
  cuantos = 0;         
  // se restan dos para que no dibuje el \n
  for(i=0; i<palabra.length()-2; i+=1){
    inChar = palabra[i];
    j = 0;
    if (inChar == ' '){
        vx[cuantos]=dit*5;
        cuantos = cuantos+1;
    }else{
    for(j=0; j<36; j+=1){
      if(diccionario[j].caract==inChar){
        k=0;
        for(k=0; k<diccionario[j].cantidad; k+=1){
            vx[cuantos]=diccionario[j].vector[k];
            cuantos = cuantos+1;
            if(k == diccionario[j].cantidad-1){
              break;
            }
            vx[cuantos]=dit;
            cuantos = cuantos+1;
        }
        if(palabra[i+1]!=' '){
          vx[cuantos]=dash;
          cuantos = cuantos+1;
        }
      }
     }
    }
  }
  contador=0;
  estado = !estado;
}



