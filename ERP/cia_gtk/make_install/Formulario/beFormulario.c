#include "beFormulario.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowFormulario.c"

GtkWidget *be_Formulario (
	BESQL_Sintaxe *Sintaxe,
	BESQL_Sintaxe *SintaxeLog,
	BESQL_Sintaxe *SintaxePerfil,
	GtkWidget *window,
	BESQL_ACESSO_ENUM nFlags,
	gboolean bOpcoes,
	const gchar *sArquivoSVG_ListaPadrao,
	const gchar *sArquivoSVG_FormularioPadrao,
	const gchar *sArquivoFonte,
	const gchar *sLeitorPDF,
	const BEGLOBAL_Empresa_Cliente *Empresa,
	gboolean bLog,
	GtkWidget *(*func_Subtabela)
		(BESQL_Sintaxe *Sintaxe,
		BESQL_Sintaxe *SintaxeLog,
		BESQL_Sintaxe *SintaxePerfil,
		GtkWidget *window,
		BESQL_ACESSO_ENUM nFlags,
		gboolean bOpcoes,
		gboolean bLog,
		gboolean bAtualizar,
		gpointer user_data),
	gboolean (*func_Subtabela_atualizar)
		(GtkWidget *Subtabela),
	void (*func_Tabela)
		(BESQL_Tabela *Tabela,
		gpointer user_data),
	void (*func_Executavel)
		(const gchar *sComando,
		const gchar *sParametros,
		gpointer user_data),
	gpointer user_data
	)
{
	_BEFormulario_Janela *Janela = NULL;
	GtkAccelGroup *accel;
	gchar *sErros = NULL;

	//preparar teclas de atalho
	accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (window), accel);
	//objetos da janela
	Janela = g_new (_BEFormulario_Janela, 1);
	Janela->window = window;
	Janela->formulario = be_Interface_criar_windowFormulario_tableFormulario (window, accel, NULL, Janela, NULL);//carregar tabela
	Janela->tabulador = BE_interfaces_widget (Janela->window, "notebookFormulario");
	gtk_notebook_remove_page (GTK_NOTEBOOK (Janela->tabulador), 0);//excluir o primeiro
	Janela->tabela = BE_interfaces_widget (Janela->window, "tableFormulario2");
	Janela->adicionar = BE_interfaces_widget (Janela->window, "toolbuttonAdicionar");
	Janela->remover = BE_interfaces_widget (Janela->window, "toolbuttonRemover");
	Janela->salvar = BE_interfaces_widget (Janela->window, "toolbuttonSalvar");
	Janela->bConfAdicionar = TRUE;
	Janela->bConfRemover = TRUE;
	Janela->bConfSalvar = TRUE;
	Janela->bPadraoAutomatico = TRUE;
	Janela->dados = BE_interfaces_widget (Janela->window, "toolbuttonDados");
	Janela->relatorios = BE_interfaces_widget (Janela->window, "toolbuttonRelatorios");
	Janela->opcoes = BE_interfaces_widget (Janela->window, "toolbuttonOpcoes");
	Janela->perfil = BE_interfaces_widget (Janela->window, "toolbuttonPerfil");
	Janela->Sintaxe = Sintaxe;//consulta atual
	Janela->SintaxeAlterar = NULL;
	Janela->SintaxeAdicionar = NULL;
	Janela->SintaxeRemover = NULL;
	Janela->SintaxeLog = SintaxeLog;//log de ações
	Janela->SintaxePerfil = SintaxePerfil;//salvar configurações personalizadas
	Janela->CampoID = be_sql_selecionar_campo_id (Janela->Sintaxe);
	Janela->CampoAutoID = be_sql_selecionar_campo_auto_id (Janela->Sintaxe);
	Janela->sArquivoSVG_ListaPadrao = sArquivoSVG_ListaPadrao;
	Janela->sArquivoSVG_FormularioPadrao = sArquivoSVG_FormularioPadrao;
	Janela->sArquivoFonte = sArquivoFonte;
	Janela->sLeitorPDF = sLeitorPDF;
	Janela->Empresa = Empresa;
	Janela->Controles = NULL;
	Janela->func_Subtabela = func_Subtabela;
	Janela->func_Subtabela_atualizar = func_Subtabela_atualizar;
	Janela->func_Tabela = func_Tabela;
	Janela->func_Executavel = func_Executavel;
	Janela->user_data = user_data;

	//CIA
	Janela->bCIA = Janela->Sintaxe->Tabela->bCIA;
	if (Janela->bCIA)
	{
		Janela->SintaxeAlterar = be_sql_abrir_consulta (Sintaxe->alt_Tabela, TRUE, &sErros);
		Janela->SintaxeAdicionar = be_sql_abrir_consulta (Sintaxe->adi_Tabela, TRUE, &sErros);
		Janela->SintaxeRemover = be_sql_abrir_consulta (Sintaxe->exc_Tabela, TRUE, &sErros);
	}
	if (!Janela->SintaxeAlterar)
		Janela->SintaxeAlterar = Sintaxe;
	if (!Janela->SintaxeAdicionar)
		Janela->SintaxeAdicionar = Sintaxe;
	if (!Janela->SintaxeRemover)
		Janela->SintaxeRemover = Sintaxe;

	//verificar permissões, adicionar
	if (!Janela->CampoAutoID || !(Janela->SintaxeAdicionar->Tabela->nFlags & BESQL_ACESSO_SELECIONAR) || !(Janela->SintaxeAdicionar->Tabela->nFlags & BESQL_ACESSO_INSERIR) || !(nFlags & BESQL_ACESSO_INSERIR))
		gtk_widget_hide (Janela->adicionar);
	//verificar permissões, remover
	if (!(Janela->SintaxeRemover->Tabela->nFlags & BESQL_ACESSO_SELECIONAR) || !(Janela->SintaxeRemover->Tabela->nFlags & BESQL_ACESSO_APAGAR) || !(nFlags & BESQL_ACESSO_APAGAR))
		gtk_widget_hide (Janela->remover);
	//verificar permissões, alterar
	if (!(Janela->SintaxeAlterar->Tabela->nFlags & BESQL_ACESSO_SELECIONAR) || !(Janela->SintaxeAlterar->Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(nFlags & BESQL_ACESSO_ALTERAR))
		gtk_widget_hide (Janela->salvar);

	//verificar permissões em janelas que não existe
	if (Janela->bCIA)
	{
		if (Janela->SintaxeAlterar == Sintaxe)
			gtk_widget_hide (Janela->salvar);
		if (Janela->SintaxeAdicionar == Sintaxe)
			gtk_widget_hide (Janela->adicionar);
		if (Janela->SintaxeRemover == Sintaxe)
			gtk_widget_hide (Janela->remover);
	}

	//verificar índices
	if (Janela->bCIA)
	{
		if (!Janela->CampoID)
		{
			gtk_widget_hide (Janela->remover);//não remover
			gtk_widget_hide (Janela->salvar);//não salvar
		}
	}

	//esconder o separador
	if (!GTK_WIDGET_VISIBLE (Janela->adicionar) && !GTK_WIDGET_VISIBLE (Janela->remover) && !GTK_WIDGET_VISIBLE (Janela->salvar))
		gtk_widget_hide (BE_interfaces_widget (Janela->window, "separatortoolitem8"));
	//corrigir acesso para remover
	if (Janela->SintaxeRemover != Sintaxe)
	{
		BE_sql_fechar_consulta (Janela->SintaxeRemover);
		Janela->SintaxeRemover = Sintaxe;
	}
	//esconder relatórios
	if (!Janela->sArquivoFonte || !Janela->sArquivoSVG_FormularioPadrao)
		gtk_widget_hide (Janela->relatorios);

	//esconder opções
	if (!bOpcoes)
		gtk_widget_hide (Janela->opcoes);
	//esconder perfil
	if (!Janela->SintaxePerfil)
		gtk_widget_hide (Janela->perfil);
	//esconder o separador
	if (!GTK_WIDGET_VISIBLE (Janela->opcoes) && !GTK_WIDGET_VISIBLE (Janela->perfil))
		gtk_widget_hide (BE_interfaces_widget (Janela->window, "separatortoolitem9"));

	//consulta inicial
	if (!sErros)
		_be_Formulario_executar_consulta (Janela);

	//meus sinais
	g_signal_connect (Janela->formulario, "destroy", G_CALLBACK (_be_Formulario_on_destroy), Janela);//fechar tudo

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	return Janela->formulario;
}

