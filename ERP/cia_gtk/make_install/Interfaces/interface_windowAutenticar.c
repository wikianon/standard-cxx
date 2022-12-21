/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowAutenticar.h"

GtkWidget *be_Interface_criar_windowAutenticar (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Autenticar");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), FALSE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), FALSE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "beos113.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_widget_set_name (mae, "windowAutenticar");
	gtk_widget_set_size_request (mae, 330, -1);//width_request
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowAutenticar_table5 (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowAutenticar_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_table5 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTable:
	//border_width
	//visible
	//n_rows
	//n_columns
	//homogeneous
	//row_spacing
	//column_spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_table_new (5, 3, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 4);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 4);//column_spacing
	gtk_widget_set_name (mae, "table5");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "table5", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_label5 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_label6 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 1, 2, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_image17 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowAutenticar_entryIdentidade (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 0, 1, 0, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_entrySenha (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 1, 2, 0, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_hbuttonbox3 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 3, 4, 5, 0, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_hseparator1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 3, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowAutenticar_frameAlterarSenha (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 3, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label5 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Identidade");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label5");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label5", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label6 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Senha");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label6");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label6", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_image17 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImage:
	//visible
	//pixbuf
	//xalign
	//yalign
	//xpad
	//ypad
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_new_from_file ("acesso.png");//pixbuf
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image17");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image17", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_entryIdentidade (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_entry_set_max_length (GTK_ENTRY (mae), 32);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryIdentidade");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryIdentidade", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_entrySenha (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkEntry:
	//visible
	//can_focus
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
	gtk_entry_set_visibility (GTK_ENTRY (mae), FALSE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 32);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entrySenha");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entrySenha", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_hbuttonbox3 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHButtonBox:
	//visible
	//layout_style
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (mae), GTK_BUTTONBOX_SPREAD);//layout_style
	gtk_box_set_spacing (GTK_BOX (mae), 2);//spacing
	gtk_widget_set_name (mae, "hbuttonbox3");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbuttonbox3", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_buttonOK (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowAutenticar_buttonCancelar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
	//has_default
	//can_focus
	//relief
	//focus_on_click
	//-> clicked
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new ();
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "buttonOK");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowAutenticar_on_buttonOK_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_alignment28 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_alignment28 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkAlignment:
	//visible
	//xalign
	//yalign
	//xscale
	//yscale
	//top_padding
	//bottom_padding
	//left_padding
	//right_padding
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_alignment_new (0.5, 0.5, 0, 0);//xalign, yalign, xscale, e yscale
	gtk_alignment_set_padding (GTK_ALIGNMENT (mae), 0, 0, 0, 0);//top_padding, bottom_padding, left_padding, e right_padding
	gtk_widget_set_name (mae, "alignment28");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment28", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_hbox28 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_hbox28 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox28");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox28", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_image580 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowAutenticar_label63 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_image580 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImage:
	//visible
	//stock
	//icon_size
	//xalign
	//yalign
	//xpad
	//ypad
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_new_from_stock ("gtk-ok", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image580");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image580", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label63 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("_OK");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label63");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label63", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_buttonCancelar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_focus
	//relief
	//focus_on_click
	//-> clicked
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new ();
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "buttonCancelar");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect_swapped (G_OBJECT (mae), "clicked", G_CALLBACK (gtk_widget_destroy), janela);
	//teclas de atalho
	if (accel)
		gtk_widget_add_accelerator (mae, "clicked", accel, GDK_Escape, 0, GTK_ACCEL_VISIBLE);
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonCancelar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_alignment29 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_alignment29 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkAlignment:
	//visible
	//xalign
	//yalign
	//xscale
	//yscale
	//top_padding
	//bottom_padding
	//left_padding
	//right_padding
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_alignment_new (0.5, 0.5, 0, 0);//xalign, yalign, xscale, e yscale
	gtk_alignment_set_padding (GTK_ALIGNMENT (mae), 0, 0, 0, 0);//top_padding, bottom_padding, left_padding, e right_padding
	gtk_widget_set_name (mae, "alignment29");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment29", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_hbox29 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_hbox29 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox29");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox29", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_image581 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowAutenticar_label64 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_image581 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImage:
	//visible
	//stock
	//icon_size
	//xalign
	//yalign
	//xpad
	//ypad
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_new_from_stock ("gtk-cancel", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image581");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image581", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label64 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("_Cancelar");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label64");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label64", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_hseparator1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHSeparator:
	//height_request
	//visible
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hseparator_new ();
	gtk_widget_set_name (mae, "hseparator1");
	gtk_widget_set_size_request (mae, -1, 20);//height_request
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hseparator1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_frameAlterarSenha (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkFrame:
	//label_xalign
	//label_yalign
	//shadow_type
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_frame_new (NULL);
	gtk_frame_set_label_align (GTK_FRAME (mae), 0, 0.5);//label_xalign e label_yalign
	gtk_frame_set_shadow_type (GTK_FRAME (mae), GTK_SHADOW_ETCHED_IN);//shadow_type
	gtk_widget_set_name (mae, "frameAlterarSenha");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "frameAlterarSenha", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_alignment4 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	filha = be_Interface_criar_windowAutenticar_label12 (janela, accel, tip, Data, NULL);
	gtk_frame_set_label_widget (GTK_FRAME (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_alignment4 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkAlignment:
	//visible
	//xalign
	//yalign
	//xscale
	//yscale
	//top_padding
	//bottom_padding
	//left_padding
	//right_padding
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_alignment_new (0.5, 0.5, 1, 1);//xalign, yalign, xscale, e yscale
	gtk_alignment_set_padding (GTK_ALIGNMENT (mae), 0, 0, 12, 0);//top_padding, bottom_padding, left_padding, e right_padding
	gtk_widget_set_name (mae, "alignment4");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment4", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_table6 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_table6 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTable:
	//border_width
	//visible
	//n_rows
	//n_columns
	//homogeneous
	//row_spacing
	//column_spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_table_new (2, 3, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 4);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 4);//column_spacing
	gtk_widget_set_name (mae, "table6");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "table6", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowAutenticar_image19 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowAutenticar_label13 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_entryNova (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 0, 1, 0, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_entryConfirma (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 1, 2, 0, 0, 0, 0);
	filha = be_Interface_criar_windowAutenticar_label14 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 1, 2, GTK_FILL, 0, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_image19 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImage:
	//visible
	//pixbuf
	//xalign
	//yalign
	//xpad
	//ypad
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_new_from_file ("senhas.png");//pixbuf
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image19");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image19", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label13 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("<i>Nova</i>");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "<i>Nova</i>");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label13");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label13", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_entryNova (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkEntry:
	//visible
	//can_focus
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
	gtk_entry_set_visibility (GTK_ENTRY (mae), FALSE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 32);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryNova");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryNova", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_entryConfirma (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkEntry:
	//visible
	//can_focus
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
	gtk_entry_set_visibility (GTK_ENTRY (mae), FALSE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 32);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryConfirma");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryConfirma", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label14 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("<i>ConfirmaÃ§Ã£o</i>");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "<i>ConfirmaÃ§Ã£o</i>");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label14");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label14", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowAutenticar_label12 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("<i>Alterar senha</i>");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "<i>Alterar senha</i>");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label12");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label12", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowAutenticar_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_focus (BE_interfaces_widget (janela, "entryIdentidade"));//has_focus
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
}
