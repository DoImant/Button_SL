//////////////////////////////////////////////////////////////////////////////
/// @file Button_SL.cpp
/// @author Kai R.
/// @brief Button is a class for debouncing button queries. The debounce time can be specified.
///        The object returns a true if a button press was detected, otherwise a false is returned.
///        The debounce time can also be used to specify a minimum press length (e.g. a button must
///        be pressed for at least 0.5 seconds).
///
///        ButtonSL is extended so that a distinction can be made between a short and long button press.
///        A time (in ms) can be specified after which a button press is considered "long".
///        Correspondingly, the tick() method returns the status notPressed, shortPressed or longPressed.
///        Library: https://github.com/DoImant/Button_SL.git
///
/// @date 2022-05-04
/// @version 1.0
///
/// @date 2022-10-08
/// @version 1.0.1        change CAPS Enums
///
/// @date 2022-14-11
/// @version 1.1.0        The ButtonSL class can now provide a status after the specified time period
///                       for a long button press, even if the button remains permanently pressed.
///
/// @date 2023-12-06
/// @version 1.1.4        Repeated debounce in the button class removed.
///
/// @date 2024-07-06
/// @version 1.1.5        The autorelease is only executed once as long as the button has not been released. 
///                       Previously, if the button was held down continuously, the autorelease was executed 
///                       every time the time for a long button press expired. 
///
/// @copyright Copyright (c) 2022
/// MIT license, check license.md for more information
/// All text above must be included in any redistribution
//////////////////////////////////////////////////////////////////////////////

#include "Button_SL.hpp"

namespace Btn {
//////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the pin to which the pushbutton is connected
///
/// @param pinNr
//////////////////////////////////////////////////////////////////////////////
void Button::begin() {
  uint8_t mode = (activeState) ? INPUT : INPUT_PULLUP;
  pinMode(pin, mode);
}

//////////////////////////////////////////////////////////////////////////////
/// @brief The Button query. The tick() method should be called in an endless loop.
///
/// @return bool true if the pushbutton was pressed for the time period "dbTime_ms".
//////////////////////////////////////////////////////////////////////////////
bool Button::tick() {
  bool flag = false;
  if (digitalRead(pin) == activeState) {
    switch (compareState) {
      case LOW:
        compareState = HIGH;   // set to HIGH when Button was pressed
        timeStamp = millis();
        [[fallthrough]];
      case HIGH:
        if (millis() - timeStamp > dbTime_ms) {
          // If the button press is equal to the specified debounce time, confirm the button press with true.
          flag = true;
        }
        break;
    }
  } else if (compareState == true) {
    compareState = LOW;   // set to LOW if button is no longer pressed
  }
  return flag;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Set the debounce time
///
/// @param dbTime
//////////////////////////////////////////////////////////////////////////////
void Button::setDebounceTime_ms(uint16_t dbT_ms) { dbTime_ms = dbT_ms; }

//////////////////////////////////////////////////////////////////////////////
/// @brief The ButtonSL query. The tick() method should be called in an endless loop.
///
/// @param time_ms        time (in ms) from which a key press is recognized as long
/// @return ButtonState   The states are "Not pressed", "short pressed" and "long pressed"
//////////////////////////////////////////////////////////////////////////////
ButtonState ButtonSL::tick() {
  uint_least32_t now = millis();
  compareState = state;
  state = digitalRead(pin);
  if (state == activeState && compareState != activeState) {
    timeStamp = now;
    hasReleased = false;
  } else if (state != activeState && compareState == activeState) {
    timeStamp = now - timeStamp;
    if (timeStamp >= dbTime_ms && !hasReleased) {   // released after debounce time?
      return (timeStamp >= time_ms) ? ButtonState::longPressed : ButtonState::shortPressed;
    }
    // hasReleased = false;
  } else if (autoRelease && not hasReleased && (compareState == activeState)) {
    if (now - timeStamp >= time_ms) {
      // state = !compareState;
      hasReleased = true;
      timeStamp = time_ms;
      return ButtonState::longPressed;
    }
  }
  return ButtonState::notPressed;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Set the time (in ms) from which a key press is recognized as long.
///
/// @param th
//////////////////////////////////////////////////////////////////////////////
void ButtonSL::setTimeThreshold_ms(uint16_t th_ms) { time_ms = th_ms; }

//////////////////////////////////////////////////////////////////////////////
/// @brief Returns the amount of time the button was pressed.
///
/// @return uint32_t Time in milliseconds
//////////////////////////////////////////////////////////////////////////////
uint32_t ButtonSL::getDuration_ms(void) const { return timeStamp; }
}   // namespace Btn