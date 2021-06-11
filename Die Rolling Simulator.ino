#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,6,5,4,3); // initialise the lcd library
// assign components to pins
const int contrastPin = 8;
const int tiltPin = 7;
const int button = 2;
const int pot = A1;
// declare variables
int contrast = 75;
int tiltState = 0;
int prevTiltState = 0;
int dieLimit = 0;
int potVal = 0;
int isRolling = 0;
unsigned long previousInterrupt; //last time an interrupt is registered

// get the die to be rolled from the potentiometer and print it on the lcd
void getDieValue() {
  // change the type of die to be rolled according to potVal
  potVal = analogRead(pot);
  delay(10);
  if (potVal < 100){ // if potentiometer value is less than 100
    dieLimit = 3;  // set dieLimit to 3 which correlates to a D2
  }
  else if (potVal < 250){
    dieLimit = 5; // D4
  }
  else if (potVal < 400){
    dieLimit = 7; // D6
  }
  else if (potVal < 550){
    dieLimit = 9; // D8
  }
  else if (potVal < 700){
    dieLimit = 11; // D10
  }
  else if (potVal < 850){
    dieLimit = 13; // D12
  }
  else if (potVal > 850){
    dieLimit = 21; // D20
  }
  // print the die to be rolled on the lcd
  lcd.setCursor(7,1);
  lcd.print("D");
  lcd.setCursor(8,1);
  lcd.print(dieLimit-1);
  lcd.print("       ");
}

void setup() {
  Serial.begin(9600); // include a serial connection
  // setup inputs
  pinMode(button, INPUT);
  pinMode(tiltPin, INPUT);
  pinMode(pot, INPUT);
  // setup interrupt 
  attachInterrupt(digitalPinToInterrupt(2), buttonInput, RISING); // specify interrupt routine for the button
  // setup lcd
  analogWrite(contrastPin,contrast);
  lcd.begin(16, 2);
}

void loop() {
  // if a die is not currently being rolled
  if (isRolling == 0) {
    // print on the screen
    lcd.setCursor(0,0);
    lcd.print(" DIE SIMULATOR  ");
  
    getDieValue();
    // detect if device is shaken
    // if tiltState is different than prevTiltState and is currently LOW, roll the die
    tiltState = digitalRead(tiltPin);
    if (tiltState != prevTiltState){
      if (tiltState == LOW) {
        // the device enters a rolling state
        isRolling = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Rolling...");
        delay(2500);
        lcd.clear();
        lcd.setCursor(8, 1);
        // print a number between 0 and dieLimit (exclusive)
        lcd.print(random(1, dieLimit));
      }
    }
    prevTiltState = tiltState;  
  }
}

// proceed to the next roll after players have acknowledged the result of the previous roll
void buttonInput() {
  //setup a 200ms "no interrupts" window to prevent a glitch where one button press is registered twice
  if(millis() - previousInterrupt > 200){
    isRolling = 0;
    lcd.clear();
  }
  previousInterrupt = millis();
}
