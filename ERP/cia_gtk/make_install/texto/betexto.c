#include "betexto.h"

BETEXTO *be_texto_abrir (const gchar *sArquivoFonte, const gchar *sFontePadrao, const gchar *sEstilo, const gfloat fTamanhoPadrao)
{
	BETEXTO *Texto = NULL;
	BEFONTES *Fonte = NULL;

	Texto = g_new (BETEXTO, 1);
	Texto->NoXMLs = NULL;
	Texto->Fontes = NULL;
	if (fTamanhoPadrao >= 1.0)
		Texto->fTamanho = fTamanhoPadrao;
	else
		Texto->fTamanho = 10.0;
	//carregar fonte padrão
	if (sArquivoFonte)
	{
		Texto->sArquivoFonte = g_strdup (sArquivoFonte);
		Fonte = be_fontes_carregar (Texto->sArquivoFonte, sFontePadrao, sEstilo);
	}
	//sair se a fonte não existir
	if (!Fonte)
		return NULL;
	//anexar fonte
	Texto->Fontes = g_slist_append (Texto->Fontes, Fonte);

	return Texto;
}

GNode *be_texto_analisar (
	BETEXTO *Texto,
	const gchar *sExpressao,
	gfloat fLarguraMax,
	gfloat fAlturaLinha,
	gboolean bQuebrar,
	gboolean bUTF8
	)
{
	BEXML *XML = NULL;
	GNode *No = NULL, *NoNovo = NULL;
	BEFONTES *Fonte = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	gchar *sXML = NULL, *sTemp = NULL;
	gint nCont = 0;

	//iniciar valores
	Texto->nLinha = 1;
	Texto->fX = 0.0;
	Texto->fY = 0.0;
	Texto->fYMax = 0.0;
	Texto->fYMin = 0.0;
	Texto->fYMinAnt = 0.0;
	Texto->fLarguraMax = fLarguraMax < BETEXTO_LARGURA_MIN ? BETEXTO_LARGURA_MIN : fLarguraMax;
	Texto->fAlturaLinha = fAlturaLinha < BETEXTO_ALTURA_MIN ? 1.0 : fAlturaLinha / 100.0;
	Texto->bQuebrar = bQuebrar;
	Texto->fLarguraBloco = 0.0;
	Texto->fAlturaBloco = 0.0;
	Texto->nEspacos = 0;
	Texto->bQuebrarAgora = FALSE;

	//fonte padrão
	Fonte = (BEFONTES *) Texto->Fontes->data;
	sXML = g_strdup_printf ("<font face='%s' size='%.3f' style='%s' color='000000' bgcolor='FFFFFF'>%s</font>", Fonte->sNome, Texto->fTamanho, Fonte->sEstilo, sExpressao);
	//conversões
	BE_free (sTemp);
	sTemp = sXML;
	sXML = be_strreplace (sTemp, "<br>", "<br/>");
	if (!bUTF8)
	{
		BE_free (sTemp);
		sTemp = sXML;
		sXML = BE_ISO_UTF8 (sTemp);
	}
	for (nCont = 0; nCont < g_utf8_strlen (sXML, -1); nCont++)
	{
		if (sXML[nCont] >= 0 && sXML[nCont] < 32)
			sXML[nCont] = '?';
	}

	//carregar Nó
	XML = be_xml_abrir (sXML, FALSE);
	if (XML)
	{
		No = be_xml_no_carregar (XML, FALSE);
		//anexar Nó
		Texto->NoXMLs = g_slist_append (Texto->NoXMLs, No);
		//colocar texto real dentro de um elemento 'texto'
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_texto_preparar_texto, NULL);
		//localizar negrito itálico
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_texto_preparar_negrito_italico, NULL);
		//completar fontes com marcações diferentes, itálico, negrito, subescrito, sobreescrito, etc.
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_texto_preparar_fontes, NULL);
		//carregar todas as fontes
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_texto_carregar_fontes, Texto);
		//calcular altura, largura, quebra de linha
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_texto_dimensoes, Texto);
		//última linha
		_be_texto_adicionar_linha (No, Texto);

		//bloco do texto
		NoElemento = g_new (BEXML_No_Elemento, 1);
		NoElemento->sNome = g_strdup ("bloco");
		NoElemento->Atributos = NULL;
		NoElemento->sValor = NULL;
		//adicionar atributo largura original
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("largura original");
		NoAtributo->sValor = g_strdup_printf ("%.3f", fLarguraMax);
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
		//adicionar atributo largura
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("largura");
		NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fLarguraBloco);
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
		//adicionar atributo altura
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("altura");
		NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fAlturaBloco);
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
		//criar nó
		NoNovo = g_node_new (NoElemento);
		//adicionar irmão
		g_node_prepend (g_node_get_root (No), NoNovo);
	}
	BE_xml_fechar (XML);
	//limpar
	BE_free (sXML);
	BE_free (sTemp);

