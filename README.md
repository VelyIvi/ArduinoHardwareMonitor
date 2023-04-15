# Arduino Hardware Monitor
Monitors the hardware of you PC and displays it on an external display using an Arduino.

This was a one day challange for me to finally complete an Arduino project that is usefull. I exacly finished the at the deadline, so the challange was succesfull.
 
**The build has to be run with _admin privleges_ to work as intended.**
 
It uses openhardwaremonitor to read the data from your pc and using C# and serial sends it to the Arduino, where the microcontroller interprets it using Strings.
The pc UI was created using "Windows Forms Designer" in the .NET environment.
And the TFT lcd i used worked best with a custom library: TFT_HX8357



### Libraries
TFT Library: https://github.com/Bodmer/TFT_HX8357

Openhardwaremonitor: https://github.com/openhardwaremonitor

### Used projects/discussions:

https://projecthub.arduino.cc/zakrzu/f73c6780-5b6d-43e3-8f71-7ca992daa858

http://openhardwaremonitor.org/wordpress/wp-content/uploads/2011/04/OpenHardwareMonitor-WMI.pdf

https://forum.arduino.cc/t/need-help-parsing-serial-data-stream-please/210671/12
