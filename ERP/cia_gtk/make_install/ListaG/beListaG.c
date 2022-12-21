#include "beListaG.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowListaG.c"

gboolean be_ListaG (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe, const gchar *sSelAtual)
{
	_BEListaG_Janela Janela;
	guint nIDEvent = 0;

	//janela
	Janela.window = be_Interface_criar_windowListaG (winPai, NULL, NULL, &Janela);
	//lista
	Janela.treeview = BE_interfaces_widget (Janela.window, "treeviewListaG");
	//cores alternadas
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeview), TRUE);
	//localizar
	Janela.entry = BE_interfaces_widget (Janela.window, "entryLocalizar");
	gtk_widget_grab_focus (Janela.entry);//foco
	//label
	Janela.label = BE_interfaces_widget (Janela.window, "labelPosicao");
	//objetos da janela
	Janela.primeiro = BE_interfaces_widget (Janela.window, "toolbuttonPrimeiro");
	Janela.anterior = BE_interfaces_widget (Janela.window, "toolbuttonVoltar");
	Janela.posterior = BE_interfaces_widget (Janela.window, "toolbuttonAvancar");
	Janela.ultimo = BE_interfaces_widget (Janela.window, "toolbuttonUltimo");
	Janela.Sintaxe = Sintaxe;
	Janela.cronometro = NULL;
	Janela.bErros = FALSE;
	Janela.nPagina = 1;

	//linhas
	if (Sintaxe)
		Sintaxe->lLinhasPaginaMax = BELISTAG_LINHAS;
	//preparar cronômetro
	nIDEvent = g_timeout_add (500, _be_ListaG_timeout, &Janela);//chamar função a cada 500ms
	Janela.cronometro = g_timer_new();//criar cronômetro
	g_timer_stop (Janela.cronometro);//parar cronômetro
	//consulta inicial, forçar evento
	_be_Interface_sinal_windowListaG_on_toolbuttonPrimeiro_clicked (NULL, &Janela);
	//seleção inicial
	if (sSelAtual)
	{
		gtk_entry_set_text (GTK_ENTRY (Janela.entry), sSelAtual);
		g_timer_reset (Janela.cronometro);
		g_timer_start (Janela.cronometro);
	}
	//loop
	gtk_main ();

	g_timer_destroy (Janela.cronometro);
	g_source_remove (nIDEvent);
	//retornar
	return !Janela.bErros;
}

void _be_ListaG_adicionar_colunas (_BEListaG_Janela *Janela)
{
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	guint nColunaAtiva = 0;
	gchar *sTemp = NULL;

	//renderização das células
	renderer = gtk_cell_renderer_text_new ();
	//colunas
	for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo && !be_sql_campo_binario (Campo))//somente os ativos e não binários
		{
			col = gtk_tree_view_column_new ();
			sTemp = BE_ISO_UTF8 (Campo->sNome);
			gtk_tree_view_column_set_title (col, sTemp);//título, aceita acentuações
			gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento
			gtk_tree_view_column_set_resizable (col, TRUE);//largura variavel
			gtk_tree_view_column_set_min_width (col, 30);
			gtk_tree_view_column_set_max_width (col, 600);
			gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
			gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
			gtk_tree_view_column_add_attribute (col, renderer, "text", nColunaAtiva);//anexando os dados na renderização do texto
			gtk_tree_view_append_column (GTK_TREE_VIEW (Janela->treeview), col);//adicionando coluna ao treeview
			BE_free (sTemp);
			nColunaAtiva++;
		}
	}
}

GtkTreeModel *_be_ListaG_criar_modelo (_BEListaG_Janela *Janela)
{
	GtkListStore *store = NULL;
	GType *pTypes = NULL;//colunas
	guint nColuna = 0, nColunas = 0;

	//criando modelo
	nColunas = be_sql_campo_quantidade_ativos (Janela->Sintaxe);
	if (nColunas > 0)
	{
		pTypes = g_new (GType, nColunas);
		for (nColuna = 0; nColuna < nColunas; nColuna++)
			pTypes[nColuna] = G_TYPE_STRING;
		store = gtk_list_store_newv (nColunas, pTypes);
		BE_free (pTypes);
	}

	return GTK_TREE_MODEL (store);
}

