#include "bePrincipal.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowPrincipal.c"

gboolean bFocoInicial = FALSE;
gchar *sConfiguracao_Local = NULL;
gchar *sArquivo_Fontes = NULL;

void be_Principal (BESQL *SQL)
{
	_BEPrincipal_Janela *Janela = NULL;
	guint nIDEvent = 0;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;

	//objetos da janela
	Janela = g_new (_BEPrincipal_Janela, 1);
	Janela->window = be_Interface_criar_windowPrincipal (NULL, NULL, NULL, Janela);
	Janela->status = BE_interfaces_widget (Janela->window, "statusbarStatus");
	Janela->tabulador = BE_interfaces_widget (Janela->window, "notebookPrincipal");
	gtk_notebook_remove_page (GTK_NOTEBOOK (Janela->tabulador), 0);//excluir o primeiro
	//menus
		//escritórios
		Janela->empresa = BE_interfaces_widget (Janela->window, "menubarEmpresa");
			Janela->senha = BE_interfaces_widget (Janela->window, "menubarSenha");
			Janela->usuario = BE_interfaces_widget (Janela->window, "menubarUsuario_menu");
			Janela->console = BE_interfaces_widget (Janela->window, "menubarConsole");
			Janela->auditoria = BE_interfaces_widget (Janela->window, "menubarAuditoria");
			Janela->iniciar = BE_interfaces_widget (Janela->window, "menubarIniciar");
			Janela->adicionar = BE_interfaces_widget (Janela->window, "menubarAdicionar");
			Janela->excluir = BE_interfaces_widget (Janela->window, "menubarExcluir");
		Janela->sair = BE_interfaces_widget (Janela->window, "menubarSair");
		//operações
		Janela->operacoes = BE_interfaces_widget (Janela->window, "menubarMenu_menu");
		Janela->janelas = BE_interfaces_widget (Janela->window, "menubarJanelas");
		Janela->acesso = BE_interfaces_widget (Janela->window, "menubarAcesso");
		Janela->ordenada = BE_interfaces_widget (Janela->window, "menubarOrdenada");
		Janela->aberta = BE_interfaces_widget (Janela->window, "menubarFecharJanela");
		//ajuda
		Janela->config = BE_interfaces_widget (Janela->window, "menubarConfig");
		Janela->servidor = BE_interfaces_widget (Janela->window, "menubarServidor");
		Janela->ajuda = BE_interfaces_widget (Janela->window, "menubarAjuda");
		Janela->sobre = BE_interfaces_widget (Janela->window, "menubarSobre");
	Janela->SQL = SQL;
	Janela->BD = NULL;
	Janela->SintaxeLog = NULL;
	Janela->SintaxePerfil = NULL;
	//menus
	Janela->Menus = NULL;
	Janela->MenusUsuario = NULL;
	//empresa principal
	Janela->Empresa = NULL;
	//usuário
	Janela->sUsuario = NULL;
	Janela->sDescricaoPessoal = NULL;
	Janela->sEmailPessoal = NULL;
	Janela->sModelo = NULL;
	//configurações local
	Janela->Ini = g_key_file_new ();
	Janela->sLerPDF = NULL;
	Janela->sLerPNG = NULL;
	Janela->sNavegador = NULL;
	Janela->sEnviarEmail = NULL;
	Janela->sEditarPNG = NULL;
	Janela->sEditarTexto = NULL;
	Janela->sCalculadora = NULL;
	Janela->bJanelaSeparada = FALSE;
	Janela->nExpirar = atoi (be_sql_informacoes (SQL, BESQL_INFO_EXPIRACAO, &sErros));//valor inicial
	Janela->nExpirarAtual = be_tempo_sec () + Janela->nExpirar;//valor inicial
	Janela->sStatus = NULL;
	//acesso rápido
	Janela->sAcessoRapido = NULL;
	//ativar callback
	be_sql_callback (SQL, _be_Principal_callback_sql, Janela);

	//foco inicial
	gtk_widget_grab_focus (Janela->window);
	//maximizar
	gtk_window_maximize (GTK_WINDOW (Janela->window));

	//meus sinais
	g_signal_connect (Janela->window, "delete_event", G_CALLBACK (_be_Principal_on_delete_event), Janela);//fechar tudo

    //preparar configuração local
    BE_free (sTemp);
    sTemp = be_diretorio_local (TRUE);
    sConfiguracao_Local = g_strdup_printf ("%s%s", sTemp, BEPRINCIPAL_CONFIGURACAO_LOCAL);
    sArquivo_Fontes = g_strdup_printf ("%s%s", sTemp, BEPRINCIPAL_ARQUIVO_FONTES);
    //verificar se arquivo configuração local existe
    if (!g_file_test (sConfiguracao_Local, G_FILE_TEST_EXISTS))
    {
        //copiar de uma versão local
        if (g_file_test (BEPRINCIPAL_CONFIGURACAO_LOCAL, G_FILE_TEST_EXISTS))
        {
            BE_free (sTemp);
            #ifdef G_OS_UNIX
                sTemp = g_strdup_printf ("cp %s %s", BEPRINCIPAL_CONFIGURACAO_LOCAL, sConfiguracao_Local);
            #endif
            #ifdef G_OS_WIN32
                sTemp = g_strdup_printf ("cp %s \"%s\"", BEPRINCIPAL_CONFIGURACAO_LOCAL, sConfiguracao_Local);
            #endif
            system (sTemp);
        }
    }
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	else
	{
		//conectado
		_be_Principal_conectar (Janela);
        //atualizar menu do usuário
        _be_Principal_menu_usuario (Janela);
        //janelas internas
        BE_free (sTemp);
        if ((sTemp = be_sql_perfil_ler (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_SEPARADA, BEPRINCIPAL_PERFIL_SEPARADA_JANELA, &sErros)))
            Janela->bJanelaSeparada = atoi (sTemp);
        gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (Janela->janelas), Janela->bJanelaSeparada);
        //acesso inicial
        BE_free (sTemp);
        if ((sTemp = be_sql_perfil_ler (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, BEPRINCIPAL_PERFIL_MENU_INICIAL, &sErros)))
            _be_Principal_executar_acessos (Janela, sTemp);
        //informar novos erros
        if (sErros)
        {
            be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
            BE_free (sErros);
        }
	}

	//relógio
	nIDEvent = g_timeout_add (BEPRINCIPAL_TIMEOUT, _be_Principal_timeout, Janela);//chamar função a cada BEPRINCIPAL_TIMEOUT (ms)

	//loop principal
	gtk_main ();

	//limpar
	g_source_remove (nIDEvent);
	BE_free (sTemp);
}

gboolean _be_Principal_callback_sql (BESQL *SQL, gint nErro, const gchar *sErro, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	//reiniciar a contagem
	if (nErro == 0)
	{
		Janela->nExpirarAtual = be_tempo_sec () + Janela->nExpirar;//valor inicial
	}
	//erros
	else if (nErro == 2006 || nErro == 2013)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Sua conexão foi perdida.\nReinicie o programa.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		return FALSE;
	}

	return TRUE;//informa erros
}

