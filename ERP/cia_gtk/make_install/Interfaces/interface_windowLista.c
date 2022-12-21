/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowLista.h"

GtkWidget *be_Interface_criar_windowLista (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Lista");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "arquivo006.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 500, 200);//default_width e default_height
	gtk_widget_set_name (mae, "windowLista");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowLista_table1 (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowLista_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_table1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_table_new (2, 2, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 4);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 4);//column_spacing
	gtk_widget_set_name (mae, "table1");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "table1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_scrolledwindow1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowLista_hbuttonbox1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 2, 1, 2, 0, GTK_FILL, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_scrolledwindow1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkScrolledWindow:
	//visible
	//hscrollbar_policy
	//vscrollbar_policy
	//shadow_type
	//window_placement
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mae), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);//hscrollbar_policy e vscrollbar_policy
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (mae), GTK_SHADOW_IN);//shadow_type
	gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (mae), GTK_CORNER_TOP_LEFT);//window_placement
	gtk_widget_set_name (mae, "scrolledwindow1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_treeviewLista (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_treeviewLista (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTreeView:
	//visible
	//tooltip
	//can_focus
	//headers_visible
	//rules_hint
	//reorderable
	//enable_search
	//fixed_height_mode
	//hover_selection
	//hover_expand
	//-> row_activated
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_tree_view_new ();
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (mae), TRUE);//headers_visible
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (mae), TRUE);//rules_hint
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (mae), TRUE);//reorderable
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mae), TRUE);//enable_search
	gtk_widget_set_name (mae, "treeviewLista");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Digite a palavra para localizar.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "row_activated", G_CALLBACK (_be_Interface_sinal_windowLista_on_treeviewLista_row_activated), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "treeviewLista", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_hbuttonbox1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHButtonBox:
	//visible
	//layout_style
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (mae), GTK_BUTTONBOX_DEFAULT_STYLE);//layout_style
	gtk_box_set_spacing (GTK_BOX (mae), 2);//spacing
	gtk_widget_set_name (mae, "hbuttonbox1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbuttonbox1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_buttonOK (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowLista_buttonCancelar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowLista_on_buttonOK_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_alignment22 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_alignment22 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment22");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment22", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_hbox22 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_hbox22 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox22");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox22", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_image574 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowLista_label57 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_image574 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image574");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image574", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_label57 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label57");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label57", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_buttonCancelar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	filha = be_Interface_criar_windowLista_alignment23 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_alignment23 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment23");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment23", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_hbox23 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_hbox23 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox23");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox23", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowLista_image575 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowLista_label58 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_image575 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image575");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image575", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowLista_label58 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label58");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label58", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowLista_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
}
