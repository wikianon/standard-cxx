#include "beTabela.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowTabela.c"

static gboolean bSQLCarregado = FALSE;

GtkWidget *be_Tabela (
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
	gboolean bAtualizar,
	void (*func_Formulario)(BESQL_Tabela *Tabela, GSList *Filtros, gpointer user_data),
	void (*func_Tabela)(BESQL_Tabela *Tabela, gpointer user_data),
	gpointer user_data
	)
{
	_BETabela_Janela *Janela = NULL;
	GtkAccelGroup *accel;
	gchar *sErros = NULL;

	//preparar teclas de atalho
	accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (window), accel);
	//objetos da janela
	Janela = g_new (_BETabela_Janela, 1);
	Janela->window = window;
	Janela->tabela = be_Interface_criar_windowTabela_tableTabela (window, accel, NULL, Janela, NULL);//carregar tabela
	Janela->treeview = BE_interfaces_widget (Janela->window, "treeviewTabela");//lista
	Janela->renderer = NULL;
	Janela->editable = NULL;
	Janela->label = BE_interfaces_widget (Janela->window, "labelPosicao");//label de posição da página
	Janela->primeiro = BE_interfaces_widget (Janela->window, "toolbuttonPrimeiro");
	Janela->anterior = BE_interfaces_widget (Janela->window, "toolbuttonVoltar");
	Janela->posterior = BE_interfaces_widget (Janela->window, "toolbuttonAvancar");
	Janela->ultimo = BE_interfaces_widget (Janela->window, "toolbuttonUltimo");
	Janela->adicionar = BE_interfaces_widget (Janela->window, "toolbuttonAdicionar");
	Janela->bConfAdicionar = TRUE;
	Janela->remover = BE_interfaces_widget (Janela->window, "toolbuttonRemover");
	Janela->bConfRemover = TRUE;
	Janela->salvar = BE_interfaces_widget (Janela->window, "toolbuttonSalvar");
	Janela->bConfSalvar = TRUE;
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
	Janela->nPagina = 1;
	Janela->colLocalizar = NULL;
	Janela->sBloqueioID = NULL;
	Janela->sArquivoSVG_ListaPadrao = sArquivoSVG_ListaPadrao;
	Janela->sArquivoSVG_FormularioPadrao = sArquivoSVG_FormularioPadrao;
	Janela->sArquivoFonte = sArquivoFonte;
	Janela->sLeitorPDF = sLeitorPDF;
	Janela->Empresa = Empresa;
	Janela->bLog = bLog;
	Janela->func_Formulario = func_Formulario;
	Janela->func_Tabela = func_Tabela;
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
	if (!(Janela->SintaxeAdicionar->Tabela->nFlags & BESQL_ACESSO_SELECIONAR) || !(Janela->SintaxeAdicionar->Tabela->nFlags & BESQL_ACESSO_INSERIR) || !(nFlags & BESQL_ACESSO_INSERIR))
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

	//esconder o separador
	if (!GTK_WIDGET_VISIBLE (Janela->adicionar) && !GTK_WIDGET_VISIBLE (Janela->remover) && !GTK_WIDGET_VISIBLE (Janela->salvar))
		gtk_widget_hide (BE_interfaces_widget (Janela->window, "separatortoolitem5"));
	//corrigir acesso para remover
	if (Janela->SintaxeRemover != Sintaxe)
	{
		BE_sql_fechar_consulta (Janela->SintaxeRemover);
		Janela->SintaxeRemover = Sintaxe;
	}
	//esconder relatórios
	if (!Janela->sArquivoFonte || !Janela->sArquivoSVG_ListaPadrao || !Janela->sArquivoSVG_FormularioPadrao)
		gtk_widget_hide (Janela->relatorios);

	//esconder opções
	if (!bOpcoes)
		gtk_widget_hide (Janela->opcoes);
	//esconder perfil
	if (!Janela->SintaxePerfil)
		gtk_widget_hide (Janela->perfil);
	//esconder o separador
	if (!GTK_WIDGET_VISIBLE (Janela->opcoes) && !GTK_WIDGET_VISIBLE (Janela->perfil))
		gtk_widget_hide (BE_interfaces_widget (Janela->window, "separatortoolitem7"));

	//linhas
	if (Sintaxe)
		Sintaxe->lLinhasPaginaMax = BETABELA_LINHAS;
	//consulta inicial
	if (bAtualizar)
		_be_Interface_sinal_windowTabela_on_toolbuttonPrimeiro_clicked (NULL, Janela);

	//meus sinais
	g_signal_connect (Janela->tabela, "destroy", G_CALLBACK (_be_Tabela_on_destroy), Janela);//fechar tudo

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	//foco inicial
	gtk_widget_grab_focus (Janela->treeview);
	//cores alternadas
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela->treeview), TRUE);

	return Janela->tabela;
}

gboolean be_Tabela_atualizar (GtkWidget *Tabela)
{
	GtkWidget *Filho = NULL, *Toolbar = NULL, *Botao = NULL;
	GList *Filhos = NULL, *ListaL = NULL;

	//procurar toolbar
	Filhos = gtk_container_get_children (GTK_CONTAINER (Tabela));
	for (ListaL = Filhos; ListaL; ListaL = g_list_next (ListaL))
	{
		Filho = (GtkWidget *) ListaL->data;
		if (g_str_equal ("toolbarTabela", gtk_widget_get_name (Filho)))
		{
			Toolbar = Filho;
			break;
		}
	}
	BE_list_free (Filhos);
	//procurar botão voltar
	if (Toolbar)
	{
		Filhos = gtk_container_get_children (GTK_CONTAINER (Toolbar));
		for (ListaL = Filhos; ListaL; ListaL = g_list_next (ListaL))
		{
			Filho = (GtkWidget *) ListaL->data;
			if (g_str_equal ("toolbuttonVoltar", gtk_widget_get_name (Filho)))
			{
				Botao = Filho;
				break;
			}
		}
	}
	BE_list_free (Filhos);
	//atualizar
	if (Botao)
	{
		g_signal_emit_by_name (Botao, "clicked", NULL);
		return TRUE;
	}

	return FALSE;
}

void _be_Tabela_adicionar_colunas (_BETabela_Janela *Janela)
{
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;
	GtkTreeModel *model = NULL;
	BESQL_Campo *Campo = NULL, *CampoAlterar = NULL;
	GSList *ListaL = NULL;
	guint nColunaAtiva = 0;
	gchar *sTemp = NULL;
	gboolean bSomenteLeitura = FALSE;

	if (!Janela->Sintaxe)
		return;

	//loop nos campos
	for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)//somente os ativos
		{
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
					else if (!CampoAlterar->bAlteravel || g_str_equal (Campo->sNome, BESQL_CAMPO_ID) || be_sql_campo_binario (CampoAlterar))
						bSomenteLeitura = TRUE;
					else
						bSomenteLeitura = FALSE;
				}
				else
					bSomenteLeitura = TRUE;
			}
			else if (!Campo->bAlteravel || be_sql_campo_binario (Campo))
				bSomenteLeitura = TRUE;

			//colunas
			col = gtk_tree_view_column_new ();
			BE_free (sTemp);
			sTemp = BE_ISO_UTF8 (Campo->sNome);
			gtk_tree_view_column_set_title (col, sTemp);//título, aceita acentuações
			gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento, centro
			gtk_tree_view_column_set_min_width (col, BETABELA_COLUNAS_LARGURA_MINIMA);
			gtk_tree_view_column_set_max_width (col, BETABELA_COLUNAS_LARGURA_MAXIMA);
			gtk_tree_view_column_set_resizable (col, FALSE);//largura não variavel (não altera com o mouse)
			//gtk_tree_view_column_set_clickable (col, TRUE);
			gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_GROW_ONLY);
            //preparar modelo
			model = NULL;
			//renderizar combo (drop-down)
			if (!bSomenteLeitura && Campo->sParametros && (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC))
			{
				renderer = gtk_cell_renderer_combo_new ();
				model = _be_Tabela_modelo_combo_atualizar (Janela, Campo, NULL, NULL);
				g_object_set (renderer, "model", model, "text-column", 0, "has-entry", TRUE, NULL);//dados do combo
				//alterar largura mínima
				gtk_tree_view_column_set_min_width (col, BETABELA_COLUNAS_LARGURA_MINIMA_COMBO);
			}
			//renderizar combo para campos com possibilidade de nulos
			/*else if (!bSomenteLeitura && !Campo->sParametros && Campo->bAnulavel && !be_sql_campo_numerico (Campo) && !be_sql_campo_logico (Campo))
			{
				renderer = gtk_cell_renderer_combo_new ();
				model = _be_Tabela_modelo_combo_atualizar (Janela, Campo, NULL, NULL);
				g_object_set (renderer, "model", model, "text-column", 0, "has-entry", TRUE, NULL);//dados do combo
			}*/
			//renderizar texto, BESQL_TIPO_HIE e BESQL_TIPO_EXE
			else
			{
				//verificar se há parâmetros em campos sem tratamento específico
				if (Campo->sParametros)
					bSomenteLeitura = TRUE;
				renderer = gtk_cell_renderer_text_new ();
			}

			//renderizar geral, atributos existentes
			gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
			gtk_tree_view_column_add_attribute (col, renderer, "markup", nColunaAtiva);//exibição
			gtk_tree_view_column_add_attribute (col, renderer, "text", nColunaAtiva + 1);//original, puro
			gtk_tree_view_column_add_attribute (col, renderer, "background", nColunaAtiva + 2);//cor de fundo
			gtk_tree_view_column_add_attribute (col, renderer, "editable", nColunaAtiva + 3);
			//sinais da célula
			if (!bSomenteLeitura)
			{
				g_signal_connect (renderer, "editing-started", G_CALLBACK (_be_Tabela_cell_editing_started), Janela);
				g_signal_connect (renderer, "editing-canceled", G_CALLBACK (_be_Tabela_cell_editing_canceled), Janela);
				g_signal_connect (renderer, "edited", G_CALLBACK (_be_Tabela_cell_edited), Janela);
			}

			//alinhamento
			if (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE)
				g_object_set (renderer, "xalign", 0.0, NULL);//esquerda
			else if (be_sql_campo_texto (Campo))
				g_object_set (renderer, "xalign", 0.0, NULL);//esquerda
			else if (be_sql_campo_numerico (Campo))
				g_object_set (renderer, "xalign", 1.0, NULL);//direita
			else
				g_object_set (renderer, "xalign", 0.5, NULL);//centro

			gtk_tree_view_append_column (GTK_TREE_VIEW (Janela->treeview), col);//adicionando coluna ao treeview
			//célula
			g_object_set_data (G_OBJECT (renderer), "coluna", GINT_TO_POINTER (nColunaAtiva));//criando propriedades
			g_object_set_data (G_OBJECT (renderer), "campo", Campo);//criando propriedades
			g_object_set_data (G_OBJECT (renderer), "combo", model);//criando propriedades
			//coluna
			g_object_set_data (G_OBJECT (col), "coluna", GINT_TO_POINTER (nColunaAtiva));//criando propriedades
			g_object_set_data (G_OBJECT (col), "campo", Campo);//criando propriedades
			g_object_set_data (G_OBJECT (col), "combo", model);//criando propriedades
			g_object_set_data (G_OBJECT (col), "somenteleitura", GINT_TO_POINTER (bSomenteLeitura));//criando propriedades
			//definir coluna inicial para localizar
			if (nColunaAtiva == 0)
				Janela->colLocalizar = col;

			nColunaAtiva += BETABELA_PROPRIEDADES_POR_COLUNA;
		}
	}

	BE_free (sTemp);
}

