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
