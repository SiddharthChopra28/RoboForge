import sys
import subprocess

TOP = '''#include "Arduino.h"

Socket sock = Socket("127.0.0.1", {{{{(PORT)}}}});

void send_msg(){
    static string prev;
    string o = giveOutput(comps);
    if (prev == o){
        return;
    }
    sock.sendMessage(o);
    prev = o;
}
'''

BOTTOM = '''

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




'''


def read_file(filename):
    with open(filename, 'r') as f:
        s = f.read()
    return s

def write_code(filename, code):
    with open(filename, 'w') as f:
        f.write(code)

def ardparser(filename, port):
    c = TOP.replace("{{{{(PORT)}}}}", str(port)) + read_file(filename) + BOTTOM
    write_code("./simulator/simulator.cpp", c)

    if sys.platform == "linux":
        subprocess.run("cd simulator && g++ -o simulator simulator.cpp hserial.cpp sock.cpp funcs.cpp components.cpp _time.cpp engine.cpp", shell=True)
        subprocess.Popen("./simulator/simulator")



    elif sys.platform == "win32":
        # do something
        pass

