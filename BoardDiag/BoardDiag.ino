#include <AutoPID.h>
#include <MovingAverageFilter.h>

MovingAverageFilter filter(1000);

float kp = 2.0;
float ki = 1.0;
float kd = 0;

float ts = 100;
float trise = 5; //ms
float tfall = 5; //ms

double p1_bar;
double on_time;
double setp = 0.50;

const int BP1 = A6;
const int MB1 = 6;
int tic = 0;
int pressure1;

unsigned long cur_time;
unsigned long pre_time;

bool switch_on;
bool real_switch;

//AutoPID controller(&p1_bar, &setp, &on_time, 0, ts, kp, ki, kd);
AutoPIDRelay controller(&p1_bar, &setp, &switch_on, ts, kp, ki, kd);

void setup() {
  pinMode(MB1, OUTPUT);
  pinMode(BP1, INPUT);
  //controller.SetMode(AUTOMATIC);
  //controller.SetOutputLimits(0,ts);
  controller.setTimeStep(10);
  Serial.begin(115200);
  cur_time = millis();
  pre_time = millis();
}

void loop() {
  pressure1 = analogRead(BP1);
  //p1_bar = pressure1*0.03163 - 1.2372;
  float raw_p1_bar = pressure1*0.03163 - 1.2372;
  p1_bar = filter.getValue(raw_p1_bar);

  controller.run();
  double pulse = controller.getPulseValue() * ts;
  
  if(pulse < (trise+tfall)){
    real_switch = false;
  }
  else if(pulse > (ts-trise-tfall)){
    real_switch = true;
  }
  else{
    real_switch = switch_on;
  }
  digitalWrite(MB1,real_switch);
  
  /*
  if (on_time < trise) { on_time = trise; }
  if (on_time > ts-tfall) { on_time = ts; }
  cur_time = millis();
  if (cur_time - pre_time > ts){
    pre_time = cur_time
  }
  if (cur_time-pre_time > on_time){
    digitalWrite(MB1, HIGH);
  }
  else{
    digitalWrite(MB1, LOW);
  }
  */
  
  //digitalWrite(MB1, HIGH);
  
  tic++;
  if(tic==100){
    tic=0;
    //Serial.print(micros()); Serial.print(", ");
    Serial.print(p1_bar); Serial.print(",");
    Serial.print(raw_p1_bar); Serial.print(",");
//    Serial.print(pulse); Serial.print(", ");
//    Serial.print(p1_bar); Serial.print(", ");
//    Serial.print(switch_on); Serial.print(", ");
    Serial.print(real_switch + 2); Serial.print('\n');
  }
}
