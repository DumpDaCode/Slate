/*
* Slate is a simple paint program which. It is built using C along with GTK.
* It offers basic tools like pencil, line, curve, circle, bucket fill, eraser
* and undo to a depth of 10 while redo at a depth of 1 with respect to undo.
* This application works only on linux. It is a hobby project that I will revisit
* after some time. Any form of suggestions are welcomed.
*
* Attempted By: Rajiv Singh
* Last Modified: 28-11-2022
*/

#include "ui.h"
#include "events.h"
#include "variables.h"

struct Sheet drawing_sheet;
struct Color foreground, background;

static gboolean app_destroy(GtkWidget* widget, gpointer data){
/*
    This destroys the app.
*/
    g_application_quit(G_APPLICATION(data));
    return FALSE;
}

void app_init(GApplication *app, gpointer data){
/*
    This initializes the app.
*/
    GtkBuilder *builder;
    GtkCssProvider *css;
    GObject *window, *panel, *label1, *label2, *sheet;
    GdkPixbuf *icon;

//  Setting the global variables and resources;
    drawing_sheet.width =400, drawing_sheet.height =500;
    foreground.red = 0, foreground.green = 0, foreground.blue = 0;
    background.red = 255, background.green = 255, background.blue = 255;
    ui_register_resource();

//  Retriving the UI file and CSS file and adding remaining functionalities.
    builder = gtk_builder_new_from_resource("/slate/res/description.ui");
    css = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(css, "/slate/res/style.css");
    window = gtk_builder_get_object(builder, "window");
    panel = gtk_builder_get_object(builder, "panel");
    sheet = gtk_builder_get_object(builder, "sheet");
    label1 =gtk_builder_get_object(builder, "label1");
    label2 = gtk_builder_get_object(builder, "label2");
    icon = gdk_pixbuf_new_from_resource("/slate/res/icons/icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(window), icon);
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(label1)), "sec-head");
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(label2)), "sec-head");
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(panel)), "panel");
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_builder_connect_signals(builder, builder);

//  Events responsible for destroying the app and responding to Ctrl+w, Ctrl+z, Ctrl+y.
    g_signal_connect(window, "destroy", G_CALLBACK(app_destroy), app);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), app);

//  Adding window to application, setting visibility of UI elements and the cursor for sheet.
    gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(window));
    gtk_widget_show_all(GTK_WIDGET(window));
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_PENCIL));
}

#ifdef _WIN32
int WinMain (void * hInstance, void * hPrevInstance, char ** argv, int nCmdShow){
	GtkApplication *app = gtk_application_new("slate.src", G_APPLICATION_DEFAULT_FLAGS);
#else
int main (int argc, char ** argv){
	GtkApplication *app = gtk_application_new("slate.src", G_APPLICATION_FLAGS_NONE);
#endif
/*
    Application.
*/
    int status;
    g_signal_connect(app, "activate", G_CALLBACK(app_init), NULL);
    status = g_application_run (G_APPLICATION (app), 0, argv);
    g_object_unref (app);
    return status;
}
