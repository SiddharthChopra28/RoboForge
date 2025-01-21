#include "engine.h"

void processInput(string& recd, bool& exit, std::vector<shared_ptr<Component>>& comps, std::vector<shared_ptr<Wire>>& wires, std::shared_ptr<Arduino> ard){

    if (recd == "NODATA"){
        return;
    }

    if (recd.substr(0, 8) != "COMMAND:"){
        return;
        // corrupted input
    }

    string cmd = "";
    for (int i=8; i<recd.length(); i++){
        if (recd[i] == ':'){
            break;
        }
        cmd+=recd[i];
    }

    if (cmd == "CIRCUIT"){
        string j_str = recd.substr(16);
        json circuit;
        try{
            circuit = json::parse(j_str);
        }
        catch (json::parse_error&){
            return;
        }
        comps.clear();
        wires.clear();
        try{
            for (const auto& id: circuit["COMPS"]){
                switch(id.get<int>()){
                    case (ARDUINO):
                        comps.push_back(ard);
                        break;
                    case (IR):
                        comps.push_back(IRSensor::create(3,5));
                        break;
                    case (LED):
                        comps.push_back(Led::create(2, 5));
                        break;
                }
            }

        }
        catch(json::parse_error&){}
//        try{
            for (const auto& conn: circuit["CONNECTIONS"]){
                wires.push_back(make_shared<Wire>());

                comps[conn[0][0].get<int>()]->connectPin(conn[0][1].get<int>(),  wires.back());
                comps[conn[1][0].get<int>()]->connectPin(conn[1][1].get<int>(),  wires.back());
                cout<<"OK"<<endl;
            }

            cout<<"done";
//        }
//        catch (json::parse_error){}
    }

    else if (cmd == "CONDITIONS"){
        string j_str = recd.substr(19   );
        json conds = json::parse(j_str);
        for (int i=0; i<comps.size(); i++){
            comps[i]->setState(conds[i].get<int>());
        }
    }

    else if (cmd == "EXIT"){
        exit = true;
        return;
    }

}

string giveOutput(std::vector<shared_ptr<Component>>& comps){
    json j;

    std::vector<int> states;
    states.reserve(comps.size());
    for (auto & comp : comps){
        states.push_back(comp->getState());
    }
    j["STATES"] = states;
    return j.dump();
}