gchar *_be_Tabela_localizar_valores (_BETabela_Janela *Janela, BESQL_Campo *CampoL, gchar *path)
{
	BESQL_Campo *Campo = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeIter iter;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sText = NULL, *sText1 = NULL;
	gchar *sValor = NULL;
	gint nCol = 0;

	//verificar linha
	if (!path || strlen (path) == 0)
		return NULL;

	//recuperar modelo
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	//selecionar linha
	if (gtk_tree_model_get_iter_from_string (model, &iter, path))
	{
		//loop das colunas
		nCol = 0;
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			//somente campo definido
			if (g_str_equal (CampoL->sNome, Campo->sNome))
			{
				//recuperar valores em HTML
				BE_free (sText);
				BE_free (sText1);
				gtk_tree_model_get (model, &iter, nCol, &sText, -1);//recuperar anterior
				gtk_tree_model_get (model, &iter, nCol + 1, &sText1, -1);//recuperar original, sem tags
				//verificar alterações
				if (!g_str_equal (sText, sText1) && g_strrstr (sText, BETABELA_ALTERADO_TAG))
				{
					//verificar alterações
					BE_free (sTemp);
					BE_free (sTemp1);
					sTemp = be_strextract (sText, ">", "</span>", 0);
					sTemp1 = BE_UTF8_ISO (sTemp);
					BE_free (sTemp);
					sValor = be_markup_text (sTemp1);
				}
				else
					sValor = g_strdup (sText1);
			}
			nCol += BETABELA_PROPRIEDADES_POR_COLUNA;
		}
	}

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);

	return sValor;
}

GtkTreeModel *_be_Tabela_modelo_combo_atualizar (_BETabela_Janela *Janela, BESQL_Campo *Campo, GtkListStore *store, gchar *path)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *CampoTemp = NULL, *CampoExibir = NULL, *CampoID = NULL;
	GtkTreeIter iter;
	GSList *ListaL = NULL;
	gchar **aLista = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCont = 0;
	gboolean bSQL = FALSE;

	if (store)
	{
        gtk_list_store_clear (store);
        bSQL = TRUE;
	}
    else
    {
		store = gtk_list_store_new (1, G_TYPE_STRING);
		if (!bSQLCarregado)
		{
			bSQLCarregado = TRUE;
			bSQL = TRUE;
		}
    }

	//lista de parâmetros
	if (bSQL && Campo->sParametros && Campo->nTipo != BESQL_TIPO_EXE)
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
						sTemp = _be_Tabela_localizar_valores (Janela, CampoTemp, path);
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
						sTemp = _be_Tabela_localizar_valores (Janela, CampoTemp, path);
						//adicionar filtro
						if (sTemp && strlen(sTemp) > 0)
							be_sql_filtro_adicionar (Sintaxe, CampoTemp, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_E, 0, FALSE);
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
						//primeiro ítem o valor atual
						gtk_list_store_append (store, &iter);
						gtk_list_store_set (store, &iter, 0, "Atual", -1);
						//ordenar por exibição
						be_sql_ordem_adicionar (Sintaxe, CampoExibir, BESQL_ORDENACAO_ASC);
						//linhas
						Sintaxe->lLinhasPaginaMax = BETABELA_LINHAS_COMBO + 1;
						if (be_sql_selecionar (Sintaxe, 1, &sErros) > 0)
						{
							//loop dos dados da primeira página
							nCont = 0;
							while (be_sql_selecionar_proxima_linha (Sintaxe) && nCont < BETABELA_LINHAS_COMBO)
							{
								BE_free (sTemp);
								BE_free (sTemp1);
								sTemp1 = be_sql_campo_valor_limitado_formatado (CampoExibir, BETABELA_LENSTRING_COMBO, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
								if (sTemp1)
								{
									gtk_list_store_append (store, &iter);
									gtk_list_store_set (store, &iter, 0, sTemp1, -1);
								}
								nCont++;
							}
						}
						//indicar que há mais linhas
						if (nCont >= BETABELA_LINHAS_COMBO)
						{
							//indicar no fim
							gtk_list_store_append (store, &iter);
							gtk_list_store_set (store, &iter, 0, BETABELA_MAIS_COMBO, -1);
						}
						//possibilidade de alterações
						gtk_list_store_append (store, &iter);
						gtk_list_store_set (store, &iter, 0, BETABELA_ALTERAR_COMBO, -1);
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
	//opção de anular o valor do campo
	else if (Campo->bAnulavel && !be_sql_campo_numerico (Campo) && !be_sql_campo_logico (Campo))
	{
		//primeiro ítem o valor atual
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, "Atual", -1);
		//segundo ítem é nulo
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, BETABELA_NULO, -1);
	}
	BE_free (sTemp);
	BE_free (sTemp1);

	return GTK_TREE_MODEL (store);
}

