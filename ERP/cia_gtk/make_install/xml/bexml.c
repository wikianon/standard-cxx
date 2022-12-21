#include "bexml.h"

BEXML *be_xml_abrir (const gchar *sExpressao, gboolean bArquivo)
{
	BEXML *XML = NULL;
	xmlTextReaderPtr reader;

	//abrir XML
	if (bArquivo)
		reader = xmlReaderForFile (sExpressao, NULL, 0);//arquivo
	else
		reader = xmlReaderForDoc ((guchar *) sExpressao, NULL, NULL, 0);//string
	if (reader)
	{
		XML = g_new (BEXML, 1);
		XML->sExpressao = g_strdup (sExpressao);
		XML->sID = NULL;
		XML->bArquivo = bArquivo;
		XML->reader = reader;
		XML->lPosicao = 0;
		XML->lQuantidade = 0;
		XML->Atributos = NULL;
		XML->nQuantidadeAtributos = 0;
	}

	return XML;
}

void _be_xml_fechar (BEXML *XML)
{
	GSList *ListaL = NULL;
	_BEXML_Atributo *Atributo = NULL;

	if (XML)
	{
		if (XML->Atributos)
		{
			for (ListaL = XML->Atributos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Atributo = (_BEXML_Atributo *) ListaL->data;
				BE_free (Atributo->sNome);
				BE_free (Atributo->sValor);
				BE_free (Atributo);
			}
			BE_slist_free (XML->Atributos);
		}
		if (XML->reader)
			xmlFreeTextReader (XML->reader);
		if (XML->sExpressao)
			BE_free (XML->sExpressao);
		if (XML->sID)
			BE_free (XML->sID);
		BE_free (XML);
	}
	xmlCleanupParser ();
	xmlMemoryDump ();
}

void be_xml_mover (BEXML *XML, const glong lPosicao)
{
	GSList *ListaL = NULL;
	_BEXML_Atributo *Atributo = NULL;
	gchar *sElemento = NULL;
	gchar *sTemp = NULL;

	if (lPosicao < XML->lPosicao || XML->lPosicao < 0)//reiniciar
	{
		//fechar XML
		xmlFreeTextReader (XML->reader);
		//reabrir XML
		if (XML->bArquivo)
			XML->reader = xmlReaderForFile (XML->sExpressao, NULL, 0);//arquivo
		else
			XML->reader = xmlReaderForDoc ((guchar *) XML->sExpressao, NULL, NULL, 0);//string
		XML->lPosicao = 0;
		BE_free (XML->sID);
		XML->sID = g_strdup (".");
		sElemento = g_strdup (".");
	}
	//limpar atributos
	if (XML->Atributos)
	{
		for (ListaL = XML->Atributos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Atributo = (_BEXML_Atributo *) ListaL->data;
			BE_free (Atributo->sNome);
			BE_free (Atributo->sValor);
			BE_free (Atributo);
		}
		BE_slist_free (XML->Atributos);
		XML->nQuantidadeAtributos = 0;
	}
	//loop
	for (XML->lPosicao = 0; XML->lPosicao < (lPosicao + 1); XML->lPosicao++)
	{
		//abrir elemento
		if (xmlTextReaderNodeType (XML->reader) == XML_READER_TYPE_ELEMENT)
		{
			//pegar o elemento atual
			BE_free (sElemento);
			sElemento = g_strdup ((gchar *) xmlTextReaderName (XML->reader));
			//identificação para todos: acrescentar ID
			if (xmlTextReaderIsEmptyElement (XML->reader) == 0)
			{
				sTemp = XML->sID;
				if (sTemp)
					XML->sID = g_strdup_printf ("%s%s.", sTemp, sElemento);
				else
					XML->sID = g_strdup_printf ("%s.", sElemento);
				BE_free (sTemp);
			}
		}
		//avançar próximo, mas verificar quantidade ultrapassada
		if (xmlTextReaderRead (XML->reader) < 1)
		{
			XML->lQuantidade = XML->lPosicao;
			XML->lPosicao = -1;
			break;
		}

		//fechar elemento
		if (xmlTextReaderNodeType (XML->reader) == XML_READER_TYPE_END_ELEMENT)
			XML->sID[strlen (XML->sID) - (g_utf8_strlen ((gchar *) xmlTextReaderName (XML->reader), - 1) + 1)] = '\0';
	}
	BE_free (sElemento);
	BE_free (sTemp);
}