gboolean _be_Formulario_executar_consulta (_BEFormulario_Janela *Janela)
{
	gchar *sErros = NULL;
	//selecionar linha
	if (be_sql_selecionar (Janela->Sintaxe, 1, &sErros) > 0)
	{
		//informação
		if (be_sql_selecionar_proxima_linha (Janela->Sintaxe) > 0)
		{
			//somente leitura
			if (!GTK_WIDGET_VISIBLE (Janela->salvar) || !GTK_WIDGET_VISIBLE (Janela->remover))
			{
				//não fazer nada
			}
			else
			{
				//habilitar salvar e remover
				gtk_widget_set_sensitive (Janela->remover, TRUE);
			}
			//habilitar outros campos
			gtk_widget_set_sensitive (Janela->dados, TRUE);
			gtk_widget_set_sensitive (Janela->relatorios, TRUE);
			gtk_widget_set_sensitive (Janela->opcoes, TRUE);
			gtk_widget_set_sensitive (Janela->perfil, TRUE);
			//exibir
			_be_Formulario_criar (Janela);
			_be_Formulario_atualizar (Janela);

			return TRUE;
		}
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	//desabilitar outros campos
	gtk_widget_set_sensitive (Janela->remover, FALSE);
	gtk_widget_set_sensitive (Janela->dados, FALSE);
	gtk_widget_set_sensitive (Janela->relatorios, FALSE);
	gtk_widget_set_sensitive (Janela->opcoes, FALSE);
	gtk_widget_set_sensitive (Janela->perfil, FALSE);
	//acultar
	_be_Formulario_destruir (Janela);

	return FALSE;
}

void _be_Formulario_criar (_BEFormulario_Janela *Janela)
{
	BEGlobal_Janela *Mouse = NULL;
	GtkWidget *scroll = NULL;
	GtkWidget *viewport = NULL;
	GtkWidget *table = NULL;
	GtkWidget *foco = NULL;
	GdkColor color;
	GtkTooltips *dica = NULL;
	_BEFormulario_Controle *Controle = NULL;
	BESQL_Campo *Campo = NULL, *CampoAlterar = NULL;
	GSList *ListaL = NULL, *Grupos = NULL;
	const gchar *sGrupoAnterior = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCont = 0, nCont1 = 0, nCont2 = 0, nTab = 0, nCol = 0, nColMax = 0, nLin = 0;
	gboolean bSomenteLeitura = FALSE;

	//não tentar criar novamente
	if (Janela->Controles)
		return;

	//verificar resolução da tela para definir colunas
	nCont = gdk_screen_get_width (gdk_screen_get_default ());
	//gtk_window_get_size (GTK_WINDOW (Janela->window), &nCont, NULL);
	if (nCont > 1024)
		nColMax = 2 * 5;//acima
	if (nCont > 800)
		nColMax = 2 * 4;//1024 * 768
	else if (nCont > 640)
		nColMax = 2 * 3;//800 x 600
	else
		nColMax = 2 * 2;//640 x 480

	//nColMax = 2 * 3;//somente para testes

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//copiar campos
	Grupos = g_slist_copy (Janela->Sintaxe->Campos);
	//ordenar grupos
	Grupos = g_slist_sort (Grupos, _be_Formulario_ordenar_lista);

	//tabela inicial, sem grupo
	table = Janela->tabela;
	//iniciar valores
	nCol = 0;
	nLin = 0;
	//exibir
	gtk_widget_show (table);

	//loop nos campos, por ordem de agrupamento
	for (ListaL = Grupos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)//somente os ativos
		{
			//separar grupos
			if ((sGrupoAnterior && Campo->sGrupo && !g_str_equal (sGrupoAnterior, Campo->sGrupo)) || (Campo->sGrupo && !sGrupoAnterior))
			{
				//novo scroll
				scroll = gtk_scrolled_window_new (NULL, NULL);
				gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
				gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scroll), GTK_SHADOW_NONE);
				//criar tabela inicial, grupos
				table = gtk_table_new (1, 2, FALSE);
				gtk_table_set_row_spacings (GTK_TABLE (table), 2);
				gtk_table_set_col_spacings (GTK_TABLE (table), 2);
				gtk_container_set_border_width (GTK_CONTAINER (table), 2);
				//anexar tabela no scroll
				gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scroll), table);
				//acessar viewport
				viewport = gtk_widget_get_parent (table);
				gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport), GTK_SHADOW_NONE);
				//acrescentar no tabulador
				nTab = gtk_notebook_append_page (GTK_NOTEBOOK (Janela->tabulador), scroll, NULL);
				gtk_notebook_set_current_page (GTK_NOTEBOOK (Janela->tabulador), nTab);
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sGrupo);
				gtk_notebook_set_tab_label_text (GTK_NOTEBOOK (Janela->tabulador), scroll, sTemp);
				gtk_notebook_set_menu_label_text (GTK_NOTEBOOK (Janela->tabulador), scroll, sTemp);
				//exibir
				gtk_widget_show_all (scroll);
				//iniciar valores
				nCol = 0;
				nLin = 0;
			}
			sGrupoAnterior = Campo->sGrupo;

			//verificar acesso
			bSomenteLeitura = FALSE;
			if (!GTK_WIDGET_VISIBLE (Janela->salvar))
				bSomenteLeitura = TRUE;
			else if (Janela->bCIA)
			{
				CampoAlterar = be_sql_selecionar_campo (Janela->SintaxeAlterar, Campo->sNome);
				if (CampoAlterar)
				{
					if (g_str_equal (CampoAlterar->sNome, "ID") || g_str_equal (CampoAlterar->sNome, "Status") || g_str_equal (CampoAlterar->sNome, "Data"))
						bSomenteLeitura = TRUE;
					else if (!CampoAlterar->bAlteravel || g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
						bSomenteLeitura = TRUE;
				}
				else
					bSomenteLeitura = TRUE;
			}
			else if (!Campo->bAlteravel)
				bSomenteLeitura = TRUE;
			if (Campo->sParametros && !(Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE || Campo->nTipo == BESQL_TIPO_EXE))
				bSomenteLeitura = TRUE;

			//criar tipo
			Controle = g_new (_BEFormulario_Controle, 1);
			Controle->Janela = Janela;
			Controle->Campo = Campo;
			Controle->label = NULL;
			Controle->controle = NULL;
			Controle->subcontrole1 = NULL;
			Controle->subcontrole2 = NULL;
			Controle->botao1 = NULL;
			Controle->botao2 = NULL;
			Controle->sString = NULL;
			Controle->bSomenteLeitura = bSomenteLeitura;
			Controle->Sintaxe = NULL;
			Controle->CampoFiltro = NULL;
			//redimensionar tabela
			gtk_table_resize (GTK_TABLE (table), nLin + 1, nColMax);

			//verificar tipo - redimenssionar texto longo
			if (be_sql_campo_texto (Campo) && Campo->nComprimentoMax > BEFORMULARIO_LENSTRING_ENTRY)
			{
				if (nCol > 0)
					nLin++;
				nCol = 0;
			}
			//verificar tipo - redimenssionar sub-tabela
			else if (Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros)
			{
				if (nCol > 0)
					nLin++;
				nCol = 0;
			}
			//não exibir rótulos se o grupo tiver o mesmo nome e for o primeiro da lista
			if (!(Campo->sGrupo && g_str_equal (Campo->sGrupo, Campo->sNome) && nCol == 0 && nLin == 0))
			{
				//nome, rótulo
				BE_free (sTemp);
				BE_free (sTemp1);
				sTemp1 = BE_ISO_UTF8 (Campo->sNome);
				//verificar comprimento máximo, quebrar se necessário
				if (strlen (sTemp1) > BEFORMULARIO_LENSTRING_ROTULO)
				{
					for (nCont = 0, nCont1 = 0, nCont2 = 0; nCont < strlen (sTemp1); nCont++, nCont2++)
					{
						//marcar posição do último espaço
						if (sTemp1[nCont] == ' ')
							nCont1 = nCont;
						//verificar ultrapasagem
						if (nCont2 >= BEFORMULARIO_LENSTRING_ROTULO && nCont1 > 0)
						{
							sTemp1[nCont1] = '\n';//quebrar
							nCont2 = 0;//reiniciar contagem
						}
					}
					BE_free (sTemp);
				}
				sTemp = g_strdup_printf ("<span foreground='blue' size='small'> %s</span>", sTemp1);
				Controle->label = gtk_label_new (NULL);
				gtk_label_set_use_markup (GTK_LABEL (Controle->label), TRUE);
				gtk_label_set_markup (GTK_LABEL (Controle->label), sTemp);
				gtk_label_set_justify (GTK_LABEL (Controle->label), GTK_JUSTIFY_RIGHT);
				gtk_label_set_line_wrap (GTK_LABEL (Controle->label), TRUE);
				gtk_misc_set_alignment (GTK_MISC (Controle->label), 1, 0);
				gtk_misc_set_padding (GTK_MISC (Controle->label), 0, 2);
				//exibir
				gtk_widget_show (Controle->label);
				//anexar na tabela
				if (Controle->label)
					gtk_table_attach (GTK_TABLE (table), Controle->label, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
			}

			//tipos de dados:
			//tipo combo-box, list-box, hierarquia
			if (!bSomenteLeitura && (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE))
			{
				Controle->controle = gtk_table_new (1, 1, FALSE);//lin x col
				//criar combo pela primeira vez
                Controle->subcontrole1 = gtk_combo_box_entry_new_text ();
                //atualizar lista
				//anexar
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->subcontrole1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				//sinal
				g_signal_connect (Controle->subcontrole1, "changed", G_CALLBACK (_be_Formulario_on_combo_changed), Controle);
				g_signal_connect (Controle->subcontrole1, "set-focus-child", G_CALLBACK (_be_Formulario_on_combo_set_focus_child), Controle);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//tipo datas
			else if (!bSomenteLeitura && (Campo->nTipo == BESQL_TIPO_DAT || Campo->nTipo == MYSQL_TYPE_DATE))
			{
				Controle->controle = gtk_table_new (1, 2, FALSE);//lin x col
				gtk_table_set_row_spacings (GTK_TABLE (Controle->controle), 1);
				gtk_table_set_col_spacings (GTK_TABLE (Controle->controle), 1);
				Controle->subcontrole1 = gtk_entry_new ();
				gtk_entry_set_max_length (GTK_ENTRY (Controle->subcontrole1), Campo->nComprimentoMax);
				Controle->botao1 = gtk_button_new ();
				gtk_container_add (GTK_CONTAINER (Controle->botao1), gtk_image_new_from_stock (GTK_STOCK_GO_DOWN, GTK_ICON_SIZE_SMALL_TOOLBAR));
				//dica
				dica = gtk_tooltips_new ();
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 ("Abrir calendário");
				gtk_tooltips_set_tip (dica, Controle->botao1, sTemp, sTemp);
				//anexar
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->subcontrole1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->botao1, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
				//sinal
				g_signal_connect (Controle->botao1, "clicked", G_CALLBACK (_be_Formulario_on_calendar_clicked), Controle);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//tipo lógico/booleano
			else if (Campo->nTipo == BESQL_TIPO_BOL || Campo->nTipo == MYSQL_TYPE_BIT)
			{
				Controle->controle = gtk_table_new (1, 1, FALSE);//lin x col
				Controle->subcontrole1 = gtk_check_button_new ();
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Controle->subcontrole1), FALSE);
				//somente leitura
				if (bSomenteLeitura)
					gtk_widget_set_sensitive (Controle->subcontrole1, FALSE);
				//anexar
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->subcontrole1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				//sinal
				g_signal_connect (Controle->subcontrole1, "toggled", G_CALLBACK (_be_Formulario_on_toggle_button_toggled), Janela);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//tipo executável
			else if (Campo->nTipo == BESQL_TIPO_EXE)
			{
				Controle->controle = gtk_table_new (1, 2, FALSE);//lin x col
				gtk_table_set_row_spacings (GTK_TABLE (Controle->controle), 1);
				gtk_table_set_col_spacings (GTK_TABLE (Controle->controle), 1);
				Controle->subcontrole1 = gtk_entry_new ();
				gtk_entry_set_max_length (GTK_ENTRY (Controle->subcontrole1), Campo->nComprimentoMax);
				//somente leitura
				if (bSomenteLeitura)
				{
					gtk_editable_set_editable (GTK_EDITABLE (Controle->subcontrole1), FALSE);
					//alterar cor de fundo
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA, &color);
					gtk_widget_modify_base (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
					//alterar cor da fonte
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA_FONTE, &color);
					gtk_widget_modify_text (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
				}
				Controle->botao1 = gtk_button_new ();
				gtk_container_add (GTK_CONTAINER (Controle->botao1), gtk_image_new_from_stock (GTK_STOCK_EXECUTE, GTK_ICON_SIZE_SMALL_TOOLBAR));
				dica = gtk_tooltips_new ();
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 ("Executar aplicativo");
				gtk_tooltips_set_tip (dica, Controle->botao1, sTemp, sTemp);
				//anexar
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->subcontrole1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->botao1, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
				//sinal
				g_signal_connect (Controle->botao1, "clicked", G_CALLBACK (_be_Formulario_on_executar_clicked), Controle);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//tipo dados binário
			else if (be_sql_campo_binario (Campo))
			{
				Controle->controle = gtk_table_new (2, 1, FALSE);//lin x col
				gtk_table_set_row_spacings (GTK_TABLE (Controle->controle), 2);
				gtk_table_set_col_spacings (GTK_TABLE (Controle->controle), 2);
				//somente leitura
				if (bSomenteLeitura)
				{
					Controle->subcontrole1 = gtk_entry_new ();
					gtk_editable_set_editable (GTK_EDITABLE (Controle->subcontrole1), FALSE);
					//alterar cor de fundo
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA, &color);
					gtk_widget_modify_base (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
					//alterar cor da fonte
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA_FONTE, &color);
					gtk_widget_modify_text (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
				}
				else
				{
					Controle->subcontrole1 = gtk_combo_box_new_text ();
					//valor
					gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), "Atual");//atual
					gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), BEFORMULARIO_NENHUM_COMBO);
					gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), BEFORMULARIO_INSERIR_COMBO);
					gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), BEFORMULARIO_RECUPERAR_COMBO);
					gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
					//sinal
					g_signal_connect (Controle->subcontrole1, "changed", G_CALLBACK (_be_Formulario_on_binary_changed), Controle);
				}
				//anexar
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->subcontrole1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//tipo texto longo
			else if (be_sql_campo_texto (Campo) && Campo->nComprimentoMax > BEFORMULARIO_LENSTRING_ENTRY)
			{
				Controle->controle = gtk_scrolled_window_new (NULL, NULL);
				gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (Controle->controle), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
				gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (Controle->controle), GTK_SHADOW_IN);
				Controle->subcontrole1 = gtk_text_view_new ();
				//somente leitura
				if (bSomenteLeitura)
				{
					gtk_text_view_set_editable (GTK_TEXT_VIEW (Controle->subcontrole1), FALSE);
					//alterar cor de fundo
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA, &color);
					gtk_widget_modify_base (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
					//alterar cor da fonte
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA_FONTE, &color);
					gtk_widget_modify_text (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
				}
				else
				{
					gtk_text_view_set_editable (GTK_TEXT_VIEW (Controle->subcontrole1), TRUE);
				}
				gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (Controle->subcontrole1), GTK_WRAP_WORD_CHAR);
				gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (Controle->subcontrole1), FALSE);
				//dimensão
				gtk_widget_set_size_request (Controle->controle, BEFORMULARIO_CONTROLE_LARGURA_MIN, BEFORMULARIO_CONTROLE_TEXTOLONGO_ALTURA_MIN);
				//anexar
				gtk_container_add (GTK_CONTAINER (Controle->controle), Controle->subcontrole1);
				//sinais
				g_signal_connect (gtk_text_view_get_buffer (GTK_TEXT_VIEW (Controle->subcontrole1)), "changed", G_CALLBACK (_be_Formulario_on_text_view_changed), Janela);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//tipo subtabela
			else if (Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros)
			{
				Controle->controle = _be_Formulario_subtabela (Controle);
				//dimensão
				if (Controle->controle)
				{
					gtk_widget_set_size_request (Controle->controle, BEFORMULARIO_CONTROLE_LARGURA_MIN, BEFORMULARIO_CONTROLE_ALTURA_MIN * 2);
					//exibir
					gtk_widget_show (Controle->controle);
				}
			}
			//tipo texto simples
			else
			{
				Controle->controle = gtk_table_new (1, 1, FALSE);//lin x col
				Controle->subcontrole1 = gtk_entry_new ();
				gtk_entry_set_max_length (GTK_ENTRY (Controle->subcontrole1), Campo->nComprimentoMax);
				//somente leitura
				if (bSomenteLeitura)
				{
					gtk_editable_set_editable (GTK_EDITABLE (Controle->subcontrole1), FALSE);
					//alterar cor de fundo
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA, &color);
					gtk_widget_modify_base (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
					//alterar cor da fonte
					gdk_color_parse (BEGLOBAL_COR_BLOQUEADA_FONTE, &color);
					gtk_widget_modify_text (Controle->subcontrole1, GTK_STATE_NORMAL, &color);
				}
				//anexar
				gtk_table_attach (GTK_TABLE (Controle->controle), Controle->subcontrole1, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				//exibir
				gtk_widget_show_all (Controle->controle);
			}
			//Faltando BESQL_TIPO_RTF, BESQL_TIPO_DUP

            //foco inicial
            if (!foco && Controle->subcontrole1 && (GTK_IS_ENTRY (Controle->subcontrole1) || GTK_IS_CHECK_BUTTON (Controle->subcontrole1) || GTK_IS_COMBO_BOX (Controle->subcontrole1) || GTK_IS_TEXT_VIEW (Controle->subcontrole1)))
                foco = Controle->subcontrole1;

			//verificar tipo - dar espaço para sub-tabela e texto longo sozinho
			if (be_sql_campo_texto (Campo) && Campo->nComprimentoMax > BEFORMULARIO_LENSTRING_ENTRY && Controle->controle)
			{
				//anexar na tabela
				if (Controle->label)
					gtk_table_attach (GTK_TABLE (table), Controle->controle, 1, nColMax + 1, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_EXPAND | GTK_FILL, 0, 0);
				else
					gtk_table_attach (GTK_TABLE (table), Controle->controle, 0, nColMax, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_EXPAND | GTK_FILL, 0, 0);
				//avançar linha
				nLin++;
				nCol = 0;
			}
			//verificar tipo - dar espaço para sub-tabela e texto longo sozinho
			else if (Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros && Controle->controle)
			{
				//anexar na tabela
				if (Controle->label)
					gtk_table_attach (GTK_TABLE (table), Controle->controle, 1, nColMax + 1, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_EXPAND | GTK_FILL, 0, 0);
				else
					gtk_table_attach (GTK_TABLE (table), Controle->controle, 0, nColMax, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_EXPAND | GTK_FILL, 0, 0);
				//avançar linha
				nLin++;
				nCol = 0;
			}
			else
			{
				//anexar na tabela
				if (Controle->controle)
					gtk_table_attach (GTK_TABLE (table), Controle->controle, nCol + 1, nCol + 2, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
				//avançar coluna
				nCol += 2;
				if (nCol >= nColMax)
				{
					nCol = 0;
					//avançar linha
					nLin++;
				}
			}
			//anexar controle na lista
			Janela->Controles = g_slist_append (Janela->Controles, Controle);

			if (Controle && Controle->subcontrole1)
			{
				//sinal padrão
				g_signal_connect (Controle->subcontrole1, "key_press_event", G_CALLBACK (_be_Interface_sinal_windowFormulario_on_tableFormulario_key_press_event), Janela);
				//sinal de alterações
				if (GTK_IS_EDITABLE (Controle->subcontrole1))
					g_signal_connect (Controle->subcontrole1, "changed", G_CALLBACK (_be_Formulario_on_editable_changed), Janela);
				//largura mínima padrão
				gtk_widget_set_size_request (Controle->subcontrole1, BEFORMULARIO_CONTROLE_LARGURA_MIN, -1);
			}
		}
	}
	//mouse padrão
	BE_janela_mouse_normal (Mouse, FALSE);
	//foco no primeiro controle
	if (foco)
        gtk_widget_grab_focus (foco);

	BE_free (sTemp);
	BE_free (sTemp1);
	BE_slist_free (Grupos);
}

