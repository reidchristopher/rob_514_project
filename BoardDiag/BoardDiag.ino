#include <AutoPID.h>
#include "MovingAverageFilter.h"
#include "SetPointPot.h"

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

AutoPIDRelay controller(&p1_bar, &setp, &switch_on, ts, kp, ki, kd);

SetPointPot set_point_pot(15, 0, 5);

void setup() {
  pinMode(MB1, OUTPUT);
  pinMode(BP1, INPUT);
  controller.setTimeStep(10);
  Serial.begin(115200);
  cur_time = millis();
  pre_time = millis();
}

void loop() {
  pressure1 = analogRead(BP1);
  float raw_p1_bar = pressure1*0.03163 - 1.2372;
  p1_bar = filter.getValue(raw_p1_bar);

  setp = set_point_pot.getSetPoint();

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
  
  tic++;
  if(tic==100){
    tic=0;
    //Serial.print(micros()); Serial.print(", ");
    Serial.print(p1_bar); Serial.print(",");
    Serial.print(raw_p1_bar); Serial.print(",");
    Serial.print(setp); Serial.print(",");
//    Serial.print(pulse); Serial.print(", ");
//    Serial.print(switch_on); Serial.print(", ");
    Serial.print(real_switch + 2); Serial.print('\n');
  }
}
