/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowPropriedades.h"

GtkWidget *be_Interface_criar_windowPropriedades (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Propriedades");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "propriedades.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 500, 300);//default_width e default_height
	gtk_widget_set_name (mae, "windowPropriedades");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowPropriedades_table4 (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowPropriedades_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_table4 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_table_new (2, 1, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 4);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 4);//column_spacing
	gtk_widget_set_name (mae, "table4");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "table4", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_scrolledwindow5 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowPropriedades_hbuttonbox2 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, 0, 0, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_scrolledwindow5 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mae), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);//hscrollbar_policy e vscrollbar_policy
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (mae), GTK_SHADOW_NONE);//shadow_type
	gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (mae), GTK_CORNER_TOP_LEFT);//window_placement
	gtk_widget_set_name (mae, "scrolledwindow5");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "scrolledwindow5", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_viewportFundo (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_viewportFundo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkViewport:
	//visible
	//shadow_type
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_viewport_new (NULL, NULL);
	gtk_viewport_set_shadow_type (GTK_VIEWPORT (mae), GTK_SHADOW_NONE);//shadow_type
	gtk_widget_set_name (mae, "viewportFundo");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "viewportFundo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_tableFundo (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_tableFundo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkTable:
	//visible
	//n_rows
	//n_columns
	//homogeneous
	//row_spacing
	//column_spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_table_new (9, 3, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 2);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 2);//column_spacing
	gtk_widget_set_name (mae, "tableFundo");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "tableFundo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_labelNumero (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelFonte (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 2, 3, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelCor (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 3, 4, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelArquivo (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 4, 5, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelDiretorio (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 5, 6, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelLista (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 6, 7, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryNumero (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryFonte (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryCor (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 3, 4, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryArquivo (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 4, 5, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryDiretorio (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 5, 6, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_buttonFonte (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 2, 3, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_buttonCor (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 3, 4, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_buttonArquivo (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 4, 5, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_buttonDiretorio (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 5, 6, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryLista (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 6, 7, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_buttonLista (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 6, 7, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_entryPropriedades (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelPropriedades (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labellogico (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 7, 8, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_checkbuttonLogico (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 2, 3, 7, 8, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelRotulo (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 8, 9, GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPropriedades_labelInformacao (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 1, 2, 8, 9, GTK_FILL, 0, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelNumero (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("NÃºmero");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelNumero");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelNumero", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelFonte (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Fonte");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelFonte");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelFonte", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelCor (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Cor");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelCor");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelCor", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelArquivo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Arquivo");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelArquivo");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelArquivo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelDiretorio (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("DiretÃ³rio");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelDiretorio");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelDiretorio", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelLista (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Lista");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelLista");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelLista", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryNumero (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 0);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryNumero");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryNumero", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryFonte (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 0);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryFonte");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryFonte", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryCor (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 0);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryCor");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryCor", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryArquivo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 0);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryArquivo");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryArquivo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryDiretorio (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 0);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryDiretorio");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryDiretorio", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonFonte (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "buttonFonte");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_buttonFonte_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonFonte", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image16 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image16 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-select-font", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image16");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image16", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonCor (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "buttonCor");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_buttonCor_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonCor", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image12 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image12 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-select-color", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image12");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image12", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonArquivo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "buttonArquivo");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_buttonArquivo_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonArquivo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image13 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image13 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-floppy", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image13");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image13", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonDiretorio (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "buttonDiretorio");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_buttonDiretorio_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonDiretorio", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image14 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image14 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-directory", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image14");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image14", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryLista (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 0);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryLista");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryLista", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonLista (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "buttonLista");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_buttonLista_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonLista", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image464 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image464 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-indent", 4);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image464");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image464", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_entryPropriedades (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_editable_set_editable (GTK_EDITABLE (mae), FALSE);//editable
	gtk_entry_set_visibility (GTK_ENTRY (mae), TRUE);//visibility
	gtk_entry_set_max_length (GTK_ENTRY (mae), 256);//max_length
	gtk_entry_set_has_frame (GTK_ENTRY (mae), TRUE);//has_frame
	gtk_entry_set_invisible_char (GTK_ENTRY (mae), '*');//invisible_char
	gtk_entry_set_activates_default (GTK_ENTRY (mae), TRUE);//activates_default
	gtk_widget_set_name (mae, "entryPropriedades");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "entryPropriedades", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelPropriedades (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Textos");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelPropriedades");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelPropriedades", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labellogico (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("LÃ³gico");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labellogico");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labellogico", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_checkbuttonLogico (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkCheckButton:
	//visible
	//can_focus
	//label
	//use_underline
	//relief
	//focus_on_click
	//active
	//inconsistent
	//draw_indicator
	//-> toggled
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_check_button_new ();
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (mae), TRUE);//draw_indicator
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (mae), FALSE);//active
	gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (mae), FALSE);//inconsistent
	gtk_widget_set_name (mae, "checkbuttonLogico");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "toggled", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_checkbuttonLogico_toggled), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "checkbuttonLogico", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelRotulo (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("RÃ³tulo");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelRotulo");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelRotulo", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_labelInformacao (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("InformaÃ§Ãµes");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_markup (GTK_LABEL (mae), "InformaÃ§Ãµes");//label e use_markup
	gtk_label_set_use_markup (GTK_LABEL (mae), TRUE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), TRUE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0, 0);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "labelInformacao");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "labelInformacao", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_hbuttonbox2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "hbuttonbox2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbuttonbox2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_buttonOK (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPropriedades_buttonCancelar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowPropriedades_on_buttonOK_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_alignment26 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_alignment26 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment26");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment26", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_hbox26 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_hbox26 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox26");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox26", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image578 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowPropriedades_label61 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image578 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image578");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image578", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_label61 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label61");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label61", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_buttonCancelar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	filha = be_Interface_criar_windowPropriedades_alignment27 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_alignment27 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "alignment27");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "alignment27", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_hbox27 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_hbox27 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkHBox:
	//visible
	//homogeneous
	//spacing
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_hbox_new (FALSE, 2);//homogeneous e spacing
	gtk_widget_set_name (mae, "hbox27");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbox27", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPropriedades_image579 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = be_Interface_criar_windowPropriedades_label62 (janela, accel, tip, Data, NULL);
	gtk_box_pack_start (GTK_BOX (mae), filha, FALSE, FALSE, 0);//expand, fill e padding
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_image579 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "image579");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image579", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPropriedades_label62 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "label62");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label62", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowPropriedades_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
}
