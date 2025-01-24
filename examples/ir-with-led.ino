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