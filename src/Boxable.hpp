#ifndef BOXABLE_H
#define BOXABLE_H

#include <gtk/gtk.h>


class Boxable
{
public:
    virtual void addToBox(GtkWidget* box);

protected:
    Boxable() {};

    GtkWidget* widget;

};

#endif

