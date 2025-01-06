TOP = '''#include "Arduino.h"
#include "sock.h"

'''

BOTTOM = '''

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

