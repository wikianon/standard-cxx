/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEPDF                                                                        |
+------------------------------------------------------------------------------+
| BEPDF � uma biblioteca livre, voc� pode redistribu�-la e/ou                  |
| modific�-la dentro dos termos da Licen�a P�blica Geral GNU como              |
| publicada pela Funda��o do Software Livre (FSF); na vers�o 2 da              |
| Licen�a, ou (na sua opni�o) qualquer vers�o.                                 |
| Este programa � distribuido na esperan�a que possa ser �til,                 |
| mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUA��O a qualquer |
| MERCADO ou APLICA��O EM PARTICULAR.                                          |
| Veja a Licen�a P�blica Geral GNU para maiores detalhes.                      |
| Voc� deve ter recebido uma c�pia da Licen�a P�blica Geral GNU                |
| junto com este programa, se n�o, escreva para a Funda��o do Software         |
| Livre(FSF) Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     |
+------------------------------------------------------------------------------+
*/
#ifndef _BEPDF_H
	#define _BEPDF_H
	#include "../global/beglobal.h"
	#include "../comprimir/becomprimir.h"
	#include "../texto/betexto.h"

	//defini��es
	#define BEPDF_ADOBE_VERSAO 1.3
	#define BEPDF_ADOBE_NOMES_LEN 64
	#define BEPDF_VERSAO 1.1
	#define BEPDF_NOME "Bibliotecas Estendidas - PDF"
	#define BEPDF_PIXEL_POR_POLEGADA 72.0 / 25.4
	#define BEPDF_A4_ALTURA 297.0 * BEPDF_PIXEL_POR_POLEGADA
	#define BEPDF_A4_LARGURA 210.0 * BEPDF_PIXEL_POR_POLEGADA
	#define BEPDF_SUBLINHADO_ESPESSURA 0.25
	#define BEPDF_SUBLINHADO_ESPACAMENTO 1
	#define BEPDF_SUBSOB_ESCRITO 0.58
	#define BEPDF_SOBRESCRITO 0.33
	#define BEPDF_SUBESCRITO -0.20
	#define BEPDF_LATIN_TEXT_ENCODING "WinAnsiEncoding"
	#define BEPDF_DEG_RAD M_PI / 180.0
	#define BEPDF_ELIPSE_SEG 4
	#define BEPDF_ELIPSE_DT 3.0
	#define BEPDF_ELIPSE_ARC 4.0 / 3.0 * (M_SQRT2 - 1)
	#define BEPDF_COMPACTAR TRUE
	#define BEPDF_JPEG_QUALIDADE "85"

	//Posi��es relativas
	typedef enum
	{
		BEPDF_LOC_DESCONHECIDO = 0,
		BEPDF_LOC_CORPO = 1,
		BEPDF_LOC_CABECALHO = 2,
		BEPDF_LOC_RODAPE = 4,
		BEPDF_LOC_FUNDO = 8,
	} BEPDF_LOC_ENUM;
	//Tipos de bordas
	typedef enum
	{
		BEPDF_BOR_NENHUMA = 0,
		BEPDF_BOR_SUPERIOR = 1,
		BEPDF_BOR_DIREITA = 2,
		BEPDF_BOR_INFERIOR = 4,
		BEPDF_BOR_ESQUERDA = 8,
		BEPDF_BOR_RETANGULO = BEPDF_BOR_SUPERIOR | BEPDF_BOR_DIREITA | BEPDF_BOR_INFERIOR | BEPDF_BOR_ESQUERDA,
		BEPDF_BOR_ELIPSE = 16,
		BEPDF_BOR_INCLINADA1 = 32,
		BEPDF_BOR_INCLINADA2 = 64
	} BEPDF_BOR_ENUM;
	//Alinhamentos
	typedef enum
	{
		BEPDF_ALI_NENHUM = 0,
		BEPDF_ALI_CENTRO = 1,
		BEPDF_ALI_DIREITO = 2,
		BEPDF_ALI_JUSTIFICADO = 4,
		BEPDF_ALI_AJUSTADO = 8,
		BEPDF_ALI_MEDIO = 16,
		BEPDF_ALI_INFERIOR = 32
	} BEPDF_ALI_ENUM;

	//Objeto Imagem JPEG
	typedef struct
	{
		GdkPixbuf *pixbuf;//constante
		guint nObjeto;
		gboolean bRedimensionar;
		gint nImagem;//imagem
	} BEPDF_Imagem;
	//Objeto C�lula
	typedef struct
	{
		guint nPagina;//p�gina atual
		BEPDF_LOC_ENUM nLocal;//posi��o relativa: corpo, cabe�alho, rodap� ou fundo
		gfloat fX;//posi��o absoluta, em pontos
		gfloat fY;//posi��o absoluta, em pontos
		gfloat fXTexto;//posi��o relativa � fX, padr�o � 0
		gfloat fYTexto;//posi��o relativa � fY, padr�o � 0
		gfloat fLargura;//posi��o em pontos
		gfloat fAltura;//posi��o em pontos
		gfloat fBorEspessura;//espessura da borda
		gint nBorPontilhado;//espessura do ponto
		BEPDF_BOR_ENUM nBorPosicao;//tipo de borda
		gfloat fAngInicial;//para nBorPosicao = BEPDF_BOR_ELIPSE server como �ngulo inicial: padr�o = 0 :: para nBorPosicao = BEPDF_BOR_RETANGULO serve como raio das curvas dos cantos
		gfloat fAngFinal;//para nBorPosicao = BEPDF_BOR_ELIPSE server como �ngulo final: padr�o = 360
		gchar *sCorBorda;//em hexadecimal, 6 d�gitos
		gchar *sCorFundo;//em hexadecimal, 6 d�gitos
		BEPDF_ALI_ENUM nAlinhamento;//alinhamento do texto
		gfloat fAngulo;//�ngulo da c�lula,padr�o � 0
		GNode *NoTexto;//n� do processamento de texto
		BEPDF_Imagem *Imagem;//imagem
	} BEPDF_Celula;
	//Objeto P�gina
	typedef struct
	{
		gfloat fAltura;
		gfloat fLargura;
		gfloat fMargEsquerda;
		gfloat fMargDireita;
		gfloat fMargSuperior;
		gfloat fMargInferior;
		gfloat fCabAltura;
		gfloat fRodAltura;
		gfloat fRodEspaco;
		gchar sTitulo[BEPDF_ADOBE_NOMES_LEN];
		gchar sAutor[BEPDF_ADOBE_NOMES_LEN];
		gchar sProdutor[BEPDF_ADOBE_NOMES_LEN];
	} BEPDF_Pagina;
	//Objeto buffer do texto
	typedef struct
	{
		const BEPDF_Celula *Celula;//c�lula atual
		gint nFonte;//�ndice da fonte atual
		const BETEXTO *Texto;//atual
		GString *Buffer;//buffer atual
		GString *BufferSublinhado;//buffer atual
		gfloat fTamanho;//tamanho da fonte atual
		const gchar *sCor;//atual
		gboolean bSobreescrito;//sup atual
		gboolean bSubeescrito;//sub atual
		gint nLinha;//linha atual
		gfloat fX;//atual
		gfloat fY;//atual
		gfloat fXSublinhado;//atual
	} _BEPDF_Buffer_Texto;
	//Estrutura principal
	typedef struct
	{
		BEPDF_Pagina *Pagina;//p�gina, todas as p�ginas s�o iguais
		BETEXTO *Texto;//processamento do texto de todas c�lulas
		gulong _lPaginas;//quantidade de p�ginas definidas nas c�lulas
		GSList *_Celulas;//aponta para (BEPDF_Celula *)
		GSList *_Imagens;//aponta para (BEPDF_Imagem *)
	} BEPDF;

	//Prot�tipos
	BEPDF *be_pdf_novo (const gchar *sArquivoFonte, const gchar *sFontePadrao, const gchar *sEstiloPadrao, const gfloat fTamanhoPadrao);
	#define BE_pdf_pagina_padrao(p,t,a,d) be_pdf_pagina(p,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,t,a,d)
	BEPDF_Pagina *be_pdf_pagina (//altera��o na pagina: valores menor ou igual � 0 zer�o ignorados
		BEPDF *PDF,
		gfloat fAltura,//altura da p�gina: o padr�o � BEPDF_A4_ALTURA
		gfloat fLargura,//largura da p�gina: o padr�o � BEPDF_A4_LARGURA
		gfloat fMargEsquerda,//margens
		gfloat fMargDireita,
		gfloat fMargSuperior,
		gfloat fMargInferior,
		gfloat fCabAltura,//cabe�alho
		gfloat fRodAltura,//rodap�
		const gchar *sTitulo,//comprimento m�ximo de BEPDF_ADOBE_NOMES_LEN caract�res
		const gchar *sAutor,//comprimento m�ximo de BEPDF_ADOBE_NOMES_LEN caract�res
		const gchar *sProdutor);//comprimento m�ximo de BEPDF_ADOBE_NOMES_LEN caract�res
	gboolean be_pdf_gravar (BEPDF *PDF, const gchar *sArquivo);//gerar o PDF, a extens�o ".pdf" n�o � inclu�da automaticamente
	#define BE_pdf_fechar(PDF); PDF=_be_pdf_fechar(PDF);
	BEPDF_Celula *be_pdf_nova_celula (BEPDF *PDF);
	BEPDF_Imagem *be_pdf_nova_imagem (BEPDF *PDF, GdkPixbuf *pixbuf, gboolean bRedimensionar);
	void be_pdf_celula_corrigir (const BEPDF *PDF, BEPDF_Celula *Celula);//corrigir valores da c�lula com valores v�lidos, e deixando a mesma dentro de seus limites
	//Prot�tipos reservados
	BEPDF *_be_pdf_fechar (BEPDF *PDF);
	guchar *_be_pdf_fontes_pdf (const BEPDF *PDF, const BEFONTES *Fonte, glong *lLen);//inclui arquivo da fonte no PDF
	guchar *_be_pdf_fontes_info (const BEPDF *PDF, const BEFONTES *Fonte, glong *lLen);//processa e inclui informa��es da fonte
	guchar *_be_pdf_celulas_pdf (const BEPDF *PDF, const guint nPagina, const BEPDF_LOC_ENUM nLocal, glong *lLen);//processa e inclui as c�lulas
	gboolean _be_pdf_texto_loop (GNode *node, gpointer data);//loop recursivo, dos N�s

	//passar BEPDF *PDF para Constante, mas para isto deve ter um outro m�todo de guardar os objetos
	/* Pr�ximas atualiza��es:
		- Tabela simplificada (uni�o f�cil de v�rias c�lulas)
		- C�digo de barras EAN13
		- Gr�ficos
		- Possibilidade de utilizar SVG para criar e editar os relat�rios. Talvez criar algumas macros.
	*/
	/* NOTAS:
	-> Utilizado como manual de refer�ncia o Adobe Portable Document Format Version 1.3, e FPDF 1.53 de Olivier PLATHEY.
	-> Para utilizar qualquer medida de coordenada ou dimens�o em mil�metros, fazer: mm * BEPDF_PIXEL_POR_POLEGADA
	-> Para qualquer valor float a precis�o ser� arredondada para 3 casas decimais.
	-> Para escrever os textos s�o permitidos as seguintes tags:
		Nova Linha		<br>
		Fonte:			<font face=nome da fonte>
		Tamanho:		<font size=tamanho da fonte>
		Sublinhado:		<u> e </u>
		Negrito:		<b> e </b>
		It�lico:		<i> e </i>
		Sobrescrito:	<sup> e </sup>
		Subescrito:		<sub> e </sub>
		Cor:			<font color=(RRGGBB em hexadecimal)>
		Caract�res		Utilizar \xHH dentro da string (HH = hexadecimal)
	-> O controle das p�ginas fica por sua conta
	-> O sublinhado n�o pode ser utilizado com o alinhamento justificado ou ajustado
	-> Somente ser�o aceitos caract�res de 32 � 255 da tabela ASCII, os demais ser�o ignorados
	-> Ainda n�o possui tratamento direto para UTF8 */

	//#define BEPDF_DEPURAR
#endif
