
void loop() {
  SafeDoor = digitalRead(DoorSwitchPin);
  PowerFail = digitalRead(PowerFailPin);
    if (SafeDoor) {
    Neo_B = 255;
  } else {
    Neo_B = 0;
  }
  if (PowerFail) {
    Neo_G = 55;
  } else {
    Neo_G = 0;
  }
  if (!PowerFail){
    if (SafeStat){
      if (!PowerFailAl)
      PowerFailA();
    }
  }
  RemoteOn = mcp.digitalRead(RemOn);
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFI_Setup();
  }
  if (SafeStat) {
    if (SafeDoor != DoorClose) {
      if (Alert == 0) {
        Neo_R = 255;
        Alert = 1;
        #ifdef OLED
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.setTextColor(BLACK, WHITE);
        display.println("!!!!!!!WARNING!!!!!!!");
        display.setTextColor(WHITE, BLACK);
        display.println("Door opend While Safe");
        display.println("locked, sending Mesage");
        display.setTextColor(BLACK, WHITE);
        display.println("!!!!!!!WARNING!!!!!!!");
        display.setTextColor(WHITE, BLACK);
        #endif
        bot.sendMessageWithReplyKeyboard(Gruppe_id, "WARNING!!!! \n Safe Opend without unlocking, Confirm Alert!", "", "[[\"/confirmAlert\"],[\"/leave_key\", \"/take_key\"],[\"/status\", \"/safelock\"],[\"/open_door\", \"/close_door\"]]", true);
        #ifdef OLED
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.setTextColor(BLACK, WHITE);
        display.println("!!!!!!!WARNING!!!!!!!");
        display.setTextColor(WHITE, BLACK);
        display.println("Door opend While Safe");
        display.println("locked Mesage sended");
        display.setTextColor(BLACK, WHITE);
        display.println("!!!!!!!WARNING!!!!!!!");
        display.setTextColor(WHITE, BLACK);
        display.display();
        #endif
      }
    }
  }

  pixels.setPixelColor(0, pixels.Color(Neo_R, Neo_G, Neo_B));
  pixels.show();
}
