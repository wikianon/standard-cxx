#include "beCalendario.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowCalendario.c"

GDate *be_Calendario (GtkWindow *winPai, const GDate *Data)
{
	_BECalendario_Janela Janela;

	//janela
	Janela.window = be_Interface_criar_windowCalendario (winPai, NULL, NULL, &Janela);
	Janela.calendar = BE_interfaces_widget (Janela.window, "calendarCalendario");
	Janela.Data = NULL;

	//foco inicial no calendário
	gtk_widget_grab_focus (Janela.calendar);
	//selecionar data
	if (Data && g_date_valid (Data))
	{
		gtk_calendar_select_month (GTK_CALENDAR (Janela.calendar), g_date_get_month (Data) - 1, g_date_get_year (Data));
		gtk_calendar_select_day (GTK_CALENDAR (Janela.calendar), g_date_get_day (Data));
	}

	gtk_main ();

	return Janela.Data;
}

void _be_Interface_sinal_windowCalendario_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
	_BECalendario_Janela *Janela = NULL;
	guint nAno = 0, nMes = 0, nDia = 0;

	Janela = (_BECalendario_Janela *) user_data;

	gtk_calendar_get_date (GTK_CALENDAR (Janela->calendar), &nAno, &nMes, &nDia);
	Janela->Data = g_date_new ();
	g_date_set_dmy (Janela->Data, nDia, nMes + 1, nAno);

	gtk_widget_destroy (Janela->window);//fechar janela
}
