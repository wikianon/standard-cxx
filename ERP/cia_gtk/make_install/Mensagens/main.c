#include "beMensagens.h"

static void testes (GtkWidget *widget, gpointer data)
{
	gchar *sTemp = NULL;

	if (be_Mensagens (GTK_WINDOW (data), FALSE, "<b>Testando. </b>\n<i>Tá muito fácil ?.</i>", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) == GTK_RESPONSE_YES)
		g_print ("Sim\n");
	else
		g_print ("Nao\n");

	if (be_Mensagens_Ext (GTK_WINDOW (data), FALSE, "Testando", "<b>Testando. </b>\n<i>Tá muito fácil ?. KJHkjh aksja akjh  kaksajd  kaksj \nkkaja skkjhsd kajsd  kajsd\nkajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjha sja akjh  kaksajd  kaksj kkaja skkjhsd kajsd  \nkajsd  kajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjh skdjkjaskdjhkajsdk  kjashd asd \n44456 sdfsd6 6546sdf s6df5 4 46s5d4f4 sdf sdf7879sdf sdf \nKJHkjh aksja akjh  kaksajd  kaksj \nkkaja skkjhsd kajsd  kajsd\nkajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjha sja akjh  kaksajd  kaksj kkaja skkjhsd kajsd  \nkajsd  kajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjh skdjkjaskdjhkajsdk  kjashd asd \n44456 sdfsd6 6546sdf s6df5 4 46s5d4f4 sdf sdf7879sdf sdf \nKJHkjh aksja akjh  kaksajd  kaksj \nkkaja skkjhsd kajsd  kajsd\nkajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjha sja akjh  kaksajd  kaksj kkaja skkjhsd kajsd  \nkajsd  kajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjh skdjkjaskdjhkajsdk  kjashd asd \n44456 sdfsd6 6546sdf s6df5 4 46s5d4f4 sdf sdf7879sdf sdf \nKJHkjh aksja akjh  kaksajd  kaksj \nkkaja skkjhsd kajsd  kajsd\nkajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjha sja akjh  kaksajd  kaksj kkaja skkjhsd kajsd  \nkajsd  kajsd ask kajsd akjhsdkajsh kajsd kjaksj kkjh skdjkjaskdjhkajsdk  kjashd asd \n44456 sdfsd6 6546sdf s6df5 4 46s5d4f4 sdf sdf7879sdf sdf \n++++++++++</i>", GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) == GTK_RESPONSE_YES)
		g_print ("Sim\n");
	else
		g_print ("Nao\n");

	sTemp = be_Mensagens_in (GTK_WINDOW (data), FALSE, "Testando", "<b>Testando.</b>\n<i>Digite alguma coisa.</i>", "Não vai logo");
	//be_Mensagens (GTK_WINDOW (data), TRUE, "Erro de conex: Cant connect to MySQL server on 'localhost'", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);

	printf ("%s\n", sTemp);

	BE_free (sTemp);
}

int main(int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);

	GtkWidget *window = NULL;
	GtkWidget *button = NULL;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	button = gtk_button_new_with_label ("Testes");
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (testes), GTK_WINDOW (window));
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (window), TRUE);
	gtk_widget_show_all (window);

	testes (NULL, GTK_WINDOW (window));

	gtk_main ();

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}

