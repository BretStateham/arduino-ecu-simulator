#include <mcp_can.h>
#include <SPI.h>
#include "LiquidCrystal_I2C.h"
#include "pids.h"

#define I2C_ADDR 0x27  // Add your address here.
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

//Using Cory J Fowler's fork of the Seeed Studio CAN BUS Shield library:
//https://github.com/coryjfowler/MCP_CAN_lib

//Using the New LiquidCrystal I2C Library from:
//https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// Using this to try and test the MCP2515 data ready interrupt.
// #define led_pin 12

#define CAN0_INT 2                                     // Set INT to pin 2
MCP_CAN CAN0(10);                                      // Set CS to pin 10 for the ElecFreaks CAN-BUS Shield v1.2

// This will store the ID of the incoming can message
long unsigned int canId = 0x000;
// this is the length of the incoming message
unsigned char len = 0;
// This the eight byte buffer of the incoming message data payload
unsigned char buf[8];
String canMessageRead="";
//char str[20];

int getSensor(int pin, int inMin, int inMax, unsigned int outMin, unsigned int outMax, String sensorName){
  
  //Read the sensor
  int sensor = analogRead(pin);

  //Clamp the sensor to be no greater than inMax
  if(sensor > inMax) { sensor = inMax; }
  
  //Map it to the desired output min-max range
  int s = map(sensor,inMin,inMax,outMin,outMax);
  
  //Clamp it to the desired output min-max range
  s = max(min(s,outMax),outMin);
  
  //Serial.print(sensorName + ": ");
  //Serial.print(sensor);
  //Serial.print(",");
  //Serial.println(s);
  return s;
}

void showSensorOnLCD(int line, String label, unsigned int value)
{
  label = label + ":";
  String stringVal = String(value);
  int spaces = 20 - label.length() - stringVal.length();
    
  lcd.setCursor(0,line);
  lcd.print(label);
  Serial.print(label);
  for(int s = 0; s < spaces; s++){
    lcd.print(" ");
    Serial.print(" ");
  }
  lcd.print(stringVal);
  Serial.println(stringVal);

  
}

byte uintMSB(unsigned int value)
{
  return (byte)((value & 0xFF00) >> 8);
}

byte uintLSB(unsigned int value)
{
  return (byte)(value & 0x00FF);
}

// volatile int state = LOW;

//void pin_ISR() {
//  state = !digitalRead(CAN0_INT);
//  digitalWrite(led_pin,state);
//}

void setup() {
  Serial.begin(115200);

  //pinMode(led_pin,OUTPUT);
  //digitalWrite(led_pin,LOW);

  lcd.begin (20,4); //  our LCD is a 20x4, change for your LCD if needed
  
  // LCD Backlight ON
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();  // Clear the LCD
  lcd.home (); // go home on LCD
  lcd.print("Bret's Arduino ECU"); 
  
START_INIT:

  // Try and open the CAN controller:
  // https://github.com/coryjfowler/MCP_CAN_lib/blob/master/mcp_can_dfs.h
  // MCP_ANY = Disables Masks and Filters and accepts any message on the CAN Bus (MCP_STDEXT,MCP_STD,MCP_EXT,MCP_ANY)
  // CAN_500KBPS specifies a baud rate of 500 Kbps
  // MCP_16MHZ indicates a 16MHz oscillator (crystal) is used as a clock for the MCP2515 chip
  //           you need to check your MCP2515 circuit for the right frequency (MCP_8MHZ, MCP_16MHZ, MCP_20MHZ)
  if(CAN_OK == CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ))                   
  {
      Serial.println("CAN BUS Shield init ok!");
      CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.
      pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input

      //attachInterrupt(0, pin_ISR, CHANGE);
  }
  else
  {
      Serial.println("CAN BUS Shield init fail");
      Serial.println("Init CAN BUS Shield again");
      delay(100);
      goto START_INIT;
  }
}

