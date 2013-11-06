#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>

#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "Config.hpp"

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
    path = NULL;
    setDefaults();
    if (findPath()) {
        readFile();
    }
}

Config::~Config()
{
    if (path) {
        g_free(path);
    }
    for (unsigned int i = 0; i < clocks.size(); i++) {
        delete clocks[i];
    }
}

const gchar *const Config::getPath()
{
    return path;
}

void Config::setDefaults()
{
    show_calendar = true;
    show_timezones = false;
    clock_format = string("%H:%M");
    mainwindow_decorated = false;
    mainwindow_keep_above = true;
    mainwindow_sticky = false;
    mainwindow_skip_taskbar = true;
    mainwindow_position = GTK_WIN_POS_MOUSE;
    mainwindow_xoffset = 0;
    mainwindow_yoffset = 0;
    mark_today = true;
    show_week_numbers = false;
    close_on_unfocus = false;
}

bool Config::findPath()
{
    // First try XDG_CONFIG_HOME (usually `~/.config`).
    const gchar *const user_config_dir = g_get_user_config_dir();
    path = g_build_filename(user_config_dir, "gsimplecal", "config", NULL);
    if (g_file_test(path, G_FILE_TEST_EXISTS)) {
        return true;
    }
    g_free(path);
    path = NULL;

    // Then try XDG_CONFIG_DIRS (or `/etc/xdg`).
    const gchar *const *const system_config_dirs = g_get_system_config_dirs();
    for (const gchar *const *dir = system_config_dirs; dir && *dir; dir++) {
        path = g_build_filename(*dir, "gsimplecal", "config", NULL);
        if (g_file_test(path, G_FILE_TEST_EXISTS)) {
            return true;
        }
        g_free(path);
        path = NULL;
    }

    // Otherwise, bail.
    return false;
}

void Config::readFile()
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return;
    }
    string line;
    while (!file.eof()) {
        getline(file, line);
        parseLine(line);
    }
}

void Config::parseLine(string line)
{
    if (!line.length() || line[0] == '#') {
        return;
    }
    size_t pos = line.find_first_of('=');
    if (pos == string::npos) {
        return;
    }
    string var = strip(line.substr(0, pos));
    string val = strip(line.substr(pos + 1));

    addOption(var, val);
}

void Config::addOption(string var, string val)
{
    if (var == "show_calendar") {
        if (!fromString<bool>(show_calendar, val)) {
            show_calendar = true;
        }
    } else if (var == "show_timezones") {
        if (!fromString<bool>(show_timezones, val)) {
            show_timezones = false;
        }
    } else if (var == "clock_format") {
        clock_format = val;
    } else if (var == "clock_label") {
        ClockInfo* clockinfo = new ClockInfo;
        clockinfo->label = val;
        clocks.push_back(clockinfo);
    } else if (var == "clock_tz") {
        clocks[clocks.size() - 1]->timezone = val;
    } else if (var == "mainwindow_decorated") {
        if (!fromString<bool>(mainwindow_decorated, val)) {
            mainwindow_decorated = false;
        }
    } else if (var == "mainwindow_keep_above") {
        if (!fromString<bool>(mainwindow_keep_above, val)) {
            mainwindow_keep_above = true;
        }
    } else if (var == "mainwindow_sticky") {
        if (!fromString<bool>(mainwindow_sticky, val)) {
            mainwindow_sticky = false;
        }
    } else if (var == "mainwindow_skip_taskbar") {
        if (!fromString<bool>(mainwindow_skip_taskbar, val)) {
            mainwindow_skip_taskbar = true;
        }
    } else if (var == "mainwindow_position") {
        if (val == "center") {
            mainwindow_position = GTK_WIN_POS_CENTER;
        } else if (val == "mouse") {
            mainwindow_position = GTK_WIN_POS_MOUSE;
        } else {
            mainwindow_position = GTK_WIN_POS_NONE;
        }
    } else if (var == "mainwindow_xoffset") {
        stringstream convert(val);
        if (!(convert >> mainwindow_xoffset)) {
            mainwindow_xoffset = 0;
        }
    }  else if (var == "mainwindow_yoffset") {
        stringstream convert(val);
        if (!(convert >> mainwindow_yoffset)) {
            mainwindow_yoffset = 0;
        }
    } else if (var == "mainwindow_resizable") {
        if (!fromString<bool>(mainwindow_resizable, val)) {
            mainwindow_resizable = true;
        }
    } else if (var == "mark_today") {
        if (!fromString<bool>(mark_today, val)) {
            mark_today = true;
        }
    } else if (var == "external_viewer") {
        external_viewer = val;
    } else if (var == "show_week_numbers") {
        if (!fromString<bool>(show_week_numbers, val)) {
            show_week_numbers = false;
        }
    } else if (var == "close_on_unfocus") {
        if (!fromString<bool>(close_on_unfocus, val)) {
            close_on_unfocus = false;
        }
    } else if (var == "force_lang") {
        force_lang = val;
    }
}

template <class T> bool Config::fromString(T& t, const string& s)
{
    istringstream iss(s);
    return !(iss >> t).fail();
}

string Config::strip(const string& s)
{
    string::size_type const first = s.find_first_not_of(' ');
    if (first == string::npos) {
        return string();
    }
    return s.substr(first, s.find_last_not_of(' ') - first + 1);
}
