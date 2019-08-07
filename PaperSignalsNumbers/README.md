![2S](../images/image1.png?raw=true)
# Simple Number Signal
The goal of this signal is to take an input of a single digit and show that digit on the face plate of your Paper Signal.

For instance, if you would want to show the digit 5, you would say

> Set the number signal parameter to five

or 
> Set the custom intent parameter to five

and your Paper Signal should turn the servo, to indicate the number five on the face plate.


## To do

This is a sketch that mostly work out of the box. All you have to do is to fill in your wifi credentials (share wifi from your phone) and upload the sketch to the ESP8266. 

You also have to draw the digits yourself on the face plate of the signal. Remember that the servo moves about 180 degrees, and that it could be a good idea to have a "no digit" position, to mark the start and end of a sequence. 
You might have to do some trial and erroring to get it right ;) 

If you want, you could do the **TASK** marked in APICalls.cpp, and enable the signal to show numbers with unlimited digits. We will use this functionality in the Calculator signal. 


## Solution

For this simple sketch, there are no specific solution, as everything works "out of the box". If you want to do the task, you could check out the Calculator signal, to see how it is done there. 

## FAQ

**Q:** I uploaded my signal, but nothing is happening?
**A** Try disconnecting and connecting your Paper Signal from your computer. There could be a connection error, due to the servo pin being affected by the upload. You should not need to upload again. 

**Q:** Oh no, it still won't work! 
**A** There could be an error with either your servo or your ESP8266. Bring the Paper Signal to one of the tutors, and we will check it for you :) 

**Q:** My Google Assistant / Paper Signal doesn't understand me. 
**A** Well, yes.. It's not a perfect voice recognition thing, we know. Also it could be affected by noise. If nothing works, you could type instead of talking. It could also help to change your cellphone language.
