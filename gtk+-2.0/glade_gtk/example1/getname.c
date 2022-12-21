#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
// gcc -o getname getname.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0)
GtkBuilder *builder;
GtkWidget  *window1;

G_MODULE_EXPORT void on_window1_destroy (GtkObject *object, gpointer user_data)
{
    gtk_main_quit();
}

G_MODULE_EXPORT void on_button2_clicked (GtkObject *object, gpointer user_data)
{
    gtk_main_quit();
}

G_MODULE_EXPORT void on_button1_clicked (GtkObject *object, gpointer user_data)
{
    const gchar *name;
    GtkWidget *name_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    g_print("Name is: %s\n", name);
}

int main(int argc, char** argv)
{
    GError     *error = NULL;

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.*/
    if( ! gtk_builder_add_from_file( builder, "simple.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return 1;
    }

    /* Get main window pointer from UI */
    window1 = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );

    /* Connect signals */
    gtk_builder_connect_signals( builder, NULL );

    /* Destroy builder, since we don't need it anymore */
    //g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window1 );

    /* Start main loop */
    gtk_main();

    return 0;
}