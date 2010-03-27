#include <iostream>
#include <gtk/gtk.h>
#include <unique/unique.h>

#include "MainWindow.hpp"
#include "Config.hpp"


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


static void destroy()
{
    delete main_window;
    gtk_main_quit();
}

static bool time_handler(GtkWidget *widget)
{
    main_window->updateTime();
    return true;
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
        unique_app_watch_window(app, main_window->getWindow());
        g_signal_connect(app, "message-received",
                         G_CALLBACK(message_received_cb), NULL);
        gtk_signal_connect(GTK_OBJECT(main_window->getWindow()), "destroy",
                           GTK_SIGNAL_FUNC(destroy), NULL);

        Config* config = Config::getInstance();
        if (config->show_timezones) {
            g_timeout_add(30000, (GSourceFunc)time_handler, NULL);
        }

        gtk_main();
    }

    g_object_unref(app);
    return 0;
}

