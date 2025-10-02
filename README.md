# RoboForge
Virtual playground for designing and testing Arduino circuits  

### Note
currently works only on linux  

## Requirements
### For Linux
```cmake``` command should be available.  
```xterm``` should be available / installed for the app to work.  
python 3.12 with modules listed in requirements.txt

## User's manual
To run the app, use python ```main.py``` 

Example ino files in ```examples/``` are included for your reference.
  
To connect a wire, right click on any pin to get a wire at your mouse pointer. To connect this wire, right click again at any pin.  

Right clicking on a connected wire will disconnect it and attach it to your pointer. To delete a wire, press ```ESC``` while holding the wire with your pointer.  
  
To toggle / change the state / reading of any sensor, scroll on it with your mousewheel.  

Do not close the terminal window that pops up after running the code, or it will cause the simulator to stop.

If there is error in user code, it will cause compilation to fail and will show error in the python console  

![alt text](https://github.com/sdswoc/robo-forge/blob/master/assets/example.png?raw=true)

## Program architecture
The frontend / GUI is written in python using pygame, and connects to a custom generated C++ backend using websockets.  
When the simulation is started (run code button is pressed), the user's uploaded code is processed and freshly compiled, against a custom Arduino.h header file, that simulates the hardware functions virtually.  
This compiled code also has a communication layer added to it, that enables its connection with the GUI. The GUI sends a snapshot of the circuit and simulation conditions, and the user's code is executed based on those conditions.  
The backend also has the ability to write any simulated serial output to the console, using xterm.  
The backend periodically responds to the frontend with any effects on the simulation environment that need to be shown to the user, which are displayed accordingly on the GUI.  


