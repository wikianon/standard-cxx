#include "bePropriedades.h"

static void testes (GtkWidget *widget, gpointer data)
{
	GSList *Propriedades = NULL;
	gchar *sRotulo = NULL;
	gchar *sTexto1 = NULL, *sTexto2 = NULL;//textos
	gchar *sNumero1 = NULL;//números
	gchar *sLogico = NULL;
	gchar *sCor1 = NULL, *sCor2 = NULL;//cores
	gchar *sFonte1 = NULL, *sFonte2 = NULL;//fontes
	gchar *sArquivo1 = NULL, *sArquivo2 = NULL;//selecionar arquivo
	gchar *sDir1 = NULL, *sDir2 = NULL;//selecionar diretório
	gchar *sLista1 = NULL, *sLista2 = NULL;//listas
	GSList *Lista1 = NULL, *Lista2 = NULL;//listas

	//texto 1
	sTexto1 = g_strdup ("Testando texto 1...");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_TEXTO, "Texto 1", &sTexto1, NULL);
	//rótulo 1
	sRotulo = g_strdup ("<b>Testando o rótulo:</b>\nTudo OK");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ROTULO, "Informações", &sRotulo, NULL);
	//texto 2
	sTexto2 = g_strdup ("Testando texto 2...");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_TEXTO, "Texto 2", &sTexto2, NULL);
	//nenhum
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_NENHUM, NULL, NULL, NULL);
	//número 1
	sNumero1 = g_strdup_printf("%.2f", 50.5);//somente floats
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_NUMERO, "Numero 1", &sNumero1, NULL);
	//lógico
	sLogico = g_strdup ("1");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LOGICO, "Lógico 1", &sLogico, NULL);
	//cor 1
	sCor1 = g_strdup ("#0000FF");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_COR, "Cor 1", &sCor1, NULL);
	//cor 2
	sCor2 = g_strdup ("#FF0000");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_COR, "Cor 2", &sCor2, NULL);
	//fonte 1
	sFonte1 = g_strdup ("Verdana 15");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_FONTE, "Fonte 1", &sFonte1, NULL);
	//fonte 2
	sFonte2 = g_strdup ("Arial 8");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_FONTE, "Fonte 2", &sFonte2, NULL);
	//arquivo 1
	sArquivo1 = g_strdup ("/home/kurumin/Sei lá/imagém.jpeg");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO, "Arquivo 1", &sArquivo1, NULL);
	//arquivo 2
	sArquivo2 = NULL;
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_ARQUIVO_LIVRE, "Arquivo 2", &sArquivo2, NULL);
	//dir 1
	sDir1 = g_strdup ("/home/kurumin/Meus Documentos/Vídeos");
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_DIRETORIO, "Diretorio 1", &sDir1, NULL);
	//dir 2
	sDir2 = NULL;
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_DIRETORIO, "Diretorio 1", &sDir2, NULL);
	//lista 1
	Lista1 = g_slist_append (Lista1, "Luciano Trindade Pereira");
	Lista1 = g_slist_append (Lista1, "Pereira");
	Lista1 = g_slist_append (Lista1, "Trindadeé");
	Lista1 = g_slist_append (Lista1, "Sei la");
	Lista1 = g_slist_append (Lista1, " Sei lá");
	Lista1 = g_slist_append (Lista1, "Testando a lista");
	sLista1 = g_strdup_printf("%d", 2);//somente inteiros
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LISTA, "Lista 1", &sLista1, Lista1);
	//lista 2
	Lista2 = g_slist_append (Lista2, "Botas1");
	Lista2 = g_slist_append (Lista2, "Botas");
	Lista2 = g_slist_append (Lista2, "Botas0");
	Lista2 = g_slist_append (Lista2, "Lista padrao");
	Lista2 = g_slist_append (Lista2, "Execucao");
	sLista2 = g_strdup_printf("%d", 3);//somente inteiros
	Propriedades = be_Propriedades_adicionar (Propriedades, FALSE, BEPROPRIEDADES_LISTA, "Lista 2", &sLista2, Lista2);

	//abrir janela
	if (be_Propriedades (GTK_WINDOW (data), Propriedades))
	{
		printf ("Texto 1: %s\n", sTexto1);
		printf ("Texto 2: %s\n", sTexto2);
		printf ("Numero: %.2f\n", g_strtod (sNumero1, NULL));
		printf ("Logico: %d\n", (int) g_strtod (sLogico, NULL));
		printf ("Cor 1: %s\n", sCor1);
		printf ("Cor 2: %s\n", sCor2);
		printf ("Fonte 1: %s\n", sFonte1);
		printf ("Fonte 2: %s\n", sFonte2);
		printf ("Arquivo 1: %s\n", sArquivo1);
		printf ("Arquivo 2: %s\n", sArquivo2);
		printf ("Dir 1: %s\n", sDir1);
		printf ("Dir 2: %s\n", sDir2);
		printf ("Lista 1: %d - %s\n", atoi (sLista1), (gchar *) g_slist_nth_data (Lista1, atoi (sLista1)));
		printf ("Lista 2: %d - %s\n", atoi (sLista2), (gchar *) g_slist_nth_data (Lista2, atoi (sLista2)));
	}
	else
		printf ("(null)\n");

	printf ("Logico: %s\n", sLogico);

	BE_free (sNumero1);
	BE_free (sLogico);
	BE_free (sDir1);
	BE_free (sDir2);
	BE_free (sArquivo1);
	BE_free (sArquivo2);
	BE_free (sFonte1);
	BE_free (sFonte2);
	BE_free (sCor1);
	BE_free (sCor2);
	BE_free (sRotulo);
	BE_free (sTexto1);
	BE_free (sTexto2);
	BE_free (sLista1);
	BE_free (sLista2);
	BE_slist_free (Lista1);
	BE_slist_free (Lista2);
	BE_slist_free (Propriedades);
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
	gtk_widget_show_all (window);

	testes (NULL, GTK_WINDOW (window));

	gtk_main ();

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
