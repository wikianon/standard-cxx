#include "besql.h"

gboolean executar_callback (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;

	//loop nas campos e valores
	for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		//exibir
		printf ("%d %s = %s (%d %d) [%d]\n", nLinha, Campo->sNome, Campo->sValor, Campo->nComprimento, Campo->nComprimentoMax, Campo->nTipo);
	}

	return TRUE;//FALSE para parar o loop.
}

gboolean executar_callback_salvar (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data)
{
	BESQL_Campo *Campo = NULL;
	FILE *File = NULL;
	gchar *sTemp = NULL;

	if (Campos)
	{
		Campo = (BESQL_Campo *) g_slist_nth_data (Campos, 0);
		if (Campo->sValor[1])
		{
			if (Campo->sValor[1] == '1' || Campo->sValor[1] == '2' || Campo->sValor[1] == '3' || Campo->sValor[1] == '4')
				sTemp = g_strdup_printf ("indice/%c/a/%s.txt", Campo->sValor[0], Campo->sValor);
			else
				sTemp = g_strdup_printf ("indice/%c/%c/%s.txt", Campo->sValor[0], Campo->sValor[1], Campo->sValor);
		}
		else
			sTemp = g_strdup_printf ("indice/%c/%s.txt", Campo->sValor[0], Campo->sValor);
		//iniciar arquivo
		if ((File = fopen (sTemp, "wb")))
		{
			Campo = (BESQL_Campo *) g_slist_nth_data (Campos, 1);
			fprintf (File, "%s", Campo->sValor);
			//fechar arquivo
			fclose (File);
		}
	}
	//limpar
	BE_free (sTemp);

	return TRUE;//FALSE para parar o loop.
}

void testes1 ()
{
	BESQL *SQL = NULL;
	BESQL_BD *BD = NULL;
	BESQL_Sintaxe *SintaxeLog = NULL;
	BESQL_Sintaxe *SintaxePerfil = NULL;
	BESQL_Sintaxe *SintaxeEmpresa = NULL;
	BESQL_Sintaxe *SintaxeUsuario = NULL;
	BESQL_Sintaxe *SintaxeMenu = NULL;
	gchar *sErros = NULL;

	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);
	//selecionando BD pelo número
	//BD = (BESQL_BD *) g_slist_nth_data (SQL->BDs, 2);
	//be_sql_selecionar_bd (SQL, BD->sNome, &sErros);
	//selecionando BD pelo nome
	BD = be_sql_selecionar_bd (SQL, "est_eng_implant", &sErros);

	be_sql_executar (SQL, "SELECT AMMOE('1234567.890');", executar_callback, NULL, &sErros);
	SintaxeLog = be_sql_abrir_consulta (be_sql_selecionar_tabela (BD, "sis_log"), TRUE, &sErros);
	SintaxePerfil = be_sql_abrir_consulta (be_sql_selecionar_tabela (BD, "sis_configuracoes"), TRUE, &sErros);
	SintaxeEmpresa = be_sql_abrir_consulta (be_sql_selecionar_tabela (BD, "sis_empresa"), TRUE, &sErros);
	SintaxeUsuario = be_sql_abrir_consulta (be_sql_selecionar_tabela (BD, "sis_info_usuario"), TRUE, &sErros);
	SintaxeMenu = be_sql_abrir_consulta (be_sql_selecionar_tabela (BD, "sis_menu"), TRUE, &sErros);

	BE_sql_fechar_consulta (SintaxeMenu);
	BE_sql_fechar_consulta (SintaxeUsuario);
	BE_sql_fechar_consulta (SintaxeEmpresa);
	BE_sql_fechar_consulta (SintaxePerfil);
	BE_sql_fechar_consulta (SintaxeLog);
	BE_sql_fechar_conexao (SQL);

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);
}

void testes2 ()
{
	BESQL *SQL = NULL;
	BESQL_BD *BD = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	gchar *sErros = NULL;

	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);
	BD = be_sql_selecionar_bd (SQL, "est_eng_implant", &sErros);
	Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (BD, "menu"), TRUE, &sErros);
	printf ("Adicionado: %d\n", (gint) be_sql_adicionar (Sintaxe, NULL));

	BE_sql_fechar_consulta (Sintaxe);
	BE_sql_fechar_conexao (SQL);

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);
}

void testes3 ()
{
	BESQL *SQL = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;

	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);
	be_sql_executar (SQL, "SELECT Palavra, Texto FROM dic.dic WHERE Sentido = 'br|en' ORDER BY Palavra;", executar_callback_salvar, NULL, &sErros);
	BE_sql_fechar_conexao (SQL);

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);
	BE_free (sTemp);
}

