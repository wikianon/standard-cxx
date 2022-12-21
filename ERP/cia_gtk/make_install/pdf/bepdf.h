/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEPDF                                                                        |
+------------------------------------------------------------------------------+
| BEPDF é uma biblioteca livre, você pode redistribuí-la e/ou                  |
| modificá-la dentro dos termos da Licença Pública Geral GNU como              |
| publicada pela Fundação do Software Livre (FSF); na versão 2 da              |
| Licença, ou (na sua opnião) qualquer versão.                                 |
| Este programa é distribuido na esperança que possa ser útil,                 |
| mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÃO a qualquer |
| MERCADO ou APLICAÇÃO EM PARTICULAR.                                          |
| Veja a Licença Pública Geral GNU para maiores detalhes.                      |
| Você deve ter recebido uma cópia da Licença Pública Geral GNU                |
| junto com este programa, se não, escreva para a Fundação do Software         |
| Livre(FSF) Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     |
+------------------------------------------------------------------------------+
*/
#ifndef _BEPDF_H
	#define _BEPDF_H
	#include "../global/beglobal.h"
	#include "../comprimir/becomprimir.h"
	#include "../texto/betexto.h"

	//definições
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

	//Posições relativas
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
	//Objeto Célula
	typedef struct
	{
		guint nPagina;//página atual
		BEPDF_LOC_ENUM nLocal;//posição relativa: corpo, cabeçalho, rodapé ou fundo
		gfloat fX;//posição absoluta, em pontos
		gfloat fY;//posição absoluta, em pontos
		gfloat fXTexto;//posição relativa à fX, padrão é 0
		gfloat fYTexto;//posição relativa à fY, padrão é 0
		gfloat fLargura;//posição em pontos
		gfloat fAltura;//posição em pontos
		gfloat fBorEspessura;//espessura da borda
		gint nBorPontilhado;//espessura do ponto
		BEPDF_BOR_ENUM nBorPosicao;//tipo de borda
		gfloat fAngInicial;//para nBorPosicao = BEPDF_BOR_ELIPSE server como ângulo inicial: padrão = 0 :: para nBorPosicao = BEPDF_BOR_RETANGULO serve como raio das curvas dos cantos
		gfloat fAngFinal;//para nBorPosicao = BEPDF_BOR_ELIPSE server como ângulo final: padrão = 360
		gchar *sCorBorda;//em hexadecimal, 6 dígitos
		gchar *sCorFundo;//em hexadecimal, 6 dígitos
		BEPDF_ALI_ENUM nAlinhamento;//alinhamento do texto
		gfloat fAngulo;//ângulo da célula,padrão é 0
		GNode *NoTexto;//nó do processamento de texto
		BEPDF_Imagem *Imagem;//imagem
	} BEPDF_Celula;
	//Objeto Página
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
		const BEPDF_Celula *Celula;//célula atual
		gint nFonte;//índice da fonte atual
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
		BEPDF_Pagina *Pagina;//página, todas as páginas são iguais
		BETEXTO *Texto;//processamento do texto de todas células
		gulong _lPaginas;//quantidade de páginas definidas nas células
		GSList *_Celulas;//aponta para (BEPDF_Celula *)
		GSList *_Imagens;//aponta para (BEPDF_Imagem *)
	} BEPDF;

	//Protótipos
	BEPDF *be_pdf_novo (const gchar *sArquivoFonte, const gchar *sFontePadrao, const gchar *sEstiloPadrao, const gfloat fTamanhoPadrao);
	#define BE_pdf_pagina_padrao(p,t,a,d) be_pdf_pagina(p,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,t,a,d)
	BEPDF_Pagina *be_pdf_pagina (//alteração na pagina: valores menor ou igual à 0 zerão ignorados
		BEPDF *PDF,
		gfloat fAltura,//altura da página: o padrão é BEPDF_A4_ALTURA
		gfloat fLargura,//largura da página: o padrão é BEPDF_A4_LARGURA
		gfloat fMargEsquerda,//margens
		gfloat fMargDireita,
		gfloat fMargSuperior,
		gfloat fMargInferior,
		gfloat fCabAltura,//cabeçalho
		gfloat fRodAltura,//rodapé
		const gchar *sTitulo,//comprimento máximo de BEPDF_ADOBE_NOMES_LEN caractéres
		const gchar *sAutor,//comprimento máximo de BEPDF_ADOBE_NOMES_LEN caractéres
		const gchar *sProdutor);//comprimento máximo de BEPDF_ADOBE_NOMES_LEN caractéres
	gboolean be_pdf_gravar (BEPDF *PDF, const gchar *sArquivo);//gerar o PDF, a extensão ".pdf" não é incluída automaticamente
	#define BE_pdf_fechar(PDF); PDF=_be_pdf_fechar(PDF);
	BEPDF_Celula *be_pdf_nova_celula (BEPDF *PDF);
	BEPDF_Imagem *be_pdf_nova_imagem (BEPDF *PDF, GdkPixbuf *pixbuf, gboolean bRedimensionar);
	void be_pdf_celula_corrigir (const BEPDF *PDF, BEPDF_Celula *Celula);//corrigir valores da célula com valores válidos, e deixando a mesma dentro de seus limites
	//Protótipos reservados
	BEPDF *_be_pdf_fechar (BEPDF *PDF);
	guchar *_be_pdf_fontes_pdf (const BEPDF *PDF, const BEFONTES *Fonte, glong *lLen);//inclui arquivo da fonte no PDF
	guchar *_be_pdf_fontes_info (const BEPDF *PDF, const BEFONTES *Fonte, glong *lLen);//processa e inclui informações da fonte
	guchar *_be_pdf_celulas_pdf (const BEPDF *PDF, const guint nPagina, const BEPDF_LOC_ENUM nLocal, glong *lLen);//processa e inclui as células
	gboolean _be_pdf_texto_loop (GNode *node, gpointer data);//loop recursivo, dos Nós

	//passar BEPDF *PDF para Constante, mas para isto deve ter um outro método de guardar os objetos
	/* Próximas atualizações:
		- Tabela simplificada (união fácil de várias células)
		- Código de barras EAN13
		- Gráficos
		- Possibilidade de utilizar SVG para criar e editar os relatórios. Talvez criar algumas macros.
	*/
	/* NOTAS:
	-> Utilizado como manual de referência o Adobe Portable Document Format Version 1.3, e FPDF 1.53 de Olivier PLATHEY.
	-> Para utilizar qualquer medida de coordenada ou dimensão em milímetros, fazer: mm * BEPDF_PIXEL_POR_POLEGADA
	-> Para qualquer valor float a precisão será arredondada para 3 casas decimais.
	-> Para escrever os textos são permitidos as seguintes tags:
		Nova Linha		<br>
		Fonte:			<font face=nome da fonte>
		Tamanho:		<font size=tamanho da fonte>
		Sublinhado:		<u> e </u>
		Negrito:		<b> e </b>
		Itálico:		<i> e </i>
		Sobrescrito:	<sup> e </sup>
		Subescrito:		<sub> e </sub>
		Cor:			<font color=(RRGGBB em hexadecimal)>
		Caractéres		Utilizar \xHH dentro da string (HH = hexadecimal)
	-> O controle das páginas fica por sua conta
	-> O sublinhado não pode ser utilizado com o alinhamento justificado ou ajustado
	-> Somente serão aceitos caractéres de 32 À 255 da tabela ASCII, os demais serão ignorados
	-> Ainda não possui tratamento direto para UTF8 */

	//#define BEPDF_DEPURAR
#endif
