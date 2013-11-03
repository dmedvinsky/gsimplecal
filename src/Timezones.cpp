#include <sys/time.h>
#include <gtk/gtk.h>

#include "Config.hpp"
#include "Timezones.hpp"
#include "Clock.hpp"
#include "config.h"


Timezones::Timezones()
{
#if HAVE_GTK_BOX_NEW
    widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#else
    widget = gtk_vbox_new(false, 0);
#endif

    Config* config = Config::getInstance();
    for (unsigned int clock_num = 0; clock_num < config->clocks.size(); clock_num++) {
        Clock* clock = new Clock(config->clocks[clock_num]->label,
                                 config->clocks[clock_num]->timezone);
        clock->addToBox(widget);
        clocks.push_back(clock);
    }

    gtk_widget_show(widget);
}

Timezones::~Timezones()
{
    for (unsigned int clock_num = 0; clock_num < clocks.size(); clock_num++) {
        delete clocks[clock_num];
    }
    gtk_widget_destroy(widget);
}

void Timezones::updateTime()
{
    struct timeval clock_time;
    gettimeofday(&clock_time, 0);
    for (unsigned int clock_num = 0; clock_num < clocks.size(); clock_num++) {
        clocks[clock_num]->updateTime(clock_time);
    }
}
