int LED[4] = {D8, D7, D6, D5};
int BUTTON[4] = {D1, D2, D3, D4};
int sequence[300];
int gameover = 0;
int lastbutton = 0;
int maxlength = 300;

void setup() {
  pinMode(LED[0], OUTPUT);
  pinMode(LED[1], OUTPUT);
  pinMode(LED[2], OUTPUT);
  pinMode(LED[3], OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  
  //=========================================================================================================
  //Better random seed generator from https://www.utopiamechanicus.com/article/better-arduino-random-numbers/
  //=========================================================================================================
    unsigned long seed=0, count=32;
    while (--count)
      seed = (seed<<1) | (analogRead(0)&1);
    randomSeed(seed);
  //=========================================================================================================
}

void loop() {

  getsequence(); //Set up pattern sequence array
  gameover = 0; //Reset gameover flag
  startgame(); //start the game!

  //Main game loop
  for (int sequencelength = 0; sequencelength < maxlength; sequencelength++) {
    
    //Play back the current sequence iteration
    for (int playbackstep = 0; playbackstep <= sequencelength; playbackstep++) {
      delay(200);
      digitalWrite(LED[sequence[playbackstep]], HIGH);
      delay(700);
      digitalWrite(LED[sequence[playbackstep]], LOW);
    }

    //Then sub loop to receive input sequence
    for (int inputstep = 0; inputstep <= sequencelength; inputstep++) {
      //Delay here until player presses a button
      do {
        delay(100);
      } while (digitalRead(BUTTON[0]) == HIGH && digitalRead(BUTTON[1]) == HIGH && digitalRead(BUTTON[2]) == HIGH && digitalRead(BUTTON[3]) == HIGH);

      //light up the LED for whichever button is pressed, and pause until button is released.
      do {
        for (int i = 0; i < 4; i++) {
          digitalWrite(LED[i], !digitalRead(BUTTON[i]));
          //Also set lastbutton flag.
          if (digitalRead(BUTTON[i]) == LOW) {
            lastbutton = i;
          }
        }
      } while (digitalRead(BUTTON[0]) == LOW || digitalRead(BUTTON[1]) == LOW || digitalRead(BUTTON[2]) == LOW || digitalRead(BUTTON[3]) == LOW);

      lightsout(); //Make sure all LEDs are now off...

      if (lastbutton == sequence[inputstep]) {
        //Button was correct! just delay a little and continue...
        delay(200);
      }
      else {
        //otherwise game over
        gameover = 1;
        break;
      }

      //Last button must have been correct. Looping back to wait for next button press.

    }

    if (gameover == 1) {break;} //Game over, go back to beginning

    //Otherwise, loop back for another round.

  }

}

//Initialize pattern sequence
void getsequence()
{
  for (int i = 0; i < maxlength; i++) {
    sequence[i] = random(0, 4);
  }
}

//Start the game. Flash all LEDs forever until user presses a button.
void startgame()
{

  //Pause just in case the user still has a button pressed from the last round...
  //delay(400);

  //Flash all LEDs, and wait for user to press any button
  int i = 0;
  do {
    if (i > 3) {i = 0;}
    digitalWrite(LED[i], HIGH);
    delay(80);
    digitalWrite(LED[i], LOW);
    i++;
  } while (digitalRead(BUTTON[0]) == HIGH && digitalRead(BUTTON[1]) == HIGH && digitalRead(BUTTON[2]) == HIGH && digitalRead(BUTTON[3]) == HIGH);

  //Leaving this chunk of debug code, due to occasional boot-up glitch...
  //Serial.println("Button pressed! Starting game!");
  //Serial.print(!digitalRead(BUTTON[0]));
  //Serial.print(!digitalRead(BUTTON[1]));
  //Serial.print(!digitalRead(BUTTON[2]));
  //Serial.println(!digitalRead(BUTTON[3]));

}

void lightsout()
{
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED[i], LOW);
  }
}
