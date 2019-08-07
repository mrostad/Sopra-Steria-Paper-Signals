
![2S](./images/image1.png?raw=true)
# Sopra Steria Paper Signals workshop

Welcome to Sopra Steria's Paper Signal workshop! 

Please read [the manual](./Oppgave.pdf), and get started with the examples and assignments. 
You could do all of them or some of them (or none of them, if you have your completely own ideas!) in any order you want, 
but we suggest starting with *PaperSignalsNumber* just to get a short introduction, 
then continuing with *PaperSignalsCalculator* and *PaperSignalsAirQuality*. 


## To do
Clone or download this whole repository, and follow the instructions in [the manual](./Oppgave.pdf). 
Most of the source code have been structured out for you. It is up to you to handle the business code, and to fill out the code where it is needed. Of course you are not limited to edit those sections. Do what you want, go crazy. 
Each code example have its own README file. Be sure to read it. 

## Solutions
There are some suggestions for solutions in the *Solutions* folder. Feel free to look at them any time, or even just upload them to the ESP8266. But remember that you might also come up with better solutions yourselves :) 

## FAQ

**Q:** I uploaded my signal, but nothing is happening?  
**A:** Try disconnecting and connecting your Paper Signal from your computer. There could be a connection error, due to the servo pin being affected by the upload. You should not need to upload again. 

**Q:** Oh no, it still won't work!  
**A:** There could be an error with either your servo or your ESP8266. Bring the Paper Signal to one of the tutors, and we will check it for you :) 

**Q:** I don't understand why my signal won't function / what happens in the code?  
**A:** You can use Arduino's *Serial Monitor* to read the serial port of your ESP8266 when it is connected to your computer. Set the baud rate correctly (115200 in our examples) and you can read whatver is printed by `Serial.println(...);` in the code

**Q:** My Google Assistant / Paper Signal doesn't understand me.  
**A:** Well, yes.. It's not a perfect voice recognition thing, we know. Also it could be affected by noise. If nothing works, you could type instead of talking. It could also help to change your cellphone language.

**Q:** I get an error when compiling: "DynamicJsonBuffer is a class from ArduinoJson 5. Please see arduinojson.org/upgrade to learn how to upgrade your program to ArduinoJson version 6"  
**A:** You have installed an unsupported version of the ArduinoJson library. Go to Library Manager in Arduino and install the latest supported version, **version 5.13.5**