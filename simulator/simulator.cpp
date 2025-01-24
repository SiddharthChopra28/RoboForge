#include "Arduino.h"

Socket sock = Socket("127.0.0.1", 2892);

void send_msg(){
    static string prev;
    string o = giveOutput(comps);
    if (prev == o){
        return;
    }
    sock.sendMessage(o);
    prev = o;
}
int led = 10;

void setup(){
    pinMode(led, OUTPUT);

}

void loop(){
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);

}

int main(){
    ::start_time = hr_clock::now();

    setup();

    bool exit = false;
    string prev_msg;
    
    while (!exit){
        string recd = sock.getMessage();
        
        processInput(recd, exit, comps, wires, ard);
        if (!sleeping){
            loop();
        }
        send_msg();
    }

    shutdown(sock.sock, SHUT_RDWR);
    close(sock.sock);

}