int main (int argc, char *argv[])
{
	//inicializar biblioteca estendida
	be_global_abrir (&argc, &argv, 0);

	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL, *SintaxeAlterar = NULL, *SintaxeAdicionar = NULL;
	BESQL_Campo *Campo = NULL;
	//BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL;
	//GSList *Lista = NULL, *Lista1 = NULL;
	gchar *sErros = NULL;
	glong lLinha = 0;
	gchar *sTemp = NULL;

	//testes
	//testes1 ();
	//testes2 ();
	//testes3 ();

	//SQL = be_sql_abrir_conexao_segura ("suportetecnico", "", NULL, 0, -1, NULL, "client-key.pem", "client-cert.pem", "ca-cert.pem", NULL, NULL);
	//SQL = BE_sql_abrir_conexao ("demo", "", NULL, 0, -1, NULL);
	//SQL = BE_sql_abrir_conexao ("demo", "", NULL, 0, -1, &sErros);

	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);

	//execução direta
	be_sql_executar (SQL, "SHOW PROCESSLIST", executar_callback, NULL, &sErros);
	//be_sql_executar (SQL, "SET @A=15;SELECT @A", executar_callback, NULL, &sErros);
	//be_sql_executar (SQL, "SELECT @A", executar_callback, NULL, &sErros);
	//SQL = NULL;
	//be_sql_executar (SQL, "SELECT * FROM vis_insumos;", executar_callback, NULL, &sErros);
	be_sql_executar (SQL, "SHOW STATUS", executar_callback, NULL, &sErros);
	//if (SQL)
        //be_sql_executar (SQL, "SET GLOBAL query_cache_size = 40000", executar_callback, NULL, &sErros);
    //    printf ("%s\n", sTemp = be_sql_informacoes (SQL, BESQL_INFO_SEGURO, NULL));
	//be_sql_executar (SQL, "SELECT TIMESTAMPDIFF(SECOND, '2006-08-15 14:16:50', NOW())/60", executar_callback, NULL, &sErros);
	//printf ("Afetadas: %d\n", be_sql_executar (SQL, "UPDATE cia_demo.cli_sincronizar SET BA=7 WHERE ID=1;", NULL, NULL, &sErros));
	//printf ("%s\n", sTemp = be_sql_informacoes (SQL, BESQL_INFO_USUARIO, &sErros));

	//printf ("Compilação: %s\n", sTemp = be_sql_informacoes (SQL, BESQL_INFO_COMPILACAO, &sErros));

	if (SQL)
	{
		//printf ("Alterar senha: %d\n", be_sql_alterar_senha (SQL, "", NULL));

		//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "cli_cep_logradouros"), TRUE, NULL);
		//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "vis_cep_logradouros"), TRUE, NULL);
		//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "cli_ordem"), TRUE, NULL);
		//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "suporte", NULL), "erros"), TRUE, NULL);
		//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", NULL), "vis_insumos"), TRUE, NULL);
		Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "est_eng_predial", &sErros), "vis_insumos"), TRUE, &sErros);

		//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "sis_configuracoes"), TRUE, NULL);
		if (Sintaxe)
		{
			Campo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 2);
			//Campo->bAtivo = FALSE;
			//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "3", BESQL_RELACAO_OU, 0, FALSE);
			//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, NULL, BESQL_RELACAO_OU, 0, FALSE);
			//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "13061235", BESQL_RELACAO_OU, 0, FALSE);
			/*Campo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 3);
			be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "81", BESQL_RELACAO_E, 18, FALSE);
			Campo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 4);
			be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "79", BESQL_RELACAO_OU, 18, FALSE);
			Campo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 1);
			be_sql_ordem_adicionar (Sintaxe, Campo, BESQL_ORDENACAO_DESC);*/

			be_sql_contar_linhas (Sintaxe, NULL);
			be_sql_comprimento_linhas (Sintaxe, NULL);
			be_sql_totalizar_linhas (Sintaxe, NULL);
			printf ("Selecionado %d de %d\n\n", (gint) be_sql_selecionar (Sintaxe, 1, NULL), (gint) Sintaxe->lLinhas);
			lLinha = be_sql_selecionar_proxima_linha (Sintaxe);
			//while ((lLinha = be_sql_selecionar_proxima_linha (Sintaxe)))
			{
				printf ("Linha: %d\n", (gint) lLinha);
				for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Campo = (BESQL_Campo *) ListaL->data;
					printf ("%s = %s (%d) [%d] %d", Campo->sNome, Campo->sValor, Campo->nTipo, Campo->nComprimento, Campo->nComprimentoMax);
					printf ("  --%s", sTemp = be_sql_campo_valor_limitado (Campo, 20));
					BE_free (sTemp);
					printf ("\n");
				}
				printf ("\n");
			}
			//excluir
			//printf ("Excluído: %d\n", (gint) be_sql_excluir_CIA (Sintaxe, "1001000", NULL));
			//printf ("Excluído: %d\n", (gint) be_sql_excluir_CIA (Sintaxe, "1002000", NULL));

			//alterar
			/*Campo = be_sql_selecionar_campo (Sintaxe, "Conteudo");
			printf ("Inserir: %d\n", be_sql_inserir_arquivo (Campo, "erro.txt", 'B'));
			Campo->bAfetar = TRUE;
			printf ("Alterado: %d\n", (gint) be_sql_alterar (Sintaxe, NULL));

			be_sql_selecionar (Sintaxe, 1, NULL);
			be_sql_selecionar_proxima_linha (Sintaxe);*/
			Campo = be_sql_selecionar_campo (Sintaxe, "Conteudo");
			printf ("Salvar: %d\n", be_sql_recuperar_arquivo (Campo, "erro1.txt", 'B'));

			//alterar
			/*Campo = be_sql_selecionar_campo (Sintaxe, "Mão de Obra");
			Campo->sValorNovo = g_strdup ("R$ 52,01");
			Campo->bAfetar = TRUE;
			Campo = be_sql_selecionar_campo (Sintaxe, "Data Abertura");
			Campo->sValorNovo = g_strdup ("1");
			Campo->bAfetar = TRUE;
			Campo = be_sql_selecionar_campo (Sintaxe, "Endereço");
			Campo->sValorNovo = g_strdup ("Testes de caçador, é mais feliz.' AND ''");
			Campo->bAfetar = TRUE;
			//printf ("Alterado: %d\n", (gint) be_sql_alterar (Sintaxe, NULL));*/

			/*//alterar pelo índice
			Campo = be_sql_selecionar_campo (Sintaxe, "Material");
			if (Campo)
			{
				Campo->sValorNovo = g_strdup ("80.8");
				Campo->bAfetar = TRUE;

				Indice = be_sql_selecionar_indice (Sintaxe, "PRIMARY", NULL);
				if (Indice)
				{
					Indice->sValorFiltro = g_strdup ("1");
					Indice->bAfetar = TRUE;

					printf ("Alterado: %d\n", (gint) be_sql_alterar_indice (Sintaxe, FALSE, NULL));
				}
			}*/

			//adicionar
			/*Campo = be_sql_selecionar_campo (Sintaxe, "Ordem");
			if (Campo)
			{
				Campo->sValorNovo = g_strdup ("5100000");
				Campo->bAfetar = TRUE;
			}
			Campo = be_sql_selecionar_campo (Sintaxe, "Material");
			if (Campo)
			{
				Campo->sValorNovo = g_strdup ("R$ 51.000,51");
				Campo->bAfetar = TRUE;
			}
			printf ("Adicionado: %d\n", (gint) be_sql_adicionar (Sintaxe, TRUE, NULL));*/

			/*//excluir por índice
			Indice = be_sql_selecionar_indice (Sintaxe, "PRIMARY", NULL);
			if (Indice)
			{
				Indice->sValorFiltro = g_strdup ("8221");
				Indice->bAfetar = TRUE;

				printf ("Excluido: %d\n", (gint) be_sql_excluir_indice (Sintaxe, NULL));
			}*/

		}
		/*
		//alterar
		//SintaxeAlterar = be_sql_abrir_consulta (Sintaxe->alt_Tabela, TRUE, NULL);
		SintaxeAlterar = Sintaxe;
		if (SintaxeAlterar)
		{
			Campo = be_sql_selecionar_campo (SintaxeAlterar, "Material");
			Campo->sValorNovo = g_strdup ("R$ 80,05");
			Campo->bAfetar = TRUE;
			Campo = be_sql_selecionar_campo (SintaxeAlterar, "Data Abertura");
			Campo->sValorNovo = g_strdup ("21/07/2006");
			Campo->bAfetar = TRUE;
			printf ("Alterado: %d\n", (gint) be_sql_alterar_CIA (SintaxeAlterar, "2", TRUE, NULL));
			//usuário bloqueando um ID
			//printf ("%d\n", be_sql_usuario_bloqueando_CIA (SintaxeAlterar, "10", NULL));
		}
		//adicionar
		//SintaxeAdicionar = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), Sintaxe->adi_Tabela->sNome), TRUE, NULL);
		if (SintaxeAdicionar)
		{
			Campo = be_sql_selecionar_campo (SintaxeAdicionar, "ID Bairro");
			Campo->sValorNovo = g_strdup ("3");
			Campo->bAfetar = TRUE;
			//printf ("Adicionado: %d\n", (gint) be_sql_adicionar_CIA (SintaxeAdicionar, TRUE, NULL));
		}*/

		//fechar
		BE_sql_fechar_consulta (Sintaxe);
		BE_sql_fechar_consulta (SintaxeAlterar);
		BE_sql_fechar_consulta (SintaxeAdicionar);
	}
	BE_sql_fechar_conexao (SQL);

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);

	//finalizar biblioteca extendida
	be_global_fechar ();
	return 0;
}
