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
    void goToday();
    void goLeft();
    void goDown();
    void goUp();
    void goRight();

    bool markToday();
    bool runExternalViewer();

protected:
    void _change(int year_offset, int month_offset, int day_offset);
    unsigned int _n_days(unsigned int year, unsigned int month);

private:
    guint today_year;
    guint today_month;
    guint today_day;
};

#endif
