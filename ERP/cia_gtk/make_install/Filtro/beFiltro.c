#include "beFiltro.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowFiltro.c"

gboolean be_Filtro (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe, BESQL_Sintaxe *SintaxePerfil)
{
	GtkTreeSelection *selection = NULL;
	GtkTreeModel *model = NULL;
	//GdkColor color;
	_BEFiltro_Janela Janela;

	//objetos da janela
	Janela.bOK = FALSE;
	Janela.window = be_Interface_criar_windowFiltro (winPai, NULL, NULL, &Janela);
	Janela.treeview = BE_interfaces_widget (Janela.window, "treeviewFiltro");
	Janela.Sintaxe = Sintaxe;
	Janela.SintaxePerfil = SintaxePerfil;
	Janela.Filtros = NULL;//backup

	//esconder perfil
	if (!Janela.SintaxePerfil)
		gtk_widget_hide (BE_interfaces_widget (Janela.window, "buttonPerfil"));

	//modelo de dados
	model = _be_Filtro_criar_modelo (&Janela);
	if (model)
	{
		gtk_tree_view_set_model (GTK_TREE_VIEW (Janela.treeview), model);
		g_object_unref (model);
		//colunas
		_be_Filtro_adicionar_colunas (&Janela);
	}
	//alterar cor de fundo
	//gdk_color_parse (BEFILTRO_COR_BORDA, &color);
	//	gtk_widget_modify_base (Janela.treeview, GTK_STATE_NORMAL, &color);
	//cores alternadas
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (Janela.treeview), TRUE);

	//propriedades da lista
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (Janela.treeview));
	//gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_NONE);

	//preparar backup
	if (Janela.Sintaxe)
		Janela.Filtros = be_sql_filtro_duplicar_lista (Janela.Sintaxe->Filtros, NULL);
	//atualizar dados
	_be_Filtro_atualizar (&Janela);
	//foco
	gtk_widget_grab_focus (Janela.treeview);

	//loop
	gtk_main ();

	//limpar backup
	if (Janela.bOK)
	{
		BE_sql_filtro_limpar_lista (Janela.Filtros);
	}
	//restaurar backup
	else
	{
		BE_sql_filtro_limpar_lista (Janela.Sintaxe->Filtros);
		Janela.Sintaxe->Filtros = Janela.Filtros;
	}

	return Janela.bOK;
}

GtkTreeModel *_be_Filtro_criar_modelo (_BEFiltro_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	GtkListStore *store = NULL;
	GType *pTypes = NULL;//colunas
	GSList *ListaL = NULL;
	gint nColuna = 0, nColunas = 0;

	//criando modelo
	nColunas = g_slist_length (Janela->Sintaxe->Campos) * BEFILTRO_PROPRIEDADES_POR_COLUNA;
	if (nColunas > 0)
	{
		pTypes = g_new (GType, nColunas);
		//atribuindo tipo
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			pTypes[nColuna] = G_TYPE_STRING;//texto formatado
			pTypes[nColuna + 1] = G_TYPE_STRING;//texto puro

			nColuna += BEFILTRO_PROPRIEDADES_POR_COLUNA;
		}
		store = gtk_list_store_newv (nColunas, pTypes);
		BE_free (pTypes);
	}

	return GTK_TREE_MODEL (store);
}