gchar *_be_Tabela_mais_combo (_BETabela_Janela *Janela, BESQL_Campo *Campo, gchar *path, gchar *sTexto)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *CampoExibir = NULL, *CampoID = NULL, *CampoTemp = NULL;
	gchar **aLista = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL, *sResultado = NULL;

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

	if (Sintaxe  && (Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
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
				sTemp = _be_Tabela_localizar_valores (Janela, CampoTemp, path);
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
				sTemp = _be_Tabela_localizar_valores (Janela, CampoTemp, path);
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
				{
					BE_free (sTemp);
					sTemp = be_sql_campo_valor_limitado_formatado (CampoExibir, BETABELA_LENSTRING_COMBO, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
					//formatando exibição
					sResultado = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s</span><span background='#FFFF00' foreground='#FFFF00' size='small'>[#%s#]</span>\n<span size='small'>%s</span>", sTemp, CampoID->sValor, sTexto);//o indice do combo fica oculto, com a cor de fundo igual da fonte

				}
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

GtkTreeModel *_be_Tabela_criar_modelo (_BETabela_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	GtkListStore *store = NULL;
	GType *pTypes = NULL;//colunas
	GSList *ListaL = NULL;
	gint nColunaAtiva = 0, nColunas = 0;

	//criando modelo
	nColunas = be_sql_campo_quantidade_ativos (Janela->Sintaxe) * BETABELA_PROPRIEDADES_POR_COLUNA;
	if (nColunas > 0)
	{
		pTypes = g_new (GType, nColunas);
		//atribuindo tipo
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo)//somente os ativos
			{
				pTypes[nColunaAtiva] = G_TYPE_STRING;//texto formatado
				pTypes[nColunaAtiva + 1] = G_TYPE_STRING;//texto puro, original
				pTypes[nColunaAtiva + 2] = G_TYPE_STRING;//cor de fundo
				pTypes[nColunaAtiva + 3] = G_TYPE_BOOLEAN;//editável

				nColunaAtiva += BETABELA_PROPRIEDADES_POR_COLUNA;
			}
		}
		store = gtk_list_store_newv (nColunas, pTypes);
		BE_free (pTypes);
	}

	return GTK_TREE_MODEL (store);
}

void _be_Tabela_atualizar (_BETabela_Janela *Janela, gboolean bTudo)
{
	GtkTreeIter iter;
	GtkTreeModel *model = NULL;
	GtkTreeViewColumn *col = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Ordem *Ordem = NULL;
	GSList *ListaL = NULL;
	GList *Cols = NULL, *Renders = NULL;
	guint nColunaAtiva = 0;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sErros = NULL;
	gboolean bSomenteLeitura = FALSE;

	//desabilitar os botões
	gtk_widget_set_sensitive (Janela->posterior, FALSE);
	gtk_widget_set_sensitive (Janela->relatorios, FALSE);
	gtk_widget_set_sensitive (Janela->remover, FALSE);
	gtk_widget_set_sensitive (Janela->salvar, FALSE);

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//atualizar tudo
	if (bTudo)
	{
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela->treeview), NULL);
		//limpar tudo
		for (Cols = gtk_tree_view_get_columns (GTK_TREE_VIEW (Janela->treeview)); Cols; Cols= g_list_next (Cols))
		{
			for (Renders = gtk_tree_view_column_get_cell_renderers (GTK_TREE_VIEW_COLUMN (Cols->data)); Renders; Renders = g_list_next (Renders))
			{
				gtk_tree_view_column_clear_attributes (GTK_TREE_VIEW_COLUMN (Cols->data), GTK_CELL_RENDERER (Renders->data));
				gtk_object_destroy (Renders->data);
			}
			Renders = g_list_first (Renders);
			BE_list_free (Renders);

			gtk_tree_view_column_clear (GTK_TREE_VIEW_COLUMN (Cols->data));
			gtk_tree_view_remove_column (GTK_TREE_VIEW (Janela->treeview), GTK_TREE_VIEW_COLUMN (Cols->data));
		}
		Cols = g_list_first (Cols);
		BE_list_free (Cols);
	}
	else
	{
		//recuperar modelo
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	}
	if (!model)//criar pela primeira vez as colunas e o modelo
	{
		//modelo de dados
		model = _be_Tabela_criar_modelo (Janela);
		if (model)
		{
			gtk_tree_view_set_model (GTK_TREE_VIEW (Janela->treeview), model);
			g_object_unref (model);
			//colunas
			_be_Tabela_adicionar_colunas (Janela);
		}
	}
	//liberando travas
	if (Janela->sBloqueioID)
	{
		be_sql_liberar_bloqueio_CIA (Janela->SintaxeAlterar, Janela->sBloqueioID, &sErros);
		BE_free (Janela->sBloqueioID);
	}
	//excluindo as antigas
	if (model)
		gtk_list_store_clear (GTK_LIST_STORE (model));
	//executar
	if (be_sql_selecionar (Janela->Sintaxe, Janela->nPagina, &sErros) > 0 && model)
	{
		//recuperar a primeira ordem
		if (Janela->Sintaxe->Ordens)
			Ordem = (BESQL_Ordem *) Janela->Sintaxe->Ordens->data;
		//informação
		BE_free (sTemp);
		if (Janela->Sintaxe->lLinhas < 0)
			sTemp = g_strdup_printf ("<span foreground='#000000' size='large'><b>%d de ?</b></span>", (gint) be_sql_selecionar_proxima_linha (Janela->Sintaxe));
		else
			sTemp = g_strdup_printf ("<span foreground='#000000' size='large'><b>%d de %d</b></span>", (gint) be_sql_selecionar_proxima_linha (Janela->Sintaxe), (gint) Janela->Sintaxe->lLinhas);
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
		//carregando os dados
		do
		{
			gtk_list_store_append (GTK_LIST_STORE (model), &iter);
			nColunaAtiva = 0;
			for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				if (Campo->bAtivo)//somente os ativos
				{
					//recuperar coluna atual
					col = gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColunaAtiva / BETABELA_PROPRIEDADES_POR_COLUNA);
					bSomenteLeitura = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (col), "somenteleitura"));
					//verificar ordenação
					if (Ordem && Ordem->Campo == Campo)
					{
						gtk_tree_view_column_set_sort_indicator (col, TRUE);
						if (Ordem->nOrdem == BESQL_ORDENACAO_ASC)
							gtk_tree_view_column_set_sort_order (col, GTK_SORT_ASCENDING);
						else
							gtk_tree_view_column_set_sort_order (col, GTK_SORT_DESCENDING);
					}
					else
						gtk_tree_view_column_set_sort_indicator (col, FALSE);
					//existe um valor não nulo
					if (Campo->sValor)
					{
						//imagem ou arquivo (binário)
						if (be_sql_campo_binario (Campo))
						{
							BE_free (sTemp);
							BE_free (sTemp1);
							sTemp1 = be_sql_campo_valor_limitado_formatado (Campo, BETABELA_LENSTRING_BIN, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8 | BESQL_CAMPOS_LIMITAR_FORMATO_XML);
							if (!sTemp1)
								sTemp1 = g_strdup ("");
							if (Campo->nComprimento > 1073741824)
								sTemp = g_strdup_printf ("<span foreground='%s'>%s (%.3f GB)</span>", BEGLOBAL_COR_BLOQUEADA_FONTE, sTemp1, (gfloat) Campo->nComprimento / 1073741824.0);
							else if (Campo->nComprimento > 1048576)
								sTemp = g_strdup_printf ("<span foreground='%s'>%s (%.2f MB)</span>", BEGLOBAL_COR_BLOQUEADA_FONTE, sTemp1, (gfloat) Campo->nComprimento / 1048576.0);
							else if (Campo->nComprimento > 1024)
								sTemp = g_strdup_printf ("<span foreground='%s'>%s (%.1f KB)</span>", BEGLOBAL_COR_BLOQUEADA_FONTE, sTemp1, (gfloat) Campo->nComprimento / 1204.0);
							else
								sTemp = g_strdup_printf ("<span foreground='%s'>%s (%d B)</span>", BEGLOBAL_COR_BLOQUEADA_FONTE, sTemp1, Campo->nComprimento);
							BE_free (sTemp1);
							sTemp1 = BE_ISO_UTF8 (sTemp);
							gtk_list_store_set (GTK_LIST_STORE (model), &iter,
								nColunaAtiva, sTemp1,
								nColunaAtiva + 1, "",
								nColunaAtiva + 2, BEGLOBAL_COR_BLOQUEADA,
								nColunaAtiva + 3, FALSE,
								-1);//formatado e puro
						}
						else
						{
							BE_free (sTemp);
							BE_free (sTemp1);
							sTemp = be_sql_campo_valor_limitado_formatado (Campo, BETABELA_LENSTRING, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);//Campo->nComprimento
							if (Campo->nComprimento > BETABELA_LENSTRING || bSomenteLeitura)
							{
								sTemp1 = g_markup_printf_escaped ("<span foreground='%s'>%s</span>", BEGLOBAL_COR_BLOQUEADA_FONTE, sTemp); //BEGLOBAL_COR_BLOQUEADA_FONTE
								gtk_list_store_set (GTK_LIST_STORE (model), &iter,
									nColunaAtiva, sTemp1,
									nColunaAtiva + 1, sTemp,
									nColunaAtiva + 2, BEGLOBAL_COR_BLOQUEADA,
									nColunaAtiva + 3, FALSE,
									-1);//formatado e puro
							}
							else
							{
								sTemp1 = g_markup_printf_escaped ("%s", sTemp);
								gtk_list_store_set (GTK_LIST_STORE (model), &iter,
									nColunaAtiva, sTemp1,
									nColunaAtiva + 1, sTemp,
									nColunaAtiva + 3, TRUE,
									-1);//formatado e puro
							}
						}
					}
					else//valor é nulo
					{
						if (bSomenteLeitura)
						{
							gtk_list_store_set (GTK_LIST_STORE (model), &iter,
								nColunaAtiva, BETABELA_NULO_TAG,
								nColunaAtiva + 1, BETABELA_NULO,
								nColunaAtiva + 2, BEGLOBAL_COR_BLOQUEADA,
								nColunaAtiva + 3, FALSE,
								-1);//formatado e puro
						}
						else
						{
							gtk_list_store_set (GTK_LIST_STORE (model), &iter,
								nColunaAtiva, BETABELA_NULO_TAG,
								nColunaAtiva + 1, BETABELA_NULO,
								nColunaAtiva + 3, TRUE,
								-1);//formatado e puro
						}
					}

					nColunaAtiva += BETABELA_PROPRIEDADES_POR_COLUNA;
				}
			}
		} while (be_sql_selecionar_proxima_linha (Janela->Sintaxe));
		//habilitar os botões
		gtk_widget_set_sensitive (Janela->posterior, TRUE);
		gtk_widget_set_sensitive (Janela->relatorios, TRUE);
	}
	else
	{
		sTemp = g_strdup ("<span foreground='#FF0000' size='large'><i>Nenhum</i></span>");
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
		BE_free (sTemp);
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		gtk_widget_set_sensitive (Janela->adicionar, FALSE);
	}
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);
	//foco na tabela
	gtk_widget_grab_focus (Janela->treeview);

	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Tabela_cell_editing_started (GtkCellRenderer *renderer, GtkCellEditable *editable, gchar *path, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	GtkEntry *entry = NULL;
	GtkComboBox *combo = NULL;
	GtkTreeModel *model = NULL;
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	gchar *sText = NULL;
	gint nCol = 0;

	//recuperar informações original
	Janela = (_BETabela_Janela *) user_data;
	Janela->renderer = renderer;//célula de edição atual
	Janela->editable = editable;//controle de edição atual
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	store = g_object_get_data (G_OBJECT (renderer), "combo");
	nCol = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (renderer), "coluna"));//recuperando propriedade criada
	Campo = g_object_get_data (G_OBJECT (renderer), "campo");//recuperando propriedade criada
	//alterar na tela
	if (gtk_tree_model_get_iter_from_string (model, &iter, path))
	{
		gtk_tree_model_get (model, &iter, nCol + 1, &sText, -1);//recuperar original

		//texto
		if (GTK_IS_ENTRY (editable))
		{
			entry = GTK_ENTRY (editable);
			gtk_entry_set_text (entry, sText);
			//comprimento na edição
			if (be_sql_campo_logico (Campo))
				gtk_entry_set_max_length (entry, 3);
			else
				gtk_entry_set_max_length (entry, Campo->nComprimentoMax > BETABELA_LENSTRING ? BETABELA_LENSTRING : Campo->nComprimentoMax);
		}
		//combo
		else if (GTK_IS_COMBO_BOX_ENTRY (editable))
		{
			//atualizar lista
			_be_Tabela_modelo_combo_atualizar (Janela, Campo, store, path);

			combo = GTK_COMBO_BOX (editable);
			gtk_combo_box_remove_text (combo, 0);//excluir o primeiro ítem
			gtk_combo_box_prepend_text (combo, sText);//adicionar um novo primeiro ítem
			gtk_combo_box_set_active (combo, 0);//selecionar o primeiro ítem
		}
	}

	BE_free (sText);
}

void _be_Tabela_cell_editing_canceled (GtkCellRenderer *renderer, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;

	//recuperar informações original
	Janela = (_BETabela_Janela *) user_data;
	Janela->renderer = NULL;//cancelar célula atual
	Janela->editable = NULL;//cancelar célula atual
	//manter o foco na tabela
	if (Janela->treeview && GTK_IS_WIDGET (Janela->treeview))
        gtk_widget_grab_focus (Janela->treeview);
}

void _be_Tabela_cell_edited (GtkCellRendererText *cellrenderertext, gchar *arg1, gchar *arg2, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	GtkTreeModel *model = NULL;
	GtkListStore *store = NULL;
	gchar **aLista = NULL;
	GtkTreeIter iter;
	gchar *sText = NULL, *sText1 = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL, *sTemp3 = NULL;
	gchar *sSQL = NULL;
	gchar *sErros = NULL;
	gint nCol = 0;
	gboolean bManterFoco = TRUE;

	//recuperar informações
	Janela = (_BETabela_Janela *) user_data;
	Janela->renderer = NULL;//cancelar célula atual
	Janela->editable = NULL;//cancelar célula atual
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	SQL = Janela->Sintaxe->Tabela->BD->SQL;
	//alterar na tela
	if (arg2 && gtk_tree_model_get_iter_from_string (model, &iter, arg1))
	{
		//recuperando propriedades criadas
		nCol = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cellrenderertext), "coluna"));
		Campo = g_object_get_data (G_OBJECT (cellrenderertext), "campo");
		store = g_object_get_data (G_OBJECT (cellrenderertext), "combo");

		gtk_tree_model_get (model, &iter, nCol, &sText, -1);//recuperar anterior
		gtk_tree_model_get (model, &iter, nCol + 1, &sText1, -1);//recuperar original
		//não alterar se os valores forem iguais
		if (g_str_equal (sText1, arg2))
		{
			sTemp = g_markup_printf_escaped ("%s",sText1);
			gtk_list_store_set (GTK_LIST_STORE (model), &iter, nCol, sTemp, -1);//adicionar novo
		}
		else
		{
			//conversão lógica
			if (be_sql_campo_logico (Campo))
			{
				//formatando exibição
				if (strlen (arg2) > 0 && (arg2[0] == 's' || arg2[0] == 'S' || arg2[0] == 'v' || arg2[0] == 'V' || arg2[0] == '1'))
					sTemp = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s</span>\n<span size='small'>%s</span>", "Sim", sText1);
				else
					sTemp = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s</span>\n<span size='small'>%s</span>", "NÃ£o", sText1);
				//adicionar novo
				gtk_list_store_set (GTK_LIST_STORE (model), &iter, nCol, sTemp, -1);
				//ativar botão salvar
				gtk_widget_set_sensitive (Janela->salvar, TRUE);
			}
			else
			{
				//mais combo
				if (g_str_equal (arg2, BETABELA_MAIS_COMBO) && (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE))
				{
					//receber valor formatado
					BE_free (sTemp);
					sTemp = _be_Tabela_mais_combo (Janela, Campo, arg1, sText1);
					if (sTemp)
					{
						//adicionar novo
						gtk_list_store_set (GTK_LIST_STORE (model), &iter, nCol, sTemp, -1);
						//ativar botão salvar
						gtk_widget_set_sensitive (Janela->salvar, TRUE);
					}
				}
				//alterar combo
				else if (g_str_equal (arg2, BETABELA_ALTERAR_COMBO) && (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE))
				{
					_be_Tabela_alterar_combo (Janela, Campo);
					bManterFoco = FALSE;
				}
				//texto normal
				else
				{
					//corrigir comprimento
					if (strlen (arg2) > (Campo->nComprimentoMax > BETABELA_LENSTRING ? BETABELA_LENSTRING : Campo->nComprimentoMax))
						arg2[(Campo->nComprimentoMax > BETABELA_LENSTRING ? BETABELA_LENSTRING : Campo->nComprimentoMax)] = '\0';

					//verificar necessidade de localização automática, para combos
					if (strlen (arg2) > 0 && Campo->sParametros && Campo->nTipo != BESQL_TIPO_EXE)
					{
						aLista = g_strsplit (Campo->sParametros, "; ", -1);
							//`ID1`; `vis_Tabela2`; `ID2`; `Exibir2`. A relação (JOIN) para exibição já estará dentro dos Views.
							//(O separador é ponto-e-vírgula e espaço.)
							//aLista[0] = `ID1`
							//aLista[1] = `vis_Tabela2`
							//aLista[2] = `ID2`
							//aLista[3] = `Exibir2`
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
							sTemp3 = BE_UTF8_ISO (arg2);
							sSQL = g_strdup_printf ("SELECT `%s` FROM `%s` WHERE `%s`='%s' LIMIT 1;", sTemp1, sTemp, sTemp2, sTemp3);
							//executar consulta
							if (be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sTemp1, &sErros) > 0)
							{
								//formatando exibição localizada
								BE_free (sTemp);
								sTemp = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s</span><span background='#FFFF00' foreground='#FFFF00' size='small'>[#%s#]</span>\n<span size='small'>%s</span>", arg2, sTemp1, sText1);//o indice do combo fica oculto, com a cor de fundo igual da fonte
							}
							else
							{
								BE_free (sTemp);
								sTemp = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s ?</span><span background='#FFFF00' foreground='#FFFF00' size='small'>[#%s#]</span>\n<span size='small'>%s</span>", arg2, arg2, sText1);
							}
							//informar erros
							if (sErros)
							{
								be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
								BE_free (sErros);
							}
						}
						else
						{
							//formatando exibição
							BE_free (sTemp);
							sTemp = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s</span>\n<span size='small'>%s</span>", arg2, sText1);
						}
						BE_strfreev (aLista);
					}
					else
					{
						//formatando exibição
						BE_free (sTemp);
						sTemp = g_markup_printf_escaped ("<span background='#FFFF00' weight='bold'>%s</span>\n<span size='small'>%s</span>", arg2, sText1);
					}
					gtk_list_store_set (GTK_LIST_STORE (model), &iter, nCol, sTemp, -1);
					//ativar botão salvar
					gtk_widget_set_sensitive (Janela->salvar, TRUE);
				}
			}
		}
	}
	//manter o foco na tabela
    if (bManterFoco && Janela->treeview && GTK_IS_WIDGET (Janela->treeview))
        gtk_widget_grab_focus (Janela->treeview);

	//limpar
	BE_free (sText);
	BE_free (sText1);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sTemp3);
	BE_free (sSQL);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonPrimeiro_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;

	Janela = (_BETabela_Janela *) user_data;
	//posicao inicial
	Janela->nPagina = 1;
	//atualizar
	_be_Tabela_atualizar (Janela, FALSE);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonVoltar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;

	Janela = (_BETabela_Janela *) user_data;
	if (Janela->nPagina > 1)
		Janela->nPagina--;
	//atualizar
	_be_Tabela_atualizar (Janela, FALSE);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonAvancar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;

	Janela = (_BETabela_Janela *) user_data;
	Janela->nPagina++;
	_be_Tabela_atualizar (Janela, FALSE);//atualizar	
}

