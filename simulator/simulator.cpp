#include "Arduino.h"

Socket sock = Socket("127.0.0.1", 7814);

void send_msg(){
    static string prev;
    giveOutput(comps, container);
    if (prev == container.dump()){
        return;
    }
    sock.sendMessage(container.dump());
    prev = container.dump();
    container.clear();

}

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
        cout<<std::flush;

    }

    shutdown(sock.sock, SHUT_RDWR);
    close(sock.sock);

}



