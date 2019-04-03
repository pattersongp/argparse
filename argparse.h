#include <string>
#include <functional>
#include <map>
#include <vector>

class Argument {
public:
    typedef std::function<void (void)> Function;
private:
    std::string opt;
    std::string optlong;
    Function callback;
public:
    Argument(std::string opt, std::string optlong, Function callback)
        : opt(opt), optlong(optlong), callback(callback) { }

    std::string &getOpt() { return opt; }
    void operator()(void) { callback(); }
};

class ArgParse {
private:
    std::vector<Argument> args;
    std::string name;
    std::map<std::string, std::vector<std::string>> actuals;

    std::map<std::string, std::vector<std::string>> parseArgv(char *argv[]);
public:
    ArgParse(std::string progName = "");

    void addArg(std::string opt, std::string optlong, Argument::Function f);
    void addAction(Argument::Function f);

    void parse(char *argv[]);
    std::string getFlags();
    std::vector<std::string> getActuals(std::string key) { return actuals[key]; }
};
