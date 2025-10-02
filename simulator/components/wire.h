//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_WIRE_H
#define ROBOFORGE_WIRE_H

#include "base.h"

class Wire : public std::enable_shared_from_this<Wire>{
public:
    std::shared_ptr<Component> c1 = nullptr;
    std::shared_ptr<Component> c2 = nullptr;

    std::shared_ptr<Pin> p1 = nullptr;
    std::shared_ptr<Pin> p2 = nullptr;


    bool connectEnd(std::shared_ptr<Pin>); //returns true if success. false if fail
    bool disconnectEnd(std::shared_ptr<Pin>); //returns true if success. false if fail
    int get_no_ends_connected() const;
};

#endif //ROBOFORGE_WIRE_H