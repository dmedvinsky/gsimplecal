#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Config
{
public:
    static Config* getInstance();

    // options
    bool show_calendar;
    bool show_timezones;

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
