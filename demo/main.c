#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "../widget/WritePadWidget.h"

static void myprint(WritePadWidget *wp, GString *msg)
{
    g_print("I think you wrote '%s'\n", msg->str);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    static GtkWidget *window = NULL;

    if (!window) {
        GtkWidget *write, *headerbar;

        window = gtk_application_window_new(app);

        write = WritePadWidget_new();
        gtk_window_set_child(GTK_WINDOW(window), write);

        headerbar = gtk_header_bar_new();

        gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
        gtk_window_set_title(GTK_WINDOW(window), "WritePadWidget demo");
        g_object_add_weak_pointer(G_OBJECT(window), (gpointer *)&window);

        g_signal_connect(write, "writepad-recognized", G_CALLBACK(myprint), NULL);
    }

    if (!gtk_widget_get_visible(window))
        gtk_widget_show(window);
    else
        gtk_window_destroy(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
    int flags = 0;

    GtkApplication *app = gtk_application_new("io.thesola.WritepadDemo",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return 0;
}
