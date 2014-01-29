<<<<<<< HEAD
//20/01/2014 added Dew Point

=======
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
#include <DHT22.h>
#include <stdio.h> // Only used for sprintf
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
#include <LiquidCrystal.h>

<<<<<<< HEAD

// Data wire is plugged into port 7 on the Arduino
const int DHT22_PIN = 7;
const int ledPin = 6;
int faultPin = A5;
=======
byte DownChar[8] = {
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b11111,
	0b01110,
	0b00100
};

byte UpChar[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100
};

// Data wire is plugged into port 7 on the Arduino
#define DHT22_PIN 7
const int ledPin = 6;
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
int ret;
int ledState = LOW;
long previousMillis = 0;
long interval = 300000;
<<<<<<< HEAD
float dew_point = 0.0;
=======
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
// initialize the libary 
LiquidCrystal lcd(8, 9, 5, 4, 3, 2);
<<<<<<< HEAD

// IP/MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {10,52,0,176};
byte gateway[] = {10,52,0,1};

// Your Xively key to let you upload data
char xivelyKey[] = "Your KEY";
// Define the strings for our datastream IDs
char myHumidSensor[] = "Humidity";
char myTempSensor[] = "Temperature";
char myDewPoint[] = "Dew_Point_Temperature";
char buf[16];

XivelyDatastream datastreams[] = {
  XivelyDatastream(myTempSensor, strlen(myTempSensor), DATASTREAM_FLOAT),
  XivelyDatastream(myHumidSensor, strlen(myHumidSensor), DATASTREAM_FLOAT),
  XivelyDatastream(myDewPoint, strlen(myDewPoint), DATASTREAM_FLOAT)
};

// Finally, wrap the datastreams into a feed
XivelyFeed feed(211109309, datastreams, 3 /* number of datastreams */);
=======
// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Your Xively key to let you upload data
char xivelyKey[] = "Your_API_KEY";
// Define the strings for our datastream IDs
char myHumidSensor[] = "Humidity";
char myTempSensor[] = "Temperature";
char buf[32];
XivelyDatastream datastreams[] = {
  XivelyDatastream(myTempSensor, strlen(myTempSensor), DATASTREAM_FLOAT),
  XivelyDatastream(myHumidSensor, strlen(myHumidSensor), DATASTREAM_FLOAT),
  //XivelyDatastream(buf, strlen(buf), DATASTREAM_STRING)
};

// Finally, wrap the datastreams into a feed
XivelyFeed feed(211109309, datastreams, 2 /* number of datastreams */);
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb

EthernetClient client;
XivelyClient xivelyclient(client);


void setup(void)
{
<<<<<<< HEAD
=======
  lcd.createChar(0, DownChar);
  lcd.createChar(1, UpChar);
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
  lcd.begin(16,2);
  //lcd.cursor();    // helps with layout
  lcd.setCursor(0,0);
  lcd.print("A Temp&Humidity");
  lcd.setCursor(0, 1);
  lcd.print("  IOT Device   ");
  delay(6000);
  lcd.clear();
<<<<<<< HEAD
  lcd.print("DP C ");
  lcd.print(" Temp  %RH");
=======
  lcd.print("Temp ");
  lcd.write((uint8_t)0);
  lcd.write((uint8_t)1);
  lcd.print(" Humidity");
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
  lcd.setCursor(0, 1);
  
  // start serial port
  Serial.begin(9600);
<<<<<<< HEAD
   
   Serial.println("Attempting to get an IP address using DHCP:");
   if (!Ethernet.begin(mac)) {
    Serial.println("failed to get an IP address using DHCP, trying manually");
        lcd.print("IP Failed!!");
        digitalWrite(faultPin, HIGH);
       //Ethernet.begin(mac, ip, gateway);
=======
  Serial.println("DHT22 Library Demo");
  
   Serial.println("Attempting to get an IP address using DHCP:");
   if (!Ethernet.begin(mac)) {
    Serial.println("failed to get an IP address using DHCP, trying manually");
        //Ethernet.begin(mac, ip, gateway);
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
        delay(10000);   
   }
  
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
  pinMode(ledPin, OUTPUT);
<<<<<<< HEAD
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

=======
    
}

>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
void loop(void)
{ 
 DHT22_ERROR_t errorCode;
  
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(3000); // set warm up to 3s
  unsigned long currentMillis = millis();
<<<<<<< HEAD
  //Serial.println(currentMillis); // debug
=======
 // Serial.println(currentMillis); // debug
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
  
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      //Serial.print("Got Data ");
      //Serial.print(myDHT22.getTemperatureC());
      //Serial.print("C ");      
      //Serial.print(myDHT22.getHumidity());
      //Serial.println("%");
<<<<<<< HEAD
            
      dew_point = calc_dewpoint(myDHT22.getHumidity(),myDHT22.getTemperatureC()); //calculate dew point
      //Serial.println(dew_point);
      lcd.print(dew_point); // Display dew point on LCD
      lcd.print("  ");     
      
      sprintf(buf, "%hi.%01hiC %i.%01i ", myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
                   
     // sprintf(buf, "%hi.%01hiC %i.%01i %%RH",
       //            myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
         //          myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
      lcd.print(buf);
      lcd.setCursor(0, 1);
            
      if(currentMillis - previousMillis > interval)     // Upload code
=======
      
      sprintf(buf, "%hi.%01hi C, %i.%01i %%RH",
                   myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
      lcd.print(buf);
      lcd.setCursor(0, 1);
      
      if(currentMillis - previousMillis > interval)
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
      {
         previousMillis = currentMillis;
         datastreams[0].setFloat(myDHT22.getTemperatureC());         
         datastreams[1].setFloat(myDHT22.getHumidity());
<<<<<<< HEAD
         datastreams[2].setFloat(dew_point);
=======
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
         
         lcd.print("Uploading.......");
         Serial.println("Uploading it to Xively");
         ret = xivelyclient.put(feed, xivelyKey);
         Serial.print("xivelyclient.put returned ");
         lcd.setCursor(13, 1);
         lcd.print(ret);
         Serial.println(ret);               
         
<<<<<<< HEAD
        if (ret != 200){
          lcd.setCursor(0, 0);
          lcd.print("Network Err        ");
          digitalWrite(faultPin, HIGH);
          lcd.setCursor(0, 1);
          ledState = HIGH;
        }
        
=======
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
        if (ledState == LOW)
           ledState = HIGH;
        else
           ledState = LOW;;
        digitalWrite(ledPin, ledState);
      }
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.print("check sum error ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      break;
    case DHT_BUS_HUNG:
<<<<<<< HEAD
      lcd.setCursor(0, 0);
      lcd.print("BUS Hung        ");
      digitalWrite(faultPin, HIGH);
      lcd.setCursor(0, 1);
=======
      Serial.println("BUS Hung ");
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
<<<<<<< HEAD
      lcd.setCursor(0, 0);
      digitalWrite(faultPin, HIGH);
      lcd.print("ACK time out         ");
      lcd.setCursor(0, 1);
=======
      Serial.println("ACK time out ");
>>>>>>> fb039c9ebb88cef71d2c375d3c4a4262fb6b55eb
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      Serial.println("Polled to quick ");
      break;
  }
}
