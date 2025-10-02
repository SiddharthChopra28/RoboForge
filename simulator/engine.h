#ifndef ROBOFORGE_ENGINE_H
#define ROBOFORGE_ENGINE_H

#include<iostream>
//#include<vector>
#include "components/bundle.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void processInput(string& recd, bool& exit, std::vector<std::shared_ptr<Component>>& comps, std::vector<shared_ptr<Wire>>& wires, std::shared_ptr<Arduino> ard);

void giveOutput(std::vector<shared_ptr<Component>>& comps, json& j);

#endif
