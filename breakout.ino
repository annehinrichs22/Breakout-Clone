// OLED Breakout using Adafruit GFX Library

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define BUTUP A3
#define BUTDOWN A0
#define BUTRIGHT A2
#define BUTLEFT A1

int quit = 0;
int score = 0;
int highscore = 0;

int balkbreedte = 10;
int balkx = (64-(balkbreedte / 2));
int balky = 62;
int balkx1 = (64+(balkbreedte / 2));
int balkspeed = 2;

int ballx = 64;
int bally = 57;
int balldirectionx = 0;
int balldirectiony = -1;
int directionpath = 0;
int ballspeed = 1;
int previouspath = 0;

void setup()   {
  Serial.begin(9600);

  pinMode(BUTUP, INPUT);
  pinMode(BUTDOWN, INPUT);
  pinMode(BUTRIGHT, INPUT);
  pinMode(BUTLEFT, INPUT);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Build by A. Hinrichs (C)2017");
  display.display();
  delay(2000);
}



void loop(){
  
  // redefinition section -------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  quit = 0;

  balkbreedte = 10;
  balkx = (64-(balkbreedte / 2));
  balky = 62;
  balkx1 = (64+(balkbreedte / 2));
  balkspeed = 2;

  ballx = 64;
  bally = 57;
  balldirectionx = 0;
  balldirectiony = -1;
  directionpath = 0;
  ballspeed = 1;
  previouspath = 0;
  

  // don't reset the score and highscore

  // intro ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(64 - (22 / 2), 32 - (8 / 2) - 10);
  display.println("BRICK");
  display.setCursor(64 - (15 / 2), 32 - (8 / 2));
  display.println("WALL");
  display.display();
  delay(500);
  display.clearDisplay();

  while (digitalRead(BUTUP) == ! HIGH) {
    Serial.println("hallo");
    // title
    display.setCursor(64 - (22 / 2), 32 - (8 / 2) - 10);
    display.println("BRICK");
    display.setCursor(64 - (15 / 2), 32 - (8 / 2));
    display.println("WALL");
    display.setCursor(0, 0);
    // High score
    if (score > highscore) {
      highscore = score;
    }
    display.println("High score:");
    display.setCursor(20, 8);
    display.print(highscore);
    
    display.display();
    // turns the text on and off
    delay(200);
    display.setCursor(64 - (128 / 2), 32 - (8 / 2) + 10);
    display.println("Press UP long to continue");
    display.display();
    delay(200);
    display.clearDisplay();
  }

  // redefine score
  score = 0;

  // the game ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
  while (quit == 0) {

    // check inputs
    if (digitalRead(BUTLEFT) == HIGH) {
      balkx = balkx - balkspeed;
      balkx1 = balkx1 - balkspeed;
    }
    if (digitalRead(BUTRIGHT) == HIGH) {
      balkx = balkx + balkspeed;
      balkx1 = balkx1 + balkspeed;
    }

    // balk checken
    if (balkx < 0) {
      balkx = 0;
      balkx1 = 0 + balkbreedte;
    }
    if (balkx1 > 128) {
      balkx1 = 128;
      balkx = 128 - balkbreedte;
    }

    previouspath = directionpath;
    
    // ball en balk checken
    if (bally == (balky -1)){
      if (ballx > (balkx +3) && ballx < (balkx1 -3)){
        directionpath = 0;
      }
      if (ballx > (balkx1 -3) && ballx < balkx1){
        directionpath = 1;
      }
      if (ballx > balkx && ballx < (balkx +3)){
        directionpath = 2;
      }
    }

    // ball maken
    
      //ball checken
      // path 0
      if (directionpath == 0){
        if (previouspath != directionpath){
        balldirectiony = -ballspeed;
        }
        else{
          balldirectiony = -ballspeed;
        }
      if (bally < 0) {
        balldirectiony = ballspeed;
      }
      }

      // path 1
      if (directionpath == 1){
        if (previouspath != directionpath){
        balldirectionx = ballspeed;
        balldirectiony = -ballspeed;
        }
      if (ballx < 0) {
        balldirectionx = ballspeed;
        balldirectiony = ballspeed;
      }
      if (ballx > 128) {
        balldirectionx = -ballspeed;
        balldirectiony = -ballspeed;
      }
      if (bally < 0) {
        balldirectionx = -ballspeed;
        balldirectiony = ballspeed;
      }
      }

      // path 2
      if (directionpath == 2){
        if (previouspath != directionpath){
        balldirectionx = -ballspeed;
        balldirectiony = -ballspeed;
        }
      if (ballx < 0) {
        balldirectionx = ballspeed;
        balldirectiony = -ballspeed;
      }
      if (ballx > 128) {
        balldirectionx = -ballspeed;
        balldirectiony = ballspeed;
      }
      if (bally < 0) {
        balldirectionx = ballspeed;
        balldirectiony = ballspeed;
      }
      }
      
    // ball check
    if (bally > 64) {
        quit = 1;
      }

    // ball aanpassen
    ballx = ballx + balldirectionx;
    bally = bally + balldirectiony;

    // tekenen
    display.drawLine(balkx, balky, balkx1, balky, WHITE);
    display.drawPixel(ballx, bally, WHITE);
    display.display();
    delay(10);
    display.clearDisplay();
  }
}