#ifdef BETEXTO_DEPURAR
	g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);
#endif

	return No;
}

void _be_texto_fechar (BETEXTO *Texto)
{
	BEFONTES *Fonte = NULL;
	GNode *No = NULL;
	GSList *ListaL = NULL;

	//nós
	for (ListaL = Texto->NoXMLs; ListaL; ListaL = g_slist_next (ListaL))
	{
		No = (GNode *) ListaL->data;
		BE_xml_no_descarregar (No);
	}
#ifdef BETEXTO_DEPURAR
	printf ("CARREGADAS %d FONTES\n", g_slist_length (Texto->Fontes));
#endif
	//fontes
	for (ListaL = Texto->Fontes; ListaL; ListaL = g_slist_next (ListaL))
	{
		Fonte = (BEFONTES *) ListaL->data;
#ifdef BETEXTO_DEPURAR
		printf ("\t%s %s - %s\n", Fonte->sNome, Fonte->sEstilo, Fonte->sEstiloUsual);
#endif
		be_fontes_descarregar (Fonte);
	}
	BE_free (Texto->sArquivoFonte);
	BE_slist_free (Texto->Fontes);
	BE_free (Texto);
}

gboolean _be_texto_preparar_texto (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL, *NoElementoNovo = NULL;
	GNode *NoNovo = NULL;

	NoElemento = (BEXML_No_Elemento *) node->data;
	if (!g_str_equal (NoElemento->sNome, "texto") && NoElemento->sValor)
	{
		//criando elemento texto
		NoElementoNovo = g_new (BEXML_No_Elemento, 1);
		NoElementoNovo->sNome = g_strdup ("texto");
		NoElementoNovo->Atributos = NULL;
		NoElementoNovo->sValor = g_strdup (NoElemento->sValor);
		//criar nó
		NoNovo = g_node_new (NoElementoNovo);
		//adicionar filho
		node = g_node_prepend (node, NoNovo);
		//limpar antigo
		BE_free (NoElemento->sValor);
	}

	return FALSE;
}

gboolean _be_texto_carregar_fontes (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	BEFONTES *FonteNova = NULL;
	BETEXTO *Texto = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	const gchar *sFonte = NULL, *sEstilo = NULL;
	gint nFonte = 0;
	const gchar *sValor = NULL;

	NoElemento = (BEXML_No_Elemento *) node->data;
	Texto = (BETEXTO *) data;
	//fontes
	if (g_str_equal (NoElemento->sNome, "font"))
	{
		//procurar por fonte
		sFonte = be_xml_no_atributos_valor (NoElemento->Atributos, "face");
		if (sFonte)
		{
			//procurar por estilo
			sEstilo = be_xml_no_atributos_valor (NoElemento->Atributos, "style");
			//localizar fonte existente
			nFonte = be_texto_localizar_fonte (Texto, sFonte, sEstilo);
			//verificar nova fonte
			if (nFonte < 0)
			{
				if (sEstilo)
					FonteNova = be_fontes_carregar (Texto->sArquivoFonte, sFonte, sEstilo);
				else
					FonteNova = be_fontes_carregar (Texto->sArquivoFonte, sFonte, "");
				//verificar se carregou
				if (FonteNova)
				{
					//verificar mais uma vez, se a nova fonte já existe
					nFonte = be_texto_localizar_fonte (Texto, (const gchar *) FonteNova->sNome, (const gchar *) FonteNova->sEstilo);
					if (nFonte < 0)
					{
						//anexar fonte
						Texto->Fontes = g_slist_append (Texto->Fontes, FonteNova);
						nFonte = g_slist_length (Texto->Fontes) - 1;
					}
					//descarregar fonte duplicada
					else
						be_fontes_descarregar (FonteNova);
				}
				//impossível carregar fonte
				else
					nFonte = 0;
			}
			//adicionar indice da fonte existente
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "indice");//procurar atributo
			if (!sValor)
			{
				NoAtributo = g_new (BEXML_No_Atributo, 1);
				NoAtributo->sNome = g_strdup ("indice");
				NoAtributo->sValor = g_strdup_printf ("%d", nFonte);
				//anexar
				NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
			}
		}
	}

	BE_free (sTemp);
	BE_free (sTemp1);

	return FALSE;
}

