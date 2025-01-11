import sys
import subprocess

TOP = '''#include "Arduino.h"
'''

BOTTOM = '''

int main(){
    ::start_time = hr_clock::now();
    ::ard = Arduino(32,5);
    Socket sock = Socket("127.0.0.1", 8080);

    setup();

    bool exit = false;
    while (!exit){
        string recd = sock.getMessage();
        processInput(recd, exit, comps, wires);
        loop();
        sock.sendMessage(";"+giveOutput()+";");
    }
}
'''


def read_file(filename):
    with open(filename, 'r') as f:
        s = f.read()
    return s

def write_code(filename, code):
    with open(filename, 'w') as f:
        f.write(code)

def ardparser(filename):
    c = TOP + read_file(filename) + BOTTOM
    write_code("./simulator/simulator.cpp", c)
    # g++ -o simulator simulator.cpp hserial.cpp funcs.cpp components.cpp _time.cpp
    # if sys.platform == "linux":
    #     subprocess.run("cd simulator && g++ -o simulator simulator.cpp hserial.cpp sock.cpp funcs.cpp components.cpp _time.cpp", shell=True)
    #     subprocess.Popen("./simulator/simulator")
    #
    #     # subprocess.Popen(["./simulator/simulator"])
    # elif sys.platform == "win32":
    #     # do something
    #     pass

