void I2C_SetUp(){
  mcp.begin(0);

  mcp.pinMode(RemPower,OUTPUT);
  mcp.pinMode(RemSingel,OUTPUT);
  mcp.pinMode(RemShock,OUTPUT);
  mcp.pinMode(RemVib,OUTPUT);
  mcp.pinMode(RemOn, INPUT);
  mcp.pinMode(5,OUTPUT);
  mcp.digitalWrite(RemPower, HIGH);
  mcp.digitalWrite(RemSingel, HIGH);
  mcp.digitalWrite(RemShock, HIGH);
  mcp.digitalWrite(RemVib, HIGH);
  mcp.digitalWrite(5, LOW);
}
void I2C_PotSetUp(){
  Wire.begin();
  //TWBR = 12;  // 400 kHz (maximum)
  myMCP4551.begin();
  delay(100);
  myMCP4551.setWiper(20);//set level 10
}

void SetStrength(int strength){
Raw_Value = strength + strength;
    myMCP4551.setWiper(Raw_Value);
}


//  mcp.digitalWrite(Pin, value);
