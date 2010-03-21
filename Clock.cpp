#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <gtk/gtk.h>

#include "Clock.h"

using namespace std;


Clock::Clock(const string& label, const string& timezone)
{
    this->timezone = timezone;

    widget = gtk_hbox_new(true, 0);

    label_label = gtk_label_new(label.c_str());
    time_label = gtk_label_new(timezone.c_str());

    gtk_box_pack_start(GTK_BOX(widget), label_label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(widget), time_label, false, false, 0);

    gtk_widget_show(label_label);
    gtk_widget_show(time_label);
    gtk_widget_show(widget);
}

Clock::~Clock()
{
    gtk_widget_destroy(time_label);
    gtk_widget_destroy(label_label);
    gtk_widget_destroy(widget);
}


void Clock::updateTime(const timeval& time)
{
    string time_text = getTimeForTZ(time, timezone);
    gtk_label_set_text(GTK_LABEL(time_label), time_text.c_str());
}

string Clock::getTimeForTZ(const timeval& time, const string& zone)
{
    const char* old_tz = getenv("TZ");
    setenv("TZ", zone.c_str(), 1);

    struct tm* result = localtime(&time.tv_sec);

    if (old_tz) {
        setenv("TZ", old_tz, 1);
        delete[] old_tz;
    } else {
        unsetenv("TZ");
    }

    // format time
    char buffer[12];
    strftime(buffer, sizeof(buffer), "%H:%M", result);
    return string(buffer);
}
