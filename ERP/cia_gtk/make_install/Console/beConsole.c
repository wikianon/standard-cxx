#include "beConsole.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowConsole.c"

GtkWidget *be_Console (GtkWidget *window, BESQL *SQL)
{
	_BEConsole_Janela *Janela = NULL;
	GtkTextBuffer *buffer = NULL;
	GtkAccelGroup *accel;
	gchar *sErros = NULL;

	//preparar teclas de atalho
	accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (window), accel);
	//objetos da janela
	Janela = g_new (_BEConsole_Janela, 1);
	Janela->window = window;
	Janela->tabela = BE_interfaces_widget (window, "tableConsole");
	Janela->tabela = be_Interface_criar_windowConsole_tableConsole (window, accel, NULL, Janela, NULL);//carregar tabela
	Janela->textview = BE_interfaces_widget (window, "textviewConsole");
	Janela->treeview = BE_interfaces_widget (window, "treeviewConsole");
	Janela->executar = BE_interfaces_widget (window, "toolbuttonExecutar");
	Janela->exportar = BE_interfaces_widget (window, "toolbuttonExportar");
	Janela->rotulo = BE_interfaces_widget (window, "labelConsole");
	Janela->SQL = SQL;
	Janela->bVertical = FALSE;
	Janela->Campos = NULL;
	Janela->nCampos = 0;
	Janela->nLinhas = 0;

	//limpar status
	gtk_label_set_text (GTK_LABEL (Janela->rotulo), NULL);

	//informações iniciais: ajuda
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (Janela->textview));
    gtk_text_buffer_set_text (buffer, "\t- Digite a consulta, selecione a mesma, e clique em executar (F5).\n"
    "\t- Para exibicao na vertical, utilize \"\\G\" no final da consulta, ao inves do \";\".\n"
    "\t- Multiplas consultas sao permitidas, desde que separadas por \";\" ou \"\\G\".\n\n"
    , -1);

	//meus sinais
	g_signal_connect (Janela->tabela, "destroy", G_CALLBACK (_be_Console_on_destroy), Janela);//fechar tudo

	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	//foco inicial
	gtk_widget_grab_focus (Janela->textview);

	//retornar tabela
	return Janela->tabela;
}

void _be_Interface_sinal_windowConsole_on_toolbuttonExportar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEConsole_Janela *Janela = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sTemp = NULL, *sNomeArquivo = NULL;

	Janela = (_BEConsole_Janela *) user_data;

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
		//exportar
		if (_be_Console_exportar_dados (Janela, sNomeArquivo, "Console"))
		{
			//informar
			BE_free (sTemp);
			sTemp = g_strdup_printf ("Arquivo gravado em %s.", sNomeArquivo);
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		}
	}

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);

	//limpar
	BE_free (sTemp);
	BE_free (sNomeArquivo);
}

gboolean _be_Interface_sinal_windowConsole_on_tableConsole_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	_BEConsole_Janela *Janela = NULL;

	Janela = (_BEConsole_Janela *) user_data;

	//teclas de atalho
	if (event->state & GDK_CONTROL_MASK)
	{
		switch (event->keyval)
		{
            case GDK_e://exportar
            case GDK_E:
                _be_Interface_sinal_windowConsole_on_toolbuttonExportar_clicked (GTK_TOOL_BUTTON (Janela->exportar), Janela);
                break;
		}
	}
	else
	{
		switch (event->keyval)
		{
			case GDK_F5://atualizar
				_be_Interface_sinal_windowConsole_on_toolbuttonExecutar_clicked (GTK_TOOL_BUTTON (Janela->executar), Janela);
				break;
			default:
				return FALSE;
				break;
		}
	}
	return TRUE;
}