void _be_Principal_conectar (_BEPrincipal_Janela *Janela)
{
	BESQL_BD *BD = NULL;
	BESQL_Sintaxe *SintaxeTemp = NULL;
	BESQL_Campo *Campo = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;
	gint nPos = 0;

	//banco de dados (empresas)
	for (ListaL = Janela->SQL->BDs; ListaL; ListaL = g_slist_next (ListaL))
	{
		BD = (BESQL_BD *) ListaL->data;
		Lista = g_slist_append (Lista, BE_ISO_UTF8 (BD->sNome));
	}
	//pedir preferência para usuário, se houver mais de 1
	if (g_slist_length (Lista) > 1)
	{
		nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Empresa principal", Lista, 0);
		if (nPos >= 0)
			Janela->BD = (BESQL_BD *) g_slist_nth_data (Janela->SQL->BDs, nPos);
	}
	else
		Janela->BD = (BESQL_BD *) g_slist_nth_data (Janela->SQL->BDs, 0);
	//limpar lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		BE_free (sTemp);
		sTemp = (gchar *) ListaL->data;
	}
	BE_slist_free (Lista);

	//conectar consultas
	if (Janela->BD)
	{
		//selecionar BD
		be_sql_selecionar_bd (Janela->SQL, Janela->BD->sNome, &sErros);
		//carregar todas em cache ?
		if (Janela->SQL->nCacheMax == -2)
		{
			//informar no status
			_be_Principal_status_eventual (Janela, "Criando cache das tabelas...");
			//mouse
			Mouse = be_janela_mouse_pensando (Janela->window);
			//carregar todas
			be_sql_abrir_consulta_cache_todas (Janela->BD, &sErros);
			//mouse padrão
			BE_janela_mouse_normal (Mouse, TRUE);
		}
		//conectar Log de Ações
		Janela->SintaxeLog = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->BD, "sis_log"), TRUE, &sErros);
        //ordenar por AutoIncremento (ID) decrescente
        Campo = be_sql_selecionar_campo_id (Janela->SintaxeLog);
        be_sql_ordem_adicionar (Janela->SintaxeLog, Campo, BESQL_ORDENACAO_DESC);

		//conectar Perfil do Usuário (Configurações)
		Janela->SintaxePerfil = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->BD, "sis_configuracoes"), TRUE, &sErros);
		/*
		sis_configuracoes_publicas
		sis_fluxo
		sis_listagem*/

		//informações da empresa principal
		SintaxeTemp = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->BD, "sis_empresa"), TRUE, &sErros);
		if (SintaxeTemp)
		{
			if (be_sql_selecionar (SintaxeTemp, 1, &sErros) > 0)
			{
				be_sql_selecionar_proxima_linha (SintaxeTemp);

				Janela->Empresa = g_new (BEGLOBAL_Empresa_Cliente, 1);
				Janela->Empresa->sRazaoSocial = NULL;
				Janela->Empresa->sCNPJ = NULL;
				Janela->Empresa->sIE = NULL;
				Janela->Empresa->sTelefone = NULL;
				Janela->Empresa->sFax = NULL;
				Janela->Empresa->sPABX = NULL;
				Janela->Empresa->sEmail = NULL;
				Janela->Empresa->sSite = NULL;
				Janela->Empresa->sCep = NULL;
				Janela->Empresa->sTipoLogradouro = NULL;
				Janela->Empresa->sLogradouro = NULL;
				Janela->Empresa->sComplemento = NULL;
				Janela->Empresa->sBairro = NULL;
				Janela->Empresa->sCidade = NULL;
				Janela->Empresa->sEstado = NULL;
				Janela->Empresa->sPais = NULL;
				Janela->Empresa->Logotipo = NULL;
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 0))->sValor)
					Janela->Empresa->sNomeFantasia = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 0))->sValor);
				else
					Janela->Empresa->sNomeFantasia = g_strdup ("Sem nome");
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 1))->sValor)
					Janela->Empresa->sRazaoSocial = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 1))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 3))->sValor)
					Janela->Empresa->sCNPJ = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 3))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 4))->sValor)
					Janela->Empresa->sIE = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 4))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 5))->sValor)
					Janela->Empresa->sTelefone = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 5))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 6))->sValor)
					Janela->Empresa->sFax = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 6))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 7))->sValor)
					Janela->Empresa->sPABX = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 7))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 8))->sValor)
					Janela->Empresa->sEmail = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 8))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 9))->sValor)
					Janela->Empresa->sSite = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 9))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 10))->sValor)
					Janela->Empresa->sCep = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 10))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 11))->sValor)
					Janela->Empresa->sTipoLogradouro = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 11))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 12))->sValor)
					Janela->Empresa->sLogradouro = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 12))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 13))->sValor)
					Janela->Empresa->sComplemento = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 13))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 14))->sValor)
					Janela->Empresa->sBairro = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 14))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 15))->sValor)
					Janela->Empresa->sCidade = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 15))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 16))->sValor)
					Janela->Empresa->sEstado = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 16))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 17))->sValor)
					Janela->Empresa->sPais = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 17))->sValor);
				if (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 2))->sValor)
					Janela->Empresa->Logotipo = be_sql_recuperar_arquivo_pixbuf ((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 2), 'I');
			}
		}
		BE_sql_fechar_consulta (SintaxeTemp);

		//informações do usuário
		SintaxeTemp = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->BD, "sis_info_usuario"), TRUE, &sErros);
		if (SintaxeTemp)
		{
			if (be_sql_selecionar (SintaxeTemp, 1, &sErros) > 0)
			{
				be_sql_selecionar_proxima_linha (SintaxeTemp);

				Janela->sUsuario = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 0))->sValor);
				Janela->sDescricaoPessoal = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 1))->sValor);
				Janela->sEmailPessoal = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 2))->sValor);
				Janela->sModelo = g_strdup (((BESQL_Campo *) g_slist_nth_data (SintaxeTemp->Campos, 3))->sValor);
			}
		}
		BE_sql_fechar_consulta (SintaxeTemp);

		//atualizar menus
		SintaxeTemp = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->BD, "sis_menu"), TRUE, &sErros);
		if (SintaxeTemp)
			_be_Principal_menus (Janela, SintaxeTemp);
		BE_sql_fechar_consulta (SintaxeTemp);
	}

	//carregar configurações local
	_be_Principal_configuracoes_local (Janela, FALSE);

	//status
	_be_Principal_status (Janela, TRUE);	

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	BE_free (sTemp);
}

void _be_Principal_desconectar (_BEPrincipal_Janela *Janela)
{
	_BEPrincipal_Menu *Menu = NULL;
	GSList *ListaL = NULL;

	//desconectar consultas
	BE_sql_fechar_consulta (Janela->SintaxeLog);
	BE_sql_fechar_consulta (Janela->SintaxePerfil);
	//menus
	for (ListaL = Janela->Menus; ListaL; ListaL = g_slist_next (ListaL))
	{
		Menu = (_BEPrincipal_Menu *) ListaL->data;
		BE_free (Menu->sTransacao);
		BE_free (Menu->sVisao);
		BE_free (Menu->sID);
		BE_free (Menu);
	}
	BE_slist_free (Janela->Menus);
	//limpar empresas
	BE_empresa_cliente_limpar (Janela->Empresa);
	//usuário
	BE_free (Janela->sUsuario);
	BE_free (Janela->sDescricaoPessoal);
	BE_free (Janela->sEmailPessoal);
	BE_free (Janela->sModelo);
	//configurações local
	g_key_file_free (Janela->Ini);
	BE_free (Janela->sLerPDF);
	BE_free (Janela->sLerPNG);
	BE_free (Janela->sNavegador);
	BE_free (Janela->sEnviarEmail);
	BE_free (Janela->sEditarPNG);
	BE_free (Janela->sEditarTexto);
	BE_free (Janela->sCalculadora);
	BE_free (Janela->sStatus);
	BE_free (sConfiguracao_Local);
	BE_free (sArquivo_Fontes);
	//printf ("OK - Limpar Tudo\n");
}

