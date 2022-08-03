/**
 * @file        WritePadWidget.h
 * @brief       Gtk4 widget for the WritePad handwriting recognition API
 * @author      Karim Vergnes <me@thesola.io>
 * @copyright   GNU LGPLv3
 */

#ifndef __WRITEPADWIDGET_H
#define __WRITEPADWIDGET_H

#include <gtk/gtk.h>
#include <glib/gi18n.h>

G_BEGIN_DECLS;


#define WPAD_TYPE_WIDGET WritePadWidget_get_type()
G_DECLARE_FINAL_TYPE(WritePadWidget, WritePadWidget, WPAD, WIDGET, GtkWidget);

struct _WritePadWidgetClass {
    GtkWidgetClass parent_class;
};

/**
 * Instantiate a new WritePadWidget.
 *
 * @brief       Instantiate a WritePadWidget.
 * @return      A GtkWidget pointer representing the WritePadWidget.
 */
GtkWidget *WritePadWidget_new(void);

G_END_DECLS;

#endif //__WRITEPADWIDGET_H

// vim: ft=c.doxygen