void _be_Formulario_destruir (_BEFormulario_Janela *Janela)
{
	_BEFormulario_Controle *Controle = NULL;
	GtkWidget *Filho = NULL;
	GSList *ListaL = NULL;
	GList *Filhos = NULL, *ListaL1 = NULL;

	if (Janela->Controles)
	{
		//estrutura de controle
		for (ListaL = Janela->Controles; ListaL; ListaL = g_slist_next (ListaL))
		{
			Controle = (_BEFormulario_Controle *) ListaL->data;
			BE_free (Controle->sString);
			BE_free (Controle);
		}
		BE_slist_free (Janela->Controles);

		//procurar por filhos de tabela
		Filhos = gtk_container_get_children (GTK_CONTAINER (Janela->tabela));
		for (ListaL1 = Filhos; ListaL1; ListaL1 = g_list_next (ListaL1))
		{
			Filho = (GtkWidget *) ListaL1->data;
			gtk_widget_destroy (Filho);
		}
		BE_list_free (Filhos);
		//procurar por filhos de tabulador
		Filhos = gtk_container_get_children (GTK_CONTAINER (Janela->tabulador));
		for (ListaL1 = Filhos; ListaL1; ListaL1 = g_list_next (ListaL1))
		{
			Filho = (GtkWidget *) ListaL1->data;
			gtk_widget_destroy (Filho);
		}
		BE_list_free (Filhos);
	}
}

void _be_Formulario_atualizar (_BEFormulario_Janela *Janela)
{
	_BEFormulario_Controle *Controle = NULL;
	BESQL_Campo *Campo = NULL;
	GtkTextBuffer *buffer = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gboolean bSomenteLeitura = FALSE;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	for (ListaL = Janela->Controles; ListaL; ListaL = g_slist_next (ListaL))
	{
		Controle = (_BEFormulario_Controle *) ListaL->data;
		Campo = Controle->Campo;
		bSomenteLeitura = Controle->bSomenteLeitura;
		//tipos combo
		if (!bSomenteLeitura && (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE))
		{
			//atualizar ítens do combo
			if (Campo->sValor)
			{
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sValor);
			}
			else
				sTemp = g_strdup ("");//BEFORMULARIO_NULO
			//substituir texto atual
			gtk_combo_box_remove_text (GTK_COMBO_BOX (Controle->subcontrole1), 0);
			gtk_combo_box_insert_text (GTK_COMBO_BOX (Controle->subcontrole1), 0, sTemp);
			//selecionar o novo ítem
			gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
		}
		//tipo lógico
		else if (Campo->nTipo == BESQL_TIPO_BOL || Campo->nTipo == MYSQL_TYPE_BIT)
		{
			if (Campo->sValor && strlen (Campo->sValor) > 0 && (Campo->sValor[0] == 's' || Campo->sValor[0] == 'S' || Campo->sValor[0] == 'v' || Campo->sValor[0] == 'V' || Campo->sValor[0] == '1'))
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Controle->subcontrole1), TRUE);
			else
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Controle->subcontrole1), FALSE);
		}
		//tipo dados binários (arquivos e imagens)
		else if (be_sql_campo_binario (Campo))
		{
			BE_free (sTemp);
			BE_free (sTemp1);
			sTemp1 = be_sql_campo_valor_limitado_formatado (Campo, BEFORMULARIO_LENSTRING_BIN, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8 | BESQL_CAMPOS_LIMITAR_FORMATO_XML);
			if (!sTemp1)
				sTemp1 = g_strdup ("");
			if (Campo->nComprimento > 1073741824)
				sTemp = g_strdup_printf ("%s (%.3f GB)", sTemp1, (gfloat) Campo->nComprimento / 1073741824.0);
			else if (Campo->nComprimento > 1048576)
				sTemp = g_strdup_printf ("%s (%.2f MB)", sTemp1, (gfloat) Campo->nComprimento / 1048576.0);
			else if (Campo->nComprimento > 1024)
				sTemp = g_strdup_printf ("%s (%.1f KB)", sTemp1, (gfloat) Campo->nComprimento / 1204.0);
			else
				sTemp = g_strdup_printf ("%s (%d B)", sTemp1, Campo->nComprimento);
			//somente leitura
			if (bSomenteLeitura)
			{
				//valor
				gtk_entry_set_text (GTK_ENTRY (Controle->subcontrole1), sTemp);
			}
			else
			{
				//substituir texto atual
				gtk_combo_box_remove_text  (GTK_COMBO_BOX (Controle->subcontrole1), 0);
				gtk_combo_box_insert_text (GTK_COMBO_BOX (Controle->subcontrole1), 0, sTemp);
				//selecionar o novo ítem
				gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
			}
			//visualizador
			BE_free (Controle->sString);
			_be_Formulario_visualizar_binario (Controle);
		}
		//tipo texto longo
		else if (be_sql_campo_texto (Campo) && Campo->nComprimentoMax > BEFORMULARIO_LENSTRING_ENTRY)
		{
			if (Campo->sValor)
			{
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sValor);
			}
			else
				sTemp = g_strdup ("");//BEFORMULARIO_NULO
			//substituir texto atual
			buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (Controle->subcontrole1));
			gtk_text_buffer_set_text (buffer, sTemp, -1);
		}
		//tipo subtabela
		else if (Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros)
		{
			//limpar filtro
			be_sql_filtro_limpar (Controle->Sintaxe, TRUE);
			//preparar novo filtro
			if (Controle->CampoFiltro)
			{
				be_sql_filtro_adicionar (Controle->Sintaxe, Controle->CampoFiltro, BESQL_COMPARACAO_IGUAL, Controle->Campo->sValor, BESQL_RELACAO_E, 0, TRUE);
			}
			//atualizar
			if (Janela->func_Subtabela_atualizar && Controle->controle)
				Janela->func_Subtabela_atualizar (Controle->controle);
		}
		//tipo texto comum
		else
		{
			if (Campo->sValor)
			{
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sValor);
			}
			else
				sTemp = g_strdup ("");//BEFORMULARIO_NULO
			//substituir texto atual
			gtk_entry_set_text (GTK_ENTRY (Controle->subcontrole1), sTemp);
		}
	}
	//desbloquear subtabelas
	_be_Formulario_bloquear_subtabelas (Janela, FALSE);
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	BE_free (sTemp);
	BE_free (sTemp1);
}

gchar *_be_Formulario_localizar_valores (_BEFormulario_Janela *Janela, BESQL_Campo *CampoL)
{
	_BEFormulario_Controle *Controle = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sValor = NULL;
	const gchar *sValorGravado = NULL;

	//localizar valor para filtro
	for (ListaL = Janela->Controles; ListaL; ListaL = g_slist_next (ListaL))
	{
		Controle = (_BEFormulario_Controle *) ListaL->data;
		Campo = Controle->Campo;
		//somente campo definido
		if (g_str_equal (CampoL->sNome, Campo->sNome))
		{
			//verificar tipos de campos
			if (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE)
			{
				//verificar valor
				BE_free (sTemp);
				sTemp = gtk_combo_box_get_active_text (GTK_COMBO_BOX (Controle->subcontrole1));
				sValor = g_strdup (sTemp);
				sValorGravado = Campo->sValor;
			}
			else if (Campo->nTipo == BESQL_TIPO_BOL || Campo->nTipo == MYSQL_TYPE_BIT)
			{
				if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (Controle->subcontrole1)))
					sValor = g_strdup ("1");
				else
					sValor = g_strdup ("0");
				sValorGravado = Campo->sValor;
			}
			else
			{
				sValor = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Controle->subcontrole1)));
				sValorGravado = Campo->sValor;
			}
		}
	}
	//verificar valor inicial
	if (!sValor && sValorGravado)
		sValor = g_strdup (sValorGravado);

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);

	return sValor;
}

void _be_Formulario_combo_box_atualizar (_BEFormulario_Controle *Controle)
{
	_BEFormulario_Janela *Janela = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL, *CampoTemp = NULL, *CampoExibir = NULL, *CampoID = NULL;
	GSList *ListaL = NULL;
	gchar **aLista = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sValorAtual = NULL;
	gint nCont = 0;

	Janela = Controle->Janela;
	Campo = Controle->Campo;

	//recuperar valor gravado
	if (Campo->sValor)
		sValorAtual = BE_ISO_UTF8 (Campo->sValor);
	else
		sValorAtual = g_strdup_printf (" ");

	//remover todos ítens existentes
	for (nCont = BEFORMULARIO_LINHAS_COMBO + 4; nCont >= 0; nCont--)
        gtk_combo_box_remove_text (GTK_COMBO_BOX (Controle->subcontrole1), nCont);

	//lista de parâmetros
	if (Campo->sParametros)
	{
		aLista = g_strsplit (Campo->sParametros, "; ", -1);
			//`ID1`; `vis_Tabela2`; `ID2`; `Exibir2`. A relação (JOIN) para exibição já estará dentro dos Views.
			//(O separador é ponto-e-vírgula e espaço.)
			//aLista[0] = `ID1`
			//aLista[1] = `vis_Tabela2`
			//aLista[2] = `ID2`
			//aLista[3] = `Exibir2`
			//aLista[4] = `Filtro1`//opcional
			//aLista[5] = `Filtro2`//opcional
		if (aLista[0] && aLista[1] && aLista[2] && aLista[3])
		{
			//preparar nome da tabela
			BE_free (sTemp);
			sTemp = g_strdup (aLista[1]+1);
			sTemp[strlen (sTemp)-1] = '\0';
			//abrir consulta relacionada
			Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->Sintaxe->Tabela->BD, sTemp), TRUE, &sErros);

			if (Sintaxe && (Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
			{
				//limpar filtro
				be_sql_filtro_limpar (Sintaxe, FALSE);
				//verificar Filtro1, opcional
				if (aLista[4])
				{
					//preparar campo Filtro1
					BE_free (sTemp);
					sTemp = g_strdup (aLista[4]+1);
					sTemp[strlen (sTemp)-1] = '\0';
					CampoTemp = be_sql_selecionar_campo (Sintaxe, sTemp);
					//verificar campo válido, nas duas tabelas
					if (CampoTemp)
					{
						//localizar valor
						BE_free (sTemp);
						sTemp = _be_Formulario_localizar_valores (Janela, CampoTemp);
						//adicionar filtro
						if (sTemp && strlen(sTemp) > 0)
							be_sql_filtro_adicionar (Sintaxe, CampoTemp, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_E, 0, FALSE);
					}
				}
				//verificar Filtro2, opcional
				if (aLista[4] && aLista[5])
				{
					//preparar campo Filtro2
					BE_free (sTemp);
					sTemp = g_strdup (aLista[5]+1);
					sTemp[strlen (sTemp)-1] = '\0';
					CampoTemp = be_sql_selecionar_campo (Sintaxe, sTemp);
					//verificar campo válido, nas duas tabelas
					if (CampoTemp)
					{
						//localizar valor
						BE_free (sTemp);
						sTemp = _be_Formulario_localizar_valores (Janela, CampoTemp);
						//adicionar filtro
						if (sTemp && strlen(sTemp) > 0)
							be_sql_filtro_adicionar (Sintaxe, CampoTemp, BESQL_COMPARACAO_IGUAL, sTemp, BESQL_RELACAO_E, 0, FALSE);
					}
				}

				//desativar todos os campos
				for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
				{
					CampoTemp = (BESQL_Campo *) ListaL->data;
					CampoTemp->bAtivo = FALSE;
				}
				//preparar campo id
				BE_free (sTemp);
				sTemp = g_strdup (aLista[2]+1);
				sTemp[strlen (sTemp)-1] = '\0';
				CampoID = be_sql_selecionar_campo (Sintaxe, sTemp);
				if (CampoID)
				{
					CampoID->bAtivo = TRUE;
					//preparar campo exibir
					BE_free (sTemp);
					sTemp = g_strdup (aLista[3]+1);
					sTemp[strlen (sTemp)-1] = '\0';
					CampoExibir = be_sql_selecionar_campo (Sintaxe, sTemp);
					//dados
					if (CampoExibir)
					{
						CampoExibir->bAtivo = TRUE;
						//primeiro ítem é o valor atual
						gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), sValorAtual);
						//ordenar por exibição
						be_sql_ordem_adicionar (Sintaxe, CampoExibir, BESQL_ORDENACAO_ASC);
						//linhas
						Sintaxe->lLinhasPaginaMax = BEFORMULARIO_LINHAS_COMBO + 1;
						if (be_sql_selecionar (Sintaxe, 1, &sErros) > 0)
						{
							//loop dos dados da primeira página
							nCont = 0;
							while (be_sql_selecionar_proxima_linha (Sintaxe) && nCont < BEFORMULARIO_LINHAS_COMBO)
							{
								BE_free (sTemp);
								BE_free (sTemp1);
								sTemp1 = be_sql_campo_valor_limitado_formatado (CampoExibir, BEFORMULARIO_LENSTRING_COMBO, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
								if (sTemp1)
								{
									sTemp = g_strdup_printf ("%s", sTemp1);
									//adicionar ítem
									gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), sTemp);
								}
								nCont++;
							}
						}
						//indicar que há mais linhas
						if (nCont >= BEFORMULARIO_LINHAS_COMBO)
						{
							//indicar no fim
							gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), BEFORMULARIO_MAIS_COMBO);
						}
						//possibilidade de alterações
						gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), BEFORMULARIO_ALTERAR_COMBO);
					}
				}
			}
			//informar erros
			if (sErros)
			{
				be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
				BE_free (sErros);
			}

			BE_sql_fechar_consulta (Sintaxe);
		}
		BE_strfreev (aLista);
	}
	else if (Campo->bAnulavel && !be_sql_campo_numerico (Campo) && !be_sql_campo_logico (Campo))
	{
		//primeiro ítem é o valor atual
		gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), sValorAtual);
		//segundo ítem é nulo
		gtk_combo_box_append_text (GTK_COMBO_BOX (Controle->subcontrole1), BEFORMULARIO_NULO);
	}

	//limpar valor
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sValorAtual);
}