void _be_Principal_configuracoes_local (_BEPrincipal_Janela *Janela, gboolean bGravar)
{
	FILE *File = NULL;
	gchar *sTemp = NULL;

	if (bGravar)
	{
		//ativar configurações
		#ifdef G_OS_UNIX
			if (Janela->sLerPDF)
				g_key_file_set_string (Janela->Ini, "lerpdf", "linux", Janela->sLerPDF);
			if (Janela->sLerPNG)
				g_key_file_set_string (Janela->Ini, "lerpng", "linux", Janela->sLerPNG);
			if (Janela->sNavegador)
				g_key_file_set_string (Janela->Ini, "navegador", "linux", Janela->sNavegador);
			if (Janela->sEnviarEmail)
				g_key_file_set_string (Janela->Ini, "enviaremail", "linux", Janela->sEnviarEmail);
			if (Janela->sEditarPNG)
				g_key_file_set_string (Janela->Ini, "editarpng", "linux", Janela->sEditarPNG);
			if (Janela->sEditarTexto)
				g_key_file_set_string (Janela->Ini, "editartexto", "linux", Janela->sEditarTexto);
			if (Janela->sCalculadora)
				g_key_file_set_string (Janela->Ini, "calculadora", "linux", Janela->sCalculadora);
		#endif
		#ifdef G_OS_WIN32
			if (Janela->sLerPDF)
				g_key_file_set_string (Janela->Ini, "lerpdf", "windows", Janela->sLerPDF);
			if (Janela->sLerPNG)
				g_key_file_set_string (Janela->Ini, "lerpng", "windows", Janela->sLerPNG);
			if (Janela->sNavegador)
				g_key_file_set_string (Janela->Ini, "navegador", "windows", Janela->sNavegador);
			if (Janela->sEnviarEmail)
				g_key_file_set_string (Janela->Ini, "enviaremail", "windows", Janela->sEnviarEmail);
			if (Janela->sEditarPNG)
				g_key_file_set_string (Janela->Ini, "editarpng", "windows", Janela->sEditarPNG);
			if (Janela->sEditarTexto)
				g_key_file_set_string (Janela->Ini, "editartexto", "windows", Janela->sEditarTexto);
			if (Janela->sCalculadora)
				g_key_file_set_string (Janela->Ini, "calculadora", "windows", Janela->sCalculadora);
		#endif

		//salvar configurações
		if ((File = fopen (sConfiguracao_Local, "wb")))
		{
			BE_free (sTemp);
			sTemp = g_key_file_to_data (Janela->Ini, NULL, NULL);
			fwrite (sTemp, strlen (sTemp), 1, File);
			//fechando arquivo
			fclose (File);
		}
		else
		{
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Não foi possível gravar o arquivo %s.", sConfiguracao_Local);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		}
	}
	else
	{
		if (g_key_file_load_from_file (Janela->Ini, sConfiguracao_Local, G_KEY_FILE_NONE, NULL))
		{
			#ifdef G_OS_UNIX
				Janela->sLerPDF = g_key_file_get_string (Janela->Ini, "lerpdf", "linux", NULL);//xppdf, kpdf, acroread
				Janela->sLerPNG = g_key_file_get_string (Janela->Ini, "lerpng", "linux", NULL);
				Janela->sNavegador = g_key_file_get_string (Janela->Ini, "navegador", "linux", NULL);
				Janela->sEnviarEmail = g_key_file_get_string (Janela->Ini, "enviaremail", "linux", NULL);
				Janela->sEditarPNG = g_key_file_get_string (Janela->Ini, "editarpng", "linux", NULL);
				Janela->sEditarTexto = g_key_file_get_string (Janela->Ini, "editartexto", "linux", NULL);
				Janela->sCalculadora = g_key_file_get_string (Janela->Ini, "calculadora", "linux", NULL);
			#endif
			#ifdef G_OS_WIN32
				Janela->sLerPDF = g_key_file_get_string (Janela->Ini, "lerpdf", "windows", NULL);
				Janela->sLerPNG = g_key_file_get_string (Janela->Ini, "lerpng", "windows", NULL);
				Janela->sNavegador = g_key_file_get_string (Janela->Ini, "navegador", "windows", NULL);
				Janela->sEnviarEmail = g_key_file_get_string (Janela->Ini, "enviaremail", "windows", NULL);
				Janela->sEditarPNG = g_key_file_get_string (Janela->Ini, "editarpng", "windows", NULL);
				Janela->sEditarTexto = g_key_file_get_string (Janela->Ini, "editartexto", "windows", NULL);
				Janela->sCalculadora = g_key_file_get_string (Janela->Ini, "calculadora", "windows", NULL);
			#endif
			if (Janela->sLerPDF && strlen (Janela->sLerPDF) == 0)
			{
				BE_free (Janela->sLerPDF);
			}
			if (Janela->sLerPNG && strlen (Janela->sLerPNG) == 0)
			{
				BE_free (Janela->sLerPNG);
			}
			if (Janela->sNavegador && strlen (Janela->sNavegador) == 0)
			{
				BE_free (Janela->sNavegador);
			}
			if (Janela->sEnviarEmail && strlen (Janela->sEnviarEmail) == 0)
			{
				BE_free (Janela->sEnviarEmail);
			}
			if (Janela->sEditarPNG && strlen (Janela->sEditarPNG) == 0)
			{
				BE_free (Janela->sEditarPNG);
			}
			if (Janela->sEditarTexto && strlen (Janela->sEditarTexto) == 0)
			{
				BE_free (Janela->sEditarTexto);
			}
			if (Janela->sCalculadora && strlen (Janela->sCalculadora) == 0)
			{
				BE_free (Janela->sCalculadora);
			}
		}
	}

	BE_free (sTemp);
}

void _be_Principal_menus (_BEPrincipal_Janela *Janela, BESQL_Sintaxe *Sintaxe)
{
	_BEPrincipal_Menu *Menu = NULL, *Menu_A = NULL;
	GtkWidget *menu = NULL;
	BESQL_Campo *CampoTitulo = NULL, *CampoTransacao = NULL, *CampoVisao = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sErros = NULL;
	glong lLinhas = 0;
	gint nPaginaDados = 1;

	//criar menus
	if (Sintaxe && Janela->BD)
	{
		//loop dos dados
		nPaginaDados = 1;
		do
		{
			lLinhas = be_sql_selecionar (Sintaxe, nPaginaDados, &sErros);
			while (lLinhas > 0 && be_sql_selecionar_proxima_linha (Sintaxe))
			{
				CampoTitulo = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 1);//título
				CampoTransacao = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 4);//transação
				CampoVisao = (BESQL_Campo *) g_slist_nth_data (Sintaxe->Campos, 2);//visualização ou tabela
				//criar estrutura de menu
				Menu = g_new (_BEPrincipal_Menu, 1);
				Menu->Janela = Janela;
				if (CampoTransacao->sValor)
					Menu->sTransacao = g_strdup (CampoTransacao->sValor);
				else
					Menu->sTransacao = NULL;
				if (CampoVisao->sValor)
					Menu->sVisao = g_strdup (CampoVisao->sValor);
				else
					Menu->sVisao = NULL;
				Menu->sID = NULL;
				Menu->Mae = NULL;
				Menu->Filha = NULL;
				//criar menu
				BE_free (sTemp);
				BE_free (sTemp1);
				//verificar valores válidos
				if (CampoTitulo->sValor && CampoTransacao->sValor)
				{
					sTemp1 = g_strdup_printf ("%s [%s]", CampoTitulo->sValor, CampoTransacao->sValor);
					sTemp = BE_ISO_UTF8 (sTemp1);
					Menu->menuitem = gtk_menu_item_new_with_mnemonic (sTemp);
					//informar na lista de menus
					Janela->Menus = g_slist_append (Janela->Menus, Menu);
					//classificar
					if (Menu->sTransacao)
					{
						//mães
						if (strlen (Menu->sTransacao) == 1)
						{
							gtk_menu_shell_append (GTK_MENU_SHELL (Janela->operacoes), Menu->menuitem);//anexar
						}
						//filho
						else
						{
							while (strlen (Menu->sTransacao) <= strlen (Menu_A->sTransacao))
								Menu_A = Menu_A->Mae;//voltar

							menu = gtk_menu_item_get_submenu (GTK_MENU_ITEM (Menu_A->menuitem));
							if (!menu)
							{
								menu = gtk_menu_new ();
								gtk_menu_item_set_submenu (GTK_MENU_ITEM (Menu_A->menuitem), menu);
							}
							gtk_menu_shell_append (GTK_MENU_SHELL (menu), Menu->menuitem);//anexar
							Menu->Mae = Menu_A;//avançar
							Menu_A->Filha = Menu;
						}
						//anterior
						Menu_A = Menu;
					}
					else
						gtk_menu_shell_append (GTK_MENU_SHELL (Janela->operacoes), Menu->menuitem);//anexar

					//sinais
					g_signal_connect (Menu->menuitem, "activate", G_CALLBACK (_be_Principal_on_menubarOperacoes_activate), Menu);
					//exibir
					gtk_widget_show (Menu->menuitem);
				}
			}
			nPaginaDados++;
		} while (lLinhas > 0 && lLinhas >= BESQL_MAX_LINHAS_POR_PAGINA);
	}

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Principal_on_menubarOperacoes_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Menu *Menu = NULL;
	_BEPrincipal_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	gchar *sErros = NULL;
	gint nTipo = 0;

	Menu = (_BEPrincipal_Menu *) user_data;
	Janela = Menu->Janela;

	if (Menu->sVisao && !Menu->Filha && Janela->BD)
	{
		//mouse
		Mouse = be_janela_mouse_pensando (Janela->window);
		//consulta
		Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->BD, Menu->sVisao), TRUE, &sErros);

		//aplicar filtro
		if (Menu->sID && strlen (Menu->sID) > 0)
		{
			Campo = be_sql_selecionar_campo (Sintaxe, "ID");
			be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, Menu->sID, BESQL_RELACAO_E, 0, FALSE);
			nTipo = 1;
		}

		//mouse padrão
		BE_janela_mouse_normal (Mouse, TRUE);

		//abrir janela
		if (Janela->bJanelaSeparada)
			_be_Principal_abrir_janela (Janela, Sintaxe, nTipo);
		else
			_be_Principal_abrir_janela_interna (Janela, Sintaxe, nTipo);
		//limpar filro
		BE_free (Menu->sID);
	}

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
}

