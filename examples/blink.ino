// circuit diagram - connect +ve end of external LED to 10 and negative to GND

int led = LED_BUILTIN;
int led2 = 10;

void setup(){
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);
}

void loop(){
    digitalWrite(led, HIGH);
    digitalWrite(led2, LOW);
    delay(1000);
    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
    delay(1000);

}
