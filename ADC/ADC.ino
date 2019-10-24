
int x; //Reads the analog value on pin A3 into x
int y;
void setup() {
  // put your setup code here, to run once:
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(A3);
  y = analogRead(A2);
  Serial.print("Analog valueX:");
  Serial.print(x);
  Serial.print("  Analog valueY:");
  Serial.println(y);
}
