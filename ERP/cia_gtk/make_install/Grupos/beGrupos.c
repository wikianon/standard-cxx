#include "beGrupos.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowGrupos.c"

/*******************************************************************************
Janela para agrupamento de �tens.
Os s�tens est�o em dispostos em duas listas: Dispon�veis e Selecionados.
Na lista dos Selecionados tamb�m � poss�vel alterar a ordem dos �tens.
*******************************************************************************/

gboolean be_Grupos (GtkWindow *winPai, GSList **Disponivel, GSList **Selecionado)
{
/*******************************************************************************
Abre a janela de grupos.
Nesta janela cont�m uma lista dos �tens dispon�veis e outra dos �tens selecionados.
Todos os �tens s�o strings no formato UTF8.
Argumentos:
    winPai : janela parente, superior
    Disponivel : endere�o da lista dos �tens dispon�veis
    Selecionado : endere�o da lista dos �tens selecionados
    Retorno: se o usu�rio confirmou ou n�o, as mudan�as
*******************************************************************************/
	GtkTreeModel *model = NULL;
	_BEGrupos_Janela Janela;

	Janela.window = be_Interface_criar_windowGrupos (winPai, NULL, NULL, &Janela);
	//lista dos dispon�veis
	Janela.treeviewDisp = BE_interfaces_widget (Janela.window, "treeviewDisponivel");
		//modelo de dados
		model = _be_Grupos_create_model (*Disponivel);
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeviewDisp), model);
		g_object_unref (model);
		_be_Grupos_add_columns (GTK_TREE_VIEW (Janela.treeviewDisp), "Disponíveis");//colunas; nomes devem estar convertidos para UTF8
		gtk_tree_view_set_search_column (GTK_TREE_VIEW (Janela.treeviewDisp), 0);//pesquisa interativa pela segunda coluna
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeviewDisp), TRUE);//cores alternadas da lista

	//lista dos utilizado
	Janela.treeviewSel = BE_interfaces_widget (Janela.window, "treeviewSelecionado");
		//modelo de dados
		model = _be_Grupos_create_model (*Selecionado);
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeviewSel), model);
		g_object_unref (model);
		_be_Grupos_add_columns (GTK_TREE_VIEW (Janela.treeviewSel), "Selecionados");//colunas
		gtk_tree_view_set_search_column (GTK_TREE_VIEW (Janela.treeviewSel), 0);//pesquisa interativa pela segunda coluna
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeviewSel), TRUE);//cores alternadas da lista

	gtk_widget_grab_focus (Janela.treeviewDisp);//foco inicial na primeira lista

	//objetos da janela
	Janela.bOK = FALSE;
	Janela.Disponivel = Disponivel;
	Janela.Selecionado = Selecionado;

	//loop de intera��o com o usu�rio; s� sai daqui quando a janela Grupos for fechada
	gtk_main ();

	return Janela.bOK;
}

GtkTreeModel *_be_Grupos_create_model (GSList *Lista)
{
/*******************************************************************************
Cria��o do modelo de listas.
Fun��o somente para uso interno.
Argumentos:
    Lista : lista dos �tens
    Retorno: modelo de lista
*******************************************************************************/
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GSList *ListaL = NULL;

    store = gtk_list_store_new (1, G_TYPE_STRING);//criando modelo de strings
	//loop dos �tens da lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		gtk_list_store_append (store, &iter);//reservando espa�o pro �tem
		gtk_list_store_set (store, &iter, 0, (gchar *) ListaL->data, -1);//adicionando o �ten no modelo
	}

	return GTK_TREE_MODEL (store);
}

void _be_Grupos_add_columns (GtkTreeView *treeview, gchar *sTitulo)
{
/*******************************************************************************
Cria��o de modelo de colunas.
A coluna � criada e inserida diretamente no GtkTreeView.
Fun��o somente para uso interno.
Argumentos:
    treeview : GtkTreeView da lista
    sTitulo : nome da coluna
    Retorno: n�o retorna
*******************************************************************************/
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;

	renderer = gtk_cell_renderer_text_new ();//renderiza��o das c�lulas
	col = gtk_tree_view_column_new();//cria��o da coluna
	gtk_tree_view_column_set_title(col, sTitulo);//t�tulo, aceita acentua��es
	gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento, centralizado
	gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderiza��o � coluna
	gtk_tree_view_column_add_attribute (col, renderer, "text", 0);//anexando os dados na renderiza��o do texto
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);//adicionando coluna ao treeview
}

void _be_Interface_sinal_windowGrupos_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usu�rio clicar no bot�o OK, esta fun��o � chamado. Os �tens das duas listas (dispon�veis e selecionados) em GtkTreeView s�o sincronizados com as listas em GSList. A fun��o be_Grupos() retorna verdadeiro.
Fun��o chamada pelo evento do sinal on_buttonOK_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	//selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo de lista
		BE_slist_free (*Janela->Selecionado);//limpar lista
		//loop nos �tens selecionados
		while (gtk_tree_model_get_iter_first (model, &iter))
		{
			gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
			gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover
			*Janela->Selecionado = g_slist_append (*Janela->Selecionado, sItem);//adicionar na lista
		}
	//dispon�vel
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo de lista
		BE_slist_free (*Janela->Disponivel);//limpar lista
		//loop dis �tens dispon�veis
		while (gtk_tree_model_get_iter_first (model, &iter))
		{
			gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
			gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover
			*Janela->Disponivel = g_slist_append (*Janela->Disponivel, sItem);//adicionar na lista
		}

	//fechar janela
	Janela->bOK = TRUE;
	gtk_widget_destroy (Janela->window);//fechar janela Grupos
}

