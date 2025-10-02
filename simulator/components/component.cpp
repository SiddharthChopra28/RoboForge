//
// Created by siddharth on 10/2/25.
//

#include "component.h"
#include "wire.h"
#include "pin.h"

Component::Component(int no_pins, float ov){
    no_of_pins = no_pins;
    pins.resize(no_pins);
    operatingVoltage = ov;
}


void Component::connectPin(int pinid, std::shared_ptr<Wire> wire) {

    std::shared_ptr<Pin> pin = pins[pinid];

    if (!wire->connectEnd(pin)){
        return;
    }

    if (wire->get_no_ends_connected() == 2){
        pin->update_potential();
    }
}

void Component::disconnectPin(int pinid, std::shared_ptr<Wire> wire) const{
    std::shared_ptr pin = pins[pinid];

    if (!wire->disconnectEnd(pin)){
        return;
    }

    if (wire->get_no_ends_connected() == 1){
        std::shared_ptr<Pin> otherpin = (wire->p1 == nullptr) ? wire->p2 : wire->p1;

        if (pin->mode == OUTPUT){
            if (otherpin->mode == OUTPUT){
                return; // what to do??
            }
            else if (otherpin->mode == INPUT){
                otherpin->setPinPotential(otherpin->defaultPotential);
            }
        }
        else if (pin->mode == INPUT){
            if (otherpin->mode == INPUT){
                return; //? what to do?
            }
            if (otherpin->mode == OUTPUT){
                pin->setPinPotential(pin->defaultPotential);
            }
        }
    }
}
