#include <QTRSensors.h>
#define NUM_SENSORS   6      // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
QTRSensorsRC qtrrc((unsigned char[]) {7,8,9,10,12,5},NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int r[NUM_SENSORS];
unsigned int Position;
float lasterror = 0;
float kp,ki,kd;
float error,derivative,output,integral;
float desiredvalue=2500;// changes with no of sensors
int r1=11;
int r2=10;
int l1=9;
int l2=6;
int speedl=55;
int speedr=50;
int speed=60;
int rms;
int lms;
#define max_speedr 150
#define max_speedl 150
void setup()
{ Serial.begin(9600);
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {Serial.println(i);
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}
void loop(){
 pid();
 Serial.println("   ");
 }
void pid(){
unsigned int position = qtrrc.readLine(r);
for (unsigned char i = 0; i < NUM_SENSORS; i++){
Serial.print(r[i]);
Serial.print('\t');}
Serial.print(position); // comment this line out if you are using raw values
Serial.print("    " );
         kp=0.01;//0.009;//0.01//0.018;//0.02;    //0.0425;
         kd=0;//0.007;//0.009;     //0.15;
         //ki=0;
         error=desiredvalue-position;
         derivative=error-lasterror;
         //integral=lasterror+error;
         output=kp*error+kd*derivative;//ki*integral;
         Serial.print(error);
         Serial.print("      ");
         Serial.print(output);
         Serial.print("    ");
         lasterror=error;
         rms=speedr-output;
         lms=speedl+output;
         if(lms>max_speedl){lms=max_speedl;}
         if(lms<0){lms=0;}
         if(rms>max_speedr){ rms=max_speedr;}
         if(rms<0){rms=0;}
         Serial.print(rms);
         Serial.print("   ");
         Serial.print(lms);
         analogWrite(r1,rms);
          analogWrite(r2,0);
          analogWrite(l1,lms);
          analogWrite(l2,0);
         
 }
void forward()
{      
  analogWrite(r2,0);
  analogWrite(l2,0);
}
void backward()
{
  analogWrite(r2,speed);
  analogWrite(r1,0);
  analogWrite(l2,speed);
  analogWrite(l1,0);
}
void left(){
  analogWrite(r1,speed);
  analogWrite(r2,0);
  analogWrite(l1,0);
  analogWrite(l2,speed);
}
void right()
{
  analogWrite(r1,0);
  analogWrite(r2,speed);
  analogWrite(l1,speed);
  analogWrite(l2,0);
}
void stop(){
  analogWrite(r1,0);
  analogWrite(r2,0);
  analogWrite(l1,0);
  analogWrite(l2,0);
}
