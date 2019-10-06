/////////////   qrt library pololu   ///////////////
#include <QTRSensors.h>
#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
QTRSensorsRC qtrrc((unsigned char[]) { 7,8,9,10,11,12 },
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int r[NUM_SENSORS];
unsigned int Position;
/////////////   pid   ///////////////
float lasterror = 0;
float kp,ki,kd;
float error,derivative,output,integral;
float desiredvalue=2500;
//////// motors //////////////
int ena=6;
int enb=5;
int l1=A4;
int l2=A3;
int r1=A1;
int r2=A2;
int rms;
int lms;
int rbs=200;
int lbs=170;
#define max_speed 240
int Speed=100;
////////// shortest path //////////
char a[100]={};
int pathsize=100;
int n;
int z6,u2,g1;
int z1=0;int z2=0;int z5=0; int z7;
//// extrab switchs ///////
int shortswitch =12;
int ss,sw;
int i;

void setup()
{
  delay(500);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(enb,OUTPUT); 
  pinMode(l1,OUTPUT);
  pinMode(l2,OUTPUT);
  pinMode(r1,OUTPUT);
  pinMode(r2,OUTPUT);
  pinMode(12,INPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {  Serial.println(i);
     qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call
   }
  digitalWrite(13, LOW);  
  for (int i = 0; i < NUM_SENSORS; i++)
  { Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  { Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void loop()
{
  pid();
 if ((r[2]>600)&&(r[3]>600)){
 forward();}
 else{
  turndected();
 }
  
 }
   
void pid(){
  unsigned int position = qtrrc.readLine(r);
           kp=0.0425;
           kd=0.15;
        error=desiredvalue-position;
        derivative=error-lasterror;
        output=kp*error+kd*derivative;
        rms=rbs-output;
        lms=lbs+output;
  if(lms>max_speed){lms=max_speed;}
  if(lms<0){lms=0;}
  if(rms>max_speed){ rms=max_speed;}
  if(rms<0){rms=0;}
      analogWrite(ena,rms);
      analogWrite(enb,lms);
}
void turndected(){ 
  if((r[0]>600)&&(r[1]>600)&&(r[2]>600)&&(r[3]>600)&&(r[4]>600)&&(r[5]>600))
  {
    forward();
    delay(100);
    qtrrc.readLine(r);
    if((r[0]>600)&&(r[1]>600)&&(r[2]>600)&&(r[3]>600)&&(r[4]>600)&&(r[5]>600))
     {
         forward();
         delay(100);
         qtrrc.readLine(r);
          if((r[0]>600)&&(r[1]>600)&&(r[2]>600)&&(r[3]>600)&&(r[4]>600)&&(r[5]>600))
          {
            Serial.print("THE MAZE IS SOLVED");
            a[pathsize]='D';
            while(a[n]!='d'){n++;}
            Serial.print(n);Serial.print("    ");
            Serial.print("the obtained path is      ");
            for(int i=0;i<=n+1;i++){
              Serial.print(a[i]);
            }
            shortest_path();
           }}
    else{
      leftturn();
    }
  }
if((r[0]<400)&&(r[1]<400)&&(r[2]>600)&&(r[3]>600)&&(r[4]>600)&&(r[5]>600))
  {
  leftturn();
  }
  
if(((r[4]<400)||(r[5]<400))&&((r[2]>600)||(r[3]>600))&&((r[1]>600)||(r[0]>600))){
        forward();
        delay(2);
        qtrrc.readLine(r);
        if ((r[2]>600)||(r[3]>600)){
            forward();
            a[pathsize]='S';
            pathsize++;}
        else if((r[0]<400)&&(r[1]<400)&&(r[2]<400)&&(r[3]<400)&&(r[4]<400)&&(r[5]<400)){
                rightturn();}} 
if ((r[0]<400)&&(r[1]<400)&&(r[2]<400)&&(r[3]<400)&&(r[4]<400)&&(r[5]<400)){
    backturn();}
                }

void forward(){
      digitalWrite(l2,HIGH);
      digitalWrite(l1,LOW);
      digitalWrite(r1,HIGH);
      digitalWrite(r2,LOW);}
      
void Stop(){
      analogWrite(ena,0);
      analogWrite(enb,0);
      digitalWrite(l1,LOW);
      digitalWrite(l2,LOW);
      digitalWrite(r1,LOW);
      digitalWrite(r2,LOW);}
      
void backward(){
      analogWrite(ena,150);
      analogWrite(enb,135);
      digitalWrite(l1,HIGH);
      digitalWrite(l2,LOW);
      digitalWrite(r1,LOW);
      digitalWrite(r2,HIGH);}
      
void left(){
      analogWrite(ena,130);
      analogWrite(enb,120);
      digitalWrite(l1,HIGH);
      digitalWrite(l2,LOW);
      digitalWrite(r1,HIGH);
      digitalWrite(r2,LOW);}
      
void right(){
      analogWrite(ena,130);
      analogWrite(enb,120);
      digitalWrite(l2,HIGH);
      digitalWrite(l1,LOW);
      digitalWrite(r2,HIGH);
      digitalWrite(r1,LOW);}
        
void leftturn(){
     Stop();
     delay(200);
    while((r[3]>600)&&(r[2]>600)){
          left();
          delay(1);
          qtrrc.readLine(r);}
     while((r[3]<600)&&(r[2]<600)){
          left();
          delay(1);
          qtrrc.readLine(r);}
          Serial.print("LEFT  TUTN ");
          a[pathsize]='l';
          pathsize++;
     Stop();
     delay(200);}
void backturn(){
     Stop();
     delay(200);
     while((r[3]<600)&&(r[2]<600)){
          left();
          qtrrc.readLine(r);}
          Serial.print("BACK TURN");
           a[pathsize]='b';
          pathsize++;
     Stop();
     delay(200);}
     
void rightturn(){
     Stop();
     delay(200);
     while((r[3]<600)&&(r[2]<600)){
          right();
          qtrrc.readLine(r);}
          Serial.print("RIGHT  TUTN  ");
           a[pathsize]='r';
          pathsize++;
     Stop();
     delay(200);}
 