glong be_xml_mover_id (BEXML *XML, const gchar *sID)
{
	glong lCont = 0, lPosA = 0;

	if (sID)
	{
		//começar pela posição atual
		lPosA = be_xml_posicao (XML);
		if (lPosA < 0)
			lPosA = 0;
		//loop apartir da posição atual
		for (lCont = lPosA; lCont < be_xml_quantidade (XML); lCont++)
		{
			be_xml_mover (XML, lCont);
			if (be_xml_id (XML))
				if (g_str_equal (be_xml_id (XML), sID))
						return lCont;
		}
		//se não achou, recomeçar do inicio
		for (lCont = 0; lCont < lPosA; lCont++)
		{
			be_xml_mover (XML, lCont);
			if (be_xml_id (XML))
				if (g_str_equal (be_xml_id (XML), sID))
						return lCont;
		}
	}

	return -1;
}

glong be_xml_posicao (BEXML *XML)
{
	return XML->lPosicao - 1;
}

glong be_xml_quantidade (BEXML *XML)
{
    //contar quantidade
	if (XML->lQuantidade <= 0)
		be_xml_mover (XML, G_MAXLONG - 1);

	return XML->lQuantidade;
}

const gchar *be_xml_id (BEXML *XML)
{
	return XML->sID;
}

const gchar *be_xml_valor (BEXML *XML)
{
	if (xmlTextReaderNodeType (XML->reader) == XML_READER_TYPE_TEXT)
		return (gchar *) xmlTextReaderValue (XML->reader);
	else
		return (gchar *) xmlTextReaderName (XML->reader);
}

guint be_xml_tipo (BEXML *XML)
{
	if (xmlTextReaderIsEmptyElement (XML->reader) == 1 && xmlTextReaderNodeType (XML->reader) == XML_READER_TYPE_ELEMENT)
		return BEXML_READER_TYPE_ELEMENT_EMPTY;
	else
		return xmlTextReaderNodeType (XML->reader);
}

gint be_xml_nivel (BEXML *XML)
{
	return xmlTextReaderDepth (XML->reader);
}

//		ATRIBUTOS

gint be_xml_atributos_quantidade (BEXML *XML)
{
	_BEXML_Atributo *Atributo = NULL;

	if (!XML->Atributos && xmlTextReaderHasAttributes (XML->reader) == 1)
	{
		while (xmlTextReaderMoveToNextAttribute (XML->reader) == 1)//tentar avançar para o próximo atributo
		{
			Atributo = g_new (_BEXML_Atributo, 1);
			Atributo->sNome = g_strdup ((gchar *) xmlTextReaderName (XML->reader));
			Atributo->sValor = g_strdup ((gchar *) xmlTextReaderValue (XML->reader));
			XML->Atributos = g_slist_append (XML->Atributos, Atributo);
		}
		//voltar para o elemento
		xmlTextReaderMoveToElement (XML->reader);
		XML->nQuantidadeAtributos = g_slist_length (XML->Atributos);
	}

	if (XML->Atributos)
		return XML->nQuantidadeAtributos;
	else
		return 0;
}

const gchar *be_xml_atributos_nomes (BEXML *XML, const gint nPosicao)
{
	if (XML->Atributos && nPosicao >= 0 && nPosicao < XML->nQuantidadeAtributos)
		return ((_BEXML_Atributo *) g_slist_nth_data (XML->Atributos, nPosicao))->sNome;
	else
		return NULL;
}

const gchar *be_xml_atributos_valores (BEXML *XML, const gint nPosicao)
{
	if (XML->Atributos && nPosicao >= 0 && nPosicao < XML->nQuantidadeAtributos)
		return ((_BEXML_Atributo *) g_slist_nth_data (XML->Atributos, nPosicao))->sValor;
	else
		return NULL;
}

gint be_xml_atributos_localizar (BEXML *XML, const char *sNome)
{
	gint nCont = 0;

	if (XML->Atributos && sNome)
		for (nCont = 0; nCont < be_xml_atributos_quantidade (XML); nCont++)
			if (g_str_equal (((_BEXML_Atributo *) g_slist_nth_data (XML->Atributos, nCont))->sNome, sNome))
				return nCont;

	return -1;
}