void _be_Interface_sinal_windowTabela_on_toolbuttonUltimo_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;

	Janela = (_BETabela_Janela *) user_data;

	//informação do pensamento
	sTemp = g_strdup ("<span foreground='#008800' size='large'><i>Procesando quantidade...</i></span>");
	gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	BE_free (sTemp);
	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);
	//contar linhas
	be_sql_contar_linhas (Janela->Sintaxe, &sErros);
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);
	//posicao final
	Janela->nPagina = be_sql_paginas (Janela->Sintaxe);
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	else
		_be_Tabela_atualizar (Janela, FALSE);//atualizar
}

void _be_Interface_sinal_windowTabela_on_toolbuttonAdicionar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL, *CampoAdicionar = NULL;
	BESQL_Filtro *Filtro = NULL;
	GtkTreeViewColumn *col = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GSList *ListaL = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;
	gboolean bAtualizar = TRUE, bSomenteLeitura = FALSE;
	guint nColunaAtiva = 0;
	gint nCont = 0;

	Janela = (_BETabela_Janela *) user_data;

	//salvar alterações pendente; análise futura
	//_be_Interface_sinal_windowTabela_on_toolbuttonSalvar_clicked (NULL, user_data);

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->adicionar) || !GTK_WIDGET_VISIBLE (Janela->adicionar))
		return;
	//confirmar esta operação
	if (Janela->bConfAdicionar)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Deseja realmente adicionar uma nova linha ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;

		if (g_slist_length (Janela->Sintaxe->Filtros) > 0 && be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "O filtro está ativo, isto poderá alterar algum valor padrão.\nContinuar assim mesmo ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}

	//informação do pensamento
	sTemp = g_strdup ("<span foreground='#008800' size='large'><i>Adicionando...</i></span>");
	gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	BE_free (sTemp);
	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//lista de campos, definir valores padrões
	nColunaAtiva = 0;
	for (ListaL = Janela->Sintaxe->Campos, nCont = 0; ListaL; ListaL = g_slist_next (ListaL), nCont++)
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)
		{
			col = gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColunaAtiva / BETABELA_PROPRIEDADES_POR_COLUNA);
			bSomenteLeitura = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (col), "somenteleitura"));
			if (!bSomenteLeitura && Campo->sValorPadrao && strlen (Campo->sValorPadrao) > 0)
			{
				CampoAdicionar = be_sql_selecionar_campo (Janela->SintaxeAdicionar, Campo->sNome);
				if (CampoAdicionar)
				{
					BE_free (CampoAdicionar->sValorNovo);
					CampoAdicionar->sValorNovo = g_strdup (Campo->sValorPadrao);
					CampoAdicionar->bAfetar = TRUE;
				}
			}
			nColunaAtiva += BETABELA_PROPRIEDADES_POR_COLUNA;
		}
	}

	//lista de campos com filtros, definir valores padrões
    for (ListaL = Janela->Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
    {
        Filtro = (BESQL_Filtro *) ListaL->data;
        //preparar filtros
        if (Filtro->sValor && Filtro->bAtivo && (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_IGUAL) || g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_PARECIDO)))
        {
            CampoAdicionar = be_sql_selecionar_campo (Janela->SintaxeAdicionar, Filtro->Campo->sNome);
            if (CampoAdicionar)
            {
                BE_free (CampoAdicionar->sValorNovo);
                CampoAdicionar->sValorNovo = g_strdup (Filtro->sValor);
                CampoAdicionar->bAfetar = TRUE;
            }
        }
    }
	//lista de campos com filtros de União (oculto para usuário), utilizado na relação de formulários x subtabelas
    for (ListaL = Janela->Sintaxe->FiltrosUniao; ListaL; ListaL = g_slist_next (ListaL))
    {
        Filtro = (BESQL_Filtro *) ListaL->data;
        //preparar filtros
        if (Filtro->sValor && Filtro->bAtivo && (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_IGUAL) || g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_PARECIDO)))
        {
            CampoAdicionar = be_sql_selecionar_campo (Janela->SintaxeAdicionar, Filtro->Campo->sNome);
            if (CampoAdicionar)
            {
                BE_free (CampoAdicionar->sValorNovo);
                CampoAdicionar->sValorNovo = g_strdup (Filtro->sValor);
                CampoAdicionar->bAfetar = TRUE;
            }
        }
    }

	//adicionar
	if (Janela->bCIA)
	{
		if (!be_sql_adicionar_CIA (Janela->SintaxeAdicionar, &sErros))
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível adicionar uma linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
		}
	}
	else
	{
		if (be_sql_adicionar (Janela->SintaxeAdicionar, &sErros) != 1)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível adicionar uma linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
		}
	}
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
		_be_Interface_sinal_windowTabela_on_toolbuttonPrimeiro_clicked (NULL, Janela);
		//_be_Tabela_atualizar (Janela, FALSE);
		//mouse padrão
		BE_janela_mouse_normal (Mouse, FALSE);
	}
	else
	{
		//mouse padrão
		BE_janela_mouse_normal (Mouse, TRUE);
		//informação de erro com correção
		BE_free (sTemp);
		sTemp = g_strdup ("<span foreground='#008800' size='large'><i>? de ?</i></span>");
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	}

	BE_free (sTemp);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonRemover_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	GtkTreePath *path = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeViewColumn *col = NULL;
	GtkTreeIter iter;
	BEGlobal_Janela *Mouse = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL, *sText = NULL;
	gint nCol = 0;
	gboolean bAtualizar = FALSE;

	Janela = (_BETabela_Janela *) user_data;

	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	gtk_tree_view_get_cursor (GTK_TREE_VIEW (Janela->treeview), &path, &col);
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->remover) || !GTK_WIDGET_VISIBLE (Janela->remover) || !path)
		return;
	//confirmar esta operação
	if (Janela->bConfRemover)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "<b>Deseja realmente excluir esta linha ?</b>", GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}

	//informação do pensamento
	sTemp = g_strdup ("<span foreground='#008800' size='large'><i>Removendo...</i></span>");
	gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	BE_free (sTemp);
	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//recuperar iter
	gtk_tree_model_get_iter (model, &iter, path);
	//loop dos campos
	nCol = 0;
	for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)//somente os ativos
		{
			BE_free (sText);
			gtk_tree_model_get (model, &iter, nCol + 1, &sText, -1);//recuperar original

			if (Janela->bCIA)//CIA
			{
				if (g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
				{
					//obter trava
					if (be_sql_obter_bloqueio_excluir_CIA (Janela->SintaxeRemover, sText, &sErros))
					{
						//marcar trava, para futura liberação
						BE_free (Janela->sBloqueioID);
						Janela->sBloqueioID = g_strdup (sText);
						//remover e verificar
						if (!be_sql_excluir_CIA (Janela->SintaxeRemover, sText, &sErros))
						{
							be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível remover esta linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
						}
						else
							bAtualizar = TRUE;
					}
					else
					{
						BE_free (sTemp);
						sTemp = g_strdup_printf ("O usuário %d está bloqueando esta linha de dados.\nTente mais tarde.", be_sql_usuario_bloqueando_CIA (Janela->SintaxeAlterar, sText, &sErros));
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
					}
					break;
				}
			}
			else
			{
				//loop nos índices
				for (ListaL1 = Janela->Sintaxe->Indices; ListaL1; ListaL1 = g_slist_next (ListaL1))
				{
					Indice = (BESQL_Indice *) ListaL1->data;
					if (Indice->nTipo == BESQL_INDICE_PRIMARIA && Indice->Campo == Campo)
					{
						//verificar nulos
						if (Campo->bAnulavel && g_str_equal (sText, BETABELA_NULO))
							Indice->sValorFiltro = NULL;
						else
							Indice->sValorFiltro = BE_UTF8_ISO (sText);
						Indice->bAfetar = TRUE;
					}
				}
			}

			nCol += BETABELA_PROPRIEDADES_POR_COLUNA;
		}
	}
	//remover e verificar
	if (!Janela->bCIA && Indice)
	{
		if (be_sql_excluir_indice (Janela->SintaxeRemover, &sErros) <= 0)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível remover esta linha de dados.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
		}
		else
			bAtualizar = TRUE;
	}
	//faltando campo Indice
	else if (!Janela->bCIA)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna com índice para identificar esta linha.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
	}

	//verificar coluna ID
	if (Janela->bCIA && !bAtualizar)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna ID para identificar as linhas.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
	}

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
		_be_Tabela_atualizar (Janela, FALSE);
		//mouse padrão
		BE_janela_mouse_normal (Mouse, FALSE);
	}
	else
	{
		//mouse padrão
		BE_janela_mouse_normal (Mouse, TRUE);
		//informação de erro com correção
		BE_free (sTemp);
		sTemp = g_strdup ("<span foreground='#008800' size='large'><i>? de ?</i></span>");
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	}

	BE_free (sTemp);
	BE_free (sText);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonSalvar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL, *CampoID = NULL, *CampoAlterar = NULL;
	BESQL_Indice *Indice = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeIter iter;
	BEGlobal_Janela *Mouse = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sText = NULL, *sText1 = NULL, *sID = NULL;
	gchar *sErros = NULL;
	gint nLinha = 0, nCol = 0;
	gboolean bAlterar = FALSE, bAtualizar = TRUE;

	Janela = (_BETabela_Janela *) user_data;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->salvar) || !GTK_WIDGET_VISIBLE (Janela->salvar))
		return;

	//confirmar esta operação
	if (Janela->bConfSalvar)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Deseja realmente alterar a(s) linha(s) ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}
	//informação do pensamento
	sTemp = g_strdup ("<span foreground='#008800' size='large'><i>Salvando...</i></span>");
	gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	BE_free (sTemp);
	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);
	//recuperar modelo
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	//loop das linhas
	sTemp = g_strdup_printf ("%d", nLinha);
	while (gtk_tree_model_get_iter_from_string (model, &iter, sTemp))
	{
		//loop das colunas
		nCol = 0;
		bAlterar = FALSE;
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo)//somente os ativos
			{
				BE_free (sText);
				BE_free (sText1);
				gtk_tree_model_get (model, &iter, nCol, &sText, -1);//recuperar anterior
				gtk_tree_model_get (model, &iter, nCol + 1, &sText1, -1);//recuperar original

				//verificar chaves
				if (Janela->bCIA)
				{
					if (g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
					{
						CampoID = Campo;
						BE_free (sID);
						sID = g_strdup (sText1);
					}
				}
				else
				{
					//loop nos índices
					for (ListaL1 = Janela->Sintaxe->Indices; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Indice = (BESQL_Indice *) ListaL1->data;
						if (Indice->nTipo == BESQL_INDICE_PRIMARIA && Indice->Campo == Campo)
						{
							//verificar nulos
							if (Campo->bAnulavel && g_str_equal (sText1, BETABELA_NULO))
								Indice->sValorFiltro = NULL;
							else
								Indice->sValorFiltro = BE_UTF8_ISO (sText1);
							Indice->bAfetar = TRUE;
						}
					}
				}
				//verificar alterações
				if (!g_str_equal (sText, sText1) && g_strrstr (sText, BETABELA_ALTERADO_TAG))
				{
					BE_free (sTemp);
					BE_free (sTemp1);
					sTemp = be_strextract (sText, ">", "</span>", 0);
					sTemp1 = BE_UTF8_ISO (sTemp);
					BE_free (sTemp);
					sTemp = be_markup_text (sTemp1);

					if (Janela->bCIA)
					{
						CampoAlterar = be_sql_selecionar_campo (Janela->SintaxeAlterar, Campo->sNome);
						if (CampoAlterar)
						{
							//informar dados para alterar
							if (CampoAlterar->bAnulavel && g_str_equal (sTemp, BETABELA_NULO))
								CampoAlterar->sValorNovo = NULL;
							else if (Campo->nTipo == BESQL_TIPO_SEL || Campo->nTipo == BESQL_TIPO_OPC || Campo->nTipo == BESQL_TIPO_HIE)
							{
								CampoAlterar->sValorNovo = be_strextract (sText, ">[#", "#]<", 0);
								//verificar código direto
								if (!CampoAlterar->sValorNovo)
									CampoAlterar->sValorNovo = g_strdup (sTemp);
							}
							else
								CampoAlterar->sValorNovo = g_strdup (sTemp);
							CampoAlterar->bAfetar = TRUE;
							bAlterar = TRUE;
						}
					}
					else
					{
						//informar dados para alterar
						if (Campo->bAnulavel && g_str_equal (sTemp, BETABELA_NULO))
							Campo->sValorNovo = NULL;
						else
							Campo->sValorNovo = g_strdup (sTemp);
						Campo->bAfetar = TRUE;
						bAlterar = TRUE;
					}
				}
				nCol += BETABELA_PROPRIEDADES_POR_COLUNA;
			}
		}
		//alterar
		if (Janela->bCIA)//CIA
		{
			if (bAlterar && CampoID)
			{
				//obter trava
				if (be_sql_obter_bloqueio_CIA (Janela->SintaxeAlterar, sID, &sErros))
				{
					//marcar trava, para futura liberação
					BE_free (Janela->sBloqueioID);
					Janela->sBloqueioID = g_strdup (sID);
					//alterar e verificar
					if (be_sql_alterar_CIA (Janela->SintaxeAlterar, sID, &sErros))
					{
						//marcar alterações na tela
						nCol = 0;
						for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
						{
							Campo = (BESQL_Campo *) ListaL->data;
							if (Campo->bAtivo)//somente os ativos
							{
								BE_free (sText);
								BE_free (sText1);
								gtk_tree_model_get (model, &iter, nCol, &sText, -1);//recuperar anterior
								gtk_tree_model_get (model, &iter, nCol + 1, &sText1, -1);//recuperar original
								//verificar alterações
								if (!g_str_equal (sText, sText1) && g_strrstr (sText, BETABELA_ALTERADO_TAG))
									gtk_list_store_set (GTK_LIST_STORE (model), &iter, nCol, BETABELA_ALTERADO, -1);//adicionar novo
								nCol += BETABELA_PROPRIEDADES_POR_COLUNA;
							}
						}
					}
					else
					{
						//erros
						BE_free (sTemp);
						sTemp = g_strdup_printf ("Não foi possível alterar a linha da coluna ID = %s.", sID);
						be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
						bAtualizar = FALSE;

						//informar erros
						if (sErros)
						{
							be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
							BE_free (sErros);
						}
						break;
					}
				}
				else
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("O usuário %d está bloqueando a linha da coluna ID = %s.\nTente mais tarde.", be_sql_usuario_bloqueando_CIA (Janela->SintaxeAlterar, sID, &sErros), sID);
					be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
					bAtualizar = FALSE;
				}
			}
			//faltando campo ID
			else if (bAlterar)
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna ID para identificar as linhas.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
				bAtualizar = FALSE;
				break;
			}
		}
		else if (bAlterar && Indice)
		{
			//alterar e verificar
			if (be_sql_alterar_indice (Janela->SintaxeAlterar, &sErros) <= 0)
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível alterar uma linha.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
				bAtualizar = FALSE;
				break;
			}
		}
		//faltando campo Indice
		else if (bAlterar)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna com índice para identificar as linhas.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			bAtualizar = FALSE;
			break;
		}

		//informar erros
		if (sErros)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_free (sErros);
			bAtualizar = FALSE;
			break;
		}
		//avançar linha
		nLinha++
		BE_free (sTemp);
		sTemp = g_strdup_printf ("%d", nLinha);
	}
	//atualizar
	if (bAtualizar)
	{
		_be_Tabela_atualizar (Janela, FALSE);
		//mouse padrão
		BE_janela_mouse_normal (Mouse, FALSE);
	}
	else
	{
		//mouse padrão
		BE_janela_mouse_normal (Mouse, TRUE);
		//informação de erro com correção
		BE_free (sTemp);
		sTemp = g_strdup ("<span foreground='#008800' size='large'><i>? de ?</i></span>");
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	}

	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sText);
	BE_free (sText1);
	BE_free (sID);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonDados_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	GSList *Lista = NULL;
	gint nPos = -1;

	Janela = (_BETabela_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, "Localizar (Ctrl+L)");//0
	Lista = g_slist_append (Lista, "Filtrar (Ctrl+F)");//1
	Lista = g_slist_append (Lista, "Ordenar");//2
	Lista = g_slist_append (Lista, "Exportar (Ctrl+E)");//3
	nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Ajustar dados", Lista, 0);
	//limpar lista
	BE_slist_free (Lista);

	//selecionar opção
	switch (nPos)
	{
		case 0://localizar
			_be_Tabela_localizar (Janela);
			break;
		case 1://filtrar
			_be_Tabela_filtrar (Janela);
			break;
		case 2://ordenar
			_be_Tabela_ordem (Janela);
			break;
		case 3://exportar
			_be_Tabela_exportar (Janela);
			break;
	}
}

