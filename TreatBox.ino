// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_mfGFX.h>

// This #include statement was automatically added by the Particle IDE.
#include "Arduino_ST7789.h"

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_mfGFX.h>

// This #include statement was automatically added by the Particle IDE.
#include "Arduino_ST7789.h"

#include <SPI.h>

#define TFT_MOSI       A5
#define TFT_MISO       A4
#define TFT_CLK        A3
#define TFT_CS         A2
#define TFT_RST        A1
#define TFT_DC         A0

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS); // Using Hardware SPI
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_CLK, TFT_CS); // Using software SPI (bitbanging)
Servo myServo;

int photonLED = D7;
int servo = D0;
int distanceEcho = D6;
int distanceTrig = D2;
int button = D5;

int servoStartPos = 150;
int servoEndPos = 0;
int counter = 0;
int inFront = 0;
int buttonState = 0;

long now;
long then;

unsigned long duration;


void setup() {

  Particle.function("feed", feed);
  Particle.variable("inFront", inFront);
  Particle.variable("counter", counter);

  tft.init(135, 240);

  myServo.attach(servo);
  myServo.write(servoStartPos);

  pinMode(photonLED, OUTPUT);
  pinMode(distanceEcho, INPUT);
  pinMode(distanceTrig, OUTPUT);

  pinMode(button, INPUT);

  sampleColors();
  treatOutput();
}

void loop() {

  delay(10);
  buttonState = digitalRead(button);
  digitalWrite(distanceTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceTrig, LOW);

  duration = pulseIn(distanceEcho, HIGH);

  //checks if movment in front of the box
  if (duration > 1500) {
    digitalWrite(photonLED, HIGH);
    inFront = 0;
  } else {
    digitalWrite(photonLED, LOW);
    inFront = 1;
  }

  //controlls button for adding treats
  if (buttonState == HIGH) {
    counter++;
    treatOutput();
  }
}

//treatOutput writes to screen
void treatOutput()
{
  tft.fillScreen(BLACK);
  tft.setFont(CENTURY_8); // TIMESNR_8, CENTURY_8, ARIAL_8, COMICS_8, GLCDFONT
  tft.setRotation(3);
  tft.setCursor(30, 10);
  tft.setTextSize(2);
  tft.print("Treats dispensed ");
  tft.setCursor(100, 44);
  tft.setTextSize(5);
  tft.print(counter);
}

void sampleColors()
{
  tft.fillScreen(WHITE);
  tft.fillScreen(CYAN);
  tft.fillScreen(RED);
  tft.fillScreen(BLUE);
  tft.fillScreen(GREEN);
  tft.fillScreen(YELLOW);
  tft.fillScreen(MAGENTA);
  tft.fillScreen(BLACK);
}

//feed functoion operates servo
int feed(String command)
{
  if (command == "give")
  {
    myServo.write(servoEndPos);
    delay(500);
    myServo.write(servoStartPos);
    counter--;
    treatOutput();
    return 1;
  }
}
