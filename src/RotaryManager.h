#ifndef ROTARY_MANAGER_H
#define ROTARY_MANAGER_H

#include <EncButton.h>
#include <map>

enum RotaryEvent {
  RIGHT,
  LEFT,
  PRESS,
  CLICK,
  HOLD,
};

class RotaryManager {
public:
  RotaryManager(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);
  void begin();
  void handle();

  void onEvent(RotaryEvent event, std::function<void()> callback);

private:
  EncButton encoder;
  std::map<RotaryEvent, std::function<void()>> callbacks;
};

#endif
