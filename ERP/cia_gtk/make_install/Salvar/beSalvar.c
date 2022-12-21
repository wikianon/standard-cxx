#include "beSalvar.h"

/*******************************************************************************
Janelas salvar arquivos.
*******************************************************************************/

gchar *be_Salvar (GtkWindow *winPai, gboolean bUTF8, const gchar *sFiltro, const gchar *sArquivoAtual)
{
/*******************************************************************************
Exibe uma janela salvar arquivos.
Pode utilizar filtro para selecionar por extens�o. Ex.: *.bmp *jpg(imagens)\n*.bmp(bitmap)\n*.jpg(JPEG)\n*.*(Todos)
Argumentos:
    winPai : janela parente, superior
    bUTF8 : indica se sFiltro, sArquivoAtual j� est�o em UTF8
    sFiltro : filtro para extens�es
    sArquivoAtual : nome sugerido para o arquivo, ou nulo
    Retorno: novo nome do arquivo com caminho completo em UTF8, ou nulo
*******************************************************************************/
	GtkWidget *window = NULL;
	GtkFileFilter *filter = NULL;
	gchar *sArquivo = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	gchar **Lista = NULL, **Lista1 = NULL;
	gchar *sTextoUTF8 = NULL;
	gint nCont = 0, nCont1 = 0;

	//janela
	window = gtk_file_chooser_dialog_new ("Salvar arquivo", winPai, GTK_FILE_CHOOSER_ACTION_SAVE, "gtk-cancel", GTK_RESPONSE_CANCEL, "gtk-save", GTK_RESPONSE_OK, NULL);
	//centralizada
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);
	//preparar filtro
	if (sFiltro)
	{
		//convers�o
		BE_free (sTextoUTF8);
		if (bUTF8)
			sTextoUTF8 = g_strdup (sFiltro);//copiar filtro
		else
			sTextoUTF8 = BE_ISO_UTF8 (sFiltro);//convers�o do filtro para UTF8
		//lista de par�metros: filtros
		Lista = g_strsplit (sTextoUTF8, "\n", -1);//criando uma lista de filtros, separados por '\n'
		//loop nos filtros
		for (nCont = 0; nCont < g_strv_length (Lista); nCont++)
		{
			filter = gtk_file_filter_new ();//criar novo filtro
			BE_free (sTemp);
			BE_free (sTemp1);
			//separando nomes dos filtros
			sTemp = be_substr (Lista[nCont], 0, be_strpos (Lista[nCont], "(", 0));
			sTemp1 = be_strextract (Lista[nCont], "(", ")", 0);//extrair intervalo entre '()'
			//lista de par�metros: extens�es
			Lista1 = g_strsplit (sTemp, " ", -1);
			//loop nas extens�es do filtro atual
			for (nCont1 = 0; nCont1 < g_strv_length (Lista1); nCont1++)
			{
				gtk_file_filter_add_pattern (filter, Lista1[nCont1]);//adicionar extens�es
			}
			BE_strfreev (Lista1);//limpar lista
			gtk_file_filter_set_name (filter, sTemp1);//preparar nome do filtro
			gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (window), filter);//adicionar nome do filtro
		}
		BE_strfreev (Lista);//limpar lista
	}
	//preparar arquivo
	if (sArquivoAtual)
	{
		BE_free (sTemp);
		sTemp = g_path_get_basename (sArquivoAtual);//recuperar somente nome do arquivo, separar do caminho
        //verificar compatibilidade do nome do arquivo com o tipo de sistema de arquivo suportado pelo SO atual, em UTF8
		if (bUTF8 && be_arquivo_localidade_UTF8 ())
		{
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sArquivoAtual);//preparar nome sugerido para arquivo
			gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (window), sTemp);//preparar nome sugerido para arquivo
		}
		else if (!bUTF8 && !be_arquivo_localidade_UTF8 ())
		{
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sArquivoAtual);//preparar nome sugerido para arquivo
			//convers�o
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_ISO_UTF8 (sTemp);
			gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (window), sTextoUTF8);//preparar nome sugerido para arquivo, convertido
		}
		else if (bUTF8 && !be_arquivo_localidade_UTF8 ())
		{
			//convers�o
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_UTF8_ISO (sArquivoAtual);//converter para ISO
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sTextoUTF8);//preparar nome sugerido para arquivo, convertido
			gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (window), sTemp);//preparar nome sugerido para arquivo, convertido
		}
		else if (!bUTF8 && be_arquivo_localidade_UTF8 ())
		{
			//convers�o
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_ISO_UTF8 (sArquivoAtual);//converter para UTF8
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sTextoUTF8);//preparar nome sugerido para arquivo, convertido
			//convers�o
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_ISO_UTF8 (sTemp);//converter para UTF8
			gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (window), sTextoUTF8);//preparar nome sugerido para arquivo, convertido
		}
	}
	//loop de intera��o com o usu�rio; s� sai daqui quando a janela Grupos for fechada
	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_OK)
	{
		sArquivo = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (window));//recuperar o nome do arquivo aceito pelo usu�rio
		//verificar compatibilidade do nome do arquivo com o tipo de sistema de arquivo suportado pelo SO atual, em UTF8
		if (sArquivo && !be_arquivo_localidade_UTF8 ())
		{
			//convers�o
			BE_free (sTemp);
			BE_free (sTemp1);
			//separar o nome do caminho
			sTemp = g_path_get_basename (sArquivo);//recuperar nome
			sArquivo[strlen (sArquivo) - strlen (sTemp)] = '\0';//recuperar caminho
			sTemp1 = BE_ISO_UTF8 (sArquivo);//converter caminho para UTF8
			BE_free (sArquivo);
			sArquivo = g_strdup_printf ("%s%s", sTemp1, sTemp);//nome completo em UTF8
		}
		//incluir extens�o pelo filtro selecionado
		if (sFiltro)
		{
			//convers�o
			BE_free (sTextoUTF8);
			if (bUTF8)
				sTextoUTF8 = g_strdup (sFiltro);
			else
				sTextoUTF8 = BE_ISO_UTF8 (sFiltro);

			filter = gtk_file_chooser_get_filter (GTK_FILE_CHOOSER (window));//recuperar filtro selecionado pelo usu�rio
			if (filter)
			{
				//criar lista de filtros: Ex.: *.bmp *jpg(Imagens)\n*.bmp(bitmap)\n*.jpg(JPEG)\n*.*(Todos)
				Lista = g_strsplit (sTextoUTF8, "\n", -1);
				//loop dos filtros
				for (nCont = 0; nCont < g_strv_length (Lista); nCont++)
				{
					//comparar filtro dispon�vel com filtro selecionado. Ex.: *.bmp *jpg(Imagens)
					if (g_strrstr (Lista[nCont], gtk_file_filter_get_name (filter)))
					{
						BE_free (sTemp2);
						sTemp2 = g_strdup (Lista[nCont]);//copiar filtro dispon�vel
						//recuperar a primeira extens�o. Ex.: *.jpg
						nCont1 = be_strpos (Lista[nCont], " ", 0);//primeira possibilidade, final da primeira extens�o
						nCont = be_strpos (Lista[nCont], "(", 0);//segunda possibilidade, final da primeira extens�o
						//verificar e recuperar a primeira extens�o dispon�vel
						if (nCont1 > 0 && nCont1 < nCont)
							sTemp2[nCont1] = '\0';
						else
							sTemp2[nCont] = '\0';
						break;
					}
				}
				BE_strfreev (Lista);//limpar lista
                //verificar se o usu�rio incluiu uma extens�o e excluir
				BE_free (sTemp);
				sTemp = be_arquivo_extensao (sArquivo);//recuperar extens�o digitada
				if (sTemp)
					sArquivo[strlen (sArquivo) - strlen (sTemp) - 1] = '\0';//separar extens�o do arquivo
                //incluir a extens�o selecionada pelo filtro, ou digitada pelo usu�rio
				BE_free (sTemp1);
				sTemp1 = sArquivo;
				if (sTemp && g_strrstr (sTemp2 + 2, "*"))
					sArquivo = g_strdup_printf ("%s.%s", sTemp1, sTemp);//o fitro (*.*) permite manter a extens�o digitada pelo usu�rio
				else if (g_strrstr (sTemp2 + 2, "*"))
					sArquivo = g_strdup_printf ("%s", sTemp1);//o fitro (*.*) permite manter sem extens�o
				else
					sArquivo = g_strdup_printf ("%s.%s", sTemp1, sTemp2 + 2);//incluir extens�o do filtro selecionado
			}
		}
	}
	else
		sArquivo = NULL;//sem arquivo, cancelado pelo usu�rio
	//convers�o de barras, para Windows '\', e Linux '/'
	if (sArquivo)
	{
		BE_free (sTemp);
		sTemp = sArquivo;
		sArquivo = be_strreplace (sTemp, "/", G_DIR_SEPARATOR_S);
	}

	//liberando mem�ria
	gtk_widget_destroy (window);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sTextoUTF8);

	return sArquivo;
}
