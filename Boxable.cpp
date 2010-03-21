#include <gtk/gtk.h>

#include "Boxable.h"


void Boxable::add_to_box(GtkWidget* box)
{
    if (widget) {
        gtk_box_pack_start(GTK_BOX(box), widget, false, false, 0);
    }
}