void _be_ListaG_atualizar (_BEListaG_Janela *Janela)
{
	GtkTreeIter iter;
	GtkTreeModel *model = NULL;
	BEGlobal_Janela *Mouse = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	guint nColunaAtiva = 0;
	gchar *sTemp = NULL;
	gchar *sErros = NULL;

	//informação do pensamento
	sTemp = g_strdup ("<span foreground='#008800' size='large'><i>Selecionando...</i></span>");
	gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
	BE_free (sTemp);
	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);
	//recuperar modelo
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	if (!model)//criar pela primeira vez as colunas e o modelo
	{
		//modelo de dados
		model = _be_ListaG_criar_modelo (Janela);
		if (model)
		{
			gtk_tree_view_set_model (GTK_TREE_VIEW (Janela->treeview), model);
			g_object_unref (model);
			//colunas
			_be_ListaG_adicionar_colunas (Janela);
		}
	}
	//excluindo as antigas
	if (model)
		gtk_list_store_clear (GTK_LIST_STORE (model));
	//executar
	if (be_sql_selecionar (Janela->Sintaxe, Janela->nPagina, &sErros) > 0 && model)
	{
		//informação
		if (Janela->Sintaxe->lLinhas < 0)
			sTemp = g_strdup_printf ("<span foreground='#000000' size='large'><b>%d de ?</b></span>", (gint) be_sql_selecionar_proxima_linha (Janela->Sintaxe));
		else
			sTemp = g_strdup_printf ("<span foreground='#000000' size='large'><b>%d de %d</b></span>", (gint) be_sql_selecionar_proxima_linha (Janela->Sintaxe), (gint) Janela->Sintaxe->lLinhas);
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
		BE_free (sTemp);
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
					if (Campo->sValor)//existe um valor não nulo
					{
						sTemp = be_sql_campo_valor_limitado_formatado (Campo, BELISTAG_LENSTRING, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
						gtk_list_store_set (GTK_LIST_STORE (model), &iter, nColunaAtiva, sTemp, -1);
						BE_free (sTemp);
					}
					else//valor é nulo
						gtk_list_store_set (GTK_LIST_STORE (model), &iter, nColunaAtiva, BELISTAG_NULO, -1);
					nColunaAtiva++;
				}
			}
		} while (be_sql_selecionar_proxima_linha (Janela->Sintaxe));
		//desabilitar o botão avançar
		gtk_widget_set_sensitive (Janela->posterior, TRUE);
	}
	else
	{
		sTemp = g_strdup ("<span foreground='#FF0000' size='large'><i>Nenhum</i></span>");
		gtk_label_set_markup (GTK_LABEL (Janela->label), sTemp);
		BE_free (sTemp);
		//desabilitar o botão avançar
		gtk_widget_set_sensitive (Janela->posterior, FALSE);

	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		//informar retorno do erro
		Janela->bErros = TRUE;
	}
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);
}

void _be_Interface_sinal_windowListaG_on_treeviewLista_row_activated (GtkTreeView *treeview, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data)
{
	BESQL_Campo *Campo = NULL;
	GtkTreeModel *model;
	GtkTreeIter iter;
	_BEListaG_Janela *Janela = NULL;
	gchar *sLocalizar = NULL, *sErros = NULL;

	Janela = (_BEListaG_Janela *) user_data;
	model = gtk_tree_view_get_model (treeview);
	if (gtk_tree_model_get_iter (model, &iter, arg1))
	{
		gtk_tree_model_get (model, &iter, 0, &sLocalizar, -1);//selecionar
		//selecionar chave primária
		Campo = be_sql_selecionar_campo_indice (Janela->Sintaxe, BESQL_INDICE_PRIMARIA);
		//se não, selecionar chave unica
		if (!Campo)
			Campo = be_sql_selecionar_campo_indice (Janela->Sintaxe, BESQL_INDICE_UNICA);
		//se ainda não, selecionar primeiro campo
		if (!Campo)
			Campo = (BESQL_Campo *) Janela->Sintaxe->Campos->data;
		//limpar todos filtros atuais, da lista união
		be_sql_filtro_limpar (Janela->Sintaxe, TRUE);
		//verificar texto e filtrar
		if (strlen (sLocalizar) > 0)//filtrar
			be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_PARECIDO, sLocalizar, BESQL_RELACAO_OU, 0, TRUE);
		//atualizar
		be_sql_selecionar (Janela->Sintaxe, 1, &sErros);
		be_sql_selecionar_proxima_linha (Janela->Sintaxe);
		if (sErros)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_free (sErros);
			//informar retorno do erro
			Janela->bErros = TRUE;
		}
		//limpar
		BE_free (sLocalizar);
		//fechar janela
		gtk_widget_destroy (Janela->window);
	}
}

