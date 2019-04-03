#include <iostream>

#include "argparse.h"

static std::vector<std::string> getArgs(char *argv[]) {
    std::vector<std::string> actuals;
    char **tmp = argv+1;

    while(*tmp) {
        actuals.push_back(std::string(*tmp));
        ++ tmp;
    }
    return std::move(actuals);
}

std::map<std::string, std::vector<std::string>> ArgParse::parseArgv(char *argv[]) {
    std::map<std::string, std::vector<std::string>> argumentMap;

    auto actuals = getArgs(argv);
    std::string prev = "_imp";
    for(auto v : actuals) {
        if(v[0] == '-') {
            argumentMap[v] = std::vector<std::string> ();
            prev = v;
        }
        else {
            argumentMap[prev].push_back(v);
        }
    }

    return argumentMap;
}

ArgParse::ArgParse(std::string progName) {
    name = progName;
    addArg("-h", "--help", [&](){ std::cout << "usage: " << progName
            << " [" << getFlags() << "]" << std::endl;
    });
}

void ArgParse::addArg(std::string opt, std::string optlong,
    Argument::Function f) {

    args.push_back(Argument(opt, optlong, f));
}

void ArgParse::addAction(Argument::Function f) {
    args.push_back(Argument("", "", f));
}

void ArgParse::parse(char *argv[]) {
    actuals = parseArgv(argv);
    for(auto f : args) {
        if(actuals.find(f.getOpt()) != actuals.end()) {
            f();
        }
        else if(f.getOpt() == "") {
            f();
        }
    }
}

std::string ArgParse::getFlags() {
    std::string options;
    for(auto a : args) {
        options += a.getOpt().substr(1);
    }
    return options;
}
