#include <DHT22.h>
#include <stdio.h> // Only used for sprintf
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
#include <LiquidCrystal.h>

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 7
const int ledPin = 6;
int ret;
int ledState = LOW;
long previousMillis = 0;
long interval = 300000;

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);

// initialize the libary 
LiquidCrystal lcd(8, 9, 5, 4, 3, 2);

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Your Xively key to let you upload data
char xivelyKey[] = "7jM92EM2nDNPGPkePjLI3NHkU0DiCSimf1kACR2sdpqLlHCv";


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

EthernetClient client;
XivelyClient xivelyclient(client);

void setup(void)
{
  lcd.begin(16,2);
  //lcd.cursor();    // helps with layout
  lcd.setCursor(0,0);
  lcd.print("A Temp&Humidity");
  lcd.setCursor(0, 1);
  lcd.print("  IOT Device   ");
  delay(6000);
  lcd.clear();
  lcd.print("Temp    Humidity");
  lcd.setCursor(0, 1);
  
  // start serial port
  Serial.begin(9600);
  Serial.println("DHT22 Library Demo");
  
   Serial.println("Attempting to get an IP address using DHCP:");
   if (!Ethernet.begin(mac)) {
    Serial.println("failed to get an IP address using DHCP, trying manually");
        //Ethernet.begin(mac, ip, gateway);
        delay(10000);   
   }
  
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
  pinMode(ledPin, OUTPUT);
    
}

void loop(void)
{ 
 DHT22_ERROR_t errorCode;
  
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(3000);
  unsigned long currentMillis = millis();
  Serial.println(currentMillis);
  
  Serial.print("Requesting data...");
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      //Serial.print("Got Data ");
      //Serial.print(myDHT22.getTemperatureC());
      //Serial.print("C ");      
      //Serial.print(myDHT22.getHumidity());
      //Serial.println("%");
      
      sprintf(buf, "%hi.%01hi C, %i.%01i %%RH",
                   myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
      lcd.print(buf);
      lcd.setCursor(0, 1);
      
      if(currentMillis - previousMillis > interval)
      {
         previousMillis = currentMillis;
         datastreams[0].setFloat(myDHT22.getTemperatureC());         
         datastreams[1].setFloat(myDHT22.getHumidity());
         
         lcd.print("Uploading.......");
         Serial.println("Uploading it to Xively");
         ret = xivelyclient.put(feed, xivelyKey);
         Serial.print("xivelyclient.put returned ");
         lcd.setCursor(13, 1);
         lcd.print(ret);
         Serial.println(ret);               
         
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
      Serial.println("BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      Serial.println("ACK time out ");
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
