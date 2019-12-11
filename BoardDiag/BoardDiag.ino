const int BP1 = A6;
const int MB1 = 7;
int tic = 0;
int pressure1;

void setup() {
  pinMode(MB1, OUTPUT);
  pinMode(BP1, INPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(MB1, HIGH);
  pressure1 = analogRead(BP1);

  tic++;
  if(tic==100){
    tic=0;
    Serial.print(micros()); Serial.print(", ");
    Serial.print(pressure1); Serial.print('\n');
  }
}
