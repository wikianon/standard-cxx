/*
+------------------------------------------------------------------------------+
| ARQUIVO GERADO AUTOMATICAMENTE POR be_Interfaces                             |
+------------------------------------------------------------------------------+
*/
#include "interface_windowCalendario.h"

GtkWidget *be_Interface_criar_windowCalendario (GtkWindow *winpai, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data)
{
	GtkWidget *mae = NULL, *filha = NULL;

	mae = gtk_window_new (GTK_WINDOW_TOPLEVEL);//type
	gtk_window_set_title (GTK_WINDOW (mae), "CanledÃ¡rio");//title
	gtk_window_set_position (GTK_WINDOW (mae), GTK_WIN_POS_CENTER);//window_position
	gtk_window_set_modal (GTK_WINDOW (mae), TRUE);//modal
	gtk_window_set_resizable (GTK_WINDOW (mae), TRUE);//resizable
	gtk_window_set_destroy_with_parent (GTK_WINDOW (mae), TRUE);//destroy_with_parent
	gtk_window_set_icon_from_file (GTK_WINDOW (mae), "escri129.png", NULL);//icon
	gtk_window_set_decorated (GTK_WINDOW (mae), TRUE);//decorated
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (mae), FALSE);//skip_taskbar_hint
	gtk_window_set_type_hint (GTK_WINDOW (mae), GDK_WINDOW_TYPE_HINT_DIALOG);//type_hint
	gtk_window_set_gravity (GTK_WINDOW (mae), GDK_GRAVITY_CENTER);//gravity
	gtk_window_set_focus_on_map (GTK_WINDOW (mae), TRUE);//focus_on_map
	gtk_window_set_urgency_hint (GTK_WINDOW (mae), FALSE);//urgency_hint
	gtk_widget_set_name (mae, "windowCalendario");
	//sinais
	g_signal_connect (G_OBJECT (mae), "destroy", G_CALLBACK (gtk_main_quit), Data);
	//teclas de atalho
	if (!accel)
		accel = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (mae), accel);
	if (!tip)
		tip = gtk_tooltips_new ();
	//filhas
	filha = be_Interface_criar_windowCalendario_table8 (mae, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;
	//filha da principal externa
	if (winpai)
		gtk_window_set_transient_for (GTK_WINDOW (mae), winpai);
	gtk_widget_show (mae);//visible
	//ajustes final
	_be_Interfaces_windowCalendario_ajustes_finais (mae);

	return mae;
}

GtkWidget *be_Interface_criar_windowCalendario_table8 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "table8");
	gtk_widget_show (mae);//visible
	gtk_container_set_border_width (GTK_CONTAINER (mae), 4);//border_width
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "table8", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowCalendario_hbuttonbox4 (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 1, 2, 0, GTK_FILL, 0, 0);
	filha = be_Interface_criar_windowCalendario_calendarCalendario (janela, accel, tip, Data, NULL);
	gtk_table_attach (GTK_TABLE (mae), filha, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowCalendario_hbuttonbox4 (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	gtk_widget_set_name (mae, "hbuttonbox4");
	gtk_widget_show (mae);//visible
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "hbuttonbox4", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = be_Interface_criar_windowCalendario_buttonOK (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = be_Interface_criar_windowCalendario_buttonCancelar (janela, accel, tip, Data, NULL);
	gtk_container_add (GTK_CONTAINER (mae), filha);
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowCalendario_buttonOK (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
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
	//-> clicked
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-ok");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-ok");//label
	gtk_button_set_focus_on_click (GTK_BUTTON (mae), TRUE);//focus_on_click
	gtk_button_set_relief (GTK_BUTTON (mae), GTK_RELIEF_NORMAL);//relief
	gtk_button_set_alignment (GTK_BUTTON (mae), 0.5, 0.5);//xalign e yalign
	gtk_widget_set_name (mae, "buttonOK");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_DEFAULT);//can_default
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	g_signal_connect (G_OBJECT (mae), "clicked", G_CALLBACK (_be_Interface_sinal_windowCalendario_on_buttonOK_clicked), Data);
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "buttonOK", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowCalendario_buttonCancelar (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkButton:
	//visible
	//can_focus
	//label
	//use_stock
	//relief
	//focus_on_click
	//-> clicked
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_button_new_from_stock ("gtk-cancel");//label
	gtk_button_set_use_stock (GTK_BUTTON (mae), TRUE);//use_stock
	gtk_button_set_label (GTK_BUTTON (mae), "gtk-cancel");//label
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
	filha = NULL;

	return mae;
}

GtkWidget *be_Interface_criar_windowCalendario_calendarCalendario (GtkWidget *janela, GtkAccelGroup *accel, GtkTooltips *tip, gpointer Data, GtkWidget *mae)
{
	//GtkCalendar:
	//visible
	//can_focus
	//display_options
	GtkWidget *filha = NULL;

	if (!mae)
		mae = gtk_calendar_new ();
	gtk_calendar_set_display_options (GTK_CALENDAR (mae), GTK_CALENDAR_SHOW_HEADING|GTK_CALENDAR_SHOW_DAY_NAMES|GTK_CALENDAR_SHOW_WEEK_NUMBERS);//display_options
	gtk_widget_set_name (mae, "calendarCalendario");
	gtk_widget_show (mae);//visible
	GTK_WIDGET_SET_FLAGS (mae, GTK_CAN_FOCUS);//can_focus
	//sinais
	//teclas de atalho
	//referência
	if (janela)
		g_object_set_data_full (G_OBJECT (janela), "calendarCalendario", gtk_widget_ref (mae), (GDestroyNotify) gtk_widget_unref);
	//filhas
	filha = NULL;//sem filhas

	return mae;
}

void _be_Interfaces_windowCalendario_ajustes_finais (GtkWidget *janela)
{
	gtk_widget_grab_default (BE_interfaces_widget (janela, "buttonOK"));//has_default
}
