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

int PaperSignals::GetCalculation(String input){
  String calcString[3]; //This is our "math problem" array. the first slot in the array is our first operand, the second slot is the operator, and the third slot is our second operand. 
  int stringIndex = 0;  //This will keep track of the index of the calc array
  int result = 0;       //this will be our final result

  for (int i = 0; i < input.length(); i++)
  {
    if (input[i] == ' ')
    {
      //If we find a space in our input, we know that we must move on in our 
      stringIndex++;
      continue;
    }
    // Add to the operand, or possibly add the operand to the 
    calcString[stringIndex] += input[i];
  }

  //Now that we have our math problem divided up in our array, we can extract the operands and the operator
  String op = calcString[1];
  int operand1 = calcString[0].toInt();
  int operand2 = calcString[2].toInt();

  //We try to parse what our operator is, solve the problem, and store the answer in our result variable. 
  if (op == "+") {result = operand1 + operand2;}
  if (op == "-") {result = operand1 - operand2;}
  if (op == "x") {result = operand1 * operand2;}
  if (op == "/") {result = (int)round(operand1 / operand2);}
  if (op == "÷") {result = (int)round(operand1 / operand2);}
  if (op == "times") {result = operand1 * operand2;}
  if (op == "plus") {result = operand1 + operand2;}
  if (op == "minus") {result = operand1 - operand2;}

  Serial.println("Result of calculation is " + result);
  return result;
}


void PaperSignals::Calculator(String customIntentData){
  Serial.println("Calculator input: " + customIntentData);
  
  int answer = GetCalculation(customIntentData);
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

    //Just something to jiggle the arrow, if the same digits comes twice in a row.
    if (digit == previousDigit){
      MoveServoToPosition(180-(18*(digit+1)-10), 1);
      delay(50);
      MoveServoToPosition(180-(18*(digit+1)), 1);
    }
    
    //If it is a new digit, then we move the servo, so the arrow points at it. 
    else {
      MoveServoToPosition(180-(18*(digit+1)), 4);
    }
    
    previousDigit = digit;

    //Let the arrow rest for one second on each digit
    delay(1000);
  }

  //After we have shown all digits, we go back to start, and wait three seconds. 
  MoveServoToPosition(180, 1);
  delay(3000);
}


void PaperSignals::CustomExecution(String JSONData)
{
  DynamicJsonBuffer customIntentBuffer;
  JsonObject& customIntentRoot = customIntentBuffer.parseObject(JSONData);
  String customIntentData = customIntentRoot["parameters"]["customParameter"];
  Serial.println("Current Custom Parameter Data: " + customIntentData);
  
  Calculator(customIntentData);

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
