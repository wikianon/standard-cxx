#include "bexml.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extendida
	be_global_abrir (&argc, &argv, 0);

	BEXML *XML = NULL;
	GNode *No = NULL;
	char space[256];
	glong lCont = 0;
	gchar *sString = NULL, *sTemp = NULL;

	//ler XML
	printf ("STRING\n");
	sTemp = BE_ISO_UTF8 ("<doc>Teste <b>sei lá</b>. Abra sua <font face='arial' size='12'>mente (<b>agora</b>)</font>. Formadas por &#060;&#174;&#062;.<b>Eu denovo.47</b>d<novo>h sssdkksd</novo>%s</doc>");
	//sString = g_markup_printf_escaped ("%s%s", sTemp,"<>");
	sString = g_strdup (sTemp);
	BE_free (sTemp);
	XML = be_xml_abrir (sString, FALSE);
	if (XML)
	{
		printf ("Qt: %d\n", (gint) be_xml_quantidade (XML));
		//loop em todos
		for (lCont = 0; lCont < be_xml_quantidade (XML); lCont++)
		{
			be_xml_mover (XML, lCont);

			memset (&space, '\0', 255);
			if (be_xml_nivel (XML) > 0)
				memset (&space, '-', be_xml_nivel (XML) * 4);
			printf ("%d %s(%s) %s %d [%d]\n", (gint) be_xml_posicao (XML), space, be_xml_id (XML), be_xml_valor (XML), (gint) be_xml_tipo (XML), be_xml_atributos_quantidade (XML));
		}
		//localização direta
		printf ("Localização:\n");
		be_xml_mover (XML, 9);//apartir deste ponto
		if (be_xml_mover_id (XML, ".doc.font.") >= 0)
		{
			memset (&space, '\0', 255);
			if (be_xml_nivel (XML) > 0)
				memset (&space, '-', be_xml_nivel (XML) * 4);
			printf ("%d %s(%s) %s %d [%d]\n", (gint) be_xml_posicao (XML), space, be_xml_id (XML), be_xml_valor (XML), (gint) be_xml_tipo (XML), be_xml_atributos_quantidade (XML));
		}
	}
	BE_xml_fechar (XML);
	BE_free (sString);

	XML = be_xml_abrir ("beInterfaces.glade", TRUE);
	if (XML)
	{
		printf ("\nQt: %d\n\n", (gint) be_xml_quantidade (XML));
		//loop em todos
		for (lCont = 161; lCont < be_xml_quantidade (XML); lCont++)
		{
			/*be_xml_mover (XML, lCont);

			memset (&space, '\0', 255);
			if (be_xml_nivel (XML) > 0)
				memset (&space, '-', be_xml_nivel (XML) * 4);
			printf ("%d %s(%s) %s %d [%d]\n", (gint) be_xml_posicao (XML), space, be_xml_id (XML), be_xml_valor (XML), (gint) be_xml_tipo (XML), be_xml_atributos_quantidade (XML));
			//atributos
			for (nCont = 0; nCont < be_xml_atributos_quantidade (XML); nCont++)
				printf ("%d %s\t%d [%s = %s]\n", (gint) be_xml_posicao (XML), space, (gint) nCont, be_xml_atributos_nomes (XML, nCont), be_xml_atributos_valores (XML, nCont));*/
			be_xml_mover (XML, lCont);
			printf ("%d %d %d\n", (gint) lCont, (gint) be_xml_posicao (XML), (gint) be_xml_quantidade (XML));
		}
		printf ("\n");
	}
	BE_xml_fechar (XML);


	//ler arquivo
	/*printf ("\nARQUIVO\n");
	//XML = be_xml_abrir ("Modelo.svg", TRUE);
	//XML = be_xml_abrir ("Modelo.html", TRUE);
	XML = be_xml_abrir ("beInterfaces.glade", TRUE);
	if (XML)
	{
		printf ("Qt: %d\n", (gint) be_xml_quantidade (XML));
		//loop em todos
		for (lCont = 0; lCont < be_xml_quantidade (XML); lCont++)
		{
			be_xml_mover (XML, lCont);

			memset (&space, '\0', 255);
			if (be_xml_nivel (XML) > 0)
				memset (&space, '-', be_xml_nivel (XML) * 4);
			printf ("%d %s(%s) %s %d [%d]\n", (gint) be_xml_posicao (XML), space, be_xml_id (XML), be_xml_valor (XML), (gint) be_xml_tipo (XML), be_xml_atributos_quantidade (XML));
			//atributos
			for (nCont = 0; nCont < be_xml_atributos_quantidade (XML); nCont++)
				printf ("%d %s\t%d [%s = %s]\n", (gint) be_xml_posicao (XML), space, (gint) nCont, be_xml_atributos_nomes (XML, nCont), be_xml_atributos_valores (XML, nCont));
		}
		//localização direta
		printf ("Localização:\n");
		be_xml_mover (XML, 100);//apartir deste ponto
		if (be_xml_mover_id (XML, ".svg.g.text.") >= 0)
		{
			memset (&space, '\0', 255);
			if (be_xml_nivel (XML) > 0)
				memset (&space, '-', be_xml_nivel (XML) * 4);
			printf ("%d %s(%s) %s %d [%d]\n", (gint) be_xml_posicao (XML), space, be_xml_id (XML), be_xml_valor (XML), (gint) be_xml_tipo (XML), be_xml_atributos_quantidade (XML));
			//localizar atributos
			printf ("Localização de propriedades:\n");
			printf ("\t%d\n", be_xml_atributos_localizar (XML, "id"));
		}
	}
	BE_xml_fechar (XML);

	//carregar árvore
	printf ("\nÁRVORE\n");*/

	sString = BE_ISO_UTF8 ("<doc>Testes<b>sei lá</b>. Abra sua <font face='arial' size='12'>mente (<b>agora</b>)</font>. Formadas por &#060;&#174;&#062;.<b>Eu denovo.</b><novo></novo>%s</doc>");
	XML = be_xml_abrir (sString, FALSE);
	//XML = be_xml_abrir ("Modelo.xml", TRUE);
	if (XML)
	{
		No = be_xml_no_carregar (XML, FALSE);
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);
		BE_xml_no_descarregar (No);
	}
	BE_xml_fechar (XML);
	BE_free (sString);


	printf ("%s\n", g_markup_printf_escaped ("%s", BE_ISO_UTF8("JOSÉ MARIA BARROCA, Nº 405, R. - <>SUMARÉ")));

	//finalizar biblioteca extendida
	be_global_fechar ();
	return 0;
}