void _be_Filtro_adicionar_colunas (_BEFiltro_Janela *Janela)
{
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	guint nColuna = 0;
	gchar *sTemp = NULL;

	for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		//colunas
		col = gtk_tree_view_column_new ();
		BE_free (sTemp);
		sTemp = BE_ISO_UTF8 (Campo->sNome);
		gtk_tree_view_column_set_title (col, sTemp);//título, aceita acentuações
		gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento
		gtk_tree_view_column_set_min_width (col, 30);
		gtk_tree_view_column_set_max_width (col, 600);
		gtk_tree_view_column_set_resizable (col, TRUE);//largura variavel
		gtk_tree_view_column_set_clickable (col, TRUE);
		gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_AUTOSIZE);

		//renderizar texto
		renderer = gtk_cell_renderer_text_new ();
		//g_object_set (renderer, "cell-background", BEFILTRO_COR_FUNDO, "editable", TRUE, NULL);//propriedades
		g_object_set (renderer, "editable", TRUE, NULL);//propriedades
		//renderizar geral
		gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
		gtk_tree_view_column_add_attribute (col, renderer, "markup", nColuna);
		gtk_tree_view_column_add_attribute (col, renderer, "text", nColuna + 1);
		//sinais da célula
		g_signal_connect (renderer, "editing-canceled", G_CALLBACK (_be_Filtro_cell_editing_canceled), Janela);
		g_signal_connect (renderer, "edited", G_CALLBACK (_be_Filtro_cell_edited), Janela);

		gtk_tree_view_append_column (GTK_TREE_VIEW (Janela->treeview), col);//adicionando coluna ao treeview
		//célula
		g_object_set_data (G_OBJECT (renderer), "coluna", GINT_TO_POINTER (nColuna));//criando propriedades
		g_object_set_data (G_OBJECT (renderer), "campo", Campo);//criando propriedades
		//coluna
		nColuna += BEFILTRO_PROPRIEDADES_POR_COLUNA;
	}

	BE_free (sTemp);
}

void _be_Filtro_cell_editing_canceled (GtkCellRenderer *renderer, gpointer user_data)
{
	_BEFiltro_Janela *Janela = NULL;

	//recuperar informações original
	Janela = (_BEFiltro_Janela *) user_data;
	//manter o foco na tabela
	if (Janela->treeview && GTK_IS_WIDGET (Janela->treeview))
        gtk_widget_grab_focus (Janela->treeview);
}

