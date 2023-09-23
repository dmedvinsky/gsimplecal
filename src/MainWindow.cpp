#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "MainWindow.hpp"
#include "Config.hpp"
#include "Calendar.hpp"
#include "Timezones.hpp"
#include "config.h"


bool closeCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                   GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->close();
    }
    return true;
}
bool runExternalViewerCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                               GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->runExternalViewer();
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
bool goLeftCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                  GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->goLeft();
    }
    return true;
}
bool goDownCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                  GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->goDown();
    }
    return true;
}
bool goUpCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                  GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->goUp();
    }
    return true;
}
bool goRightCallback(GtkAccelGroup *group, GObject *obj, guint keyval,
                  GdkModifierType mod, gpointer user_data)
{
    if (user_data) {
        ((MainWindow*)user_data)->goRight();
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
#if HAVE_GTK_BOX_NEW
    children_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
#else
    children_box = gtk_vbox_new(false, 10);
#endif

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

    Shortcut keys[] = {{GDK_KEY_Escape, 0, closeCallback},
                       {GDK_KEY_Return, 0, runExternalViewerCallback},
                       {GDK_KEY_q, GDK_CONTROL_MASK, closeCallback},
                       {GDK_KEY_w, GDK_CONTROL_MASK, closeCallback},
                       {GDK_KEY_n, GDK_SHIFT_MASK, nextYearCallback},
                       {GDK_KEY_p, GDK_SHIFT_MASK, prevYearCallback},
                       {GDK_KEY_n, 0, nextMonthCallback},
                       {GDK_KEY_p, 0, prevMonthCallback},
                       {GDK_KEY_h, 0, goLeftCallback},
                       {GDK_KEY_j, 0, goDownCallback},
                       {GDK_KEY_k, 0, goUpCallback},
                       {GDK_KEY_l, 0, goRightCallback},
                       {GDK_KEY_g, 0, goTodayCallback},
                       {GDK_KEY_Home, 0, goTodayCallback}};
    for (int key = 0; key < sizeof(keys) / sizeof(Shortcut); key++) {
        closure = g_cclosure_new(G_CALLBACK(keys[key].func), (gpointer)this, NULL);
        gtk_accel_group_connect(accelerators, keys[key].key,
                                (GdkModifierType)keys[key].modifier,
                                (GtkAccelFlags)0, closure);
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
    g_signal_emit_by_name(widget, "destroy");
}

void MainWindow::runExternalViewer()
{
    calendar->runExternalViewer();
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

void MainWindow::goLeft()
{
    calendar->goLeft();
}
void MainWindow::goDown()
{
    calendar->goDown();
}
void MainWindow::goUp()
{
    calendar->goUp();
}
void MainWindow::goRight()
{
    calendar->goRight();
}
