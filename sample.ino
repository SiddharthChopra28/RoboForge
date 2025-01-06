int a = 1;

void setup(){
    Serial.begin(9600);
    Serial.print("Hello world");
}

void loop(){
    Serial.print("Bye world");
}