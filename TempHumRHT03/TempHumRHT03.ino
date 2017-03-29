
//20/01/2014 added Dew Point
//26/02/2015 added 20x4 display
//12/03/2016 Display data
//13/03/2016 Removed Xively
//https://github.com/boutboutnico/LCD_I2C_lib

#include <DHT22.h>
#include <stdio.h> // Only used for sprintf
#include <SPI.h>
#include <Ethernet.h>
#include <lcd_I2C.h>
#include <Wire.h>

// Data wire is plugged into port 7 on the Arduino
const int DHT22_PIN = 7;
const int ledPin = 6;
int faultPin = 5;
int ret;
int ledState = LOW;
int ledFault = LOW;
long previousMillis = 0;
long interval = 180000; //3 Mins //300000; 5 mins
float dew_point = 0.0;

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
// initialize the libary 
//LiquidCrystal lcd(8, 9, 5, 4, 3, 2);
LCD_I2C lcd(0x27, 20, 4);  // Set the LCD I2C address


// IP/MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {10,52,0,176};
byte gateway[] = {10,52,0,1};


char buf[16];

EthernetClient client;

void setup(void)
{
  lcd.begin();
  lcd.backlight(true);
  //lcd.cursor();    // Uncommeting helps with layout
  lcd.cursorXY(1,0);
  lcd.print("A Temp & Humidity");
  lcd.cursorXY(5, 1);  //2 1
  lcd.print("IOT Device   ");
  delay(6000);
  lcd.cursorXY(0, 2);     

      
  // start serial port
  Serial.begin(9600);
   
   Serial.println("Attempting to get an IP address using DHCP:");
   lcd.print("Getting IP Address");
   lcd.cursorXY(0,3);
   
   if (!Ethernet.begin(mac)) {
    Serial.println("failed to get an IP address using DHCP, trying manually");
        lcd.clear();
        lcd.cursorXY(0,3);
        lcd.print("IP Failed!!");
        digitalWrite(faultPin, HIGH);
       //Ethernet.begin(mac, ip, gateway);
        delay(10000);   
   }
  
  Serial.print("My address:");  
  Serial.println(Ethernet.localIP());
  
  lcd.print(Ethernet.localIP());
  delay(6000);
  lcd.clear();
  
  pinMode(ledPin, OUTPUT);
  pinMode(faultPin, OUTPUT);
    
}
//--------------------------------------------------------------------
// calculates dew point
// input: humidity [%RH], temperature [°C]
// output: dew point [°C]
float calc_dewpoint(float h,float t){

float k,dew_point ;
k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t);
dew_point = 243.12*k/(17.62-k);
return dew_point;
}

void loop(void)
{ 
 DHT22_ERROR_t errorCode;
  
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(3000); // set warm up to 3s
  unsigned long currentMillis = millis();
  // Serial.println(currentMillis); // debug
  
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
                  
      dew_point = calc_dewpoint(myDHT22.getHumidity(),myDHT22.getTemperatureC()); //calculate dew point
    // Serial.println(dew_point); //debug
    lcd.print("Dew Point: ");
    lcd.print(dew_point);
    lcd.print(" C ");
    
    lcd.cursorXY(0,1);     
      
      sprintf(buf, "%hi.%01hiC %i.%01i ", myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
                   
     lcd.print("Temp  Humidity    ");
     lcd.cursorXY(0,2);
     lcd.print(buf);
     lcd.print("%RH ");
     lcd.cursorXY(0, 0);
            
/*    if(currentMillis - previousMillis > interval)
      {
         previousMillis = currentMillis;
         datastreams[0].setFloat(myDHT22.getTemperatureC());         
         datastreams[1].setFloat(myDHT22.getHumidity());
         datastreams[2].setFloat(dew_point);
         
         lcd.clear();
         lcd.print("Uploading.......");
         Serial.println("Uploading it to Xively");
         ret = xivelyclient.put(feed, xivelyKey);
         Serial.print("xivelyclient.put returned ");
         lcd.setCursor(0, 1);
         lcd.print("put returned ");
         lcd.print(ret);
         Serial.println(ret);  
         delay(4000);
         lcd.clear();             
        
        //if ((ledFault == HIGH) && (Ethernet.maintain() == 2)){
         //ledFault = LOW;
         //digitalWrite(faultPin, ledFault);
       // } 
       
        if (ret != 200){
          lcd.setCursor(0, 0);
          lcd.print("Network Err        ");
          digitalWrite(faultPin, HIGH);
          lcd.setCursor(0, 1);
          ledFault = HIGH;
          Ethernet.maintain();
        }
        else {
          ledFault = LOW;
          digitalWrite(faultPin, LOW);        
        }          
        
        if (ledState == LOW){
           ledState = HIGH;
        }
        else {
           ledState = LOW;
        }
        
        digitalWrite(ledPin, ledState);
        digitalWrite(faultPin, ledFault);  
   */   
      break;
    case DHT_ERROR_CHECKSUM:
      lcd.cursorXY(0, 3);
      lcd.print("Check Sum");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
    case DHT_BUS_HUNG:
      lcd.cursorXY(0, 3);
      lcd.print("BUS Hung");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
    case DHT_ERROR_NOT_PRESENT:
     lcd.cursorXY(0, 3);
      lcd.print("Not Present");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
    case DHT_ERROR_ACK_TOO_LONG:
     lcd.cursorXY(0, 3);
      lcd.print("Ack Too Long");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      lcd.cursorXY(0, 3);
      lcd.print("Sync Timeout");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      lcd.cursorXY(0, 3);
      lcd.print("DHT-Error-Timeout");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
    case DHT_ERROR_TOOQUICK:
      lcd.cursorXY(0, 3);
      lcd.print("DHT-Error Too Quick");
      digitalWrite(faultPin, HIGH);
      lcd.cursorXY(0, 0);
      break;
  }
}
