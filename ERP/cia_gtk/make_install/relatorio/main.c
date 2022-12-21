#include "berelatorio.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extendida
	be_global_abrir (&argc, &argv, 0);

	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	BEGLOBAL_Empresa_Cliente *Empresa = NULL;
	gchar *sErros = NULL, *sTemp = NULL;
	#ifdef G_OS_UNIX
		const gchar *sArquivoFonte = "/home/luciano/.cia/fontes";
	#endif
	#ifdef G_OS_WIN32
		//const gchar *sArquivoFonte = "../fontes/suporte/fontes.txt";
		const gchar *sArquivoFonte = "fontes.txt";
	#endif
	//const gchar *sArquivoSVG = "lista_padrao.svg"; const gchar *sArquivo = "lista_padrao.pdf";
	//const gchar *sArquivoSVG = "formulario_padrao.svg"; const gchar *sArquivo = "formulario_padrao.pdf";
	const gchar *sArquivoSVG = "Detalhes da Ordem.svg"; const gchar *sArquivo = "atual.pdf";
	//const gchar *sArquivoSVG = "Modelo.svg"; const gchar *sArquivo = "relatorio.pdf";
	//gint nCont = 0;

	//SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, NULL);
	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, NULL);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "vis_CEP Logradouros"), TRUE, &sErros);
	//SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, NULL);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "cli_CEP Logradouros"), TRUE, NULL);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", NULL), "cli_ordem"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "est_eng_implant", NULL), "vis_obras"), TRUE, &sErros);
	Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "est_eng_predial", NULL), "vis_obras"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "mysql", NULL), "proc"), TRUE, &sErros);

    if (Sintaxe)
	{
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 0));
		be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "5773", BESQL_RELACAO_OU, 0, FALSE);
		//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "55", BESQL_RELACAO_OU, 0, FALSE);
		//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "5699", BESQL_RELACAO_OU, 0, FALSE);

		/*for (nCont = 6; nCont < 29; nCont++)
		{
			Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, nCont)); Campo->bAtivo = FALSE;
		}*/
		/*Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 1)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 3)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 4)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 7)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 8)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 9)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 10)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 11)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 12)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 13)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 16)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 17)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 19)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 20)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 22)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 23)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 25)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 26)); Campo->bAtivo = FALSE;
		Campo = ((BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 27)); Campo->bAtivo = FALSE;*/

		//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_MENOR, "1022050", BESQL_RELACAO_OU, 0, FALSE);
		//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_MENOR, "1007000", BESQL_RELACAO_OU, 0, FALSE);
		//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "13061235", BESQL_RELACAO_OU, 0, FALSE);
		//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "28924211", BESQL_RELACAO_OU, 0, FALSE);
	}
	sTemp = be_arquivo_nome_sugerido ("Testes de relatório", 0, TRUE);
	printf ("Nome sugerido: %s\n", sTemp);

	Empresa = g_new (BEGLOBAL_Empresa_Cliente, 1);
	Empresa->sNomeFantasia = g_strdup ("Programação Profissional e Visual");
	Empresa->sRazaoSocial = g_strdup ("Programação Profissional e Visual LTDA");
	Empresa->sCNPJ = NULL;
	Empresa->sIE = NULL;
	Empresa->sTelefone = g_strdup ("19 3269-8620");
	Empresa->sFax = g_strdup ("19 3269-8620");
	Empresa->sPABX = NULL;
	Empresa->sEmail = g_strdup ("comercial@provisual.com.br");
	Empresa->sSite = NULL;
	Empresa->sCep = g_strdup ("13061-235");
	Empresa->sTipoLogradouro = g_strdup ("Rua");
	Empresa->sLogradouro = g_strdup ("Collechio");
	Empresa->sComplemento = g_strdup ("156");
	Empresa->sBairro = g_strdup ("Vila Castelo Branco");
	Empresa->sCidade = g_strdup ("Campinas");
	Empresa->sEstado = g_strdup ("SP");
	Empresa->sPais = NULL;
	//Empresa->Logotipo = gdk_pixbuf_new_from_file ("logo.png", NULL);
	Empresa->Logotipo = gdk_pixbuf_new_from_file ("logo_est_eng.jpg", NULL);

	if (be_relatorio_gerar (sArquivoSVG, sArquivo, sArquivoFonte, NULL, "Testes de relatório", Empresa, Sintaxe, &sErros))
	{
		#ifdef G_OS_UNIX
			system (g_strdup_printf ("xpdf %s", sArquivo));
			//system(g_strdup_printf ("kpdf %s", sArquivo));
			//system(g_strdup_printf ("acroread %s", sArquivo));
		#endif
		#ifdef G_OS_WIN32
			//system (g_strdup_printf ("\"C:\\Arquivos de programas\\Adobe\\Acrobat 5.0\\Reader\\AcroRd32.exe\" %s", sArquivo));
			//system (g_strdup_printf ("C:/Arquivos de programas/Adobe/Acrobat 5.0/Reader/AcroRd32.exe %s &", sArquivo));
			//printf ("%d\n", g_spawn_command_line_async (g_strdup_printf ("\"C:\\Arquivos de programas\\Adobe\\Reader 8.0\\Reader\\AcroRd32.exe\" %s", sArquivo), NULL));
			printf ("%d\n", g_spawn_command_line_async (g_strdup_printf ("\"C:\\Arquivos de Programas\\Foxit Software\\Foxit Reader\\Foxit Reader.exe\" %s", sArquivo), NULL));
		#endif
	}
	else
		printf ("Relatorio falhou\n");

	if (sErros)
		g_print ("Erros: %s\n", sErros);
	BE_free (sErros);

	//limpar
	BE_sql_fechar_consulta (Sintaxe);
	BE_sql_fechar_conexao (SQL);
	BE_empresa_cliente_limpar (Empresa);
	BE_free (sTemp);

	/*#ifdef G_OS_WIN32
		system("pause");
	#endif*/

	be_global_fechar ();
	return 0;
}
