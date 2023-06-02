#define batpin 36

int vbat = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(batpin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  vbat = analogRead(batpin);
  Serial.println("vbat:" + String(vbat));
  delay(1000);
}