void _be_Principal_sair (_BEPrincipal_Janela *Janela, gboolean bPerguntar)
{
	if (bPerguntar && be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Deseja realmente sair ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
		return;

	//limpar tudo
	_be_Principal_desconectar (Janela);
	gtk_widget_destroy (Janela->window);
	BE_free (Janela);
}

void _be_Principal_status_eventual (_BEPrincipal_Janela *Janela, const gchar *sInformacao)
{
	//limpar
	gtk_statusbar_pop (GTK_STATUSBAR (Janela->status), 1);
	//exibir
	gtk_statusbar_push (GTK_STATUSBAR (Janela->status), 1, sInformacao);
}

void _be_Principal_status (_BEPrincipal_Janela *Janela, gboolean bAtualizar)
{
	gchar *sErros = NULL;
	gchar *sIP = NULL, *sConexao = NULL, *sUsuario = NULL, *sSegura = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	
	//atualizar informações
	if (bAtualizar)
	{
		//limpar
		gtk_statusbar_pop (GTK_STATUSBAR (Janela->status), 1);
		BE_free (Janela->sStatus);
		//IP
		sIP = be_sql_informacoes (Janela->SQL, BESQL_INFO_CLIENTE_IP, &sErros);
		//conexão
		sConexao = be_sql_informacoes (Janela->SQL, BESQL_INFO_CONEXAO, &sErros);
		//usuário
		sUsuario = be_sql_informacoes (Janela->SQL, BESQL_INFO_USUARIO, &sErros);
		//conexão segura SSL
		sSegura = be_sql_informacoes (Janela->SQL, BESQL_INFO_SEGURO, &sErros);
		//formatar exibição
		BE_free (sTemp);
		if (sSegura)
            sTemp = g_strdup_printf ("<< %s >>: Usuário: %s  |  Conexão: %s  |  IP: %s", sSegura, Janela->sUsuario ? Janela->sUsuario : sUsuario , sConexao, sIP);
        else
            sTemp = g_strdup_printf ("Usuário: %s  |  Conexão: %s  |  IP: %s", Janela->sUsuario ? Janela->sUsuario : sUsuario , sConexao, sIP);
		//converter
		Janela->sStatus = BE_ISO_UTF8 (sTemp);
	}
	//verificar se há tempo para expirar
	BE_free (sTemp);
	if (Janela->nExpirar > 0)
	{
		//contar tempo para expiração
		if (be_tempo_sec () < Janela->nExpirarAtual)
		{
			if ((Janela->nExpirarAtual - be_tempo_sec ()) > 3600)
				sTemp = g_strdup_printf ("%s | Expirar: %d h", Janela->sStatus, (Janela->nExpirarAtual - be_tempo_sec ()) / 3600);//min
			else
				sTemp = g_strdup_printf ("%s | Expirar: %d min", Janela->sStatus, (Janela->nExpirarAtual - be_tempo_sec ()) / 60);//min
		}
		else
		{
			sTemp = g_strdup_printf ("%s | Expirado", Janela->sStatus);
			Janela->nExpirar = 0;
		}
	}
	else
		sTemp = g_strdup_printf ("%s | Expirado", Janela->sStatus);

	//exibir
	gtk_statusbar_push (GTK_STATUSBAR (Janela->status), 1, sTemp);

	//empresa
	if (Janela->Empresa && Janela->Empresa->sNomeFantasia)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_ISO_UTF8 (Janela->Empresa->sNomeFantasia);
		sTemp = g_strdup_printf ("%s :: %s", sTemp1, g_get_application_name ());
		gtk_window_set_title (GTK_WINDOW (Janela->window), sTemp);
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	BE_free (sIP);
	BE_free (sConexao);
	BE_free (sUsuario);
	BE_free (sSegura);
	BE_free (sTemp);
	BE_free (sTemp1);
}

gboolean _be_Principal_timeout (gpointer data)
{
	_BEPrincipal_Janela *Janela = NULL;

	Janela = (_BEPrincipal_Janela *) data;

	_be_Principal_status (Janela, FALSE);

	return TRUE;//continuar
}

void _be_Interface_sinal_windowPrincipal_on_menubarEmpresa_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	GString *Temp = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	if (!Janela->Empresa)
		return;

	//empresa principal
	Temp = g_string_new ("");
	if (Janela->Empresa->sNomeFantasia)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sNomeFantasia);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "<span size='large'><b>%s</b></span>\n", sTemp1);
	}
	if (Janela->Empresa->sRazaoSocial)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sRazaoSocial);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "%s\n", sTemp1);
	}
	if (Janela->Empresa->sCNPJ)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sCNPJ);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "CNPJ: %s\n", sTemp1);
	}
	if (Janela->Empresa->sIE)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sIE);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "IE: %s\n", sTemp1);
	}
	if (Janela->Empresa->sTelefone)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sTelefone);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "Fone: %s\n", sTemp1);
	}
	if (Janela->Empresa->sFax)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sFax);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "Fax: %s\n", sTemp1);
	}
	if (Janela->Empresa->sPABX)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sPABX);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "PABX: %s\n", sTemp1);
	}
	if (Janela->Empresa->sEmail)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sEmail);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "<span foreground='#0000FF'><i>%s</i></span>\n", sTemp1);
	}
	if (Janela->Empresa->sSite)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sSite);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "<span foreground='#0000FF'><i>%s</i></span>\n", sTemp1);
	}
	if (Janela->Empresa->sCep)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sCep);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "CEP: %s\n", sTemp1);
	}
	if (Janela->Empresa->sTipoLogradouro)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sTipoLogradouro);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "Endereco: %s", sTemp1);
	}
	if (Janela->Empresa->sLogradouro)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sLogradouro);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, " %s", sTemp1);
	}
	if (Janela->Empresa->sComplemento)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sComplemento);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, ", %s", sTemp1);
	}
	if (Janela->Empresa->sBairro)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sBairro);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, " - %s", sTemp1);
	}
	if (Janela->Empresa->sCidade)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sCidade);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "\n%s", sTemp1);
	}
	if (Janela->Empresa->sEstado)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sEstado);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, "/%s", sTemp1);
	}
	if (Janela->Empresa->sPais)
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = BE_ISO_UTF8 (Janela->Empresa->sPais);
		sTemp1 = g_markup_printf_escaped ("%s", sTemp);
		g_string_append_printf (Temp, " - %s", sTemp1);
	}
	//exibir
	sTemp = BE_string_free (Temp);
	be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);

	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Interface_sinal_windowPrincipal_on_menubarConsole_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	GtkWidget *window = NULL;
	BEGlobal_Janela *Mouse = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	if (!Janela->SQL)
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//criar nova janela
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Console");
	gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW (window));
	//gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (Janela->window));//filho
	//gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	//anexar tabela de log
	gtk_container_add (GTK_CONTAINER (window), be_Console (window, Janela->SQL));
	//exibir
	gtk_widget_show (window);

	//mouse padrão
	BE_janela_mouse_normal (Mouse, FALSE);
}

void _be_Interface_sinal_windowPrincipal_on_menubarAuditoria_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	GtkWidget *window = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sTemp = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	if (!Janela->SintaxeLog)
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//criar nova janela
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	sTemp = BE_ISO_UTF8 ("Auditoria - Log de Ações");
	gtk_window_set_title (GTK_WINDOW (window), sTemp);
	gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW (window));
	//gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (Janela->window));//filho
	//gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	//preparar filtro
	be_sql_filtro_limpar (Janela->SintaxeLog, FALSE);
	//anexar tabela de log
	gtk_container_add (GTK_CONTAINER (window), be_Tabela (Janela->SintaxeLog, NULL, NULL, window, BESQL_ACESSO_SELECIONAR, FALSE, BEPRINCIPAL_ARQUIVO_LISTA_PADRAO, BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO, sArquivo_Fontes, Janela->sLerPDF, Janela->Empresa, TRUE, TRUE, _be_Principal_callback_Formulario1, _be_Principal_callback_Tabela1, Janela));
	//exibir
	gtk_widget_show (window);

	//mouse padrão
	BE_janela_mouse_normal (Mouse, FALSE);

	//limpar
	BE_free (sTemp);
}

void _be_Principal_on_menubarUsuario_activate (GtkMenuItem *menuitem, gpointer user_data)
{
    _BEPrincipal_Janela *Janela = NULL;
    _BEPrincipal_Menu *Menu = NULL;

    Menu = (_BEPrincipal_Menu *) user_data;
    Janela = Menu->Janela;

    //executar transação
    if (Menu->sTransacao)
        _be_Principal_executar_acessos (Janela, Menu->sTransacao);
}

void _be_Principal_menu_usuario (_BEPrincipal_Janela *Janela)
{
	_BEPrincipal_Menu *Menu = NULL;
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;

	//excluir menus: loop nos menus existentes
	for (ListaL = Janela->MenusUsuario; ListaL; ListaL = g_slist_next (ListaL))
	{
		Menu = (_BEPrincipal_Menu *) ListaL->data;
		BE_free (Menu->sTransacao);
		BE_free (Menu->sVisao);
		BE_free (Menu->sID);
		gtk_widget_destroy (Menu->menuitem);//destruir menu antigo
		BE_free (Menu);
	}
	BE_slist_free (Janela->MenusUsuario);

	//carregar lista de perfil
	Lista = be_sql_perfil_variaveis (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, &sErros);
	//loop na lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		sTemp = (gchar *) ListaL->data;
        //criar estrutura de menu
        Menu = g_new (_BEPrincipal_Menu, 1);
        Menu->Janela = Janela;
        //carregar código de acesso (Transação)
        Menu->sTransacao = be_sql_perfil_ler (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, sTemp, &sErros);
        Menu->sVisao = g_strdup (sTemp);
        if (Menu->sTransacao)
        {
            BE_free (sTemp);
            sTemp = g_strdup_printf ("%s [%s]", Menu->sVisao, Menu->sTransacao);
            Menu->menuitem = gtk_menu_item_new_with_mnemonic (sTemp);
        }
        else
            Menu->menuitem = gtk_menu_item_new_with_mnemonic (Menu->sVisao);

		Menu->sID = NULL;
		Menu->Mae = NULL;
		Menu->Filha = NULL;
		//informar na lista de menus
		Janela->MenusUsuario = g_slist_append (Janela->MenusUsuario, Menu);
		//anexar
		gtk_menu_shell_append (GTK_MENU_SHELL (Janela->usuario), Menu->menuitem);
		//sinais
		g_signal_connect (Menu->menuitem, "activate", G_CALLBACK (_be_Principal_on_menubarUsuario_activate), Menu);
		//exibir
		gtk_widget_show (Menu->menuitem);
		//limpar
		BE_free (sTemp);
	}
	BE_slist_free (Lista);
}

