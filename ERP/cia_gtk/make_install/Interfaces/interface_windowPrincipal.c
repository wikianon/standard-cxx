/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowPrincipal.h"

GtkWidget *be_Interface_criar_windowPrincipal (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "Controle Integrado Administrativo");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), FALSE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), FALSE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "principal.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_NORMAL);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_window_set_default_size (GTK_WINDOW (mae), 640, 480);//default_width e default_height
	gtk_widget_set_name (mae, "windowPrincipal");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowPrincipal_tablePrincipal (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowPrincipal_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_tablePrincipal (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_table_new (3, 1, FALSE);//n_rows, n_columns e homogeneous
	gtk_table_set_row_spacings (GTK_TABLE (mae), 0);//row_spacing
	gtk_table_set_col_spacings (GTK_TABLE (mae), 0);//column_spacing
	gtk_widget_set_name (mae, "tablePrincipal");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "tablePrincipal", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubar (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPrincipal_statusbarStatus (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, 0, 0, 0);
	filha = be_Interface_criar_windowPrincipal_notebookPrincipal (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuBar:
	//visible
	//pack_direction
	//child_pack_direction
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_bar_new ();
	gtk_widget_set_name (mae, "menubar");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menuEscritorio (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menuOperacoes (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menuAjuda (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menuEscritorio (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuItem:
	//visible
	//label
	//use_underline
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_item_new_with_mnemonic ("_EscritÃ³rio");//label e use_underline
	gtk_widget_set_name (mae, "menuEscritorio");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menuEscritorio", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menuEscritorio_menu (janela, accel, tip, Data, NULL);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menuEscritorio_menu (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenu:
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_new ();
	gtk_widget_set_name (mae, "menuEscritorio_menu");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menuEscritorio_menu", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubarEmpresa (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarUsuario (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_separador2 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarSair (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarEmpresa (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//tooltip
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Empresa");//label e use_underline
	gtk_widget_set_name (mae, "menubarEmpresa");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "InformaÃ§Ãµes da empresa atual.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarEmpresa_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarEmpresa", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image701 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image701 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-dialog-info", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image701");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image701", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarUsuario (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuItem:
	//visible
	//label
	//use_underline
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_item_new_with_mnemonic ("_UsuÃ¡rio");//label e use_underline
	gtk_widget_set_name (mae, "menubarUsuario");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarUsuario", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubarUsuario_menu (janela, accel, tip, Data, NULL);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarUsuario_menu (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenu:
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_new ();
	gtk_widget_set_name (mae, "menubarUsuario_menu");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarUsuario_menu", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubarConsole (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarAuditoria (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarIniciar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarAdicionar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarExcluir (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_separador1 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarConsole (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//tooltip
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Console");//label e use_underline
	gtk_widget_set_name (mae, "menubarConsole");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Linha de comando, acesso direto.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarConsole_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarConsole", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image702 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image702 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-fullscreen", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image702");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image702", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarAuditoria (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Auditoria (log de aÃ§Ãµes)");//label e use_underline
	gtk_widget_set_name (mae, "menubarAuditoria");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarAuditoria_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarAuditoria", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image703 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image703 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-info", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image703");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image703", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarIniciar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//tooltip
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Iniciar");//label e use_underline
	gtk_widget_set_name (mae, "menubarIniciar");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "TransaÃ§Ãµes que serÃ£o iniciadas ao abrir a aplicaÃ§Ã£o.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarIniciar_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarIniciar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image704 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image704 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-home", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image704");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image704", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarAdicionar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Adicionar menu");//label e use_underline
	gtk_widget_set_name (mae, "menubarAdicionar");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarAdicionar_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarAdicionar", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image705 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image705 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-add", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image705");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image705", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarExcluir (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("Exclui_r menu");//label e use_underline
	gtk_widget_set_name (mae, "menubarExcluir");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarExcluir_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarExcluir", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image706 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image706 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-remove", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image706");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image706", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_separador1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorMenuItem:
	//visible
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_separator_menu_item_new ();
	gtk_widget_set_name (mae, "separador1");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separador1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_separador2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorMenuItem:
	//visible
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_separator_menu_item_new ();
	gtk_widget_set_name (mae, "separador2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separador2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarSair (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_stock
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_from_stock ("gtk-quit", accel);//label e use_stock
	gtk_widget_set_name (mae, "menubarSair");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarSair_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarSair", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menuOperacoes (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuItem:
	//visible
	//label
	//use_underline
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_item_new_with_mnemonic ("_OperaÃ§Ãµes");//label e use_underline
	gtk_widget_set_name (mae, "menuOperacoes");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menuOperacoes", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menuOperacoes_menu (janela, accel, tip, Data, NULL);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menuOperacoes_menu (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenu:
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_new ();
	gtk_widget_set_name (mae, "menuOperacoes_menu");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menuOperacoes_menu", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubarJanelas (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarAcesso (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarOrdenada (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarMenu (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_separador5 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarFecharJanela (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarJanelas (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkCheckMenuItem:
	//visible
	//label
	//use_underline
	//active
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_check_menu_item_new_with_mnemonic ("_Janelas externas");//label e use_underline
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (mae), FALSE);//active
	gtk_widget_set_name (mae, "menubarJanelas");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarJanelas_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarJanelas", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarAcesso (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//tooltip
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Acesso rÃ¡pido");//label e use_underline
	gtk_widget_set_name (mae, "menubarAcesso");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "Utilizar cÃ³digo da janela.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarAcesso_activate), Data);
	//teclas de atalho
	if (accel)
		gtk_widget_add_accelerator (mae, "activate", accel, GDK_F6, 0, GTK_ACCEL_VISIBLE);
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarAcesso", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image707 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image707 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-jump-to", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image707");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image707", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarOrdenada (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Lista ordenada");//label e use_underline
	gtk_widget_set_name (mae, "menubarOrdenada");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarOrdenada_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarOrdenada", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image708 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image708 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-indent", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image708");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image708", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarMenu (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuItem:
	//visible
	//label
	//use_underline
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_item_new_with_mnemonic ("_Menu");//label e use_underline
	gtk_widget_set_name (mae, "menubarMenu");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarMenu", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubarMenu_menu (janela, accel, tip, Data, NULL);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarMenu_menu (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenu:
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_new ();
	gtk_widget_set_name (mae, "menubarMenu_menu");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarMenu_menu", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_item1 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_item1 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuItem:
	//label
	//use_underline
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_item_new_with_mnemonic ("item1");//label e use_underline
	gtk_widget_set_name (mae, "item1");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "item1", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_separador5 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorMenuItem:
	//visible
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_separator_menu_item_new ();
	gtk_widget_set_name (mae, "separador5");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separador5", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarFecharJanela (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//sensitive
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_Fechar janela interna");//label e use_underline
	gtk_widget_set_name (mae, "menubarFecharJanela");
	gtk_widget_set_sensitive (mae, FALSE);//sensitive
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarFecharJanela_activate), Data);
	//teclas de atalho
	if (accel)
		gtk_widget_add_accelerator (mae, "activate", accel, GDK_F4, 0, GTK_ACCEL_VISIBLE);
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarFecharJanela", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image709 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image709 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-cancel", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image709");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image709", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menuAjuda (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenuItem:
	//visible
	//label
	//use_underline
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_item_new_with_mnemonic ("Aj_uda");//label e use_underline
	gtk_widget_set_name (mae, "menuAjuda");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menuAjuda", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menuAjuda_menu (janela, accel, tip, Data, NULL);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menuAjuda_menu (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkMenu:
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_menu_new ();
	gtk_widget_set_name (mae, "menuAjuda_menu");
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menuAjuda_menu", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_menubarConfig (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarServidor (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarAjuda (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_separador4 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowPrincipal_menubarSobre (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarConfig (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("_ConfiguraÃ§Ãµes");//label e use_underline
	gtk_widget_set_name (mae, "menubarConfig");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarConfig_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarConfig", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image710 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image710 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-properties", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image710");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image710", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarServidor (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//tooltip
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("Ser_vidor");//label e use_underline
	gtk_widget_set_name (mae, "menubarServidor");
	gtk_tooltips_set_tip (tip ? tip : gtk_tooltips_new (), mae, "InformaÃ§Ãµes do servidor.", NULL);//tooltip
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarServidor_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarServidor", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image711 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image711 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-network", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image711");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image711", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarAjuda (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_underline
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_with_mnemonic ("Aj_uda");//label e use_underline
	gtk_widget_set_name (mae, "menubarAjuda");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarAjuda_activate), Data);
	//teclas de atalho
	if (accel)
		gtk_widget_add_accelerator (mae, "activate", accel, GDK_F1, 0, GTK_ACCEL_VISIBLE);
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarAjuda", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_image712 (janela, accel, tip, Data, NULL);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_image712 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_image_new_from_stock ("gtk-help", 1);//stock e icon_size
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "image712");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "image712", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_separador4 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkSeparatorMenuItem:
	//visible
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_separator_menu_item_new ();
	gtk_widget_set_name (mae, "separador4");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "separador4", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_menubarSobre (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkImageMenuItem:
	//visible
	//label
	//use_stock
	//-> activate
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_image_menu_item_new_from_stock ("gtk-about", accel);//label e use_stock
	gtk_widget_set_name (mae, "menubarSobre");
	gtk_widget_show (mae);//visible
	//sinais
	g_signal_connect (G_OBJECT (mae), "activate", G_CALLBACK (_be_Interface_sinal_windowPrincipal_on_menubarSobre_activate), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "menubarSobre", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_statusbarStatus (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkStatusbar:
	//visible
	//has_resize_grip
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_statusbar_new ();
	gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (mae), TRUE);//has_resize_grip
	gtk_widget_set_name (mae, "statusbarStatus");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "statusbarStatus", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_notebookPrincipal (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkNotebook:
	//visible
	//can_focus
	//show_tabs
	//show_border
	//tab_pos
	//scrollable
	//enable_popup
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_notebook_new ();
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (mae), TRUE);//scrollable
	gtk_notebook_set_show_tabs (GTK_NOTEBOOK (mae), TRUE);//show_tabs
	gtk_notebook_set_show_border (GTK_NOTEBOOK (mae), TRUE);//show_border
	gtk_notebook_popup_enable (GTK_NOTEBOOK (mae));//enable_popup
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (mae), GTK_POS_TOP);//tab_pos
	gtk_widget_set_name (mae, "notebookPrincipal");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "notebookPrincipal", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowPrincipal_fixed2 (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	gtk_notebook_set_tab_label_packing (GTK_NOTEBOOK (mae), filha, FALSE, TRUE, GTK_PACK_START);//tab_expand e tab_fill
	filha = be_Interface_criar_windowPrincipal_label46 (janela, accel, tip, Data, NULL);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (mae), gtk_notebook_get_nth_page (GTK_NOTEBOOK (mae), -1), filha);//label
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_fixed2 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkFixed:
	//visible
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_fixed_new ();
	gtk_widget_set_name (mae, "fixed2");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "fixed2", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowPrincipal_label46 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
		mae = gtk_label_new ("Exemplo");//label em UTF8
	gtk_label_set_use_underline (GTK_LABEL (mae), FALSE);//use_underline
	gtk_label_set_use_markup (GTK_LABEL (mae), FALSE);//use_markup
	gtk_label_set_justify (GTK_LABEL (mae), GTK_JUSTIFY_LEFT);//justify
	gtk_label_set_line_wrap (GTK_LABEL (mae), FALSE);//wrap
	gtk_label_set_selectable (GTK_LABEL (mae), FALSE);//selectable
	gtk_misc_set_alignment (GTK_MISC (mae), 0.5, 0.5);//xalign e yalign
	gtk_misc_set_padding (GTK_MISC (mae), 0, 0);//xpad e ypad
	gtk_widget_set_name (mae, "label46");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "label46", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowPrincipal_ajustes_finais (GtkWidget *janela)
{
}
