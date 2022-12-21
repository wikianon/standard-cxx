#include "bePropriedades.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowPropriedades.c"

gboolean be_Propriedades (GtkWindow *winPai, GSList *Propriedades)
{
	GtkWidget *viewport = NULL;
	GtkWidget *table = NULL, *tablemodelo = NULL;
	GSList *ListaL = NULL;
	_BEPropriedades_Tipo *Tipo = NULL;
	_BEPropriedades_Janela Janela;
	gint nCol = 0, nLin = 0;
	gint nPos = 0;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//janela
	Janela.bOK = FALSE;
	Janela.Propriedades = Propriedades;
	Janela.window = be_Interface_criar_windowPropriedades (winPai, NULL, NULL, &Janela);
	//nova tabela
	table = gtk_table_new (g_slist_length (Propriedades), 3, FALSE);
	gtk_table_set_row_spacings (GTK_TABLE (table), 2);
	gtk_table_set_col_spacings (GTK_TABLE (table), 2);
	//propriedades
	for (ListaL = Propriedades; ListaL; ListaL = g_slist_next (ListaL))
	{
		Tipo = (_BEPropriedades_Tipo *) ListaL->data;
		//janelas
		Tipo->Janela = &Janela;
		//labels comum
		nCol = 0;
		Tipo->label = be_Interface_criar_windowPropriedades_labelPropriedades (NULL, NULL, NULL, Tipo, NULL);
		gtk_table_attach (GTK_TABLE (table), Tipo->label, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
		if (Tipo->sNome)
			gtk_label_set_label (GTK_LABEL (Tipo->label), Tipo->sNome);
		else
			gtk_label_set_label (GTK_LABEL (Tipo->label), "");

		nCol++;
		//rótulo: informações
		if (Tipo->nTipo == BEPROPRIEDADES_ROTULO)
		{
            Tipo->entry = be_Interface_criar_windowPropriedades_labelInformacao (NULL, NULL, NULL, Tipo, NULL);
            gtk_table_attach (GTK_TABLE (table), Tipo->entry, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
            //valor
            if (Tipo->Valor && *Tipo->Valor)
                gtk_label_set_label (GTK_LABEL (Tipo->entry), *Tipo->Valor);
            else
                gtk_label_set_label (GTK_LABEL (Tipo->entry), "");
		}
		//entry comum
		else
        {
            Tipo->entry = be_Interface_criar_windowPropriedades_entryPropriedades (NULL, NULL, NULL, Tipo, NULL);
            gtk_table_attach (GTK_TABLE (table), Tipo->entry, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
            gtk_editable_set_editable (GTK_EDITABLE (Tipo->entry), FALSE);
            //valor padrão
            if (Tipo->Valor && *Tipo->Valor)
                gtk_entry_set_text (GTK_ENTRY (Tipo->entry), *Tipo->Valor);
        }
		//ítens específicos
		nCol++;
		switch (Tipo->nTipo)
		{
			case BEPROPRIEDADES_NENHUM:
				gtk_widget_hide (Tipo->entry);
				break;
			case BEPROPRIEDADES_ROTULO:
				break;
			case BEPROPRIEDADES_TEXTO:
				gtk_editable_set_editable (GTK_EDITABLE (Tipo->entry), TRUE);
				break;
			case BEPROPRIEDADES_NUMERO:
				gtk_editable_set_editable (GTK_EDITABLE (Tipo->entry), TRUE);
				break;
			case BEPROPRIEDADES_LOGICO:
				Tipo->button = be_Interface_criar_windowPropriedades_checkbuttonLogico (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				//valor padrão
				if (g_strtod (*Tipo->Valor, NULL) != 0.0)
					gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Tipo->button), TRUE);
				//esconder edição de texto
				gtk_widget_hide (Tipo->entry);
				break;
			case BEPROPRIEDADES_FONTE:
				Tipo->button = be_Interface_criar_windowPropriedades_buttonFonte (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				break;
			case BEPROPRIEDADES_COR:
				Tipo->button = be_Interface_criar_windowPropriedades_buttonCor (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				break;
			case BEPROPRIEDADES_ARQUIVO:
				Tipo->button = be_Interface_criar_windowPropriedades_buttonArquivo (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				break;
			case BEPROPRIEDADES_ARQUIVO_LIVRE:
				Tipo->button = be_Interface_criar_windowPropriedades_buttonArquivo (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				gtk_editable_set_editable (GTK_EDITABLE (Tipo->entry), TRUE);
				break;
			case BEPROPRIEDADES_DIRETORIO:
				Tipo->button = be_Interface_criar_windowPropriedades_buttonDiretorio (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				break;
			case BEPROPRIEDADES_LISTA:
				Tipo->button = be_Interface_criar_windowPropriedades_buttonLista (NULL, NULL, NULL, Tipo, NULL);
				gtk_table_attach (GTK_TABLE (table), Tipo->button, nCol, nCol + 1, nLin, nLin + 1, GTK_FILL, GTK_FILL, 0, 0);
				//valores da lista
				nPos = (gint) g_strtod (*Tipo->Valor, NULL);
				if (nPos >= 0 && nPos < g_slist_length (Tipo->Lista))
				{
					BE_free (sTemp);
					BE_free (sTemp1);

					if (Tipo->bUTF8)
						sTemp = g_strdup_printf ("%d - %s", nPos, (gchar *) g_slist_nth_data (Tipo->Lista, nPos));
					else
					{
						sTemp1 = g_strdup_printf ("%d - %s", nPos, (gchar *) g_slist_nth_data (Tipo->Lista, nPos));
						sTemp = BE_ISO_UTF8 (sTemp1);
					}
					gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sTemp);
				}
				break;
		}
		nLin++;
	}
	//view
	viewport = BE_interfaces_widget (Janela.window, "viewportFundo");
	//desanexar tabela modelo
	tablemodelo = BE_interfaces_widget (Janela.window, "tableFundo");
	gtk_container_remove (GTK_CONTAINER (viewport), tablemodelo);
	//anexar e exibir nova tabela
	gtk_container_add (GTK_CONTAINER (viewport), table);
	gtk_widget_show (table);
	//foco inicial, no primeiro controle
	Tipo = (_BEPropriedades_Tipo *) Propriedades->data;;
	if (GTK_WIDGET_VISIBLE (Tipo->entry))
		gtk_widget_grab_focus (Tipo->entry);
	else if (GTK_WIDGET_VISIBLE (Tipo->button))
		gtk_widget_grab_focus (Tipo->button);
	//destruindo tabela modelo, e seus filhos
	gtk_widget_destroy (tablemodelo);

	//loop
	gtk_main ();
	//liberar
	_be_Propriedades_Liberar (Propriedades);
	BE_free (sTemp);
	BE_free (sTemp1);

	return Janela.bOK;
}

void _be_Interface_sinal_windowPropriedades_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	_BEPropriedades_Janela *Janela = NULL;
	gchar *sTemp = NULL;
	GSList *ListaL = NULL;

	Janela = (_BEPropriedades_Janela *) user_data;
	Janela->bOK = TRUE;
	//repassar valores
	for (ListaL = Janela->Propriedades; ListaL; ListaL = g_slist_next (ListaL))
	{
		Tipo = (_BEPropriedades_Tipo *) ListaL->data;
		if (Tipo->nTipo != BEPROPRIEDADES_ROTULO && Tipo->Valor)
		{
			sTemp = *Tipo->Valor;//pegar valor antigo
			if (strlen (gtk_entry_get_text (GTK_ENTRY (Tipo->entry))) > 0)
				*Tipo->Valor = g_strdup (gtk_entry_get_text (GTK_ENTRY (Tipo->entry)));
			else
				*Tipo->Valor = NULL;
		}
		//liberar valor antigo
		BE_free (sTemp);
	}
	//fechar janela principal
	gtk_widget_destroy (Janela->window);
}

GSList *be_Propriedades_adicionar (GSList *Propriedades, gboolean bUTF8, BEPROPRIEDADES_ENUM nTipo, const gchar *sNome, gchar **Valor, GSList *Lista)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	gchar *sTemp = NULL;

	Tipo = g_new (_BEPropriedades_Tipo, 1);
	Tipo->nTipo = nTipo;
	if (sNome)
	{
		if (bUTF8)
		{
			Tipo->sNome = g_strdup (sNome);
		}
		else
		{
			sTemp = BE_ISO_UTF8 (sNome);
			Tipo->sNome = sTemp;

			if (Valor && *Valor)
			{
				sTemp = *Valor;
				*Valor = BE_ISO_UTF8 (sTemp);
				BE_free (sTemp);
			}
		}
	}
	else
		Tipo->sNome = NULL;
	Tipo->Valor = Valor;
	Tipo->Lista = Lista;
	Tipo->label = NULL;
	Tipo->entry = NULL;
	Tipo->button = NULL;
	Tipo->bUTF8 = bUTF8;
	Propriedades = g_slist_append (Propriedades, Tipo);

	return Propriedades;
}

void _be_Propriedades_Liberar (GSList *Propriedades)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	GSList *ListaL = NULL;

	for (ListaL = Propriedades; ListaL; ListaL = g_slist_next (ListaL))
	{
		Tipo = (_BEPropriedades_Tipo *) ListaL->data;
		BE_free (Tipo->sNome);
		BE_free (Tipo);
	}
}

void _be_Interface_sinal_windowPropriedades_on_buttonFonte_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	gchar *sFonte = NULL;

	Tipo = (_BEPropriedades_Tipo *) user_data;
	//abrir janela
	if ((sFonte = be_Fonte (GTK_WINDOW (Tipo->Janela->window), gtk_entry_get_text (GTK_ENTRY (Tipo->entry)))))
		gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sFonte)
	//liberar
	BE_free (sFonte);
}

void _be_Interface_sinal_windowPropriedades_on_buttonCor_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	gchar *sCor = NULL;

	Tipo = (_BEPropriedades_Tipo *) user_data;
	//abrir janela
	if ((sCor = be_Cor (GTK_WINDOW (Tipo->Janela->window), gtk_entry_get_text (GTK_ENTRY (Tipo->entry)))))
		gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sCor)
	//liberar
	BE_free (sCor);
}

