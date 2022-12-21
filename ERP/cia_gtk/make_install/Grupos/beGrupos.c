#include "beGrupos.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowGrupos.c"

/*******************************************************************************
Janela para agrupamento de ítens.
Os sítens estão em dispostos em duas listas: Disponíveis e Selecionados.
Na lista dos Selecionados também é possível alterar a ordem dos ítens.
*******************************************************************************/

gboolean be_Grupos (GtkWindow *winPai, GSList **Disponivel, GSList **Selecionado)
{
/*******************************************************************************
Abre a janela de grupos.
Nesta janela contém uma lista dos ítens disponíveis e outra dos ítens selecionados.
Todos os ítens são strings no formato UTF8.
Argumentos:
    winPai : janela parente, superior
    Disponivel : endereço da lista dos ítens disponíveis
    Selecionado : endereço da lista dos ítens selecionados
    Retorno: se o usuário confirmou ou não, as mudanças
*******************************************************************************/
	GtkTreeModel *model = NULL;
	_BEGrupos_Janela Janela;

	Janela.window = be_Interface_criar_windowGrupos (winPai, NULL, NULL, &Janela);
	//lista dos disponíveis
	Janela.treeviewDisp = BE_interfaces_widget (Janela.window, "treeviewDisponivel");
		//modelo de dados
		model = _be_Grupos_create_model (*Disponivel);
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeviewDisp), model);
		g_object_unref (model);
		_be_Grupos_add_columns (GTK_TREE_VIEW (Janela.treeviewDisp), "DisponÃ­veis");//colunas; nomes devem estar convertidos para UTF8
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

	//loop de interação com o usuário; só sai daqui quando a janela Grupos for fechada
	gtk_main ();

	return Janela.bOK;
}

GtkTreeModel *_be_Grupos_create_model (GSList *Lista)
{
/*******************************************************************************
Criação do modelo de listas.
Função somente para uso interno.
Argumentos:
    Lista : lista dos ítens
    Retorno: modelo de lista
*******************************************************************************/
	GtkListStore *store = NULL;
	GtkTreeIter iter;
	GSList *ListaL = NULL;

    store = gtk_list_store_new (1, G_TYPE_STRING);//criando modelo de strings
	//loop dos ítens da lista
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		gtk_list_store_append (store, &iter);//reservando espaço pro ítem
		gtk_list_store_set (store, &iter, 0, (gchar *) ListaL->data, -1);//adicionando o íten no modelo
	}

	return GTK_TREE_MODEL (store);
}

void _be_Grupos_add_columns (GtkTreeView *treeview, gchar *sTitulo)
{
/*******************************************************************************
Criação de modelo de colunas.
A coluna é criada e inserida diretamente no GtkTreeView.
Função somente para uso interno.
Argumentos:
    treeview : GtkTreeView da lista
    sTitulo : nome da coluna
    Retorno: não retorna
*******************************************************************************/
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;

	renderer = gtk_cell_renderer_text_new ();//renderização das células
	col = gtk_tree_view_column_new();//criação da coluna
	gtk_tree_view_column_set_title(col, sTitulo);//título, aceita acentuações
	gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento, centralizado
	gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
	gtk_tree_view_column_add_attribute (col, renderer, "text", 0);//anexando os dados na renderização do texto
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);//adicionando coluna ao treeview
}

void _be_Interface_sinal_windowGrupos_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usuário clicar no botão OK, esta função é chamado. Os ítens das duas listas (disponíveis e selecionados) em GtkTreeView são sincronizados com as listas em GSList. A função be_Grupos() retorna verdadeiro.
Função chamada pelo evento do sinal on_buttonOK_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	//selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo de lista
		BE_slist_free (*Janela->Selecionado);//limpar lista
		//loop nos ítens selecionados
		while (gtk_tree_model_get_iter_first (model, &iter))
		{
			gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
			gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover
			*Janela->Selecionado = g_slist_append (*Janela->Selecionado, sItem);//adicionar na lista
		}
	//disponível
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo de lista
		BE_slist_free (*Janela->Disponivel);//limpar lista
		//loop dis ítens disponíveis
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
Move o ítem selecionado na coluna dos Disponíveis para a coluna dos Selecionados.
Função chamada pelo evento do sinal on_buttonAdicionar_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperando ítem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover de disponível
		//adicionar em selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo da lista
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);//reservando espaço pro ítem
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sItem, -1);//adicionando o ítem
		//foco na primeira lista
		if (Janela->treeviewDisp && GTK_IS_WIDGET (Janela->treeviewDisp))
            gtk_widget_grab_focus (Janela->treeviewDisp);
	}
	BE_free (sItem);//limpar
}

