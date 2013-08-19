#include <DHT22.h>
#include <stdio.h> // Only used for sprintf
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

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

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Your Xively key to let you upload data
char xivelyKey[] = "7jM92EM2nDNPGPkePjLI3NHkU0DiCSimf1kACR2sdpqLlHCv";


// Define the strings for our datastream IDs
char myHumidSensor[] = "Humidity";
char myTempSensor[] = "Temperature";
char buf[128];

XivelyDatastream datastreams[] = {
  XivelyDatastream(myHumidSensor, strlen(myHumidSensor), DATASTREAM_FLOAT),
  XivelyDatastream(myTempSensor, strlen(myTempSensor), DATASTREAM_FLOAT),
  //XivelyDatastream(buf, strlen(buf), DATASTREAM_STRING)
};

// Finally, wrap the datastreams into a feed
XivelyFeed feed(211109309, datastreams, 2 /* number of datastreams */);

EthernetClient client;
XivelyClient xivelyclient(client);

void setup(void)
{
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
      Serial.print("Got Data ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      
      if(currentMillis - previousMillis > interval)
      {
         previousMillis = currentMillis;
         datastreams[0].setFloat(myDHT22.getHumidity());
         datastreams[1].setFloat(myDHT22.getTemperatureC());
         Serial.println("Uploading it to Xively");
         ret = xivelyclient.put(feed, xivelyKey);
         Serial.print("xivelyclient.put returned ");
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
