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

#include "APICalls.h"

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

void PaperSignals::ShowNumber(String customIntentData){
  Serial.println("Calculator input: " + customIntentData);
  int input = customIntentData.toInt();

  //Max number for this simple example is the number 9. If the number is greater, we simply set it to 9. 
  if(input>9){input = 9;} 

  //Move the servo to position to indicate number 9. (The servo moves from 0 to 180 degrees, remember?)
  MoveServoToPosition(180-(18*(input+1)), 4); 

  //Rest for some time, before fetching a new result from Firebase.
  delay(2000);


  /** 
   * ------------------------------------------TASK A------------------------------------------
   * This example can only support numbers 0-9. Can you extend it to show all numbers? 
   * Hint: Look at the PaperSignalsCalculator application, to see how it is done there (remember to include the necessary libraries)
   * ------------------------------------------------------------------------------------------
   */
   
}

void PaperSignals::CustomExecution(String JSONData)
{
  DynamicJsonBuffer customIntentBuffer;
  JsonObject& customIntentRoot = customIntentBuffer.parseObject(JSONData);

  //Pick just the custom parameter from the JSON file we have downloaded from Firebase. 
  String customIntentData = customIntentRoot["parameters"]["customParameter"];
  Serial.print("Current Custom Parameter Data: " + customIntentData);

  //Pass the custom parameter (the number we want to show) to the ShowNumber function.
  ShowNumber(customIntentData);

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

/**
 * When you give commands to your Paper Signal with your phone, an entry in a Google Firebase database will get updated. 
 * This function connects to the Google Firebase database and downloads the JSON file with the updates. 
 */
String PaperSignals::getSignalByID(String signalID){
    char* host = "gweb-paper-signals.firebaseio.com"; //The Google Firebase host
    String url = "/signals/" + signalID + ".json";    //Use our own Signal ID, which we have defined in Credentials.h
    DynamicJsonBuffer jsonBuffer;

    //Get the JSON file from Google Firebase
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
