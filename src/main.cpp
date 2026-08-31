#include <Arduino.h>
#include <LiquidCrystal.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT11 configuration
#define DHTPIN 9
#define DHTTYPE DHT11

// Initialize sensor and LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Track the selected temperature unit
bool fahrenheit = false;
String tempUnit = "C";

// Track when the DHT11 was last read
unsigned long lastSensorReadTime = 0;

// Button state tracking for debouncing
int previousButtonState = HIGH;
int stableButtonState = HIGH;

// Debounce timing
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Button is connected to digital pin 8
#define BUTTONPIN 8


void handleButton() {
    // Read the current button state
    int buttonState = digitalRead(BUTTONPIN);

    // If the button changed, restart the debounce timer
    if (previousButtonState != buttonState) {
        lastDebounceTime = millis();
    }

    // Only accept the change if it has remained stable long enough
    if (millis() - lastDebounceTime >= debounceDelay) {

        // Check if the stable state actually changed
        if (buttonState != stableButtonState) {
            stableButtonState = buttonState;

            // LOW means the button is pressed (INPUT_PULLUP)
            if (stableButtonState == LOW) {
                fahrenheit = !fahrenheit;
            }
        }
    }

    // Update the displayed temperature unit
    if (fahrenheit) {
        tempUnit = "F";
    } else {
        tempUnit = "C";
    }

    // Save the current state for the next loop
    previousButtonState = buttonState;
}


void updateDisplay() {
    // Clear the previous sensor readings
    lcd.clear();

    // Read temperature and humidity from the DHT11
    float temperature = dht.readTemperature(fahrenheit, false);
    float humidity = dht.readHumidity();

    // Check whether either sensor reading failed
    if (isnan(temperature) || isnan(humidity)) {
        lcd.print("DHT11 Error!");
        lcd.setCursor(0, 1);
        lcd.print("Check sensor");
    }

    else {
        // Print readings to the Serial Monitor
        Serial.println("Temperature: " + String(temperature) + " °" + tempUnit);
        Serial.println("Humidity: " + String(humidity) + " %");

        // Display temperature on the first LCD row
        lcd.print("Temp: " + String(temperature) + " " + tempUnit);

        // Move to the second LCD row for humidity
        lcd.setCursor(0, 1);
        lcd.print("Hum: " + String(humidity) + " %");
    }
}


void setup() {
    // Initialize the LCD, DHT11, and Serial Monitor
    lcd.begin(16, 2);
    dht.begin();
    Serial.begin(9600);

    // Use the internal pull-up resistor for the button
    pinMode(BUTTONPIN, INPUT_PULLUP);
}


void loop() {
    // Continuously check for button presses
    handleButton();

    // Update sensor readings once every second without blocking the program
    if (millis() - lastSensorReadTime >= 1000) {
        updateDisplay();

        // Schedule the next sensor update
        lastSensorReadTime += 1000;
    }
}
