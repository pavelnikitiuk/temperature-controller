#ifndef ROTARY_MANAGER_H
#define ROTARY_MANAGER_H

#include <EncButton.h>
#include <map>

enum RotaryEvent {
  ROTATE,
  RIGHT,
  LEFT,
  RIGHT_HOLD,
  LEFT_HOLD,
  FAST_RIGHT,
  FAST_LEFT,

  PRESS,
  RELEASE,
  RELEASE_HOLD,
  CLICK,
  HOLDED,
  HOLD,
  SINGLE,
  DOUBLE
};

class RotaryManager {
public:
  RotaryManager(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);
  void begin();
  void update();

  void onEvent(RotaryEvent event, std::function<void()> callback);

private:
  EncButton encoder;
  std::map<RotaryEvent, std::function<void()>> callbacks;
};

#endif