void _be_Tabela_localizar (_BETabela_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	BESQL_Filtro *Filtro = NULL;
	GtkTreePath *path = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeViewColumn *col = NULL;
	GtkTreeIter iter;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sValor = NULL;
	gint nCol = 0;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	gtk_tree_view_get_cursor (GTK_TREE_VIEW (Janela->treeview), &path, &col);
	if (col)
		Janela->colLocalizar = col;

	if (Janela->colLocalizar)
	{
		nCol = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (Janela->colLocalizar), "coluna"));//recuperando propriedade criada
		Campo = g_object_get_data (G_OBJECT (Janela->colLocalizar), "campo");//recuperando propriedade criada

		//imagem ou arquivo (binário), não tentar filtrar
		if (be_sql_campo_binario (Campo))
			return;

		//obter o último filtro
		if (Janela->Sintaxe->Filtros)
			Filtro = (BESQL_Filtro *) (g_slist_last (Janela->Sintaxe->Filtros))->data;
		//recuperar valor da tela
		if (path && gtk_tree_model_get_iter (model, &iter, path))
			gtk_tree_model_get (model, &iter, nCol + 1, &sValor, -1);//recuperar original
		//recuperar valor do filtro anterior
		else if (Filtro)
		{
			if (!Filtro->sValor || g_str_equal (Filtro->sValor, BETABELA_NULO))
				sValor = BE_ISO_UTF8 (BETABELA_NULO);
			else//excluir curingas para exibir
			{
				sValor = BE_ISO_UTF8 (Filtro->sValor + strlen (BESQL_CURINGA_TODOS));
				sValor[strlen (sValor) - strlen (BESQL_CURINGA_TODOS)] = '\0';
			}
		}
		//janela de interação
		sTemp1 = g_strdup_printf ("Localizar por <b><i>%s</i></b>", gtk_tree_view_column_get_title (Janela->colLocalizar));
		sTemp = be_Mensagens_in (GTK_WINDOW (Janela->window), TRUE, "Localizar", sTemp1, sValor);
		//filtrar
		if (sTemp)
		{
			BE_free (sTemp1);
			sTemp1 = sTemp;
			sTemp = BE_UTF8_ISO (sTemp1);

			//limpar filtro anterior
			if (Filtro)
			{
				BE_sql_filtro_excluir (Janela->Sintaxe, Filtro, FALSE);
			}
			//novo filtro
			if (strlen (sTemp) > 0)
			{
				//comparar com nulos
				if (g_str_equal (sTemp, BETABELA_NULO))
					be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_NULO, NULL, BESQL_RELACAO_E, 0, FALSE);
				else
				{
					//incluir curingas
					BE_free (sTemp1);
					sTemp1 = sTemp;
					sTemp = g_strdup_printf ("%s%s%s", BESQL_CURINGA_TODOS, sTemp1, BESQL_CURINGA_TODOS);
					//comparar
					be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_E, 0, FALSE);
				}
			}
			//atualizar
			Janela->nPagina = 1;
			_be_Tabela_atualizar (Janela, FALSE);
		}
	}

	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sValor);
}

void _be_Tabela_filtrar (_BETabela_Janela *Janela)
{
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	//janela Filtro
	if (be_Filtro (GTK_WINDOW (Janela->window), Janela->Sintaxe, Janela->SintaxePerfil))
	{
		//atualizar
		Janela->nPagina = 1;
		_be_Tabela_atualizar (Janela, FALSE);
	}
}

void _be_Tabela_ordem (_BETabela_Janela *Janela)
{
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	//janela Ordem
	if (be_Ordem (GTK_WINDOW (Janela->window), Janela->Sintaxe))
	{
		//atualizar
		Janela->nPagina = 1;
		_be_Tabela_atualizar (Janela, FALSE);
	}
}