void _be_Filtro_cell_edited (GtkCellRendererText *cellrenderertext, gchar *arg1, gchar *arg2, gpointer user_data)
{
	_BEFiltro_Janela *Janela = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Filtro *Filtro = NULL;
	GSList *ListaL = NULL;
	GtkTreePath *path = NULL;
	GtkTreeViewColumn *col = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	gchar *sValor = NULL;
	gchar const *sComparacao = BESQL_COMPARACAO_PARECIDO;
	gint nRelacao = BESQL_RELACAO_E, nLinha = 0, nColuna = 0, nMover = 0, nComprimento = 0;
	gboolean bRelacaoAteAnterior = FALSE;

	//recuperar informações
	Janela = (_BEFiltro_Janela *) user_data;
	//alterar na tela
	if (arg2)
	{
		//recuperando propriedades
		Campo = g_object_get_data (G_OBJECT (cellrenderertext), "campo");
		nColuna = (GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cellrenderertext), "coluna"))) / BEFILTRO_PROPRIEDADES_POR_COLUNA;
		nLinha = atoi (arg1);
		//limpar filtro existente, nesta coluna e linha
		do
		{
			for (ListaL = Janela->Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
			{
				Filtro = (BESQL_Filtro *) ListaL->data;
				if (Filtro->bAtivo && Filtro->nGrupo == nLinha && Filtro->Campo == Campo)
				{
					BE_sql_filtro_excluir (Janela->Sintaxe, Filtro, FALSE);
					break;
				}
			}
		}
		while (!Filtro && Janela->Sintaxe->Filtros);
		//adicionar
		if (arg2 && strlen (arg2) > 0)
		{
			//preparar valor inicial
			BE_free (sTemp);
			sTemp = BE_UTF8_ISO (arg2);// ISO
			do
			{
				//preparar
				nRelacao = BESQL_RELACAO_E;
				//remover espaços, iniciais e finais
				BE_free (sTemp1);
				sValor = g_strdup (g_strstrip (sTemp));// sem espaços iniciais e finais
				sTemp1 = g_ascii_strdown (sValor, -1);//tudo em minúsculo
				BE_free (sTemp);
				sTemp = g_strdup_printf ("%s ", sTemp1);//acrescentar espaço no final
				//recuperar sinal de comparação
				nMover = 0;
				if (g_strstr_len (sTemp, 12, "is not null "))
				{
					nMover = 12;
					sComparacao = BESQL_COMPARACAO_NAO_NULO;
				}
				else if (g_strstr_len (sTemp, 11, "não é nulo "))
				{
					nMover = 11;
					sComparacao = BESQL_COMPARACAO_NAO_NULO;
				}
				else if (g_strstr_len (sTemp, 9, "não nulo "))
				{
					nMover = 9;
					sComparacao = BESQL_COMPARACAO_NAO_NULO;
				}
				else if (g_strstr_len (sTemp, 10, "não vazio "))
				{
					nMover = 10;
					sComparacao = BESQL_COMPARACAO_NAO_NULO;
				}
				else if (g_strstr_len (sTemp, 8, "is null "))
				{
					nMover = 8;
					sComparacao = BESQL_COMPARACAO_NULO;
				}
				else if (g_strstr_len (sTemp, 7, "é nulo "))
				{
					nMover = 7;
					sComparacao = BESQL_COMPARACAO_NULO;
				}
				else if (g_strstr_len (sTemp, 5, "nulo "))
				{
					nMover = 5;
					sComparacao = BESQL_COMPARACAO_NULO;
				}
				else if (g_strstr_len (sTemp, 6, "vazio "))
				{
					nMover = 6;
					sComparacao = BESQL_COMPARACAO_NULO;
				}
				else if (g_strstr_len (sTemp, 9, "is false "))
				{
					nMover = 9;
					sComparacao = BESQL_COMPARACAO_FALSO;
				}
				else if (g_strstr_len (sTemp, 8, "é falso "))
				{
					nMover = 8;
					sComparacao = BESQL_COMPARACAO_FALSO;
				}
				else if (g_strstr_len (sTemp, 6, "falso "))
				{
					nMover = 6;
					sComparacao = BESQL_COMPARACAO_FALSO;
				}
				else if (g_strstr_len (sTemp, 4, "não "))
				{
					nMover = 4;
					sComparacao = BESQL_COMPARACAO_FALSO;
				}
				else if (g_strstr_len (sTemp, 8, "is true "))
				{
					nMover = 8;
					sComparacao = BESQL_COMPARACAO_VERDADEIRO;
				}
				else if (g_strstr_len (sTemp, 13, "é verdadeiro "))
				{
					nMover = 13;
					sComparacao = BESQL_COMPARACAO_VERDADEIRO;
				}
				else if (g_strstr_len (sTemp, 11, "verdadeiro "))
				{
					nMover = 11;
					sComparacao = BESQL_COMPARACAO_VERDADEIRO;
				}
				else if (g_strstr_len (sTemp, 10, "é verdade "))
				{
					nMover = 10;
					sComparacao = BESQL_COMPARACAO_VERDADEIRO;
				}
				else if (g_strstr_len (sTemp, 8, "verdade "))
				{
					nMover = 8;
					sComparacao = BESQL_COMPARACAO_VERDADEIRO;
				}
				else if (g_strstr_len (sTemp, 4, "sim "))
				{
					nMover = 4;
					sComparacao = BESQL_COMPARACAO_VERDADEIRO;
				}
				else if (g_strstr_len (sTemp, 5, "like "))
				{
					nMover = 5;
					sComparacao = BESQL_COMPARACAO_PARECIDO;
				}
				else if (g_strstr_len (sTemp, 9, "parecido "))
				{
					nMover = 9;
					sComparacao = BESQL_COMPARACAO_PARECIDO;
				}
				else if (g_strstr_len (sTemp, 8, "similar "))
				{
					nMover = 8;
					sComparacao = BESQL_COMPARACAO_PARECIDO;
				}
				else if (g_strstr_len (sTemp, 8, "próximo "))
				{
					nMover = 8;
					sComparacao = BESQL_COMPARACAO_PARECIDO;
				}
				else if (g_strstr_len (sTemp, 3, "<=>"))
				{
					nMover = 3;
					sComparacao = BESQL_COMPARACAO_IGUAL_SEMNULO;
				}
				else if (g_strstr_len (sTemp, 2, ">="))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_MAIOR_IGUAL;
				}
				else if (g_strstr_len (sTemp, 2, "=>"))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_MAIOR_IGUAL;
				}
				else if (g_strstr_len (sTemp, 2, "<="))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_MENOR_IGUAL;
				}
				else if (g_strstr_len (sTemp, 2, "=<"))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_MENOR_IGUAL;
				}
				else if (g_strstr_len (sTemp, 10, "diferente "))
				{
					nMover = 10;
					sComparacao = BESQL_COMPARACAO_DIFERENTE;
				}
				else if (g_strstr_len (sTemp, 2, "<>"))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_DIFERENTE;
				}
				else if (g_strstr_len (sTemp, 2, "><"))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_DIFERENTE;
				}
				else if (g_strstr_len (sTemp, 2, "!="))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_DIFERENTE;
				}
				else if (g_strstr_len (sTemp, 2, "=!"))
				{
					nMover = 2;
					sComparacao = BESQL_COMPARACAO_DIFERENTE;
				}
				else if (g_strstr_len (sTemp, 1, "="))
				{
					nMover = 1;
					sComparacao = BESQL_COMPARACAO_IGUAL;
				}
				else if (g_strstr_len (sTemp, 1, ">"))
				{
					nMover = 1;
					sComparacao = BESQL_COMPARACAO_MAIOR;
				}
				else if (g_strstr_len (sTemp, 1, "<"))
				{
					nMover = 1;
					sComparacao = BESQL_COMPARACAO_MENOR;
				}
				//remover sinais de comparação
				if (strlen (sValor) >= nMover && nMover > 0)
				{
					BE_free (sTemp1);
					sTemp1 = sValor;
					sValor = g_strdup (sTemp1 + nMover);
					sValor = g_strchug (sValor);//remover espaços iniciais
					BE_free (sTemp1);
					sTemp1 = sTemp;
					sTemp = g_strdup (sTemp + nMover);
					sTemp = g_strchug (sTemp);//remover espaços iniciais
				}
				//corrigir sinais (relação Até)
				if (bRelacaoAteAnterior)
				{
					sComparacao = BESQL_COMPARACAO_MENOR_IGUAL;
					bRelacaoAteAnterior = FALSE;
				}
				//corrigir sinais (uso de curingas)
				else if (g_strrstr (sTemp, BEFILTRO_CURINGA_TODOS) || g_strrstr (sTemp, BEFILTRO_CURINGA_PELOMENOSUM) || g_strrstr (sTemp, BESQL_CURINGA_TODOS) || g_strrstr (sTemp, BESQL_CURINGA_PELOMENOSUM) || g_strrstr (sTemp, BESQL_CURINGA_SOMENTEUM))
					sComparacao = BESQL_COMPARACAO_PARECIDO;

				//exclui valores entre aspas
				BE_free (sTemp1);
				sTemp1 = be_strextract (sTemp, "\"", "\"", 0);
				if (sTemp1)
				{
					BE_free (sTemp2);
					sTemp2 = sTemp;
					sTemp = be_strreplace (sTemp2, sTemp1, "");
				}
				//recuperar relação
				nComprimento = 0;
				nMover = 0;
				if ((nMover = be_strpos (sTemp, " or ", 0)) > 0 || (nMover = be_strpos (sTemp, " ou ", 0)) > 0)
				{
					nComprimento = 4;
					nRelacao = BESQL_RELACAO_OU;
				}
				else if ((nMover = be_strpos (sTemp, " | ", 0)) > 0)
				{
					nComprimento = 3;
					nRelacao = BESQL_RELACAO_OU;
				}
				else if ((nMover = be_strpos (sTemp, " and ", 0)) > 0)
				{
					nComprimento = 5;
					nRelacao = BESQL_RELACAO_E;
				}
				else if ((nMover = be_strpos (sTemp, " e ", 0)) > 0 || (nMover = be_strpos (sTemp, " & ", 0)) > 0)
				{
					nComprimento = 3;
					nRelacao = BESQL_RELACAO_E;
				}
				else if ((nMover = be_strpos (sTemp, " à ", 0)) > 0)
				{
					nComprimento = 3;
					nRelacao = BESQL_RELACAO_E;
					bRelacaoAteAnterior = TRUE;
					//corrigir sinal
					sComparacao = BESQL_COMPARACAO_MAIOR_IGUAL;
				}
				else if ((nMover = be_strpos (sTemp, " ao ", 0)) > 0)
				{
					nComprimento = 4;
					nRelacao = BESQL_RELACAO_E;
					bRelacaoAteAnterior = TRUE;
					//corrigir sinal
					sComparacao = BESQL_COMPARACAO_MAIOR_IGUAL;
				}
				else if ((nMover = be_strpos (sTemp, " até ", 0)) > 0)
				{
					nComprimento = 5;
					nRelacao = BESQL_RELACAO_E;
					bRelacaoAteAnterior = TRUE;
					//corrigir sinal
					sComparacao = BESQL_COMPARACAO_MAIOR_IGUAL;
				}
				else if ((nMover = be_strpos (sTemp, ", ", 0)) > 0)
				{
					nComprimento = 2;
					nRelacao = BESQL_RELACAO_OU;
				}
				//corrigir pisição
				if (sTemp1)
					nMover += strlen (sTemp1);
				//remover relação e preparar restante
				BE_free (sTemp);
				if (strlen (sValor) >= (nMover + nComprimento) && nComprimento > 0)
				{
					sTemp = g_strdup (sValor + nMover + nComprimento);
					sValor[nMover] = '\0';
				}
				//remover espaços iniciais e finais
				sValor = g_strdup (g_strstrip (sValor));
				//remover aspas iniciais e finais
				if (strlen (sValor) > 1 && sValor[0] == '\"')
					sValor[0] = ' ';
				if (strlen (sValor) > 1 && sValor[strlen (sValor) - 1] == '\"')
					sValor[strlen (sValor) - 1] = ' ';
				//remover espaços iniciais e finais
				sValor = g_strdup (g_strstrip (sValor));
				//converter curingas
				BE_free (sTemp1);
				sTemp1 = sValor;
				sValor = be_strreplace (sTemp1, BEFILTRO_CURINGA_TODOS, BESQL_CURINGA_TODOS);// *
				BE_free (sTemp1);
				sTemp1 = sValor;
				sValor = be_strreplace (sTemp1, BEFILTRO_CURINGA_PELOMENOSUM, BESQL_CURINGA_PELOMENOSUM);// ?
				//corrigir valoes
				if (g_str_equal (sComparacao, BESQL_COMPARACAO_NULO) || g_str_equal (sComparacao, BESQL_COMPARACAO_NAO_NULO) || g_str_equal (sComparacao, BESQL_COMPARACAO_VERDADEIRO) || g_str_equal (sComparacao, BESQL_COMPARACAO_FALSO))
				{
					BE_free (sValor);
				}
				//aplicar novo filtro
				//printf ("%s|%s|%s| Resto: |%s|\n", sComparacao, sValor, nRelacao == BESQL_RELACAO_E ? "E" : "OU", sTemp);
				Filtro = be_sql_filtro_adicionar (Janela->Sintaxe, Campo, sComparacao, sValor, nRelacao, nLinha, FALSE);
			}
			while (sTemp && strlen (sTemp) > 0);
		}
		//atualizar alterações
		_be_Filtro_atualizar (Janela);
		//manter seleção
		path = gtk_tree_path_new_from_string (arg1);
		col = gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColuna);
		if (path && col)
			gtk_tree_view_set_cursor_on_cell (GTK_TREE_VIEW (Janela->treeview), path, col, GTK_CELL_RENDERER (cellrenderertext), FALSE);
		gtk_tree_path_free (path);
	}
	//manter o foco na tabela
	if (Janela->treeview && GTK_IS_WIDGET (Janela->treeview))
        gtk_widget_grab_focus (Janela->treeview);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sValor);
}

