#include <gtk/gtk.h>
#include <unique/unique.h>


static GtkWidget *main_window = NULL;


/**
 * UniqueApp message-received signal processor.
 */
static UniqueResponse message_received_cb(UniqueApp *app, UniqueCommand command,
        UniqueMessageData *message, guint time_, gpointer user_data)
{
    if (command == UNIQUE_CLOSE) {
        // Second instance sent 'close' command
        gtk_signal_emit_by_name(GTK_OBJECT(main_window), "destroy");
    }

    return UNIQUE_RESPONSE_OK;
}


/**
 * Gtk destroy signal processor.
 */
void destroy(void)
{
    gtk_main_quit();
}


void create_main_window()
{
    // init calendar widget container
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "");
    // make it undecorated,
    gtk_window_set_decorated(GTK_WINDOW(main_window), false);
    // ... topmost
    gtk_window_set_keep_above(GTK_WINDOW(main_window), true);
    // ... omnipresent
    gtk_window_stick(GTK_WINDOW(main_window));
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), true);
    // place it near mouse
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_MOUSE);

    gtk_signal_connect(GTK_OBJECT(main_window), "destroy",
            GTK_SIGNAL_FUNC(destroy), NULL);

    // create calendar widget itself
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
        // if calendar was running, close it, emulating the toggle-switch
        unique_app_send_message(app, UNIQUE_CLOSE, NULL);
    } else {
        // create a window
        create_main_window();
        unique_app_watch_window(app, GTK_WINDOW(main_window));
        // listen for 'close' message from second instance
        g_signal_connect(app, "message-received", G_CALLBACK(message_received_cb), NULL);

        gtk_main();
    }

    g_object_unref(app);
    return 0;
}