void _be_Interface_sinal_windowConsole_on_toolbuttonExecutar_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	_BEConsole_Janela *Janela = NULL;
	GtkTextBuffer *buffer = NULL;
	GtkTextIter start, end;
	GList *Cols = NULL, *Renders = NULL;
	GString *Status = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sErros = NULL;
	gchar *sConsulta = NULL, *sTemp = NULL;
	gchar **aConsultas = NULL;
	gint nCont = 0;

	Janela = (_BEConsole_Janela *) user_data;

    //desabilitar exportar
    gtk_widget_set_sensitive (Janela->exportar, FALSE);

    //recuperar consulta
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (Janela->textview));
    //gtk_text_buffer_get_bounds (buffer, &start, &end);//recuperar tudo
    if (gtk_text_buffer_get_selection_bounds (buffer, &start, &end))//recuperar somente selecionado
    {
        sTemp = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
        sConsulta = BE_UTF8_ISO (sTemp);
    }
	if (!sConsulta)
	{
		BE_free (sTemp);
		return;
	}

	//mouse
	Mouse = be_janela_mouse_pensando (Janela->window);

    //limpar tudo
    gtk_tree_view_set_model (GTK_TREE_VIEW (Janela->treeview), NULL);
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
    Janela->Campos = NULL;
    Janela->nCampos = 0;
    Janela->nLinhas = 0;

    //verificar vertical
    if (be_strpos (sConsulta, BECONSOLE_VERTICAL, 0) >= 0)
    {
        BE_free (sTemp);
        sTemp = sConsulta;
        sConsulta = be_strreplace (sTemp, BECONSOLE_VERTICAL, ";");
        Janela->bVertical = TRUE;
    }
    else
        Janela->bVertical = FALSE;

	//separar vários comandos
	aConsultas = g_strsplit (sConsulta, ";", -1);
	//preparar status
	Status = g_string_new ("\n");
	//loop dos comandos
    for (nCont = 0; nCont < g_strv_length (aConsultas) && !sErros; nCont++)
    {
        BE_free (sTemp);
        sTemp = sConsulta;
        sConsulta = g_strdup (aConsultas[nCont]);
        //remover espaços iniciais e finais
        sConsulta = g_strstrip (sConsulta);

        //executar consulta
        if (sConsulta && strlen (sConsulta) > 0)
        {
            be_sql_executar (Janela->SQL, sConsulta, _be_Console_callback_SQL, Janela, &sErros);

            //recuperar status
            BE_free (sTemp);
            sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_ALTERACAO, &sErros);
            if (sTemp)
                g_string_append_printf (Status, "\n>>>>>\t%s\t...\t%s", sTemp, sConsulta);
        }
    }
	//informar status, a última informação
    BE_free (sTemp);
    sTemp = be_sql_informacoes (Janela->SQL, BESQL_INFO_ALTERACAO, &sErros);
    gtk_label_set_markup (GTK_LABEL (Janela->rotulo), sTemp);
	g_string_free (Status, TRUE);
    //limpar array
    BE_strfreev (aConsultas);

	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);
	//informar erros
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
    //limpar
    BE_free (sConsulta);
    BE_free (sTemp);
}