void _be_Tabela_exportar (_BETabela_Janela *Janela)
{
	BEGlobal_Janela *Mouse = NULL;
	gchar *sTemp = NULL, *sNomeArquivo = NULL;
	gchar *sErros = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->dados) || !GTK_WIDGET_VISIBLE (Janela->dados))
		return;

	//verificar se há quantidade de linhas
	if (Janela->Sintaxe->lLinhas <= 0)
		_be_Interface_sinal_windowTabela_on_toolbuttonUltimo_clicked (GTK_TOOL_BUTTON (Janela->ultimo), Janela);

	//verificar e avisar sobre muitas linhas
	if (Janela->Sintaxe->lLinhas > BETABELA_AVISO_LINHAS)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Há muitos dados para serem exportados, este processo pode demorar.\nDeseja realmente continuar ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}
	//verificar e avisar sobre muitas, muitas linhas mesmo
	if (Janela->Sintaxe->lLinhas > BETABELA_SUPER_AVISO_LINHAS)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "<b>Tem certeza absoluta que realmente deseja continuar ?</b>", GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}

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
	//linhas, manter a quantidade da tabela e não padrão
	if (Janela->Sintaxe)
		Janela->Sintaxe->lLinhasPaginaMax = BETABELA_LINHAS;

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

void _be_Interface_sinal_windowTabela_on_toolbuttonRelatorios_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sNomeArquivo = NULL, *sNomeRelatorio = NULL;
	const gchar *sArquivo = NULL, *sArquivoFonte = NULL;
	gchar *sArquivo_Padrao = NULL;
	gchar *sErros = NULL;
	gint nPos = -1, nCol = 0;

	Janela = (_BETabela_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->relatorios) || !GTK_WIDGET_VISIBLE (Janela->relatorios))
		return;

	//verificar se há quantidade de linhas
	if (Janela->Sintaxe->lLinhas <= 0)
		_be_Interface_sinal_windowTabela_on_toolbuttonUltimo_clicked (GTK_TOOL_BUTTON (Janela->ultimo), Janela);

	//verificar e avisar sobre muitas linhas
	if (Janela->Sintaxe->lLinhas > BETABELA_AVISO_LINHAS)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Há muitos dados para serem impressos, este processo pode demorar.\nDeseja realmente continuar ?", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}
	//verificar e avisar sobre muitas, muitas linhas mesmo
	if (Janela->Sintaxe->lLinhas > BETABELA_SUPER_AVISO_LINHAS)
	{
		if (be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "<b>Tem certeza absoluta que realmente deseja continuar ?</b>", GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO) != GTK_RESPONSE_YES)
			return;
	}

	//criando a lista
	Lista = g_slist_append (Lista, g_strdup ("Lista Padrão"));//0
	Lista = g_slist_append (Lista, g_strdup ("Formulário Padrão"));//1
	Lista = g_slist_append (Lista, g_strdup ("Lista Padrão (salvar arquivo)"));//2
	Lista = g_slist_append (Lista, g_strdup ("Formulário Padrão (salvar arquivo)"));//3
	Lista = g_slist_append (Lista, g_strdup ("Formulário Padrão (salvar em vários arquivos)"));//4
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
		Lista = g_slist_append (Lista, g_strdup (sTemp));//5
		Lista = g_slist_append (Lista, g_strdup_printf ("%s (salvar arquivo)", sTemp));//6
		Lista = g_slist_append (Lista, g_strdup_printf ("%s (salvar em vários arquivos)", sTemp));//7
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
    sArquivo_Padrao = g_strdup_printf ("%s%s", sTemp, BETABELA_RELATORIO_ATUAL);
	//selecionar relatório
	switch (nPos)
	{
		case -1://sair
			break;
		case 0://lista padrão
			sArquivo = Janela->sArquivoSVG_ListaPadrao;
			sNomeArquivo = BE_ISO_UTF8 (sArquivo_Padrao);
			break;
		case 1://formulário padrão
			sArquivo = Janela->sArquivoSVG_FormularioPadrao;
			sNomeArquivo = BE_ISO_UTF8 (sArquivo_Padrao);
			break;
		case 2://lista padrão, salvar arquivo
			sArquivo = Janela->sArquivoSVG_ListaPadrao;
			sNomeArquivo = be_arquivo_nome_sugerido ("Lista Padrão", 0, TRUE);
			break;
		case 3://formulário padrão, salvar arquivo
			sArquivo = Janela->sArquivoSVG_FormularioPadrao;
			sNomeArquivo = be_arquivo_nome_sugerido ("Formulário Padrão", 0, TRUE);
			break;
		case 4://formulário padrão, salvar em vários arquivos
			sArquivo = Janela->sArquivoSVG_FormularioPadrao;
			sNomeArquivo = be_arquivo_nome_sugerido ("Formulário Padrão", 0, TRUE);
			break;
		default://personalizados
			sArquivo = (gchar *) g_slist_nth_data (Janela->Sintaxe->Tabela->Relatorios, (nPos - 5) / 3);
			if (((nPos - 5) % 3) == 0)//inteiro, exibir
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
	if (nPos == 2 || nPos == 3 || nPos == 4 || (nPos > 4 && ((nPos - 5) % 3) != 0))
	{
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, sNomeArquivo);
		BE_free (sNomeArquivo);
		sNomeArquivo = be_Salvar (GTK_WINDOW (Janela->window), be_arquivo_localidade_UTF8 (), "*.pdf(Documento PDF)\n*.*(Todos)", sTemp);
	}
	//opção de quebrar em vários arquivos
	if (sNomeArquivo && (nPos == 4 || (nPos > 6 && ((nPos - 7) % 3) == 0)))
	{
		BE_free (sTemp);
		//criando a lista de campos
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo && !be_sql_campo_binario (Campo))//somente os ativos
				Lista = g_slist_append (Lista, Campo->sNome);
		}
		//janela para seleção
		nCol = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Coluna para complementar o início nome", Lista, 0);
		//limpar lista
		BE_slist_free (Lista);
		//selecionar o campo
		if (nCol >= 0)
			Campo = (BESQL_Campo *) g_slist_nth_data (Janela->Sintaxe->Campos, nCol);
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
	if (sArquivo && sNomeArquivo && nCol >= 0 && be_relatorio_gerar (sArquivo, sNomeArquivo, sArquivoFonte, Campo, sNomeRelatorio, Janela->Empresa, Janela->Sintaxe, &sErros))
	{
		//opções
		if (nPos == 2 || nPos == 3 || nPos == 4 || (nPos > 4 && ((nPos - 5) % 3) != 0))//informar relatório salvo
		{
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Relatório gravado em %s.", sNomeArquivo);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		}
		else//exibir
		{
			BE_free (sTemp);
			if (Janela->sLeitorPDF)
			{
				#ifdef G_OS_UNIX
					sTemp = g_strdup_printf ("%s %s", Janela->sLeitorPDF, sNomeArquivo);//preparar
    			#endif
				#ifdef G_OS_WIN32
					sTemp = g_strdup_printf ("\"%s\" \"%s\"", Janela->sLeitorPDF, sNomeArquivo);//preparar
				#endif
			}
			//abrir relatório
			//system
			if (!sTemp || !g_spawn_command_line_async (sTemp, NULL))//informar erros
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível abrir o leitor de PDF.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		}
	}
	//linhas, manter a quantidade da tabela e não do relatório
	if (Janela->Sintaxe)
		Janela->Sintaxe->lLinhasPaginaMax = BETABELA_LINHAS;

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
	BE_free (sArquivo_Padrao);
}

void _be_Interface_sinal_windowTabela_on_toolbuttonOpcoes_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	GSList *Lista = NULL;
	gint nPos = -1;

	Janela = (_BETabela_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, "Subtotais");//0
	Lista = g_slist_append (Lista, "Propriedades");//1
	if (Janela->SintaxeLog)
		Lista = g_slist_append (Lista, "Auditoria (log de ações)");//2
	if (Janela->func_Formulario)
		Lista = g_slist_append (Lista, "Abrir formulário (F3)");//3
	nPos = be_Lista (GTK_WINDOW (Janela->window), FALSE, "Opções", Lista, 0);
	//limpar lista
	BE_slist_free (Lista);

	//corrigir posições
	if (!Janela->SintaxeLog && nPos >= 2)
		nPos++;
	//selecionar opção
	switch (nPos)
	{
		case 0://subtotais
			_be_Tabela_subtotais (Janela);
			break;
		case 1://propriedades
			_be_Tabela_propriedades (Janela);
			break;
		case 2://auditoria
			_be_Tabela_auditoria (Janela);
			break;
		case 3://abrir formulários
			_be_Tabela_abrir_formulario (Janela);
			break;
	}
}

void _be_Tabela_subtotais (_BETabela_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GSList *ListaL = NULL;
	GString *Temp = NULL;
	gchar *sErros = NULL, *sTemp = NULL;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

	//totalizar linhas
	be_sql_totalizar_linhas (Janela->Sintaxe, &sErros);

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	else
	{
		Temp = g_string_new ("<b>Somatória: </b>\n");
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo && Campo->sTotal && strlen (Campo->sTotal) > 0)
			{
				BE_free (sTemp);
				//sTemp = g_markup_printf_escaped ("\n<i>%s</i> = <span foreground='#0000FF'>%s</span>", Campo->sNome, Campo->sTotal);//ESTÁ DANDO ERRO
				sTemp = g_strdup_printf ("\n%s = %s", Campo->sNome, Campo->sTotal);//SUBSTITUINDO O CÓDIGO ANTERIOR
				g_string_append_printf (Temp, "%s", sTemp);
			}
		}
		BE_free (sTemp);
		sTemp = BE_string_free (Temp);
		be_Mensagens_Ext (GTK_WINDOW (Janela->window), FALSE, "Subtotais", sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
	}

	BE_free (sTemp);
}

void _be_Tabela_auditoria (_BETabela_Janela *Janela)
{
	GtkWidget *window = NULL;
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
	be_sql_filtro_limpar (Janela->SintaxeLog, TRUE);
	Campo = be_sql_selecionar_campo (Janela->SintaxeLog, "Tabela");
	if (Campo)
	{
		Campo->bAtivo = FALSE;
		if (Janela->bCIA)
			be_sql_filtro_adicionar (Janela->SintaxeLog, Campo, BESQL_COMPARACAO_IGUAL, Janela->Sintaxe->Tabela->sNomeReal, BESQL_RELACAO_E, 0, TRUE);
		else
			be_sql_filtro_adicionar (Janela->SintaxeLog, Campo, BESQL_COMPARACAO_IGUAL, Janela->Sintaxe->Tabela->sNome, BESQL_RELACAO_E, 0, TRUE);
	}
	//anexar tabela de log
	gtk_container_add (GTK_CONTAINER (window), be_Tabela (Janela->SintaxeLog, NULL, NULL, window, BESQL_ACESSO_SELECIONAR, FALSE, Janela->sArquivoSVG_ListaPadrao, Janela->sArquivoSVG_FormularioPadrao, Janela->sArquivoFonte, Janela->sLeitorPDF, Janela->Empresa, TRUE, TRUE, Janela->func_Formulario, Janela->func_Tabela, Janela->user_data));
	//exibir
	gtk_widget_show (window);

	//mouse padrão
	BE_janela_mouse_normal (Mouse, FALSE);

	BE_free (sTemp);
}

