int motor1  = 9;
int motor11 = 8;
int motor2 = 7;
int motor22 = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1, OUTPUT);
  pinMode(motor11, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor22, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(motor11, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(motor2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(motor2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(motor11, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}
