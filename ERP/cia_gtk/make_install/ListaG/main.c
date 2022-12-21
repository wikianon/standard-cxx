#include "beListaG.h"

static void testes (GtkWidget *widget, gpointer data)
{
	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	gchar *sErros = NULL;

	//SQL = BE_sql_abrir_conexao ("demo", "", NULL, 0, -1, &sErros);
	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "vis_CEP Logradouros"), TRUE, &sErros);
	Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "est_eng_implant", &sErros), "vis_estoque"), TRUE, &sErros);
	//SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, NULL);
	//Sintaxe = be_sql_abrir_consulta (_be_sql_selecionar_tabela (_be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "cli_CEP Logradouros"), TRUE, NULL);
	if (Sintaxe)
	{
		//Campo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 2);
		//Campo->bAtivo = FALSE;
		//be_ListaG (GTK_WINDOW (data), Sintaxe, "collechio");
		if (be_ListaG (GTK_WINDOW (data), Sintaxe, NULL))
		{
			Campo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 0);
			printf ("%s = %s\n", Campo->sNome, Campo->sValor);
		}
	}
	BE_sql_fechar_consulta (Sintaxe);
	BE_sql_fechar_conexao (SQL);

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);
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

