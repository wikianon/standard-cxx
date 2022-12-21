#include "bepdf.h"

BEPDF *be_pdf_novo (const gchar *sArquivoFonte, const gchar *sFontePadrao, const gchar *sEstiloPadrao, const gfloat fTamanhoPadrao)
{
	BEPDF *PDF = NULL;
	BEPDF_Pagina *Pagina = NULL;
	//criar PDF
	PDF = g_new (BEPDF, 1);
	PDF->Pagina = NULL;
	PDF->Texto = be_texto_abrir (sArquivoFonte, sFontePadrao, sEstiloPadrao, fTamanhoPadrao);
	PDF->_lPaginas = 1;//página inicial
	PDF->_Celulas = NULL;
	PDF->_Imagens = NULL;
	//sair se não houver esta fonte
	if (!PDF->Texto)
	{
		g_printerr ("BEPDF: Nao foi possivel carregar a fonte padrao: %s %s\n", sFontePadrao, sEstiloPadrao);
		BE_free (PDF);
		return NULL;
	}

	//criar página
	Pagina = g_new (BEPDF_Pagina, 1);
	Pagina->fAltura = BEPDF_A4_ALTURA;//A4
	Pagina->fLargura = BEPDF_A4_LARGURA;
	Pagina->fMargEsquerda = 10.0 * BEPDF_PIXEL_POR_POLEGADA;//20% da página
	Pagina->fMargDireita = 10.0 * BEPDF_PIXEL_POR_POLEGADA;
	Pagina->fMargSuperior = 10.0 * BEPDF_PIXEL_POR_POLEGADA;
	Pagina->fMargInferior = 10.0 * BEPDF_PIXEL_POR_POLEGADA;
	Pagina->fCabAltura = 10.0 * BEPDF_PIXEL_POR_POLEGADA;//40% da página
	Pagina->fRodAltura = 5.0 * BEPDF_PIXEL_POR_POLEGADA;
	Pagina->sTitulo[0] = '\0';
	Pagina->sAutor[0] = '\0';
	Pagina->sProdutor[0] = '\0';
	//anexar página ao PDF
	PDF->Pagina = Pagina;

	return PDF;
}

BEPDF_Pagina *be_pdf_pagina (
	BEPDF *PDF,
	gfloat fAltura,
	gfloat fLargura,
	gfloat fMargEsquerda,
	gfloat fMargDireita,
	gfloat fMargSuperior,
	gfloat fMargInferior,
	gfloat fCabAltura,
	gfloat fRodAltura,
	const gchar *sTitulo,
	const gchar *sAutor,
	const gchar *sProdutor)
{
	BEPDF_Pagina *Pagina = NULL;

	Pagina = PDF->Pagina;
	if (fAltura > 0.0)
		Pagina->fAltura = fAltura;
	if (fLargura > 0.0)
		Pagina->fLargura = fLargura;
	if (fMargEsquerda > 0.0)
		Pagina->fMargEsquerda = fMargEsquerda;
	if (fMargDireita > 0.0)
		Pagina->fMargDireita = fMargDireita;
	if (fMargSuperior > 0.0)
		Pagina->fMargSuperior = fMargSuperior;
	if (fMargInferior > 0.0)
		Pagina->fMargInferior = fMargInferior;
	if (fCabAltura > 0.0)
		Pagina->fCabAltura = fCabAltura;
	if (fRodAltura > 0.0)
		Pagina->fRodAltura = fRodAltura;
	if (sTitulo)
		g_strlcpy (Pagina->sTitulo, sTitulo, BEPDF_ADOBE_NOMES_LEN);
	if (sAutor)
		g_strlcpy (Pagina->sAutor, sAutor, BEPDF_ADOBE_NOMES_LEN);
	if (sProdutor)
		g_strlcpy (Pagina->sProdutor, sProdutor, BEPDF_ADOBE_NOMES_LEN);

	return Pagina;
}

BEPDF *_be_pdf_fechar (BEPDF *PDF)
{
	BEPDF_Celula *Celula = NULL;
	BEPDF_Imagem *Imagem = NULL;
	GSList *ListaL = NULL;

	if (!PDF)
		return NULL;

	//pagina
	BE_free (PDF->Pagina);
	//texto
	BE_texto_fechar (PDF->Texto);
	//Células
	for (ListaL = PDF->_Celulas; ListaL; ListaL = g_slist_next (ListaL))
	{
		Celula = (BEPDF_Celula *) ListaL->data;
		BE_free (Celula->sCorBorda);
		BE_free (Celula->sCorFundo);
		BE_free (Celula);
	}
	BE_slist_free (PDF->_Celulas);
	//imagens
	for (ListaL = PDF->_Imagens; ListaL; ListaL = g_slist_next (ListaL))
	{
		Imagem = (BEPDF_Imagem *) ListaL->data;
		BE_free (Imagem);
	}
	BE_slist_free (PDF->_Imagens);

	//PDF
	BE_free (PDF);

	return NULL;
}

