//#include <PWM.h>
#include <PID_v1.h>

// Constants
const int BP1 = A6;
const int BP2 = A4;
const int SetPoint_pin = A5;
const int MB1 = 7;
const int MB2 = 10;
const int period = 4000; // microseconds
const int32_t frequency = 100; // Hz
const int Step = 15; // 0 - 255 duty step

// User Input
const char pulse1_max = '3';
const char pulse1_up = '2';
const char pulse1_down = '1';
const char pulse1_min = '`';
const char pulse2_max = '=';
const char pulse2_up = '-';
const char pulse2_down = '0';
const char pulse2_min = '9';
const char set_up = 'e';
const char set_down = 'w';
const char set_max = 'r';
const char set_min = 'q';

// PID Set Point Control
double setp_max = 5; // max 5 bar feedback/output max
double setp_min = 0; // min 1 bar feedback/output min
double setp_bar = 0; // variable input set by user
double setp_LIM = 4;

// PID Input
//double volt1; // pressure transducer 1 (bin)
//double volt2; // pressure transducer 2 (bin)
double input_bar_1; // pressure transducer 1 (bar)
double input_volts_1; // pressure transducer 1 (bar)
double input_bar_2; // pressure transducer 2 (bar)
double input_volts_2; // pressure transducer 2 (bar)
double input3; // pressure transducer 3 (bar)
double output; // PWM (PID controlled)
double output1; // PWM (PID controlled)
double output2; // PWM (PID controlled)
double pid_threshold = 0.3; //

// PID Constants
double kp = 1.0 / 5 * 1.5 * 0.7; //*0.5*.5*.5;
double ki = 3 / 0.1 / 10 * 2 * 0;
double kd = 0;

PID ValveControl(&input_bar_2, &output, &setp_bar, kp, ki, kd, DIRECT);

// Variables
volatile int tic = 0; // tic cycle to serial print
char keybrd_input; // user input
int pulse1 = 0; // inital 0 microsecond pulse width (PWM manual control)
int pulse2 = 0; // inital 0 microsecond pulse width (PWM manual control)


void setup() {
  //InitTimersSafe();
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(BP1, INPUT);
  pinMode(BP2, INPUT);
  //SetPinFrequencySafe(MB1, frequency);
  //SetPinFrequencySafe(MB2, frequency);
  setp_bar = 0;
  ValveControl.SetMode(AUTOMATIC);
  ValveControl.SetOutputLimits(-1, 1);
  //cur_time = micros()
  //pre_time = micros()
  Serial.begin(115200);
}


void loop() {

  input_volts_1 = analogRead(BP1) * (5.0 / 1024.0 * 2.0); // pressure (bar)
  input_volts_2 = analogRead(BP2) * (5.0 / 1024.0 * 2.0); // pressure (bar)
  input_bar_1 = (input_volts_1 - 0.1)/9.9*16;
  input_bar_2 = (input_volts_2 - 0.1)/9.9*16;
  setp_bar = (analogRead(SetPoint_pin) / 1024.0) * setp_LIM;


  ValveControl.Compute();

  if (output > 1) {
    output = 1;
  }
  if (output < -1) {
    output = -1;
  }

  if (output >= 0) {
    output1 = (1 - abs(output));
  } else {
    output1 = 1;
  }

  if (output < 0) {
    output2 = abs(output);
  } else {
    output2 = 0;
  }
/*
  if (abs(setp_bar - input_bar_2) > pid_threshold) {
    pwmWrite(MB1, output1 * 255);
    pwmWrite(MB2, output2 * 255);
  } else {
    pwmWrite(MB1, 255);
    pwmWrite(MB2, 255);
  }
*/
  
  tic++;
  if (tic == 100) {
    tic = 0;
    Serial.print(micros()); Serial.print(", ");
    Serial.print(setp_bar); Serial.print(", ");
    Serial.println(input_bar_2); Serial.print(", ");
    /*
    Serial.print("PID_out:");
    Serial.print(output);
    Serial.print('\t');

    Serial.print("Duty1:");
    Serial.print(output1);
    Serial.print('\t');

    Serial.print("Duty2:");
    Serial.print(output2);
    Serial.print('\t');

    Serial.print("PV = ");
    Serial.print(input_bar_2);
    Serial.print(" bar\t");

    Serial.print(" (");
    Serial.print(input_volts_2);
    Serial.print(" V)\t");

    Serial.print("SetP:");
    Serial.println(setp_bar);
    */

  } //if(tic==100)
} //main()
