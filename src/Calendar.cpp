#include <gtk/gtk.h>

#include "Calendar.hpp"
#include "Config.hpp"


void monthChangedCb(GtkCalendar *calendar, gpointer cls)
{
    if (cls) {
        ((Calendar*)cls)->markToday();
    }
}


Calendar::Calendar()
{
    widget = gtk_calendar_new();
    gtk_calendar_set_display_options(GTK_CALENDAR(widget),
            (GtkCalendarDisplayOptions)(GTK_CALENDAR_SHOW_HEADING +
                                        GTK_CALENDAR_SHOW_DAY_NAMES));

    Config* config = Config::getInstance();
    if (config->mark_today) {
        // Store today date to know be able to mark it after month changes.
        gtk_calendar_get_date((GtkCalendar*)widget,
                              &today_year, &today_month, &today_day);
        markToday();
        gtk_signal_connect(GTK_OBJECT(widget), "month-changed",
                           GTK_SIGNAL_FUNC(monthChangedCb), (gpointer)this);
    }

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
