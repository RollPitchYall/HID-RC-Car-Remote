# Traxxas RC Remote as USB Game Controller
## _Using an Arduino Micro_

This project uses the guts of a Traxxas remote to create
an RC game controller by replacing its microcontroller.

- Cheaper than a off the shelf gaming steering wheel
- Comfortable ergonomics
- Other controllers can be converted too
- easier than 3d printng a remote

![picRemote](https://github.com/RollPitchYall/HID-RC-Car-Remote/blob/main/Photos/Remote.jpg)

## Features

- USB HID Device - game controller
- Steering and throttle gimbals (potentiometers)
- 1 Aux dial (potentiometer) and 1 button
- 128x32 OLED screen (optional)
- red/green led fading effects

![picRemote](https://github.com/RollPitchYall/HID-RC-Car-Remote/blob/main/Photos/OLED%20Indicator.jpg)

## Libraries
- Gamepad https://github.com/gamelaster/ArduinoGamepad
- Adafruit_SSD1306.h
- Adafruit_GFX.h
- Wire.h


## Wiring

| Component | Pin |
| ------ | ------ |
| OLED SDA | 2 |
| OLED SCL | 3 |
| Button | 4 |
| Red Led | 5* |
| Green Led | 6* |
| Aux Potentiometer | A1 |
| Throttle Potentiometer | A2 |
| Steering Potentiometer | A3 |

**add a 220 or 330 ohm resistor as well 

![Internals](https://github.com/RollPitchYall/HID-RC-Car-Remote/blob/main/Photos/Internals.jpg)
![InternalWires](https://github.com/RollPitchYall/HID-RC-Car-Remote/blob/main/Photos/Internals%20Wiring.jpg)
![Arduino Micro Pinout](https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png)


## Future Features
- multi button mode (Aux dial angle changes what the button does)
- simpler calibration of steering center and throttle center
- non linear steering and throttle output
- something more useful with the leds, indicating control mode, or which multi button is selected

## Development

Want to contribute? Great! I'm new to github, you are welcome to make a pull request or whatever its called.

## License

MIT

