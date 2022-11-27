# Button_SL Library

The Button_SL library consists of two classes (Button and ButtonSL) for querying buttons.

Button is a class for debouncing button queries. The debounce time can be specified.
The object returns a true if a button press was detected, otherwise a false is returned.
The debounce time can also be used to specify a minimum press length (e.g. a button must
be pressed for at least 0.5 seconds).

ButtonSL is extended so that a distinction can be made between a short and long button press.
A time (in ms) can be specified after which a button press is considered "long".
Correspondingly, the tick() method returns the status notPressed, shortPressed or longPressed.

## Examples

A button object for PIN2. The button is connected via VCC, with a pull-down resistor on the Arduino pin.
Therefore the active state is HIGH:
`Button btn{2,HIGH};`

Initialize the object and set the debounce time to 20ms (default is 30ms):

```C++
#include <Button_SL.hpp>

using namespace Btn;
Button btn{2,HIGH};

// in void setup()
void setup() {
  btn.begin();
  btn.setDebounceTime_ms(20);
}

// in void loop()
void loop() {
  if (btn.tick() == true){
    Serial.println("Button was pressed");
  }
}
```

Create a button object for PIN2 that determines the length of a button press. The button is connected with a
pulldown resistor. Therefore the active state is HIGH:  
`ButtonSL btnSL{2,1500,HIGH};`

*btnSL* will return Buttonstate::shortPressed for a press duration < 1500ms.
For >= 1500ms Buttonstate::longPressed will be returned. The time is stored in an **unsigned int** attribute
and must not exceed 65535 ms.  

```C++
#include <Button_SL.hpp>

using namespace Btn;
ButtonSL btnSL{2,1500,HIGH};

// in void setup()
void setup() {
  btnSL.begin();
}

// in void loop()
void loop() {
  switch (btnSL.tick()) {
    case ButtonState::shortPressed: 
      Serial.print(". "); Serial.print(btnb.getDuration_ms()); Serial.println(" ms"); 
      break;
    case ButtonState::longPressed: 
      Serial.print("- "); Serial.print(btnb.getDuration_ms()); Serial.println(" ms"); 
      break;
    default: 
      break;  
  }
}
```

If the button is connected to GND and the internal pullup resistor of the microcontroller is to be used,
the button objects can be created as follows:

```C++
Button btn{2};              // Use pin 2, INPUT_PULLUP
ButtonSL btnSL{3,1500};     // Use pin 3, 1500ms for long, INPUT_PULLUP
ButtonSL btnSL2{4};         // Use pin 4, 1000ms for long, INPUT_PULLUP
```

If the active status of the connected buttons is HIGH, the objects can be created in the following way:

```C++
Button btn{2,HIGH}    
// No Pullup, the default value does not apply here! All three parameters must be specified.
ButtonSL btn3{5,1000,HIGH}; // Use pin 5, 1000ms for long, INPUT
```

INPUT_PULLUP is the default value with which the button objects are initialized.
Therefore, no specification must be made here. **BUT**, if ButtonSL objects with active state HIGH are used,
all three parameters must be specified when creating the object.

You can test this library with the online simulator wokwi:: [Button Example](https://wokwi.com/projects/346522345074066004)

## Default values

* Active status for a pushbutton press: INPUT_PULLUP
* Time limit for the distinction between short and long button presses: 1000ms (max. 65535 ms).
* Debounce time: 30ms (max. 65535 ms).
* For ButtonSL releaseOff()

## Methods

The methods of the objects are named so that their function should be self-explanatory:

Button Methods:

| Method                      | Class     | Action                                                                 |
|-----------------------------|-----------|------------------------------------------------------------------------|
|begin()                      | Button/SL |Initialize Pins                                                         |
|tick()                       | Button/SL |Query the pin status                                                    |
|setDebounceTime_ms(uint16_t) | Button/SL |Set Debouncetime in ms                                                  |
|releaseOn()                  | ButtonSL  |Automatic release of the push-button after the time period for a long push-button action has elapsed.       |
|releaseOff() (default)       | ButtonSL  |No automatic release of the push-button                                 |
|setTimeThreshold_ms(uint16_t)| ButtonSL  |Set the time threshold from which a button press is recognized as long. |
|getDuration_ms()             | ButtonSL  |Delivers the time in ms that the pushbutton was pressed                 |
