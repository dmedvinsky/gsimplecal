#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glib/gstdio.h>

#include "Config.h"

using namespace std;


Config* Config::_instance = NULL;

Config* Config::getInstance()
{
    if (_instance == NULL) {
        _instance = new Config();
        atexit(Destroy);
    }
    return _instance;
}

void Config::Destroy()
{
    delete _instance;
    _instance = NULL;
}


Config::Config()
{
    getDefaults();
    if (getFile()) {
        readFile();
    }
}

Config::~Config()
{
}


void Config::getDefaults()
{
    show_calendar = true;
}

bool Config::getFile()
{
    // use XDG config dir (~/.config/ usually)
    char *path_to_config;
    path_to_config = g_build_filename(g_get_user_config_dir(),
                                      "gsimplecal", "config", NULL);
    if (g_file_test(path_to_config, G_FILE_TEST_EXISTS)) {
        file.open(path_to_config, ios::in);
    }
    g_free(path_to_config);
    return file.is_open();
}

void Config::readFile()
{
    string line;
    while (!file.eof()) {
        getline(file, line);
        parseLine(line);
    }
    file.close();
}

void Config::parseLine(string line)
{
    if (!line.length() || line[0] == '#') {
        return;
    }
    int pos;
    for (pos = line.length() - 1; pos >= 0; pos--) {
        if (line[pos] == '=') {
            break;
        }
    }
    if (pos == -1) {
        return;
    }
    string var = strip(line.substr(0, pos));
    string val = strip(line.substr(pos + 1));

    cout << var << " = " << val << endl;

    addOption(var, val);
}

void Config::addOption(string var, string val)
{
    if (var == "show_calendar") {
        if (!fromString<bool>(show_calendar, val)) {
            show_calendar = true;
        }
    }
}

template <class T> bool Config::fromString(T& t, const string& s)
{
    istringstream iss(s);
    return !(iss >> t).fail();
}

string Config::strip(const string& s)
{
    return s.substr(s.find_first_not_of(' '),
                    s.find_last_not_of(' ') + 1);
}

