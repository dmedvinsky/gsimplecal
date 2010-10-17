#include <iostream>
#include <string.h>

#include <signal.h>
#include <gtk/gtk.h>

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

int main(int argc, char *argv[])
{
    Unique* unique = new Unique();
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

    gtk_init(&argc, &argv);
    main_window = new MainWindow();

    gtk_signal_connect(GTK_OBJECT(main_window->getWindow()), "destroy",
                       GTK_SIGNAL_FUNC(destroy), NULL);

    Config* config = Config::getInstance();
    if (config->show_timezones) {
        g_timeout_add(30000, (GSourceFunc)time_handler, NULL);
    }

    gtk_main();

    unique->stop();
    return 0;
}