void _be_Formulario_on_combo_changed (GtkComboBox *widget, gpointer user_data)
{
	_BEFormulario_Controle *Controle = NULL;
	_BEFormulario_Janela *Janela = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	Controle = (_BEFormulario_Controle *) user_data;
	Janela = Controle->Janela;

	sTemp = gtk_combo_box_get_active_text (GTK_COMBO_BOX (Controle->subcontrole1));
	if (g_str_equal (sTemp, BEFORMULARIO_MAIS_COMBO))
	{
		sTemp1 = _be_Formulario_mais_combo (Janela, Controle->Campo);
		if (sTemp1)
		{
			//substituir texto atual
			gtk_combo_box_remove_text (GTK_COMBO_BOX (Controle->subcontrole1), 0);
			gtk_combo_box_insert_text (GTK_COMBO_BOX (Controle->subcontrole1), 0, sTemp1);
			//selecionar o novo ítem
			gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
		}
		else
			//selecionar o ítem atual
			gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
	}
	else if (g_str_equal (sTemp, BEFORMULARIO_ALTERAR_COMBO))
	{
		//alterar combo
		_be_Formulario_alterar_combo (Janela, Controle->Campo);
		//selecionar o ítem atual
		gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
	}

	//bloquear subtabelas
	_be_Formulario_bloquear_subtabelas (Janela, TRUE);

	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Formulario_on_combo_set_focus_child (GtkContainer *container, GtkWidget *widget, gpointer user_data)
{
	if (GTK_IS_BUTTON (widget))//ganhar foco
	{
		//atualizar lista
		_be_Formulario_combo_box_atualizar ((_BEFormulario_Controle *) user_data);
	}
	else if (GTK_IS_ENTRY (widget))//ganhar foco
	{
		//não fazer nada
	}
}

gchar *_be_Formulario_combo_box_localizar (_BEFormulario_Controle *Controle)
{
	_BEFormulario_Janela *Janela = NULL;
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	gchar **aLista = NULL;
	gchar *sTexto = NULL, *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL, *sTemp3 = NULL;
	gchar *sResultado = NULL;
	gchar *sSQL = NULL;
	gchar *sErros = NULL;

	//recuperar informações
	Janela = Controle->Janela;
	Campo = Controle->Campo;
	SQL = Janela->Sintaxe->Tabela->BD->SQL;

	sTexto = gtk_combo_box_get_active_text (GTK_COMBO_BOX (Controle->subcontrole1));
	if (Campo->sParametros)
	{
		aLista = g_strsplit (Campo->sParametros, "; ", -1);
			//`ID1`; `vis_Tabela2`; `ID2`; `Exibir2`. A relação (JOIN) para exibição já estará dentro dos Views.
			//(O separador é ponto-e-vírgula e espaço.)
			//aLista[0] = `ID1`
			//aLista[1] = `vis_Tabela2`
			//aLista[2] = `ID2`
			//aLista[3] = `Exibir2`
			//aLista[4] = `Filtro1`//opcional
			//aLista[5] = `Filtro2`//opcional
		if (aLista[0] && aLista[1] && aLista[2] && aLista[3])
		{
			//preparar nome da tabela
			BE_free (sTemp);
			sTemp = g_strdup (aLista[1]+1);
			sTemp[strlen (sTemp)-1] = '\0';
			//preparar campo id
			BE_free (sTemp1);
			sTemp1 = g_strdup (aLista[2]+1);
			sTemp1[strlen (sTemp1)-1] = '\0';
			//preparar campo exibir
			BE_free (sTemp2);
			sTemp2 = g_strdup (aLista[3]+1);
			sTemp2[strlen (sTemp2)-1] = '\0';
			//criar consulta rápida
			BE_free (sTemp3);
			sTemp3 = BE_UTF8_ISO (sTexto);
			sSQL = g_strdup_printf ("SELECT `%s` FROM `%s` WHERE `%s`='%s' LIMIT 1;", sTemp1, sTemp, sTemp2, sTemp3);
			//executar consulta
			if (be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sTemp1, &sErros) > 0)
				sResultado = g_strdup (sTemp1);//valor localizado
			else
				sResultado = g_strdup (sTexto);//valor não localizado, pode ser acesso direto com ID
			//informar erros
			if (sErros)
			{
				be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
				BE_free (sErros);
			}
		}
		BE_strfreev (aLista);
	}
	//limpar
	BE_free (sTexto);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sTemp3);
	BE_free (sSQL);

	return sResultado;
}

gchar *_be_Formulario_mais_combo (_BEFormulario_Janela *Janela, BESQL_Campo *Campo)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *CampoExibir = NULL, *CampoID = NULL, *CampoTemp = NULL;
	gchar **aLista = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL,*sResultado = NULL;

	//lista de parâmetros
	aLista = g_strsplit (Campo->sParametros, "; ", -1);
		//`ID1`; `vis_Tabela2`; `ID2`; `Exibir2`. A relação (JOIN) para exibição já estará dentro dos Views.
		//(O separador é ponto-e-vírgula e espaço.)
		//aLista[0] = `ID1`
		//aLista[1] = `vis_Tabela2`
		//aLista[2] = `ID2`
		//aLista[3] = `Exibir2`
		//aLista[4] = `Filtro1`//opcional
		//aLista[5] = `Filtro2`//opcional
	//preparar nome da tabela
	BE_free (sTemp);
	sTemp = g_strdup (aLista[1]+1);
	sTemp[strlen (sTemp)-1] = '\0';
	//abrir consulta relacionada
	Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->Sintaxe->Tabela->BD, sTemp), TRUE, &sErros);

	if (Sintaxe && (Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
	{
		//limpar filtro
		be_sql_filtro_limpar (Sintaxe, FALSE);
		//verificar Filtro1, opcional
		if (aLista[4])
		{
			//preparar campo Filtro1
			BE_free (sTemp);
			sTemp = g_strdup (aLista[4]+1);
			sTemp[strlen (sTemp)-1] = '\0';
			CampoTemp = be_sql_selecionar_campo (Sintaxe, sTemp);
			//verificar campo válido, nas duas tabelas
			if (CampoTemp)
			{
				//localizar valor
				BE_free (sTemp);
				sTemp = _be_Formulario_localizar_valores (Janela, CampoTemp);
				//adicionar filtro
				if (sTemp && strlen(sTemp) > 0)
					be_sql_filtro_adicionar (Sintaxe, CampoTemp, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_E, 0, FALSE);
			}
		}
		//verificar Filtro2, opcional
		if (aLista[4] && aLista[5])
		{
			//preparar campo Filtro2
			BE_free (sTemp);
			sTemp = g_strdup (aLista[5]+1);
			sTemp[strlen (sTemp)-1] = '\0';
			CampoTemp = be_sql_selecionar_campo (Sintaxe, sTemp);
			//verificar campo válido, nas duas tabelas
			if (CampoTemp)
			{
				//localizar valor
				BE_free (sTemp);
				sTemp = _be_Formulario_localizar_valores (Janela, CampoTemp);
				//adicionar filtro
				if (sTemp && strlen(sTemp) > 0)
					be_sql_filtro_adicionar (Sintaxe, CampoTemp, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_E, 0, FALSE);
			}
		}

		//preparar campo id
		BE_free (sTemp);
		sTemp = g_strdup (aLista[2]+1);
		sTemp[strlen (sTemp)-1] = '\0';
		CampoID = be_sql_selecionar_campo (Sintaxe, sTemp);
		//preparar campo exibir
		BE_free (sTemp);
		sTemp = g_strdup (aLista[3]+1);
		sTemp[strlen (sTemp)-1] = '\0';
		CampoExibir = be_sql_selecionar_campo (Sintaxe, sTemp);
		//ordenar por exibição
		be_sql_ordem_adicionar (Sintaxe, CampoExibir, BESQL_ORDENACAO_ASC);
		//dados
		if (CampoID && CampoExibir)
		{
			if (be_ListaG (GTK_WINDOW (Janela->window), Sintaxe, NULL))
			{
				if (CampoID->sValor && CampoExibir->sValor)
					sResultado = be_sql_campo_valor_limitado_formatado (CampoExibir, BEFORMULARIO_LENSTRING_COMBO, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
			}
		}
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	BE_sql_fechar_consulta (Sintaxe);
	BE_strfreev (aLista);
	BE_free (sTemp);

	return sResultado;
}

void _be_Formulario_alterar_combo (_BEFormulario_Janela *Janela, BESQL_Campo *Campo)
{
	BEGlobal_Janela *Mouse = NULL;
	gchar **aLista = NULL;
	gchar *sTemp = NULL;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//lista de parâmetros
	aLista = g_strsplit (Campo->sParametros, "; ", -1);
		/*
		`ID1`; `vis_Tabela2`; `ID2`; `Exibir2`. A relação (JOIN) para exibição já estará dentro dos Views.
		(O separador é ponto-e-vírgula e espaço.)
		aLista[0] = `ID1`
		aLista[1] = `vis_Tabela2`
		aLista[2] = `ID2`
		aLista[3] = `Exibir2`
		*/
	//preparar nome da tabela
	if (aLista[1])
	{
		BE_free (sTemp);
		sTemp = g_strdup (aLista[1]+1);
		sTemp[strlen (sTemp)-1] = '\0';
		//abrir tabela
		if (Janela->func_Tabela)
		{
			Janela->func_Tabela (be_sql_selecionar_tabela (Janela->Sintaxe->Tabela->BD, sTemp), Janela->user_data);
			//mouse padrão
			BE_janela_mouse_normal (Mouse, FALSE);
		}
	}
	//mouse padrão
    BE_janela_mouse_normal (Mouse, TRUE);

	BE_strfreev (aLista);
	BE_free (sTemp);
}

void _be_Formulario_on_calendar_clicked (GtkButton *button, gpointer user_data)
{
	_BEFormulario_Controle *Controle = NULL;
	_BEFormulario_Janela *Janela = NULL;
	GDate *Data = NULL, *DataNova = NULL;
	gchar sBuffer[256];

	Controle = (_BEFormulario_Controle *) user_data;
	Janela = Controle->Janela;

	//preparando a data de entrada
	Data = g_date_new ();
	g_date_set_parse (Data, gtk_entry_get_text (GTK_ENTRY (Controle->subcontrole1)));
	//janela para seleção
	DataNova = be_Calendario (GTK_WINDOW (Janela->window), Data);
	if (DataNova && g_date_valid (DataNova))
	{
		g_date_strftime (sBuffer, 256, "%d/%m/%Y", DataNova);
		gtk_entry_set_text (GTK_ENTRY (Controle->subcontrole1), sBuffer);
		BE_date_free (DataNova);
	}

	BE_date_free (Data);
}

void _be_Formulario_visualizar_binario (_BEFormulario_Controle *Controle)
{
	_BEFormulario_Janela *Janela = NULL;
	GdkPixbuf *Imagem = NULL, *Imagem1 = NULL;
	GtkWidget *viewport = NULL, *event_box = NULL, *image = NULL;
	gfloat fLargura = 0.0, fAltura = 0.0;
	gchar *sTemp = NULL;

	Janela = Controle->Janela;

	//limpar
	if (Controle->subcontrole2)
	{
		viewport = gtk_widget_get_parent (Controle->subcontrole2);
		gtk_widget_destroy (viewport);
		Controle->subcontrole2 = NULL;
	}
	//verificar e exibir imagem
	if (Controle->sString)
	{
		if (be_arquivo_localidade_UTF8 ())
		{
			sTemp = BE_ISO_UTF8 (Controle->sString);
			Imagem = gdk_pixbuf_new_from_file (sTemp, NULL);
		}
		else
			Imagem = gdk_pixbuf_new_from_file (Controle->sString, NULL);
	}
	else
		Imagem = be_sql_recuperar_arquivo_pixbuf (Controle->Campo, 'I');

	if (Imagem)
	{
		//escalonar
		fLargura = (gfloat) gdk_pixbuf_get_width (Imagem);
		fAltura = (gfloat) gdk_pixbuf_get_height (Imagem);
		be_redimensionar_proporcional (&fLargura, &fAltura, (gfloat) BEFORMULARIO_CONTROLE_LARGURA_MIN - 4, (gfloat) BEFORMULARIO_CONTROLE_ALTURA_MIN - 4, FALSE);
		Imagem1 = gdk_pixbuf_scale_simple (Imagem, (gint) fLargura, (gint) fAltura, GDK_INTERP_HYPER);
		//imagem
		viewport = gtk_viewport_new (NULL, NULL);
		gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport), GTK_SHADOW_IN);
		event_box = gtk_event_box_new ();
		image = gtk_image_new ();
		gtk_container_add (GTK_CONTAINER (event_box), image);
		//Controle->subcontrole2 = gtk_image_new ();
		Controle->subcontrole2 = event_box;
		//inserir
		gtk_image_set_from_pixbuf (GTK_IMAGE (image), Imagem1);//imagem reduzida
		//dados puro
		BE_pixbuf_free (Imagem);
		BE_pixbuf_free (Imagem1);
		//dimensão
		gtk_widget_set_size_request (Controle->subcontrole2, BEFORMULARIO_CONTROLE_LARGURA_MIN - 4, BEFORMULARIO_CONTROLE_ALTURA_MIN - 4);
		//anexar
		gtk_container_add (GTK_CONTAINER (viewport), Controle->subcontrole2);
		gtk_table_attach (GTK_TABLE (Controle->controle), viewport, 0, 1, 1, 2, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);
		//exibir
		gtk_widget_show_all (viewport);
		//sinais
		g_signal_connect (G_OBJECT (event_box), "button_press_event", G_CALLBACK (_be_Formulario_on_button_press), Controle);
	}

	BE_free (sTemp);
}

gboolean _be_Formulario_on_button_press (GtkWidget *event_box, GdkEventButton *event, gpointer data)
{
    _BEFormulario_Controle *Controle = NULL;
    _BEFormulario_Janela *Janela = NULL;
    GtkWidget *window = NULL;
    GtkWidget *image = NULL;
    GdkPixbuf *Imagem = NULL, *Imagem1 = NULL;
    BEGlobal_Janela *Mouse = NULL;
    gchar *sTemp = NULL;
    gint nLargura = 0, nAltura = 0;
    gfloat fLargura = 0.0, fAltura = 0.0;

    Controle = (_BEFormulario_Controle *) data;
    Janela = Controle->Janela;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//verificar resolução da tela para zoom
	nLargura = gdk_screen_get_width (gdk_screen_get_default ());
	nAltura = gdk_screen_get_height (gdk_screen_get_default ());

	//verificar e exibir imagem
	if (Controle->sString)
	{
		if (be_arquivo_localidade_UTF8 ())
		{
			sTemp = BE_ISO_UTF8 (Controle->sString);
			Imagem = gdk_pixbuf_new_from_file (sTemp, NULL);
		}
		else
			Imagem = gdk_pixbuf_new_from_file (Controle->sString, NULL);
	}
	else
		Imagem = be_sql_recuperar_arquivo_pixbuf (Controle->Campo, 'I');

	if (!Imagem)
	    return TRUE;

	//preparar janela de zoom
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (Janela->window));//filho
	gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	gtk_window_set_title (GTK_WINDOW (window), "Zoom");
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW (window));
	//gtk_window_set_icon_from_file (GTK_WINDOW (window), "../Interfaces/janela.png", NULL);

	//escalonar
	fLargura = (gfloat) gdk_pixbuf_get_width (Imagem);
	fAltura = (gfloat) gdk_pixbuf_get_height (Imagem);
	be_redimensionar_proporcional (&fLargura, &fAltura, (gfloat) nLargura, (gfloat) nAltura, TRUE);
	Imagem1 = gdk_pixbuf_scale_simple (Imagem, (gint) fLargura, (gint) fAltura, GDK_INTERP_HYPER);
 	//inserir
	image = gtk_image_new ();
	gtk_image_set_from_pixbuf (GTK_IMAGE (image), Imagem1);//imagem original
	BE_pixbuf_free (Imagem);//dados puro
	BE_pixbuf_free (Imagem1);//dados puro
	//anexar
	gtk_container_add (GTK_CONTAINER (window), image);

	//exibir
	gtk_widget_show_all (window);

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	return TRUE;
}

