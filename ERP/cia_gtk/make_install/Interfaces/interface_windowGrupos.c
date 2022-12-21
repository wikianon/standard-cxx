/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowGrupos.h"

GtkWidget *be_Interface_criar_windowGrupos (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Grupos");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 500, -1);//default_width
	gtk_widget_set_name (mae, "windowGrupos");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowGrupos_table1 (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowGrupos_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_table1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "table1");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "table1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_scrolledwindow1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowGrupos_scrolledwindow2 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowGrupos_hbuttonbox1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 3, 1, 2, 0, GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowGrupos_vbuttonbox1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 0, 1, 0, 0, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_scrolledwindow1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkScrolledWindow:
	//visible
	//can_focus
	//hscrollbar_policy
	//vscrollbar_policy
	//shadow_type
	//window_placement
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mae), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);//hscrollbar_policy e vscrollbar_policy
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (mae), GTK_SHADOW_IN);//shadow_type
	gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (mae), GTK_CORNER_TOP_LEFT);//window_placement
	gtk_widget_set_name (mae, "scrolledwindow1");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_treeviewDisponivel (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_treeviewDisponivel (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTreeView:
	//visible
	//can_focus
	//headers_visible
	//rules_hint
	//reorderable
	//enable_search
	//fixed_height_mode
	//hover_selection
	//hover_expand
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_tree_view_new ();
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (mae), TRUE);//headers_visible
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (mae), FALSE);//rules_hint
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (mae), FALSE);//reorderable
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mae), TRUE);//enable_search
	gtk_widget_set_name (mae, "treeviewDisponivel");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "treeviewDisponivel", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_scrolledwindow2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkScrolledWindow:
	//visible
	//can_focus
	//hscrollbar_policy
	//vscrollbar_policy
	//shadow_type
	//window_placement
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mae), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);//hscrollbar_policy e vscrollbar_policy
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (mae), GTK_SHADOW_IN);//shadow_type
	gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (mae), GTK_CORNER_TOP_LEFT);//window_placement
	gtk_widget_set_name (mae, "scrolledwindow2");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_treeviewSelecionado (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_treeviewSelecionado (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTreeView:
	//visible
	//can_focus
	//headers_visible
	//rules_hint
	//reorderable
	//enable_search
	//fixed_height_mode
	//hover_selection
	//hover_expand
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_tree_view_new ();
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (mae), TRUE);//headers_visible
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (mae), FALSE);//rules_hint
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (mae), FALSE);//reorderable
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mae), TRUE);//enable_search
	gtk_widget_set_name (mae, "treeviewSelecionado");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "treeviewSelecionado", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbuttonbox1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	filha = be_Interface_criar_windowGrupos_buttonOK (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowGrupos_buttonCancelar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonOK_clicked), Data);
	//teclas de atalho
	if (accel)
		gtk_widget_add_accelerator (mae, "clicked", accel, GDK_Return, 0, GTK_ACCEL_VISIBLE);
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment18 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment18 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment18");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment18", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox18 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox18 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox18");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox18", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image570 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label53 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image570 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image570");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image570", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label53 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label53");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label53", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonCancelar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	filha = be_Interface_criar_windowGrupos_alignment19 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment19 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment19");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment19", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox19 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox19 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox19");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox19", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image571 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label54 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image571 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image571");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image571", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label54 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label54");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label54", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_vbuttonbox1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkVButtonBox:
	//visible
	//layout_style
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_vbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (mae), GTK_BUTTONBOX_DEFAULT_STYLE);//layout_style
	gtk_box_set_spacing (GTK_BOX (mae), 0);//spacing
	gtk_widget_set_name (mae, "vbuttonbox1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "vbuttonbox1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_buttonAdicionar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowGrupos_buttonRemover (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowGrupos_buttonAcima (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowGrupos_buttonAbaixo (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowGrupos_buttonTodos (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowGrupos_buttonNenhum (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonAdicionar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
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
	gtk_widget_set_name (mae, "buttonAdicionar");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonAdicionar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonAdicionar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment15 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment15 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment15");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment15", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox15 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox15 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox15");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox15", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image567 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label50 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image567 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-go-forward", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image567");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image567", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label50 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("_Adicionar");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label50");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label50", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonRemover (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
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
	gtk_widget_set_name (mae, "buttonRemover");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonRemover_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonRemover", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment16 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment16 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment16");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment16", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox16 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox16 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox16");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox16", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image568 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label51 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image568 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-go-back", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image568");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image568", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label51 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("_Remover");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label51");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label51", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonAcima (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
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
	gtk_widget_set_name (mae, "buttonAcima");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonAcima_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonAcima", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment3 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment3 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment3");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment3", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox3 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox3 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox3");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox3", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image3 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label3 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image3 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-go-up", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image3");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image3", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label3 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("Aci_ma");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label3");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label3", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonAbaixo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
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
	gtk_widget_set_name (mae, "buttonAbaixo");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonAbaixo_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonAbaixo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment17 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment17 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment17");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment17", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox17 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox17 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox17");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox17", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image569 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label52 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image569 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-go-down", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image569");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image569", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label52 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("A_baixo");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label52");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label52", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonTodos (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
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
	gtk_widget_set_name (mae, "buttonTodos");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonTodos_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonTodos", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment2 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox2 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image2 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label2 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-add", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("_Todos");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), TRUE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_buttonNenhum (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_default
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
	gtk_widget_set_name (mae, "buttonNenhum");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowGrupos_on_buttonNenhum_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonNenhum", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_alignment1 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_alignment1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_hbox1 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_hbox1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowGrupos_image1 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowGrupos_label1 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_image1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-remove", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowGrupos_label1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new_with_mnemonic ("_Nenhum");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), TRUE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowGrupos_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
}
