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
    bool hasRun;
public:
    Argument(ArgumentType type, string opt, string optlong,
        Function callback, string desc = "")
        : type(type), opt(opt), optlong(optlong), callback(callback),
        desc(desc), hasRun(false) { }
    Argument(const Argument &rhs)
        : type(rhs.type), opt(rhs.opt), optlong(rhs.optlong),
        callback(rhs.callback), desc(rhs.desc), hasRun(rhs.hasRun) { }
    Argument() { }

    const string &getOpt() const { return opt; }
    const string &getOptlong() const { return optlong; }
    const string &getDescription() const { return desc; }
    void operator()(void) { callback(); }
    ArgumentType getArgumentType() { return type; }
    bool hasDescription() const { return desc != ""; }
    void setHasRun() { hasRun = true; }
    bool getHasRun() { return hasRun; }

    friend ostream &operator<<(ostream &stream, const Argument &arg);
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
    void runAll();
    void run(string arg);
    string getFlags();
    vector<string> getActuals(string key) { return actualParameters[key]; }
};