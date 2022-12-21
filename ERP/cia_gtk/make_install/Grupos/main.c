#include "beGrupos.h"

static void testes (GtkWidget *widget, gpointer data)
{
	GSList *pLista1 = NULL, *pLista2 = NULL, *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//criando a lista
	pLista1 = g_slist_append (pLista1, "Luciano Trindade Pereira");
	pLista1 = g_slist_append (pLista1, "Pereira");
	pLista1 = g_slist_append (pLista1, "Trindade");
	pLista1 = g_slist_append (pLista1, sTemp = BE_ISO_UTF8 ("Sei lá"));//BE_free deve ser usdo depois
	pLista1 = g_slist_append (pLista1, "Sei la");
	pLista1 = g_slist_append (pLista1, "Testando a lista");
	pLista1 = g_slist_append (pLista1, "Botas1");
	pLista1 = g_slist_append (pLista1, "Botas");
	pLista1 = g_slist_append (pLista1, "Botas0");
	pLista1 = g_slist_append (pLista1, "Lista padrao");
	pLista1 = g_slist_append (pLista1, sTemp1 = BE_ISO_UTF8 ("Execução"));//BE_free deve ser usdo depois
	pLista1 = g_slist_sort (pLista1, be_string_ordenar_string);//ordenar

	//janela para seleção
	if (be_Grupos (GTK_WINDOW (data), &pLista1, &pLista2))
	{
		for (ListaL = pLista2; ListaL; ListaL = g_slist_next (ListaL))
			g_print ("Selecionado: %s\n", (gchar *) ListaL->data);

		for (ListaL = pLista1; ListaL; ListaL = g_slist_next (ListaL))
			g_print ("Disponivel: %s\n", (gchar *) ListaL->data);
	}
	else
		g_print ("(null)\n");

	BE_slist_free (pLista1);
	BE_slist_free (pLista2);
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
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (testes), GTK_WINDOW (window));
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_widget_show_all (window);

	testes (NULL, GTK_WINDOW (window));

	gtk_main ();

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}

