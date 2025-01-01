# QFlightinstruments
QFlightinstruments is a collection of flight instruments Qt widgets.

## Components

Project consists of Qt flights instruments library and an example Qt GUI application.

### Electronic Flight Instrument System

* Electronic Attitude Direction Indicator (EADI)
* Electronic Horizontal Situation Indicator (EHSI)

![QFI](screenshot_01.jpg)

### Basic Six

* Airspeed Indicator (ASI)
* Attitude Indicator (AI)
* Altimeter (ALT)
* Turn Coordinator (TC)
* Heading Indicator (HI)
* Vertical Speed Indicator (VSI)

![QFI](screenshot_02.jpg)

## Usage

Both flight instruments library and an example application are intended to be built with ```qmake```. There are appropriate Qt Creator project files. Flight instruments library is located in the ```src/qfi/``` directory, it includes source code files, Qt Creator ```pri``` file, Qt Resource Compiler ```qrc``` file and instruments graphics files.

```libqfi.pro``` project files allows to create dynamic shared object containing instruments library.

### Creating simple Qt application video

[![Using QFlightinstruments](video_01.jpg)](https://www.youtube.com/watch?v=3V6-1mbGpxw)

## See also

Visit [http://marekcel.pl/qflightinstruments](http://marekcel.pl/qflightinstruments) for more information.
# ATCControlSimulator

I, Robert LaChausse, did not draw the dynamic tools. I merely used Marek M. Cel's base creation and expanded greatly, including adding audio instructions, resource files, working intuitive UI elements, a validation system, and more. This was turned the project from a
visual air traffic simulator to a controllable air traffic simulator. Big thanks to my professor for bringing Mareks base to attention and encouraging use of QT in lecture.

Final Score on project: 56/33, 170%. 
Final Score in class: A, 99%.
