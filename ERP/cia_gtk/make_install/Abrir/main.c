#include "beAbrir.h"

void testes (GtkWidget *widget, gpointer data)
{
	gchar *sArquivo = NULL;
	//if ((sArquivo = be_Abrir (NULL, NULL)))
	//if ((sArquivo = be_Abrir (GTK_WINDOW (data), "*.gz", "/home/kurumin/CIA-Backup.tar.gz")))

	//if ((sArquivo = be_Abrir (GTK_WINDOW (data), FALSE, "*.bmp *jpg *jpeg(Imagéns)\n*.bmp(bitmap)\n*.jpg *.jpeg(JPEG)\n*.*(Todos)", "/home/kurumin/Novo Vectraé.jpg")))
	if ((sArquivo = be_Abrir (GTK_WINDOW (data), FALSE, NULL, NULL)))
	{
		printf ("%s : %d\n", sArquivo, be_arquivo_tamanho (BE_UTF8_ISO (sArquivo)));
	}
	else
		printf ("<null>\n");
	BE_free (sArquivo);
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