void _be_Interface_sinal_windowPrincipal_on_menubarIniciar_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	gchar *sErros = NULL;
	gchar *sValor = NULL, *sTemp = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	//carregar existente
	sTemp = be_sql_perfil_ler (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, BEPRINCIPAL_PERFIL_MENU_INICIAL, &sErros);
	//janela de interação
	sValor = be_Mensagens_in (GTK_WINDOW (Janela->window), FALSE, "Acesso Inicial", "Digite o código da janela.\n\n<i>Ex.: PRT, PRT801, IW32, C5 C445 BR89</i>", sTemp);
	if (sValor)
	{
		//salvar
		if (strlen (sValor) > 0)
        {
            //salvar
            if (!be_sql_perfil_salvar (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, BEPRINCIPAL_PERFIL_MENU_INICIAL, sValor, FALSE, &sErros))
                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível salvar o acesso inicial.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
        }
        //excluir
        else if (!be_sql_perfil_excluir (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, BEPRINCIPAL_PERFIL_MENU_INICIAL, &sErros))
        {
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível excluir este perfil.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
		}
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//limpar
	BE_free (sValor);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarAdicionar_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	gchar *sErros = NULL;
	gchar *sValor = NULL, *sVariavel = NULL, *sTemp = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	//janela de interação
	sVariavel = be_Mensagens_in (GTK_WINDOW (Janela->window), FALSE, "Adicionar menu de acesso", "Digite um nome para este menu de acesso.", NULL);
	if (sVariavel && strlen (sVariavel) > 0)
	{
        //janela de interação
        sValor = be_Mensagens_in (GTK_WINDOW (Janela->window), FALSE, sVariavel, "Digite o código da janela.\n\n<i>Ex.: PRT, PRT801, IW32, C5 C445 BR89</i>", sTemp);
        if (sValor && strlen (sValor) > 0)
        {
            //salvar
            if (!be_sql_perfil_salvar (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, sVariavel, sValor, FALSE, &sErros))
                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível salvar o menu de acesso.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
        }
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//atualizar menu
	_be_Principal_menu_usuario (Janela);

	//limpar
	BE_free (sVariavel);
	BE_free (sValor);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarExcluir_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sErros = NULL;
	gchar *sVariavel = NULL, *sTemp = NULL;
	gint nPos = 0;

	Janela = (_BEPrincipal_Janela *) user_data;

	//carregar lista de perfil
	Lista = be_sql_perfil_variaveis (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, &sErros);
	//exibir lista
	if (g_slist_length (Lista) > 0)
	{
		nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Excluir menu de acesso", Lista, 0);
		//recuperar valor
		BE_free (sTemp);
		sTemp = (gchar *) g_slist_nth_data (Lista, nPos);
		if (sTemp)
			sVariavel = g_strdup (sTemp);
	}
	//limpar lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		sTemp = (gchar *) ListaL->data;
		BE_free (sTemp);
	}
	BE_slist_free (Lista);

	//localizar existente
	if (sVariavel && !sErros)
	{
		if (!be_sql_perfil_excluir (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_MENU, sVariavel, &sErros))
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível excluir este menu de acesso.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
		}
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//atualizar menu
	_be_Principal_menu_usuario (Janela);

	//limpar
	BE_free (sVariavel);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarSair_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;
	_be_Principal_sair (Janela, TRUE);
}

void _be_Interface_sinal_windowPrincipal_on_menubarJanelas_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	gchar *sErros = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	Janela->bJanelaSeparada = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (Janela->janelas));
    //salvar
    if (!be_sql_perfil_salvar (Janela->SintaxePerfil, BEPRINCIPAL_PERFIL_SEPARADA, BEPRINCIPAL_PERFIL_SEPARADA_JANELA, Janela->bJanelaSeparada ? "1" : "0", FALSE, &sErros))
        be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível salvar esta alteração.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);

    //informar novos erros
    if (sErros)
    {
        be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
        BE_free (sErros);
    }
}

gboolean _be_Principal_executar_acessos (_BEPrincipal_Janela *Janela, const gchar *sAcesso)
{
	_BEPrincipal_Menu *Menu = NULL;
	GSList *ListaL = NULL;
	gchar **aLista = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTransacao = NULL, *sID = NULL;
	gboolean bAchou = FALSE;
	gint nCont = 0;

	//verificar acesso válido
	if (!Janela || !sAcesso)
		return FALSE;

	//remover espaços iniciais e finais
	sTemp = g_strdup (sAcesso);
	sTemp = g_strstrip (sTemp);
	//preparar lista
	aLista = g_strsplit (sTemp, " ", -1);

	//loop nas transações
	for (nCont = 0; nCont < g_strv_length (aLista); nCont++)
	{
		//verificar se há pergunta
		if (g_strrstr (aLista[nCont], "*"))
		{
            BE_free (sID);
            if ((sID = be_Mensagens_in (GTK_WINDOW (Janela->window), FALSE, "Acesso rápido", "Digite o código ID.", NULL)))
            {
                BE_free (sTemp1);
                sTemp1 = aLista[nCont];
                aLista[nCont] = be_strreplace (sTemp1, "*", sID);
            }
        }
		//separar código  e ID
		BE_free (sTemp1);
		BE_free (sTransacao);
		BE_free (sID);
		sTemp1 = g_ascii_strup (aLista[nCont], -1);//restante
		sTransacao = be_strclear (sTemp1, BEGLOBAL_CONVERSAO_LETRAS);
		sID = be_strclear (sTemp1, BEGLOBAL_CONVERSAO_DIGITOS);
		//menus
		for (ListaL = Janela->Menus; ListaL; ListaL = g_slist_next (ListaL))
		{
			Menu = (_BEPrincipal_Menu *) ListaL->data;
			if (g_strrstr (Menu->sTransacao, sTransacao))
			{
				if (Menu->sVisao && !Menu->Filha)
				{
					bAchou = TRUE;
					BE_free (Menu->sID);
					Menu->sID = g_strdup (sID);
					_be_Principal_on_menubarOperacoes_activate (GTK_MENU_ITEM (Menu->menuitem), Menu);
				}
				//deu certo, sair do loop
				break;
			}
		}
		//informar
		if (!bAchou)
		{
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Não foi possível abrir o código '%s'.", sTransacao);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		}
    }

    //limpar
    BE_strfreev (aLista);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTransacao);
	BE_free (sID);

    return bAchou;
}

