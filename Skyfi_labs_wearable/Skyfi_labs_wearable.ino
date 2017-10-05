#include "TM1637.h"
#include "SevenSegmentFun.h"
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

#define CLK 9 //pins for TM1637
#define DIO 10

TM1637 tm1637(CLK,DIO);

//variables
int pulsePin = 0; //pulse sensor signal pin connected to analog pin 0
int blinkPin = 13; //pin to blink led at each beat

//volatile variables, used in the interrupt service routine!
volatile int BPM; //int holds raw analog from 0 pin, updated every 2ms
volatile int Signal; //holds the incoming raw data
volatile int IBI = 600; //int thtat holds the time interval between beats! must be seeded!
volatile boolean Pulse = false; //"TRUE" when user's live heartbeat is detected. "FALSE" when not a "live beat"
volatile boolean QS = false; //becomes true when lillypad arduino finds a beat.

//regards serial output--set this as false
static boolean serialVisual = false;

void setup(){
  pinMode(blinkPin,OUTPUT); // pin that will blink to your hearbeat
  Serial.begin(115200); // sets the baud rate
  display1(); // calls the display1 function
  tm1637.init(); //initialze display
  tm1637.set(BRIGHT_DARKEST); //sets the brightness of display
  interruptSetup(); //sets up to read pulse sensor signal every 2ms
  //if you are poering the pulse sensor at voltage less than the board voltage,
  //un-comment the next line and apply that voltage to the -ref pin
  //analogReference(EXTERNAL);
}

void loop(){
  serialOutput();
  if(QS == true){     //a heartbeat was found
                      //BPM and IBI have been determined
                      //quantified self "QS" true when arduino finds a heartbeat
                      serialOutputWhenBeatHappens(); // A beat happened, output that to serial.
                      QS = false; //reset the Quantified  self flagfor next time
                  

                  int hundred = ((BPM/100)%10);
                  int tens = ((BPM/10)%10);
                  int unit = BPM % 10;

                  Serial.println(hundred); //display BPM on serial monitor
                  Serial.println(tens);
                  Serial.println(unit);

                  tm1637.point(POINT_OFF);
                  tm1637.display(0,11); // to display b in first segment
                  tm1637.display(1,hundred); // to display in second segment
                  tm1637.display(2,tens); // to display in third segment
                  tm1637.display(3,unit); // to display in fourth segment
                 
  }
   delay(20); // take a break before next loop               
}



