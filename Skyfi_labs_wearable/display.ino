

//function to display the animating effects
void display1(){
                SevenSegmentFun display(9,10); //pins for tm1637
                display.begin();    //initializes the display
                display.setBacklight(100); //set brightness of display to hundred percent
                byte repeats = 1; //this variable tells how many times the scrolling text should be displayed
                display.scrollingText("INITIALIZING",repeats); //display INITIALIZING on the display
                unsigned int numMoves = 100; unsigned int timeDelay = 100;
                display.bouchingBall(numMoves,timeDelay);
                display.clear(); //to clear the display
              
}

