#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "MainWindow.hpp"
#include "Config.hpp"
#include "Calendar.hpp"
#include "Timezones.hpp"


bool closeCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                   GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->close();
    }
    return true;
}
bool nextYearCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                      GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->nextYear();
    }
    return true;
}
bool prevYearCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                      GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->prevYear();
    }
    return true;
}
bool nextMonthCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                       GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->nextMonth();
    }
    return true;
}
bool prevMonthCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                       GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->prevMonth();
    }
    return true;
}
bool goTodayCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                     GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->goToday();
    }
    return true;
}


MainWindow::MainWindow()
{
    Config* config = Config::getInstance();
    gint xpos, ypos;
    widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(widget), "gsimplecal");
    gtk_window_set_decorated(GTK_WINDOW(widget), config->mainwindow_decorated);
    gtk_window_set_position(GTK_WINDOW(widget), config->mainwindow_position);
    gtk_window_get_position(GTK_WINDOW(widget), &xpos, &ypos);
    gtk_window_move(GTK_WINDOW(widget),  config->mainwindow_xoffset + xpos,  config->mainwindow_yoffset + ypos);
    gtk_window_set_resizable(GTK_WINDOW(widget), config->mainwindow_resizable);
    gtk_window_set_keep_above(GTK_WINDOW(widget),
                              config->mainwindow_keep_above);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(widget),
                                     config->mainwindow_skip_taskbar);
    if (config->mainwindow_sticky) {
        gtk_window_stick(GTK_WINDOW(widget));
    }

    // Create box for child items
    children_box = gtk_vbox_new(false, 10);

    calendar = NULL;
    if (config->show_calendar) {
        calendar = new Calendar();
        calendar->addToBox(children_box);
    }
    timezones = NULL;
    if (config->show_timezones) {
        timezones = new Timezones();
        timezones->updateTime();
        timezones->addToBox(children_box);
    }

    gtk_container_add(GTK_CONTAINER(widget), children_box);
    gtk_widget_show(children_box);
    gtk_widget_show(widget);

    // Connect keyboard accelerators
    GtkAccelGroup *accelerators = gtk_accel_group_new();
    GClosure *closure;

    Shortcut keys[] = {{GDK_Escape, 0, closeCallback},
                       {GDK_q, GDK_CONTROL_MASK, closeCallback},
                       {GDK_w, GDK_CONTROL_MASK, closeCallback},
                       {GDK_j, GDK_SHIFT_MASK, nextYearCallback},
                       {GDK_k, GDK_SHIFT_MASK, prevYearCallback},
                       {GDK_j, 0, nextMonthCallback},
                       {GDK_k, 0, prevMonthCallback},
                       {GDK_g, 0, goTodayCallback},
                       {GDK_Home, 0, goTodayCallback}};
    for (int key = 0; key < 9; key++) {
        closure = g_cclosure_new(G_CALLBACK(keys[key].func), (gpointer)this, NULL);
        gtk_accel_group_connect(accelerators, keys[key].key,
                                (GdkModifierType)keys[key].modifier,
                                (GtkAccelFlags)NULL, closure);
        g_closure_unref(closure);
    }

    gtk_window_add_accel_group(GTK_WINDOW(widget), accelerators);
}

MainWindow::~MainWindow()
{
    if (calendar) {
        delete calendar;
    }
    if (timezones) {
        delete timezones;
    }
    gtk_widget_destroy(children_box);
    gtk_widget_destroy(widget);
}

GtkWindow* MainWindow::getWindow()
{
    return GTK_WINDOW(widget);
}

void MainWindow::updateTime()
{
    if (timezones) {
        timezones->updateTime();
    }
}

void MainWindow::close()
{
    gtk_signal_emit_by_name(GTK_OBJECT(widget), "destroy");
}

void MainWindow::goToday()
{
    calendar->goToday();
}
void MainWindow::nextMonth()
{
    calendar->nextMonth();
}
void MainWindow::prevMonth()
{
    calendar->prevMonth();
}

void MainWindow::nextYear()
{
    calendar->nextYear();
}
void MainWindow::prevYear()
{
    calendar->prevYear();
}