/* Modelo em árvore */
GNode *be_xml_no_carregar (BEXML *XML, gboolean bUTF8)
{
	BEXML_No_Elemento *NoElemento = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	GNode *No = NULL, *NoAnt = NULL;
	gint nNivel = 0;

	//loop
	while (xmlTextReaderRead (XML->reader))
	{
		nNivel = xmlTextReaderDepth (XML->reader) + 1;//nível
		if (xmlTextReaderNodeType (XML->reader) == XML_READER_TYPE_ELEMENT)//criar nó
		{
			NoElemento = g_new (BEXML_No_Elemento, 1);
			if (bUTF8)
			{
				NoElemento->sNome = g_strdup ((gchar *) xmlTextReaderName (XML->reader));
			}
			else
			{
				NoElemento->sNome = BE_UTF8_ISO ((gchar *) xmlTextReaderName (XML->reader));
			}
			NoElemento->Atributos = NULL;
			NoElemento->sValor = NULL;
			//atributos
			if (xmlTextReaderHasAttributes (XML->reader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute (XML->reader) == 1)//tentar avançar para o próximo atributo
				{
					NoAtributo = g_new (BEXML_No_Atributo, 1);
					if (bUTF8)
					{
						NoAtributo->sNome = g_strdup ((gchar *) xmlTextReaderName (XML->reader));
						NoAtributo->sValor = g_strdup ((gchar *) xmlTextReaderValue (XML->reader));
					}
					else
					{
						NoAtributo->sNome = BE_UTF8_ISO ((gchar *) xmlTextReaderName (XML->reader));
						NoAtributo->sValor = BE_UTF8_ISO ((gchar *) xmlTextReaderValue (XML->reader));
					}
					NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
				}
				xmlTextReaderMoveToElement (XML->reader);//voltar para o elemento
			}
			//criar nó
			No = g_node_new (NoElemento);
			//verificar parentesco
			if (NoAnt)
				g_node_append (be_xml_no_pai_nivel (NoAnt, nNivel-1), No);//anexar filho ao pai
			NoAnt = No;
		}
		else if (NoElemento && xmlTextReaderNodeType (XML->reader) == XML_READER_TYPE_TEXT)//valor
		{
			if (nNivel != g_node_depth (No) + 1)
			{
				//criar novo elemento para uma lista de valores
				NoElemento = g_new (BEXML_No_Elemento, 1);
				NoElemento->sNome = g_strdup ("texto");
				NoElemento->Atributos = NULL;

				//criar nó
				No = g_node_new (NoElemento);
				//verificar parentesco
				if (NoAnt)
					g_node_append (be_xml_no_pai_nivel (NoAnt, nNivel - 1), No);//anexar filho ao pai
				NoAnt = No;
			}
			//atribuir valor
			if (bUTF8)
			{
				NoElemento->sValor = g_strdup ((gchar *) xmlTextReaderValue (XML->reader));
			}
			else
			{
				NoElemento->sValor = BE_UTF8_ISO ((gchar *) xmlTextReaderValue (XML->reader));
			}
		}
	}

	if (No)
	{
#ifdef BEXML_DEPURAR
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);
#endif
		return g_node_get_root (No);
	}
	else
		return NULL;
}

void _be_xml_no_descarregar (GNode *No)
{
	if (No)
	{
		No = g_node_get_root (No);
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_descarregar_loop, NULL);
		No = g_node_get_root (No);
		g_node_destroy (No);
	}
}

GNode *be_xml_no_pai_nivel (GNode *No, int nNivel)
{
	if (!No)//não existe este nó
		return NULL;
	else if (g_node_depth (No) == nNivel)//achou, é o pai
		return No;
	else if (g_node_depth (No) < nNivel)//ancestral
		return be_xml_no_pai_nivel (No->children, nNivel);
	else if (g_node_depth (No) > nNivel)//herdeiro
		return be_xml_no_pai_nivel (No->parent, nNivel);
	else
		return NULL;
}

