#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <string>
#include <sys/time.h>
#include <gtk/gtk.h>

#include "Boxable.hpp"

using namespace std;


class Clock : public Boxable
{
public:
    Clock(const string& label, const string& timezone);
    ~Clock();

    void updateTime(const timeval& time);

protected:
    string timezone;

    GtkWidget* label_label;
    GtkWidget* time_label;

    string getTimeForTZ(const timeval& time, const string& zone);

};

#endif
