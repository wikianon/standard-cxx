#include "beOrdem.h"

static void testes (GtkWidget *widget, gpointer data)
{
	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;

	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, NULL);
	if (SQL)
	{
		Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", NULL), "cli_insumos"), TRUE, NULL);
		if (Sintaxe)
		{
			//ordenar
			Campo = be_sql_selecionar_campo (Sintaxe, "Nome");
			if (Campo)
			{
				be_sql_ordem_adicionar (Sintaxe, Campo, BESQL_ORDENACAO_DESC);
			}
			//janela de ordem
			if (be_Ordem (GTK_WINDOW (data), Sintaxe))
			{
				printf ("OK\n");
			}
			printf ("Ordenados: %d\n", g_slist_length (Sintaxe->Ordens));
		}
	}
	//fechar
	BE_sql_fechar_consulta (Sintaxe);
	BE_sql_fechar_conexao (SQL);
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

