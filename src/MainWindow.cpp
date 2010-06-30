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

    int keys[3][2] = {{GDK_Escape, 0},
                      {GDK_q, GDK_CONTROL_MASK},
                      {GDK_w, GDK_CONTROL_MASK}};
    for (int key = 0; key < 3; key++) {
        closure = g_cclosure_new(G_CALLBACK(closeCallback), (gpointer)this, NULL);
        gtk_accel_group_connect(accelerators, keys[key][0],
                                (GdkModifierType)keys[key][1],
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
