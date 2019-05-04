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
    string desc;
public:
    Argument(ArgumentType type, string opt, string optlong,
        Function callback, string desc = "")
        : type(type), opt(opt), optlong(optlong), callback(callback) , desc(desc) { }
    Argument(const Argument &rhs)
        : type(rhs.type), opt(rhs.opt), optlong(rhs.optlong),
        callback(rhs.callback), desc(rhs.desc) { }
    Argument() { }

    const string &getOpt() const { return opt; }
    const string &getOptlong() const { return optlong; }
    const string &getDescription() const { return desc; }
    void operator()(void) { callback(); }
    ArgumentType getArgumentType() { return type; }
};

class ArgParse {
private:
    map<string, Argument> args;
    map<string, string> longToShort;
    map<string, vector<string>> actualParameters;
    string progName;
    string progDesc;

    map<string, vector<string>> parseArgv(char *argv[]);
public:
    ArgParse(string progName = "", string progDesc = "");

    void addArg(Argument::ArgumentType type, string opt, string optlong,
            Argument::Function f, string desc = "");

    void parse(char *argv[]);
    string getFlags();
    vector<string> getActuals(string key) { return actualParameters[key]; }
};
