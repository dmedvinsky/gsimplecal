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
    if (config->mark_today) {
        // Store today date to be able to mark it after month changes.
        gtk_calendar_get_date((GtkCalendar*)widget,
                              &today_year, &today_month, &today_day);
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
    _change(1, 0);
}
void Calendar::prevYear()
{
    _change(-1, 0);
}

void Calendar::nextMonth()
{
    _change(0, 1);
}
void Calendar::prevMonth()
{
    _change(0, -1);
}

void Calendar::_change(int year_offset, int month_offset)
{
    int year, month;
    gtk_calendar_get_date((GtkCalendar*)widget, (guint*)&year, (guint*)&month, NULL);
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