gboolean _be_texto_preparar_negrito_italico (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL, *NoElementoPai = NULL;
	GNode *NoPai = NULL, *NoPai2 = NULL;
	gboolean bAlterar = FALSE;
	const gchar *sValor = NULL;

	NoElemento = (BEXML_No_Elemento *) node->data;
	if (g_str_equal (NoElemento->sNome, "i"))
	{
		//procurar por um b, ancestral
		NoPai = be_xml_no_nome (node->parent, "b", BEXML_LOCALIZAR_PAI);
		//procurar por uma fonte, ancestral
		NoPai2 = be_xml_no_atributo (node->parent, "font", "style", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai2->data;
		sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "style");//procurar atributo

		if (NoPai && sValor)
		{
			//verificar quem é mais próximo
			if (g_node_depth (NoPai) > g_node_depth (NoPai2))
				bAlterar = TRUE;
			else if (g_str_equal (sValor, "negrito"))
				bAlterar = TRUE;
		}
		else if (sValor && g_str_equal (sValor, "negrito"))
		{
				bAlterar = TRUE;
		}
		else if (NoPai)
		{
			bAlterar = TRUE;
		}
	}
	else if (g_str_equal (NoElemento->sNome, "b"))
	{
		//procurar por um b, ancestral
		NoPai = be_xml_no_nome (node->parent, "i", BEXML_LOCALIZAR_PAI);
		//procurar por uma fonte, ancestral
		NoPai2 = be_xml_no_atributo (node->parent, "font", "style", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai2->data;
		sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "style");//procurar atributo

		if (NoPai && sValor)
		{
			//verificar quem é mais próximo
			if (g_node_depth (NoPai) > g_node_depth (NoPai2))
				bAlterar = TRUE;
			else if (g_str_equal (sValor, "italico"))
				bAlterar = TRUE;
		}
		else if (sValor && g_str_equal (sValor, "italico"))
		{
				bAlterar = TRUE;
		}
		else if (NoPai)
		{
			bAlterar = TRUE;
		}
	}
	//alterar nome
	if (bAlterar)
	{
		BE_free (NoElemento->sNome);
		NoElemento->sNome = g_strdup ("bi");//negrito itálico
	}

	return FALSE;
}

gboolean _be_texto_preparar_fontes (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL, *NoElementoPai = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	GNode *NoPai = NULL;
	const gchar *sValor = NULL;
	gfloat fValor = 0.0;

	NoElemento = (BEXML_No_Elemento *) node->data;

	//negrito, itálico e negrito-itálico
	if (g_str_equal (NoElemento->sNome, "i") || g_str_equal (NoElemento->sNome, "b") || g_str_equal (NoElemento->sNome, "bi"))
	{
		//procurar por uma fonte, ancestral
		NoPai = be_xml_no_atributo (node->parent, "font", "face", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "face");//procurar atributo
		//adicionar atributo fonte
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("face");
		NoAtributo->sValor = g_strdup (sValor);
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);

		//procurar por um tamanho de fonte, ancestral
		NoPai = be_xml_no_atributo (node->parent, "font", "size", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "size");//procurar atributo
		//adicionar atributo fonte
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("size");
		NoAtributo->sValor = g_strdup (sValor);
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);

		//adicionar atributo estilo
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("style");
		if (g_str_equal (NoElemento->sNome, "i"))
			NoAtributo->sValor = g_strdup ("italico");
		else if (g_str_equal (NoElemento->sNome, "b"))
			NoAtributo->sValor = g_strdup ("negrito");
		else if (g_str_equal (NoElemento->sNome, "bi"))
			NoAtributo->sValor = g_strdup ("negrito italico");
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);

		//alterar nome
		BE_free (NoElemento->sNome);
		NoElemento->sNome = g_strdup ("font");
	}
	//subescrito e sobreescrito
	else if (g_str_equal (NoElemento->sNome, "sup") || g_str_equal (NoElemento->sNome, "sub"))
	{
		//procurar por um tamanho de fonte, ancestral
		NoPai = be_xml_no_atributo (node->parent, "font", "size", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "size");//procurar atributo
		//adicionar atributo fonte
		fValor = g_strtod (sValor, NULL);
		NoAtributo = g_new (BEXML_No_Atributo, 1);
		NoAtributo->sNome = g_strdup ("size");
		NoAtributo->sValor = g_strdup_printf ("%.3f", fValor * BETEXTO_SUBSOB_ESCRITO);
		NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
	}

	return FALSE;
}

