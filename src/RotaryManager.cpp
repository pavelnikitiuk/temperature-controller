#include "RotaryManager.h"

RotaryManager::RotaryManager(uint8_t pinA, uint8_t pinB, uint8_t buttonPin)
    : encoder(pinA, pinB, buttonPin, INPUT_PULLUP) {

    }

void RotaryManager::begin() {
  encoder.setBtnLevel(LOW);
  encoder.setClickTimeout(500);
  encoder.setDebTimeout(50);
  encoder.setHoldTimeout(600);
  encoder.setStepTimeout(200);

  encoder.setEncReverse(0);
  encoder.setEncType(EB_STEP);
  encoder.setFastTimeout(30);
  encoder.counter = 0;
}

void RotaryManager::onEvent(RotaryEvent event, std::function<void()> callback) {
  callbacks[event] = callback;
}

void RotaryManager::update() {
  encoder.tick();

  if (encoder.right() && callbacks.count(RIGHT))
    callbacks[RIGHT]();
  if (encoder.left() && callbacks.count(LEFT))
    callbacks[LEFT]();
  if (encoder.click() && callbacks.count(PRESS))
    callbacks[PRESS]();
  
}
