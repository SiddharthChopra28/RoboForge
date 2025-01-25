#include "Arduino.h"

Socket sock = Socket("127.0.0.1", 5613);

void send_msg(){
    static string prev;
    string o = giveOutput(comps);
    if (prev == o){
        return;
    }
    sock.sendMessage(o);
    prev = o;
}
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