void _be_Formulario_on_binary_changed (GtkComboBox *widget, gpointer user_data)
{
	_BEFormulario_Controle *Controle = NULL;
	_BEFormulario_Janela *Janela = NULL;
	GtkWidget *viewport = NULL;
	gchar *sTemp = NULL;

	Controle = (_BEFormulario_Controle *) user_data;
	Janela = Controle->Janela;

	sTemp = gtk_combo_box_get_active_text (GTK_COMBO_BOX (Controle->subcontrole1));
	//verificar
	if (!sTemp)
		return;

	if (g_str_equal (sTemp, BEFORMULARIO_INSERIR_COMBO))
	{
		_be_Formulario_on_binary_up_clicked (Controle);
		gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
	}
	else if (g_str_equal (sTemp, BEFORMULARIO_RECUPERAR_COMBO))
	{
		_be_Formulario_on_binary_down_clicked (Controle);
		gtk_combo_box_set_active (GTK_COMBO_BOX (Controle->subcontrole1), 0);
	}
	else if (g_str_equal (sTemp, BEFORMULARIO_NENHUM_COMBO))
	{
		//ocultar visualização
		if (Controle->subcontrole2)
		{
			viewport = gtk_widget_get_parent (Controle->subcontrole2);
			gtk_widget_hide (viewport);
		}
	}
	else
	{
		//exibir visualização
		_be_Formulario_visualizar_binario (Controle);
	}

	//bloquear subtabelas
	_be_Formulario_bloquear_subtabelas (Janela, TRUE);

	BE_free (sTemp);
}

void _be_Formulario_on_editable_changed (GtkComboBox *widget, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;

	Janela = user_data;
	//bloquear subtabelas
	_be_Formulario_bloquear_subtabelas (Janela, TRUE);
}

void _be_Formulario_on_toggle_button_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;

	Janela = user_data;
	//bloquear subtabelas
	_be_Formulario_bloquear_subtabelas (Janela, TRUE);
}

void _be_Formulario_on_text_view_changed (GtkTextBuffer *textbuffer, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;

	Janela = user_data;
	//bloquear subtabelas
	_be_Formulario_bloquear_subtabelas (Janela, TRUE);
}

void _be_Formulario_on_binary_up_clicked (_BEFormulario_Controle *Controle)
{
	_BEFormulario_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	GdkPixbufFormat *Formato = NULL;
	GString *Temp = NULL;
	GSList *ListaL = NULL;
	gchar **aLista = NULL;
	gchar *sTemp = NULL, *sLocal = NULL, *sFormatos = NULL;
	gint nCont = 0;

	Janela = Controle->Janela;
	Campo = Controle->Campo;
	//preparando lista de formatos suportados
	Temp = g_string_new ("*.*(Todos)\n");
	for (ListaL = gdk_pixbuf_get_formats (); ListaL; ListaL = g_slist_next (ListaL))
	{
		Formato = (GdkPixbufFormat *) ListaL->data;
		//adicionr extensões
		aLista = gdk_pixbuf_format_get_extensions (Formato);
		for (nCont = 0; nCont < g_strv_length (aLista); nCont++)
			g_string_append_printf (Temp, "*.%s ", aLista[nCont]);
		BE_strfreev (aLista);
		//remover o último espaço
		Temp = g_string_truncate (Temp, Temp->len - 1);
		//adicionar nome da tipo
		g_string_append_printf (Temp, "(%s)\n", gdk_pixbuf_format_get_description (Formato));
	}
	//remover o último \n
	Temp = g_string_truncate (Temp, Temp->len - 1);
	sFormatos = BE_string_free (Temp);
	//preparar local inicial
	sLocal = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, g_get_user_name ());

	//selecionar arquivo
	if ((sTemp = be_Abrir (GTK_WINDOW (Janela->window), be_arquivo_localidade_UTF8 (), sFormatos, sLocal)))
	{
		//carregar nome do arquivo
		BE_free (Controle->sString);
		//conversão
		Controle->sString = BE_UTF8_ISO (sTemp);
		//verificar comprimento do arquivo
		nCont = be_arquivo_tamanho (Controle->sString);
		if (nCont > Campo->nComprimentoMax)
		{
			BE_free (sTemp);
			if (Campo->nComprimentoMax > 1073741824)
				sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %.3f GB", (gfloat) Campo->nComprimentoMax / 1073741824.0);
			else if (Campo->nComprimentoMax > 1048576)
				sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %.2f MB", (gfloat) Campo->nComprimentoMax / 1048576.0);
			else if (Campo->nComprimentoMax > 1024)
				sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %.1f KB", (gfloat) Campo->nComprimentoMax / 1204.0);
			else
				sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %d B", Campo->nComprimentoMax);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_free (Controle->sString);
		}
		else if (nCont > BEGLOBAL_TAMANHO_MAX_ARQUIVO)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "O arquivo selecionado é grande demais.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_free (Controle->sString);
		}
		else if (nCont > 0)
		{
			//alterar combo
			BE_free (sLocal);
			sLocal = g_path_get_basename (sTemp);
			gtk_combo_box_remove_text (GTK_COMBO_BOX (Controle->subcontrole1), 0);
			gtk_combo_box_insert_text (GTK_COMBO_BOX (Controle->subcontrole1), 0, sLocal);
			//alterar visualização
			_be_Formulario_visualizar_binario (Controle);
		}
		else
		{
		  BE_free (Controle->sString);
        }
	}

	BE_free (sTemp);
	BE_free (sLocal);
	BE_free (sFormatos);
}

void _be_Formulario_on_binary_down_clicked (_BEFormulario_Controle *Controle)
{
	_BEFormulario_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	GdkPixbufFormat *Formato = NULL;
	GdkPixbuf *Imagem = NULL;
	GString *Temp = NULL;
	GSList *ListaL = NULL;
	gchar **aLista = NULL;
	gchar *sTemp = NULL, *sLocal = NULL, *sFormatos = NULL;
	gint nCont = 0;

	Janela = Controle->Janela;
	Campo = Controle->Campo;
	if (!Campo->sValor || Campo->nComprimento == 0)
		return;

	//preparando lista de formatos suportados para salvar
	Temp = g_string_new ("*.*(Qualquer)\n");
	for (ListaL = gdk_pixbuf_get_formats (); ListaL; ListaL = g_slist_next (ListaL))
	{
		Formato = (GdkPixbufFormat *) ListaL->data;
		if (gdk_pixbuf_format_is_writable (Formato))
		{
			//adicionr estensões
			aLista = gdk_pixbuf_format_get_extensions (Formato);
			for (nCont = 0; nCont < g_strv_length (aLista); nCont++)
				g_string_append_printf (Temp, "*.%s ", aLista[nCont]);
			BE_strfreev (aLista);
			//remover o último espaço
			Temp = g_string_truncate (Temp, Temp->len - 1);
			//adicionar nome da tipo
			g_string_append_printf (Temp, "(%s)\n", gdk_pixbuf_format_get_description (Formato));
		}
	}
	//remover o último \n
	Temp = g_string_truncate (Temp, Temp->len - 1);
	sFormatos = BE_string_free (Temp);
	//conversão
	BE_free (sTemp);
	if (be_arquivo_localidade_UTF8 ())
		sTemp = BE_ISO_UTF8 (Campo->sValor);
	else
		sTemp = g_strdup (Campo->sValor);
	//preparar local inicial
	sLocal = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, sTemp);
	//salvar
	if ((sTemp = be_Salvar (GTK_WINDOW (Janela->window), be_arquivo_localidade_UTF8 (), sFormatos, sLocal)))
	{
		//conversão
		BE_free (sLocal);
		sLocal = sTemp;
		sTemp = BE_UTF8_ISO (sLocal);
		BE_free (sFormatos);
		sFormatos = be_arquivo_extensao (sTemp);

		Imagem = be_sql_recuperar_arquivo_pixbuf (Campo, 'I');
		//salvar imagem
		if (Imagem)
		{
			//conversão de formatos específicos
			if (g_str_equal (sFormatos, "jpg") || g_str_equal (sFormatos, "JPG"))
			{
				BE_free (sFormatos);
				sFormatos = g_strdup ("jpeg");
			}
			//salvar
			if (!gdk_pixbuf_save (Imagem, sTemp, sFormatos, NULL, NULL))
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível salvar esta imagem.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			}
			BE_pixbuf_free (Imagem);
		}
		//salvar arquivo
		else
		{
			if (!be_sql_recuperar_arquivo (Campo, sTemp, 'B'))
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível salvar este arquivo.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			}
		}
	}

	BE_free (sTemp);
	BE_free (sLocal);
	BE_free (sFormatos);
}

void _be_Formulario_on_executar_clicked (GtkButton *button, gpointer user_data)
{
	_BEFormulario_Controle *Controle = NULL;
	_BEFormulario_Janela *Janela = NULL;

	Controle = (_BEFormulario_Controle *) user_data;
	Janela = Controle->Janela;

	if (!Controle->Campo->sParametros || !Controle->Campo->sValor || strlen (Controle->Campo->sValor) == 0)
		return;

	//verificar e chamar callback (Comando e Parâmetros)
	if (Janela->func_Executavel)
		Janela->func_Executavel (Controle->Campo->sParametros, Controle->Campo->sValor, Janela->user_data);
}

GtkWidget *_be_Formulario_subtabela (_BEFormulario_Controle *Controle)
{
	_BEFormulario_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GtkWidget *Subtabela = NULL;
	gchar **aLista = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;

	Janela = Controle->Janela;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//lista de parâmetros
	aLista = g_strsplit (Controle->Campo->sParametros, "; ", -1);
		/*
		aLista[0] = `vis_Tabela2`
		aLista[1] = `ID2`
		*/
	//preparar nome da tabela
	BE_free (sTemp);
	sTemp = g_strdup (aLista[0]+1);
	sTemp[strlen (sTemp)-1] = '\0';
	//abrir consulta relacionada
	Controle->Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (Janela->Sintaxe->Tabela->BD, sTemp), TRUE, &sErros);

	if (Controle->Sintaxe  && (Controle->Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
	{
		//limpar filtro do log
		if (Janela->SintaxeLog)
            be_sql_filtro_limpar (Janela->SintaxeLog, FALSE);

		//preparar nome do campo
		BE_free (sTemp);
		sTemp = g_strdup (aLista[1]+1);
		sTemp[strlen (sTemp)-1] = '\0';
		//preparar filtro atual
		Controle->CampoFiltro = be_sql_selecionar_campo (Controle->Sintaxe, sTemp);
		if (Controle->CampoFiltro)
			Controle->CampoFiltro->bAtivo = FALSE;
		//tabela
		if (Janela->func_Subtabela)
			Subtabela = Janela->func_Subtabela (Controle->Sintaxe, Janela->SintaxeLog, Janela->SintaxePerfil, Janela->window, BESQL_ACESSO_COMPLETO, TRUE, FALSE, FALSE, Janela->user_data);
	}
	//mouse padrão
	if (Subtabela)
	{
        BE_janela_mouse_normal (Mouse, FALSE);
	}
    else
    {
        BE_janela_mouse_normal (Mouse, TRUE);
    }

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		BE_sql_fechar_consulta (Controle->Sintaxe);
	}

	BE_strfreev (aLista);
	BE_free (sTemp);

	return Subtabela;
}

