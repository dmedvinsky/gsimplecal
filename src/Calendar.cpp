#include <gtk/gtk.h>

#include "Calendar.hpp"


Calendar::Calendar()
{
    widget = gtk_calendar_new();
    gtk_calendar_set_display_options(GTK_CALENDAR(widget),
            (GtkCalendarDisplayOptions)(GTK_CALENDAR_SHOW_HEADING +
                                        GTK_CALENDAR_SHOW_DAY_NAMES));
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
