int temp_sens = A0;

void setup(){
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
    int sensorValue = analogRead(temp_sens);

    float temp = sensorValue*100.0/1023.0;
    if (temp>40){
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else{
        digitalWrite(LED_BUILTIN, LOW);
    }
    Serial.println(temp);

}