void _be_Filtro_atualizar (_BEFiltro_Janela *Janela)
{
	GtkTreeIter iter;
	GtkTreeModel *model = NULL;
	GtkTreeViewColumn *col = NULL;
	BEGlobal_Janela *Mouse = NULL;
	GString *Expressao = NULL, *Formatado = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Filtro *Filtro = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	guint nColuna = 0;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nLinha = 0, nRelacaoAnt = -1;

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);
	//recuperar modelo
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
	//excluindo as antigas
	if (model)
		gtk_list_store_clear (GTK_LIST_STORE (model));
	//loop nas linhas
	for (nLinha = 0; model && nLinha < BEFILTRO_LINHAS; nLinha++)
	{
		//recuperar item
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);
		//loop nos campos
		nColuna = 0;
		for (ListaL = Janela->Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			//recuperar coluna atual
			col = gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColuna / BEFILTRO_PROPRIEDADES_POR_COLUNA);
			//loop nos filtros
			Expressao = g_string_new ("");
			Formatado = g_string_new ("");
			nRelacaoAnt = -1;
			for (ListaL1 = Janela->Sintaxe->Filtros; ListaL1; ListaL1 = g_slist_next (ListaL1))
			{
				Filtro = (BESQL_Filtro *) ListaL1->data;
				if (Filtro->bAtivo && Filtro->nGrupo == nLinha && Filtro->Campo == Campo)
				{
					BE_free (sTemp);
					BE_free (sTemp1);
					//relação anterior
					if (nRelacaoAnt == BESQL_RELACAO_OU)
					{
						g_string_append_printf (Expressao, " ou ");
						g_string_append_printf (Formatado, "<span foreground='#FF0000'><b><i> ou </i></b></span>");
					}
					else if (nRelacaoAnt == BESQL_RELACAO_E)
					{
						g_string_append_printf (Expressao, " e ");
						g_string_append_printf (Formatado, "<span foreground='#FF0000'><b><i> e </i></b></span>");
					}
					nRelacaoAnt = Filtro->nRelacao;
					//comparação
					if (Filtro->sComparacao)
					{
						if (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_PARECIDO))
						{
							//não fazer nada
						}
						else if (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_NULO))
						{
							g_string_append_printf (Expressao, "vazio ");
							g_string_append_printf (Formatado, "<b>vazio </b>");
						}
						else if (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_NAO_NULO))
						{
							g_string_append_printf (Expressao, "nÃ£o vazio ");
							g_string_append_printf (Formatado, "<b>nÃ£o vazio </b>");
						}
						else if (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_VERDADEIRO))
						{
							g_string_append_printf (Expressao, "verdadeiro ");
							g_string_append_printf (Formatado, "<b>verdadeiro </b>");
						}
						else if (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_FALSO))
						{
							g_string_append_printf (Expressao, "falso ");
							g_string_append_printf (Formatado, "<b>falso </b>");
						}
						else
						{
							g_string_append_printf (Expressao, "%s ", Filtro->sComparacao);
							BE_free (sTemp);
							sTemp = g_markup_printf_escaped ("%s", Filtro->sComparacao);
							g_string_append_printf (Formatado, "<b>%s </b>", sTemp);
						}
					}
					//valor
					if (Filtro->sValor)
					{
						//converter curingas
						BE_free (sTemp);
						sTemp = BE_ISO_UTF8 (Filtro->sValor);
						BE_free (sTemp1);
						sTemp1 = sTemp;
						sTemp = be_strreplace (sTemp1, BESQL_CURINGA_TODOS, BEFILTRO_CURINGA_TODOS);// %
						BE_free (sTemp1);
						sTemp1 = sTemp;
						sTemp = be_strreplace (sTemp1, BESQL_CURINGA_PELOMENOSUM, BEFILTRO_CURINGA_PELOMENOSUM);// !
						//proteger sinais
						if (g_strrstr (sTemp, " "))
						{
							BE_free (sTemp1);
							sTemp1 = sTemp;
							sTemp = g_strdup_printf ("\"%s\"", sTemp1);
						}
						BE_free (sTemp1);
						sTemp1 = g_markup_printf_escaped ("%s", sTemp);
						g_string_append_printf (Expressao, "%s", sTemp);
						g_string_append_printf (Formatado, "<span foreground='#0000FF'>%s</span>", sTemp1);
					}
				}
			}
			//adicionar na lista
			gtk_list_store_set (GTK_LIST_STORE (model), &iter,
				nColuna, g_string_free (Formatado, FALSE),
				nColuna + 1, g_string_free (Expressao, FALSE),
				-1);//formatado e puro
			Formatado = NULL;
			Expressao = NULL;
			nColuna += BEFILTRO_PROPRIEDADES_POR_COLUNA;
		}
	}
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Interface_sinal_windowFiltro_on_buttonPerfil_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEFiltro_Janela *Janela = NULL;
	GSList *Lista = NULL, *ListaL = NULL, *ListaL1 = NULL;
	gchar *sErros = NULL;
	gint nPos = -1;
	gchar *sTemp = NULL, *sVariavel = NULL;

	Janela = (_BEFiltro_Janela *) user_data;
	//verificar botão
	if (!Janela->SintaxePerfil)
		return;

	//criando a lista
	Lista = g_slist_append (Lista, g_strdup ("Salvar perfil"));//0
	Lista = g_slist_append (Lista, g_strdup ("Excluir perfil"));//1

	//perfil existente
	sTemp = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFILTRO_PERFIL);
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
			_be_Filtro_salvar_perfil (Janela);
			break;
		case 1://excluir
			_be_Filtro_excluir_perfil (Janela);
			break;
		default://carregar
			_be_Filtro_carregar_perfil (Janela, sVariavel);
			break;
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}

	BE_free (sTemp);
	BE_free (sVariavel);
}

