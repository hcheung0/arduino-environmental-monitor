Arduino Environmental Monitor

An Arduino Uno reads temperature and humidity from a DHT11 sensor, shows them live on a 16x2 LCD, and logs them over serial. A push button toggles between °C and °F.

Hardware
- Elegoo Uno R3 (Arduino Uno compatible)
- DHT11 temperature/humidity sensor
- LCD1602 display (4-bit mode)
- Push button
- Breadboard
- Jumper wires
- Potentiometer
- Resistors

How it works
- Sensing: the DHT11 is read once per second using the Adafruit DHT library. Temperature is requested in the currently selected unit.
- Display: `updateDisplay()` clears the LCD and shows temperature on row 1 and humidity on row 2. If the sensor returns an invalid reading (NaN), it shows an error message instead of garbage values.
- Unit toggle: `handleButton()` flips between °C and °F on each press. The button uses the internal pull-up resistor (pressed = LOW) and is debounced in software with a 50 ms stability window.
- Non-blocking timing: both the debounce and the 1-second sensor refresh use `millis()` instead of `delay()`, so button presses are checked continuously while the display updates on schedule.

Build and upload
- Built with PlatformIO (Arduino framework) in VS Code.
- Dependencies: Dependencies (installed automatically by PlatformIO via `platformio.ini`): `LiquidCrystal` and Adafruit's `DHT sensor library`, which also pulls in `Adafruit Unified Sensor`.
```
pio run --target upload
pio device monitor -b 9600
```

Project structure
```
src/main.cpp   # all firmware: setup, button handling, display updates
platformio.ini
```
