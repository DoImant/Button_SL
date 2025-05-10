#include <Button_SL.hpp>
#include "Button_SL.hpp"

//////////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary class for time-controlled functions.
///
//////////////////////////////////////////////////////////////////////////////
class Timer {
public:
  void start() { timeStamp = millis(); }
  bool operator()(const unsigned long duration) const { return millis() - timeStamp >= duration; }

private:
  unsigned long timeStamp {0};
};

//////////////////////////////////////////////////
// Global constants and variables
//////////////////////////////////////////////////
constexpr uint8_t btn_pin1 {2};
constexpr uint8_t btn_pin2 {3};
constexpr uint8_t btn_pin3 {4};
constexpr uint8_t led_short {6};
constexpr uint8_t led_long {7};
constexpr unsigned long duration {500};
constexpr unsigned long blink_duration {200};

using namespace Btn;

Button btn{btn_pin1};
ButtonSL bArray[]{
    {btn_pin2}, 
    {btn_pin3, 1500, HIGH}
};
Timer wait;

//////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the program.
///
//////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  pinMode(led_short, OUTPUT);
  pinMode(led_long, OUTPUT);

  // Initialize Buttons
  for (auto &buttons : bArray) {
    buttons.begin();
  }
  btn.begin();
  btn.setDebounceTime_ms(20);
  bArray[1].releaseOn();
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Check Buttons.
///
//////////////////////////////////////////////////////////////////////////////
void loop() {

  if (btn.tick()) { Serial.print("."); }

  switch (bArray[0].tick()) {
    case ButtonState::shortPressed:
      Serial.print("B1 (short) ");
      Serial.print(bArray[0].getDuration_ms());
      Serial.println(" ms");
      break;
    case ButtonState::longPressed:
      Serial.print("B1 (long) ");
      Serial.print(bArray[0].getDuration_ms());
      Serial.println(" ms");
      break;
    default: break;
  }

  switch (bArray[1].tick()) {
    case ButtonState::shortPressed:
      wait.start();
      digitalWrite(led_short, HIGH);
      Serial.print("B2 (short) ");
      Serial.print(bArray[1].getDuration_ms());
      Serial.println(" ms");
      break;
    case ButtonState::longPressed: 
      Serial.print("B2 (long) ");
      Serial.print(bArray[1].getDuration_ms());
      Serial.println(" ms");
      Serial.println("Auto released");
      break;
    case ButtonState::pressed:
      if (wait(blink_duration)) {
        digitalWrite(led_long, !digitalRead(led_long));
        wait.start();
      }
      break;
    default:
      if (digitalRead(led_short) && wait(duration)) { digitalWrite(led_short, LOW); }
      if (digitalRead(led_long)) { digitalWrite(led_long, LOW); }
      break;
  }
}