GNode *be_xml_no_nome (GNode *No, const gchar *sElementoNome, BEXML_LOCALIZAR_ENUM nDirecao)
{
	BEXML_No_Elemento *NoElemento = NULL;
	GNode *NoPai = NULL;

	if (!No)//não existe este nó
		return NULL;

	NoElemento = (BEXML_No_Elemento *) No->data;
	//verificar elemento
	if (g_str_equal (NoElemento->sNome, sElementoNome))
		NoPai = No;
	else//não é, continuar procurando
	{
		if (nDirecao == BEXML_LOCALIZAR_PAI)
			NoPai = be_xml_no_nome (No->parent, sElementoNome, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
			NoPai = be_xml_no_nome (No->next, sElementoNome, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_FILHO)
			NoPai = be_xml_no_nome (No->children, sElementoNome, nDirecao);
	}

	return NoPai;
}

GNode *be_xml_no_valor (GNode *No, const gchar *sElementoNome, const gchar *sValor, BEXML_LOCALIZAR_ENUM nDirecao)
{
	BEXML_No_Elemento *NoElemento = NULL;
	GNode *NoPai = NULL;

	if (!No)//não existe este nó
		return NULL;

	NoElemento = (BEXML_No_Elemento *) No->data;
	//verificar elemento
	if (g_str_equal (NoElemento->sNome, sElementoNome))
	{
		//verificar valor
		if ((!NoElemento->sValor && !sValor) || (NoElemento->sValor && sValor && g_str_equal (NoElemento->sValor, sValor)))
			NoPai = No;
		else//não é, continuar procurando
		{
			if (nDirecao == BEXML_LOCALIZAR_PAI)
				NoPai = be_xml_no_valor (No->parent, sElementoNome, sValor, nDirecao);
			else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
				NoPai = be_xml_no_valor (No->next, sElementoNome, sValor, nDirecao);
			else if (nDirecao == BEXML_LOCALIZAR_FILHO)
				NoPai = be_xml_no_valor (No->children, sElementoNome, sValor, nDirecao);
		}
	}
	else//não é, continuar procurando
	{
		if (nDirecao == BEXML_LOCALIZAR_PAI)
			NoPai = be_xml_no_valor (No->parent, sElementoNome, sValor, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
			NoPai = be_xml_no_valor (No->next, sElementoNome, sValor, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_FILHO)
			NoPai = be_xml_no_valor (No->children, sElementoNome, sValor, nDirecao);
	}

	return NoPai;
}

GNode *be_xml_no_atributo (GNode *No, const gchar *sElementoNome, const gchar *sAtributoNome, BEXML_LOCALIZAR_ENUM nDirecao)
{
	BEXML_No_Elemento *NoElemento = NULL;
	GNode *NoPai = NULL;
	const gchar *sValor = NULL;

	if (!No)//não existe este nó
		return NULL;

	NoElemento = (BEXML_No_Elemento *) No->data;
	//verificar elemento
	if (!sElementoNome || g_str_equal (NoElemento->sNome, sElementoNome))
	{
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, sAtributoNome);//procurar atributo
		if (sValor)
			NoPai = No;
		else//não achou, continuar procurando
		{
			if (nDirecao == BEXML_LOCALIZAR_PAI)
				NoPai = be_xml_no_atributo (No->parent, sElementoNome, sAtributoNome, nDirecao);
			else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
				NoPai = be_xml_no_atributo (No->next, sElementoNome, sAtributoNome, nDirecao);
			else if (nDirecao == BEXML_LOCALIZAR_FILHO)
				NoPai = be_xml_no_atributo (No->children, sElementoNome, sAtributoNome, nDirecao);
		}
	}
	else//não é, continuar procurando
	{
		if (nDirecao == BEXML_LOCALIZAR_PAI)
			NoPai = be_xml_no_atributo (No->parent, sElementoNome, sAtributoNome, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
			NoPai = be_xml_no_atributo (No->next, sElementoNome, sAtributoNome, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_FILHO)
			NoPai = be_xml_no_atributo (No->children, sElementoNome, sAtributoNome, nDirecao);
	}

	return NoPai;
}

GNode *be_xml_no_atributo_valor (GNode *No, const gchar *sElementoNome, const gchar *sAtributoNome, const gchar *sAtributoValor, BEXML_LOCALIZAR_ENUM nDirecao)
{
	BEXML_No_Elemento *NoElemento = NULL;
	GNode *NoPai = NULL;
	const gchar *sValor = NULL;

	if (!No)//não existe este nó
		return NULL;

	NoElemento = (BEXML_No_Elemento *) No->data;
	//verificar elemento
	if (!sElementoNome || g_str_equal (NoElemento->sNome, sElementoNome))
	{
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, sAtributoNome);//procurar atributo
		if (sValor && g_str_equal (sValor, sAtributoValor))
			NoPai = No;
		else//não achou, continuar procurando
		{
			if (nDirecao == BEXML_LOCALIZAR_PAI)
				NoPai = be_xml_no_atributo_valor (No->parent, sElementoNome, sAtributoNome, sAtributoValor, nDirecao);
			else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
				NoPai = be_xml_no_atributo_valor (No->next, sElementoNome, sAtributoNome, sAtributoValor, nDirecao);
			else if (nDirecao == BEXML_LOCALIZAR_FILHO)
				NoPai = be_xml_no_atributo_valor (No->children, sElementoNome, sAtributoNome, sAtributoValor, nDirecao);
		}
	}
	else//não é, continuar procurando
	{
		if (nDirecao == BEXML_LOCALIZAR_PAI)
			NoPai = be_xml_no_atributo_valor (No->parent, sElementoNome, sAtributoNome, sAtributoValor, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_IRMAO)
			NoPai = be_xml_no_atributo_valor (No->next, sElementoNome, sAtributoNome, sAtributoValor, nDirecao);
		else if (nDirecao == BEXML_LOCALIZAR_FILHO)
			NoPai = be_xml_no_atributo_valor (No->children, sElementoNome, sAtributoNome, sAtributoValor, nDirecao);
	}

	return NoPai;
}

GNode *be_xml_no_localizar_elemento (GNode *No, const gchar *sElementoNome, const gchar *sAtributoNome, const gchar *sAtributoValor)
{
	_BEXML_No_Localizar * Localizar = NULL;
	GNode *NoEncontrado = NULL;

	//criar
	Localizar = g_new (_BEXML_No_Localizar, 1);
	Localizar->sElementoNome = sElementoNome;
	Localizar->sAtributoNome = sAtributoNome;
	Localizar->sAtributoValor = sAtributoValor;
	Localizar->No = NULL;
	//localizar recursivamente
	g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_localizar_loop, Localizar);

	NoEncontrado = Localizar->No;
	//limpar
	BE_free (Localizar);

	return NoEncontrado;
}