void _be_Tabela_abrir_formulario (_BETabela_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeIter iter;
	BEGlobal_Janela *Mouse = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL, *Filtros = NULL;
	gboolean bManterFoco = TRUE;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCol = 0;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;
	//verificar formulário
	if (!Janela->func_Formulario)
		return;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);
	//seleção atual
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeview));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		//backup dos filtros
		Filtros = be_sql_filtro_duplicar_lista (Janela->Sintaxe->Filtros, NULL);
		//limpar filtro atual
		be_sql_filtro_limpar (Janela->Sintaxe, FALSE);

		//loop das colunas
		nCol = 0;
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo)//somente os ativos
			{
				gtk_tree_model_get (model, &iter, nCol + 1, &sTemp, -1);//recuperar original
				sTemp1 = BE_UTF8_ISO (sTemp);
				//verificar chaves
				if (Janela->bCIA)
				{
					if (g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
					{
						//aplicar filtro
						be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, sTemp1, BESQL_RELACAO_E, 0, FALSE);
						break;
					}
				}
				else
				{
					//loop nos índices
					for (ListaL1 = Janela->Sintaxe->Indices; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Indice = (BESQL_Indice *) ListaL1->data;
						if (Indice->nTipo == BESQL_INDICE_PRIMARIA && Indice->Campo == Campo)
						{
							//aplicar filtro
							be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, sTemp1, BESQL_RELACAO_E, 0, FALSE);
						}
					}
				}
				nCol += BETABELA_PROPRIEDADES_POR_COLUNA;
			}
		}

		//chamar formulário
		if (Janela->func_Formulario)
		{
			if (g_slist_length (Janela->Sintaxe->Filtros) > 0)
			{
				Janela->func_Formulario (Janela->Sintaxe->Tabela, Janela->Sintaxe->Filtros, Janela->user_data);
				bManterFoco = FALSE;
			}
			else
			//faltando campo Indice ou ID
			{
				be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não há uma coluna com índice para identificar a linha.", GTK_MESSAGE_WARNING, GTK_BUTTONS_OK);
			}
		}

		//limpar filtro atual
		be_sql_filtro_limpar (Janela->Sintaxe, FALSE);
		//restaurar filtros
		Janela->Sintaxe->Filtros = Filtros;
	}
	
	//mouse padrão
    BE_janela_mouse_normal (Mouse, bManterFoco);

	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Tabela_propriedades (_BETabela_Janela *Janela)
{
	GSList *Lista = NULL;
	gint nPos = -1;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, "Informações da tabela");//0
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
			_be_Tabela_info_tabela (Janela);
			break;
		case 1://Confirmações
			_be_Tabela_confirmacoes (Janela);
			break;
		case 2://Exibição
			_be_Tabela_exibir_campos (Janela);
			break;
		case 3://Valor padrão
			_be_Tabela_valor_padrao (Janela);
			break;
	}
}

void _be_Tabela_confirmacoes (_BETabela_Janela *Janela)
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

void _be_Tabela_exibir_campos (_BETabela_Janela *Janela)
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
		_be_Tabela_atualizar (Janela, TRUE);
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

void _be_Tabela_valor_padrao (_BETabela_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	GtkTreeViewColumn *col = NULL;
	GSList *ListaL = NULL, *Propriedades = NULL;
	gchar *sTemp = NULL, *sValor[g_slist_length (Janela->Sintaxe->Campos)];
	gint nCont = 0;
	gboolean bSomenteLeitura = FALSE;
	guint nColunaAtiva = 0;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->opcoes) || !GTK_WIDGET_VISIBLE (Janela->opcoes))
		return;
	if (!GTK_WIDGET_VISIBLE (Janela->adicionar))
		return;

	//criando a lista de campos
	nColunaAtiva = 0;
	for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL), nCont++)
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)
		{
			//recuperar coluna atual
			col = gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColunaAtiva / BETABELA_PROPRIEDADES_POR_COLUNA);
			bSomenteLeitura = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (col), "somenteleitura"));
			if (!bSomenteLeitura)
			{
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Valor padrão de '%s'", Campo->sNome);
				if (Campo->sValorPadrao)
					sValor[nCont] = g_strdup (Campo->sValorPadrao);
				else
					sValor[nCont] = g_strdup ("");
				Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_TEXTO, sTemp, &sValor[nCont], NULL);
			}
			else
				sValor[nCont] = NULL;

			nColunaAtiva += BETABELA_PROPRIEDADES_POR_COLUNA;
		}
		else
			sValor[nCont] = NULL;
	}

	//abrir janela
	if (be_Propriedades (GTK_WINDOW (Janela->window), Propriedades))
	{
		//lista de campos
		nColunaAtiva = 0;
		for (ListaL = Janela->Sintaxe->Campos, nCont = 0; ListaL; ListaL = g_slist_next (ListaL), nCont++)
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo)
			{
				col = gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColunaAtiva / BETABELA_PROPRIEDADES_POR_COLUNA);
				bSomenteLeitura = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (col), "somenteleitura"));
				if (!bSomenteLeitura)
				{
					BE_free (Campo->sValorPadrao);
					if (sValor[nCont] && strlen (sValor[nCont]) > 0)
						Campo->sValorPadrao = BE_UTF8_ISO (sValor[nCont]);
				}
				nColunaAtiva += BETABELA_PROPRIEDADES_POR_COLUNA;
			}
			BE_free (sValor[nCont]);
		}
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

void _be_Tabela_info_tabela (_BETabela_Janela *Janela)
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

void _be_Interface_sinal_windowTabela_on_toolbuttonPerfil_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	GSList *Lista = NULL, *ListaL = NULL, *ListaL1 = NULL;
	gchar *sErros = NULL;
	gint nPos = -1;
	gchar *sTemp = NULL, *sVariavel = NULL;

	Janela = (_BETabela_Janela *) user_data;
	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->perfil) || !GTK_WIDGET_VISIBLE (Janela->perfil))
		return;

	//criando a lista
	Lista = g_slist_append (Lista, g_strdup ("Salvar perfil"));//0
	Lista = g_slist_append (Lista, g_strdup ("Excluir perfil"));//1

	//perfil existente
	sTemp = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BETABELA_PERFIL);
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
			_be_Tabela_salvar_perfil (Janela);
			break;
		case 1://excluir
			_be_Tabela_excluir_perfil (Janela);
			break;
		default://carregar
			_be_Tabela_carregar_perfil (Janela, sVariavel);
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

