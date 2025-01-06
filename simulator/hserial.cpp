#include "hserial.h"


//void HSerial::begin(unsigned long baud){
//    //do nothing;
//}

//void HSerial::end(){
//    // do nothing;
//}


void HSerial::flush(){
    cout<<std::flush;
}

string HSerial::readString() {
    string x;
    getline(cin, x);
    return x;
}

int HSerial::read(){
    unsigned int byte;
    cin>>byte;
    return byte;
}

int HSerial::parseInt(){
    int num;
    cin>>num;
    return num;
}
float HSerial::parseFloat(){
    float num;
    cin>>num;
    return num;
}
