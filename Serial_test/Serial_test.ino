void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Run");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    int val = Serial.read();
    Serial.println(val);
  }
}
