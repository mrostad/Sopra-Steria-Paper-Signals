![2S](../images/image1.png?raw=true)
# Calculator Signal
The goal of this signal is to take an input of two operands and an operator, perform some math and show the result as a sequence of numbers on your Paper Signal. 

For instance, if you would want to show the result of 36 + 6, you could say 

> Set the calculator parameter to thirty six plus six

or 
> Set the custom intent parameter to thirty six plus six

and your Paper Signal should first show the digit 4, then the digit 2. 


## To do

Most of the source code have been structured out for you. It is up to you to handle the business code, to actually add the input numbers, and show show the numbers sequentially on the Paper Signal. Fill out the code where the `TASKS` are marked. Of course you are not limited to edit those sections. Do what you want, go crazy. 

Oh, and you also have to draw the digits yourself on the face plate of the signal. Remember that the servo moves about 180 degrees, and that it could be a good idea to have a "no digit" position, to mark the start and end of a sequence. 


## Solution
There is a suggestion for a solution in the Solutions/PaperSignalsCalculator folder. Feel free to look at it any time. But remember that you might as well come up with a better solution yourselves :) 

## FAQ

**Q:** I uploaded my signal, but nothing is happening?
**A** Try disconnecting and connecting your Paper Signal from your computer. There could be a connection error, due to the servo pin being affected by the upload. You should not need to upload again. 

**Q:** Oh no, it still won't work! 
**A** There could be an error with either your servo or your ESP8266. Bring the Paper Signal to one of the tutors, and we will check it for you :) 

**Q:** My Google Assistant / Paper Signal doesn't understand me. 
**A** Well, yes.. It's not a perfect voice recognition thing, we know. Also it could be affected by noise. If nothing works, you could type instead of talking. It could also help to change your cellphone language.