void _be_Filtro_salvar_perfil (_BEFiltro_Janela *Janela)
{
	BESQL_Filtro *Filtro = NULL;
	GSList *ListaL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sErros = NULL;
	gchar *sNome = NULL, *sVariavel = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCont = 0;

	//verificar botão
	if (!Janela->SintaxePerfil)
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
		sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFILTRO_PERFIL);
		//preparar perfil
		if (sVariavel)
		{
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
	else
        //atualizar alterações
        _be_Filtro_atualizar (Janela);

    //limpar
	BE_free (sNome);
	BE_free (sVariavel);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_key_file_free (Ini);
}

void _be_Filtro_excluir_perfil (_BEFiltro_Janela *Janela)
{
	GSList *Lista = NULL, *ListaL = NULL;
	gchar *sErros = NULL;
	gint nPos = -1;
	gchar *sNome = NULL, *sVariavel = NULL;
	gchar *sTemp = NULL;

	//verificar botão
	if (!Janela->SintaxePerfil)
		return;

	//perfil existente
	sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFILTRO_PERFIL);
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
	else
        //atualizar alterações
        _be_Filtro_atualizar (Janela);

	BE_free (sNome);
	BE_free (sVariavel);
	BE_free (sTemp);
}

void _be_Filtro_carregar_perfil (_BEFiltro_Janela *Janela, const gchar *sVariavel)
{
	BESQL_Campo *Campo = NULL;
	BESQL_Filtro *Filtro = NULL;
	GKeyFile *Ini = NULL;
	gchar *sErros = NULL;
	gchar **aValor = NULL;
	gchar *sNome = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCont = 0;

	//verificar botão
	if (!Janela->SintaxePerfil)
		return;

	if (sVariavel && strlen (sVariavel) > 0)
	{
		//localizar existente
		sNome = g_strdup_printf ("%s-%s", Janela->Sintaxe->Tabela->sNome, BEFILTRO_PERFIL);
        if ((Ini = be_sql_perfil_ler_ini (Janela->SintaxePerfil, sNome, sVariavel, &sErros)))
        {
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
        }
	}
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	else
		//atualizar alterações
		_be_Filtro_atualizar (Janela);

	//limpar
	BE_free (sNome);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_key_file_free (Ini);
}

void _be_Interface_sinal_windowFiltro_on_buttonLimpar_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEFiltro_Janela *Janela = NULL;

	Janela = (_BEFiltro_Janela *) user_data;

	//limpar
	be_sql_filtro_limpar (Janela->Sintaxe, FALSE);
	//atualizar alterações
	_be_Filtro_atualizar (Janela);
}

void _be_Interface_sinal_windowFiltro_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEFiltro_Janela *Janela = NULL;

	Janela = (_BEFiltro_Janela *) user_data;
	Janela->bOK = TRUE;

	//fechar janela
	gtk_widget_destroy (Janela->window);
}
