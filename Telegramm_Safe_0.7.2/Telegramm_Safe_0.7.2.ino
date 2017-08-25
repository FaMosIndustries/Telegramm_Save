const String Version = "Version: v0.7.2";
/*******************************************************************
    Telegramm Bot  written by Dakota / Jenna /Fabian Morgenthaler
      JJJJ.MM.DD/Version
      2017.03.17/v0.1
      2017.03.18/v0.2
      2017.03.19/v0.3
      2017.03.19/v0.4
      2017.03.23/v0.5
      2017.03.26/v0.5.1
      2017.05.14/v0.6.0 /Dakota alias FaMo
      2017.08.06/v0.7.0 /Dakota alias FaMo
      2017.08.06/v0.7.1 /Dakota alias FaMo
      2017.08.06/v0.7.2 /Dakota alias FaMo


      v0.1
        Created from other bot
      v0.2
        re create the bot because of some trubels
      v0.3
        working version with death lock funktion
      v0.4
        working test script on customer safe
      v0.5
        update the script and change some funktions
        corect spelling
        add Key funktion
      v0.5.1
        add special case if Pet takes the key

      v0.6.0
        add Shock collar /in progress
        add Neopixel to show Status

      v0.7.0
        remove the transmitter
        add Power fail dedectiopne
        add configurabel OLED Display
      v0.7.1
        including the I2C libs for:
          MCP4551-103     I2C Potentiometer
          MCP23008        I2C I/O Extentione
        add reading status of the remote (ON/OFF)
      v0.7.2
        dooing some changes in the developed funktions to get them ready for the released
        


********************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Servo.h>
#include  <Wire.h> //I2C library

#include "Adafruit_MCP23008.h"  //I2C I/O library

#include  <MCP4551.h>  // I2C Potentiometer library

//#define DEBUG 1
//#define TEST 1
//#define OLED 1
/**********Pin SetUp**********/ //
//#define D0                D0  //  free pin
//      SCL (I2C)           D1  //  SCL Pin of the I2C Bus
//      SDA (I2C)           D2  //  SDA Pin of the I2C Bus
#define OLED_RESET          D3  //  maybe this pin can be released
#define ledPin              D4  //  NeoPixel
#define PowerFailPin        D5  //  Power Fail Detection Pin
#define DoorSwitchPin       D6  //  Door Sensor
#define ServoPin            D7  //  Servo
//#define D8                D8  //  free pin
/**********Pin SetUp**********/
/********** I2C I/O **********/
  Adafruit_MCP23008 mcp;
  #define RemPower          1  // Power Button
  #define RemSingel         0  // Singel or continius
  #define RemShock          3  // Shock Button
  #define RemVib            2  // vibrations Button
  #define RemOn             4  // dedect if remote is turned on
/********** I2C I/O **********/

/********** I2C Pot **********/
MCP4551 myMCP4551;
  uint8_t  Raw_Value = 0;
/********** I2C Pot **********/
/*   NEOPIXEL  */
#include <Adafruit_NeoPixel.h>
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPixelPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);
/*   NEOPIXEL  */

// Initialize Wifi connection to the router

//char ssid[] = "S-AP";  // SSID TEKO
//char password[] = "teko2013";
//char ssid[] = "D87770000_[DAKOTA]";  // SSID Dakota
//char password[] = "Jenna_the_Wolf";
char ssid[] = "EasyBox-BC0601";  // SSID Darcus
char password[] = "76EFF61F6!";


//Bot Settings
//#define BOTtoken "311660660:AAHMwoi4kDMrQTagJPWAh0ehTy1eY-HJMq4"
//String BotName ="@jennas_open_bot (DEMO)";
//#define BOTtoken "305796110:AAG1jawFKZjmASayduhdLHRTjjsmwKmpFAg"
//String BotName ="@jenna_collar_bot";
#define BOTtoken "304560779:AAEm_1FhbcAzsrTilqbVuzkzl-WQsXIfsDo"
String BotName = "@development_save_Jenna_bot";


#ifdef OLED
String OLED_Welcome ="welcome from \n" + BotName +"\n"+ Version;

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#endif




Servo myservo;

#define clospos  360 //Servo Value when the servo is in Close Position
#define openpos   0 //Servo Value when the servo is in Open Position
#define DoorClose  LOW //Select if witch state the safe door has when it is close(HIGH or LOW)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

bool Aut            = 0;
bool SafeDoor       = 0; //0=open 1=closed
bool Alert          = 0; //0=no alert 1=alert
bool M_key          = 0; //1=master key taken
bool PowerFail      = 1; //1=Power suply OK  0= Power fail Dedectet
bool PowerFailAl    = 0;
bool SafeStat        = 0;//0=Unlocked 1=Locked
bool RemoteOn        = 0;//Remote 0=OFF 1=ON
byte Neo_R           = 0;
byte Neo_G           = 0;
byte Neo_B           = 0;
byte key_status      = 0;
/*
  key_status
  0   key not taken
  1   key is Taken
  2   admin
  3   user ok
  4   user falsch
*/
String S_vibration_length ="10";
String S_const_shock_length ="1";
String S_shock_strength = "10";
int shock_strength =10;
int const_shock_length =1;
int vibration_length =1;
int const_shock_length_del = 1000;
int vibration_length_del = 1000;

String value = "000";


String Dakota = "154970372";//Dakota//@DakotaWolfi
String Pet = "277246467";//Darcos//@Dr4c0s
String Owner_1 = "127694355";//Simon
//String Gruppe_id  = "-158118965";//Jennas Open Demo Bot
String Gruppe_id  = "-225032458";//delopment gruppe

String Welcome_text = "Welcome from Jenna's and Dakota's Telegramm Bot";



String keyboardJson ;
String mes = "";
String O_mes = "";
String key_holder;
String key_holder_name;

void setup();
void loop();
void handleNewMessages(int numNewMessages);
void WiFI_Setup();
void Access (bool Pet_Access, String chat, String id);
void Safe_write(int safepos);
void keyboard_Json();
#ifdef OLED
void OLED_Display_Setup();
#endif
void SetStrength(int strength);
void I2C_SetUp();
