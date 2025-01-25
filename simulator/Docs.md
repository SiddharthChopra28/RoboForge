# unoffical docs
Arduino uses 10 bit ADC  

things to add: serial output & input, **breadboard**, temperature sensor, dc motor

IMP: need to have g++ installed on linux  

some rules for the board:  
take default operating voltage = 5V  
it requires atleast 5V input in either Vin or Power port to work  
if power input is given to pinmode input pin, then no current will flow / we can  show error  
add functionality for reset pin (default state = high, if it is set to low momentarily, the program on the board restarts)  
ioref pin outputs the operating voltage of the board  
analog pins are read only but they can write digital signal. pwm digital pins can write analog signal, but not read analog signal  

in reality if pin is unused, the potential is floating but i consider it to be LOW by default
  
the second connected component should make sure to set the potential of the other end of the wire

// if pinmode/digitalRead/digitalWrite is used with an analog pin, it behaves like a digital pin temporarily   
if analogRead is used on it again, it transforms back to analog pin  


if we call digitalwrite on Pin in INPUT Mode:

digitalWrite(pin, HIGH): Enables the internal pull-up resistor on the pin.
digitalWrite(pin, LOW): Disables the internal pull-up resistor, leaving the pin floating.
Pin in INPUT_PULLUP Mode:

The pin is already configured with an internal pull-up resistor.
digitalWrite() has no additional effect, as the pull-up is always enabled in this mode.


if analogwrite is used on pin with inputmode then?  
if analogwrite is used on nonpwm pin, then if <-127 -> low else high  

For communication bw python and cpp code, using sockets  
python sends command to cpp, and cpp returns states of all present components  
after receiving command, also send back acknowledgment  

python sends msg in the form eg: "COMMAND:INIT:{data}" or "COMMAND:STOP:"


## Serial:

Functions supported so far
```
void begin(unsigned long baud);
template <typename T> size_t print(T value);
template <typename T> size_t println(T value);
// both of these return 0 if value is not of printable type
void end();
void flush();
string readString();
int read();
int parseInt();
float parseFloat();
```
# Time:  
supported funcs:
```
long long get_millis(hr_time_point)
long long get_micros(hr_time_point)
void delay(long int)
void delayMicroseconds(long int)
```
