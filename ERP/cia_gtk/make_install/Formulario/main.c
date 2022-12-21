#include "beFormulario.h"
#include "../Tabela/beTabela.h"

GtkWidget *func_Subtabela (
	BESQL_Sintaxe *Sintaxe,
	BESQL_Sintaxe *SintaxeLog,
	BESQL_Sintaxe *SintaxePerfil,
	GtkWidget *window,
	BESQL_ACESSO_ENUM nFlags,
	gboolean bOpcoes,
	gboolean bLog,
	gboolean bAtualizar,
	gpointer user_data)
{
	return be_Tabela (Sintaxe, SintaxeLog, SintaxePerfil, window, nFlags, bOpcoes, NULL, NULL, NULL, NULL, NULL, bLog, bAtualizar, NULL, NULL, NULL);

	return NULL;
}

void func_Executavel (const gchar *sComando, const gchar *sParametros, gpointer user_data)
{
	printf ("Linha e comando: %s %s\n", sComando, sParametros);
}

int main (int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);

	GtkWidget *window = NULL;
	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL, *SintaxeLog = NULL, *SintaxePerfil = NULL;
	BESQL_Campo *Campo = NULL;
	BEGLOBAL_Empresa_Cliente *Empresa = NULL;
	gchar *sErros = NULL;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Testando Formulario");
	gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW (window));
	gtk_window_set_icon_from_file (GTK_WINDOW (window), "../Interfaces/janela.png", NULL);
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	//SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "mysql", &sErros), "tables_priv"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "empresa"), TRUE, &sErros);
	//SQL = BE_sql_abrir_conexao ("demo", "", NULL, 0, -1, &sErros);
	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);

	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", &sErros), "vis_insumos"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "vis_cep_logradouros"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "vis_cep_bairros"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "cli_ordem"), TRUE, &sErros);
	//SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, NULL);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "sis_info_usuario"), TRUE, NULL);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", &sErros), "vis_orcamentos"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", &sErros), "vis_orcamentos_insumos"), TRUE, &sErros);
	//Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "est_eng_implant", &sErros), "vis_cotacoes"), TRUE, &sErros);
	Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "est_eng_predial", &sErros), "vis_obras"), TRUE, &sErros);
	//SintaxeLog = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "sis_log"), TRUE, &sErros);
	//SintaxeLog = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", &sErros), "sis_log"), TRUE, &sErros);
	//SintaxePerfil = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_provisual", &sErros), "sis_configuracoes"), TRUE, &sErros);
	//SintaxePerfil = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "cia_demo", &sErros), "sis_configuracoes"), TRUE, &sErros);

	if (Sintaxe && (Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
	{
		//filtrando
		Campo = be_sql_selecionar_campo (Sintaxe, "ID");
		if (Campo)
		{
			//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "13061235", BESQL_RELACAO_OU, 0, FALSE);
			//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "28924235", BESQL_RELACAO_OU, 0, FALSE);
			//be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "16", BESQL_RELACAO_OU, 0, FALSE);
			be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, "5773", BESQL_RELACAO_OU, 0, FALSE);
		}
		//informações dos relatórios
		Empresa = g_new (BEGLOBAL_Empresa_Cliente, 1);
		Empresa->sNomeFantasia = g_strdup ("Programação Profissional e Visual");
		Empresa->sRazaoSocial = NULL;
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
		Empresa->Logotipo = gdk_pixbuf_new_from_file ("../relatorio/logo.png", NULL);

		//criar tabela e anexar no container
		#ifdef G_OS_UNIX
			gtk_container_add (GTK_CONTAINER (window), be_Formulario (Sintaxe, SintaxeLog, SintaxePerfil, window, BESQL_ACESSO_COMPLETO, TRUE, "../relatorio/lista padrao.svg", "../relatorio/formulario padrao.svg", "../fontes/suporte/fontes", "xpdf", Empresa, FALSE, func_Subtabela, be_Tabela_atualizar, NULL, func_Executavel, NULL));
		#endif
		#ifdef G_OS_WIN32
			gtk_container_add (GTK_CONTAINER (window), be_Formulario (Sintaxe, SintaxeLog, SintaxePerfil, window, BESQL_ACESSO_COMPLETO, TRUE, "../relatorio/lista padrao.svg", "../relatorio/formulario padrao.svg", "../fontes/suporte/fontes.txt", "C:\\Arquivos de programas\\Adobe\\Acrobat 5.0\\Reader\\AcroRd32.exe", Empresa, FALSE, func_Subtabela, be_Tabela_atualizar, NULL, func_Executavel, NULL));
		#endif
	}
	gtk_widget_show (window);//não utilizar show all
	gtk_main ();

	BE_empresa_cliente_limpar (Empresa);
	//BE_sql_fechar_consulta (Sintaxe);//o próprio be_Formulario fechar
	BE_sql_fechar_consulta (SintaxeLog);
	BE_sql_fechar_consulta (SintaxePerfil);
	BE_sql_fechar_conexao (SQL);

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
