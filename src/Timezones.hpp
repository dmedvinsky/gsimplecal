#ifndef TIMEZONES_HPP
#define TIMEZONES_HPP

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