void _be_Tabela_salvar_perfil (_BETabela_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	BESQL_Filtro *Filtro = NULL;
	BESQL_Ordem *Ordem = NULL;
	GSList *ListaL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sErros = NULL;
	gchar *sNome = NULL, *sVariavel = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCont = 0;

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
		sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BETABELA_PERFIL);
		//preparar perfil
		if (sVariavel)
		{
			//preparar confirmação
			g_key_file_set_boolean (Ini, "Confirmacao", "Adicionar", Janela->bConfAdicionar);
			g_key_file_set_boolean (Ini, "Confirmacao", "Salvar", Janela->bConfSalvar);
			g_key_file_set_boolean (Ini, "Confirmacao", "Remover", Janela->bConfRemover);
			//loop dos campos
			for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				//preparar exibição
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sNome);
				g_key_file_set_boolean (Ini, "Exibicao", sTemp, Campo->bAtivo);
				//preparar valor padrão
				if (Campo->sValorPadrao && strlen (Campo->sValorPadrao) > 0)
				{
					BE_free (sTemp1);
					sTemp1 = g_strescape (Campo->sValorPadrao, NULL);
					g_key_file_set_string (Ini, "Padrao", sTemp, sTemp1);
				}
			}
			//loop dos filtros
			nCont = 0;
			for (ListaL = Janela->Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
			{
				Filtro = (BESQL_Filtro *) ListaL->data;
				//preparar filtros
				BE_free (sTemp);
				BE_free (sTemp1);
				if (Filtro->sValor)
					sTemp = g_strdup_printf ("%d; %s; %s; %s; %d; %d", Filtro->bAtivo, Filtro->Campo->sNome, Filtro->sComparacao, Filtro->sValor, Filtro->nRelacao, Filtro->nGrupo);
				else
					sTemp = g_strdup_printf ("%d; %s; %s; ; %d; %d", Filtro->bAtivo, Filtro->Campo->sNome, Filtro->sComparacao, Filtro->nRelacao, Filtro->nGrupo);
				sTemp1 = g_strescape (sTemp, NULL);
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Filtro%d", nCont);
				g_key_file_set_string (Ini, "Filtro", sTemp, sTemp1);
				nCont++;
			}
			//loop das ordens
			nCont = 0;
			for (ListaL = Janela->Sintaxe->Ordens; ListaL; ListaL = g_slist_next (ListaL))
			{
				Ordem = (BESQL_Ordem *) ListaL->data;
				//preparar ordens
				BE_free (sTemp);
				BE_free (sTemp1);
				sTemp = g_strdup_printf ("%d; %s; %d", Ordem->bAtivo, Ordem->Campo->sNome, Ordem->nOrdem);
				sTemp1 = g_strescape (sTemp, NULL);
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Ordem%d", nCont);
				g_key_file_set_string (Ini, "Ordem", sTemp, sTemp1);
				nCont++;
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

void _be_Tabela_excluir_perfil (_BETabela_Janela *Janela)
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
	sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BETABELA_PERFIL);
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

void _be_Tabela_carregar_perfil (_BETabela_Janela *Janela, const gchar *sVariavel)
{
	BESQL_Campo *Campo = NULL;
	BESQL_Filtro *Filtro = NULL;
	BESQL_Ordem *Ordem = NULL;
	GSList *ListaL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sErros = NULL;
	gchar *sNome = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar **aValor = NULL;
	gint nCont = 0;

	//verificar botão
	if (!GTK_WIDGET_IS_SENSITIVE (Janela->perfil) || !GTK_WIDGET_VISIBLE (Janela->perfil))
		return;

	if (sVariavel && strlen (sVariavel) > 0)
	{
		//localizar existente
		sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BETABELA_PERFIL);
		if ((Ini = be_sql_perfil_ler_ini (Janela->SintaxePerfil, sNome, sVariavel, &sErros)))
		{
			//preparar confirmação
			Janela->bConfAdicionar = g_key_file_get_boolean (Ini, "Confirmacao", "Adicionar", NULL);
			Janela->bConfSalvar = g_key_file_get_boolean (Ini, "Confirmacao", "Salvar", NULL);
			Janela->bConfRemover = g_key_file_get_boolean (Ini, "Confirmacao", "Remover", NULL);

			//loop dos campos
			for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				//preparar exibição
				BE_free (sTemp);
				sTemp = BE_ISO_UTF8 (Campo->sNome);
				Campo->bAtivo = g_key_file_get_boolean (Ini, "Exibicao", sTemp, NULL);
				//preparar valor padrão
				BE_free (sTemp1);
				sTemp1 = g_key_file_get_string (Ini, "Padrao", sTemp, NULL);
				if (sTemp1 && strlen (sTemp1) > 0)
				{
					BE_free (sTemp);
					sTemp = g_strcompress (sTemp1);
					if (sTemp && strlen (sTemp) > 0)
					{
						BE_free (Campo->sValorPadrao);
						Campo->sValorPadrao = g_strdup (sTemp);
					}
				}
			}
			//preparar filtros
			be_sql_filtro_limpar (Janela->Sintaxe, FALSE);//limpar filtro atual
			nCont = 0;
			do
			{
				BE_free (sTemp);
				BE_free (sTemp1);
				sTemp1 = g_strdup_printf ("Filtro%d", nCont);
				sTemp = g_key_file_get_string (Ini, "Filtro", sTemp1, NULL);
				if (sTemp)
				{
					BE_free (sTemp1);
					sTemp1 = g_strcompress (sTemp);
					aValor = g_strsplit (sTemp1, "; ", -1);
					Campo = be_sql_selecionar_campo (Janela->Sintaxe, aValor[1]);
					if (Campo)
					{
						//filtrar
						if (aValor[3] && strlen (aValor[3]) > 0)
							Filtro = be_sql_filtro_adicionar (Janela->Sintaxe, Campo, aValor[2], aValor[3], atoi (aValor[4]), atoi (aValor[5]), FALSE);
						else
							Filtro = be_sql_filtro_adicionar (Janela->Sintaxe, Campo, aValor[2], NULL, atoi (aValor[4]), atoi (aValor[5]), FALSE);
						Filtro->bAtivo = atoi (aValor[0]);
					}
					BE_strfreev (aValor);
				}
				nCont++;
			}
			while (sTemp);
			//preparar ordens
			be_sql_ordem_limpar (Janela->Sintaxe);//limpar ordem atual
			nCont = 0;
			do
			{
				BE_free (sTemp);
				BE_free (sTemp1);
				sTemp1 = g_strdup_printf ("Ordem%d", nCont);
				sTemp = g_key_file_get_string (Ini, "Ordem", sTemp1, NULL);
				if (sTemp)
				{
					BE_free (sTemp1);
					sTemp1 = g_strcompress (sTemp);
					aValor = g_strsplit (sTemp1, "; ", -1);
					Campo = be_sql_selecionar_campo (Janela->Sintaxe, aValor[1]);
					if (Campo)
					{
						//ordenar
						Ordem = be_sql_ordem_adicionar (Janela->Sintaxe, Campo, atoi (aValor[2]));
						Ordem->bAtivo = atoi (aValor[0]);
					}
					BE_strfreev (aValor);
				}
				nCont++;
			}
			while (sTemp);

			//atualizar tudo
			if (Janela->renderer)
				gtk_cell_renderer_editing_canceled (Janela->renderer);//cancelar edição atual
			_be_Tabela_atualizar (Janela, TRUE);
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

void _be_Tabela_alterar_combo (_BETabela_Janela *Janela, BESQL_Campo *Campo)
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

gboolean _be_Interface_sinal_windowTabela_on_tableTabela_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	GtkTreePath *path = NULL;
	GtkTreeViewColumn *col = NULL;
	gboolean bSomenteLeitura = TRUE;

	Janela = (_BETabela_Janela *) user_data;

	//recuperar campo atual
	gtk_tree_view_get_cursor (GTK_TREE_VIEW (Janela->treeview), &path, &col);
	if (col)
	{
		Campo = g_object_get_data (G_OBJECT (col), "campo");//recuperando propriedade criada
		bSomenteLeitura = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (col), "somenteleitura"));//recuperando propriedade criada
	}

	//teclas de atalho, com Ctrl: gdk/gdkkeysyms.h
	if (event->state & GDK_CONTROL_MASK)
	{
		//verificar tecla pressionada
		switch (event->keyval)
		{
			//teclas de atalho para navegação
			case GDK_Home://Ctrl+Home
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//primeiro registro
				_be_Interface_sinal_windowTabela_on_toolbuttonPrimeiro_clicked (GTK_TOOL_BUTTON (Janela->primeiro), Janela);
				break;
			//teclas de atalho para navegação
			case GDK_Left://Ctrl+Esquerda
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//registro anterior
				_be_Interface_sinal_windowTabela_on_toolbuttonVoltar_clicked (GTK_TOOL_BUTTON (Janela->anterior), Janela);
				break;
			//teclas de atalho para navegação
			case GDK_Right://Ctrl+Direita
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//registro anterior
				_be_Interface_sinal_windowTabela_on_toolbuttonAvancar_clicked (GTK_TOOL_BUTTON (Janela->posterior), Janela);
				break;
			//teclas de atalho para navegação
			case GDK_End://Ctrl+End
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//registro anterior
				_be_Interface_sinal_windowTabela_on_toolbuttonUltimo_clicked (GTK_TOOL_BUTTON (Janela->ultimo), Janela);
				break;
			//teclas de atalho para adicionar
			case GDK_a://Ctrl+a
			case GDK_A://Ctrl+A
			case GDK_Insert://Ctrl+Ins
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//adicionar
				_be_Interface_sinal_windowTabela_on_toolbuttonAdicionar_clicked (GTK_TOOL_BUTTON (Janela->adicionar), Janela);
				break;
			//teclas de atalho para excluir
			case GDK_x://Ctrl+x
			case GDK_X://Ctrl+X
			case GDK_Delete://Ctrl+Del
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//excluir
				_be_Interface_sinal_windowTabela_on_toolbuttonRemover_clicked (GTK_TOOL_BUTTON (Janela->remover), Janela);
				break;
			//teclas de atalho para salvar
			case GDK_s://Ctrl+s
			case GDK_S://Ctrl+S
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//salvar
				_be_Interface_sinal_windowTabela_on_toolbuttonSalvar_clicked (GTK_TOOL_BUTTON (Janela->salvar), Janela);
				break;
			//teclas de atalho para ajustar dados
			case GDK_d://Ctrl+d
			case GDK_D://Ctrl+D
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//ajustar dados
				_be_Interface_sinal_windowTabela_on_toolbuttonDados_clicked (GTK_TOOL_BUTTON (Janela->dados), Janela);
				break;
			//teclas de atalho para localizar
			case GDK_l://Ctrl+l
			case GDK_L://Ctrl+L
				//cancelar edição atual, se houver
				if (Janela->renderer)
					gtk_cell_renderer_editing_canceled (Janela->renderer);
				//localizar
				_be_Tabela_localizar (Janela);
				break;
			//teclas de atalho para filtrar
			case GDK_f://Ctrl+f
			case GDK_F://Ctrl+F
				//cancelar edição atual, se houver
				if (Janela->renderer)
					gtk_cell_renderer_editing_canceled (Janela->renderer);
				//filtar
				_be_Tabela_filtrar (Janela);
				break;
			//teclas de atalho para exportar
			case GDK_e://Ctrl+e
			case GDK_E://Ctrl+E
				//cancelar edição atual, se houver
				if (Janela->renderer)
					gtk_cell_renderer_editing_canceled (Janela->renderer);
				//exportar
				_be_Tabela_exportar (Janela);
				break;
			//teclas de atalho para relatórios
			case GDK_p://Ctrl+p
			case GDK_P://Ctrl+P
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//relatório
				_be_Interface_sinal_windowTabela_on_toolbuttonRelatorios_clicked (GTK_TOOL_BUTTON (Janela->relatorios), Janela);
				break;
			//teclas de atalho para opções
			case GDK_o://Ctrl+o
			case GDK_O://Ctrl+O
				//cancelar edição atual, se houver
				if (Janela->renderer)
					gtk_cell_renderer_editing_canceled (Janela->renderer);
				//opções
				_be_Interface_sinal_windowTabela_on_toolbuttonOpcoes_clicked (GTK_TOOL_BUTTON (Janela->opcoes), Janela);
				break;
			//teclas de atalho para perfil
			case GDK_r://Ctrl+r
			case GDK_R://Ctrl+R
				//cancelar edição atual, se houver
                if (Janela->renderer)
                    gtk_cell_renderer_editing_canceled (Janela->renderer);
				//perfil
				_be_Interface_sinal_windowTabela_on_toolbuttonPerfil_clicked (GTK_TOOL_BUTTON (Janela->opcoes), Janela);
				break;
			//padrão, se ação
			default:
				return FALSE;
				break;
		}
	}
	//teclas de atalho, sem Ctrl: gdk/gdkkeysyms.h
	else
	{
		//verificar tecla pressionada
		switch (event->keyval)
		{
			//teclas de atalho para exibir combo pop-up
			case GDK_Page_Down://PgDown
			case GDK_Down://PgDown
				//exibir pop-up do combo, se estiver selecionado
				if (Janela->editable && GTK_IS_COMBO_BOX_ENTRY (Janela->editable))
					gtk_combo_box_popup (GTK_COMBO_BOX (Janela->editable));
				else
					//parar outras ações, não propagar o sinal
					return FALSE;
				break;
			//teclas de atalho para abrir formulário
			case GDK_F3://F3
				//cancelar edição atual, se houver
				if (Janela->renderer)
					gtk_cell_renderer_editing_canceled (Janela->renderer);
				//abrir formulário
				_be_Tabela_abrir_formulario (Janela);
				break;
			//teclas de atalho para atualizar
			case GDK_F5://F5
				//cancelar edição atual, se houver
				if (Janela->renderer)
					gtk_cell_renderer_editing_canceled (Janela->renderer);
				//atualizar
				_be_Tabela_atualizar (Janela, FALSE);
				break;
			case GDK_Return://ENTER
				//continuar, se estiver na chave primária
				if (bSomenteLeitura && be_sql_selecionar_indice (Janela->Sintaxe, "PRIMARY", Campo->sNome))
					//abrir formulário
					_be_Tabela_abrir_formulario (Janela);
				else
					//parar outras ações, não propagar o sinal
					return FALSE;
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

gboolean _be_Interface_sinal_windowTabela_on_treeviewTabela_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	//repassar
	return _be_Interface_sinal_windowTabela_on_tableTabela_key_press_event (widget, event, user_data);
}

gboolean _be_Interface_sinal_windowTabela_on_tableTabela_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	GtkTreePath *path = NULL;
	GtkTreeViewColumn *col = NULL;
	gboolean bSomenteLeitura = TRUE;

	Janela = (_BETabela_Janela *) user_data;

	//recuperar campo atual
	gtk_tree_view_get_cursor (GTK_TREE_VIEW (Janela->treeview), &path, &col);
	if (path && col)
	{
		Campo = g_object_get_data (G_OBJECT (col), "campo");//recuperando propriedade criada
		bSomenteLeitura = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (col), "somenteleitura"));//recuperando propriedade criada
	}

	//duplo-click e se estiver na chave primária
	if (path && bSomenteLeitura && (event->type == GDK_2BUTTON_PRESS) && be_sql_selecionar_indice (Janela->Sintaxe, "PRIMARY", Campo->sNome))
	{
		//abrir formulário
		_be_Tabela_abrir_formulario (Janela);
	}
	//continuar qualquer ação que envolva pressionamento de botões
	return FALSE;
}

gboolean _be_Interface_sinal_windowTabela_on_treeviewTabela_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	//repassar
	return _be_Interface_sinal_windowTabela_on_tableTabela_button_press_event (widget, event, user_data);
}

void _be_Interface_sinal_windowTabela_on_treeviewTabela_cursor_changed (GtkTreeView *treeview, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;

	Janela = (_BETabela_Janela *) user_data;
	//habilitar o botão remover
	gtk_widget_set_sensitive (Janela->remover, TRUE);
}

void _be_Tabela_on_destroy (GtkObject *object, gpointer user_data)
{
	_BETabela_Janela *Janela = NULL;

	Janela = (_BETabela_Janela *) user_data;
	if (!Janela->bLog)
		BE_sql_fechar_consulta (Janela->Sintaxe);
	BE_free (Janela->sBloqueioID);
	BE_free (Janela);

}
