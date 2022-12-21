#include "beDiretorio.h"

void testes (GtkWidget *widget, gpointer data)
{
	gchar *sDir = NULL;
	gchar *sTemp = NULL;

	if ((sDir = be_Diretorio (GTK_WINDOW (data), FALSE, "/home/kurumin/Sei lá")))
	//if ((sDir = be_Diretorio (GTK_WINDOW (data), FALSE, "C:/Documents and Settings/Luc/Meus documentos/Meus vídeos")))
		printf ("%s\n", sDir);
	else
		printf ("<null>\n");
	BE_free (sTemp);
	BE_free (sDir);
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

