#include "beFiltro.h"

static void testes (GtkWidget *widget, gpointer data)
{
	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL, *SintaxePerfil = NULL;
	BESQL_Campo *Campo = NULL;

	SQL = BE_sql_abrir_conexao ("demo", "", NULL, 0, -1, NULL);
	if (SQL)
	{
		Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", NULL), "vis_insumos"), TRUE, NULL);
		SintaxePerfil = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", NULL), "sis_configuracoes"), TRUE, NULL);
		if (Sintaxe)
		{
			//filtrando
			Campo = be_sql_selecionar_campo (Sintaxe, "Ordem");
			if (Campo)
			{
				be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "13061235", BESQL_RELACAO_OU, 0, FALSE);
				be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "28924235", BESQL_RELACAO_OU, 1, FALSE);
				be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "16", BESQL_RELACAO_OU, 0, FALSE);
				//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "17", BESQL_RELACAO_OU, 0, FALSE);
			}

			//janela de filtro
			if (be_Filtro (GTK_WINDOW (data), Sintaxe, SintaxePerfil))
			{
				printf ("OK\n");
			}
			printf ("Filtrados: %d\n", g_slist_length (Sintaxe->Filtros));
		}
	}
	//fechar
	BE_sql_fechar_consulta (Sintaxe);
	BE_sql_fechar_consulta (SintaxePerfil);
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