void _be_Interface_sinal_windowPropriedades_on_buttonArquivo_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	gchar *sArquivo = NULL;
	const gchar *sValor = NULL;

	Tipo = (_BEPropriedades_Tipo *) user_data;
	if (strlen (gtk_entry_get_text (GTK_ENTRY (Tipo->entry))) > 0)
		sValor = gtk_entry_get_text (GTK_ENTRY (Tipo->entry));
	//abrir janela
	if (sValor && g_path_is_absolute (sValor))
	{
		if ((sArquivo = be_Abrir (GTK_WINDOW (Tipo->Janela->window), TRUE, NULL, sValor)))
			gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sArquivo);
	}
	else
	{
		if ((sArquivo = be_Abrir (GTK_WINDOW (Tipo->Janela->window), TRUE, NULL, NULL)))
			gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sArquivo);
	}
	//liberar
	BE_free (sArquivo);
}

void _be_Interface_sinal_windowPropriedades_on_checkbuttonLogico_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;

	Tipo = (_BEPropriedades_Tipo *) user_data;
	if (gtk_toggle_button_get_active (togglebutton))
		gtk_entry_set_text (GTK_ENTRY (Tipo->entry), "1");
	else
		gtk_entry_set_text (GTK_ENTRY (Tipo->entry), "0");
}

