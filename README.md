AWind library
-------------
Introduction
------------
AWind (arduino window) is Arduino-library that is intended to simplify the use of touch TFT displays for visualization of sensors data in text or chart form. It is also possible to interact with GUI by the use of display touch feature. 
The library is implemented as general window framework that allows creation of simple GUIs. The GUI is consist of text fields, buttons, chart window and etc. 
Special keyboard window is implemented to provide runtime possibility of changing text fields.
At this moment only TFT with 320x240 resolution on Arduiono Mega is tested, but the library can be adjusted to other display dimensions without big efforts.

Installation
------------
AWind library is build on top of Henning Karlsen libraries (many thanks for that libraries. It have saved pretty much of my time): 
* [UTFT] (http://www.rinkydinkelectronics.com/library.php?id=51)
* [UTouch] (http://www.rinkydinkelectronics.com/library.php?id=55)

You need uncompressed them into  arduinosketchfolder/libraries/ folder

The content of awind_full.zip from https://github.com/AndreiDegtiarev/AWind/releases has to be also uncompressed into arduinosketchfolder/libraries/ folder.

Restart the IDE

How to connect TFT-display see the UTFT documentation.

Documentation
------------
API documentation: http://andreidegtiarev.github.io/AWind/

Examples
--------
In the each example folder there is a screen-shots to demonstrate how results should looks like
* [Text] (examples/Text/README.md): Simple example that demonstrate the basic use of text window + touch interaction
* [Charts] (examples/Charts/README.md): Simple example that demonstrate the basic use of chart window 
![](examples/Charts/Example.JPG)
* [Oscilloscope] (examples/Oscilloscope/README.md): Simple Oscilloscope with possibility to adjust (in runtime) sample ratio, voltage level and signal length
![](examples/Oscilloscope/Example.JPG)
![](examples/Oscilloscope/Example1.JPG)
* [SensorsMonitor] (examples/SensorsMonitor/README.md): This example is based on my personal application - monitor temperature in my camper (outside, inside, fridge: beer has to be cold :-) and etc.). The sensors in this example work in DEMO-mode and real sensors connections is not needed
![](examples/SensorsMonitor/Example.JPG)
* [Gauges] (examples/Gauges/README.md): This example demonstrates how to use gauge and button controls
![](examples/Gauges/Example.JPG)
* [Dialogs] (examples/Dialogs/README.md): This example demonstrates how handle nested dialogs

![](examples/Dialogs/Example.JPG)
* [Tab control] (examples/TabControl/README.md): This example demonstrates how to work with tab controls

![](examples/TabControl/Example.JPG)
* [Vacuum Pump] (examples/VacuumPump/README.md): This advance example demonstrates how to work with tab controls, timer, how interact with stepper motor, relay and etc.

![](examples/VacuumPump/VacuumPump.JPG)

* [Window selector] (examples/WindowSelector/README.md): This example demonstrates how to control child window visibility

![](examples/WindowSelector/Example.JPG)