void _be_Interface_sinal_windowGrupos_on_buttonRemover_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Move o ítem selecionado na coluna dos Selecionados para a coluna dos Disponíveis.
Função chamada pelo evento do sinal on_buttonRemover_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));//recuperando ítem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &sItem, -1);//selecionar
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);//remover de disponível
		//adicionar em selecionado
		model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo da lista
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);//reservando espaço pro ítem
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sItem, -1);//adicionando o ítem
		//foco na primeira lista
		if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
            gtk_widget_grab_focus (Janela->treeviewSel);
	}
	BE_free (sItem);//limpar
}

void _be_Interface_sinal_windowGrupos_on_buttonAcima_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Eleva o ítem selecionado na coluna dos Selecionados, trocando de posição com o superior.
Função chamada pelo evento do sinal on_buttonAcima_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter, iterNext;
	gchar *sPath = NULL;
	gint nPath = 0;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));//recuperando ítem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		nPath = atoi (sPath = gtk_tree_model_get_string_from_iter (model, &iter));//recuperar hierarquia do ítem selecioando
		BE_free (sPath);//limpar
		if (nPath > 0)
		{
			if (gtk_tree_model_get_iter_from_string (model, &iterNext, sPath = g_strdup_printf ("%d", nPath - 1)))//recuperar ítem superior
			{
				gtk_list_store_swap (GTK_LIST_STORE (model), &iter, &iterNext);//trocar de posição com o ítem superior
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
Rebaixa o ítem selecionado na coluna dos Selecionados, trocando de posição com o inferior.
Função chamada pelo evento do sinal on_buttonAbaixo_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *model = NULL;
	GtkTreeSelection *selection = NULL;
	GtkTreeIter iter, iterNext;
	gchar *sPath = NULL;
	gint nPath = 0;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela->treeviewSel));//recuperando ítem selecionado
	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		nPath = atoi (sPath = gtk_tree_model_get_string_from_iter (model, &iter));//recuperar hierarquia do ítem selecioando
		BE_free (sPath);
		if (gtk_tree_model_get_iter_from_string (model, &iterNext, sPath = g_strdup_printf ("%d", nPath + 1)))//recuperar ítem inferior
		{
			gtk_list_store_swap (GTK_LIST_STORE (model), &iter, &iterNext);//trocar de posição com o ítem inferior
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
Move todos ítens da coluna dos Disponíveis para a coluna dos Selecionados.
Função chamada pelo evento do sinal on_buttonTodos_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *modelDisp = NULL, *modelSel = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	modelDisp = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo da lista dos disponíveis
	modelSel = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo da lista dos selecionados
	//loop em todos os ítens da lista Disponíveis
	while (gtk_tree_model_get_iter_first (modelDisp, &iter))
	{
		gtk_tree_model_get (modelDisp, &iter, 0, &sItem, -1);//selecionar
        gtk_list_store_remove (GTK_LIST_STORE (modelDisp), &iter);//remover
		gtk_list_store_append (GTK_LIST_STORE (modelSel), &iter);//reservara espaço na lista Selecionados
		gtk_list_store_set (GTK_LIST_STORE (modelSel), &iter, 0, sItem, -1);//adicionar ítem
		BE_free (sItem);//limpar
	}
	//foco na segunda lista
	if (Janela->treeviewSel && GTK_IS_WIDGET (Janela->treeviewSel))
        gtk_widget_grab_focus (Janela->treeviewSel);
}

void _be_Interface_sinal_windowGrupos_on_buttonNenhum_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Move todos ítens da coluna dos Selecionados para a coluna dos Disponíveis.
Função chamada pelo evento do sinal on_buttonNenhum_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEGrupos_Janela
    Retorno: não retorna
*******************************************************************************/
	_BEGrupos_Janela *Janela = NULL;
	GtkTreeModel *modelSel = NULL, *modelDisp = NULL;
	GtkTreeIter iter;
	gchar *sItem = NULL;

	Janela = (_BEGrupos_Janela *) user_data;//recuperando a esttutura criada
	modelSel = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewSel));//recuperar modelo da lista dos disponíveis
	modelDisp = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeviewDisp));//recuperar modelo da lista dos selecionados
	//loop em todos os ítens da lista Selecionados
	while (gtk_tree_model_get_iter_first (modelSel, &iter))
	{
		gtk_tree_model_get (modelSel, &iter, 0, &sItem, -1);//selecionar
		gtk_list_store_remove (GTK_LIST_STORE (modelSel), &iter);//remover
		gtk_list_store_append (GTK_LIST_STORE (modelDisp), &iter);//reservara espaço na lista Disponíveis
		gtk_list_store_set (GTK_LIST_STORE (modelDisp), &iter, 0, sItem, -1);//adicionar ítem
        BE_free (sItem);//limpar
	}
	//foco na segunda lista
	if (Janela->treeviewDisp && GTK_IS_WIDGET (Janela->treeviewDisp))
        gtk_widget_grab_focus (Janela->treeviewDisp);
}
