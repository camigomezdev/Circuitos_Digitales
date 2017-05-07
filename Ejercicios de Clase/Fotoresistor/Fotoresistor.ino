int varVolt;
int led7 = 7;
int led8 = 8;
int led9 = 9;
int led10 = 10;
int lectura = A0;

void funcion();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//Comunicacion serial a 9600 bits/seg
  pinMode(lectura, INPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  funcion();
}

void funcion(){
  varVolt = analogRead(lectura);
  Serial.println(varVolt);
  //Serial.read();
  if (varVolt<=450){
    digitalWrite(led10, LOW);
    digitalWrite(led9, LOW);
    digitalWrite(led8, LOW);
  }
  else if(varVolt>=450 && varVolt<=600){
    digitalWrite(led10, HIGH);
    digitalWrite(led9, LOW);
    digitalWrite(led8, LOW);
  } else if (varVolt>600 && varVolt<=800){
    digitalWrite(led10, HIGH);
    digitalWrite(led9, HIGH);
    digitalWrite(led8, LOW);
  } else{
    digitalWrite(led10, HIGH);
    digitalWrite(led9, HIGH);
    digitalWrite(led8, HIGH);
  }
  
 }
  
