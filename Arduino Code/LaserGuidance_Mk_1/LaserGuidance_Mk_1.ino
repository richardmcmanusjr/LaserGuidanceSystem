/**************************************************************************
File: LaserGuidance_Mk_1
Author: Richard McManus
Date: April, 2023
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include "opr5925.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On our Seeeduino SAMD21: A4(SDA), A5(SCL)
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing micro reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define PHOTO_1 A2 // First photodiode
#define PHOTO_2 A0
#define PHOTO_3 A1
#define PHOTO_4 A3
#define SERVO_YAW 9
#define SERVO_PITCH 8
#define SWITCH 7

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Initialize Display
opr5925 quad_photo(PHOTO_1, PHOTO_2, PHOTO_3, PHOTO_4); 
Servo servo_yaw;
Servo servo_pitch;
void setup() {
  servo_yaw.attach(SERVO_YAW);
  servo_pitch.attach(SERVO_PITCH);
  servo_yaw.write(90);
  servo_pitch.write(90);
  pinMode(SWITCH, INPUT);
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  quad_photo.zero(250);
}

int i = 0;
int j = 22;
float x;
float y;
int photo_1;
int photo_2;
int angle_yaw = 90;
int angle_pitch = 90;
int iterator = 0;

void loop() {
  quad_photo.update();
  display.clearDisplay();
  drawCrossHairs();
  drawHeader();
  x = quad_photo.get_x();
  y = quad_photo.get_y();
  display.drawCircle(x * (display.width() - 1), (y * (47)) + 16, 6, SSD1306_WHITE);
  display.fillCircle(x * (display.width() - 1), (y * (47)) + 16, 2, SSD1306_WHITE);
  display.display();      
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  
  if (iterator < 2*SAMPLES)
    iterator++;
  else{
    iterator = 0;
    if (digitalRead(SWITCH))
      adjust_heading(x,y);
    else {
      angle_yaw = 90;
      angle_pitch = 90;
      servo_yaw.write(angle_yaw);
      servo_pitch.write(angle_pitch);
    }    
  }

 
    
}

void drawCrossHairs() {
  display.drawLine(63, 16, 63, display.height()-1, SSD1306_WHITE);
  display.drawLine(64, 16, 64, display.height()-1, SSD1306_WHITE);
  display.drawLine(0, 39, display.width()-1, 39, SSD1306_WHITE);
  display.drawLine(0, 40, display.width()-1, 40, SSD1306_WHITE);
}

void drawHeader() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Laser Guidance System"));
  display.drawLine(0, 10, display.width()-1, 10, SSD1306_WHITE);
}

void adjust_heading(float x, float y) {
  if (x < 0.35){
    if (angle_yaw > 50){
      angle_yaw--;
      servo_yaw.write(angle_yaw);
    }
  }
  else if (x > 0.65){
    if (angle_yaw < 130){
      angle_yaw++;
      servo_yaw.write(angle_yaw);
    }
  }
    if (y < 0.35){
    if (angle_pitch < 130){
      angle_pitch++;
      servo_pitch.write(angle_pitch);
    }
  }
  else if (y > 0.65){
    if (angle_pitch > 50){
      angle_pitch--;
      servo_pitch.write(angle_pitch);
    }
  }
}
