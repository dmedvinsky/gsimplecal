#include <gtk/gtk.h>
#include <unique/unique.h>

enum
{
    COMMAND_0,
    COMMAND_CLOSE,
};


static GtkWidget *main_window = NULL;


static UniqueResponse message_received_cb(UniqueApp *app, UniqueCommand command,
        UniqueMessageData *message, guint time_, gpointer user_data)
{
    if (command == UNIQUE_CLOSE) {
        gtk_signal_emit_by_name(GTK_OBJECT(main_window), "destroy");
    }

    return UNIQUE_RESPONSE_OK;
}

void destroy(void)
{
    gtk_main_quit();
}

void create_main_window()
{
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "");
    gtk_window_set_decorated(GTK_WINDOW(main_window), false);
    gtk_window_set_keep_above(GTK_WINDOW(main_window), true);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), true);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_MOUSE);
    gtk_signal_connect(GTK_OBJECT(main_window), "destroy",
              GTK_SIGNAL_FUNC(destroy), NULL);

    GtkWidget *calendar;
    calendar = gtk_calendar_new();
    gtk_calendar_set_display_options(GTK_CALENDAR(calendar),
            (GtkCalendarDisplayOptions)(GTK_CALENDAR_SHOW_HEADING + GTK_CALENDAR_SHOW_DAY_NAMES));

    gtk_container_add(GTK_CONTAINER(main_window), calendar);
    gtk_widget_show(calendar);

    gtk_widget_show(main_window);
}


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    UniqueApp *app;
    app = unique_app_new("org.dmedvinsky.gsimplecal", NULL);

    if (unique_app_is_running(app)) {
        unique_app_send_message(app, UNIQUE_CLOSE, NULL);
    } else {
        create_main_window();
        unique_app_watch_window(app, GTK_WINDOW(main_window));
        g_signal_connect(app, "message-received", G_CALLBACK(message_received_cb), NULL);

        gtk_main();
    }

    g_object_unref(app);
    return 0;
}