void _be_Formulario_bloquear_subtabelas (_BEFormulario_Janela *Janela, gboolean bBloquear)
{
	_BEFormulario_Controle *Controle = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;

	//verificar botão salvar
	if (bBloquear && GTK_WIDGET_IS_SENSITIVE (Janela->salvar))
		return;

	//loop nos controles
	for (ListaL = Janela->Controles; ListaL; ListaL = g_slist_next (ListaL))
	{
		Controle = (_BEFormulario_Controle *) ListaL->data;
		Campo = Controle->Campo;
		//localizar sub-tabelas
		if (Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros)
			gtk_widget_set_sensitive (Controle->controle, !bBloquear);//bloquear janela
	}

	//habilitar botão salvar
	gtk_widget_set_sensitive (Janela->salvar, bBloquear);
}

void _be_Formulario_novos_valores (_BEFormulario_Janela *Janela, gboolean bAdicionar)
{
	_BEFormulario_Controle *Controle = NULL;
	BESQL_Campo *Campo = NULL, *CampoAlterar = NULL, *CampoAdicionar = NULL;
	BESQL_Indice *Indice = NULL;
	GtkTextBuffer *buffer = NULL;
	GdkPixbuf *Imagem = NULL;
	GtkTextIter start, end;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	gboolean bSomenteLeitura = FALSE, bChave = FALSE;
	gchar *sTemp = NULL;
	gint nCont = 0;

	for (ListaL = Janela->Controles; ListaL; ListaL = g_slist_next (ListaL))
	{
		Controle = (_BEFormulario_Controle *) ListaL->data;
		Campo = Controle->Campo;
		bSomenteLeitura = Controle->bSomenteLeitura;
		//verificar campo para alterações
		if (Janela->bCIA)
			CampoAlterar = be_sql_selecionar_campo (Janela->SintaxeAlterar, Campo->sNome);
		else
			CampoAlterar = Campo;
		//tipos de dados
		if (!bSomenteLeitura && CampoAlterar)
		{
			if (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE)
			{
				//localizar ID
				CampoAlterar->sValorNovo = _be_Formulario_combo_box_localizar (Controle);
				CampoAlterar->bAfetar = TRUE;
			}
			else if (Campo->nTipo == BESQL_TIPO_BOL || Campo->nTipo == MYSQL_TYPE_BIT)
			{
				if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (Controle->subcontrole1)))
					CampoAlterar->sValorNovo = g_strdup ("1");
				else
					CampoAlterar->sValorNovo = g_strdup ("0");
				//verificar necessidade
				BE_free (sTemp);
				if (Campo->sValor && strlen (Campo->sValor) > 0 && (Campo->sValor[0] == 's' || Campo->sValor[0] == 'S' || Campo->sValor[0] == 'v' || Campo->sValor[0] == 'V' || Campo->sValor[0] == '1'))
					sTemp = g_strdup ("1");
				else
					sTemp = g_strdup ("0");
				if (!g_str_equal (CampoAlterar->sValorNovo, sTemp))
					CampoAlterar->bAfetar = TRUE;
			}
			else if (be_sql_campo_binario (Campo))
			{
				BE_free (sTemp);
				sTemp = gtk_combo_box_get_active_text (GTK_COMBO_BOX (Controle->subcontrole1));

				//alterar para nulo
				if (g_str_equal (sTemp, BEFORMULARIO_NENHUM_COMBO))
				{
					CampoAlterar->sValorNovo = NULL;
					//verificar necessidade
					if (Campo->sValor)
						CampoAlterar->bAfetar = TRUE;
				}
				else if (Controle->sString)
				{
					//verificar novamente comprimento do arquivo (tamanho pode ter aumentado entre a seleção do arquivo e até aqui)
					nCont = be_arquivo_tamanho (Controle->sString);
					if (nCont > Campo->nComprimentoMax)
					{
						BE_free (sTemp);
						if (Campo->nComprimentoMax > 1073741824)
							sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %.3f GB", (gfloat) Campo->nComprimentoMax / 1073741824.0);
						else if (Campo->nComprimentoMax > 1048576)
							sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %.2f MB", (gfloat) Campo->nComprimentoMax / 1048576.0);
						else if (Campo->nComprimentoMax > 1024)
							sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %.1f KB", (gfloat) Campo->nComprimentoMax / 1204.0);
						else
							sTemp = g_strdup_printf ("O arquivo selecionado é grande demais.\nO tamanho máximo é de %d B", Campo->nComprimentoMax);
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					}
					else if (nCont > BEGLOBAL_TAMANHO_MAX_ARQUIVO)
					{
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "O arquivo selecionado é grande demais.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
					}
					else if (nCont > 0)
					{
						//imagem e conversão
						if (be_arquivo_localidade_UTF8 ())
						{
							BE_free (sTemp);
							sTemp = BE_ISO_UTF8 (Controle->sString);
							Imagem = gdk_pixbuf_new_from_file (sTemp, NULL);
						}
						else
							Imagem = gdk_pixbuf_new_from_file (Controle->sString, NULL);

						if (Imagem)
						{
							//inserir
							if (be_sql_inserir_arquivo (CampoAlterar, Controle->sString, 'I'))
								CampoAlterar->bAfetar = TRUE;
							BE_pixbuf_free (Imagem);
						}
						//arquivo binário
						else
						{
							//inserir
							if (be_sql_inserir_arquivo (CampoAlterar, Controle->sString, 'B'))
								CampoAlterar->bAfetar = TRUE;
						}
					}
				}
			}
			else if (be_sql_campo_texto (Campo) && Campo->nComprimentoMax > BEFORMULARIO_LENSTRING_ENTRY)
			{
				BE_free (sTemp);
				buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (Controle->subcontrole1));
				gtk_text_buffer_get_bounds (buffer, &start, &end);
				sTemp = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
				CampoAlterar->sValorNovo = BE_UTF8_ISO (sTemp);
				//verificar necessidade
				if (!(Campo->sValor && g_str_equal (CampoAlterar->sValorNovo, Campo->sValor)))
					CampoAlterar->bAfetar = TRUE;
			}
			else if (Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros)
			{
				//Não fazer nada
			}
			else
			{
				CampoAlterar->sValorNovo = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Controle->subcontrole1)));
				//verificar necessidade
				if (!(Campo->sValor && g_str_equal (CampoAlterar->sValorNovo, Campo->sValor)))
					CampoAlterar->bAfetar = TRUE;
			}

			//verificar campo para adições
			if (bAdicionar && !be_sql_campo_binario (Campo))
			{
				bChave = FALSE;
				//verificar campo auto_incremento
				if (Janela->CampoAutoID && Janela->CampoAutoID == Campo)
					bChave = TRUE;
				//loop nos índices
				else
				{
					for (ListaL1 = Janela->Sintaxe->Indices; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Indice = (BESQL_Indice *) ListaL1->data;
						if (Indice->Campo == Campo && (Indice->nTipo == BESQL_INDICE_PRIMARIA || Indice->nTipo == BESQL_INDICE_UNICA))
						{
							bChave = TRUE;
							break;
						}
					}
				}
				//não alterar o valor padrão de campos com chave
				if (!bChave)
				{
					if (Janela->bCIA)
					{
						CampoAdicionar = be_sql_selecionar_campo (Janela->SintaxeAdicionar, Campo->sNome);
						if (CampoAdicionar && CampoAlterar->sValorNovo)
						{
							CampoAdicionar->sValorNovo = g_strdup (CampoAlterar->sValorNovo);
							CampoAdicionar->bAfetar = TRUE;
						}
					}
					else
					{
						Campo->bAfetar = TRUE;
					}
				}
			}
		}
	}

	BE_free (sTemp);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonAdicionar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sID = NULL;
	gchar *sErros = NULL;
	gboolean bAtualizar = TRUE;

	Janela = (_BEFormulario_Janela *) user_data;

	//salvar alterações pendentes
	//_be_Interface_sinal_windowFormulario_on_toolbuttonSalvar_clicked (NULL, user_data);

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->adicionar) || !GTK_WIDGET_VISIBLE (Janela->adicionar))
		return;
	//confirmar esta operação
	if (Janela->bConfAdicionar)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Deseja realmente adicionar uma nova linha ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//recuperar valores padrão
	if (Janela->bPadraoAutomatico)
		_be_Formulario_novos_valores (Janela, TRUE);

	//adicionar
	if (Janela->bCIA)
	{
		if (!be_sql_adicionar_CIA (Janela->SintaxeAdicionar, &sErros))
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível adicionar uma linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
		}
		else
			//selecionar a última linha inserida
			sID = be_sql_informacoes (Janela->Sintaxe->Tabela->BD->SQL, BESQL_INFO_ULTIMO_ID, &sErros);
	}
	else
	{
		if (be_sql_adicionar (Janela->SintaxeAdicionar, &sErros) != 1)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível adicionar uma linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
		}
		else
			//selecionar a última linha inserida
			sID = be_sql_informacoes (Janela->Sintaxe->Tabela->BD->SQL, BESQL_INFO_ULTIMO_ID, &sErros);
	}

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		bAtualizar = FALSE;
	}
	//atualizar
	if (bAtualizar)
	{
		//filtrando por Auto_incremento
		if (sID)
		{
			//limpar filtro
			be_sql_filtro_limpar (Janela->Sintaxe, FALSE);
			//adicionar novo
			be_sql_filtro_adicionar (Janela->Sintaxe, Janela->CampoAutoID, BESQL_COMPARACAO_IGUAL, sID, BESQL_RELACAO_OU, 0, FALSE);
		}
		//exibir
		_be_Formulario_executar_consulta (Janela);
	}

	BE_free (sID);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonRemover_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;
	gboolean bAtualizar = TRUE;

	Janela = (_BEFormulario_Janela *) user_data;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->remover) || !GTK_WIDGET_VISIBLE (Janela->remover))
		return;
	//confirmar esta operação
	if (Janela->bConfRemover)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "<b>Deseja realmente excluir esta linha ?</b>", GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	if (Janela->bCIA)//CIA
	{
		//obter trava
		if (be_sql_obter_bloqueio_excluir_CIA (Janela->SintaxeRemover, Janela->CampoID->sValor, &sErros))
		{
			//remover e verificar
			if (!be_sql_excluir_CIA (Janela->SintaxeRemover, Janela->CampoID->sValor, &sErros))
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível remover esta linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
				bAtualizar = FALSE;
			}
            //liberar trava
            be_sql_liberar_bloqueio_CIA (Janela->SintaxeRemover, Janela->CampoID->sValor, &sErros);
		}
		else
		{
			BE_free (sTemp);
			sTemp = g_strdup_printf ("O usuário %d está bloqueando esta linha.\nTente mais tarde.", be_sql_usuario_bloqueando_CIA (Janela->SintaxeAlterar, Janela->CampoID->sValor, &sErros));
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
		}
	}
	else
	{
		//loop nos campos
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			//loop nos índices
			for (ListaL1 = Janela->Sintaxe->Indices; ListaL1; ListaL1 = g_slist_next (ListaL1))
			{
				Indice = (BESQL_Indice *) ListaL1->data;
				if (Indice->nTipo == BESQL_INDICE_PRIMARIA && Indice->Campo == Campo)
				{
					//verificar nulos
					if (Campo->sValor)
						Indice->sValorFiltro = g_strdup (Campo->sValor);
					else
						Indice->sValorFiltro = NULL;
					Indice->bAfetar = TRUE;
				}
			}
		}
		//remover e verificar
		if (Indice)
		{
			if (be_sql_excluir_indice (Janela->SintaxeRemover, &sErros) <= 0)
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível remover estas informações.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
				bAtualizar = FALSE;
			}
		}
		//faltando campo Indice
		else
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna com índice para identificar estas informações.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
		}
	}

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		bAtualizar = FALSE;
	}

	//atualizar
	if (bAtualizar)
		_be_Formulario_executar_consulta (Janela);

	BE_free (sTemp);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonSalvar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	gchar *sTemp = NULL, *sErros = NULL;
	gboolean bAtualizar = TRUE;

	Janela = (_BEFormulario_Janela *) user_data;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->salvar) || !GTK_WIDGET_VISIBLE (Janela->salvar))
		return;

	//confirmar esta operação
	if (Janela->bConfSalvar)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Deseja realmente alterar estas informações ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}
	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//recuperar valores alterados
	_be_Formulario_novos_valores (Janela, FALSE);

	//alterar
	if (Janela->bCIA)//CIA
	{
        //obter trava
        if (Janela->CampoID->sValor && be_sql_obter_bloqueio_CIA (Janela->SintaxeAlterar, Janela->CampoID->sValor, &sErros))
        {
            //alterar e verificar
            if (!be_sql_alterar_CIA (Janela->SintaxeAlterar, Janela->CampoID->sValor, &sErros))
            {
                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível alterar estas informações.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
                bAtualizar = FALSE;
            }
            //liberar trava
            be_sql_liberar_bloqueio_CIA (Janela->SintaxeAlterar, Janela->CampoID->sValor, &sErros);
        }
        else
        {
            BE_free (sTemp);
            sTemp = g_strdup_printf ("O usuário %d está bloqueando esta linha.\nTente mais tarde.", be_sql_usuario_bloqueando_CIA (Janela->SintaxeAlterar, Janela->CampoID->sValor, &sErros));
            be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
            bAtualizar = FALSE;
        }
	}
	//alterar e verificar
	else
	{
		//loop nos campos
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			//loop nos índices
			for (ListaL1 = Janela->Sintaxe->Indices; ListaL1; ListaL1 = g_slist_next (ListaL1))
			{
				Indice = (BESQL_Indice *) ListaL1->data;
				if (Indice->nTipo == BESQL_INDICE_PRIMARIA && Indice->Campo == Campo)
				{
					//verificar nulos
					if (Campo->sValor)
						Indice->sValorFiltro = g_strdup (Campo->sValor);
					else
						Indice->sValorFiltro = NULL;
					Indice->bAfetar = TRUE;
				}
			}
		}
		if (Indice)
		{
			if (be_sql_alterar_indice (Janela->SintaxeAlterar, &sErros) <= 0)
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível alterar estas informações.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
				bAtualizar = FALSE;
			}
		}
		//faltando campo Indice
		else
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna com índice para identificar estas informações.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
		}
	}
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		bAtualizar = FALSE;
	}
	//atualizar
	if (bAtualizar)
		_be_Formulario_executar_consulta (Janela);

    //limpar
    BE_free (sTemp);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonDados_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	GSList *Lista = NULL;
	gint nPos = -1;

	Janela = (_BEFormulario_Janela *) user_data;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, "Exportar (Ctrl+E)");//0
	nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Ajustar dados", Lista, 0);
	//limpar lista
	BE_slist_free (Lista);

	//selecionar opção
	switch (nPos)
	{
		case 0://exportar
			_be_Formulario_exportar (Janela);
			break;
	}
}

