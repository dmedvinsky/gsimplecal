#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>

#include "Calendar.h"


class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    GtkWindow* getWindow();

protected:
    GtkWidget* widget;
    GtkWidget* children_box;

    Calendar* calendar;

};

#endif
