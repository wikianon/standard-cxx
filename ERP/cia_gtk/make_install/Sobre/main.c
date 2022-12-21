#include "beSobre.h"

static void testes (GtkWidget *widget, gpointer data)
{
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *authors[] = {
		"Peter Mattis", 
		"Spencer Kimball", 
		"Josh MacDonald", 
		"e", 
		"GNU", //4
		"GTK", //5
		"MySQL", //6
		NULL
	};
	const gchar *documentors[] = {
		"Owen Taylor",
		"Tony Gale",
		"Matthias Clasen <mclasen@redhat.com>",
		"and many more...",
		NULL
	};
	
	sTemp = g_strdup_printf ("GNU C %d.%d.%d da Free Software Foundation <www.gnu.org>", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
	authors[4] = sTemp;
	sTemp1 = g_strdup_printf ("GTK+ %d.%d.%d da GIMP Toolkit <www.gtk.org>", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
	authors[5] = sTemp1;
	//MYSQL_SERVER_VERSION ou mysql_get_server_version
	//LIBXML_DOTTED_VERSION
	
	be_Sobre (GTK_WINDOW (data), "CIA", "1.0.6", "(C) 1997-2006 ProVisual", "http://www.gtk.org", "Comentarios.", NULL, (const gchar **) authors, documentors);
	
	BE_free (sTemp);
	BE_free (sTemp1);
}

int main(int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);
	
	GtkWidget *window = NULL;
	GtkWidget *button = NULL;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	button = gtk_button_new_with_label ("Testes");
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (testes), window);
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_widget_show_all (window);
	
	testes (NULL, window);
	
	gtk_main ();
	
	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