void _be_Interface_sinal_windowGrupos_on_buttonAdicionar_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Move o �tem selecionado na coluna dos Dispon�veis para a coluna dos Selecionados.
Fun��o chamada pelo evento do sinal on_buttonAdicionar_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperando �tem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover de dispon�vel
		//adicionar em selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo da lista
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);//reservando espa�o pro �tem
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sItem, -1);//adicionando o �tem
		//foco na primeira lista
		if (Janela->treeviewDisp && GTK_IS_WIDGET (Janela->treeviewDisp))
            gtk_widget_grab_focus (Janela->treeviewDisp);
	}
	BE_free (sItem);//limpar
}

void _be_Interface_sinal_windowGrupos_on_buttonRemover_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Move o �tem selecionado na coluna dos Selecionados para a coluna dos Dispon�veis.
Fun��o chamada pelo evento do sinal on_buttonRemover_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));//recuperando �tem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover de dispon�vel
		//adicionar em selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo da lista
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);//reservando espa�o pro �tem
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sItem, -1);//adicionando o �tem
		//foco na primeira lista
		if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
            gtk_widget_grab_focus (Janela->treeviewSel);
	}
	BE_free (sItem);//limpar
}

void _be_Interface_sinal_windowGrupos_on_buttonAcima_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Eleva o �tem selecionado na coluna dos Selecionados, trocando de posi��o com o superior.
Fun��o chamada pelo evento do sinal on_buttonAcima_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter, iterNext;
	gchar *sPath = NULL;
	gint nPath = 0;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));//recuperando �tem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		nPath = atoi (sPath = gtk_tree_model_get_string_from_iter (model, &iter));//recuperar hierarquia do �tem selecioando
		BE_free (sPath);//limpar
		if (nPath > 0)
		{
			if (gtk_tree_model_get_iter_from_string (model, &iterNext, sPath = g_strdup_printf ("%d", nPath - 1)))//recuperar �tem superior
			{
				gtk_list_store_swap (GTK_LIST_STORE (model), &iter, &iterNext);//trocar de posi��o com o �tem superior
				//foco na lista
				if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
                    gtk_widget_grab_focus (Janela->treeviewSel);
			}
		}
	}
	BE_free (sPath);
}

void _be_Interface_sinal_windowGrupos_on_buttonAbaixo_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Rebaixa o �tem selecionado na coluna dos Selecionados, trocando de posi��o com o inferior.
Fun��o chamada pelo evento do sinal on_buttonAbaixo_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter, iterNext;
	gchar *sPath = NULL;
	gint nPath = 0;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));//recuperando �tem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		nPath = atoi (sPath = gtk_tree_model_get_string_from_iter (model, &iter));//recuperar hierarquia do �tem selecioando
		BE_free (sPath);
		if (gtk_tree_model_get_iter_from_string (model, &iterNext, sPath = g_strdup_printf ("%d", nPath + 1)))//recuperar �tem inferior
		{
			gtk_list_store_swap (GTK_LIST_STORE (model), &iter, &iterNext);//trocar de posi��o com o �tem inferior
			//foco na lista
			if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
                gtk_widget_grab_focus (Janela->treeviewSel);
		}
	}
	BE_free (sPath);
}

void _be_Interface_sinal_windowGrupos_on_buttonTodos_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Move todos �tens da coluna dos Dispon�veis para a coluna dos Selecionados.
Fun��o chamada pelo evento do sinal on_buttonTodos_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *modelDisp = NULL, *modelSel = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	modelDisp = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo da lista dos dispon�veis
	modelSel = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo da lista dos selecionados
	//loop em todos os �tens da lista Dispon�veis
	while (gtk_tree_model_get_iter_first (modelDisp, &iter))
	{
		gtk_tree_model_get (modelDisp, &iter, 0, &sItem, -1);//selecionar
        gtk_list_store_remove (GTK_LIST_STORE (modelDisp), &iter);//remover
		gtk_list_store_append (GTK_LIST_STORE (modelSel), &iter);//reservara espa�o na lista Selecionados
		gtk_list_store_set (GTK_LIST_STORE (modelSel), &iter, 0, sItem, -1);//adicionar �tem
		BE_free (sItem);//limpar
	}
	//foco na segunda lista
	if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
        gtk_widget_grab_focus (Janela->treeviewSel);
}

void _be_Interface_sinal_windowGrupos_on_buttonNenhum_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Move todos �tens da coluna dos Selecionados para a coluna dos Dispon�veis.
Fun��o chamada pelo evento do sinal on_buttonNenhum_clicked.
Fun��o somente para uso interno.
Argumentos:
    widget : bot�o acionado
    user_data : dados repassados pela fun��o chamadora; estrutura de _BEGrupos_Janela
    Retorno: n�o retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *modelSel = NULL, *modelDisp = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	modelSel = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo da lista dos dispon�veis
	modelDisp = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo da lista dos selecionados
	//loop em todos os �tens da lista Selecionados
	while (gtk_tree_model_get_iter_first (modelSel, &iter))
	{
		gtk_tree_model_get (modelSel, &iter, 0, &sItem, -1);//selecionar
		gtk_list_store_remove (GTK_LIST_STORE (modelSel), &iter);//remover
		gtk_list_store_append (GTK_LIST_STORE (modelDisp), &iter);//reservara espa�o na lista Dispon�veis
		gtk_list_store_set (GTK_LIST_STORE (modelDisp), &iter, 0, sItem, -1);//adicionar �tem
        BE_free (sItem);//limpar
	}
	//foco na segunda lista
	if (Janela->treeviewDisp && GTK_IS_WIDGET (Janela->treeviewDisp))
        gtk_widget_grab_focus (Janela->treeviewDisp);
}
