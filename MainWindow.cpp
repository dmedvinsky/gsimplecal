#include <gtk/gtk.h>

#include "MainWindow.h"
#include "Config.h"
#include "Calendar.h"


MainWindow::MainWindow()
{
    widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(widget), "gsimplecal");
    gtk_window_set_decorated(GTK_WINDOW(widget), false);
    gtk_window_set_keep_above(GTK_WINDOW(widget), true);
    // Omnipresent
    gtk_window_stick(GTK_WINDOW(widget));
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(widget), true);
    // Place it near mouse (near the clock for example)
    gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_MOUSE);

    // Create box for child items
    children_box = gtk_vbox_new(false, 10);

    Config* config = Config::getInstance();

    calendar = NULL;
    if (config->show_calendar) {
        calendar = new Calendar();
        calendar->addToBox(children_box);
    }

    gtk_container_add(GTK_CONTAINER(widget), children_box);
    gtk_widget_show(children_box);
    gtk_widget_show(widget);
}

MainWindow::~MainWindow()
{
    if (calendar) {
        delete calendar;
    }
    gtk_widget_destroy(children_box);
    gtk_widget_destroy(widget);
}

GtkWindow* MainWindow::getWindow()
{
    return GTK_WINDOW(widget);
}

