/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_filechooserdialogAbrir.h"

GtkWidget *be_Interface_criar_filechooserdialogAbrir (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	//ajustes final
	_be_Interfaces_filechooserdialogAbrir_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_filechooserdialogAbrir_dialog_vbox1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkVBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_vbox_new (FALSE, 24);//homogeneous e spacing
	gtk_widget_set_name (mae, "dialog-vbox1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "dialog-vbox1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_filechooserdialogAbrir_dialog_action_area1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHButtonBox:
	//visible
	//layout_style
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (mae), GTK_BUTTONBOX_END);//layout_style
	gtk_widget_set_name (mae, "dialog-action_area1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "dialog-action_area1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	be_Interface_criar_filechooserdialogAbrir_button1 (janela, accel, tip, Data, NULL);//chamada especial
	be_Interface_criar_filechooserdialogAbrir_buttonOK (janela, accel, tip, Data, NULL);//chamada especial
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_filechooserdialogAbrir_button1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//can_focus
	//label
	//use_stock
	//relief
	//focus_on_click
	//response_id
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-cancel");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-cancel");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	if (janela)
		gtk_dialog_add_action_widget (GTK_DIALOG (janela), mae, -6);//response_id
	gtk_widget_set_name (mae, "button1");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "button1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_filechooserdialogAbrir_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//has_default
	//can_focus
	//label
	//use_stock
	//relief
	//focus_on_click
	//response_id
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-open");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-open");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	if (janela)
		gtk_dialog_add_action_widget (GTK_DIALOG (janela), mae, -5);//response_id
	gtk_widget_set_name (mae, "buttonOK");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

void _be_Interfaces_filechooserdialogAbrir_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
}
