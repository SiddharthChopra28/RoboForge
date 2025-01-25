#include "Arduino.h"

Socket sock = Socket("127.0.0.1", 4420);

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

void setup(){
    Serial.begin(9600);
}

void loop(){
    Serial.println("Helloowowowowow");
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