gboolean _be_Console_callback_SQL (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data)
{
	BESQL_Campo *Campo = NULL;
	_BEConsole_Janela *Janela = NULL;
	GtkTreeIter iter;
    GtkTreeModel *model = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL;
	guint nColunaAtiva = 0;
	gboolean bContinuar = TRUE;

	Janela = (_BEConsole_Janela *) user_data;

    //Campos
    if (!Janela->Campos)
    {
        Janela->Campos = Campos;
        Janela->nCampos = g_slist_length (Campos);
    }

    //habilitar exportar
    gtk_widget_set_sensitive (Janela->exportar, TRUE);
    //recuperar modelo
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));
    //criar pela primeira vez as colunas e o modelo
	if (!model)
	{
		//modelo de dados
		model = _be_Console_criar_modelo (Janela);
		if (model)
		{
			gtk_tree_view_set_model (GTK_TREE_VIEW (Janela->treeview), model);
			g_object_unref (model);
            //criar colunas
            _be_Console_adicionar_colunas (Janela);
		}
	}
    //adicionar os dados
    gtk_list_store_append (GTK_LIST_STORE (model), &iter);
    Janela->nLinhas++;//linhas
    nColunaAtiva = 0;
    //verificar limites
    if (Janela->nCampos * nLinha > BECONSOLE_LINHAS_X_COLUNAS_MAX)
    {
        //verificar vertical
        if (Janela->bVertical)
        {
            BE_free (sTemp);
            sTemp = g_strdup_printf ("... Col x Lin > %d", BECONSOLE_LINHAS_X_COLUNAS_MAX);
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sTemp, -1);
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, sTemp, -1);
        }
        else
        {
            for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
            {
                if (nColunaAtiva < Janela->nCampos)//segurança de quantidade de campos diferentes
                {
                    BE_free (sTemp);
                    sTemp = g_strdup_printf ("... Col x Lin > %d", BECONSOLE_LINHAS_X_COLUNAS_MAX);
                    gtk_list_store_set (GTK_LIST_STORE (model), &iter, nColunaAtiva, sTemp, -1);
                }
                nColunaAtiva++;
            }
        }
        bContinuar = FALSE;
    }
    else
    {
        for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
        {
            Campo = (BESQL_Campo *) ListaL->data;
            //verificar vertical
            if (Janela->bVertical)
            {
                //campos
                BE_free (sTemp);
                sTemp = BE_ISO_UTF8 (Campo->sNome);
                gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, sTemp, -1);
                //valores
                if (Campo->sValor)//existe um valor não nulo
                {
                    BE_free (sTemp);
                    sTemp = be_sql_campo_valor_limitado_formatado (Campo, Campo->nComprimento, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
                    gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, sTemp, -1);
                }
                else//valor é nulo
                    gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, BECONSOLE_NULO, -1);
                //adicionar os dados
                gtk_list_store_append (GTK_LIST_STORE (model), &iter);
                Janela->nLinhas++;//linhas
            }
            else
            {
                if (nColunaAtiva < Janela->nCampos)//segurança de quantidade de campos diferentes
                {
                    if (Campo->sValor)//existe um valor não nulo
                    {
                        BE_free (sTemp);
                        sTemp = be_sql_campo_valor_limitado_formatado (Campo, BECONSOLE_LENSTRING, BESQL_CAMPOS_LIMITAR_FORMATO_UTF8);
                        gtk_list_store_set (GTK_LIST_STORE (model), &iter, nColunaAtiva, sTemp, -1);
                    }
                    else//valor é nulo
                        gtk_list_store_set (GTK_LIST_STORE (model), &iter, nColunaAtiva, BECONSOLE_NULO, -1);
                    }
                nColunaAtiva++;
            }
        }
        //linha divisória para exibição vertical
        if (Janela->bVertical)
        {
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, "------", -1);
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 1, "------", -1);
        }
    }
    //limpar
    BE_free (sTemp);

    return bContinuar;//FALSE para parar o loop.
}

