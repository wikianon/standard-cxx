#include "beAbrir.h"

/*******************************************************************************
// Aqui contém uma simples função para selecionar um arquivo através de
// interface gráfica.
*******************************************************************************/
gchar *be_Abrir (GtkWindow *winPai, gboolean bUTF8, const gchar *sFiltro, const gchar *sArquivoAtual)
{
/*******************************************************************************
// be_Abrir
// Abre uma janela para o usuário selecionar um arquivo.
// Argumentos:
//     winPai : janela pai, se houver
//     bUTF8 : indica se os sFiltro e sArquivoAtual já estão em UTF8
//     sFiltro : define uma lista de filtros para exibição dos arquivos, separados por \n
// 		Ex.: *.bmp *jpg *jpeg(Imagéns)\n*.bmp(bitmap)\n*.jpg *.jpeg(JPEG)\n*.*(Todos)
//     sArquivoAtual : caminho completo e nome de arquivo atual
//     Retorno: nome do arquivo selecionado, em UTF8
*******************************************************************************/
	GtkWidget *window = NULL;
	GtkFileFilter *filter = NULL;
	gchar *sArquivo = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sTextoUTF8 = NULL;
	gchar **Lista = NULL, **Lista1 = NULL;
	gint nCont = 0, nCont1 = 0;

	//janela
	window = gtk_file_chooser_dialog_new ("Abrir arquivo", winPai, GTK_FILE_CHOOSER_ACTION_OPEN, "gtk-cancel", GTK_RESPONSE_CANCEL, "gtk-open", GTK_RESPONSE_OK, NULL);
	//centralizada
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);
	//filtro
	if (sFiltro)
	{
		//conversão de textos UTF8
		BE_free (sTextoUTF8);
		if (bUTF8)
			sTextoUTF8 = g_strdup (sFiltro);
		else
			sTextoUTF8 = BE_ISO_UTF8 (sFiltro);
		//filtros
		Lista = g_strsplit (sTextoUTF8, "\n", -1);//são separados por \n
		for (nCont = 0; nCont < g_strv_length (Lista); nCont++)//loop de cada filtro
		{
			filter = gtk_file_filter_new ();//criar novo filtro
			BE_free (sTemp);
			BE_free (sTemp1);
			//separar nome de cada filtro
			sTemp = be_substr (Lista[nCont], 0, be_strpos (Lista[nCont], "(", 0));
			sTemp1 = be_strextract (Lista[nCont], "(", ")", 0);
			//lista de filtros
			Lista1 = g_strsplit (sTemp, " ", -1);
			for (nCont1 = 0; nCont1 < g_strv_length (Lista1); nCont1++)
			{
				gtk_file_filter_add_pattern (filter, Lista1[nCont1]);//extensões
			}
			//limpar
			BE_strfreev (Lista1);
			gtk_file_filter_set_name (filter, sTemp1);//nome
			gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (window), filter);//adicionar
		}
		BE_strfreev (Lista);//limpar
	}
	//arquivo atual
	if (sArquivoAtual)
	{
		//verificar UTF8 de sArquivoAtual e do S.O. atual, eles tem que ser iguais, senão é necessário conversão
		if ((bUTF8 && be_arquivo_localidade_UTF8 ()) || (!bUTF8 && !be_arquivo_localidade_UTF8 ()))
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sArquivoAtual);
		else if (bUTF8 && !be_arquivo_localidade_UTF8 ())
		{
			//conversão
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_UTF8_ISO (sArquivoAtual);
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sTextoUTF8);
		}
		else if (!bUTF8 && be_arquivo_localidade_UTF8 ())
		{
            //conversão
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_ISO_UTF8 (sArquivoAtual);
			gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (window), sTextoUTF8);
		}
	}
	//loop, janela interativa está em execução
	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_OK)
	{
		//arquivo selecionado
		sArquivo = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (window));
		//devolver nome do arquivo sempre em UTF8
		if (sArquivo && !be_arquivo_localidade_UTF8 ())
		{
			//conversão
			BE_free (sTextoUTF8);
			sTextoUTF8 = sArquivo;
			sArquivo = BE_ISO_UTF8 (sTextoUTF8);
		}
	}
	else
		sArquivo = NULL;
	//conversão de barras, devolver a barra utilizada pelo S.O. atual
	if (sArquivo)
	{
		BE_free (sTemp);
		sTemp = sArquivo;
		sArquivo = be_strreplace (sTemp, "/", G_DIR_SEPARATOR_S);
	}

	//liberando memória
	gtk_widget_destroy (window);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTextoUTF8);
	//retornar
	return sArquivo;
}
