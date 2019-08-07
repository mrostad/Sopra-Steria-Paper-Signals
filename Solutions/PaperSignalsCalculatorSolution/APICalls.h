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

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <time.h>
#include "Credentials.h"

#define SERVO_PIN 2

// Servo Positions and Intent specific Numbers

#define CENTER_POSITION 90

// Test Signal
#define TEST_FIRST_POSITION 10
#define TEST_SECOND_POSITION 170
#define TEST_NUM_SWINGS 5

class PaperSignals
{
public:
	PaperSignals() {};
	~PaperSignals() {};

	void StartUp();
	String getJson(String host, String url);
	String getJsonHTTP(String host, String url);
	void ParseIntentName(String intentName, String JSONData);
	String getSignalByID(String signalID);
	void RunPaperSignals();

	// Intent Types
	String TestSignalType = "TestSignal";

 
  //This are our custom intent type. This name will be what the intent you have to say to Google Assistant. 
  //If Google Assistant is having trouble understanding what you are saying, it can be a good idea to change the name of your custom intent here:
  String CustomIntentType = "Calculator";

	String currentIntent = "";
	String currentIntentTimeStamp = "";
	bool updatedIntentTimeStamp = true;


public:

	// Use WiFiClientSecure class to create TLS connection
	WiFiClientSecure client;
	const int httpsPort = 443;
  
  int lastTimerTime = 0;
  int numTestServoSwings = 0;
  String mostRecentDateString = "";

  Servo myservo;
  int currentServoPosition = 0;

	// Signal Functions
	void DefaultExecution(String JSONData);
	void TestSignalExecution(String JSONData);
	void CustomExecution(String JSONData);

  void Calculator(String customIntentData);
  int GetCalculation(String input);
  
	void MoveServoToPosition(int position, int speed);
};
