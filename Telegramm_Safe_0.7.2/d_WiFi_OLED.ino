void WiFI_Setup() {
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  bool WiFi_S = LOW;
  delay(100);
  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  #ifdef OLED
    display.print(".");
    display.display();
  #endif

    delay(500);
    WiFi_S = !WiFi_S;
    if (WiFi_S == LOW){
      digitalWrite(ledPin, LOW);
      pixels.setPixelColor(0, pixels.Color(0,150,150));
      pixels.show();
    }else{
      digitalWrite(ledPin, HIGH);
      pixels.setPixelColor(0, pixels.Color(0,150,0));
      pixels.show();
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  #ifdef OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(OLED_Welcome);
  display.println("WIFI connected");
  display.display();
  #endif
  pixels.setPixelColor(0, pixels.Color(0,150,0));
}
#ifdef OLED
void OLED_Display_Setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  delay (200);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(OLED_Welcome);
  display.display();
}
#endif
