#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>

#include "Calendar.h"
#include "Timezones.h"


class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    GtkWindow* getWindow();
    void updateTime();

protected:
    GtkWidget* widget;
    GtkWidget* children_box;

    Calendar* calendar;
    Timezones* timezones;

};

#endif
