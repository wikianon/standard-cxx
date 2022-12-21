#include "bepdf.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extendida
	be_global_abrir (&argc, &argv, 0);

	#ifdef G_OS_UNIX
		#define TESTE "testespdf.pdf"
	#endif
	#ifdef G_OS_WIN32
		#define TESTE "testespdfwin32.pdf"
	#endif

	BEPDF *PDF = NULL;
	BEPDF_Celula *Celula = NULL;
	BEPDF_Pagina *Pagina = NULL;
	GdkPixbuf *Imagem = NULL, *Imagem1 = NULL, *Imagem2 = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	gfloat fTemp = 0.0;
	#ifdef G_OS_UNIX
		const gchar *sArquivoFonte = "fontes";
	#endif
	#ifdef G_OS_WIN32
		//const gchar *sArquivoFonte = "../fontes/suporte/fontes.txt";
		const gchar *sArquivoFonte = "fontes.txt";
	#endif
	const gchar *sExpressao = {"<font face='times new roman' size='20'>Tais funções</font><font color='AAAAAA'> que falamos, básicas, fazem parte da biblioteca C padrão (citada acima). Todo compilador C a possui, ele faz parte da padronização ANSI C. Seu compilador, independente do sistema que você utiliza, deve possuir. Outras bibliotecas a mais, além das padronizadas pelo ANSI, também vem junto com seu compilador porém não é recomendado a utilização, caso você queira escrever TAB programas portáveis (que rode em todas as plataformas). Podemos aqui citar programação gráfica, de rede, etc como casos que são 'perigosos' para programação portável. <u>Não estou dizendo que você não deve programar para estas áreas, futuramente haverão tutoriais para essas áreas por aqui, porém deve atentar-se que tal programação é peculiar a plataforma que você está <i>utilizando</i> e fazer algo <b>padronizado</b> portável torna-se quase impossível. É necessário se desassociar a stream ao arquivo físico ao se terminar um programa. Utilizamos da função fclose para isso.Ele ainda descarregar obuffer(como citado acima) e garante que o arquivo foi fechado de forma correta, pelo menos pelo programa. Uma falha fechando uma stream pode causar vários danos a integridade dos dados. Além disso, ele libera o ponteiro para o tipo <font size='20'>FILE</font> associado ao arquivo, que poderá ser utilizado para outros arquivos. O protótipo é: int fclose( FILE *stream); A stream citada acima é o óbvio ponteiro para o tipo FILE. Repare que a função retorna um inteiro, que significa o código da operação. Caso retorno 0, significa como tradicionalmente, que a operação foi bem sucedida. <font color='FF0000'>Caso contrário houveram erros, como disco cheio, corrompido ou </font>inexistente. <font color='0000FF'>Além de diversos outros problemas que podem ter ocorrido.<font face='times new roman' style='italico'> Vamos nos aprofundar nisso depois.2</font><sup>10</sup> Base<sub>10</sub> Certo</font></u></font>"};
	const gchar *sExpressao1 = {"<font face='times new roman' size='20'>Tais funções</font> que falamos, básicas, fazem parte da biblioteca C padrão (citada acima).<br>Todo compilador C a possui, ele faz parte da padronização ANSI C. Seu compilador, independente do sistema que você utiliza, deve possuir. Outras bibliotecas a mais, além das padronizadas pelo ANSI, também vem junto com seu compilador porém não é recomendado a utilização, caso você queira escrever TAB programas portáveis (que rode em todas as plataformas). Podemos aqui citar programação gráfica, de rede, etc como casos que são 'perigosos' para programação portável.<br>Não estou dizendo que você não deve programar para estas áreas, futuramente haverão tutoriais para essas áreas por aqui, porém deve atentar-se que tal programação é peculiar a plataforma que você está <i>utilizando</i> e fazer algo <b>padronizado</b> portável torna-se quase impossível. É necessário se desassociar a stream ao arquivo físico ao se terminar um programa.<br>Utilizamos da função fclose para isso.Ele ainda descarregar obuffer(como citado acima) e garante que o arquivo foi fechado de forma correta, pelo menos pelo programa. Uma falha fechando uma stream pode causar vários danos a integridade dos dados.<br>Além disso, ele libera o ponteiro para o tipo <font size='20'>FILE</font> associado ao arquivo, que poderá ser utilizado para outros arquivos. O protótipo é: int fclose( FILE *stream); A stream citada acima é o óbvio ponteiro para o tipo FILE. Repare que a função retorna um inteiro, que significa o código da operação. Caso retorno 0, significa como tradicionalmente, que a operação foi bem sucedida. <font color='FF0000'>Caso contrário houveram erros, como disco cheio, corrompido ou </font>inexistente. <font color='0000FF'>Além de diversos outros problemas que podem ter ocorrido.<font face='times new roman' style='italico'> Vamos nos aprofundar nisso depois.2</font><sup>10</sup> Base<sub>10</sub> Certo</font>"};


	#ifdef G_OS_UNIX
		PDF = be_pdf_novo (sArquivoFonte, "bitstream vera sans", NULL, 10.0);// testar com 60 e ver a largura do texto
	#endif
	#ifdef G_OS_WIN32
		PDF = be_pdf_novo (sArquivoFonte, "arial", NULL, 10.0);// testar com 60 e ver a largura do texto
	#endif
	if (PDF)
	{
		Pagina = BE_pdf_pagina_padrao (PDF, "Testes de impressão", "ProVisual - Programação Profissional e Visual", "CIA - Controle Integrado Administrativo v2.0");

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->fYTexto = 5.0;
		Celula->fXTexto = 5.0;
		Celula->NoTexto = be_texto_analisar (PDF->Texto, sExpressao, 400.0, 100.0, TRUE, FALSE);//texto da célula

		Imagem = gdk_pixbuf_new_from_file ("Novo Vectra.jpg", NULL);
		Celula->Imagem = be_pdf_nova_imagem (PDF, Imagem, TRUE);
		if (Celula->NoTexto)
		{
			NoElemento = (BEXML_No_Elemento *) Celula->NoTexto->children->data;
			fTemp = g_strtod (be_xml_no_atributos_valor (NoElemento->Atributos, "largura"), NULL);
			Celula->fLargura = fTemp + 10.0;
			fTemp = g_strtod (be_xml_no_atributos_valor (NoElemento->Atributos, "altura"), NULL);
			Celula->fAltura = fTemp + 10.0;
		}
		//exibir
		/*if (Celula->NoTexto)
			g_node_traverse (Celula->NoTexto, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);
		printf ("\n");*/

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 2;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("FF0000");
		Celula->NoTexto = be_texto_analisar (PDF->Texto, sExpressao, 500.0, 100.0, TRUE, FALSE);//texto da célula

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 2;
		Celula->fY = 100.0;
		Celula->fYTexto = 1.0;
		Celula->fXTexto = 1.0;
		Imagem1 = gdk_pixbuf_new_from_file ("logo.png", NULL);
		//Imagem1 = gdk_pixbuf_new_from_file ("home11.jpg", NULL);
		Celula->Imagem = be_pdf_nova_imagem (PDF, Imagem1, TRUE);
		Celula->fLargura = 50;//gdk_pixbuf_get_width (Imagem1);
		Celula->fAltura = 50;//gdk_pixbuf_get_height (Imagem1);

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 3;
		Celula->fY = 100.0;
		Celula->fLargura = 300.0;
		Celula->fAltura = 300.0;
		Imagem2 = gdk_pixbuf_new_from_file ("kurumin.jpg", NULL);
		Celula->Imagem = be_pdf_nova_imagem (PDF, Imagem2, TRUE);
		Celula->fBorEspessura = 2.0;

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 4;
		Celula->fLargura = 410.0;
		Celula->fAltura = 400.0;
		Celula->fX = Pagina->fLargura / 2.0 - Celula->fLargura / 2.0;
		Celula->fY = Pagina->fAltura / 2.0 - Celula->fAltura / 2.0;
		Celula->fYTexto = 5.0;
		Celula->fXTexto = 5.0;
		Celula->NoTexto = be_texto_analisar (PDF->Texto, sExpressao, 400.0, 100.0, TRUE, FALSE);//texto da célula
		Celula->nAlinhamento = BEPDF_ALI_CENTRO + BEPDF_ALI_MEDIO;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("FF0000");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEEEE");
		Celula->fAngInicial = 10.0;
		Celula->fAngulo = 3.0;
		Celula->fBorEspessura = 3.0;

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 5;
		Celula->fLargura = 410.0;
		Celula->fAltura = 380.0;
		Celula->fX = Pagina->fLargura / 2.0 - Celula->fLargura / 2.0;
		Celula->fYTexto = 5.0;
		Celula->fXTexto = 5.0;
		Celula->NoTexto = be_texto_analisar (PDF->Texto, sExpressao1, 400.0, 100.0, TRUE, FALSE);//texto da célula
		Celula->nAlinhamento = BEPDF_ALI_JUSTIFICADO + BEPDF_ALI_MEDIO;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("00FF00");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEEEE");
		Celula->fAngInicial = 10.0;
		Celula->fBorEspessura = 3.0;
		Celula->nBorPontilhado = 5;

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 5;
		Celula->fLargura = 410.0;
		Celula->fAltura = 400.0;
		Celula->fY = 420.0;
		Celula->fX = Pagina->fLargura / 2.0 - Celula->fLargura / 2.0;
		Celula->fYTexto = 5.0;
		Celula->fXTexto = 5.0;
		Celula->NoTexto = be_texto_analisar (PDF->Texto, sExpressao1, 400.0, 100.0, TRUE, FALSE);//texto da célula
		Celula->nAlinhamento = BEPDF_ALI_AJUSTADO + BEPDF_ALI_INFERIOR;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("00FF00");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEEEE");
		Celula->fAngInicial = 10.0;
		Celula->fBorEspessura = 3.0;
		Celula->nBorPontilhado = 5;

		//pizzas
		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nLocal = BEPDF_LOC_FUNDO;
		Celula->nPagina = 6;
		Celula->fLargura = 200.0;
		Celula->fAltura = 100.0;
		Celula->nBorPosicao = BEPDF_BOR_ELIPSE;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("0000FF");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEEEE");
		Celula->fAngFinal = 180.0;

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nLocal = BEPDF_LOC_CABECALHO;
		Celula->nPagina = 6;
		Celula->fLargura = 200.0;
		Celula->fAltura = 100.0;
		Celula->nBorPosicao = BEPDF_BOR_ELIPSE;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("FF0000");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEE00");
		Celula->fAngInicial = 200.0;

		//linhas
		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 6;
		Celula->fY = 200.0;
		Celula->fLargura = 200.0;
		Celula->fAltura = 200.0;
		Celula->nBorPosicao = BEPDF_BOR_ELIPSE;
		BE_free (Celula->sCorBorda);
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("AAAAFF");

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 6;
		Celula->fLargura = 200.0;
		Celula->fAltura = 100.0;
		Celula->nBorPosicao = BEPDF_BOR_INCLINADA1;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("FF0000");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEE00");

		Celula = be_pdf_nova_celula (PDF);//nova célula
		Celula->nPagina = 6;
		Celula->fLargura = 200.0;
		Celula->fAltura = 100.0;
		Celula->nBorPosicao = BEPDF_BOR_INCLINADA2;
		BE_free (Celula->sCorBorda); Celula->sCorBorda = g_strdup ("FF0000");
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("EEEE00");

		Celula = be_pdf_nova_celula (PDF);//nova célula
		BE_free (Celula->sCorFundo); Celula->sCorFundo = g_strdup ("FF0000");
		Celula->fX = 5.0;
		Celula->fLargura = -5.0;
		Celula->fY = 5.0;
		Celula->fAltura = -5.0;
		Celula->nLocal = BEPDF_LOC_CABECALHO;
		be_pdf_celula_corrigir (PDF, Celula);

		be_pdf_gravar (PDF, TESTE);
		BE_pdf_fechar (PDF);
	}
	BE_pixbuf_free (Imagem);
	BE_pixbuf_free (Imagem1);
	BE_pixbuf_free (Imagem2);

	#ifdef G_OS_UNIX
		system (g_strdup_printf ("xpdf %s", TESTE));
		//system(g_strdup_printf ("kpdf %s", TESTE));
		//system(g_strdup_printf ("acroread %s", TESTE));
	#endif
	#ifdef G_OS_WIN32
		system (g_strdup_printf ("\"D:\\Arquivos de Programas\\Foxit Software\\Foxit Reader\\Foxit Reader.exe\" %s", TESTE));
	#endif

	//finalizar biblioteca extendida
	be_global_fechar ();
	return 0;
}
