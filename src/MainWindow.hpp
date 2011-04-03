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

    void nextMonth();
    void prevMonth();
    void nextYear();
    void prevYear();
    void goToday();

    Calendar* calendar;

protected:
    GtkWidget* widget;
    GtkWidget* children_box;
    Timezones* timezones;

};

struct Shortcut
{
    int key;
    int modifier;
    bool (*func)(GtkAccelGroup*, GObject*, guint, GdkModifierType, void*);
};

#endif