void _be_Formulario_exportar (_BEFormulario_Janela *Janela)
{
	BEGlobal_Janela *Mouse = NULL;
	gchar *sTemp = NULL, *sNomeArquivo = NULL;
	gchar *sErros = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	//selecionar arquivo para salvar
	sNomeArquivo = be_arquivo_nome_sugerido ("Dados Exportados", 0, TRUE);
	BE_free (sTemp);
	sTemp = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, sNomeArquivo);
	BE_free (sNomeArquivo);
	sNomeArquivo = be_Salvar (GTK_WINDOW (Janela->window), be_arquivo_localidade_UTF8 (), "*.html *htm(Documento HTML)\n*.*(Todos)", sTemp);

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//loop nos dados
	if (sNomeArquivo)
	{
		//conversão
		BE_free (sTemp);
		sTemp = sNomeArquivo;
		sNomeArquivo = BE_UTF8_ISO (sTemp);
		//nome do documento
		BE_free (sTemp);
		if (Janela->Sintaxe->Tabela->sNomeUsual)
			sTemp = g_strdup (Janela->Sintaxe->Tabela->sNomeUsual);
		else if (Janela->Sintaxe->Tabela->sNomeReal)
			sTemp = g_strdup (Janela->Sintaxe->Tabela->sNomeReal);
		else
			sTemp = g_strdup (Janela->Sintaxe->Tabela->sNome);
		//exportar
		if (be_sql_exportar_dados (Janela->Sintaxe, sNomeArquivo, sTemp, BESQL_EXPORTAR_HTML, &sErros))
		{
			//informar
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Arquivo gravado em %s.", sNomeArquivo);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		}
	}

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//limpar
	BE_free (sTemp);
	BE_free (sNomeArquivo);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonRelatorios_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sNomeArquivo = NULL, *sNomeRelatorio = NULL;
	const gchar *sArquivo = NULL, *sArquivoFonte = NULL;
	gchar *sArquivo_Padrao = NULL;
	gchar *sErros = NULL;
	gint nPos = -1;

	Janela = (_BEFormulario_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->relatorios) || !GTK_WIDGET_VISIBLE (Janela->relatorios))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, g_strdup ("Formulário Padrão"));//0
	Lista = g_slist_append (Lista, g_strdup ("Formulário Padrão (salvar arquivo)"));//1
	//relatórios personalizados
	for (ListaL = Janela->Sintaxe->Tabela->Relatorios; ListaL; ListaL = g_slist_next (ListaL))
	{
		//preparar nome do arquivo
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = be_arquivo_extensao ((gchar *) ListaL->data);
		sTemp = g_path_get_basename ((gchar *) ListaL->data);
		if (sTemp1)
			sTemp[strlen (sTemp) - strlen (sTemp1) - 1] = '\0';
		//adicionar na lista
		Lista = g_slist_append (Lista, g_strdup (sTemp));//2
		Lista = g_slist_append (Lista, g_strdup_printf ("%s (salvar arquivo)", sTemp));//3
	}
	//janela para seleção
	nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Relatórios", Lista, 0);
	//limpar lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		sTemp = (gchar *) ListaL->data;
		BE_free (sTemp);
	}
	BE_slist_free (Lista);
    //preparar configuração local
    BE_free (sTemp);
    sTemp = be_diretorio_local (TRUE);
    sArquivo_Padrao = g_strdup_printf ("%s%s", sTemp, BEFORMULARIO_RELATORIO_ATUAL);
	//selecionar relatório
	switch (nPos)
	{
		case -1://sair
			break;
		case 0://formulário padrão
			sArquivo = Janela->sArquivoSVG_FormularioPadrao;
			sNomeArquivo = BE_ISO_UTF8 (sArquivo_Padrao);
			break;
		case 1://formulário padrão, salvar arquivo
			sArquivo = Janela->sArquivoSVG_FormularioPadrao;
			sNomeArquivo = be_arquivo_nome_sugerido ("Formulário Padrão", 0, TRUE);
			break;
		default://personalizados
			sArquivo = (gchar *) g_slist_nth_data (Janela->Sintaxe->Tabela->Relatorios, (nPos - 2) / 2);
			if (((nPos - 2) % 2) == 0)//inteiro, exibir
				sNomeArquivo = BE_ISO_UTF8 (sArquivo_Padrao);
			else//decimal, salvar arquivo
			{
				//preparar nome do arquivo
				BE_free (sTemp);
				BE_free (sTemp1);
				sTemp1 = be_arquivo_extensao (sArquivo);
				sTemp = g_path_get_basename (sArquivo);
				if (sTemp1)
					sTemp[strlen (sTemp) - strlen (sTemp1) - 1] = '\0';

				sNomeArquivo = be_arquivo_nome_sugerido (sTemp, 0, TRUE);
			}
			break;
	}

	//selecionar arquivo para salvar
	if (nPos == 1 || (nPos > 1 && ((nPos - 2) % 2) != 0))
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, sNomeArquivo);
		BE_free (sNomeArquivo);
		sNomeArquivo = be_Salvar (GTK_WINDOW (Janela->window), be_arquivo_localidade_UTF8 (), "*.pdf(Documento PDF)\n*.*(Todos)", sTemp);
	}

	//nome do relatório
	if (Janela->Sintaxe->Tabela->sNomeUsual)
		sNomeRelatorio = g_strdup (Janela->Sintaxe->Tabela->sNomeUsual);
	else if (Janela->Sintaxe->Tabela->sNomeReal)
		sNomeRelatorio = g_strdup (Janela->Sintaxe->Tabela->sNomeReal);
	else
		sNomeRelatorio = g_strdup (Janela->Sintaxe->Tabela->sNome);

	//fontes
	sArquivoFonte = Janela->sArquivoFonte;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//conversão
	BE_free (sTemp);
	if (sNomeArquivo)
	{
		sTemp = sNomeArquivo;
		sNomeArquivo = BE_UTF8_ISO (sTemp);
	}
	//gerar relatório
	if (sArquivo && sNomeArquivo && be_relatorio_gerar (sArquivo, sNomeArquivo, sArquivoFonte, NULL, sNomeRelatorio, Janela->Empresa, Janela->Sintaxe, &sErros))
	{
		//opções
		if (nPos == 1 || (nPos > 1 && ((nPos - 2) % 2) != 0))//informar relatório salvo
		{
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Relatório gravado em %s.", sNomeArquivo);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		}
		else//exibir
		{
			BE_free (sTemp);
			if (Janela->sLeitorPDF)
				#ifdef G_OS_UNIX
					sTemp = g_strdup_printf ("%s %s", Janela->sLeitorPDF, sNomeArquivo);//preparar
    			#endif
				#ifdef G_OS_WIN32
					sTemp = g_strdup_printf ("\"%s\" \"%s\"", Janela->sLeitorPDF, sNomeArquivo);//preparar
				#endif
			//abrir relatório
			//system
			if (!sTemp || !g_spawn_command_line_async (sTemp, NULL))//informar erros
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o leitor de PDF.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		}
	}
	//atualizar dados
	_be_Formulario_executar_consulta (Janela);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sNomeArquivo);
	BE_free (sNomeRelatorio);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonOpcoes_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	GSList *Lista = NULL;
	gint nPos = -1;

	Janela = (_BEFormulario_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, "Propriedades");//0
	if (Janela->SintaxeLog)
		Lista = g_slist_append (Lista, "Auditoria (log de ações)");//1
	nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Opções", Lista, 0);
	//limpar lista
	BE_slist_free (Lista);

	//selecionar opção
	switch (nPos)
	{
		case 0://propriedades
			_be_Formulario_propriedades (Janela);
			break;
		case 1://auditoria
			_be_Formulario_auditoria (Janela);
			break;
	}
}

void _be_Formulario_propriedades (_BEFormulario_Janela *Janela)
{
	GSList *Lista = NULL;
	gint nPos = -1;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, "Informações da tabela");//1
	Lista = g_slist_append (Lista, "Confirmações");//1
	Lista = g_slist_append (Lista, "Exibição de campos");//2
	if (GTK_WIDGET_VISIBLE (Janela->adicionar))
		Lista = g_slist_append (Lista, "Valor padrão");//3
	//janela para seleção
	nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Propriedades", Lista, 0);
	//limpar lista
	BE_slist_free (Lista);

	//selecionar opção
	switch (nPos)
	{
		case 0://Informações da tabela
			_be_Formulario_info_tabela (Janela);
			break;
		case 1://Confirmações
			_be_Formulario_confirmacoes (Janela);
			break;
		case 2://Exibição
			_be_Formulario_exibir_campos (Janela);
			break;
		case 3://Valor padrão
			_be_Formulario_valor_padrao (Janela);
			break;
	}
}

void _be_Formulario_auditoria (_BEFormulario_Janela *Janela)
{
	GtkWidget *window = NULL, *Subtabela = NULL;
	BESQL_Campo *Campo = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sTemp = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;
	//verificar acesso
	if (!Janela->SintaxeLog)
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//criar nova janela
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (Janela->window));//filho
	gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	sTemp = BE_ISO_UTF8 ("Auditoria - Log de Ações");
	gtk_window_set_title (GTK_WINDOW (window), sTemp);
	gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW (window));
	//gtk_window_set_icon_from_file (GTK_WINDOW (window), "../Interfaces/log.png", NULL);//ícone
	//preparar filtro
	be_sql_filtro_limpar (Janela->SintaxeLog, FALSE);
	Campo = be_sql_selecionar_campo (Janela->SintaxeLog, "Tabela");
	if (Campo)
	{
		Campo->bAtivo = FALSE;
		if (Janela->bCIA)
			be_sql_filtro_adicionar (Janela->SintaxeLog, Campo, BESQL_COMPARACAO_IGUAL, Janela->Sintaxe->Tabela->sNomeReal, BESQL_RELACAO_E, 0, FALSE);
		else
			be_sql_filtro_adicionar (Janela->SintaxeLog, Campo, BESQL_COMPARACAO_IGUAL, Janela->Sintaxe->Tabela->sNome, BESQL_RELACAO_E, 0, FALSE);
		Campo = be_sql_selecionar_campo (Janela->SintaxeLog, "Tabela ID");
		if (Campo && Janela->CampoID->sValor)
		{
			Campo->bAtivo = FALSE;
			be_sql_filtro_adicionar (Janela->SintaxeLog, Campo, BESQL_COMPARACAO_IGUAL, Janela->CampoID->sValor, BESQL_RELACAO_E, 0, FALSE);
		}
	}
	//anexar tabela de log
	if (Janela->func_Subtabela)
		Subtabela = Janela->func_Subtabela (Janela->SintaxeLog, NULL, NULL, window, BESQL_ACESSO_SELECIONAR, FALSE, TRUE, TRUE, Janela->user_data);
	if (Subtabela)
	{
		gtk_container_add (GTK_CONTAINER (window), Subtabela);
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

	BE_free (sTemp);
}

void _be_Formulario_confirmacoes (_BEFormulario_Janela *Janela)
{
	GSList *Propriedades = NULL;
	gchar *sAdicao = NULL, *sAlteracao = NULL, *sExclusao = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	sAdicao = g_strdup_printf ("%d", Janela->bConfAdicionar);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LOGICO, "Pedir confirmação na adição", &sAdicao, NULL);
	sAlteracao = g_strdup_printf ("%d", Janela->bConfSalvar);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LOGICO, "Pedir confirmação na alteração", &sAlteracao, NULL);
	sExclusao = g_strdup_printf ("%d", Janela->bConfRemover);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LOGICO, "Pedir confirmação na exclusão", &sExclusao, NULL);

	//abrir janela
	if (be_Propriedades (GTK_WINDOW (Janela->window), Propriedades))
	{
		Janela->bConfAdicionar = atoi (sAdicao);
		Janela->bConfSalvar = atoi (sAlteracao);
		Janela->bConfRemover = atoi (sExclusao);
	}

	BE_free (sAdicao);
	BE_free (sAlteracao);
	BE_free (sExclusao);
	BE_slist_free (Propriedades);
}

