void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_id = bot.messages[i].from_id;
    String from_name = bot.messages[i].from_name;
    #ifdef OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(text);
      display.println(" from " + from_name);
      display.display();
    #endif
    #ifdef DEBUG
      Serial.println(chat_id);
      Serial.println(text);
      Serial.println(from_id);
      Serial.println(from_name);
    #endif
    if (text.startsWith("/setServo")) {
      if (from_id == Dakota) {
        String pos = text.substring(10);
        int intPos = pos.toInt();
        Safe_write(intPos);
        bot.sendMessage(Dakota, "Servo set to " + pos, "");
      }
    }

    if (text.startsWith("/confirmAlert")) {
      Access (1, 0, chat_id, from_id);
      if (Aut == 1){
        if (Alert){
          Alert = 0;
          PowerFailAl = 0;
          keyboard_Json(1);
          bot.sendMessageWithReplyKeyboard(chat_id, "Alert Confirmed", "", keyboardJson, true);
        }
        else{
          if (PowerFailAl){
            Alert = 0;
            PowerFailAl = 0;
            keyboard_Json(1);
            bot.sendMessageWithReplyKeyboard(chat_id, "Alert Confirmed", "", keyboardJson, true);
          }
          else{
            keyboard_Json(1);
            bot.sendMessageWithReplyKeyboard(chat_id, "No Alert is aktive", "", keyboardJson, true);
          }
        }
      }
      else {
        bot.sendMessage(chat_id, "Access Denied", "");
      }
    }

    if (text.startsWith("/safelock")) {
      Access (1, 1, chat_id, from_id);
      if (Aut == 1) {
        if (SafeDoor == DoorClose) {
          SafeStat = 1;
          Safe_write(clospos);
          keyboard_Json(1);
          bot.sendMessageWithReplyKeyboard(chat_id, "Safe is locked now", "", keyboardJson, true);
          O_mes ="Safe is locked now";
        }
        else {
          bot.sendMessage(chat_id, "The safe door is open and must be closed before you can lock it!", "");
          O_mes ="Pleas close the Door";
        }
      }
      else {
        bot.sendMessage(chat_id, "Access Denied!", "");
        O_mes ="Access Denied!";
      }
      #ifdef OLED
      display.println(O_mes);
      display.display();
      #endif
    }
    /* key_status
       0   key not taken
       1   key is Taken
       2   admin
       3   user ok
       4   user falsch
    */

    if (text.startsWith("/safeunlock")) {
      #ifdef DEBUG
        Serial.println("DEBUG_code:safeunlock");
      #endif
      Aut = 0;
      if (M_key == 0) {
        if (key_status != 0) {//key is taken
          #ifdef DEBUG
            Serial.println("DEBUG_code:key is taken");
          #endif
          if (key_holder == from_id) {//user is
            #ifdef DEBUG
              Serial.println("DEBUG_code:user is key holder");
            #endif
            Aut = 1;
            key_status = 3;//user is key holder
          }
          else {
            #ifdef DEBUG
              Serial.println("DEBUG_code:user is not key holder");
              #endif
            key_status = 4;//user is not key holder
            Access (0, 0, chat_id, from_id);//only admin
            if (Aut == 1) {//is user abmin
              #ifdef DEBUG
                Serial.println("DEBUG_code:user is admin");
                #endif
              key_status = 2;//user is admin
            }
          }
        }
        else {//key is not taken
          #ifdef DEBUG
            Serial.println("DEBUG_code:key is not taken");
            #endif
          Access (1, 0, chat_id, from_id);
        }
        switch (key_status) {
          case 0: //key not taken
            mes = from_name + " has unlocked the safe ";
            break;
          case 1: //key is Taken
            mes = "Key is taken from " + key_holder_name;
            break;
          case 2: //admin
            mes = mes = "Key is Taken from " + key_holder_name + " but " + from_name + " had his own key and has unlocked the safe ";
            break;
          case 3: //user ok
            mes = from_name + " has put the key in the lock and opened the safe ";
            break;
          case 4: //user falsch
            mes = "Key is taken from" + key_holder_name + " and you do not have a key ";
            break;
        } //create messages
        if (Aut == 1) {
          #ifdef DEBUG
            Serial.println("DEBUG_code:unlock");
            #endif
          SafeStat = 0;
          key_status = 0;
          Safe_write(openpos);
          keyboard_Json(1);
          bot.sendMessageWithReplyKeyboard(chat_id, mes, "", keyboardJson, true);
        }
        else {
          #ifdef DEBUG
            Serial.println("DEBUG_code:access Denide");
          #endif
          bot.sendMessage(chat_id, "Access Denied!" + mes, "");
        }
      }
      else {
        bot.sendMessage(chat_id, "Access Denied!", "");
      }
    }


    if (text.startsWith("/takekey")) {
      if (SafeStat == 1) {//Safe is locked
        if (key_status <= 0) {// key is not taken
          Access (1, 1, chat_id, from_id);
            if (Aut == 1) {
              if (from_id == Pet) {
                #ifdef DEBUG
                  Serial.println("DEBUG_code:Pet");
                #endif
                mes = "No no no that was not the Deal,\nsince when are pets alowed to take ther own Key?\nI shuld keep the key for me";
                key_holder = "Safe";
                key_holder_name = "Safe";
                key_status = 1;
              }
              else {
                #ifdef DEBUG
                  Serial.println("DEBUG_code:Group Access");
                #endif
                key_holder = from_id;
                key_holder_name = from_name;
                key_status = 1;
                mes = "The Key is now Taken by " + from_name;
              }
            }
            else {
              mes = "you have not access to take the key";
              #ifdef DEBUG
                Serial.println("DEBUG_code:Access Denide");
              #endif
            }
          }
          else {
            if (key_holder == from_id) { //if key_holder want take the key
              mes = "you took the key already";
              #ifdef DEBUG
                Serial.println("DEBUG_code:Key Already taken form requestet person.. may alzheimer ;)");
              #endif
            }
            else {
              Access (0, 0, chat_id, from_id);//only admin
              if (Aut == 1) {
                mes = "the Owner replaced the lock and took the new key";
                key_holder = from_id;
                key_holder_name = from_name;
                key_status = 2;
              }
              else {
                mes = "the key is already taken by " + key_holder_name;
              }
            }
          }
        }
        else {
          mes = "you must first lock the Safe befor you can take the key";
        }
        bot.sendMessage(chat_id, mes, "");
        #ifdef OLED
        display.println(mes);
        display.display();
        #endif
      }

    if (text.startsWith("/leavekey")) {
      #ifdef DEBUG
        Serial.println("DEBUG_code:levekey");
      #endif
      Aut = 0;
        if (key_status != 0) {//key is taken
          #ifdef DEBUG
            Serial.println("DEBUG_code:key is taken");
          #endif
          if (key_holder == from_id) {//user is
            #ifdef DEBUG
              Serial.println("DEBUG_code:user is key holder");
            #endif
            Aut = 1;
            key_status = 3;//user is key holder
          }
          else {
            #ifdef DEBUG
              Serial.println("DEBUG_code:user is not key holder");
            #endif
            key_status = 4;//user is not key holder
            Access (0, 0, chat_id, from_id);//only admin
            if (Aut == 1) {//is user abmin
              #ifdef DEBUG
                Serial.println("DEBUG_code:user is admin");
              #endif
              key_status = 2;//user is admin
            }
          }
        }
        else {//key is not taken
          #ifdef DEBUG
            Serial.println("DEBUG_code:key is not taken");
          #endif
          Access (1, 0, chat_id, from_id);
        }
        switch (key_status) {
          case 0: //key not taken
            mes = "the key is not Taken";
            break;
          case 1: //key is Taken
            mes = "Key is taken from " + key_holder_name;
            break;
          case 2: //admin
            mes = "Key was Taken from " + key_holder_name + " but " + from_name + " has replaced the lock and leave the new key in the lock";
            break;
          case 3: //user ok
            mes = from_name + " has put the key back in the lock and leave it there";
            break;
          case 4: //user falsch
            mes = "Key is taken from" + key_holder_name + " and you do not have a key ";
            break;
        } //create messages
        if (Aut == 1) {
          #ifdef DEBUG
            Serial.println("DEBUG_code:key is back");
          #endif
          key_status = 0;
          keyboard_Json(1);
          bot.sendMessageWithReplyKeyboard(chat_id, mes, "", keyboardJson, true);
        }
        else {
          #ifdef DEBUG
            Serial.println("DEBUG_code:access Denide");
          #endif
          bot.sendMessage(chat_id, "Access Denied!" + mes, "");
        }
      }

    if (text.startsWith("/takemkey")) {
      if (from_id == Dakota) {
        M_key = 1;
        bot.sendMessage(Dakota, "M-Key Taken", "");
      }
    }

    if (text.startsWith("/leavemkey")) {
      if (from_id == Dakota) {
        M_key = 0;
        bot.sendMessage(Dakota, "M-Key Leaved", "");
      }
    }

    if (text.startsWith("/status")) {
      String Status = "The current Status\n";
      if (SafeDoor == DoorClose) {
        Status = Status + "Safe is Closed\n";
      } else {
        Status = Status + "Safe is Open\n";
      }
      if (PowerFail) {
        Status = Status + "Power Fail Dedection OK\n";
      } else {
        Status = Status + "Power Fail Dedection Trigert\n";
      }
      if (RemoteOn){
        Status = Status + "Remote is ON\n";
      } else {
        Status = Status + "Remote is OFF\n";
      }
      if (SafeStat) {
        if (SafeDoor == DoorClose) {
          if (key_status <= 0) {
            Status = Status + "Key is taken from " + key_holder_name + "\n";
          } else {
            Status = Status + "Safe is Locked\n";
          }
        }
        else {
          Status = Status + "Something is gone Wrong\n";
        }

        if (key_status != 0) {//key is taken
          Status = Status + key_holder_name + " has took the key\n";
        }
        else {
          Status = Status + "the key is available\n";
        }
      }
      else {
        Status = Status +  "Safe is Unlocked\n";
      }
      Status = Status + "the current settings are:\n";
      Status = Status + "Shock Strength is: "+ shock_strength +"\n";
      Status = Status + "Continue shock length is: "+ const_shock_length +"\n";
      Status = Status + "Vibration length is: "+ vibration_length +"\n";
      #ifdef DEBUG
        Serial.println("DEBUG_code:mes send" + Status + "DEBUG END");
      #endif
      bot.sendMessage(chat_id, Status, "Markdown");
      #ifdef OLED
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(Status);
        display.display();
      #endif
    }

    if (text.startsWith("/options")) {
      keyboard_Json(1);
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }
    if (text.startsWith("/setting")) {
      keyboard_Json(2);
      String setting = "witch setting you want to change?\n";
      setting = setting + "the current settings are:\n";
      setting = setting + "Shock Strength is: "+ shock_strength +"\n";
      setting = setting + "Continue shock length is: "+ const_shock_length +"\n";
      setting = setting + "Vibration length is: "+ vibration_length +"\n";
      bot.sendMessageWithReplyKeyboard(chat_id, setting, "Markdown", keyboardJson, true);
    }
    if (text.startsWith("/set_strength")) {
      S_shock_strength = text.substring(14);
      shock_strength = S_shock_strength.toInt();
      SetStrength(shock_strength);
      #ifdef DEBUG
        Serial.println("DEBUG_code strength ");

      #endif
      bot.sendMessage(chat_id, "singel shock strength is now "+S_shock_strength, "");
    }

    if (text.startsWith("/set_schock_const_length")) {
      S_const_shock_length = text.substring(25);
      const_shock_length = S_const_shock_length.toInt();
      const_shock_length_del = const_shock_length *  1000;
      bot.sendMessage(chat_id, "Continue shock length is now "+ S_const_shock_length, "");
    }
    if (text.startsWith("/set_vibrations_length")) {
      S_vibration_length = text.substring(23);
      vibration_length = S_vibration_length.toInt();
      vibration_length_del = vibration_length *  1000;
      bot.sendMessage(chat_id, "Vibration length is now "+ S_vibration_length, "");
    }
    if (text.startsWith("/schock_singel")) {
      mcp.digitalWrite(RemSingel, HIGH);
      mcp.digitalWrite(RemShock, LOW);
      delay(1000);
      mcp.digitalWrite(RemShock, HIGH);
      mcp.digitalWrite(RemSingel, HIGH);
      bot.sendMessage(chat_id, "sendet singel"+ S_shock_strength,"");
    }
    if (text.startsWith("/schock_cont")) {
      mcp.digitalWrite(RemSingel, LOW);
      mcp.digitalWrite(RemShock, LOW);
      delay(const_shock_length_del);
      mcp.digitalWrite(RemShock, HIGH);
      mcp.digitalWrite(RemSingel, HIGH);
      bot.sendMessage(chat_id, "sendet "+S_const_shock_length+" sec shock on Level "+ S_shock_strength,"");
    }
    if (text.startsWith("/back")) {
      keyboard_Json(1);
      bot.sendMessageWithReplyKeyboard(chat_id, "nothing changed", "", keyboardJson, true);
    }
    if (text.startsWith("/vibration")) {
      mcp.digitalWrite(RemVib, LOW);
      delay(vibration_length_del);
      mcp.digitalWrite(RemVib, HIGH);
      bot.sendMessage(chat_id, "sendet "+S_vibration_length+" sec vibration","");
    }
    if (text.startsWith("/Power")) {
      mcp.digitalWrite(RemPower, LOW);
      delay(5000);
      mcp.digitalWrite(RemPower, HIGH);
      bot.sendMessage(chat_id, "power comand sendet","");
    }

    #ifdef OLED
    if (text == "/DisplayReset") {
      OLED_Display_Setup();
      bot.sendMessage(chat_id, "DisplayReset", "");
    }
    #endif

    if (text.startsWith("/TEST")) {
      String test = "Chatt ID: " + chat_id + " \n";
      test = test + "User ID: " + from_id + " \n";
      test = test + "User Name: " +  from_name + " \n";
      test = test + "Text: " +  text + " \n";
      bot.sendMessage(chat_id, test, "Markdown");
      #ifdef OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(test);
      display.display();
      #endif
    }

    if (text.startsWith("/version")) {
      bot.sendMessage(chat_id, Version,"");
    }

    if (text.startsWith("/start")) {
      String welcome = Welcome_text + " " + Version + "\n";
      welcome = welcome + "/safelock : Lock the Safe\n";
      welcome = welcome + "/safeunlock : Unlock the Safe\n";
      welcome = welcome + "/takekey : Take the key from the safe\n";
      welcome = welcome + "/leavekey : Put the Key back\n";
      welcome = welcome + "/status : returns current status\n";
      welcome = welcome + "/setting : access the settings\n";
      welcome = welcome + "/vibration : trigger a vibration impuls *1\n";
      welcome = welcome + "/schock_singel : trigger a Singel shock impuls *2\n";
      welcome = welcome + "/schock_cont : trigger a shock impuls *1*2\n";
      welcome = welcome + "/options : returns the custom keyboard\n";
      welcome = welcome + "*1 Length of Impuls can be change in /setting\n";
      welcome = welcome + "*2 Strength of Impuls can be change in /setting\n";
      bot.sendMessage(chat_id, welcome, "");
      #ifdef DEBUG
        Serial.println("DEBUG_" + welcome + "DEBUG END");
      #endif

    }

  }
}

  void keyboard_Json(int setting) {
    if (setting == 2){
      keyboardJson = "[[\"/set_strength\"],[\"/set_schock_const_length\"],[\"/set_vibrations_length\"],[\"/back\"]]";
    }
    else{
      if (SafeStat) {
        //keyboardJson = "[[\"/shock\", \"/vibration\"],[\"/vibtailon\", \"/vibtailoff\"],[\"/status\", \"/safeunlock\"]]";
        keyboardJson = "[[\"/schock_singel\", \"/schock_cont\", \"/vibration\"],[\"/setting\",\"/leavekey\", \"/takekey\"],[\"/status\", \"/safeunlock\"]]";
      } else {
        //keyboardJson = "[[\"/shock\", \"/vibration\"],[\"/vibtailon\", \"/vibtailoff\"],[\"/status\", \"/safelock\"]]";
        keyboardJson = "[[\"/schock_singel\", \"/schock_cont\", \"/vibration\"],[\"/setting\",\"/leavekey\", \"/takekey\"],[\"/status\", \"/safelock\"]]";
      }
    }
    if (Alert) {
      keyboardJson = "[[\"/confirmAlert\"],[\"/schock_singel\", \"/schock_cont\", \"/vibration\"],[\"/setting\",\"/leavekey\", \"/takekey\"],[\"/status\", \"/safelock\"]]";
    }
  }
