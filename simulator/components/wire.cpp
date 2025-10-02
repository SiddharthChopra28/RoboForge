//
// Created by siddharth on 10/2/25.
//

#include "wire.h"
#include "pin.h"

bool Wire::connectEnd(std::shared_ptr<Pin> pin) {
    if (c1 != nullptr && c2 != nullptr){
        return false;
    }
    if (pin == p1 || pin == p2){
        return false;
    }
    if (c1 == nullptr){
        p1 = pin;
        c1 = pin->parentComponent;
        pin->wireConnected = true;
        pin->wire = shared_from_this();
        return true;
    }
    else{
        p2 = pin;
        c2 = pin->parentComponent;
        pin->wireConnected = true;
        pin->wire = shared_from_this();
        return true;
    }
}



bool Wire::disconnectEnd(std::shared_ptr<Pin> pin){
    if (p1 == pin){
        pin->wireConnected = false;
        pin->wire = nullptr;
        p1 = nullptr;
        c1 = nullptr;
        return true;
    }

    if (p2 == pin){
        pin->wireConnected = false;
        pin->wire = nullptr;
        p2 = nullptr;
        c2 = nullptr;
        return true;
    }
    return false;
}


int Wire::get_no_ends_connected() const {
    if (c1 == nullptr && c2 == nullptr) {
        return 0;
    }
    if ((c1 != nullptr && c2 == nullptr) || (c1 == nullptr && c2 != nullptr)){
        return 1;
    }
    else{
        return 2;
    }
}
