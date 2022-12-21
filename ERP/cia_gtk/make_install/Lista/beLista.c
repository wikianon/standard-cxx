#include "beLista.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowLista.c"

gint be_Lista (GtkWindow *winPai, gboolean bUTF8, const gchar *sTitulo, GSList *Lista, gint nPosicaoAtual)
{
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreePath *path = NULL;
	GtkTreeIter iter;
	_BELista_Janela Janela;
	gchar *sTemp = NULL;

	//propriedades da estrutura
	Janela.nPos = -1;
	Janela.bUTF8 = bUTF8;
	Janela.Lista = Lista;

	//criar janela
	Janela.window = be_Interface_criar_windowLista (winPai, NULL, NULL, &Janela);
	//tÌtulo da janela
	if (sTitulo)
	{
		if (bUTF8)
			gtk_window_set_title (GTK_WINDOW (Janela.window), sTitulo);
		else
		{
			BE_free (sTemp);
			sTemp = BE_ISO_UTF8 (sTitulo);
			gtk_window_set_title (GTK_WINDOW (Janela.window), sTemp);
		}
	}
	//lista
	Janela.treeview = BE_interfaces_widget (Janela.window, "treeviewLista");

	//modelo de dados
	model = _be_Lista_create_model (&Janela);
	gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeview), model);
	g_object_unref (model);
	//colunas
	_be_Lista_add_columns (GTK_TREE_VIEW (Janela.treeview));
	//pesquisa interativa pela segunda coluna
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (Janela.treeview), 1);
	//cores alternadas
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeview), TRUE);

	//foco inicial na lista
	gtk_widget_grab_focus (Janela.treeview);
	//comeÁar na posiÁ„o padr„o
	if (nPosicaoAtual >= 0 && nPosicaoAtual < g_slist_length (Lista))
	{
		selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela.treeview));
		gtk_tree_model_get_iter_from_string (model, &iter, sTemp = g_strdup_printf ("%d", nPosicaoAtual));
		BE_free (sTemp);
		gtk_tree_selection_select_iter (selection, &iter);
		path = gtk_tree_model_get_path (model, &iter);
		gtk_tree_view_set_cursor (GTK_TREE_VIEW (Janela.treeview), path, NULL, FALSE);
	}

	gtk_main ();

	return Janela.nPos;
}

GtkTreeModel *_be_Lista_create_model (_BELista_Janela *Janela)
{
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	gint nCont = 0;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL;

	//criando modelo
	store = gtk_list_store_new (2, G_TYPE_UINT, G_TYPE_STRING);
	//loop
	for (ListaL = Janela->Lista; ListaL; ListaL = g_slist_next (ListaL), nCont++)
	{
		gtk_list_store_append (store, &iter);
		if (Janela->bUTF8)
			gtk_list_store_set (store, &iter, 0, nCont, 1, (gchar *) ListaL->data, -1);
		else
		{
			BE_free (sTemp);
			sTemp = BE_ISO_UTF8 ((gchar *) ListaL->data);
			gtk_list_store_set (store, &iter, 0, nCont, 1, sTemp, -1);
		}
	}

	BE_free (sTemp);

	return GTK_TREE_MODEL (store);
}

void _be_Lista_add_columns (GtkTreeView *treeview)
{
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;
	gchar *sColunas[] = {"√çtem", "Texto"};
	gint nCont = 0;

	//renderizaÁ„o das cÈlulas
	renderer = gtk_cell_renderer_text_new ();

	//colunas
	for (nCont = 0; nCont < 2; nCont++)
	{
		col = gtk_tree_view_column_new();
		gtk_tree_view_column_set_title(col, sColunas[nCont]);//tÌtulo, aceita acentuaÁıes
		gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento
		gtk_tree_view_column_set_resizable (col, TRUE);//largura variavel
		//gtk_tree_view_column_set_sort_column_id (col, nCont);//ordenaÁ„o
		gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderizaÁ„o ‡ coluna
		gtk_tree_view_column_add_attribute (col, renderer, "text", nCont);//anexando os dados na renderizaÁ„o do texto
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);//adicionando coluna ao treeview
	}
}

void _be_Interface_sinal_windowLista_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
	_BELista_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;

	Janela = (_BELista_Janela *) user_data;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeview));
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &Janela->nPos, -1);//selecionar
		gtk_widget_destroy (Janela->window);//fechar janela
	}
}

void _be_Interface_sinal_windowLista_on_treeviewLista_row_activated (GtkTreeView *treeview, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data)
{
	_BELista_Janela *Janela = NULL;
	GtkTreeModel *model;
	GtkTreeIter iter;

	Janela = (_BELista_Janela *) user_data;
	model = gtk_tree_view_get_model (treeview);
	if (gtk_tree_model_get_iter (model, &iter, arg1))
	{
		gtk_tree_model_get (model, &iter, 0, &Janela->nPos, -1);//selecionar
		gtk_widget_destroy (Janela->window);//fechar janela
	}
}
