#include <iostream>
#include <gtk/gtk.h>
#include <unique/unique.h>

#include "MainWindow.h"


MainWindow* main_window;


/**
 * UniqueApp message-received signal processor.
 */
static UniqueResponse message_received_cb(UniqueApp *app, UniqueCommand command,
        UniqueMessageData *message, guint time_, gpointer user_data)
{
    if (command == UNIQUE_CLOSE) {
        gtk_main_quit();
    }

    return UNIQUE_RESPONSE_OK;
}


void destroy()
{
    delete main_window;
    gtk_main_quit();
}


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    UniqueApp *app;
    app = unique_app_new("org.dmedvinsky.gsimplecal", NULL);

    if (unique_app_is_running(app)) {
        // if calendar was running, close it, emulating the toggle-switch
        unique_app_send_message(app, UNIQUE_CLOSE, NULL);
    } else {
        main_window = new MainWindow();

        // connect our window to unique-app messages
        unique_app_watch_window(app, main_window->get_window());
        g_signal_connect(app, "message-received",
                         G_CALLBACK(message_received_cb), NULL);
        gtk_signal_connect(GTK_OBJECT(main_window->get_window()), "destroy",
                           GTK_SIGNAL_FUNC(destroy), NULL);

        gtk_main();
    }

    g_object_unref(app);
    return 0;
}

