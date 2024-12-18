#include<iostream>
#include<fstream>
#include "basecode.h"


using namespace std;


string readFile(string filename){
    fstream file;
    file.open(filename, ios::in);

    string s, final_s = "";
    while(!file.eof()){
        getline(file,s);
        final_s+=s;
        final_s+='\n';
    }

    file.close();
    return final_s;
}

string editCode(string& code){

}

int main(int argc, char** argv){
    string filename = argv[1];
    
    string c = readFile(filename);
    
    // list of all things to include: https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h
    // also add support for libraries like servo, other sensors

    //only support uno for now
    // use pipes for communicating bw main thread and compiler

    c = defs+funcs+c+mainCode;
    //this code stored in c will be compiled by a standard cpp compiler and run in a seperate thread.
    
}