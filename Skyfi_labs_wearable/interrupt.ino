
volatile int rate[10]; // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0; //used to determine pulse timing
volatile unsigned long lastBeatTime = 0; //use to find IBI
volatile int P = 512; //used to find peak in pulse wave, seeded
volatile int T = 512; //used to find trough in pulse wave, seeded
volatile int thresh = 525; //used to find instant moment of heart beat, seeded
volatile int amp = 100; //used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true; //used to seed rate array so we startup with reasnable BPM
volatile boolean secondBeat = false; //used to seed rate array so we startup with reasonablr BPM

void interruptSetup(){
  //Initializes Timer2 to throw an interrupt every 2ms
  TCCR2A = 0x02; //disable PWM on digital pins 3 and 11, and go into CTC mode
  TCCR2B = 0x05; //dont force comapare, 256 prescaler
  OCR2A = 0x7C; //set the top of the count to 124 for 500Hz sample rate
  TIMSK2 = 0x02; //enable interrupt on match between TIMER2 and OCR2A
  sei(); //make sure global interrupts are enabled
}

//This is the TIMER 2 interrupt service routine
//TIMER 2 makes sure that we take a reading every 2 milliseconds
ISR(TIMER2_COMPA_vect){  //triggered when Timer2 counts to 124
  cli();                 //disable interrupts while we do this
  Signal = analogRead(pulsePin); //read the pulse sensor
  sampleCounter += 2;
  //keep track of the time in ms with this variable
  int N = sampleCounter - lastBeatTime; //monitor the time since the last beat to avoud noise

  //find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){  //avoid dichrotic noise by waiting 3/5 of last IBI
    if(Signal < T){   //T is trough
      T = Signal; //keep track of lowest point in pulse wave
    }
    
  }


  if(Signal > thresh && Signal > P){    //thresh condition helps avoid noise
    P = Signal;  //P is the peak
  }              //keep track of highest point in the pulse wave

  //Now its time to look for the heart beat
  //signal surges up in value every time there is a pulse
  if(N > 250){
    if((Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3)){
      Pulse = true; //set the pulse flag when we think there is a pulse
      digitalWrite(blinkPin,HIGH); //turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime; //measure time between beats in ms
      lastBeatTime = sampleCounter; //keep track of time for next pulse

      if(secondBeat){     //if this is the second beat, if secondBeat== TRUE
        secondBeat= false; //clear secondBeat flag
        for(int i=0;i<=9;i++){    //seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;
        }
        
      }

      if(firstBeat){      //if its the first time we found a beat, if firstBeat == TRUE
        firstBeat = false; //clear firstBeat flag
        secondBeat = true; //set the second beat flag
        sei(); //enable interrupts again
        return; //IBI value is unreliable so discard it
      }

      //keep a running total of the last 10 IBI values
      word runningTotal = 0; //clear the runningTotal variable
      for(int i=0;i<=8;i++){      //shift data in the rate array
        rate[i] = rate[i+1];      //and drop the oldest IBI value
        runningTotal += rate[i];  // add up the oldest IBI values
        }
        rate[9] =IBI; // add the latest IBI to the rate array
        runningTotal += rate[9]; //add the latest IBI to runningTotal
        runningTotal /=10;        // average the last 10 IBI values
        BPM = 60000/runningTotal; //how manybeats can fit into a minute? thats BPM
        QS = true; //set Quabtified self flag
        //QS flag is not cleared inside this ISR
    }
  }
     if(Signal < thresh && Pulse == true){  //when the value are going down, the beat is over
      digitalWrite(blinkPin,LOW); //turn off pin 13 LED
      Pulse = false; //reset the pulse flag so we can do it again
      amp = P - T;  //get amplitude of the pulse wave
      thresh = amp/2 + T; //set thresh at 50% of the amplitude
      P = thresh; //reset these for next time
      T = thresh;
     }

     if(N > 2500){    //if 2.5 seconds go by without a beat
      thresh = 512;  //set thresh default
      P = 512; //set P default
      T = 512; //set T default
      lastBeatTime = sampleCounter; //bring the lastBeatTime up to date
      firstBeat = true; //set these to avoid noise
      secondBeat = false; //when we gwt the heartbeat back
     }
  sei();  //enable interrupts when your done!
 }//end isr

