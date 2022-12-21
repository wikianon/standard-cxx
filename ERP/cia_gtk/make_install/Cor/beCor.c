#include "beCor.h"

gchar *be_Cor (GtkWindow *winPai, const gchar *sCorAtual)
{
	GtkWidget *window = NULL;
	GtkWidget *selcor = NULL;
	GdkColor cor;
	gchar *sCor = NULL;

	//janela
	window = gtk_color_selection_dialog_new ("Selecionar cor");
	//centralizada
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);
	//seleção de cor
	selcor = ((GtkColorSelectionDialog *) window)->colorsel;
	//controles extras
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION (selcor), TRUE);
	//filha da principal
	if (winPai)
		gtk_window_set_transient_for (GTK_WINDOW (window), winPai);
	//cor atual
	if (sCorAtual)
		if (gdk_color_parse (sCorAtual, &cor))
			gtk_color_selection_set_current_color (GTK_COLOR_SELECTION (selcor), &cor);

	//loop
	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_OK)
	{
		gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (selcor), &cor);
		sCor = be_Cor_parse (&cor);
	}

	//liberando memória
	gtk_widget_destroy (window);

	return sCor;
}

gchar *be_Cor_parse (GdkColor *color)
{
	gchar *sTemp = NULL, *sTemp1 = NULL;

	sTemp = g_strdup_printf ("#%.2x%.2x%.2x", (color->red)/256, (color->green)/256, (color->blue)/256);
	sTemp1 = g_ascii_strup (sTemp, -1);
	BE_free (sTemp);

	return sTemp1;
}
