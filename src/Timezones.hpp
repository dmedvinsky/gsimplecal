#ifndef TIMEZONES_H
#define TIMEZONES_H

#include <vector>
#include <gtk/gtk.h>

#include "Boxable.hpp"
#include "Clock.hpp"


class Timezones : public Boxable
{
public:
    Timezones();
    ~Timezones();

    void updateTime();

protected:
    vector<Clock*> clocks;

};

#endif