void loop()
{
    byte ect         = getSensor(0,0,1000,0,255, "ECT");   //inMax SHOULD legitimately be 1023 but setting to 1000 for innacurate pots
    unsigned int rpm = getSensor(1,0,1000,0,65535, "RPM"); //inMax SHOULD legitimately be 1023 but setting to 1000 for innacurate pots
    unsigned int maf = getSensor(2,0,1000,0,65535, "MAF"); //inMax SHOULD legitimately be 1023 but setting to 1000 for innacurate pots
    
    //SHOW Sensors Readings on LCD
    showSensorOnLCD(1,"ECT",ect);
    showSensorOnLCD(2,"RPM",rpm);
    showSensorOnLCD(3,"MAF",maf);

    // Define the set of PIDs you wish you ECU to support.  For more information, see:
    // https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_00
    // For this sample, we are only supporting the following PIDs
    // PID (HEX)  PID (DEC)  DESCRIPTION
    // ---------  ---------  --------------------------
    //      0x05         05  Engine Coolant Temperature
    //      0x0C         12  Engine RPM
    //      0x10         16  MAF Air Flow Rate

    // As per the information on bitwise encoded PIDs (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_00)
    // Our supported PID value is: 
    //     PID 0x05 (05) - Engine Coolant Temperature
    //     |      PID 0x0C (12) - Engine RPM
    //     |      |   PID 0x10 (16) - MAF Air Flow Rate
    //     |      |   |
    //     V      V   V
    // 00001000000100010000000000000000
    // Converted to hex, that is the following four byte value
    // 0x08110000

    
    // Of course, if you want your ECU simulator to be able to respond to any PID From 0x00 to 0x20, you can just use the following PID Bit mask
    // 11111111111111111111111111111111
    // Or 
    // 0xFFFFFFFF

    // Next, we'll create the bytearray that will be the Supported PID query response data payload using the four bye supported pi hex value
    // we determined above (0x08110000):
    
    //                      0x06 - additional meaningful bytes after this one (1 byte Service Mode, 1 byte PID we are sending, and the four by Supported PID value)
    //                       |    0x41 - This is a response (0x40) to a service mode 1 (0x01) query.  0x40 + 0x01 = 0x41
    //                       |     |    0x00 - The response is for PID 0x00 (Supported PIDS 1-20)
    //                       |     |     |    0x08 - The first of four bytes of the Supported PIDS value
    //                       |     |     |     |    0x11 - The second of four bytes of the Supported PIDS value
    //                       |     |     |     |     |    0x00 - The third of four bytes of the Supported PIDS value
    //                       |     |     |     |     |      |   0x00 - The fourth of four bytes of the Supported PIDS value
    //                       |     |     |     |     |      |    |    0x00 - OPTIONAL - Just extra zeros to fill up the 8 byte CAN message data payload)
    //                       |     |     |     |     |      |    |     |
    //                       V     V     V     V     V      V    V     V 
    byte SupportedPID[8] = {0x06, 0x41, 0x00, 0x08, 0x11, 0x00, 0x00, 0x00};
    
    //SENSORS
    byte ectSensor[8] = {3, 65, 0x05, ect};
    byte rpmSensor[8] = {4, 65, 0x0C, uintMSB(rpm), uintLSB(rpm)};
    byte mafSensor[8] = {4, 65, 0x10, uintMSB(maf), uintLSB(maf)};

    //if(CAN_MSGAVAIL == CAN.checkReceive())  
    if(!digitalRead(CAN0_INT))
    {
      
      CAN0.readMsgBuf(&canId, &len, buf); 
        // FROM: https://en.wikipedia.org/wiki/OBD-II_PIDs#CAN_(11-bit)_bus_format
        // A CANId value of 0x7DF indicates a query from a diagnostic reader, which acts as a broadcast address and accepts
        // responses from any ID in the range 0x7E8 to 0x7EF.  ECUs that can respond to OBD queries listen both to the functional 
        // broadcast ID of 0x7DF and one assigned in the range 0x7E0 to 0x&7E7.  Their response has an ID of their assigned ID
        // plus 8 (e.g. 0x7E8 through 0x7EF).  Typically, the main ECU responds on 0x7E8.
        Serial.print("<<");Serial.print(canId,HEX);Serial.print(",");

        for(int i = 0; i<len; i++)
        {  
          canMessageRead = canMessageRead + buf[i] + ",";
        }
        Serial.println(canMessageRead);
        
        //Check which message was received.
        if(canMessageRead=="2,1,0,0,0,0,0,0,") {CAN0.sendMsgBuf(0x7E8, 0, 8, SupportedPID);}
        
        //SEND SENSOR STATUSES
        if(canMessageRead=="2,1,5,0,0,0,0,0,")  {CAN0.sendMsgBuf(0x7E8, 0, 8, ectSensor);}
        if(canMessageRead=="2,1,12,0,0,0,0,0,"){CAN0.sendMsgBuf(0x7E8, 0, 8, rpmSensor);}
        if(canMessageRead=="2,1,16,0,0,0,0,0,"){CAN0.sendMsgBuf(0x7E8, 0, 8, mafSensor);}

        canMessageRead="";
    }
}
