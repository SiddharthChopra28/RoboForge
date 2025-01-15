#include "engine.h"

void processInput(string& recd, bool& exit, std::vector<Component*>& comps, std::vector<shared_ptr<Wire>>& wires, Arduino& ard){

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
                        comps.push_back(&ard);
                        break;
                    case (IR):
                        comps.push_back(new IRSensor(3, 5));
                        break;
                }
            }

        }
        catch(json::parse_error&){}
//        try{
            for (const auto& conn: circuit["CONNECTIONS"]){
                wires.push_back(make_shared<Wire>());

                comps[conn[0][0].get<int>()]->connectPin(conn[0][1].get<int>(),  *wires[wires.size()-1]);
                comps[conn[1][0].get<int>()]->connectPin(conn[1][1].get<int>(),  *wires[wires.size()-1]);
                cout<<"OK"<<endl;
                // still not working ;-;
            }


//        }
//        catch (json::parse_error){}
    }

    else if (cmd == "CONDITIONS"){
        string j_str = recd.substr(19);
        json conds = json::parse(j_str);
        for (const auto& status: conds){
            cout<<status.get<int>()<<endl;
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