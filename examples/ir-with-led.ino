//circuit diagram - 1 external led with +ve at 11 & -ve at GND and another external LED with +ve at 10 and -ve at GND. One IR Sensor with out at 4, VCC to 5V and GND to GND

int led = 11;
int led2 = 10;
int ir = 4;

void setup(){
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(ir, INPUT);
}

void loop(){
    bool ir_reading = digitalRead(ir);
    if (ir_reading){
        digitalWrite(led, HIGH);
        digitalWrite(led2, HIGH);
    }
    else{
        digitalWrite(led, LOW);
        digitalWrite(led2, HIGH);
    }

}
