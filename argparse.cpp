#include <iostream>

#include "argparse.h"

using namespace std;

static vector<string> getArgs(char *argv[]) {
    vector<string> actuals;
    char **tmp = argv+1;
    bool endOptions = false;

    while(*tmp) {

        if(strcmp(*tmp, "--") == 0) {
            actuals.push_back(string(*tmp));
            endOptions = true;
            ++ tmp;
            continue;
        }

        // check if multiple flags passed as one eg -lfia
        if(!endOptions && **tmp == '-'
            && strlen(*tmp) > 2 && (*tmp)[1] != '-') {

            string multi = *tmp+1;            
            for(auto &c : multi) {
                string arg = "-";
                arg += c;
                actuals.push_back(arg);
            }
        }
        else {
            actuals.push_back(string(*tmp));
        }
        ++ tmp;
    }
    return move(actuals);
}

map<string, vector<string>> ArgParse::parseArgv(char *argv[]) {

    map<string, vector<string>> argumentMap;
    bool endOptions(false);
    auto parsedParams = getArgs(argv);
    string positionals = "positionals";
    string prev = positionals;
    string target = "";
    vector<string>::iterator it = parsedParams.begin();

#if 0
    cout << "Inspecting {";
    for(auto a : parsedParams) {
        cout << a << ", ";
    }
    cout << "}" << endl;
#endif

    while(it != parsedParams.end()) {

        // XXX need to check if an illegal argument

        // if only parsing positionals from now on
        if(it->compare("--") == 0) {
            endOptions = true;
            ++ it;
            continue;
        }

        // if optlong then convert to short
        if((*it).find("--") != (*it).npos) {
           *it = longToShort[*it];
        }

        if(!endOptions && (*it)[0] == '-') {
            argumentMap[*it] = vector<string> ();
            string tmp = *it;
            // cout << "Argument " << tmp << " has type " << args[tmp].getArgumentType() << endl;
            switch(args[tmp].getArgumentType()) {
                case Argument::TYPE_OPTION:
                    it ++;
                    for(; it != parsedParams.end() && !endOptions && (*it)[0] != '-'; ++ it)
                        argumentMap[tmp].push_back(*it);
                    break;
                case Argument::TYPE_POSITIONAL:
                case Argument::TYPE_FLAG:
                default: it ++;
            }
        }
        else {
            argumentMap[positionals].push_back(*it);
            it ++;
        }

        if(it == parsedParams.end()) break;
    }

    return argumentMap;
}

ArgParse::ArgParse(string name, string desc) {
    progDesc = desc;
    progName = name;
    addArg(Argument::ArgumentType::TYPE_FLAG, "-h", "--help",
        [&](){
            cout << progDesc << '\n' << "usage: " << progName << " "
                << "[" << getFlags() << "]" << '\n';
        for(auto &arg : args) {
            if(arg.first == "positionals") continue;
            cout << '\t' << arg.first << '\t'
                << arg.second.getDescription() << '\n';
        }
    });
}

void ArgParse::addArg(Argument::ArgumentType type, string opt,
        string optlong, Argument::Function f, string desc) {

    longToShort[optlong] = opt;
    args[opt] = Argument(type, opt, optlong, f, desc);
}

void ArgParse::parse(char *argv[]) {
    actualParameters = parseArgv(argv);

#if 0
    cout << "Parsed:" << endl;
    for(auto kv : actualParameters) {
        cout << "[" << kv.first << "]" "(" << kv.second.size() << ")"  << endl;
    }
#endif

    for(auto f : args) {
        auto opt = f.first;
        auto arg = f.second;

        if(actualParameters.find(arg.getOpt()) != actualParameters.end()
            || actualParameters.find(arg.getOptlong()) != actualParameters.end()) {

            // cout << "Found (" << arg.getOpt() << ")[" << arg.getOptlong() << "]" << endl;
            f.second();
        }
    }
}

string ArgParse::getFlags() {
    string options;
    for(auto a : args) {
        if(a.first == "positionals") continue;
        options += a.second.getOpt().substr(1);
    }
    return options;
}