#include "beDiretorio.h"

gchar *be_Diretorio (GtkWindow *winPai, gboolean bUTF8, const gchar *sDiretorioAtual)
{
	GtkWidget *window = NULL;
	gchar *sDiretorio = NULL, *sTextoUTF8 = NULL;

	//janela
	sTextoUTF8 = BE_ISO_UTF8 ("Selecionar diret�rio");
	window = gtk_file_chooser_dialog_new (sTextoUTF8, winPai, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "gtk-cancel", GTK_RESPONSE_CANCEL, "gtk-open", GTK_RESPONSE_OK, NULL);
	//centralizada
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);
	//diret�rio atual
	if (sDiretorioAtual)
	{
		if ((bUTF8 && be_arquivo_localidade_UTF8 ()) || (!bUTF8 && !be_arquivo_localidade_UTF8 ()))
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), sDiretorioAtual);
		else if (bUTF8 && !be_arquivo_localidade_UTF8 ())
		{
			//convers�o
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_UTF8_ISO (sDiretorioAtual);
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), sTextoUTF8);
		}
		else if (!bUTF8 && be_arquivo_localidade_UTF8 ())
		{
			//convers�o
			BE_free (sTextoUTF8);
			sTextoUTF8 = BE_ISO_UTF8 (sDiretorioAtual);
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (window), sTextoUTF8);
		}
	}
	//selecionar
	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_OK)
		sDiretorio = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (window));

	if (sDiretorio && !be_arquivo_localidade_UTF8 ())
	{
		//convers�o
		BE_free (sTextoUTF8);
		sTextoUTF8 = sDiretorio;
		sDiretorio = BE_ISO_UTF8 (sTextoUTF8);
	}
	//convers�o de barras
	if (sDiretorio)
	{
		BE_free (sTextoUTF8);
		sTextoUTF8 = sDiretorio;
		sDiretorio = be_strreplace (sTextoUTF8, "\\", "/");
	}

	//liberando mem�ria
	gtk_widget_destroy (window);
	BE_free (sTextoUTF8);

	return sDiretorio;
}
