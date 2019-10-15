#include <RGBmatrixPanel.h>
#include "PinChangeInterrupt.h"

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2


// To compile one needs to install Adafruit Gfx library 1.1.1, Adafruit RGB Matrix 1.5.6 & PinChangeInterrupt 1.2.6

// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);
// Double-buffered mode consumes nearly all the RAM available on the
// Arduino Uno -- only a handful of free bytes remain.  Even the
// following string needs to go in PROGMEM:

int8_t ball[3][4] = {
  {  3,  0,  1,  1 }, // Initial X,Y pos & velocity for 3 bouncy balls
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 }
};
static const uint16_t PROGMEM ballcolor[3] = {
  0x0080, // Green=1
  0x0002, // Blue=1
  0x1000  // Red=1
};

#define pinBlink 11

int signalPin0 = A4;
int signalPin1 = A5;

int left_score = 0;
int right_score = 0;
int delay_time = 500;

boolean blink_up;
boolean blink_down;
boolean winning_animation;


void setup() {
  
  matrix.begin();
  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setTextSize(1);


  // Choose a valid PinChangeInterrupt pin of your Arduino board
  pinMode(pinBlink, INPUT_PULLUP);

  pinMode(signalPin0, INPUT);
  pinMode(signalPin1, INPUT);

  pinMode(12, INPUT);
  pinMode(13, INPUT);
  
  winning_animation = false;
  left_score = 0;
  right_score = 0;
  blink_up = false;
  blink_down = false;
}

int valueC = 0;  // variable to store the value read
int valueD = 0;  // variable to store the value read
int valueB = 0;  // variable to store the value read
int valueA = 0;  // variable to store the value read

int digitalPinA = 13;
int digitalPinB = 12;
int serve = 0;
int consecutive_left_winnings = 0;
int consecutive_right_winnings = 0;

// I am not using a stack to keep the serve when the score is being decreased. not yet, due to lack of time :)
// also the consecutive X winnings are beiing wrongly reset when the score is added to the adversary
void showServe(void){

  int x = 0;
  if (serve == 1)
  {    
    for (int x = 1; x < 12; x++)
    {
      if (consecutive_left_winnings > 0 && consecutive_left_winnings % 5 == 0)
      {
        matrix.drawPixel(x, 14, matrix.Color333(1,0,0));
      }
      else
      {
        matrix.drawPixel(x, 14, matrix.Color333(0,0,1));
      }
    }
  }  
  if (serve == -1)
  {
    for (int x = 3; x < 14; x++)
    {
      if (consecutive_right_winnings > 0 && consecutive_right_winnings % 5 == 0)
      {
         matrix.drawPixel(16 + x, 14, matrix.Color333(1,0,0));
      }
      else
      {      
        matrix.drawPixel(16 + x, 14, matrix.Color333(0,0,1));
      }
    }
  }
}

void winningAnimation(void){
  byte i;
  // Bounce three balls around
  for(i=0; i<3; i++) {
    // Draw 'ball'
    matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
    // Update X, Y position
    ball[i][0] += ball[i][2];
    ball[i][1] += ball[i][3];
    // Bounce off edges
    if((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
      ball[i][2] *= -1;
    if((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
      ball[i][3] *= -1;
  }

}

void blinkOutline(int a, int b, int c) {
  // Switch Led state
  int x = 0;
  int y = 0;
 
  for (int x = 0; x < 32; x ++)
  for (int y = 0; y < 16; y ++)  
  { 
    if (x == 0 || y == 0 || x == 31 || y == 15) 
    matrix.drawPixel(x, y, matrix.Color333(a, b, c));
  }
}

void loop() {
  byte i;

  // Clear background
  matrix.fillScreen(0);
  
   if (blink_up)
   {
      blinkOutline(1,0,0);
      blink_up != blink_up;
   }

   if (blink_down)
   {
      blinkOutline(0,0,1);
      blink_down != blink_down;
   }

   showServe();

   if (winning_animation)
   {
      volatile int m;
      for(m=0; m<3; m++) {
      // Draw 'ball'
      matrix.fillCircle(ball[m][0], ball[m][1], 5, pgm_read_word(&ballcolor[m]));
      // Update X, Y position
      ball[m][0] += ball[m][2];
      ball[m][1] += ball[m][3];
      // Bounce off edges
      if((ball[m][0] == 0) || (ball[m][0] == (matrix.width() - 1)))
        ball[m][2] *= -1;
      if((ball[m][1] == 0) || (ball[m][1] == (matrix.height() - 1)))
        ball[m][3] *= -1;
      }
   }

  int x = 0;
  int y = 0;

  matrix.setCursor(1, 5);
  char tbs[5];
  sprintf(tbs, "%02d:%02d", left_score, right_score);
  matrix.print(tbs);
  
  // Update display
  matrix.swapBuffers(false);

  delay(100);
  valueD = analogRead(signalPin0);
  valueC = analogRead(signalPin1);
  valueB = digitalRead(digitalPinB);
  valueA = digitalRead(digitalPinA);


   if (blink_up == true)
   {
     blink_up = false;
   }

   if (blink_down == true)
   {
     blink_down = false;
   }
   
   
   if(valueA > 0 & valueB > 0)
    {
      delay(delay_time);
      valueA = digitalRead(digitalPinA);  // read the input pin
      valueB = digitalRead(digitalPinB);  // read the input pin
      if(valueA > 0 & valueB > 0)
      {
        left_score = 0;
        right_score = 0;
        serve = 0;
        winning_animation = false;
        consecutive_right_winnings = 0;
        consecutive_left_winnings = 0;
      }
      valueA = 0;
      valueB = 0;
      valueC = 0;
      valueD = 0;
     
      return;
    }
 
 //read analog values
 if (valueD > 900)
 {
   if (right_score > 0)
      {
     //   serve = 0;
        if (consecutive_right_winnings > 0)
        {
          consecutive_right_winnings --;
        }

        right_score -= 1;        
        delay(delay_time);
        blink_down = true;
        
        if (!((left_score >= 25  && (left_score - right_score > 1)) || (right_score >= 25 && (right_score - left_score > 1))))
        {
          winning_animation = false;
        }
      }
 } else
 
 if (valueC > 900)
 { 
   if (left_score > 0)
   {
    //  serve = 0;
      if (consecutive_left_winnings > 0)
      {
        consecutive_left_winnings --;
      }
      left_score -= 1;
      blink_down = true;        
      delay(delay_time);
      if (!((left_score >= 25  && (left_score - right_score > 1)) || (right_score >= 25 && (right_score - left_score > 1))))
      {
        winning_animation = false;
      }
   }
 }
   else
   if (valueA > 0)
    {
      consecutive_left_winnings++;
      consecutive_right_winnings = 0;
      serve = 1;
      left_score += 1;
      delay(delay_time);
      // win by 2
      if (left_score >= 25  && (left_score - right_score > 1))
        winning_animation = true;
       else
         blink_up = true;
    }
    else
    if (valueB > 0)
    {
      consecutive_left_winnings = 0;
      consecutive_right_winnings++;
      serve = -1;
      right_score += 1;
      delay(delay_time);
      // win by 2
      if (right_score >= 25 && (right_score - left_score > 1))
        winning_animation = true;
       else
         blink_up = true;
    };

     valueA = 0;
     valueB = 0;
     valueC = 0;
     valueD = 0;
}