gboolean _be_texto_dimensoes (GNode *node, gpointer data)
{
	BEXML_No_Elemento *NoElemento = NULL, *NoElementoPai = NULL, *NoElementoNovo = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	GNode *NoPai = NULL, *NoNovo = NULL;
	BEFONTES *Fonte = NULL;
	BETEXTO *Texto = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	const gchar *sTamanho = NULL, *sValor = NULL;
	gint nFonte = 0;
	gfloat fLargura = 0.0, fLarguraAtual = 0.0, fTamanho = 0.0;
	gint nCont = 0, nContQuebra = 0;
	gint nYMin = 0, nYMax = 0;
	gfloat fYMin = 0.0, fYMax = 0.0;

	NoElemento = (BEXML_No_Elemento *) node->data;
	Texto = (BETEXTO *) data;
	//textos
	if (g_str_equal (NoElemento->sNome, "texto") && NoElemento->sValor)
	{
		//procurar por uma fonte, ancestral
		NoPai = be_xml_no_atributo (node->parent, "font", "indice", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "indice");//procurar atributo
		nFonte = (gint) g_strtod (sValor, NULL);
		//procurar por um tamanho de fonte, ancestral
		NoPai = be_xml_no_atributo (node->parent, NULL, "size", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sTamanho = be_xml_no_atributos_valor (NoElementoPai->Atributos, "size");//procurar atributo
		fTamanho = g_strtod (sTamanho, NULL);

		if (nFonte >= 0 && nFonte < g_slist_length (Texto->Fontes))
		{
			//localizar fonte real
			Fonte = (BEFONTES *) g_slist_nth_data (Texto->Fontes, nFonte);

			//largura do texto
			fLargura = ((gfloat) be_fontes_largura (Fonte, NoElemento->sValor)) / Fonte->nUnidade * fTamanho;

			//verificar a quebra de linha
			if (Texto->bQuebrarAgora)
			{
				_be_texto_adicionar_linha (g_node_get_root (node), Texto);//próxima linha
				Texto->bQuebrarAgora = FALSE;
			}
			//quebrar linha
			if (Texto->bQuebrar)
			{
				//verificar se ultrapassou
				if ((Texto->fX + fLargura) > Texto->fLarguraMax)
				{
					//separar caractéres, quebra inteligente
					nContQuebra = -1;
					for (nCont = strlen (NoElemento->sValor) - 1; nCont >= 0; nCont--)//de tráz pra frente
					{
						fLarguraAtual = (gfloat) Fonte->nLetra[((guchar) NoElemento->sValor[nCont]) - 32][4] / (gfloat) Fonte->nUnidade * fTamanho;
						fLargura -= fLarguraAtual;
						//verificar se agora cabe, e marcar posição de quebra
						if (nContQuebra < 0  && (Texto->fX + fLargura) <= Texto->fLarguraMax)
							nContQuebra = nCont;
						//cortar o elemento atual e criar um próximo com o restante do texto
						if (nContQuebra >= 0 && (NoElemento->sValor[nCont] == ' '))
						{
							//criar novo elemento texto, próximo irmão
							if (nCont < strlen (NoElemento->sValor) - 1)
							{
								NoElementoNovo = g_new (BEXML_No_Elemento, 1);
								NoElementoNovo->sNome = g_strdup ("texto");
								NoElementoNovo->Atributos = NULL;
								NoElementoNovo->sValor = g_strdup (NoElemento->sValor + nCont + 1);//excluir o primeiro caractére
								//criar nó
								NoNovo = g_node_new (NoElementoNovo);
							}
							//verificar se a linha atual não vair ficar vazia
							if (nCont > 0 || Texto->fX > 0.0)
								Texto->bQuebrarAgora = TRUE;//marcar para quebrar linha, na próxima chamada
							//cortar o elemento atual aqui
							BE_free (sTemp);
							sTemp = NoElemento->sValor;
							sTemp[nCont] = '\0';
							NoElemento->sValor = g_strdup (sTemp);
							//parar loop
							break;
						}
						//palavra muito longa não cabe, forçar quebra aqui
						else if (nCont == 0)
						{
							//criar novo elemento texto, próximo irmão
							NoElementoNovo = g_new (BEXML_No_Elemento, 1);
							NoElementoNovo->sNome = g_strdup ("texto");
							NoElementoNovo->Atributos = NULL;
							//criar nó
							NoNovo = g_node_new (NoElementoNovo);
							//marcar para quebrar linha, na próxima chamada
							Texto->bQuebrarAgora = TRUE;

							//tem mais palavras nesta linha, jogar esta para a próxima linha
							if (Texto->fX > 0.0)
							{
								NoElementoNovo->sValor = g_strdup (NoElemento->sValor);//sem excluir
								//anular o elemento atual
								NoElemento->sValor[0] = '\0';
							}
							//cortar no meio
							else
							{
								NoElementoNovo->sValor = g_strdup (NoElemento->sValor + nContQuebra);//sem excluir
								//cortar o elemento atual aqui
								BE_free (sTemp);
								sTemp = NoElemento->sValor;
								sTemp[nContQuebra] = '\0';
								NoElemento->sValor = g_strdup (sTemp);
							}
						}
					}
					//recalcular largura
					fLargura = ((gfloat) be_fontes_largura (Fonte, NoElemento->sValor)) / Fonte->nUnidade * fTamanho;
				}
			}
			//verificar texto inválido
			if (NoElemento->sValor && strlen (NoElemento->sValor) == 0)
			{
				BE_free (NoElemento->sValor);
			}
			else if (NoElemento->sValor)
			{
				//contar espaços
				Texto->nEspacos += be_strcount (NoElemento->sValor, " ");
				//altura real: texto
				be_fontes_altura (Fonte, NoElemento->sValor, &nYMin, &nYMax);
			}
			//altura do texto na caixa: fonte
			fYMin = (gfloat) Fonte->nCaixa[2] / (gfloat) Fonte->nUnidade * fTamanho;
			fYMax = (gfloat) Fonte->nCaixa[3] / (gfloat) Fonte->nUnidade * fTamanho;
			//top mais
			if (fYMax > Texto->fYMax)
				Texto->fYMax = fYMax;
			if (fYMin < Texto->fYMin)
				Texto->fYMin = fYMin;

			//adicionar atributo linha
			NoAtributo = g_new (BEXML_No_Atributo, 1);
			NoAtributo->sNome = g_strdup ("linha");
			NoAtributo->sValor = g_strdup_printf ("%d", Texto->nLinha);
			NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
			//adicionar atributo X
			NoAtributo = g_new (BEXML_No_Atributo, 1);
			NoAtributo->sNome = g_strdup ("x");
			NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fX);
			NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
			//adicionar atributo largura
			NoAtributo = g_new (BEXML_No_Atributo, 1);
			NoAtributo->sNome = g_strdup ("largura");
			NoAtributo->sValor = g_strdup_printf ("%.3f", fLargura);
			NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
			//adicionar atributo yMin
			NoAtributo = g_new (BEXML_No_Atributo, 1);
			NoAtributo->sNome = g_strdup ("yMin");
			NoAtributo->sValor = g_strdup_printf ("%.3f", (gfloat) nYMin / (gfloat) Fonte->nUnidade * fTamanho);
			NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
			//adicionar atributo yMax
			NoAtributo = g_new (BEXML_No_Atributo, 1);
			NoAtributo->sNome = g_strdup ("yMax");
			NoAtributo->sValor = g_strdup_printf ("%.3f", (gfloat) nYMax / (gfloat) Fonte->nUnidade * fTamanho);
			NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);

			//próxima posição X
			Texto->fX += fLargura;
		}
	}
	//forçar pular linha
	else if (g_str_equal (NoElemento->sNome, "br"))
	{
		//próxima linha
		Texto->bQuebrarAgora = TRUE;
	}
	//espaço
	else if (g_str_equal (NoElemento->sNome, "space"))
	{
		//largura
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "largura");
		if (sValor)
		{
			fTamanho = g_strtod (sValor, NULL);
			Texto->fX += fTamanho;
		}
		//altura
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "altura");
		if (sValor)
		{
			fTamanho = g_strtod (sValor, NULL);
			Texto->fY += fTamanho;
		}
	}
	//sub-escrito
	else if (g_str_equal (NoElemento->sNome, "sub"))
	{
		//ignorar a alteração na altura do texto, pois é insignificante
	}
	//sobre-escrito
	else if (g_str_equal (NoElemento->sNome, "sup"))
	{
		//ignorar a alteração na altura do texto, pois é insignificante
	}

	BE_free (sTemp);
	BE_free (sTemp1);

	if (NoNovo)
	{
		//adicionar próximo irmão, não último
		g_node_insert_after (node->parent, node, NoNovo);
		//chamada recursiva
		_be_texto_dimensoes (NoNovo, Texto);
	}

	return FALSE;
}

