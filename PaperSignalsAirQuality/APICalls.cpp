/*
Copyright 2017 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
Copyright © 2017 All market data provided by Barchart Solutions.

BATS market data is at least 15-minutes delayed. Forex market data is at least 10-minutes delayed.
AMEX, NASDAQ, NYSE and futures market data (CBOT, CME, COMEX and NYMEX) is end-of-day.
Information is provided 'as is' and solely for informational purposes, not for trading purposes or advice,
and is delayed. To see all exchange delays and terms of use, please see our disclaimer.
*/

#include "APICalls.h"
#include "math.h"
#include <stack>

void PaperSignals::StartUp()
{
  myservo.attach(SERVO_PIN);

  //Move the servo, so we can see that the signal is alive
  MoveServoToPosition(0, 10);
  delay(300);
  MoveServoToPosition(180, 10);
  delay(300);
}

void PaperSignals::MoveServoToPosition(int position, int speed)
{
  if(position < currentServoPosition)
  {
    for(int i = currentServoPosition; i > position; i--)
    {
      myservo.write(i);
      delay(speed);
    }
  }
  else if(position > currentServoPosition)
  {
    for(int i = currentServoPosition; i < position; i++)
    {
      myservo.write(i);
      delay(speed);
    }
  }
  currentServoPosition = position;
}

void PaperSignals::DefaultExecution(String JSONData)
{
    Serial.println("Default");
}

void PaperSignals::TestSignalExecution(String JSONData)
{
  if(numTestServoSwings < TEST_NUM_SWINGS)
  {
    MoveServoToPosition(TEST_FIRST_POSITION, 10);
    MoveServoToPosition(TEST_SECOND_POSITION, 10);
    numTestServoSwings++;
  }
}

String PaperSignals::GetAirQuality(String Location)
{
  String airQualityHost = "api.waqi.info";
  String airQualityURL = "/feed/" + Location + "/?token=" + AirQualityAPIToken;

  // Get Weather
  String airQualityPayload = getJson(airQualityHost, airQualityURL);
  Serial.println("AirQuality payload for location " + Location + " is: \n" + airQualityPayload);

  return airQualityPayload;
}

void PaperSignals::AirQuality(String Location){

  String airQualityJson = GetAirQuality(Location);

  DynamicJsonBuffer jsonBufferAirQuality;
  JsonObject& airQualityRoot = jsonBufferAirQuality.parseObject(airQualityJson);

  int aqi = airQualityRoot["data"]["aqi"];
  Serial.println("AQI for " + Location + ": " + aqi);

  /**
   *------------------------------TASK A------------------------------
   *
   *Insert code to make the servo move based on the AQI for the specific city. 
   *AQI is usually a number between 0 and 300, and the servo takes a position between
   *0 and 180 degrees. 
   *
   *------------------------------------------------------------------
  **/

  /**
   *------------------------------TASK B------------------------------
   *
   *Air quality indexes dont get updated so often. 
   *Could you make it so that we only download air quality data if the Location has been changed, 
   *or if two minutes have passed? 
   *
   *------------------------------------------------------------------
  **/
}

void PaperSignals::CustomExecution(String JSONData)
{
  DynamicJsonBuffer customIntentBuffer;
  JsonObject& customIntentRoot = customIntentBuffer.parseObject(JSONData);
  String customIntentData = customIntentRoot["parameters"]["customParameter"];
  Serial.print("Current Custom Parameter Data: " + customIntentData);

  //Function to get the air quality, and move the servo based on the fetched data
  AirQuality(customIntentData);
  
}

void PaperSignals::ParseIntentName(String intentName, String JSONData)
{
    if(intentName == TestSignalType)
    {
      TestSignalExecution(JSONData);
    }
    else if(intentName.equalsIgnoreCase(CustomIntentType))
    {
      CustomExecution(JSONData);
    }
    else
    {
      DefaultExecution(JSONData);
    }
}

String PaperSignals::getSignalByID(String signalID){
    char* host = "gweb-paper-signals.firebaseio.com";
    String url = "/signals/" + signalID + ".json";
    DynamicJsonBuffer jsonBuffer;

    String payload = getJson(host, url);

    JsonObject& root = jsonBuffer.parseObject(payload);

    // Test if parsing succeeds.
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return "bad";
    }
    else
    {
      String signalInfo = root["result"];
      String intentName = root["result"]["metadata"]["intentName"];
      String intentTimeStamp = root["result"]["timestamp"];

      if(intentTimeStamp != currentIntentTimeStamp)
      {
        updatedIntentTimeStamp = true;
        numTestServoSwings = 0;
      }
      else {
        updatedIntentTimeStamp = false;
      }
      currentIntent = intentName;
      currentIntentTimeStamp = intentTimeStamp;

      return signalInfo;
    }
}

String PaperSignals::getJson(String host, String url){

    Serial.print("connecting to ");
    Serial.println(host);

    if (!client.connect(host.c_str(), httpsPort)) {
      Serial.println("connection failed");
      return "bad";
    }

    Serial.print("requesting URL ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("request sent");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if(line.indexOf("Date") != -1)
      {
        mostRecentDateString = line;
      }
      Serial.println(line);
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    String payload = client.readString();

    #ifdef PRINT_PAYLOAD
        Serial.println("reply was:");
        Serial.println("==========");
        Serial.println(payload);
        Serial.println("==========");
    #endif
    return payload;
}

void PaperSignals::RunPaperSignals()
{
  String JSONData = getSignalByID(SignalID.c_str());
  ParseIntentName(currentIntent, JSONData);
}
