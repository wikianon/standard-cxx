#include "beOrdem.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowOrdem.c"

gboolean be_Ordem (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe)
{
	GtkTreeModel *model = NULL;
	_BEOrdem_Janela Janela;

	//objetos da janela
	Janela.bOK = FALSE;
	Janela.window = be_Interface_criar_windowOrdem (winPai, NULL, NULL, &Janela);
	Janela.treeviewDisp = BE_interfaces_widget (Janela.window, "treeviewDisponivel");
	Janela.treeviewSel = BE_interfaces_widget (Janela.window, "treeviewSelecionado");
	Janela.Sintaxe = Sintaxe;

	if (Sintaxe)
	{
	//lista disponível
		//modelo de dados
		model = _be_Ordem_criar_modelo_campo (Sintaxe);
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeviewDisp), model);
		g_object_unref (model);
		//colunas
		_be_Ordem_adicionar_colunas (GTK_TREE_VIEW (Janela.treeviewDisp), "DisponÃ­veis");
		//pesquisa interativa pela segunda coluna
		gtk_tree_view_set_search_column (GTK_TREE_VIEW (Janela.treeviewDisp), 0);
		//cores alternadas
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeviewDisp), TRUE);

	//lista utilizado
		//modelo de dados
		model = _be_Ordem_criar_modelo_ordem (Sintaxe);
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeviewSel), model);
		g_object_unref (model);
		//colunas
		_be_Ordem_adicionar_colunas (GTK_TREE_VIEW (Janela.treeviewSel), "Selecionados");
		//pesquisa interativa pela segunda coluna
		gtk_tree_view_set_search_column (GTK_TREE_VIEW (Janela.treeviewSel), 0);
		//cores alternadas
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeviewSel), TRUE);
	}
	else
		gtk_widget_set_sensitive (BE_interfaces_widget (Janela.window, "buttonOK"), FALSE);
	//foco inicial na primeira lista
	gtk_widget_grab_focus (Janela.treeviewDisp);

	gtk_main ();

	return Janela.bOK;
}

GtkTreeModel *_be_Ordem_criar_modelo_campo (BESQL_Sintaxe *Sintaxe)
{
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Ordem *Ordem = NULL;
	gchar *sTemp = NULL;
	gboolean bAchou = FALSE;

	//criando modelo
	store = gtk_list_store_new (1, G_TYPE_STRING);
	//loop
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		//loop das ordens
		bAchou = FALSE;
		for (ListaL1 = Sintaxe->Ordens; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Ordem = (BESQL_Ordem *) ListaL1->data;
			if (Ordem->Campo == Campo)
			{
				bAchou = TRUE;
				break;
			}
		}
		//não adicionar se já existe
		if (!bAchou)
		{
			sTemp = BE_ISO_UTF8 (Campo->sNome);
			gtk_list_store_append (store, &iter);
			gtk_list_store_set (store, &iter, 0, sTemp, -1);
		}
	}

	BE_free (sTemp);

	return GTK_TREE_MODEL (store);
}

GtkTreeModel *_be_Ordem_criar_modelo_ordem (BESQL_Sintaxe *Sintaxe)
{
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GSList *ListaL = NULL;
	BESQL_Ordem *Ordem = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//criando modelo
	store = gtk_list_store_new (1, G_TYPE_STRING);
	//loop
	for (ListaL = Sintaxe->Ordens; ListaL; ListaL = g_slist_next (ListaL))
	{
		Ordem = (BESQL_Ordem *) ListaL->data;
		sTemp = BE_ISO_UTF8 (Ordem->Campo->sNome);
		sTemp1 = g_strdup_printf ("[%s] - %s", Ordem->nOrdem == BESQL_ORDENACAO_ASC ? "a-z" : "z-a", sTemp);
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, sTemp1, -1);
	}

	BE_free (sTemp);
	BE_free (sTemp1);

	return GTK_TREE_MODEL (store);
}

void _be_Ordem_adicionar_colunas (GtkTreeView *treeview, gchar *sTitulo)
{
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;

	//renderização das células
	renderer = gtk_cell_renderer_text_new ();
	//colunas
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, sTitulo);//título, aceita acentuações
	gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento
	gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
	gtk_tree_view_column_add_attribute (col, renderer, "text", 0);//anexando os dados na renderização do texto
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);//adicionando coluna ao treeview
}

