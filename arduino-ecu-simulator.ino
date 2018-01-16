#include <mcp_can.h>
#include <SPI.h>
#include "LiquidCrystal_I2C.h"

#define I2C_ADDR 0x27  // Add your address here.
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


MCP_CAN CAN(10);                                      // Set CS to pin 10

unsigned long canId = 0x000;

unsigned char len = 0;
unsigned char buf[8];
char str[20];

String BuildMessage="";
int MSGIdentifier=0;

int getSensor(int pin, int inMin, int inMax, int outMin, int outMax, String sensorName){
  
  //Read the sensor
  int sensor = analogRead(pin);
  
  //Map it to the desired output min-max range
  int s = map(sensor,inMin,inMax,outMin,outMax); //360,580,0,255
  
  //Clamp it to the desired output min-max range
  s = max(min(s,outMax),outMin);
  
  //Serial.print(sensorName + ": ");
  //Serial.print(sensor);
  //Serial.print(",");
  //Serial.println(s);
  return s;
}




void setup() {
  Serial.begin(115200);

  lcd.begin (20,4); //  our LCD is a 20x4, change for your LCD if needed
  
  // LCD Backlight ON
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();  // Clear the LCD
  lcd.home (); // go home on LCD
  lcd.print("Bret's Arduino ECU"); 
  
START_INIT:

  if(CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
  {
      Serial.println("CAN BUS Shield init ok!");
  }
  else
  {
      Serial.println("CAN BUS Shield init fail");
      Serial.println("Init CAN BUS Shield again");
      delay(100);
      goto START_INIT;
  }
}

void showSensorOnLCD(int line, String label, int value)
{
  label = label + ":";
  String stringVal = String(value);
  int spaces = 20 - label.length() - stringVal.length();
    
  lcd.setCursor(0,line);
  lcd.print(label);
  for(int s = 0; s < spaces; s++){
    lcd.print(" ");
  }
  lcd.print(stringVal);
}

void loop()
{
    int throttle = getSensor(0,360,580,0,255, "THROTTLE"); //random(0,255);
    int rpm= getSensor(1,0,1023,0,55, "RPM"); //random(1,55);
    int maf= getSensor(2,0,1023,0,255, "MAF"); //random(0,255);
    
    //SHOW Sensors Readings on LCD
    showSensorOnLCD(1,"THROTTLE",throttle);
    showSensorOnLCD(2,"RPM",rpm);
    showSensorOnLCD(3,"MAF",maf);
    
//    lcd.setCursor (0,1); // go to start of 2nd line
//    lcd.print("THROTTLE:" + String(throttle));
//    lcd.setCursor (0,2); // go to start of 2nd line
//    lcd.print("RPM:     " + String(rpm));
//    lcd.setCursor (0,3); // go to start of 2nd line
//    lcd.print("MAF:     " + String(maf));

    //GENERAL ROUTINE
    // HMM: http://techtinker.co.za/forum/viewtopic.php?f=14&t=10&start=30#p526
    // https://en.wikipedia.org/wiki/OBD-II_PIDs#Bitwise_encoded_PIDs
    unsigned char SupportedPID[8] = {4,65,0,0x081B8000};
    unsigned char MilCleared[7] =         {4, 65, 63, 34, 224, 185, 147}; 
    
    //SENSORS
    unsigned char throttleSensor[8] =   {4, 65, 0x11, throttle,0,0,0,0};
    unsigned char rpmSensor[8] =                {4, 65, 0x0C, rpm,0,0,0,0};
    unsigned char mafSensor[8] =          {4, 65, 0x10, maf,0,0,0,0};

    if(CAN_MSGAVAIL == CAN.checkReceive())  
    {
      
      CAN.readMsgBuf(&len, buf); 
        // FROM: https://en.wikipedia.org/wiki/OBD-II_PIDs#CAN_(11-bit)_bus_format
        // A CANId value of 0x7DF indicates a query from a diagnostic reader, which acts as a broadcast address and accepts
        // responses from any ID in the range 0x7E8 to 0x7EF.  ECUs that can respond to OBD queries listen both to the functional 
        // broadcast ID of 0x7DF and one assigned in the range 0x7E0 to 0x&7E7.  Their response has an ID of their assigned ID
        // plus 8 (e.g. 0x7E8 through 0x7EF)
        canId = CAN.getCanId();
        Serial.print("<");Serial.print(canId,HEX);Serial.print(",");

        for(int i = 0; i<len; i++)
        {  
          BuildMessage = BuildMessage + buf[i] + ",";
        }
        Serial.println(BuildMessage);
        
        //Check wich message was received.
        if(BuildMessage=="2,1,0,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 8, SupportedPID);}
        if(BuildMessage=="2,1,1,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 7, MilCleared);}
        
        //SEND SENSOR STATUSES
        if(BuildMessage=="2,1,17,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 8, throttleSensor);}
        if(BuildMessage=="2,1,16,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 8, mafSensor);}
        if(BuildMessage=="2,1,12,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 8, rpmSensor);}

        BuildMessage="";
    }
}
