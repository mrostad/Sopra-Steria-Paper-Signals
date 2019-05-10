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

// #define PRINT_PAYLOAD

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}


String makeLessPrettyJSON(String JSONData)
{
  String notPretty = "";
  for(int i = 0; i < JSONData.length(); i++)
  {
    if(JSONData.charAt(i) != '\n' && JSONData.charAt(i) != '\r' &&
      JSONData.charAt(i) != ' ' && JSONData.charAt(i) != '  ' &&
      JSONData.charAt(i) != '[' && JSONData.charAt(i) != ']')
    {
      notPretty += JSONData.charAt(i);
    }
  }

  return notPretty;
}

void PaperSignals::StartUp()
{
  myservo.attach(SERVO_PIN);
  MoveServoToPosition(CENTER_POSITION, 10); // Initialize
  delay(1000);
  MoveServoToPosition(0, 10);
  delay(1000);
  MoveServoToPosition(180, 10);
  delay(1000);
  //MoveServoToPosition(CENTER_POSITION, 10);
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

int GetResult(String in)
{
  Serial.println("Inside GetResult");
  String calc[3];
  int result = 0;
  int slot = 0;
  for (int i = 0; i < in.length(); i++)
  {
    if (in[i] == ' ')
    {
      slot++;
      Serial.println("(Inside while) Continuing");
      continue;
    }
    calc[slot] += in[i];
    Serial.println("(Inside while) Calc array: " + calc[slot]);
  }

  Serial.println("Calc array: " + calc[0] + " " + calc[1] + " " + calc[2]);
  String op = calc[1];
  int op1 = calc[0].toInt();
  int op2 = calc[2].toInt();
  if (op == "+") {result = op1 + op2;}
  if (op == "-") {result = op1 - op2;}
  if (op == "x") {result = op1 * op2;}
  if (op == "/") {result = (int)round(op1 / op2);}
  if (op == "÷") {result = (int)round(op1 / op2);}
  if (op == "times") {result = op1 * op2;}
  if (op == "plus") {result = op1 + op2;}
  if (op == "minus") {result = op1 - op2;}

  Serial.println("Result is " + String(result));
  return result;
}

void PaperSignals::CustomExecution(String JSONData)
{
  DynamicJsonBuffer customIntentBuffer;
  JsonObject& customIntentRoot = customIntentBuffer.parseObject(JSONData);
  Serial.println("Hello, Calculator speaking");
  String customIntentData = customIntentRoot["parameters"]["customParameter"];

  Serial.println("String in: " + customIntentData);
  int answer = GetResult(customIntentData);
  int previousDigit = -1;

  std::stack<int> digits;
  while (answer > 0)
  {
    int digit = answer%10;
    answer /= 10;
    digits.push(digit);
  }

  while (!digits.empty())
  {
    int digit = digits.top();
    digits.pop();
    if (digit == previousDigit){
      MoveServoToPosition(180-(18*(digit+1)-10), 1);
      delay(50);
      MoveServoToPosition(180-(18*(digit+1)), 1);
    }else {
      MoveServoToPosition(180-(18*(digit+1)), 4);
    }
    
    previousDigit = digit; 
    delay(1000);
  }

  MoveServoToPosition(180, 1);
  delay(3000);

  Serial.print("Current Custom Parameter Data: "); Serial.println(customIntentData);
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

String PaperSignals::getJsonHTTP(String host, String url){

    HTTPClient http;
    String payload;

    Serial.print("[HTTP] begin...\n");

    http.begin("http://"+host+url); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            payload = http.getString();
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();


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
