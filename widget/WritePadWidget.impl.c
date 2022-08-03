#include "WritePadWidget.impl.h"

// WritePad, as-is, uses its own stubborn UCHR instead of the standard wchar_t
// so we map out the array into a wchar_t string which can then be fed to wcstombs()
// (The thing was never designed to run outside Windows, now was it...)
static size_t ucstowcs(const UCHR *orig, wchar_t **result)
{
    size_t count = 0;

    for (; orig[count]; count++)
        ;
    *result = calloc(count, sizeof(wchar_t));
    for (size_t i = 0; orig[i]; i++)
        (*result)[i] = orig[i];
    return count;
}


static void applyStroke    (WritePadWidget *w,
                            GdkDeviceTool *tool,
                            double x, double y, double pressure)
{
    if (gdk_device_tool_get_tool_type(tool) == GDK_DEVICE_TOOL_TYPE_ERASER) {
        //TODO: Find stroke under pen and delete it
    } else {
        cairo_set_line_width(w->cr, 4 * pressure * w->brush_size);
        cairo_set_operator(w->cr, CAIRO_OPERATOR_SATURATE);
        INK_AddPixelToStroke(w->inkData, w->nStrokes, x, y, pressure);
    }

    cairo_set_source_rgba(w->cr,
                          w->draw_color.red,
                          w->draw_color.green,
                          w->draw_color.blue,
                          w->draw_color.alpha);

    cairo_line_to(w->cr, x, y);
    cairo_stroke(w->cr);
    cairo_move_to(w->cr, x, y);

}

void _impl_WritePadWidget_onStylusDown (GtkGestureStylus *st,
                                        double x, double y,
                                        WritePadWidget *w)
{
    GdkDeviceTool *tool = gtk_gesture_stylus_get_device_tool(st);

    if (gdk_device_tool_get_tool_type(tool) == GDK_DEVICE_TOOL_TYPE_ERASER) {
        //TODO: Do we even have anything to do in that case?
    } else {
        cairo_new_path(w->cr);
        INK_AddEmptyStroke(w->inkData, 0, 0);
    }
}

void _impl_WritePadWidget_onStylusMotion   (GtkGestureStylus *st,
                                            double x, double y,
                                            WritePadWidget *w)
{
    GdkTimeCoord   *backlog;
    GdkDeviceTool  *tool;
    double          pressure;
    guint           n_items;

    tool = gtk_gesture_stylus_get_device_tool(st);

    if (gtk_gesture_stylus_get_backlog(st, &backlog, &n_items)) {
        guint i;

        for (i = 0; i < n_items; i++) {
            applyStroke(w, tool,
                        backlog[i].axes[GDK_AXIS_X],
                        backlog[i].axes[GDK_AXIS_Y],
                        backlog[i].axes[GDK_AXIS_PRESSURE]);
        }
        g_free(backlog);
    } else {
        if (!gtk_gesture_stylus_get_axis(st, GDK_AXIS_PRESSURE, &pressure))
            pressure = 1;

        applyStroke(w, tool, x, y, pressure);
    }

    gtk_widget_queue_draw(GTK_WIDGET(w));
}

void _impl_WritePadWidget_onStylusUp   (GtkGestureStylus *st,
                                        double x, double y,
                                        WritePadWidget *w)
{
    w->nStrokes += 1;

    size_t      msgl;
    wchar_t    *wresult;
    char       *u8result;
    const UCHR *result =
        HWR_RecognizeInkData(w->recognizer, w->inkData, 0,
                             w->nStrokes,
                             FALSE,  // We're not async-pickup
                             FALSE,  // No need to flip Y coordinates
                             FALSE,  //TODO: Turn LTR sort into parameter
                             FALSE); // No need to use a selection filter

    msgl     = ucstowcs(result, &wresult);
    u8result = calloc(msgl * 2, sizeof(char));
    wcstombs(u8result, wresult, msgl);

    w->message = g_string_assign(w->message, u8result);
    g_signal_emit(w, _impl_WritePadWidget_signals[WRITEPAD_RECOGNIZED], 0, w->message);

    free(wresult);
    free(u8result);
}

////////////////////////////////////////////////////////////////////////////////

void _impl_WritePadWidget_snapshot(GtkWidget *w, GtkSnapshot *snap)
{
    WritePadWidget *pad = (WritePadWidget *)w;
    GtkAllocation   alloc;
    cairo_t        *cr;

    gtk_widget_get_allocation(w, &alloc);
    cr = gtk_snapshot_append_cairo(snap,
                                  &GRAPHENE_RECT_INIT(0, 0, alloc.width, alloc.height));

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_surface(cr, pad->surface, 0, 0);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_rectangle(cr, 0, 0, alloc.width, alloc.height);
    cairo_stroke(cr);

    cairo_destroy(cr);
}


void _impl_WritePadWidget_ensure_surface(WritePadWidget *pad, int w, int h)
{
    if (!pad->surface || cairo_image_surface_get_width(pad->surface) != w
                      || cairo_image_surface_get_height(pad->surface) != h) {
        cairo_surface_t *surf;

        surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
        if (pad->surface) {
            cairo_t *cr;

            cr = cairo_create(surf);
            cairo_set_source_surface(cr, pad->surface, 0, 0);
            cairo_paint(cr);

            cairo_surface_destroy(pad->surface);
            cairo_destroy(pad->cr);
            cairo_destroy(cr);
        }

        pad->surface = surf;
        pad->cr      = cairo_create(surf);
    }
}

void _impl_WritePadWidget_size_allocate(GtkWidget *widget,
                                        int w, int h,
                                        int baseline)
{
    WritePadWidget *pad = (WritePadWidget *)widget;

    _impl_WritePadWidget_ensure_surface(pad, w, h);

    GTK_WIDGET_CLASS(_impl_WritePadWidget_parent_class)->size_allocate(widget, w, h, baseline);
}

void _impl_WritePadWidget_map(GtkWidget *widget)
{
    GtkAllocation alloc;

    GTK_WIDGET_CLASS(_impl_WritePadWidget_parent_class)->map(widget);

    gtk_widget_get_allocation(widget, &alloc);

    _impl_WritePadWidget_ensure_surface((WritePadWidget *)widget, alloc.width,
                                                                  alloc.height);
}

void _impl_WritePadWidget_unmap(GtkWidget *widget)
{
    WritePadWidget *pad = (WritePadWidget *)widget;

    HWR_FreeRecognizer(pad->recognizer, "Dictionaries/User.dct",
                                        "User.lrn",
                                        "User.cor");

    g_clear_pointer(&pad->cr, cairo_destroy);
    g_clear_pointer(&pad->surface, cairo_surface_destroy);
    g_clear_pointer(&pad->inkData, INK_FreeData);

    GTK_WIDGET_CLASS(_impl_WritePadWidget_parent_class)->unmap(widget);
    g_debug("Unmapped WritePadWidget");
}
