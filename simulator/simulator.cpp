#include "Arduino.h"
#include "sock.h"


int a = 1;

void setup(){
    Serial.begin(9600);
    Serial.println("Hello world");
    // NEED TO CHANGE IMPLEMENTATION OF PRINT, SHOULD NOT WRITE TO TERMINAL BUT INSTEAD SEND THE OUTPUT TO PYTHON
}

void loop(){
    Serial.println("Bye world");
}
int main(){
    ::start_time = hr_clock::now();
    setup();
    bool exit = false;
    Socket sock = Socket("127.0.0.1", 8080);

    while (!exit){
        string recd = sock.getMessage();
        if (recd!="NODATA"){
            cout << recd << endl;
        }
        loop();
        sock.sendMessage(";THIS IS A SAMPLE OUTPUT;");
    }
}
