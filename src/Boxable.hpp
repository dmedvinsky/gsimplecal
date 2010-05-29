#ifndef BOXABLE_HPP
#define BOXABLE_HPP

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