void _be_Formulario_exibir_campos (_BEFormulario_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL, *Propriedades = NULL;
	gchar *sTemp = NULL, *sValor[g_slist_length (Janela->Sintaxe->Campos)];
	gint nCont = 0, bAchou = FALSE;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	//criando a lista de campos
	for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL), nCont++)
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (!Janela->bCIA || !g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
		{
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Exibir '%s'", Campo->sNome);
			sValor[nCont] = g_strdup_printf ("%d", Campo->bAtivo);
			Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LOGICO, sTemp, &sValor[nCont], NULL);
		}
		else
			sValor[nCont] = NULL;
	}

	//abrir janela
	if (be_Propriedades (GTK_WINDOW (Janela->window), Propriedades))
	{
		//lista de campos
		for (ListaL = Janela->Sintaxe->Campos, nCont = 0; ListaL; ListaL = g_slist_next (ListaL), nCont++)
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (!Janela->bCIA || !g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
			{
				Campo->bAtivo = atoi (sValor[nCont]);
				if (Campo->bAtivo)
					bAchou = TRUE;
			}
			else
				bAchou = TRUE;
			BE_free (sValor[nCont]);
		}
		//proteção contra erros, deve exibir pelo menos uma coluna
		if (!bAchou)
		{
			ListaL = Janela->Sintaxe->Campos;
			Campo = (BESQL_Campo *) ListaL->data;
			Campo->bAtivo = TRUE;
		}
		//atualizar tudo
		_be_Formulario_destruir (Janela);
		_be_Formulario_executar_consulta (Janela);
	}
	else
	{
		//limpar lista
		for (ListaL = Janela->Sintaxe->Campos, nCont = 0; ListaL; ListaL = g_slist_next (ListaL), nCont++)
		{
			BE_free (sValor[nCont]);
		}
	}

	BE_free (sTemp);
	BE_slist_free (Propriedades);
}

void _be_Formulario_valor_padrao (_BEFormulario_Janela *Janela)
{
	GSList *Propriedades = NULL;
	gchar *sPadrao = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	sPadrao = g_strdup_printf ("%d", Janela->bPadraoAutomatico);
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LOGICO, "Utilizar como valor padrão os valores atuais", &sPadrao, NULL);

	//abrir janela
	if (be_Propriedades (GTK_WINDOW (Janela->window), Propriedades))
	{
		Janela->bPadraoAutomatico = atoi (sPadrao);
	}

	BE_free (sPadrao);
	BE_slist_free (Propriedades);
}

void _be_Formulario_info_tabela (_BEFormulario_Janela *Janela)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	GString *Info = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL;

	Sintaxe = Janela->Sintaxe;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

    Info = g_string_new ("<b>Tabela:</b> ");
    //informações da tabela
    if (Sintaxe->Tabela->sNomeUsual  && strlen (Sintaxe->Tabela->sNomeUsual))
        g_string_append_printf (Info, "%s", Sintaxe->Tabela->sNomeUsual);

    if (Sintaxe->Tabela->sNome  && strlen (Sintaxe->Tabela->sNome))
        g_string_append_printf (Info, "\n - %s - ", Sintaxe->Tabela->sNome);

    if (Sintaxe->Tabela->sNomeReal  && strlen (Sintaxe->Tabela->sNomeReal))
        g_string_append_printf (Info, "\n (%s) ", Sintaxe->Tabela->sNomeReal);

    if (Sintaxe->Tabela->sComentarios && strlen (Sintaxe->Tabela->sComentarios) > 0)
        g_string_append_printf (Info, "\n<i>%s</i>", Sintaxe->Tabela->sComentarios);

	//criando a lista de campos
	g_string_append_printf (Info, "\n\n<b>Campos</b>:");
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
        g_string_append_printf (Info, "\n\n<span foreground='#0000FF'>%s</span>:", Campo->sNome);
		if (Campo->sComentarios)
            g_string_append_printf (Info, " <i>%s</i>", Campo->sComentarios);
        g_string_append_printf (Info, "\n - Comprimento máximo = %d", Campo->nComprimentoMax);
        g_string_append_printf (Info, "\n - Permite alteração = %s", Campo->bAlteravel ? "Sim" : "Não");
        g_string_append_printf (Info, "\n - Permite valor nulo = %s", Campo->bAnulavel ? "Sim" : "Não");
        g_string_append_printf (Info, "\n - Tipo = %s [%d]", Campo->sTipo, Campo->nTipo);
        if (Campo->sParametros)
            g_string_append_printf (Info, "\n - Parâmetros = %s", Campo->sParametros);
        if (Campo->sGrupo)
            g_string_append_printf (Info, "\n - Grupo = %s", Campo->sGrupo);
        if (Campo->sValorPadrao)
            g_string_append_printf (Info, "\n - Valor padrão original = %s", Campo->sValorPadrao);
	}

    //exibir informações
    sTemp = g_string_free (Info, FALSE);
    be_Mensagens_Ext (GTK_WINDOW (Janela->window), FALSE, "Informações", sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);

    //limpar
	BE_free (sTemp);
}

void _be_Interface_sinal_windowFormulario_on_toolbuttonPerfil_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;
	GSList *Lista = NULL, *ListaL = NULL, *ListaL1 = NULL;
	gchar *sErros = NULL;
	gint nPos = -1;
	gchar *sTemp = NULL, *sVariavel = NULL;

	Janela = (_BEFormulario_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->perfil) || !GTK_WIDGET_VISIBLE (Janela->perfil))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, g_strdup ("Salvar perfil"));//0
	Lista = g_slist_append (Lista, g_strdup ("Excluir perfil"));//1

	//perfil existente
	sTemp = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFORMULARIO_PERFIL);
	//carregar lista de perfil
	ListaL = be_sql_perfil_variaveis (Janela->SintaxePerfil, sTemp, &sErros);
	//loop na lista
	for (ListaL1 = ListaL; ListaL1; ListaL1 = g_slist_next (ListaL1))
	{
		BE_free (sTemp);
		sTemp = (gchar *) ListaL1->data;
		Lista = g_slist_append (Lista, g_strdup_printf ("Carregar perfil %s", sTemp));//apartir de 2
	}
	BE_slist_free (ListaL);
	//selecionar
    if (!sErros)
        nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Perfil", Lista, 0);
	//recuperar valor
	BE_free (sTemp);
	sTemp = (gchar *) g_slist_nth_data (Lista, nPos);
	if (sTemp)
		sVariavel = g_strdup (sTemp+16);
	//limpar lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		sTemp = (gchar *) ListaL->data;
		BE_free (sTemp);
	}
	BE_slist_free (Lista);

	//selecionar opção
	switch (nPos)
	{
		case -1://nada
			break;
		case 0://salvar
			_be_Formulario_salvar_perfil (Janela);
			break;
		case 1://excluir
			_be_Formulario_excluir_perfil (Janela);
			break;
		default://carregar
			_be_Formulario_carregar_perfil (Janela, sVariavel);
			break;
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

    //limpar
	BE_free (sTemp);
	BE_free (sVariavel);
}

void _be_Formulario_salvar_perfil (_BEFormulario_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sErros = NULL;
	gchar *sNome = NULL, *sVariavel = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->perfil) || !GTK_WIDGET_VISIBLE (Janela->perfil))
		return;

	//preparar ini
	Ini = g_key_file_new ();

	//janela de interação
	sVariavel = be_Mensagens_in (GTK_WINDOW (Janela->window), FALSE, "Salvar perfil", "Digite um nome para este perfil.", NULL);
	if (sVariavel && strlen (sVariavel) > 0)
	{
        //preparar variavel
		BE_free (sTemp);
		sTemp = sVariavel;
		sVariavel = BE_UTF8_ISO (sTemp);
		//preparar nome perfil
		BE_free (sNome);
		sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFORMULARIO_PERFIL);
		//preparar perfil
		if (sVariavel)
		{
			//preparar confirmação
			g_key_file_set_boolean (Ini, "Confirmacao", "Adicionar", Janela->bConfAdicionar);
			g_key_file_set_boolean (Ini, "Confirmacao", "Salvar", Janela->bConfSalvar);
			g_key_file_set_boolean (Ini, "Confirmacao", "Remover", Janela->bConfRemover);
			//preparar valor padrão automático
			g_key_file_set_boolean (Ini, "Padrao", "AutoPadrao", Janela->bPadraoAutomatico);
			//loop dos campos
			for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				//preparar exibição
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sNome);
				g_key_file_set_boolean (Ini, "Exibicao", sTemp, Campo->bAtivo);
			}
			//salvar
			if (!be_sql_perfil_salvar_ini (Janela->SintaxePerfil, sNome, sVariavel, Ini, FALSE, &sErros))
                be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível salvar este perfil.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
		}
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//limpar
	BE_free (sNome);
	BE_free (sVariavel);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_key_file_free (Ini);
}

void _be_Formulario_excluir_perfil (_BEFormulario_Janela *Janela)
{
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sErros = NULL;
	gint nPos = -1;
	gchar *sNome = NULL, *sVariavel = NULL;
	gchar *sTemp = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->perfil) || !GTK_WIDGET_VISIBLE (Janela->perfil))
		return;

	//perfil existente
	sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFORMULARIO_PERFIL);
	//carregar lista de perfil
	Lista = be_sql_perfil_variaveis (Janela->SintaxePerfil, sNome, &sErros);
	//exibir lista
	if (g_slist_length (Lista) > 0)
	{
		nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Excluir perfil", Lista, 0);
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
		if (!be_sql_perfil_excluir (Janela->SintaxePerfil, sNome, sVariavel, &sErros))
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
	BE_free (sNome);
	BE_free (sVariavel);
	BE_free (sTemp);
}

void _be_Formulario_carregar_perfil (_BEFormulario_Janela *Janela, const gchar *sVariavel)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sErros = NULL;
	gchar *sNome = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->perfil) || !GTK_WIDGET_VISIBLE (Janela->perfil))
		return;

	if (sVariavel && strlen (sVariavel) > 0)
	{
		//localizar existente
		sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFORMULARIO_PERFIL);
		if ((Ini = be_sql_perfil_ler_ini (Janela->SintaxePerfil, sNome, sVariavel, &sErros)))
		{
			//preparar confirmação
			Janela->bConfAdicionar = g_key_file_get_boolean (Ini, "Confirmacao", "Adicionar", NULL);
			Janela->bConfSalvar = g_key_file_get_boolean (Ini, "Confirmacao", "Salvar", NULL);
			Janela->bConfRemover = g_key_file_get_boolean (Ini, "Confirmacao", "Remover", NULL);
			//preparar valor padrão automático
			Janela->bPadraoAutomatico = g_key_file_get_boolean (Ini, "Padrao", "AutoPadrao", NULL);
			//loop dos campos
			for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				//preparar exibição
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sNome);
				Campo->bAtivo = g_key_file_get_boolean (Ini, "Exibicao", sTemp, NULL);
			}
			//atualizar tudo
			_be_Formulario_destruir (Janela);
			_be_Formulario_executar_consulta (Janela);
		}
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//limpar
	BE_free (sNome);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_key_file_free (Ini);
}

gboolean _be_Interface_sinal_windowFormulario_on_tableFormulario_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;

	Janela = (_BEFormulario_Janela *) user_data;

	//teclas de atalho, com Ctrl: gdk/gdkkeysyms.h
	if (event->state & GDK_CONTROL_MASK)
	{
		//verificar tecla pressionada
		switch (event->keyval)
		{
			//teclas de atalho para adicionar
			case GDK_a://Ctrl+a
			case GDK_A://Ctrl+a
			case GDK_Insert://Ctrl+Ins
				_be_Interface_sinal_windowFormulario_on_toolbuttonAdicionar_clicked (GTK_TOOL_BUTTON (Janela->adicionar), Janela);
				break;
			//teclas de atalho para excluir
			case GDK_x://Ctrl+x
			case GDK_X://Ctrl+X
			case GDK_Delete://Ctrl+Del
				_be_Interface_sinal_windowFormulario_on_toolbuttonRemover_clicked (GTK_TOOL_BUTTON (Janela->remover), Janela);
				break;
			//teclas de atalho para salvar
			case GDK_s://Ctrl+s
			case GDK_S://Ctrl+S
				_be_Interface_sinal_windowFormulario_on_toolbuttonSalvar_clicked (GTK_TOOL_BUTTON (Janela->salvar), Janela);
				break;
			//teclas de atalho para ajustar dados
			case GDK_d://Ctrl+d
			case GDK_D://Ctrl+D
				_be_Interface_sinal_windowFormulario_on_toolbuttonDados_clicked (GTK_TOOL_BUTTON (Janela->dados), Janela);
				break;
			//teclas de atalho para exportar
			case GDK_e://Ctrl+e
			case GDK_E://Ctrl+E
				_be_Formulario_exportar (Janela);
				break;
			//teclas de atalho para relatórios
			case GDK_p://Ctrl+p
			case GDK_P://Ctrl+P
				_be_Interface_sinal_windowFormulario_on_toolbuttonRelatorios_clicked (GTK_TOOL_BUTTON (Janela->relatorios), Janela);
				break;
			//teclas de atalho para opções
			case GDK_o://Ctrl+o
			case GDK_O://Ctrl+O
				_be_Interface_sinal_windowFormulario_on_toolbuttonOpcoes_clicked (GTK_TOOL_BUTTON (Janela->opcoes), Janela);
				break;
			//teclas de atalho para perfil
			case GDK_r://Ctrl+r
			case GDK_R://Ctrl+R
				_be_Interface_sinal_windowFormulario_on_toolbuttonPerfil_clicked (GTK_TOOL_BUTTON (Janela->opcoes), Janela);
				break;
			//padrão, sem ação
			default:
				//parar outras ações, não propagar o sinal
				return FALSE;
				break;
		}
	}
	else
	{
		switch (event->keyval)
		{
			case GDK_F5://atualizar
				_be_Formulario_executar_consulta (Janela);
				break;
			//padrão, sem ação
			default:
				//parar outras ações, não propagar o sinal
				return FALSE;
				break;
		}
	}
	//continuar qualquer ação que envolva pressionamento de teclas
	return TRUE;
}

gint _be_Formulario_ordenar_lista (gconstpointer a, gconstpointer b)
{
	BESQL_Campo *CampoA = NULL, *CampoB = NULL;

	CampoA = (BESQL_Campo *) a;
	CampoB = (BESQL_Campo *) b;

	return be_string_ordenar_string (CampoA->sGrupo, CampoB->sGrupo);
}

void _be_Formulario_on_destroy (GtkObject *object, gpointer user_data)
{
	_BEFormulario_Janela *Janela = NULL;

	Janela = (_BEFormulario_Janela *) user_data;

	_be_Formulario_destruir (Janela);
	BE_sql_fechar_consulta (Janela->Sintaxe);
	BE_free (Janela);
}