void _be_Interface_sinal_windowOrdem_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
	GtkTreeModel *model = NULL;
	GtkTreeIter iter;
	_BEOrdem_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	gchar *sItem = NULL, *sTemp = NULL;

	Janela = (_BEOrdem_Janela *) user_data;
	//limpar ordem atual
	be_sql_ordem_limpar (Janela->Sintaxe);
	//selecionado
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));
	//loop
	while (gtk_tree_model_get_iter_first (model, &iter))
	{
		BE_free (sItem);
		BE_free (sTemp);
		//selecionar
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);
		sTemp = BE_UTF8_ISO (sItem + 8);
		//remover
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
		//selecionar campo
		Campo = be_sql_selecionar_campo (Janela->Sintaxe, sTemp);
		if (Campo)
		{
			//adicionar ordem
			if (sItem[1] == 'a')
				be_sql_ordem_adicionar (Janela->Sintaxe, Campo, BESQL_ORDENACAO_ASC);
			else
				be_sql_ordem_adicionar (Janela->Sintaxe, Campo, BESQL_ORDENACAO_DESC);
		}
	}

	BE_free (sItem);
	BE_free (sTemp);

	//fechar janela
	Janela->bOK = TRUE;
	gtk_widget_destroy (Janela->window);
}

void _be_Interface_sinal_windowOrdem_on_buttonAdicionar_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEOrdem_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL, *sTemp = NULL;

	Janela = (_BEOrdem_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewDisp));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		sTemp = g_strdup_printf ("[a-z] - %s", sItem);
		//remover de disponível
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
		//adicionar em selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sTemp, -1);
		//foco na primeira lista
		if (Janela->treeviewDisp && GTK_IS_WIDGET (Janela->treeviewDisp))
            gtk_widget_grab_focus (Janela->treeviewDisp);
	}
	BE_free (sItem);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowOrdem_on_buttonRemover_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEOrdem_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL, *sTemp = NULL;

	Janela = (_BEOrdem_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		sTemp = g_strdup (sItem + 8);
		//remover de disponível
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
		//adicionar em selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sTemp, -1);
		//foco na primeira lista
		if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
            gtk_widget_grab_focus (Janela->treeviewSel);
	}
	BE_free (sItem);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowOrdem_on_buttonAcima_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEOrdem_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter, iterNext;
	gchar *sPath = NULL;
	gint nPath = 0;

	Janela = (_BEOrdem_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		nPath = atoi (sPath = gtk_tree_model_get_string_from_iter (model, &iter));
		BE_free (sPath);
		if (nPath > 0)
			if (gtk_tree_model_get_iter_from_string (model, &iterNext, sPath = g_strdup_printf ("%d", nPath - 1)))
			{
				gtk_list_store_swap (GTK_LIST_STORE (model), &iter, &iterNext);
				//foco na lista
				if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
                    gtk_widget_grab_focus (Janela->treeviewSel);
			}
	}
	BE_free (sPath);
}

void _be_Interface_sinal_windowOrdem_on_buttonAbaixo_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEOrdem_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter, iterNext;
	gchar *sPath = NULL;
	gint nPath = 0;

	Janela = (_BEOrdem_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		nPath = atoi (sPath = gtk_tree_model_get_string_from_iter (model, &iter));
		BE_free (sPath);
		if (gtk_tree_model_get_iter_from_string (model, &iterNext, sPath = g_strdup_printf ("%d", nPath + 1)))
		{
			gtk_list_store_swap (GTK_LIST_STORE (model), &iter, &iterNext);
			//foco na lista
			if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
                gtk_widget_grab_focus (Janela->treeviewSel);
		}
	}
	BE_free (sPath);
}

void _be_Interface_sinal_windowOrdem_on_buttonCrescente_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEOrdem_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL, *sTemp = NULL;

	Janela = (_BEOrdem_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		sTemp = g_strdup (sItem + 8);
		BE_free (sItem);
		sItem = g_strdup_printf ("[a-z] - %s", sTemp);
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sItem, -1);
		//foco na primeira lista
		if (Janela->treeviewSel&& GTK_IS_WIDGET (Janela->treeviewSel))
            gtk_widget_grab_focus (Janela->treeviewSel);
	}
	BE_free (sItem);
	BE_free (sTemp);
}

void _be_Interface_sinal_windowOrdem_on_buttonDecrescente_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEOrdem_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL, *sTemp = NULL;

	Janela = (_BEOrdem_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		sTemp = g_strdup (sItem + 8);
		BE_free (sItem);
		sItem = g_strdup_printf ("[z-a] - %s", sTemp);
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sItem, -1);
		//foco na primeira lista
		if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
            gtk_widget_grab_focus (Janela->treeviewSel);
	}
	BE_free (sItem);
	BE_free (sTemp);
}
