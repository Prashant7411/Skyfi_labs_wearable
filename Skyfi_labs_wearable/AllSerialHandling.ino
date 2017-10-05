void serialOutput(){
  //un-comment below line to see sensor raw data
  // sendDataToSerial('S',Signal); //goes to sendDataToSerial function
  
}


//Decides how to output BPM and IBI data
void serialOutputWhenBeatHappens(){
  if(serialVisual == true){  // code to make serial monitor visualizer work
    Serial.print("*** heart-Beat Happened ***");
    Serial.print("BPM:  ");
    Serial.print(BPM);
    Serial.print("  ");
  }else{
    sendDataToSerial('B',BPM); //send heart rate with a 'B' prefix
    sendDataToSerial('Q',IBI); //send time between beats with a 'Q' prefix
  }
}

//sends data from pulse sensor to the arduino IDE serial monitor or any third party serial readers
void sendDataToSerial(char symbol,int data){
  Serial.print(symbol);
  Serial.println(data);
}

