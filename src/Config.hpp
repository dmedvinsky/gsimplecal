#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef struct _ClockInfo
{
    string label;
    string timezone;
} ClockInfo;

class Config
{
public:
    static Config* getInstance();

    // options
    bool show_calendar;
    bool show_timezones;
    string clock_format;
    vector<ClockInfo*> clocks;

private:
    static Config* _instance;
    static void Destroy();

    template <class T> bool fromString(T& t, const string& s);
    string strip(string const& str);

    std::ifstream file;

protected:
    explicit Config();
    ~Config();

    void getDefaults();
    bool getFile();
    void readFile();
    void parseLine(string line);
    void addOption(string var, string val);

};

#endif
