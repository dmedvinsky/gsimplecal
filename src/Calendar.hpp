#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <gtk/gtk.h>

#include "Boxable.hpp"


class Calendar : public Boxable
{
public:
    Calendar();
    ~Calendar();

    void nextYear();
    void prevYear();
    void nextMonth();
    void prevMonth();

protected:
    void _change(int year_offset, int month_offset);
};

#endif
