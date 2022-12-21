#include "beCalendario.h"

static void testes (GtkWidget *widget, gpointer data)
{
	GDate *Data = NULL, *DataNova = NULL;
	gchar sBuffer[256];

	//preparando a data de entrada
	Data = g_date_new ();
	g_date_set_parse (Data, "19/12/1979");

	//janela para seleção
	DataNova = be_Calendario (GTK_WINDOW (data), Data);
	if (DataNova && g_date_valid (DataNova))
	{
		g_date_strftime (sBuffer, 256, "%d/%m/%Y %H:%M:%S", DataNova);
		printf ("Selecionado: %s\n", sBuffer);
		BE_date_free (DataNova);
	}
	else
		printf ("(null)\n");

	BE_date_free (Data);
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

	testes (NULL, GTK_WINDOW (window));

	gtk_main ();

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}

