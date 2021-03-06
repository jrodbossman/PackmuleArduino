void processSerialInput() {
   while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
    // else add it to the inputString
      buffer += inChar;
    }
  }
  if(stringComplete){
    switch (buffer[0]) {
      case 'h':
        shouldHonk = true;
        previousMillis = 0;
        Serial2.print(" ");
        break;
      case 'm':
        manualMode = true;
        ST.turn(0);
        ST.drive(0);
        Serial2.print(" ");
        break;
      case 'a':
        manualMode = false;
        Serial2.print(" ");
        break;
      case 'e':
          emergencyStopEngaged = true;
          ST.drive(0);
          ST.turn(0);
          Serial2.print("Halted");
          break;
      case 'd':
          emergencyStopEngaged = false;
          ST.drive(0);
          ST.turn(0);
          Serial2.print(" ");
          break;
      default:
        if(manualMode){
          if(buffer.length() != 6) {
            break;
          }
          int speed = buffer.substring(0,3).toInt() - 127;
          int direction = buffer.substring(3).toInt() - 127;
          if(speed < 0) {
            shouldPlayBackupTone = true;
          }
          else {
            if(!firstBeep && !shouldHonk && !shouldPlayStartup){ 
              // Stop the backup sound when necessary
              noNewTone(HORN_PIN);
            }
            shouldPlayBackupTone = false;
            firstBeep = true;
          }
          ST.drive(speed);
          ST.turn(direction);
          //Serial.println(buffer);
        }
        break;        
    }
    stringComplete = false;
    buffer = "";    
  }
}
