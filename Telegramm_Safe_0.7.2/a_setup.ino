
void setup() {
  Serial.begin(115200);
  Serial.println("");
  pinMode(DoorSwitchPin, INPUT_PULLUP);
  pinMode(PowerFailPin, INPUT);
  I2C_PotSetUp();
  pixels.begin(); // This initializes the NeoPixel library.
  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setPixelColor(0, pixels.Color(0,150,0));
  pixels.show(); // This sends the updated pixel color to the hardware.
  #ifdef OLED
  OLED_Display_Setup();
  #endif
  WiFI_Setup();
  #ifdef OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(OLED_Welcome);
  display.println("WIFI connected");
  display.display();
  #endif
  Neo_G = 150;
  /********** I2C I/O **********/
  I2C_SetUp();
  /********** I2C I/O **********/
}