void _be_Interface_sinal_windowPrincipal_on_menubarAcesso_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	//F6
	_BEPrincipal_Janela *Janela = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gboolean bAchou = FALSE;

	Janela = (_BEPrincipal_Janela *) user_data;

	//acesso inicial
	if (Janela->sAcessoRapido)
		sTemp1 = g_strdup (Janela->sAcessoRapido);
	//loop dos dos acesso
	while (!bAchou && (sTemp = be_Mensagens_in (GTK_WINDOW (Janela->window), FALSE, "Acesso rápido", "Digite o código da janela.\n\n<i>Ex.: PRT, PRT801, IW32, C5 C445 BR89</i>", sTemp1)))
	{
		bAchou = _be_Principal_executar_acessos (Janela, sTemp);
		BE_free (sTemp1);
		sTemp1 = sTemp;
	}
	//guardar o último acesso válido
	if (sTemp)
		Janela->sAcessoRapido = g_strdup (sTemp);

	//limpar
	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarOrdenada_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_BD *BD = NULL;
	BESQL_Tabela *Tabela = NULL;
	GSList *Lista = NULL, *Lista1 = NULL, *ListaL = NULL, *ListaL1 = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;
	gint nPos = -1, nCont = 0;

	Janela = (_BEPrincipal_Janela *) user_data;

	//criar lista somente do BD atual
	//loop das tabelas
	for (ListaL1 = Janela->BD->Tabelas; ListaL1; ListaL1 = g_slist_next (ListaL1))
	{
		Tabela = (BESQL_Tabela *) ListaL1->data;
		if (Tabela->nFlags & BESQL_ACESSO_SELECIONAR)
		{
			if (!g_strstr_len (Tabela->sNome, 4, "adi_") && !g_strstr_len (Tabela->sNome, 4, "alt_") && !g_strstr_len (Tabela->sNome, 4, "blo_") && !g_strstr_len (Tabela->sNome, 4, "exc_") && !g_strstr_len (Tabela->sNome, 4, "mex_"))
			{
				BE_free (sTemp);
				if (Tabela->sNomeUsual)
					sTemp = g_strdup_printf ("%s", Tabela->sNomeUsual);
				else if (Tabela->sNomeReal)
					sTemp = g_strdup_printf ("%s", Tabela->sNomeReal);
				else
					sTemp = g_strdup_printf ("%s", Tabela->sNome);
				Lista = g_slist_append (Lista, BE_ISO_UTF8 (sTemp));
				Lista1 = g_slist_append (Lista1, Tabela);//tabela real
			}
		}
	}
	//separador
	Lista = g_slist_append (Lista, BE_ISO_UTF8 ("--"));
	Lista1 = g_slist_append (Lista1, NULL);
	//loop de todos BDs
	nCont = g_slist_length (Janela->SQL->BDs);
	for (ListaL = Janela->SQL->BDs; ListaL; ListaL = g_slist_next (ListaL))
	{
		//loop das tabelas
		BD = (BESQL_BD *) ListaL->data;
		for (ListaL1 = BD->Tabelas; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Tabela = (BESQL_Tabela *) ListaL1->data;
			if (Tabela->nFlags & BESQL_ACESSO_SELECIONAR)
			{
				if (!g_strstr_len (Tabela->sNome, 4, "adi_") && !g_strstr_len (Tabela->sNome, 4, "alt_") && !g_strstr_len (Tabela->sNome, 4, "blo_") && !g_strstr_len (Tabela->sNome, 4, "exc_") && !g_strstr_len (Tabela->sNome, 4, "mex_"))
				{
					BE_free (sTemp);
					if (Tabela->sNomeUsual)
						sTemp = g_strdup_printf ("%s.%s", BD->sNome, Tabela->sNomeUsual);
					else if (Tabela->sNomeReal)
						sTemp = g_strdup_printf ("%s.%s", BD->sNome, Tabela->sNomeReal);
					else
						sTemp = g_strdup_printf ("%s.%s", BD->sNome, Tabela->sNome);
					Lista = g_slist_append (Lista, BE_ISO_UTF8 (sTemp));
					Lista1 = g_slist_append (Lista1, Tabela);//tabela real
				}
			}
		}
	}
	//lista ordenada
	ListaL = g_slist_sort (ListaL, be_string_ordenar_string);
	nPos = be_Lista (GTK_WINDOW (Janela->window), TRUE, "Tabelas", Lista, 0);

	//limpar lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		BE_free (sTemp);
		sTemp = (gchar *) ListaL->data;
	}
	BE_slist_free (Lista);

	//abrir tabela selecionada
	Tabela = (BESQL_Tabela *) g_slist_nth_data (Lista1, nPos);
	//tabela existente
	if (Tabela)
	{
		//mouse
		Mouse = be_janela_mouse_pensando (Janela->window);
		//consulta
		Sintaxe = be_sql_abrir_consulta (Tabela, TRUE, &sErros);
		//mouse padrão
		BE_janela_mouse_normal (Mouse, TRUE);
		//abrir janela
		if (Janela->bJanelaSeparada)
			_be_Principal_abrir_janela (Janela, Sintaxe, 0);
		else
			_be_Principal_abrir_janela_interna (Janela, Sintaxe, 0);
	}

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarFecharJanela_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	gint nTab = 0;

	Janela = (_BEPrincipal_Janela *) user_data;

	nTab = gtk_notebook_get_current_page (GTK_NOTEBOOK (Janela->tabulador));
	if (nTab >= 0)
	{
		gtk_notebook_remove_page (GTK_NOTEBOOK (Janela->tabulador), nTab);
	}
	//desativar fechamento, se não houver mais janelas abertas
	if (gtk_notebook_get_n_pages (GTK_NOTEBOOK (Janela->tabulador)) == 0)
		gtk_widget_set_sensitive (Janela->aberta, FALSE);
}

void _be_Principal_abrir_janela_interna (_BEPrincipal_Janela *Janela, BESQL_Sintaxe *Sintaxe, gint nTipo)
{
	GtkWidget *janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Filtro *Filtro = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	_BEPrincipal_Menu *Menu = NULL;
	gchar *sNome = NULL, *sTemp = NULL;
	gint nTab = 0;

	if (!Sintaxe)
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//nome
	if (Sintaxe->Tabela->sNomeUsual)
		sNome = BE_ISO_UTF8 (Sintaxe->Tabela->sNomeUsual);
	else if (Sintaxe->Tabela->sNomeReal)
		sNome = BE_ISO_UTF8 (Sintaxe->Tabela->sNomeReal);
	else
		sNome = BE_ISO_UTF8 (Sintaxe->Tabela->sNome);

	//loop nos menus, localizar transação
	for (ListaL = Janela->Menus; ListaL; ListaL = g_slist_next (ListaL))
	{
		Menu = (_BEPrincipal_Menu *) ListaL->data;
		//inserir código da transação
		if (Menu->sVisao && Menu->sTransacao && g_str_equal (Sintaxe->Tabela->sNome, Menu->sVisao))
		{
			BE_free (sTemp);
			sTemp = sNome;

			BE_free (Menu->sID);
			//loop dos filtros, localizar ID
			for (ListaL = Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
			{
				Filtro = (BESQL_Filtro *) ListaL->data;
				if (Filtro->bAtivo && g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_IGUAL) && g_str_equal (Filtro->Campo->sNome, "ID"))
				{
					Menu->sID = g_strdup (Filtro->sValor);
					break;
				}
			}
			//definir nome
			if (Menu->sID)
				sNome = g_strdup_printf ("%s [%s%s]", sTemp, Menu->sTransacao, Menu->sID);
			else
				sNome = g_strdup_printf ("%s [%s]", sTemp, Menu->sTransacao);
			break;
		}
	}
	//tabela padrão
	if (nTipo == 0)
	{
        //ordenar por AutoIncremento (ID) decrescente
        Campo = be_sql_selecionar_campo_id (Sintaxe);
        be_sql_ordem_adicionar (Sintaxe, Campo, BESQL_ORDENACAO_DESC);
		//abrir tabela
		janela = be_Tabela (Sintaxe, Janela->SintaxeLog, Janela->SintaxePerfil, Janela->window, BESQL_ACESSO_COMPLETO, TRUE, BEPRINCIPAL_ARQUIVO_LISTA_PADRAO, BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO, sArquivo_Fontes, Janela->sLerPDF, Janela->Empresa, FALSE, TRUE, _be_Principal_callback_Formulario1, _be_Principal_callback_Tabela1, Janela);
	}
	//formulário padrão
	else if (nTipo == 1)
	{
		janela = be_Formulario (Sintaxe, Janela->SintaxeLog, Janela->SintaxePerfil, Janela->window, BESQL_ACESSO_COMPLETO, TRUE, BEPRINCIPAL_ARQUIVO_LISTA_PADRAO, BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO, sArquivo_Fontes, Janela->sLerPDF, Janela->Empresa, FALSE, _be_Principal_callback_SubTabela1, be_Tabela_atualizar, _be_Principal_callback_Tabela1, _be_Principal_callback_botao_executavel, Janela);
	}
	//acrescentar no notebook, abas
	if (janela)
	{
		nTab = gtk_notebook_append_page (GTK_NOTEBOOK (Janela->tabulador), janela, NULL);
		gtk_notebook_set_current_page (GTK_NOTEBOOK (Janela->tabulador), nTab);
		gtk_notebook_set_tab_label_text (GTK_NOTEBOOK (Janela->tabulador), janela, sNome);
		gtk_notebook_set_menu_label_text (GTK_NOTEBOOK (Janela->tabulador), janela, sNome);
		//possibilidade de fechar
		gtk_widget_set_sensitive (Janela->aberta, TRUE);
        //mouse padrão
        BE_janela_mouse_normal (Mouse, FALSE);

		//foco inicial da janela filho
		if (!bFocoInicial)
		{
            gtk_widget_child_focus (janela, GTK_DIR_TAB_FORWARD);
            bFocoInicial = TRUE;
		}
	}
	else
	{
        //mouse padrão
        BE_janela_mouse_normal (Mouse, TRUE);
	}
	//limpar
	if (Menu)
	{
		BE_free (Menu->sID);
	}
	BE_free (sNome);
	BE_free (sTemp);
}

