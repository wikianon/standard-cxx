/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowConsole.h"

GtkWidget *be_Interface_criar_windowConsole (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Console");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), FALSE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), FALSE);//destroy_with_parent
	gtk_window_set_icon_name (GTK_WINDOW (mae), "gtk-fullscreen");//icon_name
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_NORMAL);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 400, 300);//default_width e default_height
	gtk_widget_set_name (mae, "windowConsole");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowConsole_tableConsole (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowConsole_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_tableConsole (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_table_new (3, 1, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 4);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 4);//column_spacing
	gtk_widget_set_name (mae, "tableConsole");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	g_signal_connect (G_OBJECT (mae), "key_press_event", G_CALLBACK (_be_Interface_sinal_windowConsole_on_tableConsole_key_press_event), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "tableConsole", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowConsole_scrolledwindow8 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowConsole_toolbar2 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowConsole_scrolledwindow9 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_scrolledwindow8 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "scrolledwindow8");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow8", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowConsole_treeviewConsole (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_treeviewConsole (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (mae), TRUE);//rules_hint
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (mae), TRUE);//reorderable
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mae), FALSE);//enable_search
	gtk_widget_set_name (mae, "treeviewConsole");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "treeviewConsole", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_toolbar2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "toolbar2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbar2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowConsole_toolbuttonExecutar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowConsole_toolbuttonExportar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), TRUE);//homogeneous
	filha = be_Interface_criar_windowConsole_toolitem3 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_tool_item_set_expand (GTK_TOOL_ITEM (filha), FALSE);//expand
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (filha), FALSE);//homogeneous
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_toolbuttonExecutar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = (GtkWidget *) gtk_tool_button_new ((image = gtk_image_new_from_stock ("gtk-refresh", GTK_ICON_SIZE_SMALL_TOOLBAR)), "E_xecutar");//stock_id e label
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (mae), "E_xecutar");//label
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (mae), TRUE);//use_underline
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Executar consulta selecionada (F5)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonExecutar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Executar consulta selecionada (F5)", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowConsole_on_toolbuttonExecutar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonExecutar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_toolbuttonExportar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = (GtkWidget *) gtk_tool_button_new ((image = gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_SMALL_TOOLBAR)), "_Exportar");//stock_id e label
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (mae), "_Exportar");//label
	if (image)
		gtk_widget_show (image);//exibir imagem anexa
	gtk_tool_button_set_use_underline (GTK_TOOL_BUTTON (mae), TRUE);//use_underline
	gtk_tool_item_set_is_important (GTK_TOOL_ITEM (mae), FALSE);//is_important
	gtk_tool_item_set_visible_horizontal (GTK_TOOL_ITEM (mae), TRUE);//visible_horizontal
	gtk_tool_item_set_visible_vertical (GTK_TOOL_ITEM (mae), TRUE);//visible_vertical
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (mae), tip ? tip : gtk_tooltips_new (), "Exportar (Ctrl+E)", NULL);//tooltip-especial
	gtk_widget_set_name (mae, "toolbuttonExportar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Exportar (Ctrl+E)", NULL);//tooltip
	gtk_widget_set_sensitive (mae, FALSE);//sensitive
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowConsole_on_toolbuttonExportar_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolbuttonExportar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_toolitem3 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "toolitem3");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "toolitem3", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowConsole_labelConsole (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_labelConsole (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Status");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "Status");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_CENTER);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelConsole");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelConsole", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_scrolledwindow9 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mae), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);//hscrollbar_policy e vscrollbar_policy
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (mae), GTK_SHADOW_IN);//shadow_type
	gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (mae), GTK_CORNER_TOP_LEFT);//window_placement
	gtk_widget_set_name (mae, "scrolledwindow9");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow9", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowConsole_textviewConsole (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowConsole_textviewConsole (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTextView:
	//visible
	//can_focus
	//editable
	//overwrite
	//accepts_tab
	//justification
	//wrap_mode
	//cursor_visible
	//pixels_above_lines
	//pixels_below_lines
	//pixels_inside_wrap
	//left_margin
	//right_margin
	//indent
	//text
	GtkWidget *filha = NULL;
	GtkTextBuffer *buffer = NULL;

	if (!mae)
		mae = gtk_text_view_new ();
	gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (mae), FALSE);//accepts_tab
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (mae), TRUE);//cursor_visible
	gtk_text_view_set_editable (GTK_TEXT_VIEW (mae), TRUE);//editable
	gtk_text_view_set_overwrite (GTK_TEXT_VIEW (mae), FALSE);//overwrite
	gtk_text_view_set_justification (GTK_TEXT_VIEW (mae), GTK_JUSTIFY_LEFT);//justification
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (mae), 0);//left_margin
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (mae), 0);//right_margin
	gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (mae), 0);//pixels_above_lines
	gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (mae), 0);//pixels_below_lines
	gtk_text_view_set_pixels_inside_wrap (GTK_TEXT_VIEW (mae), 0);//pixels_inside_wrap
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (mae), GTK_WRAP_CHAR);//wrap_mode
	buffer = NULL;//text
	gtk_widget_set_name (mae, "textviewConsole");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "textviewConsole", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

void _be_Interfaces_windowConsole_ajustes_finais (GtkWidget *janela)
{
}
