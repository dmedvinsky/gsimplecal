#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <gtk/gtk.h>

#include "config.h"
#include "MainWindow.hpp"
#include "Config.hpp"
#include "Unique.hpp"


MainWindow* main_window;


static void signal_handler(int signal_id)
{
    if (signal_id == SIGTERM) {
        gtk_main_quit();
    } else if (signal_id == SIGUSR1) {
        main_window->nextMonth();
    } else if (signal_id == SIGUSR2) {
        main_window->prevMonth();
    }
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

static void version()
{
    std::cout << PACKAGE_STRING << std::endl;
}

static void usage(const char* const name)
{
    std::cerr << "usage: " << name << " [options] [commands]\n"
        << "\noptions:\n"
        << "\t-h, --help\n\t\tprint this help text and quit\n"
        << "\t-v, --version\n\t\tprint version info and quit\n"
        << "\ncommands:\n"
        << "\tnext_month, prev_month\n"
        << "\t\tif program is running, switch the displayed month\n"
        << "\t\totherwise simply run the program\n"
        << "\tif no command is given\n"
        << "\t\tif program is not running, start it\n"
        << "\t\totherwise stop it\n"
        ;
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            usage(argv[0]);
            return 2;
        } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            version();
            return 0;
        }
    }

    Config* config = Config::getInstance();

    Unique* unique = new Unique(config->getPath());
    if (unique->isRunning()) {
        try {
            if (argc >= 2 && strcmp(argv[1], "next_month") == 0) {
                unique->signal(SIGUSR1);
            } else if (argc >= 2 && strcmp(argv[1], "prev_month") == 0) {
                unique->signal(SIGUSR2);
            } else {
                unique->kill();
                unique->stop();
            }
            return 0;
        } catch (UniqueException e) {
            std::cerr << "Looks like gsimplecal crashed last time."
                << " Exception message is: " << e.what() << ". Cleaning up."
                << std::endl;
            unique->stop();
        }
    }

    unique->start();
    signal(SIGTERM, &signal_handler);
    signal(SIGUSR1, &signal_handler);
    signal(SIGUSR2, &signal_handler);
    signal(SIGCHLD, SIG_IGN);

    if (config->force_lang.length()) {
        // Must be done before gtk_init call.
        setenv("LANG", config->force_lang.c_str(), 1);
    }

    gtk_init(&argc, &argv);
    main_window = new MainWindow();

    g_signal_connect(main_window->getWindow(), "destroy",
                     GCallback(destroy), NULL);

    if (config->show_timezones) {
        g_timeout_add(30000, (GSourceFunc)time_handler, NULL);
    }
    if (config->close_on_unfocus) {
        g_signal_connect(main_window->getWindow(), "focus-out-event",
                         GCallback(gtk_widget_destroy),
                         main_window->getWindow());
    }

    gtk_main();

    unique->stop();
    return 0;
}