void _be_texto_adicionar_linha (GNode *Root, BETEXTO *Texto)
{
	BEXML_No_Elemento *NoElemento = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	GNode *NoNovo = NULL;

	//posição Y desta linha
	if (Texto->nLinha > 1)
		Texto->fY += (-(Texto->fYMinAnt) + Texto->fYMax) * Texto->fAlturaLinha;
	else
		Texto->fY += (-(Texto->fYMinAnt) + Texto->fYMax);
	//criar novo elemento linha
	NoElemento = g_new (BEXML_No_Elemento, 1);
	NoElemento->sNome = g_strdup ("linha");
	NoElemento->Atributos = NULL;
	NoElemento->sValor = g_strdup_printf ("%d", Texto->nLinha);
	//adicionar atributo Y
	NoAtributo = g_new (BEXML_No_Atributo, 1);
	NoAtributo->sNome = g_strdup ("y");
	NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fY);
	NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
	//adicionar atributo yMin
	NoAtributo = g_new (BEXML_No_Atributo, 1);
	NoAtributo->sNome = g_strdup ("yMin");
	NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fYMin);
	NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
	//adicionar atributo yMax
	NoAtributo = g_new (BEXML_No_Atributo, 1);
	NoAtributo->sNome = g_strdup ("yMax");
	NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fYMax);
	NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
	//adicionar atributo largura
	NoAtributo = g_new (BEXML_No_Atributo, 1);
	NoAtributo->sNome = g_strdup ("largura");
	NoAtributo->sValor = g_strdup_printf ("%.3f", Texto->fX);
	NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);
	//adicionar atributo quantidade de espaços
	NoAtributo = g_new (BEXML_No_Atributo, 1);
	NoAtributo->sNome = g_strdup ("espacos");
	NoAtributo->sValor = g_strdup_printf ("%d", Texto->nEspacos);
	NoElemento->Atributos = g_slist_append (NoElemento->Atributos, NoAtributo);

	//criar nó
	NoNovo = g_node_new (NoElemento);
	//adicionar irmão
	g_node_append (Root, NoNovo);

	//bloco de texto
	if (Texto->fX > Texto->fLarguraBloco)
		Texto->fLarguraBloco = Texto->fX;
	if ((Texto->fY - Texto->fYMin) > Texto->fAlturaBloco)
		Texto->fAlturaBloco = Texto->fY - Texto->fYMin;

	//reiniciar
	Texto->nLinha++;
	Texto->fX = 0.0;
	Texto->fYMax = 0.0;
	Texto->fYMinAnt = Texto->fYMin;
	Texto->fYMin = 0.0;
	Texto->nEspacos = 0;
}

