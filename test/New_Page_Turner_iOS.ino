#include <BleKeyboard.h>

#define RIGHT_TURN 4
#define LEFT_TURN 5

BleKeyboard bleKeyboard;

void setup() {
  bleKeyboard.begin();
  
    
  pinMode(LEFT_TURN, INPUT_PULLUP);
  pinMode(RIGHT_TURN, INPUT_PULLUP);
}

void loop() {
  if (bleKeyboard.isConnected() && !digitalRead(RIGHT_TURN)) {
    bleKeyboard.press(KEY_RIGHT_ARROW);
    delay (100);
    bleKeyboard.releaseAll();
  }
  if (bleKeyboard.isConnected() && !digitalRead(LEFT_TURN)) {
    bleKeyboard.press(KEY_LEFT_ARROW);
    delay (100);
    bleKeyboard.releaseAll();
  }

}
