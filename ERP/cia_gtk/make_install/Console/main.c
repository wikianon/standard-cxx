#include "beConsole.h"

int main (int argc, char *argv[])
{
	//inicializar biblioteca estentida
	be_global_abrir (&argc, &argv, 0);

	BESQL *SQL = NULL;
	gchar *sErros = NULL;
	GtkWidget *window = NULL;

	SQL = BE_sql_abrir_conexao ("root", "prodpp", NULL, 0, -1, &sErros);
	//SQL = BE_sql_abrir_conexao ("root", NULL, NULL, 0, -1, &sErros);

	if (SQL)
	{
		//be_Console (NULL, SQL);
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (window), "Console");
		gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
		gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
		gtk_window_maximize (GTK_WINDOW (window));
		g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
		//anexar na janela
		gtk_container_add (GTK_CONTAINER (window), be_Console (window, SQL));
		gtk_widget_show (window);//não utilizar show all
	}
	gtk_main ();

	if (sErros)
		g_print ("%s\n", sErros);
	BE_free (sErros);

	//fechar conexão
	BE_sql_fechar_conexao (SQL);

	//finalizar biblioteca estentida
	be_global_fechar ();
	return 0;
}
