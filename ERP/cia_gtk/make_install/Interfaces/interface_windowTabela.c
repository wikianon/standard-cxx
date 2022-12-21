/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowTabela.h"

GtkWidget *be_Interface_criar_windowTabela (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Tabela");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "janela.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 400, 300);//default_width e default_height
	gtk_widget_set_name (mae, "windowTabela");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowTabela_tableTabela (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowTabela_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_tableTabela (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTable:
	//border_width
	//visible
	//n_rows
	//n_columns
	//homogeneous
	//row_spacing
	//column_spacing
	//-> key_press_event
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_table_new (2, 1, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 0);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 0);//column_spacing
	gtk_widget_set_name (mae, "tableTabela");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 2);//border_width
	//sinais
	g_signal_connect (G_OBJECT (mae), "key_press_event", G_CALLBACK (_be_Interface_sinal_windowTabela_on_tableTabela_key_press_event), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "tableTabela", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowTabela_scrolledwindow6 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowTabela_toolbarTabela (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_scrolledwindow6 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (mae), GTK_SHADOW_NONE);//shadow_type
	gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (mae), GTK_CORNER_TOP_LEFT);//window_placement
	gtk_widget_set_name (mae, "scrolledwindow6");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow6", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowTabela_treeviewTabela (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_treeviewTabela (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	//-> cursor_changed
	//-> key_press_event
	//-> button_press_event
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_tree_view_new ();
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (mae), TRUE);//headers_visible
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (mae), TRUE);//rules_hint
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (mae), TRUE);//reorderable
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mae), FALSE);//enable_search
	gtk_widget_set_name (mae, "treeviewTabela");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "cursor_changed", G_CALLBACK (_be_Interface_sinal_windowTabela_on_treeviewTabela_cursor_changed), Data);
	g_signal_connect (G_OBJECT (mae), "key_press_event", G_CALLBACK (_be_Interface_sinal_windowTabela_on_treeviewTabela_key_press_event), Data);
	g_signal_connect (G_OBJECT (mae), "button_press_event", G_CALLBACK (_be_Interface_sinal_windowTabela_on_treeviewTabela_button_press_event), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "treeviewTabela", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbarTabela (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "toolbarTabela");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbarTabela", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowTabela_toolbuttonPrimeiro (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonVoltar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolitem2 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), FALSE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonAvancar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonUltimo (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_separatortoolitem5 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), FALSE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonAdicionar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonRemover (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonSalvar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_separatortoolitem6 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), FALSE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonDados (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonRelatorios (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_separatortoolitem7 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), FALSE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonOpcoes (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowTabela_toolbuttonPerfil (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonPrimeiro (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Primeiro (Ctrl+Home)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonPrimeiro");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Primeiro (Ctrl+Home)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonPrimeiro_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonPrimeiro", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonVoltar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Voltar (Ctrl+Esq)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonVoltar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Voltar (Ctrl+Esq)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonVoltar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonVoltar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolitem2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "toolitem2");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolitem2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowTabela_labelPosicao (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_labelPosicao (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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

GtkWidget *be_Interface_criar_windowTabela_toolbuttonAvancar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "AvanÃ§ar (Ctrl+Dir)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonAvancar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "AvanÃ§ar (Ctrl+Dir)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonAvancar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonAvancar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonUltimo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Ãšltimo (Ctrl+End)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonUltimo");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Ãšltimo (Ctrl+End)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonUltimo_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonUltimo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_separatortoolitem5 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorToolItem:
	//visible
	//draw
	//visible_horizontal
	//visible_vertical
	GtkWidget *filha = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_separator_tool_item_new ();
	gtk_separator_tool_item_set_draw (GTK_SEPARATOR_TOOL_ITEM (mae), TRUE);//draw
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_widget_set_name (mae, "separatortoolitem5");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separatortoolitem5", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonAdicionar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-add");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Adicionar (Ctrl+A)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonAdicionar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Adicionar (Ctrl+A)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonAdicionar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonAdicionar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonRemover (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//sensitive
	//tooltip
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-remove");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Remover (Ctrl+X)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonRemover");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Remover (Ctrl+X)", NULL);//tooltip
	gtk_widget_set_sensitive (mae, FALSE);//sensitive
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonRemover_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonRemover", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonSalvar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//sensitive
	//tooltip
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new_from_stock ("gtk-save");//stock_id
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Salvar (Ctrl+S)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonSalvar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Salvar (Ctrl+S)", NULL);//tooltip
	gtk_widget_set_sensitive (mae, FALSE);//sensitive
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonSalvar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonSalvar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_separatortoolitem6 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorToolItem:
	//visible
	//draw
	//visible_horizontal
	//visible_vertical
	GtkWidget *filha = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_separator_tool_item_new ();
	gtk_separator_tool_item_set_draw (GTK_SEPARATOR_TOOL_ITEM (mae), TRUE);//draw
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_widget_set_name (mae, "separatortoolitem6");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separatortoolitem6", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonDados (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//label
	//use_underline
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new ((image = gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_SMALL_TOOLBAR)), "Ajustar _dados");//stock_id e label
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (mae), "Ajustar _dados");//label
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (mae), TRUE);//use_underline
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Ajustar dados (Ctrl+D)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonDados");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Ajustar dados (Ctrl+D)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonDados_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonDados", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonRelatorios (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//sensitive
	//tooltip
	//label
	//use_underline
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new ((image = gtk_image_new_from_stock ("gtk-print", GTK_ICON_SIZE_SMALL_TOOLBAR)), "_RelatÃ³rios");//stock_id e label
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (mae), "_RelatÃ³rios");//label
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (mae), TRUE);//use_underline
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "RelatÃ³rios (Ctrl+P)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonRelatorios");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "RelatÃ³rios (Ctrl+P)", NULL);//tooltip
	gtk_widget_set_sensitive (mae, FALSE);//sensitive
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonRelatorios_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonRelatorios", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_separatortoolitem7 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorToolItem:
	//visible
	//draw
	//visible_horizontal
	//visible_vertical
	GtkWidget *filha = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_separator_tool_item_new ();
	gtk_separator_tool_item_set_draw (GTK_SEPARATOR_TOOL_ITEM (mae), TRUE);//draw
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_widget_set_name (mae, "separatortoolitem7");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separatortoolitem7", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonOpcoes (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//label
	//use_underline
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new ((image = gtk_image_new_from_stock ("gtk-preferences", GTK_ICON_SIZE_SMALL_TOOLBAR)), "_OpÃ§Ãµes");//stock_id e label
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (mae), "_OpÃ§Ãµes");//label
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (mae), TRUE);//use_underline
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "OpÃ§Ãµes (Ctrl+O)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonOpcoes");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "OpÃ§Ãµes (Ctrl+O)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonOpcoes_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonOpcoes", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowTabela_toolbuttonPerfil (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkToolButton:
	//visible
	//tooltip
	//label
	//use_underline
	//stock_id
	//visible_horizontal
	//visible_vertical
	//is_important
	//-> clicked
	GtkWidget *filha = NULL;
	GtkWidget *image = NULL;

	if (!mae)
		mae = (GtkWidget *) gtk_tool_button_new ((image = gtk_image_new_from_stock ("gtk-home", GTK_ICON_SIZE_SMALL_TOOLBAR)), "_Perfil");//stock_id e label
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (mae), "_Perfil");//label
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (mae), TRUE);//use_underline
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Perfil (Ctrl+R)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonPerfil");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Perfil (Ctrl+R)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowTabela_on_toolbuttonPerfil_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonPerfil", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

void _be_Interfaces_windowTabela_ajustes_finais (GtkWidget *janela)
{
}