void _be_Principal_abrir_janela (_BEPrincipal_Janela *Janela, BESQL_Sintaxe *Sintaxe, gint nTipo)
{
	GtkWidget *window = NULL;
	GtkWidget *janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Filtro *Filtro = NULL;
	GSList *ListaL = NULL;
	_BEPrincipal_Menu *Menu = NULL;
	gchar *sNome = NULL, *sTemp = NULL;

	if (!Sintaxe)
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//nome
	if (Sintaxe->Tabela->sNomeUsual)
		sNome = BE_ISO_UTF8 (Sintaxe->Tabela->sNomeUsual);
	else if (Sintaxe->Tabela->sNomeReal)
		sNome = BE_ISO_UTF8 (Sintaxe->Tabela->sNomeReal);
	else
		sNome = BE_ISO_UTF8 (Sintaxe->Tabela->sNome);
	//loop nos menus, localizar transação
	for (ListaL = Janela->Menus; ListaL; ListaL = g_slist_next (ListaL))
	{
		Menu = (_BEPrincipal_Menu *) ListaL->data;
		//inserir código da transação
		if (Menu->sVisao && Menu->sTransacao && g_str_equal (Sintaxe->Tabela->sNome, Menu->sVisao))
		{
			BE_free (sTemp);
			sTemp = sNome;

            BE_free (Menu->sID);
            //loop dos filtros, localizar ID
            for (ListaL = Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
            {
                Filtro = (BESQL_Filtro *) ListaL->data;
                if (Filtro->bAtivo && g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_IGUAL) && g_str_equal (Filtro->Campo->sNome, "ID"))
                {
                    Menu->sID = g_strdup (Filtro->sValor);
                    break;
                }
            }
            //definir nome
			if (Menu->sID)
				sNome = g_strdup_printf ("%s [%s%s]", sTemp, Menu->sTransacao, Menu->sID);
			else
				sNome = g_strdup_printf ("%s [%s]", sTemp, Menu->sTransacao);
			break;
		}
	}

	//criar nova janela
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), sNome);
	gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW (window));
	//gtk_window_set_icon_from_file (GTK_WINDOW (window), "../Interfaces/janela.png", NULL);//ícone

	//tabela padrão
	if (nTipo == 0)
	{
		//abrir tabela
		janela = be_Tabela (Sintaxe, Janela->SintaxeLog, Janela->SintaxePerfil, window, BESQL_ACESSO_COMPLETO, TRUE, BEPRINCIPAL_ARQUIVO_LISTA_PADRAO, BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO, sArquivo_Fontes, Janela->sLerPDF, Janela->Empresa, FALSE, TRUE, _be_Principal_callback_Formulario1, _be_Principal_callback_Tabela1, Janela);
	}
	//formulário padrão
	else if (nTipo == 1)
	{
		//abrir formulário
		janela = be_Formulario (Sintaxe, Janela->SintaxeLog, Janela->SintaxePerfil, window, BESQL_ACESSO_COMPLETO, TRUE, BEPRINCIPAL_ARQUIVO_LISTA_PADRAO, BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO, sArquivo_Fontes, Janela->sLerPDF, Janela->Empresa, FALSE, _be_Principal_callback_SubTabela1, be_Tabela_atualizar, _be_Principal_callback_Tabela1, _be_Principal_callback_botao_executavel, Janela);
	}
	if (janela)
	{
		//anexar
		gtk_container_add (GTK_CONTAINER (window), janela);
		//exibir
		gtk_widget_show (window);
        //mouse padrão
        BE_janela_mouse_normal (Mouse, FALSE);
	}
	else
	{
		gtk_widget_destroy (window);
        //mouse padrão
        BE_janela_mouse_normal (Mouse, TRUE);
	}

	//foco na nova janela
	if (janela)
        gtk_widget_grab_focus (janela);

	BE_free (Menu->sID);
	BE_free (sNome);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarConfig_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	GSList *Propriedades = NULL;
	gchar *sTemp = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	//propriedades
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Leitor de PDF", &Janela->sLerPDF, NULL);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Leitor de imagens PNG", &Janela->sLerPNG, NULL);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Navegador de internet", &Janela->sNavegador, NULL);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Envio de e-mail", &Janela->sEnviarEmail, NULL);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Editor de imagens PNG", &Janela->sEditarPNG, NULL);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Editor de texto", &Janela->sEditarTexto, NULL);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Calculadora", &Janela->sCalculadora, NULL);
	if (be_Propriedades (GTK_WINDOW (Janela->window), Propriedades))
	{
		//salvar alterações
		_be_Principal_configuracoes_local (Janela, TRUE);
		//verificar e avisar
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Deseja verificar a disponibilidade de cada aplicativo ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) == GTK_RESPONSE_YES)
		{
			#ifdef G_OS_UNIX
				if (Janela->sLerPDF)
				{
				    if (!g_spawn_command_line_async (Janela->sLerPDF, NULL))
				        be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o leitor de PDF.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
				    else
				        be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Leitor de PDF OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sLerPNG)
				{
				    if (!g_spawn_command_line_async (Janela->sLerPNG, NULL))
				        be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o leitor de PNG.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
				    else
				        be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Leitor de PNG OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
			    if (Janela->sNavegador)
				{
					if (!g_spawn_command_line_async (Janela->sNavegador, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o nevagador de internet.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Navegador de internet OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sEnviarEmail)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("%s emailto:testes@testes.com.br", Janela->sEnviarEmail);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir envio de e-mail.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Envio de e-mail OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
		        if (Janela->sEditarPNG)
		        {
		            if (!g_spawn_command_line_async (Janela->sEditarPNG, NULL))
		                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o editor de PNG.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		            else
		                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Editor de PNG OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		        }
				if (Janela->sEditarTexto)
				{
		            if (!g_spawn_command_line_async (Janela->sEditarTexto, NULL))
		                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o editor de texto.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		            else
		                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Editor de texto OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sCalculadora)
				{
		            if (!g_spawn_command_line_async (Janela->sCalculadora, NULL))
		                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir a calculadora.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		            else
		                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Calculadora OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
			#endif
			#ifdef G_OS_WIN32
				if (Janela->sLerPDF)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\"", Janela->sLerPDF);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o leitor de PDF.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Leitor de PDF OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sLerPNG)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\"", Janela->sLerPNG);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o leitor de PNG.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Leitor de PNG OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sNavegador)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\"", Janela->sNavegador);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o nevagador de internet.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Navegador de internet OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sEnviarEmail)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\" emailto:testes@testes.com.br", Janela->sEnviarEmail);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir envio de e-mail.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Envio de e-mail OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sEditarPNG)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\"", Janela->sEditarPNG);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o editor de PNG.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Editor de PNG OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sEditarTexto)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\"", Janela->sEditarTexto);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o editor de texto.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Editor de texto OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
				if (Janela->sCalculadora)
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("\"%s\"", Janela->sCalculadora);
					if (!g_spawn_command_line_async (sTemp, NULL))
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir a calculadora.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					else
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Calculadora OK.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
				}
			#endif
		}
	}
	BE_slist_free (Propriedades);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarServidor_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	gchar *sErros = NULL;
	GString *Temp = NULL;
	gchar *sTemp = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;
	//informações do servidor
	Temp = g_string_new ("<span size='large'><b>MySQL</b></span>\n<i>MySQL AB</i>\n");
	//versão servidor
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_SERVIDOR_VERSAO, &sErros);
	g_string_append_printf (Temp, "\n<i>servidor: </i><span foreground='#0000FF'>%s</span>", sTemp);
	//ip servidor
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_SERVIDOR_IP, &sErros);
	g_string_append_printf (Temp, "\n<i>endereço: </i><span foreground='#0000FF'>%s:", sTemp);
	//porta servidor
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_SERVIDOR_PORTA, &sErros);
	g_string_append_printf (Temp, "%s</span>", sTemp);
	//linguagem
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_LINGUAGEM, &sErros);
	g_string_append_printf (Temp, "\n<i>linguagem: </i><span foreground='#0000FF'>%s</span>", sTemp);
	//compilação
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_COMPILACAO, &sErros);
	g_string_append_printf (Temp, "\n<i>compilação: </i><span foreground='#0000FF'>%s</span>", sTemp);
	//desempenho
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_DESEMPENHO, &sErros);
	g_string_append_printf (Temp, "\n<i>desempenho: </i><span foreground='#0000FF'>%sus</span>", sTemp);
	//fuso horário
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_FUSO_HORARIO, &sErros);
	g_string_append_printf (Temp, "\n<i>fuso horário: </i><span foreground='#0000FF'>%s</span>", sTemp);
	//diferença de minutos, entre cliente e servidor (segundos)
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_DIFERENCA_HORARIO, &sErros);
	g_string_append_printf (Temp, "\n<i>atraso: </i><span foreground='#0000FF'>%smin</span>", sTemp);
	//protocolo
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_PROTOCOLO, &sErros);
	g_string_append_printf (Temp, "\n<i>protocolo: </i><span foreground='#0000FF'>%s</span>", sTemp);

	g_string_append_printf (Temp, "\n");

	//versão cliente
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_CLIENTE_VERSAO, &sErros);
	g_string_append_printf (Temp, "\n<i>cliente: </i><span foreground='#0000FF'>%s</span>", sTemp);
	//ip cliente
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_CLIENTE_IP, &sErros);
	g_string_append_printf (Temp, "\n<i>acesso: </i><span foreground='#0000FF'>%s:", sTemp);
	//conexão cliente
	BE_free (sTemp);
	sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_CONEXAO, &sErros);
	g_string_append_printf (Temp, "%s</span>", sTemp);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	//exibir
	sTemp = BE_string_free (Temp);
	be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);

	BE_free (sTemp);
}

