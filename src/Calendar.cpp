#include <string.h>
#include <sys/time.h>

#include <gtk/gtk.h>

#include "Calendar.hpp"
#include "Config.hpp"


void fork_and_run(char* cmdline)
{
    if (strlen(cmdline)) {
        if (fork() == 0) {
            execl("/bin/sh", "/bin/sh", "-c", cmdline, NULL);
            _exit(0);
        }
    }
}

void monthChangedCb(GtkCalendar *calendar, gpointer cls)
{
    if (cls) {
        ((Calendar*)cls)->markToday();
    }
}
void dayDoubleClickCb(GtkCalendar *calendar, gpointer cls)
{
    if (cls) {
        ((Calendar*)cls)->runExternalViewer();
    }
}


Calendar::Calendar()
{
    widget = gtk_calendar_new();

    Config* config = Config::getInstance();
    g_object_set(widget,
                 "show-heading", true,
                 "show-day-names", true,
                 "show-details", false,
                 "show-week-numbers", config->show_week_numbers,
                 NULL);
    // Store today date to be able to jump to it.
    gtk_calendar_get_date((GtkCalendar*)widget,
                          &today_year, &today_month, &today_day);
    if (config->mark_today) {
        markToday();
        g_signal_connect(widget, "month-changed",
                         GCallback(monthChangedCb), (gpointer)this);
    }
    g_signal_connect(widget, "day-selected-double-click",
                     GCallback(dayDoubleClickCb), (gpointer)this);

    gtk_widget_show(widget);
}

Calendar::~Calendar()
{
    gtk_widget_destroy(widget);
}

void Calendar::nextYear()
{
    _change(1, 0, 0);
}
void Calendar::prevYear()
{
    _change(-1, 0, 0);
}

void Calendar::nextMonth()
{
    _change(0, 1, 0);
}
void Calendar::prevMonth()
{
    _change(0, -1, 0);
}

void Calendar::goLeft()
{
    _change(0, 0, -1);
}
void Calendar::goDown()
{
    _change(0, 0, 7);
}
void Calendar::goUp()
{
    _change(0, 0, -7);
}
void Calendar::goRight()
{
    _change(0, 0, 1);
}

void Calendar::_change(int year_offset, int month_offset, int day_offset)
{
    int year, month, day;
    gtk_calendar_get_date((GtkCalendar*)widget,
                          (guint*)&year, (guint*)&month, (guint*)&day);

    unsigned int n_days = _n_days(year, month);
    day += day_offset;
    if (day < 1) {
        day += _n_days(year, month - 1);
        month_offset--;
    } else if (day > n_days) {
        day -= n_days;
        month_offset++;
    }

    month += month_offset;
    if (month > 11) {
        month = 0;
        year_offset++;
    } else if (month < 0) {
        month = 11;
        year_offset--;
    }

    year += year_offset;

    gtk_calendar_select_month((GtkCalendar*)widget, (guint)month, (guint)year);
    gtk_calendar_select_day((GtkCalendar*)widget, day);
}

unsigned int Calendar::_n_days(unsigned int year, unsigned int month)
{
    if (month == 3 || month == 5 || month == 8 || month == 10) {
        return 30;
    } else if (month == 1) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (leap) {
            return 29;
        } else {
            return 28;
        }
    } else {
        return 31;
    }
}

bool Calendar::markToday()
{
    guint year, month;
    gtk_calendar_get_date((GtkCalendar*)widget, &year, &month, NULL);
    if (year == today_year && month == today_month) {
        gtk_calendar_mark_day((GtkCalendar*)widget, today_day);
        return true;
    } else {
        gtk_calendar_unmark_day((GtkCalendar*)widget, today_day);
        return false;
    }
}

void Calendar::goToday()
{
    gtk_calendar_select_month((GtkCalendar*)widget, today_month, today_year);
    gtk_calendar_select_day((GtkCalendar*)widget, today_day);
}

bool Calendar::runExternalViewer()
{
    Config* config = Config::getInstance();
    size_t len = config->external_viewer.length();
    if (len > 0) {
        int year, month, day;
        gtk_calendar_get_date((GtkCalendar*)widget,
                              (guint*)&year, (guint*)&month, (guint*)&day);
        struct tm date;
        date.tm_year = year - 1900;
        date.tm_mon = month;
        date.tm_mday = day;
        date.tm_sec = date.tm_min = date.tm_hour = date.tm_wday
                    = date.tm_yday = date.tm_isdst = 0;

        size_t buf_size = len + 64;
        char* cmd = new char[buf_size];
        strftime(cmd, buf_size, config->external_viewer.c_str(), &date);
        fork_and_run(cmd);
        delete[] cmd;
        return true;
    } else {
        return false;
    }
}