gboolean be_pdf_gravar (BEPDF *PDF, const gchar *sArquivo)
{
	BEPDF_Celula *Celula = NULL;
	BEPDF_Pagina *Pagina = NULL;
	BEPDF_Imagem *Imagem = NULL;
	BEFONTES *Fonte = NULL;
	FILE *File = NULL;
	GString *Buffer = NULL;
	GSList *XREF = NULL;
	GSList *ListaL = NULL;
	gint nCont = 0, nCont1 = 0;
	guint nQtObjetos = 0, nObjCatalogo = 0, nObjPaginas = 0, nObjFundo = 0, nObjFrente = 0, nXREF = 0;
	glong lLen = 0;
	gchar *sStream = NULL, *sTemp = NULL;
	gboolean bOK = FALSE;
	gsize nLen = 0;

	//preparando pagina modelo
	Pagina = PDF->Pagina;

	//Contando as páginas
	for (ListaL = PDF->_Celulas; ListaL; ListaL = g_slist_next (ListaL))
	{
		Celula = (BEPDF_Celula *) ListaL->data;
		if (Celula->nPagina > PDF->_lPaginas)
			PDF->_lPaginas = Celula->nPagina;
	}
#ifdef BEPDF_DEPURAR
	printf ("DEPURACAO: %s:%d - Celulas: %d - Paginas: %d\n", __FILE__, __LINE__, g_slist_length (PDF->_Celulas), PDF->_lPaginas);
#endif
	//Abrindo arquivo: Criando ou sobreescrevendo existente
	if ((File = fopen (sArquivo, "wb")))
	{
		//identificador
		BE_free (sTemp);
		sTemp = g_strdup_printf ("%%PDF-%.1f\n", BEPDF_ADOBE_VERSAO);
		nXREF = strlen (sTemp);
		fwrite (sTemp, nXREF, 1, File);
		//cabeçalho
		nQtObjetos++;
		Buffer = g_string_new ("");
		g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
		g_string_append_printf (Buffer, "   /Producer (%s)\n", Pagina->sProdutor);
		g_string_append_printf (Buffer, "   /Title (%s)\n", Pagina->sTitulo);
		g_string_append_printf (Buffer, "   /Author (%s)\n", Pagina->sAutor);
		g_string_append_printf (Buffer, "   /Creator (%s %.1f)\n", BEPDF_NOME, BEPDF_VERSAO);
		BE_free (sTemp);
		sTemp = BE_tempo ("%Y%m%d%H%M%S");
		g_string_append_printf (Buffer, "   /CreationDate (D:%s)\n", sTemp);
		g_string_append_printf (Buffer, "   /Subject (%s)\n", sArquivo);
		g_string_append_printf (Buffer, ">>\nendobj\n");
		lLen = Buffer->len;
		XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
		BE_free (sTemp);
		sTemp = BE_string_free (Buffer);
		fwrite (sTemp, lLen, 1, File);
		//fontes
		for (ListaL = PDF->Texto->Fontes; ListaL; ListaL = g_slist_next (ListaL))
		{
			Fonte = (BEFONTES *) ListaL->data;
			nQtObjetos++;
			Fonte->nObjeto = nQtObjetos;
			Buffer = g_string_new ("");
			BE_free (sStream);
			sStream = (gchar *) _be_pdf_fontes_info (PDF, Fonte, &lLen);
			g_string_append_len (Buffer, sStream, lLen);
			BE_free (sTemp);
			lLen = Buffer->len;
			XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
			BE_free (sTemp);
			sTemp = BE_string_free (Buffer);
			fwrite (sTemp, lLen, 1, File);
			//incluir fontes dentro do pdf
			nQtObjetos++;
			Buffer = g_string_new ("");
			g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
			BE_free (sStream);
			sStream = (gchar *) _be_pdf_fontes_pdf (PDF, Fonte, &lLen);
			g_string_append_printf (Buffer, "   /Length1 %d\n", (gint) lLen);//medida original
			if (BEPDF_COMPACTAR)
			{
				BE_free (sTemp);
				sTemp = sStream;
				sStream = (gchar *) be_comprimir (sTemp, (gulong *) &lLen);
				g_string_append_printf (Buffer, "   /Filter /FlateDecode\n");//compactação
			}
			g_string_append_printf (Buffer, "   /Length %d\n", (gint) lLen);//medida compactada
			g_string_append_printf (Buffer, ">>\n");
			g_string_append_printf (Buffer, "stream\n");
			g_string_append_len (Buffer, sStream, lLen);
			g_string_append_printf (Buffer, "endstream\n");
			g_string_append_printf (Buffer, "endobj\n");
			lLen = Buffer->len;
			XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
			BE_free (sTemp);
			sTemp = BE_string_free (Buffer);
			fwrite (sTemp, lLen, 1, File);
		}
#ifdef BEPDF_DEPURAR
		printf ("DEPURACAO: %s:%d - Fontes: %d\n", __FILE__, __LINE__, g_slist_length (PDF->Texto->Fontes));
#endif
		//imagens
		for (ListaL = PDF->_Imagens; ListaL; ListaL = g_slist_next (ListaL))
		{
			Imagem = (BEPDF_Imagem *) ListaL->data;
			nQtObjetos++;
			Imagem->nObjeto = nQtObjetos;
			Buffer = g_string_new ("");
			g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
			g_string_append_printf (Buffer, "   /Type /XObject\n");
			g_string_append_printf (Buffer, "   /Subtype /Image\n");
			g_string_append_printf (Buffer, "   /Width %d\n", gdk_pixbuf_get_width (Imagem->pixbuf));
			g_string_append_printf (Buffer, "   /Height %d\n", gdk_pixbuf_get_height (Imagem->pixbuf));
			g_string_append_printf (Buffer, "   /ColorSpace /DeviceRGB\n");
			g_string_append_printf (Buffer, "   /BitsPerComponent %d\n", gdk_pixbuf_get_bits_per_sample (Imagem->pixbuf));
			//sStream = be_pixbuf_get_pixels (Imagem->pixbuf);
			//sStream = gdk_pixbuf_get_pixels (Imagem->pixbuf);
			//salvar no formato JPEG
			BE_free (sStream);
			lLen = BE_pixbuf_get_len (Imagem->pixbuf);
			sStream = g_malloc (lLen * sizeof (gchar));
			if (gdk_pixbuf_save_to_buffer (Imagem->pixbuf, &sStream, &nLen, "jpeg", NULL, "quality", BEPDF_JPEG_QUALIDADE, NULL))
			{
				g_string_append_printf (Buffer, "   /Filter /DCTDecode\n");
				g_string_append_printf (Buffer, "   /Length %d\n", nLen);
				g_string_append_printf (Buffer, ">>\n");
				g_string_append_printf (Buffer, "stream\n");
				g_string_append_len (Buffer, sStream, nLen);
				g_string_append_printf (Buffer, "endstream\n");
			}
			else
			{
				//erro
				g_string_append_printf (Buffer, "   /Length 0\n");
				g_string_append_printf (Buffer, ">>\n");
			}

			g_string_append_printf (Buffer, "endobj\n");
			lLen = Buffer->len;
			XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
			BE_free (sTemp);
			sTemp = BE_string_free (Buffer);
			fwrite (sTemp, lLen, 1, File);
		}
#ifdef BEPDF_DEPURAR
		printf ("DEPURACAO: %s:%d - Imagens: %d\n", __FILE__, __LINE__, g_slist_length (PDF->_Imagens));
#endif
		//conteúdo do Cabeçalho, Rodapé: será repetido em na frente de todas as páginas
		nQtObjetos++;
		Buffer = g_string_new ("");
		g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
		BE_free (sStream);
		sStream = (gchar *) _be_pdf_celulas_pdf (PDF, 0, BEPDF_LOC_CABECALHO | BEPDF_LOC_RODAPE, &lLen);
		if (BEPDF_COMPACTAR)
		{
			BE_free (sTemp);
			sTemp = sStream;
			sStream = (gchar *) be_comprimir (sTemp, (gulong *) &lLen);
			g_string_append_printf (Buffer, "   /Filter /FlateDecode\n");
		}
		g_string_append_printf (Buffer, "   /Length %d\n", (gint) lLen);
		g_string_append_printf (Buffer, ">>\n");
		g_string_append_printf (Buffer, "stream\n");
		g_string_append_len (Buffer, sStream, lLen);
		g_string_append_printf (Buffer, "endstream\n");
		g_string_append_printf (Buffer, "endobj\n");
		lLen = Buffer->len;
		XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
		BE_free (sTemp);
		sTemp = BE_string_free (Buffer);
		fwrite (sTemp, lLen, 1, File);
		nObjFrente = nQtObjetos;

		//conteúdo do Fundo: será repetido atráz de todas as páginas
		nQtObjetos++;
		Buffer = g_string_new ("");
		g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
		BE_free (sStream);
		sStream = (gchar *) _be_pdf_celulas_pdf (PDF, 0, BEPDF_LOC_FUNDO, &lLen);
		if (BEPDF_COMPACTAR)
		{
			BE_free (sTemp);
			sTemp = sStream;
			sStream = (gchar *) be_comprimir (sTemp, (gulong *) &lLen);
			g_string_append_printf (Buffer, "   /Filter /FlateDecode\n");
		}
		g_string_append_printf (Buffer, "   /Length %d\n", (gint) lLen);
		g_string_append_printf (Buffer, ">>\n");
		g_string_append_printf (Buffer, "stream\n");
		g_string_append_len (Buffer, sStream, lLen);
		g_string_append_printf (Buffer, "endstream\n");
		g_string_append_printf (Buffer, "endobj\n");
		lLen = Buffer->len;
		XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
		BE_free (sTemp);
		sTemp = BE_string_free (Buffer);
		fwrite (sTemp, lLen, 1, File);
		nObjFundo = nQtObjetos;

		//índice de tópicos e seções: Só usaremos o primeiro tópico
		nQtObjetos++;
		Buffer = g_string_new ("");
		g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
		g_string_append_printf (Buffer, "   /Type /Catalog\n");
		g_string_append_printf (Buffer, "   /Pages %d 0 R\n", nQtObjetos + 1);
		g_string_append_printf (Buffer, ">>\nendobj\n");
		lLen = Buffer->len;
		XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
		BE_free (sTemp);
		sTemp = BE_string_free (Buffer);
		fwrite (sTemp, lLen, 1, File);
		nObjCatalogo = nQtObjetos;

		//índice das páginas
		nQtObjetos++;
		Buffer = g_string_new ("");
		g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
		g_string_append_printf (Buffer, "   /Type /Pages\n");
		g_string_append_printf (Buffer, "   /Kids [\n");
		for (nCont = 1; nCont <= PDF->_lPaginas; nCont++)
			g_string_append_printf (Buffer, "      %d 0 R\n", nQtObjetos + nCont);
		g_string_append_printf (Buffer, "   ]\n");
		g_string_append_printf (Buffer, "   /Count %d\n", (gint) PDF->_lPaginas);
		g_string_append_printf (Buffer, ">>\nendobj\n");
		lLen = Buffer->len;
		XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
		BE_free (sTemp);
		sTemp = BE_string_free (Buffer);
		fwrite (sTemp, lLen, 1, File);

		//páginas
		nObjPaginas = nQtObjetos;
		for (nCont = 1; nCont <= PDF->_lPaginas; nCont++)
		{
			nQtObjetos++;
			Buffer = g_string_new ("");
			g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
			g_string_append_printf (Buffer, "   /Type /Page\n");
			g_string_append_printf (Buffer, "   /Parent %d 0 R\n", nObjPaginas);
			g_string_append_printf (Buffer, "   /Resources\n   <<\n");
			g_string_append_printf (Buffer, "      /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
			g_string_append_printf (Buffer, "      /Font\n      <<\n");
			//loop nos textos
			for (ListaL = PDF->Texto->Fontes; ListaL; ListaL = g_slist_next (ListaL))
			{
				Fonte = (BEFONTES *) ListaL->data;
				BE_free (sTemp);
				sTemp = be_strreplace ((gchar *) Fonte->sNomeInterno, " ", "");
				g_string_append_printf (Buffer, "         /F%s %d 0 R\n", sTemp, Fonte->nObjeto);
			}
			g_string_append_printf (Buffer, "      >>\n");
			g_string_append_printf (Buffer, "      /XObject\n      <<\n");
			//loop nas imagens
			for (ListaL = PDF->_Imagens, nCont1 = 0; ListaL; ListaL = g_slist_next (ListaL), nCont1++)
			{
				Imagem = (BEPDF_Imagem *) ListaL->data;
				g_string_append_printf (Buffer, "         /Im%d %d 0 R\n", nCont1, Imagem->nObjeto);
			}
			g_string_append_printf (Buffer, "      >>\n");
			g_string_append_printf (Buffer, "   >>\n");
			g_string_append_printf (Buffer, "   /MediaBox [0 0 %.3f %.3f]\n", Pagina->fLargura, Pagina->fAltura);
			g_string_append_printf (Buffer, "   /Contents [\n");
			g_string_append_printf (Buffer, "      %d 0 R\n", nObjFundo);//fundo
			g_string_append_printf (Buffer, "      %d 0 R\n", (gint) (nQtObjetos + PDF->_lPaginas));//página
			g_string_append_printf (Buffer, "      %d 0 R\n", nObjFrente);//cabeçalho e rodapé
			g_string_append_printf (Buffer, "   ]\n");
			g_string_append_printf (Buffer, ">>\nendobj\n");
			lLen = Buffer->len;
			XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
			BE_free (sTemp);
			sTemp = BE_string_free (Buffer);
			fwrite (sTemp, lLen, 1, File);
		}
#ifdef BEPDF_DEPURAR
		printf ("DEPURACAO: %s:%d - Paginas processadas: %d\n", __FILE__, __LINE__, nCont);
#endif
		//conteúdo do corpo de cada página
		for (nCont = 0; nCont < PDF->_lPaginas; nCont++)
		{
			nQtObjetos++;
			Buffer = g_string_new ("");
			g_string_append_printf (Buffer, "%d 0 obj\n<<\n", nQtObjetos);
			BE_free (sStream);
			sStream = (gchar *) _be_pdf_celulas_pdf (PDF, nCont + 1, BEPDF_LOC_CORPO, &lLen);
			if (BEPDF_COMPACTAR)
			{
				BE_free (sTemp);
				sTemp = sStream;
				sStream = (gchar *) be_comprimir (sTemp, (gulong *) &lLen);
				g_string_append_printf (Buffer, "   /Filter /FlateDecode\n");
			}
			g_string_append_printf (Buffer, "   /Length %d\n", (gint) lLen);
			g_string_append_printf (Buffer, ">>\n");
			g_string_append_printf (Buffer, "stream\n");
			g_string_append_len (Buffer, sStream, lLen);
			g_string_append_printf (Buffer, "endstream\n");
			g_string_append_printf (Buffer, "endobj\n");
			lLen = Buffer->len;
			XREF = g_slist_append (XREF, g_strdup_printf ("%d", (gint) lLen));
			BE_free (sTemp);
			sTemp = BE_string_free (Buffer);
			fwrite (sTemp, lLen, 1, File);
		}
#ifdef BEPDF_DEPURAR
		printf ("DEPURACAO: %s:%d - Paginas processadas corpo: %d\n", __FILE__, __LINE__, nCont);
#endif
		//concluindo arquivo pdf
		Buffer = g_string_new ("");
		g_string_append_printf (Buffer, "xref\n");
		g_string_append_printf (Buffer, "0 %d\n", nQtObjetos + 1);
		g_string_append_printf (Buffer, "0000000000 65535 f \n");
		for (ListaL = XREF; ListaL; ListaL = g_slist_next (ListaL))
		{
			g_string_append_printf (Buffer, "%010d 00000 n \n", nXREF);
			nXREF += (gint) g_strtod ((gchar *) ListaL->data, NULL);
		}
		g_string_append_printf (Buffer, "trailer\n<<\n");
		g_string_append_printf (Buffer, "   /Size %d\n", nQtObjetos + 1);
		g_string_append_printf (Buffer, "   /Info 1 0 R\n");
		g_string_append_printf (Buffer, "   /Root %d 0 R\n", nObjCatalogo);
		g_string_append_printf (Buffer, ">>\n");
		g_string_append_printf (Buffer, "startxref\n");
		g_string_append_printf (Buffer, "%d\n", nXREF);
		g_string_append_printf (Buffer, "%%%%EOF\n");
		BE_free (sTemp);
		lLen = Buffer->len;
		sTemp = BE_string_free (Buffer);
		fwrite (sTemp, lLen, 1, File);
		//fechando arquivo
		fclose (File);

		bOK = TRUE;
	}
	else
		g_printerr ("BEPDF: Nao foi possivel gravar o arquivo %s\n.", sArquivo);
	//liberando memória
	for (ListaL = XREF; ListaL; ListaL = g_slist_next (ListaL))
	{
		sTemp = (gchar *) ListaL->data;
		BE_free (sTemp);
	}
	BE_slist_free (XREF);
	BE_free (sStream);
	BE_free (sTemp);

	return bOK;
}

guchar *_be_pdf_fontes_pdf (const BEPDF *PDF, const BEFONTES *Fonte, glong *lLen)
{
	FILE *File = NULL;
	const gint nBufferSIZE = BEGLOBAL_BLOCOS_ARQUIVO;
	guchar sBuffer[nBufferSIZE];
	gsize nSize = nBufferSIZE;
	GString *Buffer = NULL;

	//preparando buffer
	Buffer = g_string_new ("");
	//abrindo arquivo
	if ((File = fopen (Fonte->sArquivo, "rb")))
	{
		while (nSize == nBufferSIZE)
		{
			nSize = fread (&sBuffer, sizeof (guchar), nBufferSIZE, File);
			g_string_append_len (Buffer, (gchar *) sBuffer, nSize);
		}
		fclose (File);
	}
	//comprimento final
	*lLen = Buffer->len;

	return (guchar *) g_string_free (Buffer, FALSE);
}

guchar *_be_pdf_fontes_info (const BEPDF *PDF, const BEFONTES *Fonte, glong *lLen)
{
	GString *Buffer = NULL;
	gint nCont = 0;
	gchar *sTemp = NULL;
	gfloat fConversaoType1 = 0.0;

	//preparando buffer
	Buffer = g_string_new ("");
	//preparando conversão
	fConversaoType1 = ((gfloat) Fonte->nUnidade) / 1000.0;
	//iniciando fonte
	g_string_append_printf (Buffer, "%d 0 obj\n<<\n", Fonte->nObjeto);
	g_string_append_printf (Buffer, "   /Type /Font\n");
	if (Fonte->nUnidade == 2048)
		g_string_append_printf (Buffer, "   /Subtype /TrueType\n");
	else//type1 = 1000
		g_string_append_printf (Buffer, "   /Subtype /Type1\n");
	BE_free (sTemp);
	sTemp = be_strreplace ((gchar *) Fonte->sNomeInterno, " ", "");
	g_string_append_printf (Buffer, "   /Name /F%s\n", sTemp);
	g_string_append_printf (Buffer, "   /BaseFont /%s\n", Fonte->sNomeInterno);
	g_string_append_printf (Buffer, "   /Encoding /%s\n", BEPDF_LATIN_TEXT_ENCODING);
	g_string_append_printf (Buffer, "   /FirstChar 32\n");
	g_string_append_printf (Buffer, "   /LastChar 255\n");
	g_string_append_printf (Buffer, "   /Widths [ ");
	//limitado à tabela ASCII (32 - 255) extendida, pontos * nUnidade (xMin, xMax, yMin, yMax, Largura)
	for (nCont = 0; nCont < 223; nCont++)
		g_string_append_printf (Buffer, "%.0f ", ((gfloat) Fonte->nLetra[nCont][4]) / fConversaoType1);
	g_string_append_printf (Buffer, "]\n");
	//descritor
	g_string_append_printf (Buffer, "   /FontDescriptor\n   <<\n");
	//recuperando mais informações sobre a fonte
	g_string_append_printf (Buffer, "      /FontName /%s\n", Fonte->sNomeInterno);
	//caixa em pontos * nUnidade (xMin, xMax, yMin, yMax)
	g_string_append_printf (Buffer, "      /FontBBox [ %.0f %.0f %.0f %.0f ]\n", ((gfloat) Fonte->nCaixa[0]) / fConversaoType1, ((gfloat) Fonte->nCaixa[2]) / fConversaoType1, ((gfloat) Fonte->nCaixa[3]) / fConversaoType1, ((gfloat) Fonte->nCaixa[1]) / fConversaoType1);
	//ItalicAngle
	g_string_append_printf (Buffer, "      /ItalicAngle 0\n");
	//Ascent
	g_string_append_printf (Buffer, "      /Ascent %.0f\n", ((gfloat) Fonte->nAscent) / fConversaoType1);
	//Descent
	g_string_append_printf (Buffer, "      /Descent %.0f\n", ((gfloat) Fonte->nDescent) / fConversaoType1);
	//flags
	if (Fonte->nEstiloFlags & FT_STYLE_FLAG_ITALIC)
		g_string_append_printf (Buffer, "      /Flags %d\n", 96);
	else
		g_string_append_printf (Buffer, "      /Flags %d\n", 32);//Fonte->nFlags
	//CapHeight
	g_string_append_printf (Buffer, "      /CapHeight %.0f\n", ((gfloat) Fonte->nAscent) / fConversaoType1);
	//StemV
	if (Fonte->nEstiloFlags & FT_STYLE_FLAG_BOLD)
		g_string_append_printf (Buffer, "      /StemV 120\n");
	else
		g_string_append_printf (Buffer, "      /StemV 70\n");
	//tipo de arquivo
	if (Fonte->nUnidade == 2048)
		g_string_append_printf (Buffer, "      /FontFile2 %d 0 R\n", Fonte->nObjeto + 1);//ttf
	else
		g_string_append_printf (Buffer, "      /FontFile %d 0 R\n", Fonte->nObjeto + 1);//type1
	//fechando fonte
	g_string_append_printf (Buffer, "   >>\n");
	//fechando o objeto
	g_string_append_printf (Buffer, ">>\nendobj\n");
	//comprimento final
	*lLen = Buffer->len;
	//liberando
	BE_free (sTemp);

	return (guchar *) g_string_free (Buffer, FALSE);
}

BEPDF_Celula * be_pdf_nova_celula (BEPDF *PDF)
{
	BEPDF_Celula *Celula = NULL;
	BEPDF_Pagina *Pagina = NULL;

	Pagina = PDF->Pagina;
	Celula = g_new (BEPDF_Celula, 1);
	//valores padrão
	Celula->nPagina = 1;
	Celula->nLocal = BEPDF_LOC_CORPO;
	Celula->fX = 0.0;
	Celula->fY = 0.0;
	Celula->fXTexto = 0.0;
	Celula->fYTexto = 0.0;
	Celula->fLargura = 0.0;
	Celula->fAltura = 0.0;
	Celula->fBorEspessura = 0.0;
	Celula->nBorPontilhado = 0;
	Celula->nBorPosicao = BEPDF_BOR_RETANGULO;
	Celula->fAngInicial = 0.0;//círculo completo
	Celula->fAngFinal = 360.0;//círculo completo
	Celula->sCorBorda = g_strdup ("000000");//bordas preta
	Celula->sCorFundo = NULL;//sem cor de fundo
	Celula->nAlinhamento = BEPDF_ALI_NENHUM;
	Celula->fAngulo = 0.0;
	Celula->NoTexto = NULL;
	Celula->Imagem = NULL;
	//anexar
	PDF->_Celulas = g_slist_append (PDF->_Celulas, Celula);
	//corrigir coordenadas
	be_pdf_celula_corrigir (PDF, Celula);

	return Celula;
}

void be_pdf_celula_corrigir (const BEPDF *PDF, BEPDF_Celula *Celula)
{
	const BEPDF_Pagina *Pagina = NULL;
	gfloat fX = 0.0, fY = 0.0, fLargura = 0.0, fAltura = 0.0;

	Pagina = PDF->Pagina;
	//definir máximas na horizontal
	fX = Pagina->fMargEsquerda;
	fLargura = Pagina->fLargura - (Pagina->fMargEsquerda + Pagina->fMargDireita);
	//definir máximas na vertical
	if (Celula->nLocal == BEPDF_LOC_CORPO)
	{
		fY = Pagina->fMargSuperior + Pagina->fCabAltura;
		fAltura = Pagina->fAltura - (Pagina->fMargSuperior + Pagina->fMargInferior + Pagina->fCabAltura + Pagina->fRodAltura);
	}
	else if (Celula->nLocal == BEPDF_LOC_FUNDO)
	{
		fY = Pagina->fMargSuperior;
		fAltura = Pagina->fAltura - (Pagina->fMargSuperior + Pagina->fMargInferior);
	}
	else if (Celula->nLocal == BEPDF_LOC_CABECALHO)
	{
		fY = Pagina->fMargSuperior;
		fAltura = Pagina->fCabAltura;
	}
	else if (Celula->nLocal == BEPDF_LOC_RODAPE)
	{
		fY = Pagina->fAltura - (Pagina->fMargInferior + Pagina->fRodAltura);
		fAltura = Pagina->fRodAltura;
	}
	//posições negativas inverter as coordenadas
	if (Celula->fX < 0.0)
		Celula->fX = Pagina->fLargura + Celula->fX;
	if (Celula->fY < 0.0)
		Celula->fY = Pagina->fAltura + Celula->fY;
	//verificar posições relativas
	if (Celula->fX > fX + fLargura)
		Celula->fX = (fX + fLargura) - Celula->fLargura;
	if (Celula->fX < fX)
		Celula->fX += fX;
	if (Celula->fY > fY + fAltura)
		Celula->fY = (fY + fAltura) - Celula->fAltura;
	if (Celula->fY < fY)
		Celula->fY += fY;
	//dimensões negativas inverter valores
	if (Celula->fLargura < 0.0)
		Celula->fLargura = (fX + fLargura) + (Celula->fLargura - Celula->fX);
	if (Celula->fAltura < 0.0)
		Celula->fAltura = (fY + fAltura) + (Celula->fAltura - Celula->fY);
	//verificar dimensões máximas
	if (Celula->fX + Celula->fLargura > fX + fLargura || Celula->fLargura == 0.0)
		Celula->fLargura = (fX + fLargura) - Celula->fX;
	if (Celula->fY + Celula->fAltura > fY + fAltura || Celula->fAltura == 0.0)
		Celula->fAltura = (fY + fAltura) - Celula->fY;
	//verificar limites possíveis
	if (Celula->fBorEspessura <= 0.0)
		Celula->fBorEspessura = 0.5;
	if (Celula->nBorPontilhado < 0)
		Celula->nBorPontilhado = 0;
	if (Celula->fAngInicial < -360.0 || Celula->fAngInicial > 360.0)
		Celula->fAngInicial = 0.0;
	if (Celula->fAngFinal < -360.0 || Celula->fAngFinal > 360.0)
		Celula->fAngFinal = 360.0;
	if (Celula->fAngulo < -360.0 || Celula->fAngulo > 360.0)
		Celula->fAngulo = 0.0;
}

guchar *_be_pdf_celulas_pdf (const BEPDF *PDF, const guint nPagina, const BEPDF_LOC_ENUM nLocal, glong *lLen)
{
	const BEPDF_Pagina *Pagina = NULL;
	const BEXML_No_Elemento *NoElemento = NULL;
	BEPDF_Celula *Celula = NULL;
	BEPDF_Imagem *Imagem = NULL;
	_BEPDF_Buffer_Texto *BTexto = NULL;
	GString *Buffer = NULL, *BufferSublinhado = NULL;
	GSList *ListaL = NULL;
	gfloat fX = 0.0, fY = 0.0, fLar = 0.0, fAlt = 0.0, fXraio = 0.0, fYraio = 0.0, fAngI_rad = 0.0, fAngF_rad = 360.0, fAngIAtual = 0.0, fDT = 0.0, fDTM = 0.0;//gráficos
	gfloat fXaAnt = 0.0, fYaAnt = 0.0, fXbAnt = 0.0, fYbAnt = 0.0, fXa = 0.0, fYa = 0.0, fXb = 0.0, fYb = 0.0;//gráficos
	gfloat fhyp = 0.0, fAngTotal = 0.0, fLcNovo = 0.0, fAcNovo = 0.0;
	gfloat fTemp = 0.0;
	gchar *sTemp = NULL;
	gint nCont = 0;

	Pagina = PDF->Pagina;
	//preparando o buffer
	Buffer = g_string_new ("");
	//Contando as células
	for (ListaL = PDF->_Celulas; ListaL; ListaL = g_slist_next (ListaL))
	{
		Celula = (BEPDF_Celula *) ListaL->data;
		//somente as células selecionadas
		if (((Celula->nLocal & nLocal) && nPagina == 0) || ((Celula->nLocal & nLocal) && Celula->nPagina == nPagina))
		{
			//conversão de coordenadas
			Celula->fY = Pagina->fAltura - Celula->fY;
			fX = Celula->fX;
			fY = Celula->fY;
			fLar = Celula->fLargura;
			fAlt = -Celula->fAltura;

			//marcar ponto de restauração
			g_string_append_printf (Buffer, "q\n");
			//cor das bordas
			if (Celula->sCorBorda && strlen (Celula->sCorBorda) == 6)
			{
				BE_free (sTemp);
				sTemp = be_substr (Celula->sCorBorda, 0, 2);
				g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				BE_free (sTemp);
				sTemp = be_substr (Celula->sCorBorda, 2, 2);
				g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				BE_free (sTemp);
				sTemp = be_substr (Celula->sCorBorda, 4, 2);
				g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				g_string_append_printf (Buffer, "RG\n");
			}
			//cor de fundo
			if (Celula->sCorFundo && strlen (Celula->sCorFundo) == 6)
			{
				BE_free (sTemp);
				sTemp = be_substr (Celula->sCorFundo, 0, 2);
				g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				BE_free (sTemp);
				sTemp = be_substr (Celula->sCorFundo, 2, 2);
				g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				BE_free (sTemp);
				sTemp = be_substr (Celula->sCorFundo, 4, 2);
				g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				g_string_append_printf (Buffer, "rg\n");
			}
			//ângulo de inclinação geral
			if (Celula->fAngulo != 0.0)
			{
				//super complicado
				fXa = cos(Celula->fAngulo * BEPDF_DEG_RAD);
				fYa = sin(Celula->fAngulo * BEPDF_DEG_RAD);
				fLcNovo = (fXa * fLar + cos((90.0 - Celula->fAngulo) * BEPDF_DEG_RAD) * -fAlt) / 2.0;
				fAcNovo = (fYa * fLar + sin((90.0 - Celula->fAngulo) * BEPDF_DEG_RAD) * -fAlt) / 2.0;
				fhyp = sqrt(pow(fX, 2) + pow(fY, 2));
				fAngTotal = atan(fY / fX) + Celula->fAngulo * BEPDF_DEG_RAD;
				//Interpolação CTM:
				//Os 4 primeiros valores definem o angulo da inclinação: cos, sen, -sen, cos, do ângulo.
				//Os 2 últimos a posição X, Y.
				//O problema é que a inclinação é sempre apartir do ponto zero da página, e não do desenho atual. Se pudesse mudar para o centro do desenho seria mais fácil.
				//Então, quando se inclina uma página retangular, os desenhos posteriores também serão inclinados e movidos, pois eles não estão no ponto zero da página.
				//O segredo é calcular o quanto eles se moveram e trazer devolta, centralizando com a posição original.
				//Pura trigonometria.
				//O resultado é excelente!
				g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f %.3f %.3f cm\n", fXa , fYa, -fYa, fXa, (fX - cos(fAngTotal) * fhyp) + (fLar / 2.0 - fLcNovo), (fY - (sin(fAngTotal) * fhyp + fYa * fLar)) + fAcNovo + fAlt / 2.0);
			}
			//espessura da linha
			if (Celula->sCorBorda && strlen (Celula->sCorBorda) == 6)
			{
				g_string_append_printf (Buffer, "%.3f w\n", Celula->fBorEspessura);
				//pontilhamento da linha
				if (Celula->nBorPontilhado > 0)
					g_string_append_printf (Buffer, "[%d %d] 0 d\n", Celula->nBorPontilhado, Celula->nBorPontilhado);
			}
			//elipses, circulos e pizzas
			if (Celula->nBorPosicao == BEPDF_BOR_ELIPSE)
			{
				//posições iniciais
				fX += fLar / 2.0;
				fY += fAlt / 2.0;
				fXraio = fLar / 2.0;
				fYraio = fAlt / 2.0 * - 1.0;
				//conversão para radianos
				fAngI_rad = Celula->fAngInicial * BEPDF_DEG_RAD;
				fAngF_rad = Celula->fAngFinal * BEPDF_DEG_RAD;
				//posições angulares
				fDT = (fAngF_rad - fAngI_rad) / BEPDF_ELIPSE_SEG;//divisão de segmentos
				fDTM = fDT / BEPDF_ELIPSE_DT;// ?
				fAngIAtual = fAngI_rad;
				fXaAnt = fX + (fXraio * cos(fAngIAtual));
				fYaAnt = fY + (fYraio * sin(fAngIAtual));
				fXbAnt = -fXraio * sin(fAngIAtual);
				fYbAnt = fYraio * cos(fAngIAtual);
				//posição inicial da elipse
				g_string_append_printf (Buffer, "%.3f %.3f m\n", fXaAnt, fYaAnt);
				for (nCont = 1; nCont <= BEPDF_ELIPSE_SEG; nCont++)
				{
					fAngIAtual = (nCont * fDT) + fAngI_rad;
					fXa = fX + (fXraio * cos(fAngIAtual));
					fYa = fY + (fYraio * sin(fAngIAtual));
					fXb = -fXraio * sin(fAngIAtual);
					fYb = fYraio * cos(fAngIAtual);
					g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f %.3f %.3f c\n", fXaAnt + (fXbAnt * fDTM), fYaAnt + (fYbAnt * fDTM), fXa - (fXb * fDTM), fYa - (fYb * fDTM), fXa, fYa);
					fXaAnt = fXa;
					fYaAnt = fYa;
					fXbAnt = fXb;
					fYbAnt = fYb;
				}
				if ((fAngF_rad - fAngI_rad) < 2 * M_PI)//posição central da elipse
					g_string_append_printf (Buffer, "%.3f %.3f l\n", fX, fY);
				if (Celula->sCorBorda && Celula->sCorFundo && strlen (Celula->sCorFundo) == 6 && strlen (Celula->sCorBorda) == 6)
					g_string_append_printf (Buffer, "b\n");
				else if (Celula->sCorBorda && strlen (Celula->sCorBorda) == 6)
					g_string_append_printf (Buffer, "s\n");
				else if (Celula->sCorFundo && strlen (Celula->sCorFundo) == 6)
					g_string_append_printf (Buffer, "f\n");
			}
			//linhas inclinadas 1
			else if (Celula->sCorBorda && Celula->nBorPosicao == BEPDF_BOR_INCLINADA1 && strlen (Celula->sCorBorda) == 6)
			{
				//posição inicial da linha
				g_string_append_printf (Buffer, "%.3f %.3f m\n", fX, fY);
				//posição final da linha
				g_string_append_printf (Buffer, "%.3f %.3f l\n", fX + fLar, fY + fAlt);
				//finalizando a linha
				g_string_append_printf (Buffer, "S\n");
			}
			//linhas inclinadas 2
			else if (Celula->sCorBorda && Celula->nBorPosicao == BEPDF_BOR_INCLINADA2 && strlen (Celula->sCorBorda) == 6)
			{
				//posição inicial da linha
				g_string_append_printf (Buffer, "%.3f %.3f m\n", fX, fY + fAlt);
				//posição final da linha
				g_string_append_printf (Buffer, "%.3f %.3f l\n", fX + fLar, fY);
				//finalizando a linha
				g_string_append_printf (Buffer, "S\n");
			}
			//retângulo inclinado
			else if (Celula->nBorPosicao == BEPDF_BOR_RETANGULO && Celula->fAngInicial > 0.0)
			{
				//posições raio das bordas
				fXraio = Celula->fAngInicial;
				//posição inicial da borda
				g_string_append_printf (Buffer, "%.3f %.3f m\n", fX + fXraio, fY);
				//borda superior
				fXa = fX + fLar - fXraio;
				fYa = fY - fXraio;
				g_string_append_printf (Buffer, "%.3f %.3f l\n", fXa, fY);
				g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f %.3f %.3f c\n", fXa + (fXraio * BEPDF_ELIPSE_ARC), fYa + fXraio, fXa + fXraio, fYa + (fXraio * BEPDF_ELIPSE_ARC), fXa + fXraio, fYa);
				//borda direita
				fXa = fX + fLar - fXraio ;
				fYa = fY + fAlt + fXraio;
				g_string_append_printf (Buffer, "%.3f %.3f l\n", fX + fLar, fYa);
				g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f %.3f %.3f c\n", fXa + fXraio, fYa - (fXraio * BEPDF_ELIPSE_ARC), fXa + (fXraio * BEPDF_ELIPSE_ARC), fYa - fXraio, fXa, fYa - fXraio);
				//borda inferior
				fXa = fX + fXraio;
				fYa = fY + fAlt + fXraio;
				g_string_append_printf (Buffer, "%.3f %.3f l\n", fXa, fY + fAlt);
				g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f %.3f %.3f c\n", fXa - (fXraio * BEPDF_ELIPSE_ARC), fYa - fXraio, fXa - fXraio, fYa - (fXraio * BEPDF_ELIPSE_ARC), fXa - fXraio, fYa);
				//borda esquerda
				fXa = fX + fXraio;
				fYa = fY - fXraio;
				g_string_append_printf (Buffer, "%.3f %.3f l\n", fX, fYa);
				g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f %.3f %.3f c\n", fXa - fXraio, fYa + (fXraio * BEPDF_ELIPSE_ARC), fXa - (fXraio * BEPDF_ELIPSE_ARC), fYa + fXraio, fXa, fYa + fXraio);
				//finalizando a borda
				if (Celula->sCorBorda && Celula->sCorFundo && strlen (Celula->sCorFundo) == 6 && strlen (Celula->sCorBorda) == 6)
					g_string_append_printf (Buffer, "b\n");
				else if (Celula->sCorBorda && strlen (Celula->sCorBorda) == 6)
					g_string_append_printf (Buffer, "s\n");
				else if (Celula->sCorFundo && strlen (Celula->sCorFundo) == 6)
					g_string_append_printf (Buffer, "f\n");
			}
			//retângulo padrão
			else
			{
				//fundo
				if (Celula->sCorFundo && strlen (Celula->sCorFundo) == 6)
						g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f re f\n", fX, fY, fLar, fAlt);
				//bordas
				if (Celula->sCorBorda && strlen (Celula->sCorBorda) == 6)
				{
					if (Celula->nBorPosicao == BEPDF_BOR_RETANGULO)//otimização para retângulos
						g_string_append_printf (Buffer, "%.3f %.3f %.3f %.3f re S\n", fX, fY, fLar, fAlt);
					else if (Celula->nBorPosicao > BEPDF_BOR_NENHUMA && Celula->nBorPosicao < BEPDF_BOR_RETANGULO)//linhas individuais
					{
						//posição inicial da borda
						g_string_append_printf (Buffer, "%.3f %.3f m\n", fX, fY);
						//borda superior
						g_string_append_printf (Buffer, "%.3f %.3f ", fX + fLar, fY);
						if (Celula->nBorPosicao & BEPDF_BOR_SUPERIOR)
							g_string_append_printf (Buffer, "l\n");
						else
							g_string_append_printf (Buffer, "m\n");
						//borda direita
						g_string_append_printf (Buffer, "%.3f %.3f ", fX + fLar, fY + fAlt);
						if (Celula->nBorPosicao & BEPDF_BOR_DIREITA)
							g_string_append_printf (Buffer, "l\n");
						else
							g_string_append_printf (Buffer, "m\n");
						//borda inferior
						g_string_append_printf (Buffer, "%.3f %.3f ", fX, fY + fAlt);
						if (Celula->nBorPosicao & BEPDF_BOR_INFERIOR)
							g_string_append_printf (Buffer, "l\n");
						else
							g_string_append_printf (Buffer, "m\n");
						//borda esquerda
						g_string_append_printf (Buffer, "%.3f %.3f ", fX, fY);
						if (Celula->nBorPosicao & BEPDF_BOR_ESQUERDA)
							g_string_append_printf (Buffer, "l\n");
						else
							g_string_append_printf (Buffer, "m\n");
						//finalizando a borda
						g_string_append_printf (Buffer, "S\n");
					}
				}
			}
			//fechar o pontilhamento da linha
			if (Celula->sCorBorda && Celula->nBorPontilhado > 0 && strlen (Celula->sCorBorda) == 6)
				g_string_append_printf (Buffer, "[] 0 d\n");

			//imagem
			if ((Imagem = Celula->Imagem))
			{
				//redimensionar imagem para não sofrer esticamentos
				//o zoom + vem primeiro e o zoom - corrigi, pois a imagem pode ficar menor que a célula, mas não maior.
				if (Imagem->bRedimensionar)
				{
					fLar = gdk_pixbuf_get_width (Imagem->pixbuf);
					fAlt = gdk_pixbuf_get_height (Imagem->pixbuf);
					//ZOOM +
					//a imagem é menos curta que a célula
					if (fLar < Celula->fLargura)
					{
						fAlt = fAlt * Celula->fLargura / fLar;//diminuir proporcionalemnet a altura da célula
						fLar = Celula->fLargura;
					}
					//a imagem é menos alta que a célula
					if (fAlt < Celula->fAltura)
					{
						fLar = fLar * Celula->fAltura / fAlt;//diminuir proporcionalemnet a largura da célula
						fAlt = Celula->fAltura;
					}
					//ZOOM -
					//a imagem é mais larga que a célula
					if (fLar > Celula->fLargura)
					{
						fAlt = fAlt * Celula->fLargura / fLar;//diminuir proporcionalemnet a altura da célula
						fLar = Celula->fLargura;
					}
					//a imagem é mais alta que a célula
					if (fAlt > Celula->fAltura)
					{
						fLar = fLar * Celula->fAltura / fAlt;//diminuir proporcionalemnet a largura da célula
						fAlt = Celula->fAltura;
					}
					//incluir imagem na célula
					g_string_append_printf (Buffer, "q %.3f 0 0 %.3f %.3f %.3f cm /Im%d Do Q\n", fLar - 2 * Celula->fXTexto, fAlt - 2 * Celula->fYTexto, Celula->fX + Celula->fXTexto, Celula->fY - (Celula->fYTexto + fAlt - 2 * Celula->fYTexto), Imagem->nImagem);
				}
				else
					//incluir imagem na célula
					g_string_append_printf (Buffer, "q %.3f 0 0 %.3f %.3f %.3f cm /Im%d Do Q\n", Celula->fLargura - 2 * Celula->fXTexto, Celula->fAltura - 2 * Celula->fYTexto, Celula->fX + Celula->fXTexto, Celula->fY - (Celula->fYTexto + Celula->fAltura - 2 * Celula->fYTexto), Imagem->nImagem);
			}

			//texto
			if (Celula->NoTexto)
			{
				//sublinhado
				BufferSublinhado = g_string_new ("");
				g_string_append_printf (BufferSublinhado, "%.3f w\n", BEPDF_SUBLINHADO_ESPESSURA);
				//iniciar
				BTexto = g_new (_BEPDF_Buffer_Texto, 1);
				BTexto->Celula = Celula;
				BTexto->Texto = PDF->Texto;
				BTexto->Buffer = Buffer;
				BTexto->BufferSublinhado = BufferSublinhado;
				BTexto->nFonte = -1;
				BTexto->fTamanho = 0.0;
				BTexto->sCor = NULL;
				BTexto->bSobreescrito = FALSE;
				BTexto->bSubeescrito = FALSE;
				BTexto->nLinha = 0;
				BTexto->fX = 0.0;
				BTexto->fY = 0.0;
				BTexto->fXSublinhado = 0.0;
				//iniciar bloco de texto
				g_string_append_printf (Buffer, "BT\n");//início do bloco
				//verificar alinhamento vertical
				if (Celula->nAlinhamento & BEPDF_ALI_MEDIO)
				{
					NoElemento = (BEXML_No_Elemento *) Celula->NoTexto->children->data;
					fTemp = g_strtod (be_xml_no_atributos_valor (NoElemento->Atributos, "altura"), NULL);
					Celula->fYTexto = Celula->fAltura / 2.0 - fTemp / 2.0;
				}
				else if (Celula->nAlinhamento & BEPDF_ALI_INFERIOR)
				{
					NoElemento = (BEXML_No_Elemento *) Celula->NoTexto->children->data;
					fTemp = g_strtod (be_xml_no_atributos_valor (NoElemento->Atributos, "altura"), NULL);
					Celula->fYTexto = Celula->fAltura - fTemp;
				}
				g_string_append_printf (Buffer, "%.3f %.3f Td\n", Celula->fX + Celula->fXTexto, Celula->fY - Celula->fYTexto);//posição inicial
				//loop
				g_node_traverse (Celula->NoTexto, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_pdf_texto_loop, BTexto);
				//finalizar do bloco
				g_string_append_printf (Buffer, "\nET\n");
				//anexar sublinhado ao fim do texto
				BE_free (sTemp);
				sTemp = BE_string_free (BufferSublinhado);
				g_string_append_printf (Buffer, "%s", sTemp);
				//liberando
				BE_free (BTexto);
			}

			//restaurar marcação anterior
			g_string_append_printf (Buffer, "Q\n");
		}
	}
	//comprimento final
	*lLen = Buffer->len;
	//liberando
	BE_free (sTemp);

	return (guchar *) g_string_free (Buffer, FALSE);
}

gboolean _be_pdf_texto_loop (GNode *node, gpointer data)
{
	_BEPDF_Buffer_Texto *BTexto = NULL;
	BEXML_No_Elemento *NoElemento = NULL, *NoElementoPai = NULL;
	const BEPDF_Celula *Celula = NULL;
	GNode *NoPai = NULL;
	const BEFONTES *Fonte = NULL;
	GString *Buffer = NULL, *BufferSublinhado = NULL;
	const gchar *sTamanho = NULL, *sCor = NULL, *sValor = NULL;
	gint nFonte = 0;
	gchar *sTemp = NULL;
	gfloat fTamanho = 0.0, fX = 0.0, fY = 0.0, fLargura = 0.0, fLarguraLinha = 0.0, fLarguraBloco = 0.0;
	gfloat fJustificado = 0.0, fAjustado = 0.0;
	gint nLinha = 0, nEspacos = 0;
	gint nCont = 0;
	gboolean bSobreescrito = FALSE, bSubeescrito = FALSE, bSublinhado = FALSE;

	//inicializando
	NoElemento = (BEXML_No_Elemento*) node->data;
	BTexto = (_BEPDF_Buffer_Texto *) data;
	Buffer = BTexto->Buffer;
	BufferSublinhado = BTexto->BufferSublinhado;
	Celula = BTexto->Celula;
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
		//procurar por uma cor, ancestral
		NoPai = be_xml_no_atributo (node->parent, "font", "color", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sCor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "color");//procurar atributo
		//procurar por uma sobrescrito, ancestral
		NoPai = be_xml_no_nome (node->parent, "sup", BEXML_LOCALIZAR_PAI);
		if (NoPai)
			bSobreescrito = TRUE;
		//procurar por uma subescrito, ancestral
		NoPai = be_xml_no_nome (node->parent, "sub", BEXML_LOCALIZAR_PAI);
		if (NoPai)
			bSubeescrito = TRUE;
		//procurar por uma subescrito, ancestral
		NoPai = be_xml_no_nome (node->parent, "u", BEXML_LOCALIZAR_PAI);
		if (NoPai)
			bSublinhado = TRUE;

		//recuperar linha
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "linha");
		nLinha = (gint) g_strtod (sValor, NULL);
		//recuperar largura
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "largura");
		fLargura = g_strtod (sValor, NULL);

		//verificar fonte atual
		if (BTexto->nFonte != nFonte || BTexto->fTamanho != fTamanho)
		{
			//localizar fonte real
			if (nFonte >= 0 && nFonte < g_slist_length (BTexto->Texto->Fontes))
			{
				//fonte real
				Fonte = (BEFONTES *) g_slist_nth_data (BTexto->Texto->Fontes, nFonte);
				BE_free (sTemp);
				sTemp = be_strreplace ((gchar *) Fonte->sNomeInterno, " ", "");
				//alterar fonte atual
				BTexto->nFonte = nFonte;
				BTexto->fTamanho = fTamanho;
				g_string_append_printf (Buffer, "/F%s %.3f Tf ", sTemp, fTamanho);
			}
		}
		//verificar cor atual
		if (!BTexto->sCor || (!g_str_equal (BTexto->sCor, sCor) && strlen (sCor) == 6))
		{
			//alterar cor atual
			BTexto->sCor = sCor;
			BE_free (sTemp);
			sTemp = be_substr (sCor, 0, 2);
			g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
			BE_free (sTemp);
			sTemp = be_substr (sCor, 2, 2);
			g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
			BE_free (sTemp);
			sTemp = be_substr (sCor, 4, 2);
			g_string_append_printf (Buffer, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
			g_string_append_printf (Buffer, "rg ");
		}
		//verificar linha atual
		if (BTexto->nLinha != nLinha)
		{
			//recuperar X
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
			fX = g_strtod (sValor, NULL);
			//procurar linha
				BE_free (sTemp);
				sTemp = g_strdup_printf ("%d", nLinha);
				NoPai = be_xml_no_valor ((g_node_get_root (node))->children, "linha", sTemp, BEXML_LOCALIZAR_IRMAO);
				NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
				//recuperar Y da linha
				sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "y");//procurar atributo
				fY = g_strtod (sValor, NULL);
				//recuperar largura da linha
				sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "largura");//procurar atributo
				fLarguraLinha = g_strtod (sValor, NULL);
				//recuperar quantidade de espaços da linha
				sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "espacos");//procurar atributo
				nEspacos = (gint) g_strtod (sValor, NULL);
			//procurar bloco
				NoPai = be_xml_no_nome ((g_node_get_root (node))->children, "bloco", BEXML_LOCALIZAR_IRMAO);
				NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
				//recuperar largura do bloco
				sValor = be_xml_no_atributos_valor (NoElementoPai->Atributos, "largura original");//procurar atributo
				fLarguraBloco = g_strtod (sValor, NULL);

			//verificar alinhamento horizontal
			if (Celula->nAlinhamento & BEPDF_ALI_CENTRO)
			{
				BTexto->fXSublinhado = fLarguraBloco / 2.0 - fLarguraLinha / 2.0;
				fX += BTexto->fXSublinhado;
			}
			else if (Celula->nAlinhamento & BEPDF_ALI_DIREITO)
			{
				BTexto->fXSublinhado = fLarguraBloco - fLarguraLinha;
				fX += BTexto->fXSublinhado;
			}
			else if (Celula->nAlinhamento & BEPDF_ALI_JUSTIFICADO && nEspacos > 0)
			{
				fJustificado = (fLarguraBloco - fLarguraLinha) / (gfloat) nEspacos;
				g_string_append_printf (Buffer, "%.3f Tw ", fJustificado);
			}
			else if (Celula->nAlinhamento & BEPDF_ALI_AJUSTADO)
			{
				fAjustado = (100.0 * fLarguraBloco) / fLarguraLinha;
				g_string_append_printf (Buffer, "%.3f Tz ", fAjustado);
			}
			else
				BTexto->fXSublinhado = 0.0;

			//alterar linha atual
			g_string_append_printf (Buffer, "%.3f -%.3f TD ", fX - BTexto->fX, fY - BTexto->fY);
			BTexto->fY = fY;
			BTexto->fX = fX;
			BTexto->nLinha = nLinha;
		}
		//verificar sobrescrito : sup
		if (BTexto->bSobreescrito != bSobreescrito && !BTexto->bSubeescrito)
		{
			//alterar a altura da linha
			if (bSobreescrito)
				g_string_append_printf (Buffer, "%.3f Ts", (fTamanho / BEPDF_SUBSOB_ESCRITO) * BEPDF_SOBRESCRITO);
			else
				g_string_append_printf (Buffer, "0 Ts ");

			BTexto->bSobreescrito = bSobreescrito;
		}
		//verificar subescrito : sub
		if (BTexto->bSubeescrito != bSubeescrito && !BTexto->bSobreescrito)
		{
			//alterar a altura da linha
			if (bSubeescrito)
				g_string_append_printf (Buffer, "%.3f Ts", (fTamanho / BEPDF_SUBSOB_ESCRITO) * BEPDF_SUBESCRITO);
			else
				g_string_append_printf (Buffer, "0 Ts ");

			BTexto->bSubeescrito = bSubeescrito;
		}
		//iniciar sublinhado
		if (bSublinhado)
		{
			//recuperar x
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
			fX = g_strtod (sValor, NULL);
			//Y da linha
			fY = BTexto->fY;
			//cor das bordas
			if (strlen (sCor) == 6)
			{
				BE_free (sTemp);
				sTemp = be_substr (sCor, 0, 2);
				g_string_append_printf (BufferSublinhado, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				BE_free (sTemp);
				sTemp = be_substr (sCor, 2, 2);
				g_string_append_printf (BufferSublinhado, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				BE_free (sTemp);
				sTemp = be_substr (sCor, 4, 2);
				g_string_append_printf (BufferSublinhado, "%.3f ", ((gfloat) be_cast_hex_long (sTemp)) / 255.0);
				g_string_append_printf (BufferSublinhado, "RG ");
			}
			//linha do sublinhado
			g_string_append_printf (BufferSublinhado, "%.3f %.3f %.3f 0.000 re s\n", Celula->fX + Celula->fXTexto + fX + BTexto->fXSublinhado, (Celula->fY - Celula->fYTexto) - (fY + BEPDF_SUBLINHADO_ESPACAMENTO), fLargura);
		}

		//convertendo caractéres reservados (escape PDF)
		g_string_append_printf (Buffer, "(");
		for (nCont = 0; nCont < strlen (NoElemento->sValor); nCont++)
		{
			if (NoElemento->sValor[nCont] == '(')//abre colchetes
				g_string_append_printf (Buffer, "\\(");
			else if (NoElemento->sValor[nCont] == ')')//fecha colchetes
				g_string_append_printf (Buffer, "\\)");
			else if (NoElemento->sValor[nCont] == '\\')//barra invertida
				g_string_append_printf (Buffer, "\\\\");
			else//sem conversão
				Buffer = g_string_append_c (Buffer, NoElemento->sValor[nCont]);
		}
		g_string_append_printf (Buffer, ") Tj ");
	}
	else if (g_str_equal (NoElemento->sNome, "space"))
	{
		//procurar por um tamanho de fonte, ancestral
		NoPai = be_xml_no_atributo (node->parent, NULL, "size", BEXML_LOCALIZAR_PAI);
		NoElementoPai = (BEXML_No_Elemento *) NoPai->data;
		sTamanho = be_xml_no_atributos_valor (NoElementoPai->Atributos, "size");//procurar atributo
		fTamanho = g_strtod (sTamanho, NULL);

		//recuperar largura
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "largura");
		fLargura = g_strtod (sValor, NULL);
		if (fLargura > 0.0)
			g_string_append_printf (Buffer, "[-%.3f] TJ ", fLargura * 1000.0 / fTamanho);
	}
	//limpar
	BE_free (sTemp);

	return FALSE;
}

BEPDF_Imagem *be_pdf_nova_imagem (BEPDF *PDF, GdkPixbuf *pixbuf, gboolean bRedimensionar)
{
	BEPDF_Imagem *Imagem = NULL;

	if (pixbuf)
	{
		//criar imagem
		Imagem = g_new (BEPDF_Imagem, 1);
		Imagem->pixbuf = pixbuf;
		Imagem->nObjeto = 0;
		Imagem->bRedimensionar = bRedimensionar;
		//anexar na lista
		PDF->_Imagens = g_slist_append (PDF->_Imagens, Imagem);
		//identificar posição
		Imagem->nImagem = g_slist_length (PDF->_Imagens) - 1;

		return Imagem;
	}
	else
		return NULL;
}
