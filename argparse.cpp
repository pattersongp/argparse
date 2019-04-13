#include <iostream>

#include "argparse.h"

using namespace std;

static vector<string> getArgs(char *argv[]) {
    vector<string> actuals;
    char **tmp = argv+1;

    while(*tmp) {
        actuals.push_back(string(*tmp));
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
    vector<string>::iterator it = parsedParams.begin();

    while(it != parsedParams.end()) {

        if(it->compare("--") == 0) {
            endOptions = true;
            continue;
        }

        if(!endOptions && (*it)[0] == '-') {
            argumentMap[*it] = vector<string> ();
            string tmp = *it;
            it ++;

            switch(args[*it].getArgumentType()) {
                case Argument::TYPE_OPTION:
                    for(; it != parsedParams.end() && (*it)[0] != '-'; ++ it)
                        argumentMap[tmp].push_back(*it);
                    break;
                case Argument::TYPE_POSITIONAL:
                case Argument::TYPE_FLAG:
                default: ;
            }
        }
        else {
            argumentMap[positionals].push_back(*it);
        }

        if(it == parsedParams.end()) break;
        it ++;
    }


    return argumentMap;
}

ArgParse::ArgParse(string progName) {
    name = progName;
    addArg(Argument::ArgumentType::TYPE_FLAG, "-h", "--help",
        [&](){ cout << "usage: " << progName
            << " [" << getFlags() << "]" << endl;
    });
}

void ArgParse::addArg(Argument::ArgumentType type, string opt,
        string optlong, Argument::Function f) {

    args[opt] = Argument(type, opt, optlong, f);
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
        if(actualParameters.find(f.first) != actualParameters.end()) {
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

