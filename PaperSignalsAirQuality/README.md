![2S](../images/image1.png?raw=true)
# Air Quality Signal
The goal of this signal is to take an input of a city in the world, and the Paper Signal will show the amount of pollution, using the [Air Quality Index](https://en.wikipedia.org/wiki/Air_quality_index)
The AQI is a number, typically between 0 and 300, where 300 is the worst. ¨

For instance, if you would want to show the air quality of Beijing, you would say

> Set the pollution parameter to Beijing

or 
> Set the custom intent parameter to Beijing

and your Paper Signal should indicate the amount of pollution in Beijing right now. 


## To do

1. Fill in your wifi details in Credentials.h (share internet from your cellphone)

2. Most of the source code have been structured out for you. A function exists that fetches the AQI data from a free API: https://aqicn.org/api/ 
To make that work, you need to provide it with your own unique API key. You can get one for free at https://aqicn.org/data-platform/token/ (Really, it takes just a minute.)

3. To make the servo move based on the AQI we fetch, solve **TASK A** marked in APICalls.cpp

4. To improve the signal further, solve **TASK B** as well.

5. Finally, use your imagination, do what you want. 

## Solution
There is a suggestion for a solution in the Solutions/PaperSignalsAirQualitySolution folder. Feel free to look at it any time. But remember that you might also come up with a better solution yourselves :) 

## FAQ

See the [FAQ](../README.MD) on the main page.