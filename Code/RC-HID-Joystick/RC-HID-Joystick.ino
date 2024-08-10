#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Gamepad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long screenPreviousMillis = 0;  // Renamed to avoid conflict
const unsigned long screenUpdateInterval = 100;

// Initializing a Gamepad
Gamepad gp;

bool ledState = LOW;

// Button and input variables
int button = 4;
int redLed = 5;
int greenLed = 6;
int steering = A3;
int throttle = A2;
int sideTrim = A1;

int thVal = 1024 / 2;
int stVal = 1024 / 2;
int trVal = 1024 / 2;
int btVal = 1024 / 2;

unsigned int interval = 50;

void setup() {
  while (!Serial && millis() < 1000) {};
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;)
      ;
  }

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(throttle, INPUT);
  pinMode(steering, INPUT);
  pinMode(sideTrim, INPUT);

  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  updateScreen();
}

void loop() {
  thVal = analogRead(throttle);
  if (thVal > 529) {
    thVal = map(thVal, 529, 919, 0, -127);
    if (thVal < -127) { thVal = -127; }
  } else {
    thVal = map(thVal, 37, 529, 127, 0);
    if (thVal > 127) { thVal = 127; }
  }

  stVal = analogRead(steering);
  if (stVal > 493) {
    stVal = map(stVal, 496, 1023, 0, 127);
    if (stVal > 127) { stVal = 127; }
  } else {
    stVal = map(stVal, 0, 490, -127, 0);
    if (stVal < -127) { stVal = -127; }
  }
  trVal = map(analogRead(sideTrim), 0, 1023, -127, 127);
  btVal = !digitalRead(button);

  gp.setLeftYaxis(thVal);
  gp.setLeftXaxis(stVal);
  gp.setRightYaxis(trVal);
  gp.setRightXaxis(trVal * -1);
  gp.setButtonState(0, btVal);

  fadeLeds(10000);
  updateScreen();

  delay(5);
}

void fadeLeds(unsigned long totalTime) {
  static unsigned long fadePreviousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - fadePreviousMillis >= interval) {
    fadePreviousMillis = currentMillis;

    // Calculate the current position in the fade cycle
    float cyclePosition = (currentMillis % totalTime) / (float)totalTime;

    // Calculate the PWM values
    int redValue, greenValue;
    if (cyclePosition < 0.5) {
      greenValue = 255 - (int)(cyclePosition * 2 * 255);
      redValue = (int)(cyclePosition * 2 * 204);
    } else {
      redValue = 204 - (int)((cyclePosition - 0.5) * 2 * 204);
      greenValue = (int)((cyclePosition - 0.5) * 2 * 255);
    }

    // Write the inverted PWM values to the pins
    analogWrite(redLed, 255 - redValue);
    analogWrite(greenLed, 255 - greenValue);
  }
}


void drawVerticalLine(int x, int yStart, int yEnd) {
  display.drawLine(x, yStart, x, yEnd, SSD1306_WHITE);
}

void drawHorizontalLine(int y, int xStart, int xEnd) {
  display.drawLine(xStart, y, xEnd, y, SSD1306_WHITE);
}

void drawText(int x, int y, const char* text) {
  display.setCursor(x, y);
  display.setTextSize(1);               // Set text size
  display.setTextColor(SSD1306_WHITE);  // Set text color
  display.print(text);
}

void drawScaledVerticalLine(int yStart, int yEnd, int xMin, int xMax, int percentage) {
  // Constrain percentage to be between -127 and 127
  percentage = constrain(percentage, -127, 127);

  // Map percentage (-127 to 127) to x coordinate (xMin to xMax)
  int x = map(percentage, -127, 127, xMin, xMax);

  // Draw the vertical line at the calculated x coordinate
  display.drawLine(x, yStart, x, yEnd, SSD1306_WHITE);
}

void drawFilledRectangle(int yStart, int yEnd, int xMin, int xMax, int percentage) {
  // Constrain percentage to be between -127 and 127
  percentage = constrain(percentage, -127, 127);

  // Calculate the middle point between xMin and xMax
  int xMiddle = (xMin + xMax) / 2;

  // Map percentage (-127 to 127) to x coordinate (xMin to xMax)
  int x = map(percentage, -127, 127, xMin, xMax);

  if (x == xMiddle) {
    // If the calculated position is the middle, draw a vertical line
    display.drawLine(xMiddle, yStart, xMiddle, yEnd, SSD1306_WHITE);
  } else if (x < xMiddle) {
    // Draw rectangle from x to xMiddle
    display.fillRect(x, yStart, xMiddle - x, yEnd - yStart + 1, SSD1306_WHITE);
  } else {
    // Draw rectangle from xMiddle to x
    display.fillRect(xMiddle, yStart, x - xMiddle, yEnd - yStart + 1, SSD1306_WHITE);
  }
}


void updateScreen() {
  if (millis() - 100 > screenPreviousMillis) {
    screenPreviousMillis = millis();
    display.clearDisplay();
    drawVerticalLine(50, 0, 32);  // Draw vertical line from (64,5) to (64,25)
    drawVerticalLine(127, 0, 32);
    drawHorizontalLine(15, 0, 128);  // Draw horizontal line from (10,15) to (120,15)
    drawHorizontalLine(0, 50, 128);
    drawHorizontalLine(31, 50, 128);
    drawText(0, 3, "Throttle");   // Draw text at (4,3)
    drawText(0, 21, "Steering");  // Draw text at (4,21)

    // drawScaledVerticalLine(3, 12, 52, 125, 0);
    // drawScaledVerticalLine(18, 28, 52, 125, 0);
    drawFilledRectangle(3, 12, 52, 126, thVal);
    drawFilledRectangle(18, 28, 52, 126, stVal);
    display.display();
  }
}