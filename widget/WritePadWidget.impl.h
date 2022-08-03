#ifndef __WRITEPADWIDGET_IMPL_H
#define __WRITEPADWIDGET_IMPL_H

#include "WritePadWidget.h"
#include <InkWrapper.h>
#include <RecognizerWrapper.h>

enum {
    WRITEPAD_RECOGNIZED,    // Writing was recognized into text
    WRITEPAD_OVERFLOW,      // Writing hit rightmost edge, resize or scroll
    N_SIGNALS,
};


typedef enum {
    PROP_0,

    PROP_DESIRED_WIDTH,
    PROP_DESIRED_HEIGHT,

    PROP_RECOGNIZER_LANGUAGE,

    N_PROPERTIES,
} WritePadWidgetProperty;

struct _WritePadWidget {
    GtkWidget           parent_instance;
    cairo_surface_t    *surface;
    cairo_t            *cr;
    INK_DATA_PTR       *inkData;
    RECOGNIZER_PTR     *recognizer;
    guint               nStrokes;
    GString            *message;
    GdkRGBA             draw_color;
    GtkPadController   *pad_controller;
    double              brush_size;
};

extern gpointer _impl_WritePadWidget_parent_class;

extern guint _impl_WritePadWidget_signals[N_SIGNALS];
extern GParamSpec *_impl_WritePadWidget_props[N_PROPERTIES];

void
_impl_WritePadWidget_onStylusDown  (GtkGestureStylus *st,
                                    double x, double y,
                                    WritePadWidget *w);

void
_impl_WritePadWidget_onStylusMotion(GtkGestureStylus *st,
                                    double x, double y,
                                    WritePadWidget *w);
void
_impl_WritePadWidget_onStylusUp    (GtkGestureStylus *st,
                                    double x, double y,
                                    WritePadWidget *w);

void _impl_WritePadWidget_snapshot(GtkWidget *w, GtkSnapshot *snap);
void _impl_WritePadWidget_size_allocate(GtkWidget *widget, int w, int h, int baseline);
void _impl_WritePadWidget_map(GtkWidget *widget);
void _impl_WritePadWidget_unmap(GtkWidget *widget);

#endif //__WRITEPADWIDGET_IMPL_H
