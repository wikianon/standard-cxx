#include "beLista.h"

static void testes (GtkWidget *widget, gpointer data)
{
	GSList *Lista = NULL;
	gint nPos = -1;

	//criando a lista
	Lista = g_slist_append (Lista, "Luciano Trindade Pereira");
	Lista = g_slist_append (Lista, "Pereira");
	Lista = g_slist_append (Lista, "Trindade");
	Lista = g_slist_append (Lista, "Sei lá");
	Lista = g_slist_append (Lista, "Sei la");
	Lista = g_slist_append (Lista, "Testando a lista");
	Lista = g_slist_append (Lista, "Botas1");
	Lista = g_slist_append (Lista, "Botas");
	Lista = g_slist_append (Lista, "Botas0");
	Lista = g_slist_append (Lista, "Lista padrão");
	Lista = g_slist_append (Lista, "Execução");
	Lista = g_slist_sort (Lista, be_string_ordenar_string);//ordenar

	//janela para seleção
	nPos = be_Lista (GTK_WINDOW (data), FALSE, "Testes de lista", Lista, 4);
	if (nPos >= 0)
	   printf ("Selecionado: %s\n", (gchar *) g_slist_nth_data (Lista, nPos));
	else
	   printf ("(null)\n");

	BE_slist_free (Lista);
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

