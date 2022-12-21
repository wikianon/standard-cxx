#include "beSalvar.h"

void testes (GtkWidget *widget, gpointer data)
{
	gchar *sArquivo = NULL;
	gchar *sTemp = NULL;

	//if ((sArquivo = be_Salvar (GTK_WINDOW (data), "/home/kurumin/CIA-Backup.tar.gz")))
	//if ((sArquivo = be_Salvar (GTK_WINDOW (data), sTemp = BE_ISO_UTF8 ("/home/kurumin/Teste ão.txt"))))
	if ((sArquivo = be_Salvar (GTK_WINDOW (data), FALSE, "*.bmp *jpg(Imagens éé)\n*.bmp(bitmap)\n*.jpg(JPEG)\n*.*(Todos)", "/home/kurumin/Sei lá/imagém.jpeg")))
	//if ((sArquivo = be_Salvar (GTK_WINDOW (data), FALSE, "*.bmp *jpg(Imagens éé)\n*.bmp(bitmap)\n*.jpg(JPEG)\n*.*(Todos)", "C:/Documents and Settings/Luc/Meus documentos/Meus vídeos/db1é.mdb")))
	{
		printf ("%s\n", sArquivo);
	}
	else
		printf ("<null>\n");

	BE_free (sArquivo);
	BE_free (sTemp);
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

