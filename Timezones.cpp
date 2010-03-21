#include <sys/time.h>
#include <gtk/gtk.h>

#include "Config.h"
#include "Timezones.h"
#include "Clock.h"


Timezones::Timezones()
{
    widget = gtk_vbox_new(false, 0);

    clock = new Clock("Hawaii", ":US/Hawaii");
    clock->addToBox(widget);

    gtk_widget_show(widget);
}

Timezones::~Timezones()
{
    delete clock;
    gtk_widget_destroy(widget);
}

void Timezones::updateTime()
{
    struct timeval clock_time;
    gettimeofday(&clock_time, 0);
    clock->updateTime(clock_time);
}

