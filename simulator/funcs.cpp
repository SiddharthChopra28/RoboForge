#include "funcs.h"

void pinMode_(std::shared_ptr<Arduino> ard, uint8_t pin, uint8_t mode){
    // only for arduino pins
    if (ard->pins[pin]->type == POWER){
        // pinmode should only work for digital and analog pins, not powerpins
        return;
    }
    if (ard->pins[pin]->mode == mode){
        return;
    }
    if (mode == INPUT){
        if (ard->pins[pin]->mode == INPUT_PULLUP){
            ard->pins[pin]->defaultPotential = 0;
            ard->pins[pin]->mode = INPUT_PULLUP;
        }
        else if (ard->pins[pin]->mode == OUTPUT){
            // what to do?
            ard->pins[pin]->mode = OUTPUT;
        }
    }
    else if (mode == OUTPUT){
        if (ard->pins[pin]->mode == INPUT_PULLUP){
            ard->pins[pin]->defaultPotential = 0;
            ard->pins[pin]->setPinPotential(0);
            ard->pins[pin]->mode = OUTPUT;
        }
        else if (ard->pins[pin]->mode == INPUT){
            ard->pins[pin]->setPinPotential(0);
            ard->pins[pin]->mode = OUTPUT;
        }
    }
    else if (mode == INPUT_PULLUP){
        // set default potential to high. in case it is being changed to digital or analog set deafault potential to 0
        if (ard->pins[pin]->mode == INPUT){
            ard->pins[pin]->defaultPotential = ard->operatingVoltage;
            ard->pins[pin]->mode = INPUT_PULLUP;
        }
        else if (ard->pins[pin]->mode == OUTPUT){
            ard->pins[pin]->defaultPotential = ard->operatingVoltage;
            ard->pins[pin]->setPinPotential(ard->operatingVoltage);
            ard->pins[pin]->mode = INPUT_PULLUP;
        }

    }
}

int digitalRead_(std::shared_ptr<Arduino> ard, uint8_t pin){
    if (ard->pins[pin]->type == POWER){
        return -1; //error
    }
    if (ard->pins[pin]->type == ANALOG){
        // for analog pins use threshold
        if (ard->pins[pin]->getPinPotential()>=ard->operatingVoltage/2.f){
            return HIGH;
        }
        else{
            return LOW;
        }
    }

    return bool(ard->pins[pin]->getPinPotential()/ard->operatingVoltage);

}

void digitalWrite_(std::shared_ptr<Arduino> ard, uint8_t pin, uint8_t val){
    if (ard->pins[pin]->type == POWER){
        return;
    }
    if (ard->pins[pin]->mode == INPUT){
        if (val == HIGH){
            ard->pins[pin]->defaultPotential = HIGH*ard->operatingVoltage;
            ard->pins[pin]->setPinPotential(HIGH*ard->operatingVoltage);
            ard->pins[pin]->update_potential();
        }
        else if (val == LOW){
            ard->pins[pin]->defaultPotential = LOW;
        }
    }
    else if (ard->pins[pin]->mode == INPUT_PULLUP){
        ard->pins[pin]->setPinPotential(HIGH*ard->operatingVoltage);
        ard->pins[pin]->update_potential();

    }
    else if (ard->pins[pin]->mode == OUTPUT){
        ard->pins[pin]->setPinPotential(float(val)*(ard->operatingVoltage));
        ard->pins[pin]->update_potential();

    }
}

void analogWrite_(std::shared_ptr<Arduino> ard, uint8_t pin, int val){
    if (ard->pins[pin]->type == DIGITALPWM){
        if (ard->pins[pin]->type == OUTPUT){ //only works if pin is in output mode
            ard->pins[pin]->setPinPotential(float(val)/255.f*ard->operatingVoltage);
            ard->pins[pin]->update_potential();

        }
    }
    else{
        if (val<=127){
            digitalWrite_(ard, pin, LOW);
        }
        else{
            digitalWrite_(ard, pin, HIGH*ard->operatingVoltage);
        }
    }
}

int analogRead_(std::shared_ptr<Arduino> ard, uint8_t pin){
    if (ard->pins[pin]->type == ANALOG){
        float x = ard->pins[pin]->getPinPotential()/ard->analogReference*1023.f;
        return int(x);
    }
    else{
        return 0; //error
    }
}

void analogReference_(std::shared_ptr<Arduino> ard, uint8_t mode){
    if (mode == DEFAULT){
        ard->analogReference = ard->operatingVoltage;
    }
    else if (mode == INTERNAL){
        ard->analogReference = 1.1;
    }
    else if (mode == EXTERNAL){
        ard->analogReference = (ard->pins[PIN_AREF]->getPinPotential()!=0) ? ard->pins[PIN_AREF]->getPinPotential() : ard->operatingVoltage;
    }
}
