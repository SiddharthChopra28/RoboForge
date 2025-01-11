#ifndef ROBOFORGE_ENGINE_H
#define ROBOFORGE_ENGINE_H

#include<iostream>
//#include<vector>
#include "components.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void processInput(string& recd, bool& exit, std::vector<Component*>& comps, std::vector<Wire*>& wires, Arduino& ard);
string giveOutput();

#endif