void _be_Interface_sinal_windowPropriedades_on_buttonDiretorio_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	gchar *sDir = NULL;
	const gchar *sValor = NULL;

	Tipo = (_BEPropriedades_Tipo *) user_data;
	if (strlen (gtk_entry_get_text (GTK_ENTRY (Tipo->entry))) > 0)
		sValor = gtk_entry_get_text (GTK_ENTRY (Tipo->entry));
	//abrir janela
	if ((sDir = be_Diretorio (GTK_WINDOW (Tipo->Janela->window), TRUE, sValor)))
		gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sDir);
	//liberar
	BE_free (sDir);
}

void _be_Interface_sinal_windowPropriedades_on_buttonLista_clicked (GtkWidget *widget, gpointer user_data)
{
	_BEPropriedades_Tipo *Tipo = NULL;
	gint nPos = -1;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	Tipo = (_BEPropriedades_Tipo *) user_data;
	nPos = (gint) g_strtod ((gtk_entry_get_text (GTK_ENTRY (Tipo->entry))), NULL);
	//abrir janela
	nPos = be_Lista (GTK_WINDOW (Tipo->Janela->window), Tipo->bUTF8, Tipo->sNome, Tipo->Lista, nPos);
	if (nPos >= 0)
	{
		if (Tipo->bUTF8)
			sTemp = g_strdup_printf ("%d - %s", nPos, (gchar *) g_slist_nth_data (Tipo->Lista, nPos));
		else
		{
			sTemp1 = g_strdup_printf ("%d - %s", nPos, (gchar *) g_slist_nth_data (Tipo->Lista, nPos));
			sTemp = BE_ISO_UTF8 (sTemp1);
		}
		gtk_entry_set_text (GTK_ENTRY (Tipo->entry), sTemp);
	}

	BE_free (sTemp);
	BE_free (sTemp1);
}
