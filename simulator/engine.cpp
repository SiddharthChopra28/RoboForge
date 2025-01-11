#include "engine.h"

void processInput(string& recd, bool& exit, std::vector<Component*>& comps, std::vector<Wire*>& wires, Arduino& ard){

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
    cout<<cmd<<endl;
    if (cmd == "INIT"){
        string j_str = recd.substr(13);
        json circuit = json::parse(j_str);
        comps.clear();
        wires.clear();
        for (const auto& id: circuit["COMPS"]){
            switch(id.get<int>()){
                case (ARDUINO):
                    comps.push_back(&ard);
                case (IR):
                    comps.push_back(new IRSensor(3, 5));
            }
        }
        for (const auto& conn: circuit["CONNECTIONS"]){
            Wire w;

        }
    }
    else if (cmd == "EXIT"){
        exit = true;
        return;
    }

}

string giveOutput(){
    return "L bawt";
}