#include "WritePadWidget.impl.h"
#include "WPDictionaries.gen.h"

G_DEFINE_TYPE_WITH_CODE(WritePadWidget, WritePadWidget, GTK_TYPE_WIDGET, NULL);


guint _impl_WritePadWidget_signals[N_SIGNALS] = {
    0, 1
};

GParamSpec *_impl_WritePadWidget_props[N_PROPERTIES] = { NULL, };

gpointer _impl_WritePadWidget_parent_class;


static char *_fileInHome (const char *filename)
{
    const char *cdir = g_get_user_cache_dir();
    char *result = calloc(strlen(cdir) + strlen(filename), sizeof(char));

    strcpy(result, cdir);
    strcat(result, filename);

    return result;
}

void WritePadWidget_class_init(WritePadWidgetClass *cls)
{
    GObjectClass *ocls = G_OBJECT_CLASS(cls);
    GtkWidgetClass *wcls = GTK_WIDGET_CLASS(cls);

    wcls->snapshot      = _impl_WritePadWidget_snapshot;
    wcls->map           = _impl_WritePadWidget_map;
    wcls->unmap         = _impl_WritePadWidget_unmap;
    wcls->size_allocate = _impl_WritePadWidget_size_allocate;

    ocls->set_property  = _impl_WritePadWidget_set_property;
    ocls->get_property  = _impl_WritePadWidget_get_property;

    _impl_WritePadWidget_signals[WRITEPAD_RECOGNIZED] =
        g_signal_new("writepad-recognized", G_TYPE_FROM_CLASS(wcls),
            G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL,
            G_TYPE_NONE,
            1, G_TYPE_STRING);
}

void WritePadWidget_init(WritePadWidget *pad)
{
    GtkGesture *gesture;

    g_debug("English dict at %s\n", WP_DICT_DEFAULTS[LANGUAGE_ENGLISH]);
    g_debug("User dict at %s\n", _fileInHome("/WritePad/User.dct"));

    int flags = 0;
    //TODO: Set recognizer language as a global parameter
    //TODO: Map user files to global cache dir
    pad->recognizer = HWR_InitRecognizer("Dictionaries/English.dct",
                                         "User.dct",
                                         "User.lrn", "User.cor",
                                         LANGUAGE_ENGLISH, &flags);
    HWR_SetRecognitionFlags(pad->recognizer,
                            FLAG_USERDICT &
                            FLAG_MAINDICT &
                            FLAG_ANALYZER &
                            FLAG_CORRECTOR);

    _impl_WritePadWidget_parent_class = WritePadWidget_parent_class;
    pad->nStrokes = 0;
    pad->inkData = INK_InitData();
    pad->message = g_string_new("");
    INK_Erase(pad->inkData);

    gesture = gtk_gesture_stylus_new();
    g_signal_connect(gesture, "down",   G_CALLBACK(_impl_WritePadWidget_onStylusDown),   pad);
    g_signal_connect(gesture, "motion", G_CALLBACK(_impl_WritePadWidget_onStylusMotion), pad);
    g_signal_connect(gesture, "up",     G_CALLBACK(_impl_WritePadWidget_onStylusUp),     pad);
    gtk_widget_add_controller(GTK_WIDGET(pad), GTK_EVENT_CONTROLLER(gesture));

    g_object_set_data((GObject *) pad, "draw_color", &pad->draw_color);

    pad->draw_color = (GdkRGBA){0, 0, 0, 1};
    pad->brush_size = 1;
}

GtkWidget *WritePadWidget_new(void) { return g_object_new(WPAD_TYPE_WIDGET, NULL); }
