void PowerFailA(){
if (SafeStat) {
    bot.sendMessageWithReplyKeyboard(Gruppe_id, "WARNING!!!! \nPower Fail Dedectet, Confirm Alert!", "", "[[\"/confirmAlert\"],[\"/leave_key\", \"/take_key\"],[\"/status\", \"/safelock\"],[\"/open_door\", \"/close_door\"]]", true);
    Neo_R = 55;
    PowerFailAl = 1;
    #ifdef OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.setTextColor(BLACK, WHITE);
    display.println("!!!!!!!WARNING!!!!!!!");
    display.setTextColor(WHITE, BLACK);
    display.println("Power Fail Dedectet");
    display.println("Mesage sended");
    display.setTextColor(BLACK, WHITE);
    display.println("!!!!!!!WARNING!!!!!!!");
    display.setTextColor(WHITE, BLACK);
    display.display();
    #endif
  }
}
