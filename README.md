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

