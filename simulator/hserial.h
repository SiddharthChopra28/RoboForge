#ifndef ROBOFORGE_HSERIAL_H
#define ROBOFORGE_HSERIAL_H


#include<iostream>
using namespace std;

class HSerial{
public:
    void begin(unsigned long){};
    void end(){};

    template <typename T> size_t print(T value){
        try{
            cout<<value;
            return string(value).length();
        }
        catch(...){
            return 0;
        }
    }

    template <typename T> size_t println(T value){
        try{
            cout<<value<<endl;
            return string(value).length();
        }
        catch(...){
            return 0;
        }
    }

    void flush();

    string readString();
    int read();
    int parseInt();
    float parseFloat();

};


#endif
