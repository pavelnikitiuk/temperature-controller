#include "RotaryManager.h"

RotaryManager::RotaryManager(uint8_t pinA, uint8_t pinB, uint8_t buttonPin)
  : encoder(pinA, pinB, buttonPin) {}

void RotaryManager::begin() {
  encoder.setType(TYPE2);
}

void RotaryManager::onEvent(RotaryEvent event, std::function<void()> callback) {
  callbacks[event] = callback;
}

void RotaryManager::update() {
  encoder.tick();

  // if (encoder.isTurn() && callbacks.count(ROTATE)) callbacks[ROTATE]();
  if (encoder.isRight() && callbacks.count(RIGHT)) callbacks[RIGHT]();
  if (encoder.isLeft() && callbacks.count(LEFT)) callbacks[LEFT]();
  if (encoder.isRightH() && callbacks.count(RIGHT_HOLD)) callbacks[RIGHT_HOLD]();
  // if (encoder.isLeftH() && callbacks.count(LEFT_HOLD)) callbacks[LEFT_HOLD]();
  // if (encoder.isFastR() && callbacks.count(FAST_RIGHT)) callbacks[FAST_RIGHT]();
  // if (encoder.isFastL() && callbacks.count(FAST_LEFT)) callbacks[FAST_LEFT]();

  if (encoder.isPress() && callbacks.count(PRESS)) callbacks[PRESS]();
  // if (encoder.isRelease() && callbacks.count(RELEASE)) callbacks[RELEASE]();
  // if (encoder.isReleaseHold() && callbacks.count(RELEASE_HOLD)) callbacks[RELEASE_HOLD]();
  // if (encoder.isClick() && callbacks.count(CLICK)) callbacks[CLICK]();
  // if (encoder.isHolded() && callbacks.count(HOLDED)) callbacks[HOLDED]();
  // if (encoder.isHold() && callbacks.count(HOLD)) callbacks[HOLD]();
  // if (encoder.isSingle() && callbacks.count(SINGLE)) callbacks[SINGLE]();
  // if (encoder.isDouble() && callbacks.count(DOUBLE)) callbacks[DOUBLE]();
}