void _be_Interface_sinal_windowListaG_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
	BESQL_Campo *Campo = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	_BEListaG_Janela *Janela = NULL;
	gchar *sLocalizar = NULL, *sErros = NULL;

	Janela = (_BEListaG_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeview));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sLocalizar, -1);//selecionar
		//selecionar chave primária
		Campo = be_sql_selecionar_campo_indice (Janela->Sintaxe, BESQL_INDICE_PRIMARIA);
		//se não, selecionar chave unica
		if (!Campo)
			Campo = be_sql_selecionar_campo_indice (Janela->Sintaxe, BESQL_INDICE_UNICA);
		//se ainda não, selecionar primeiro campo
		if (!Campo)
			Campo = (BESQL_Campo *) Janela->Sintaxe->Campos->data;
		//limpar todos filtros atuais, da lista união
		be_sql_filtro_limpar (Janela->Sintaxe, TRUE);
		//verificar texto e filtrar
		if (strlen (sLocalizar) > 0)//filtrar
			be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_PARECIDO, sLocalizar, BESQL_RELACAO_OU, 0, TRUE);
		//atualizar
		be_sql_selecionar (Janela->Sintaxe, 1, &sErros);
		be_sql_selecionar_proxima_linha (Janela->Sintaxe);
		if (sErros)
		{
			be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_free (sErros);
			//informar retorno do erro
			Janela->bErros = TRUE;
		}
		//limpar
		BE_free (sLocalizar);
		//fechar janela
		gtk_widget_destroy (Janela->window);
	}
}

void _be_Interface_sinal_windowListaG_on_toolbuttonPrimeiro_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEListaG_Janela *Janela = NULL;

	Janela = (_BEListaG_Janela *) user_data;
	//posicao inicial
	Janela->nPagina = 1;
	//atualizar
	_be_ListaG_atualizar (Janela);
}

void _be_Interface_sinal_windowListaG_on_toolbuttonVoltar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEListaG_Janela *Janela = NULL;

	Janela = (_BEListaG_Janela *) user_data;
	if (Janela->nPagina > 1)
		Janela->nPagina--;
	//atualizar
	_be_ListaG_atualizar (Janela);
}

void _be_Interface_sinal_windowListaG_on_toolbuttonAvancar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEListaG_Janela *Janela = NULL;

	Janela = (_BEListaG_Janela *) user_data;
	Janela->nPagina++;
	_be_ListaG_atualizar (Janela);//atualizar
}

void _be_Interface_sinal_windowListaG_on_toolbuttonUltimo_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEListaG_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;

	Janela = (_BEListaG_Janela *) user_data;
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
		//informar retorno do erro
		Janela->bErros = TRUE;
	}
	else
		_be_ListaG_atualizar (Janela);//atualizar
}

void _be_Interface_sinal_windowListaG_on_entryLocalizar_changed (GtkEntry *entry, gpointer user_data)
{
	_BEListaG_Janela *Janela = NULL;

	Janela = (_BEListaG_Janela *) user_data;
	g_timer_reset (Janela->cronometro);
	g_timer_start (Janela->cronometro);
}

gboolean _be_ListaG_timeout (gpointer data)
{
	_BEListaG_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	const gchar *sLocalizar = NULL;
	gchar *sTemp = NULL;

	Janela = (_BEListaG_Janela *) data;
	if (g_timer_elapsed (Janela->cronometro, NULL) > 1.0)//esperar 1 seg após a digitação da última tecla
	{
		g_timer_stop (Janela->cronometro);
		g_timer_reset (Janela->cronometro);
		//limpar todos filtros atuais, da lista união
		be_sql_filtro_limpar (Janela->Sintaxe, TRUE);
		//verificar texto
		sLocalizar = gtk_entry_get_text (GTK_ENTRY (Janela->entry));
		if (strlen (sLocalizar) > 0)
		{
			sTemp = g_strdup_printf ("%s%s%s", BESQL_CURINGA_TODOS, sLocalizar, BESQL_CURINGA_TODOS);
			//loop nos campos
			for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				if (Campo->bAtivo && be_sql_campo_texto (Campo))//somente os ativos e textos
					be_sql_filtro_adicionar (Janela->Sintaxe, Campo, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_OU, 0, TRUE);
			}
		}
		//atualizar
		Janela->nPagina = 1;
		_be_ListaG_atualizar (Janela);
	}
	//limpar
	BE_free (sTemp);

	return TRUE;
}
