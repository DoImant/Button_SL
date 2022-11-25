//////////////////////////////////////////////////////////////////////////////
/// @file Button_SL.hpp
///
/// Button is a class for debouncing button queries. The debounce time can be specified.
/// The object returns a true if a button press was detected, otherwise a false is returned.
/// The debounce time can also be used to specify a minimum press length (e.g. a button must
/// be pressed for at least 0.5 seconds).
///
/// ButtonSL is extended so that a distinction can be made between a short and long button press.
/// A time (in ms) can be specified after which a button press is considered "long".
/// Correspondingly, the tick() method returns the status NONE, LONG or SHORT.
///
/// Library: https://github.com/DoImant/Button_SL.git
///
/// Copyright (c) 2022
/// MIT license, check license.md for more information
/// All text above must be included in any redistribution
//////////////////////////////////////////////////////////////////////////////

#ifndef _BUTTON_SL_HPP_
#define _BUTTON_SL_HPP_
#include <Arduino.h>

namespace Btn {
//////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////
constexpr uint8_t DEBOUNCE_TIME{30};   // The value can be reduced for buttons that bounce little. Time in ms.

//////////////////////////////////////////////////
// Variables and Classdefinitions
//////////////////////////////////////////////////
enum class ButtonState : uint8_t { notPressed, shortPressed, longPressed };

//////////////////////////////////////////////////////////////////////////////
/// @brief Button object
///
/// @param pinNr // Pin where the Button is connected
/// @param as    // LOW if the button is connected with a pull up, otherwise HIGH with a pull down resistor
//////////////////////////////////////////////////////////////////////////////
class Button {
public:
  Button(uint8_t pinNr = 0, bool as = LOW) : pin{pinNr}, activeState{as} {}
  Button(const Button &) = delete;              // No copy constructor
  Button &operator=(const Button &) = delete;   // No assignment
  Button(Button &&) = default;                  // Move it
  void begin();
  bool tick();
  void setDebounceTime_ms(uint16_t);

protected:
  uint8_t pin;           // Button PIN number
  uint8_t activeState;   // Saves whether the buttons active state is HIGH or LOW.
  uint8_t compareState{LOW};
  uint16_t dbTime_ms{DEBOUNCE_TIME};
  uint_least32_t timeStamp;
};

//////////////////////////////////////////////////////////////////////////////
/// @brief Subclass of Button
///         The subclass extends a button object with the ability to
///         distinguish between short and long button presses
//////////////////////////////////////////////////////////////////////////////
class ButtonSL : public Button {
public:
  ButtonSL(uint8_t pinNr = 0, uint16_t t_ms = 1000, bool as = LOW) : Button{pinNr, as} { time_ms = t_ms; }
  ButtonState tick();
  void setTimeThreshold_ms(uint16_t);
  void releaseOn() { release = true; }
  void releaseOff() { release = false; }
  uint32_t getDuration_ms() const;

private:
  uint16_t time_ms;   // Saves the time (in ms) from which a key press is recognized as long.
  uint8_t state{!activeState};
  uint16_t pressingTime;   // Saves the length of time that the button was pressed (ms).
  bool release{false};
  bool hasReleased{false};
};
}   // namespace Btn
#endif
