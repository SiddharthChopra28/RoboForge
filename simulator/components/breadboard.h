//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_BREADBOARD_H
#define ROBOFORGE_BREADBOARD_H

#include "base.h"

class BreadBoard: public Component{
public:
    BreadBoard(int no_pins, float ov);
    static std::shared_ptr<BreadBoard> create();
    void initialize();

};


#endif //ROBOFORGE_BREADBOARD_H