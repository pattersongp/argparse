#include <string>
#include <functional>
#include <map>
#include <vector>

using namespace std;

class Argument {
public:
    typedef function<void (void)> Function;
	enum ArgumentType {
        TYPE_OPTION,
        TYPE_FLAG,
        TYPE_POSITIONAL,
	};

private:
    ArgumentType type;
    string opt;
    string optlong;
    Function callback;
public:
    Argument(ArgumentType type, string opt, string optlong, Function callback)
        : type(type), opt(opt), optlong(optlong), callback(callback) { }
    Argument(const Argument &rhs)
        : type(rhs.type), opt(rhs.opt), optlong(rhs.optlong),
        callback(rhs.callback) { }
    Argument() { }

    string &getOpt() { return opt; }
    string &getOptlong() { return optlong; }
    void operator()(void) { callback(); }
    ArgumentType getArgumentType() { return type; }
};

class ArgParse {
private:
    map<string, Argument> args;
    string name;
    map<string, vector<string>> actualParameters;

    map<string, vector<string>> parseArgv(char *argv[]);
public:
    ArgParse(string progName = "");

    void addArg(Argument::ArgumentType type, string opt, string optlong,
            Argument::Function f);

    void parse(char *argv[]);
    string getFlags();
    vector<string> getActuals(string key) { return actualParameters[key]; }
};