void _be_Console_adicionar_colunas (_BEConsole_Janela *Janela)
{
	BESQL_Campo *Campo = NULL;
	GtkCellRenderer *renderer = NULL;
	GtkTreeViewColumn *col = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL;
	guint nColunaAtiva = 0;

    //renderização das células
    renderer = gtk_cell_renderer_text_new ();
    //exibir colunas na vertical
    if (Janela->bVertical)
    {
        //exibir coluna 1
        col = gtk_tree_view_column_new ();
        gtk_tree_view_column_set_title (col, "Campos");//título, aceita acentuações
        gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento
        gtk_tree_view_column_set_resizable (col, TRUE);//largura variavel
        gtk_tree_view_column_set_min_width (col, 30);
        gtk_tree_view_column_set_max_width (col, 600);
        gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
        gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
        gtk_tree_view_column_add_attribute (col, renderer, "text", 0);//anexando os dados na renderização do texto
        gtk_tree_view_append_column (GTK_TREE_VIEW (Janela->treeview), col);//adicionando coluna ao treeview
        //exibir coluna 2
        col = gtk_tree_view_column_new ();
        gtk_tree_view_column_set_title (col, "Valores");//título, aceita acentuações
        gtk_tree_view_column_set_alignment (col, 0.5);//alinhamento
        gtk_tree_view_column_set_resizable (col, TRUE);//largura variavel
        gtk_tree_view_column_set_min_width (col, 30);
        gtk_tree_view_column_set_max_width (col, 3000);
        gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
        gtk_tree_view_column_pack_start (col, renderer, TRUE);//anexando renderização à coluna
        gtk_tree_view_column_add_attribute (col, renderer, "text", 1);//anexando os dados na renderização do texto
        gtk_tree_view_append_column (GTK_TREE_VIEW (Janela->treeview), col);//adicionando coluna ao treeview
    }
    //exibir colunas na horizontal; padrão
    else
    {
        for (ListaL = Janela->Campos; ListaL; ListaL = g_slist_next (ListaL))
        {
            Campo = (BESQL_Campo *) ListaL->data;
            //exibir campo
            //sTemp = g_strdup ((gchar *) ListaL->data);
            sTemp = BE_ISO_UTF8 (Campo->sNome);
            col = gtk_tree_view_column_new ();
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

GtkTreeModel *_be_Console_criar_modelo (_BEConsole_Janela *Janela)
{
	GtkListStore *store = NULL;
	GType *pTypes = NULL;//colunas
	guint nColuna = 0, nColunas = 0;

	//verificar exibição na vertical
	if (Janela->bVertical)
        nColunas = 2;
    else
        nColunas = Janela->nCampos;
	//criando modelo
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

gboolean _be_Console_exportar_dados (_BEConsole_Janela *Janela, const gchar *sArquivo, const gchar *sNome)
{
	FILE *File = NULL;
	GtkTreeIter iter;
    GtkTreeModel *model = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sPath = NULL, *sTexto = NULL;
	gint nColuna = 0, nLinha = 0, nColunas = 0;

	gboolean bOK = FALSE;

	//Abrindo arquivo: Criando ou sobreescrevendo existente
	if ((File = fopen (sArquivo, "wb")))
	{
        //início, cabeçalho
        BE_free (sTemp);
        sTemp = g_strdup_printf ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd>\n");
        fwrite (sTemp, strlen (sTemp), 1, File);

        //início
        BE_free (sTemp);
        sTemp = g_strdup_printf ("<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n<meta name=\"Author\" content=\"%s %s\">\n<title>%s</title>\n</head>\n<body>\n", INTERNAL_NAME, PRODUCT_VERSION, sNome);
        fwrite (sTemp, strlen (sTemp), 1, File);
        //tabela
        BE_free (sTemp);
        sTemp = g_strdup_printf ("<table border='1' align='center' valign='middle'>\n<caption>%s</caption>\n", sNome);
        fwrite (sTemp, strlen (sTemp), 1, File);

        //recuperar modelo
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (Janela->treeview));

        //verificar vertical
        if (Janela->bVertical)
            nColunas = 2;
        else
            nColunas = Janela->nCampos;
        //loop nos dados
        for (nLinha = 0; nLinha < Janela->nLinhas; nLinha++)
        {
            //loop nos campos
            if (nLinha == 0)
            {
                for (nColuna = 0; nColuna < nColunas; nColuna++)
                {
                    //cabeçalho
                    BE_free (sTemp);
                    BE_free (sTemp1);
                    sTemp = g_strdup_printf ("<th>%s</th>\n", gtk_tree_view_column_get_title (gtk_tree_view_get_column (GTK_TREE_VIEW (Janela->treeview), nColuna)));
                    sTemp1 = BE_UTF8_ISO (sTemp);
                    fwrite (sTemp1, strlen (sTemp1), 1, File);
                }
            }
            //próxima linha
            fwrite ("</tr>\n", strlen ("</tr>\n"), 1, File);
            //loop nos dados
            for (nColuna = 0; nColuna < nColunas; nColuna++)
            {
                //dados
                BE_free (sPath);
                sPath = g_strdup_printf ("%d", nLinha);
                if (gtk_tree_model_get_iter_from_string (model, &iter, sPath))
                {
                    gtk_tree_model_get (model, &iter, nColuna, &sTexto, -1);
                    //gravar
                    BE_free (sTemp);
                    BE_free (sTemp1);
                    sTemp = g_strdup_printf ("\t<td>%s</td>\n", sTexto);
                    sTemp1 = BE_UTF8_ISO (sTemp);
                    fwrite (sTemp1, strlen (sTemp1), 1, File);
                }
            }
        }
        //fim
        BE_free (sTemp);
        sTemp = g_strdup_printf ("</table>\n</body>\n</html>");
        fwrite (sTemp, strlen (sTemp), 1, File);
        //informar
        bOK = TRUE;
		//fechar arquivo
		fclose (File);
	}

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sPath);
	BE_free (sTexto);

	return bOK;
}

void _be_Console_on_destroy (GtkObject *object, gpointer user_data)
{
	_BEConsole_Janela *Janela = NULL;

	Janela = (_BEConsole_Janela *) user_data;
	BE_free (Janela);
}
