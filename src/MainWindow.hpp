#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <gtk/gtk.h>

#include "Calendar.hpp"
#include "Timezones.hpp"


class MainWindow
{
public:
    MainWindow();
    ~MainWindow();

    GtkWindow* getWindow();
    void updateTime();

    void close();

protected:
    GtkWidget* widget;
    GtkWidget* children_box;

    Calendar* calendar;
    Timezones* timezones;

};

#endif