void _be_Interface_sinal_windowPrincipal_on_menubarAjuda_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;
	//executar ação, como se fosse um botão executável
	_be_Principal_callback_botao_executavel ("PDF manual_usuario.pdf", NULL, Janela);
}

void _be_Interface_sinal_windowPrincipal_on_menubarSobre_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	const gchar *authors[] = {
		"Luciano Trindade Pereira <luc_trindade@yahoo.com.br>",
		NULL
	};
	gchar *documenters[12];
	gchar *sTemp = NULL, *sComentarios = NULL, *sCreditos = NULL;
	gint nCont = 0;

	Janela = (_BEPrincipal_Janela *) user_data;

	BE_free (sTemp);
	sTemp = g_strdup_printf ("\n\n\n\n\n\n\n%s\né um Planejador de Recursos Empresariais (ERP).\nAtravés de um modelo abstrato é possível alta  disponibilidade, escalabilidade, segurança, padronização, especificação e personalização, à um baixo custo.", INTERNAL_NAME);
	sComentarios = BE_ISO_UTF8 (sTemp);

	//documentação
	documenters[0] = g_strdup_printf ("GNU GCC %d.%d.%d <www.gnu.org>", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
	documenters[1] = g_strdup_printf ("GLIB %d.%d.%d <www.gtk.org>", GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
	documenters[2] = g_strdup_printf ("GTK %d.%d.%d <www.gtk.org>", GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
	documenters[3] = g_strdup_printf ("MySQL %s <www.mysql.com>", MYSQL_SERVER_VERSION);
	documenters[4] = g_strdup_printf ("zLib %s <www.zlib.net>", ZLIB_VERSION);
	//documenters[5] = g_strdup_printf ("%s <www.openssl.org>", OPENSSL_VERSION_TEXT);
	documenters[5] = NULL;
	documenters[6] = g_strdup_printf ("Freetype %d.%d.%d <www.freetype.org>", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
	documenters[7] = g_strdup_printf ("Adobe PDF %.1f <www.adobe.com>", BEPDF_ADOBE_VERSAO);
	//documenters[8] = g_strdup_printf ("libcURL %s <curl.haxx.se>", LIBCURL_VERSION);
	documenters[8] = NULL;
	documenters[9] = g_strdup_printf ("libxml %s <xmlsoft.org>", LIBXML_DOTTED_VERSION);
	#ifdef G_OS_UNIX
		documenters[10] = NULL;
	#endif
	#ifdef G_OS_WIN32
		documenters[10] = g_strdup ("Dev-C++ 4.9.9.2 <www.bloodshed.net/devcpp.html>");
	#endif
	documenters[11] = NULL;

	sCreditos = BE_ISO_UTF8 (LEGAL_COPYRIGHT);
	be_Sobre (GTK_WINDOW (Janela->window), PRODUCT_NAME, PRODUCT_VERSION, sCreditos, "http://sourceforge.net/projects/ciaerp/", sComentarios, NULL, (const gchar **) authors, (const gchar **) documenters);
	//www.soltecsoft.com.br

	//limpar
	for (nCont = 0; nCont < 12; nCont++)
	{
		BE_free (documenters[nCont]);
	}
	BE_free (sTemp);
	BE_free (sComentarios);
	BE_free (sCreditos);
}

gboolean _be_Principal_on_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data)//fechar
{
	_BEPrincipal_Janela *Janela = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;
	_be_Principal_sair (Janela, TRUE);

	return TRUE;
}

void _be_Principal_callback_botao_executavel (const gchar *sComando, const gchar *sParametros, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sNovoComando = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	if (g_strstr_len (sComando, 3, "PDF") && Janela->sLerPDF)
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s%s", Janela->sLerPDF, sComando+3);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"%s", Janela->sLerPDF, sComando+3);
#endif
	else if (g_strstr_len (sComando, 3, "PNG") && Janela->sLerPNG)
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s%s", Janela->sLerPNG, sComando+3);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"%s", Janela->sLerPNG, sComando+3);
#endif
	else if (g_strstr_len (sComando, 4, "SITE") && Janela->sNavegador)
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s%s", Janela->sNavegador, sComando+4);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"%s", Janela->sNavegador, sComando+4);
#endif
	else if (g_strstr_len (sComando, 5, "EMAIL") && Janela->sEnviarEmail)
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s%s", Janela->sEnviarEmail, sComando+5);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"%s", Janela->sEnviarEmail, sComando+5);
#endif
	else if (g_strstr_len (sComando, 5, "TEXTO") && Janela->sEditarTexto)
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s%s", Janela->sEditarTexto, sComando+5);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"%s", Janela->sEditarTexto, sComando+5);
#endif
	else if (g_strstr_len (sComando, 4, "CALC") && Janela->sCalculadora)
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s%s", Janela->sCalculadora, sComando+4);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"%s", Janela->sCalculadora, sComando+4);
#endif
	else
#ifdef G_OS_UNIX
		sNovoComando = g_strdup_printf ("%s", sComando);
#endif
#ifdef G_OS_WIN32
		sNovoComando = g_strdup_printf ("\"%s\"", sComando);
#endif
	//preparando comando
	if (sParametros)
		sTemp = be_strreplace (sNovoComando, "%s", sParametros);
	else
		sTemp = g_strdup (sNovoComando);
	//executar comando
	if (!g_spawn_command_line_async (sTemp, NULL))
	{
		sTemp1 = g_strdup_printf ("Não foi possível executar: %s", sTemp);
		be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp1, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
	}

	//mouse padrão
	BE_janela_mouse_normal (Mouse, FALSE);

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sNovoComando);
}

void _be_Principal_callback_Tabela1 (BESQL_Tabela *Tabela, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sErros = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);
	//consulta
	Sintaxe = be_sql_abrir_consulta (Tabela, TRUE, &sErros);
	//abrir janela
	if (Janela->bJanelaSeparada)
		_be_Principal_abrir_janela (Janela, Sintaxe, 0);
	else
		_be_Principal_abrir_janela_interna (Janela, Sintaxe, 0);
	//mouse padrão
	BE_janela_mouse_normal (Mouse, FALSE);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
}

void _be_Principal_callback_Formulario1 (BESQL_Tabela *Tabela, GSList *Filtros, gpointer user_data)
{
	_BEPrincipal_Janela *Janela = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sErros = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;
	//abrir formulário somente se houver um filtro
	if (g_slist_length (Filtros) > 0)
	{
		//mouse
		Mouse = be_janela_mouse_pensando (Janela->window);
		//consulta
		Sintaxe = be_sql_abrir_consulta (Tabela, TRUE, &sErros);
		//aplicar filtro
		if (Sintaxe)
			Sintaxe->Filtros = be_sql_filtro_duplicar_lista (Filtros, Sintaxe);
		//abrir janela
		if (Janela->bJanelaSeparada)
			_be_Principal_abrir_janela (Janela, Sintaxe, 1);
		else
			_be_Principal_abrir_janela_interna (Janela, Sintaxe, 1);
        //mouse padrão
        BE_janela_mouse_normal (Mouse, FALSE);
	}

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
}

GtkWidget *_be_Principal_callback_SubTabela1 (
	BESQL_Sintaxe *Sintaxe,
	BESQL_Sintaxe *SintaxeLog,
	BESQL_Sintaxe *SintaxePerfil,
	GtkWidget *window,
	BESQL_ACESSO_ENUM nFlags,
	gboolean bOpcoes,
	gboolean bLog,
	gboolean bAtualizar,
	gpointer user_data
)
{
	_BEPrincipal_Janela *Janela = NULL;

	Janela = (_BEPrincipal_Janela *) user_data;

	return be_Tabela (Sintaxe, SintaxeLog, SintaxePerfil, window, nFlags, bOpcoes, BEPRINCIPAL_ARQUIVO_LISTA_PADRAO, BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO, sArquivo_Fontes, Janela->sLerPDF, Janela->Empresa, bLog, bAtualizar, _be_Principal_callback_Formulario1, _be_Principal_callback_Tabela1, Janela);
}
