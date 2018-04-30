#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_Si4713.h"

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int decreaseButton = 6;
const int increaseButton = 5;

int increaseButtonState; 
int decreaseButtonState;  

int lastIncreaseButtonState = LOW;
int lastDecreaseButtonState = LOW;

long FRECUENCIA = 9310;
#define RESETPIN 4

unsigned long lastDebounceTimeDecrease = 0;
unsigned long lastDebounceTimeIncrease = 0;

unsigned long debounceDelay = 50;

Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);

void setup() {
  pinMode(decreaseButton, INPUT);
  pinMode(increaseButton, INPUT);
  lcd.begin(16, 2);
  
  if (!radio.begin()) { // begin with address 0x63 (CS high default)
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No radio");
    delay(500);
    while (1);
  }
  
  radio.setTXpower(115);  // dBuV, 88-115 max
  
  printFrequency();

  //radio.beginRDS();


}

void loop() {
  int decreaseReading = digitalRead(decreaseButton);
  int increaseReading = digitalRead(increaseButton);
  
  if (decreaseReading != lastDecreaseButtonState) {
    lastDecreaseButtonState = millis();
  }

  if (increaseReading != lastIncreaseButtonState) {
    lastIncreaseButtonState = millis();
  }

  if ((millis() - lastDebounceTimeDecrease) > debounceDelay) {
    if (decreaseReading != decreaseButtonState) {
      decreaseButtonState = decreaseReading;
      if (decreaseButtonState == HIGH) {
        if (FRECUENCIA > 8750) {
          FRECUENCIA = FRECUENCIA - 20;
          printFrequency();
        }
      }
    }
  }

  if ((millis() - lastDebounceTimeIncrease) > debounceDelay) {
    if (increaseReading != increaseButtonState) {
      increaseButtonState = increaseReading;
      if (increaseButtonState == HIGH) {
        if (FRECUENCIA < 10800) {
          FRECUENCIA = FRECUENCIA + 20;
          printFrequency();
        }
      }
    }
  }

  lastDecreaseButtonState = decreaseReading;
  lastIncreaseButtonState = increaseButton;
}

void printFrequency() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">_   RADIO FM");
  lcd.setCursor(0, 1);
  lcd.print("FREQ: ");
  lcd.print(FRECUENCIA / 100);
  lcd.print('.'); 
  lcd.print(FRECUENCIA % 100);
  lcd.print(" MHz");
  radio.tuneFM(FRECUENCIA);
}

