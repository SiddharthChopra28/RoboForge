#ifndef ROBOFORGE_HSERIAL_H
#define ROBOFORGE_HSERIAL_H


#include<iostream>
using namespace std;
#include "json.hpp"

using json = nlohmann::json;

class HSerial{
public:
    string buffer;

    void begin(unsigned long);
    void end();

    template <typename T> size_t print(T value){
        try{
//            string val = std::to_string(value);
//            buffer+=val;
            cout<<value;
            return 0;

        }
        catch(...){
            return 0;
        }
    }

    template <typename T> size_t println(T value){
        try{
//            string val = std::to_string(value);
//            buffer+=val+'\n';
            cout<<value<<endl;


            return 0;
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
