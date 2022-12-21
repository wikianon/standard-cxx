/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_dialogEntrada.h"

GtkWidget *be_Interface_criar_dialogEntrada (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (mae), "Entrada");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), FALSE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 350, 100);//default_width e default_height
	gtk_widget_set_name (mae, "dialogEntrada");
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	be_Interface_criar_dialogEntrada_dialog_vbox2 (mae, accel, tip, Data, (GTK_DIALOG (mae))->vbox);
	be_Interface_criar_dialogEntrada_dialog_action_area2 (mae, accel, tip, Data, (GTK_DIALOG (mae))->action_area);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	//ajustes final
	_be_Interfaces_dialogEntrada_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_dialogEntrada_dialog_vbox2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkVBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_vbox_new (FALSE, 0);//homogeneous e spacing
	gtk_widget_set_name (mae, "dialog-vbox2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "dialog-vbox2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_dialogEntrada_labelMensagem (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 4);//expand, fill e padding
	filha = be_Interface_criar_dialogEntrada_entryMensagem (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 6);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_dialogEntrada_dialog_action_area2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHButtonBox:
	//visible
	//layout_style
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (mae), GTK_BUTTONBOX_SPREAD);//layout_style
	gtk_widget_set_name (mae, "dialog-action_area2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "dialog-action_area2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	be_Interface_criar_dialogEntrada_okbutton1 (janela, accel, tip, Data, NULL);//chamada especial
	be_Interface_criar_dialogEntrada_cancelbutton1 (janela, accel, tip, Data, NULL);//chamada especial
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_dialogEntrada_okbutton1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_button_new_from_stock ("gtk-ok");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-ok");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	if (janela)
		gtk_dialog_add_action_widget (GTK_DIALOG (janela), mae, -5);//response_id
	gtk_widget_set_name (mae, "okbutton1");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "okbutton1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_dialogEntrada_cancelbutton1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
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
	gtk_widget_set_name (mae, "cancelbutton1");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "cancelbutton1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_dialogEntrada_labelMensagem (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkLabel:
	//visible
	//label
	//use_underline
	//use_markup
	//justify
	//wrap
	//selectable
	//xalign
	//yalign
	//xpad
	//ypad
	//ellipsize
	//width_chars
	//single_line_mode
	//angle
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_label_new ("Texto");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "Texto");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelMensagem");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelMensagem", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_dialogEntrada_entryMensagem (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkEntry:
	//visible
	//can_focus
	//has_focus
	//editable
	//visibility
	//max_length
	//text
	//has_frame
	//invisible_char
	//activates_default
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_entry_new ();
	gtk_editable_set_editable (GTK_EDITABLE (mae), TRUE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 256);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryMensagem");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryMensagem", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_dialogEntrada_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "okbutton1"));//has_default
	gtk_widget_grab_focus (BE_interfaces_widget (janela, "entryMensagem"));//has_focus
}