gboolean _be_xml_no_localizar_loop (GNode *node, gpointer data)
{
	_BEXML_No_Localizar * Localizar = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	const gchar *sValor = NULL;

	NoElemento = (BEXML_No_Elemento *) node->data;
	Localizar = (_BEXML_No_Localizar *) data;
	//comparar
	if (!Localizar->sElementoNome || g_str_equal (NoElemento->sNome, Localizar->sElementoNome))
	{
		if (Localizar->sAtributoNome)
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, Localizar->sAtributoNome);//procurar atributo
		if (!Localizar->sAtributoNome || (sValor && g_str_equal (sValor, Localizar->sAtributoValor)))//achou
		{
			Localizar->No = node;
			return TRUE;//parar
		}
	}
	//não achou, continuar procurando
	return FALSE;
}

gboolean _be_xml_no_descarregar_loop (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	GSList *ListaL = NULL;

	NoElemento = (BEXML_No_Elemento *) node->data;
	BE_free (NoElemento->sNome);
	BE_free (NoElemento->sValor);
	//atributos

	for (ListaL = NoElemento->Atributos; ListaL; ListaL = g_slist_next (ListaL))
	{
		NoAtributo = (BEXML_No_Atributo *) ListaL->data;
		BE_free (NoAtributo->sNome);
		BE_free (NoAtributo->sValor);
		BE_free (NoAtributo);
	}
	BE_slist_free (NoElemento->Atributos);
	BE_free (NoElemento);

	return FALSE;
}

const gchar *be_xml_no_atributos_valor (GSList *Atributos, const gchar *sNome)
{
	const BEXML_No_Atributo *NoAtributo = NULL;
	GSList *ListaL = NULL;

	for (ListaL = Atributos; ListaL; ListaL = g_slist_next (ListaL))
	{
		NoAtributo = (BEXML_No_Atributo *) ListaL->data;
		if (g_str_equal (NoAtributo->sNome, sNome))
			return NoAtributo->sValor;
	}

	return NULL;
}

gboolean _be_xml_no_loop (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	GSList *ListaL = NULL;
	char space[256];

	NoElemento = (BEXML_No_Elemento*) node->data;
	memset (&space, '\0', 255);
	if (g_node_depth (node) > 1)
		memset (&space, '-', (g_node_depth (node) -1) * 4);
	printf ("%s%s = %s\n", space, NoElemento->sNome, NoElemento->sValor);
	//atributos
	for (ListaL = NoElemento->Atributos; ListaL; ListaL = g_slist_next (ListaL))
	{
		NoAtributo = (BEXML_No_Atributo *) ListaL->data;
		printf ("%s\t[%s = %s]\n", space, NoAtributo->sNome, NoAtributo->sValor);
	}

	return FALSE;
}
