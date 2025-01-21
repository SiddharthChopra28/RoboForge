#include "Arduino.h"
int a = 1;

void setup(){
//    Serial.begin(9600);
//    Serial.println("Hello world");
//    pinMode(LED_BUILTIN, OUTPUT);
//    digitalWrite(LED_BUILTIN, HIGH);

//    String x = Serial.readString();
//    Serial.println(x+";;;");


}

void loop(){
//    Serial.println("Bye world");
//    digitalWrite(LED_BUILTIN, LOW);
//    delay(1000);
//    digitalWrite(LED_BUILTIN, HIGH);

}

int main(){
    ::start_time = hr_clock::now();
    Socket sock = Socket("127.0.0.1", 8080);

    setup();

    bool exit = false;
    while (!exit){
        string recd = sock.getMessage();
        processInput(recd, exit, comps, wires, ard);
        loop();
        sock.sendMessage(giveOutput(comps));
    }
}



