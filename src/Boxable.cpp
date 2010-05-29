#include <gtk/gtk.h>

#include "Boxable.hpp"


void Boxable::addToBox(GtkWidget* box)
{
    if (widget) {
        gtk_box_pack_start(GTK_BOX(box), widget, false, false, 0);
    }
}
