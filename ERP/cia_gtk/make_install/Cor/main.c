#include "beCor.h"

void testes (GtkWidget *widget, gpointer data)
{
	gchar *sCor = NULL;

	if ((sCor = be_Cor (GTK_WINDOW (data), "#ff03ff")))
		g_print ("%s\n", sCor);
	else
		g_print ("<null>\n");

	BE_free (sCor);
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
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (testes), GTK_WINDOW (window));
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_widget_show_all (window);

	testes (NULL, window);

	gtk_main ();

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}