gint be_texto_localizar_fonte (const BETEXTO *Texto, const gchar *sNome, const gchar *sEstilo)
{
	BEFONTES *Fonte = NULL;
	GSList *ListaL = NULL;
	gint nCont = 0, nFonte = -1;
	gchar *sNome_m = NULL, *sEstilo_m = NULL;
	gchar *sTemp = NULL;

	//preparar nomes
	sNome_m = g_utf8_strdown (sNome, -1);
	if (sEstilo && strlen (sEstilo) > 0)
		sEstilo_m = g_utf8_strdown (sEstilo, -1);
	else
		sEstilo_m = g_strdup ("normal");
	//localizar fonte existente
	for (ListaL = Texto->Fontes; ListaL; ListaL = g_slist_next (ListaL), nCont++)
	{
		Fonte = (BEFONTES *) ListaL->data;
		//comparar fonte
		BE_free (sTemp);
		sTemp = g_utf8_strdown ((gchar *) Fonte->sNome, -1);
		if (g_str_equal (sTemp, sNome_m))
		{
			//comparar estilo
			BE_free (sTemp);
			sTemp = g_utf8_strdown (Fonte->sEstiloUsual, -1);
			if (g_strrstr (sTemp, sEstilo_m))
			{
				nFonte = nCont;
				break;
			}
		}
	}
	BE_free (sNome_m);
	BE_free (sEstilo_m);
	BE_free (sTemp);

	return nFonte;
}
