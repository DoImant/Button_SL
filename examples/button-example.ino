#include "Button_SL.hpp"

//////////////////////////////////////////////////
// Global constants and variables
//////////////////////////////////////////////////
constexpr uint8_t BUTTON_PIN1{2};
constexpr uint8_t BUTTON_PIN2{3};
constexpr uint8_t BUTTON_PIN3{4};

using namespace Btn;

Button btn{BUTTON_PIN1};
ButtonSL bArray[]{
    {BUTTON_PIN2},
    {BUTTON_PIN3, 1500, HIGH}
};
//////////////////////////////////////////////////
// Function forward declaration
//////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the program.
///
//////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println("Start");
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
      Serial.print("B1 . ");
      Serial.print(bArray[0].getDuration_ms());
      Serial.println(" ms");
      break;
    case ButtonState::longPressed:
      Serial.print("B1 + ");
      Serial.print(bArray[0].getDuration_ms());
      Serial.println(" ms");
      break;
    default: break;
  }

  switch (bArray[1].tick()) {
    case ButtonState::shortPressed:
      Serial.print("B2 . ");
      Serial.print(bArray[1].getDuration_ms());
      Serial.println(" ms");
      break;
    case ButtonState::longPressed:
      Serial.print("B2 + ");
      Serial.print(bArray[1].getDuration_ms());
      Serial.println(" ms");
      Serial.println("Auto released");
      break;
    default: break;
  }
}