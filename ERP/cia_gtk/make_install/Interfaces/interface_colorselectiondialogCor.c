/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_colorselectiondialogCor.h"

GtkWidget *be_Interface_criar_colorselectiondialogCor (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	//ajustes final
	_be_Interfaces_colorselectiondialogCor_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_colorselectiondialogCor_cancel_button1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//can_focus
	//label
	//use_stock
	//relief
	//focus_on_click
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-cancel");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-cancel");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "cancel_button1");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "cancel_button1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_colorselectiondialogCor_ok_button1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//can_focus
	//label
	//use_stock
	//relief
	//focus_on_click
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-ok");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-ok");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "ok_button1");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "ok_button1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_colorselectiondialogCor_help_button1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//can_default
	//can_focus
	//label
	//use_stock
	//relief
	//focus_on_click
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-help");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-help");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "help_button1");
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "help_button1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_colorselectiondialogCor_color_selectionCor (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkColorSelection:
	//border_width
	//visible
	//has_opacity_control
	//has_palette
	GtkWidget *filha = NULL;

	gtk_widget_set_name (mae, "color_selectionCor");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 5);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "color_selectionCor", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);

	return mae;
}

void _be_Interfaces_colorselectiondialogCor_ajustes_finais (GtkWidget *janela)
{
}
