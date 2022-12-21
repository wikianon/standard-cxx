#include "beglobal.h"

//testes com threads
unsigned long lThread1 = 0, lThread2 = 0, lThread3 = 0;

gpointer thread_func (gpointer data)
{
	unsigned long *lCont = NULL;

	lCont = (unsigned long *) data;

	for (*lCont = 0; *lCont < (LONG_MAX - 2); (*lCont)++);

	//return NULL;
	g_thread_exit (NULL);
	return NULL;
}

int main (int argc, char *argv[])
{
	// Arquivo somente para testes das funções
	//if (!g_thread_supported ())
	//	g_thread_init (NULL);

	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);

	/*
	TESTES GLOBAIS
	*/
	//GThread *Thread1 = NULL, *Thread2 = NULL, *Thread3 = NULL;
	int nCont;
	char *p = NULL, *d = NULL;
	float *f = NULL;

	//verificar memória
	char *s = NULL;
	s = (char *) malloc (10);

	/*for (nCont = 0; nCont < 10; nCont++)
	    s[nCont] = 'A';

	printf ("chunk_size = %d %d %d %d %d\n", s[-4], s[-3], s[-2], s[-1], s[0]);
	printf ("chunk_size = %d %d %d %d\n", s[-8], s[-7], s[-6], s[-5]);*/


	//memcpy (&H, s - sizeof (H), sizeof (H));
	//printf ("%d\n", H._size);
	free (s);

    //be_global_fechar ();
    //return 0;

	//GString *s = NULL;

	d = (char *) malloc (4 * sizeof (char));
	p = (char *) malloc (8 * sizeof (char));

	g_free (p);

	for (nCont = 0; nCont < 5; nCont++)
		f = g_renew (gfloat, f, 2);
	//g_free (f);
	//g_free (f);

	free (d);
	//p = g_strdup ("Testes\n");
	//l = (gulong) p;

	//printf ("%d %d %d\n", l, (l-1), (l-2));
	//free (p);
	//printf ("%d %d %d\n", l, (l-1), (l-2));
	//finalizar biblioteca extentida

	//printf ("%d\n", g_trap_free_size);

	//g_mem_profile ();
	//printf ("%s", NULL);

	GSList *Lista = NULL, *ListaL = NULL;
	gdouble inicio, fim;
	gchar *sTemp = NULL;
	gchar string[256];


	g_stpcpy (string,"Luciano Trindade Pereira");
	g_strreverse (string);
	printf ("%s\n", string);

	g_stpcpy (string,"  Luciano Trindade Pereira  ");
	printf ("|%s|\n", g_strchug (string));

	g_stpcpy (string,"  Luciano Trindade Pereira  ");
	printf ("|%s|\n", g_strchomp (string));

	g_stpcpy (string,"  Luciano Trindade Pereira  ");
	printf ("|%s|\n", g_strstrip (string));

	//printf ("%s\n", g_strconcat ("Luc", "aaas", NULL));

	g_stpcpy (string,"Luciano Trindade Pereira");
	printf ("%s\n", g_strcanon (string, "abcdefghijklmnopqrstuvwabc0123456789", '@'));

	g_stpcpy (string,"Luciano Trindade Pereira");
	printf ("%s\n", g_strdelimit (string, "a", '@'));

	g_stpcpy (string, "Luciano Trindade Pereira");
	printf ("%s\n", sTemp = be_strreplace (string, "i", "000"));
	BE_free (sTemp);
	printf ("%s\n", sTemp = be_strreplace_pos (string, "i", "000", 7));
	BE_free (sTemp);

	printf ("%s\n", sTemp = be_strrepeat ("Luc ", 3));
	BE_free (sTemp);

	g_stpcpy (string,"Luciano Trindade Pereira");
	printf ("%d\n", be_strcount (string, "a"));

	printf ("%s\n", sTemp = g_strdup_printf ("%.2f %s", 57.78, "asas"));//Números para strings
	BE_free (sTemp);

	g_stpcpy (string, "Luçíáno Tríndàde Pêreírã");
	printf ("|%s|\n", sTemp = be_strclear (string, BEGLOBAL_CONVERSAO_SEM_ACENTOS));
	BE_free (sTemp);

	g_stpcpy (string, " -12006.87 ");
	printf ("%.2f\n", g_ascii_strtod (string, NULL));

	printf ("%o\n", 255);

	g_stpcpy (string, "FFFFff");
	printf ("%d\n", (int) be_cast_hex_long (string));
	printf ("R %d\n", be_rgb_vermelho (be_cast_hex_long (string)));

	g_stpcpy (string, "FFFFFF");
	printf ("%d\n", (int) be_cast_hex_long (string));
	printf ("G %d\n", be_rgb_verde (be_cast_hex_long (string)));

	g_stpcpy (string, "FFFFFF");
	printf ("%d\n", (int) be_cast_hex_long (string));
	printf ("B %d\n", be_rgb_azul (be_cast_hex_long (string)));

	printf ("%s\n", sTemp = be_substr ("Luciano Trindade Pereira", 1, -1));
	BE_free (sTemp);

	printf ("%d\n", be_strpos ("Luciano Trindade Pereira", "i", 4));
	printf ("%d\n", be_strrpos ("iLuciano Trindade Pereira", "i", 0));
	printf ("%d\n", be_strpos_ascii ("Luciano Trindade Pereira", "I", 4));

	printf ("Extrair |%s|\n", sTemp = be_strextract ("asas asas asas asas Luciano Trindade Pereira", " ", " ", 26));
	BE_free (sTemp);
	printf ("Extrair Ascii |%s|\n", sTemp = be_strextract_ascii ("Luciano Trindade Pereira", "t", "IN", 8));
	BE_free (sTemp);

	sTemp = g_strdup ("Luciano Trindade Pereira");
	Lista = be_strsplit (&sTemp," ");
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
		printf ("|%s|\n", (gchar *) ListaL->data);
	BE_free (sTemp);
	BE_slist_free (Lista);

	inicio = be_tempo_usec ();
	for (ListaL = Lista; ListaL; ListaL = g_slist_next (ListaL))
	{
		sTemp = (gchar *) ListaL->data;
	}
	fim = be_tempo_usec ();

	printf ("%.3f seg\n", (fim - inicio));

	printf ("Aqui %d - %d\n", be_tempo_sec (), be_tempo_usec ());

	printf ("OK 3\n");
	printf ("\n\n");

	GDate *pDate = NULL;
	gchar sBuffer[256];

	pDate = be_tempo_GDate ();
	g_date_strftime (sBuffer, 256, "%d/%m/%Y", pDate);
	printf ("%s\n", sBuffer);
	g_date_free (pDate);

	printf ("Horário local = %s\n", sTemp = BE_tempo ("%d/%m/%Y %H:%M:%S"));
	BE_free (sTemp);
	printf ("Horário gmt = %s\n", sTemp = BE_tempo_gmt ("%d/%m/%Y %H:%M:%S"));
	BE_free (sTemp);
	printf ("Fuso horário = %d\n", be_tempo_fuso ());

	printf ("\n\n");

	printf ("%s\n", sTemp = be_arquivo_extensao ("/home/testest.1xt"));
	BE_free (sTemp);
	printf ("%s %s\n", sTemp = be_strclear ("prt1050", BEGLOBAL_CONVERSAO_LETRAS), be_strclear ("prt1050", BEGLOBAL_CONVERSAO_DIGITOS));
	BE_free (sTemp);
	printf ("%d\n", atoi ("444 - 44 asiasias"));


	gfloat L = 50.0, A = 100.0;
	be_redimensionar_proporcional (&L, &A, 5.0, 5.0, FALSE);
	printf ("%f %f\n", L, A);

	/*STARTUPINFO si;
    PROCESS_INFORMATION y;
	ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&p, sizeof(y));
	if( !CreateProcess( NULL,   // Sem módulo
        "C:WINDOWS\\notepad.exe", // linha de comando para o executável
        NULL,
        NULL,
        FALSE,            // O processo criado não herdará os handles
        0,
        NULL,
        NULL,
        &si,              // Ponteiro para STARTUPINFO
        &y )             // Ponteiro para PROCESS_INFORMATION
    )
    {
        printf( "Erro em CreateProcess: 0x%X", GetLastError() );
        return 0;
    }
	CloseHandle(y.hProcess);
    CloseHandle(y.hThread);*/


	//local usuário
	printf ("%s\n%s\n", be_diretorio_local (TRUE), be_diretorio_local_temp (TRUE));

	printf ("%s\n", be_arquivo_absoluto ("seila.bat"));

	//html
	printf ("%s\n", sTemp = be_urlencode ("\"Aardvarks lurk, OK?\""));
	BE_free (sTemp);
	printf ("%s\n", sTemp = be_urldecode ("%22Aardvarks+lurk%2C+OK%3F%22"));
	BE_free (sTemp);

	printf ("%s\n", sTemp = be_arquivo_nome_sugerido ("luciano.", 50, TRUE));
	BE_free (sTemp);
	printf ("%s\n", sTemp = be_arquivo_nome_sugerido ("luciano.txt", 50, TRUE));
	BE_free (sTemp);

	//testes sem threads
	/*printf ("Testes sem thread: Inicio\n");
	inicio = be_tempo_sec ();
	thread_func (&lThread1);//loop longo
	thread_func (&lThread2);//loop longo
	thread_func (&lThread3);//loop longo
	fim = be_tempo_sec (); printf ("Testes sem thread: Liberado = %.3f seg\n", (fim - inicio));
	fim = be_tempo_sec (); printf ("Testes sem thread: Fim = %.3f seg\n", (fim - inicio));
	//testes com threads
	printf ("Testes com thread: Inicio\n");
	inicio = be_tempo_sec ();
	Thread1 = g_thread_create (thread_func, &lThread1, TRUE, NULL);
	Thread2 = g_thread_create (thread_func, &lThread2, TRUE, NULL);
	Thread3 = g_thread_create (thread_func, &lThread3, TRUE, NULL);
	fim = be_tempo_sec (); printf ("Testes com thread: Liberado = %.3f seg\n", (fim - inicio));
	g_thread_join (Thread1);
	g_thread_join (Thread2);
	g_thread_join (Thread3);
	fim = be_tempo_sec (); printf ("Testes com thread: Fim = %.3f seg\n", (fim - inicio));*/

	//mais testes com threads
	/*Thread1 = g_thread_create (thread_func, &lThread1, FALSE, NULL);
	Thread2 = g_thread_create (thread_func, &lThread2, FALSE, NULL);
	Thread3 = g_thread_create (thread_func, &lThread3, FALSE, NULL);
	nCont = 0;
	do
	{
		g_usleep (400);
		if (lThread1 > lThread2 && lThread1 > lThread3)
			printf ("%lu\t \t \n", lThread1);
		else if (lThread2 > lThread1 && lThread2 > lThread3)
			printf (" \t%lu\t \n", lThread2);
		else if (lThread3 > lThread1 && lThread3 > lThread2)
			printf (" \t \t%lu\n", lThread3);
		//printf ("%lu\t%lu\t%lu\n", lThread1, lThread2, lThread3);
		//forçar a suspensão da thread atual, e executar a próxima
		g_thread_yield ();
	}
	while (lThread1 <= (LONG_MAX - 2) && lThread2 <= (LONG_MAX - 2) && lThread3 <= (LONG_MAX - 2));*/

	//testes python integrado
	printf ("Testes python\n");
	//PyRun_SimpleString("A = 10+5");
	//PyRun_SimpleString("print A");		
	PyObject *m = NULL, *r = NULL, *a = NULL, *v = NULL;
	m = PyImport_AddModule ("__main__");
	if (m)
	{
		m = PyModule_GetDict (m);
		//v = PyRun_String ("resultado = 10+5", Py_single_input, m, m);
		//v = PyRun_String ("resultado1 = 1 + resultado", Py_single_input, m, m);
		v = PyRun_String ("if 'Nao'[0] == 'N':\n\tresultado1 = False\nelse:\n\tresultado1 = True", Py_single_input, m, m);
		a = PyDict_GetItemString (m, "resultado1");
		r = PyObject_Str (a);
	}
	printf ("Resultado %s\n", PyString_AsString (r));
	
	//html decode
	printf ("Cota&#231;oes = %s\n", be_htmldecode ("Cota&#231;oes"));

	be_global_fechar ();
	return 0;
}
