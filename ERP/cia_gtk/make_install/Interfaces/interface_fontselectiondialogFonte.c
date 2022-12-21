/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_fontselectiondialogFonte.h"

GtkWidget *be_Interface_criar_fontselectiondialogFonte (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	//ajustes final
	_be_Interfaces_fontselectiondialogFonte_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_fontselectiondialogFonte_cancel_button2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//can_focus
	//relief
	//focus_on_click
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new ();
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "cancel_button2");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "cancel_button2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_fontselectiondialogFonte_apply_button1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//can_default
	//can_focus
	//relief
	//focus_on_click
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new ();
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "apply_button1");
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "apply_button1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_fontselectiondialogFonte_ok_button2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//can_focus
	//relief
	//focus_on_click
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new ();
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "ok_button2");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "ok_button2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_fontselectiondialogFonte_font_selection1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkFontSelection:
	//border_width
	//visible
	//preview_text
	GtkWidget *filha = NULL;

	gtk_widget_set_name (mae, "font_selection1");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "font_selection1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);

	return mae;
}

void _be_Interfaces_fontselectiondialogFonte_ajustes_finais (GtkWidget *janela)
{
}
