/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowListaG.h"

GtkWidget *be_Interface_criar_windowListaG (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Lista Gigante");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "listag.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_NORMAL);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 400, 300);//default_width e default_height
	gtk_widget_set_name (mae, "windowListaG");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowListaG_table1 (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowListaG_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_table1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_table_new (4, 2, FALSE);//n_rows, n_columns e homogeneous
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
	filha = be_Interface_criar_windowListaG_scrolledwindow1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowListaG_toolbarListaG (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 2, 0, 1, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowListaG_hbuttonbox1 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 2, 3, 4, 0, 0, 0, 0);
	filha = be_Interface_criar_windowListaG_entryLocalizar (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowListaG_image4 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_scrolledwindow1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	filha = be_Interface_criar_windowListaG_treeviewListaG (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_treeviewListaG (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	//-> row_activated
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_tree_view_new ();
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (mae), TRUE);//headers_visible
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (mae), TRUE);//rules_hint
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (mae), TRUE);//reorderable
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mae), FALSE);//enable_search
	gtk_widget_set_name (mae, "treeviewListaG");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "row_activated", G_CALLBACK (_be_Interface_sinal_windowListaG_on_treeviewLista_row_activated), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "treeviewListaG", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_toolbarListaG (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolbar:
	//visible
	//orientation
	//toolbar_style
	//tooltips
	//show_arrow
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_toolbar_new ();
	gtk_toolbar_set_orientation (GTK_TOOLBAR (mae), GTK_ORIENTATION_HORIZONTAL);//orientation
	gtk_toolbar_set_style (GTK_TOOLBAR (mae), GTK_TOOLBAR_ICONS);//toolbar_style
	gtk_toolbar_set_show_arrow (GTK_TOOLBAR (mae), TRUE);//show_arrow
	gtk_toolbar_set_tooltips (GTK_TOOLBAR (mae), TRUE);//tooltips
	gtk_widget_set_name (mae, "toolbarListaG");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbarListaG", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_toolbuttonPrimeiro (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowListaG_toolbuttonVoltar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowListaG_toolitem1 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), TRUE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), FALSE);//homogeneous
	filha = be_Interface_criar_windowListaG_toolbuttonAvancar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowListaG_toolbuttonUltimo (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_toolbuttonPrimeiro (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-goto-first");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Primeiro", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonPrimeiro");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Primeiro", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowListaG_on_toolbuttonPrimeiro_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonPrimeiro", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_toolbuttonVoltar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-go-back");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Voltar", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonVoltar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Voltar", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowListaG_on_toolbuttonVoltar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonVoltar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_toolitem1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolItem:
	//border_width
	//visible
	//visible_horizontal
	//visible_vertical
	//is_important
	GtkWidget *filha = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_item_new ();
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_widget_set_name (mae, "toolitem1");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolitem1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_labelPosicao (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_labelPosicao (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("1 de 15600");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "1 de 15600");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_CENTER);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelPosicao");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelPosicao", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_toolbuttonAvancar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-go-forward");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "AvanÃ§ar", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonAvancar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "AvanÃ§ar", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowListaG_on_toolbuttonAvancar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonAvancar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_toolbuttonUltimo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-goto-last");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Ãšltimo", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonUltimo");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Ãšltimo", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowListaG_on_toolbuttonUltimo_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonUltimo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_hbuttonbox1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "hbuttonbox1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbuttonbox1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_buttonOK (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowListaG_buttonCancelar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowListaG_on_buttonOK_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_alignment24 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_alignment24 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment24");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment24", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_hbox24 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_hbox24 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox24");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox24", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_image576 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowListaG_label59 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_image576 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image576");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image576", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_label59 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label59");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label59", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_buttonCancelar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	filha = be_Interface_criar_windowListaG_alignment25 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_alignment25 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment25");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment25", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_hbox25 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_hbox25 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox25");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox25", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowListaG_image577 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowListaG_label60 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_image577 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image577");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image577", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_label60 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label60");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label60", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_entryLocalizar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkEntry:
	//visible
	//tooltip
	//can_focus
	//has_focus
	//editable
	//visibility
	//max_length
	//text
	//has_frame
	//invisible_char
	//activates_default
	//-> changed
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_entry_new ();
	gtk_editable_set_editable (GTK_EDITABLE (mae), TRUE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 64);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), FALSE);//activates_default
	gtk_widget_set_name (mae, "entryLocalizar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Localizar nos campos de texto.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "changed", G_CALLBACK (_be_Interface_sinal_windowListaG_on_entryLocalizar_changed), Data);
	//teclas de atalho
	if (accel)
		gtk_widget_add_accelerator (mae, "grab_focus", accel, GDK_L, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryLocalizar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowListaG_image4 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-find", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image4");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image4", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowListaG_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
	gtk_widget_grab_focus (BE_interfaces_widget (janela, "entryLocalizar"));//has_focus
}
