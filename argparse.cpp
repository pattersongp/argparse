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

ostream &operator<<(ostream &stream, const Argument &arg) {
    stream << '\t' << arg.getOpt() << ", " << arg.getOptlong();
    if(arg.hasDescription())
        stream << "\n\t\t" << arg.getDescription();
    return stream;
}

// TODO need to check if an illegal argument
map<string, vector<string>> ArgParse::parseArgv(char *argv[]) {
    map<string, vector<string>> argumentMap;
    bool endOptions(false);
    auto parsedParams = getArgs(argv);
    string positionals = "positionals";
    string prev = positionals;
    string target = "";
    vector<string>::iterator it = parsedParams.begin();

    while(it != parsedParams.end()) {
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

            for(auto &pair : args) {
                if(pair.first == "positionals") continue;
                cout << args[pair.first] << '\n';
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
}

void ArgParse::runAll() {
    for(auto f : args) {
        auto opt = f.first;
        auto arg = f.second;
        if(f.second.getHasRun()) continue;
        if(actualParameters.find(arg.getOpt()) != actualParameters.end()
            || actualParameters.find(arg.getOptlong()) != actualParameters.end()) {

            f.second();
            arg.setHasRun();
        }
    }

}

// TODO templatize this for specific actions
void ArgParse::run(string toRun) {
        auto arg = args[toRun];
        arg.setHasRun();
        arg();
}

string ArgParse::getFlags() {
    string options;
    for(auto a : args) {
        if(a.first == "positionals") continue;
        options += a.second.getOpt().substr(1);
    }
    return options;
}