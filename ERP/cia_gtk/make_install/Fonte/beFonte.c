#include "beFonte.h"

gchar *be_Fonte (GtkWindow *winPai, const gchar *sFonteAtual)
{
	GtkWidget *window = NULL;
	PangoFontDescription *fontpango = NULL;
	gchar *sFonte = NULL;

	//janela
	window = gtk_font_selection_dialog_new ("Selecionar fonte");
	//centralizada
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);
	//filha da principal
	if (winPai)
		gtk_window_set_transient_for (GTK_WINDOW (window), winPai);
	//fonte atual
	if (sFonteAtual)
	{
		fontpango = pango_font_description_from_string (sFonteAtual);//testando a fonte
		if (pango_font_description_get_size (fontpango) <= 0)//verificando tamanho da fonte
			pango_font_description_set_size (fontpango, 12 * PANGO_SCALE);
		sFonte = pango_font_description_to_string (fontpango);
		gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG (window), sFonte);
		BE_free (sFonte);
		pango_font_description_free (fontpango);
	}
	//loop
	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_OK)
		sFonte = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (window));
	else
		sFonte = NULL;

	//liberando memória
	gtk_widget_destroy (window);

	return sFonte;
}
