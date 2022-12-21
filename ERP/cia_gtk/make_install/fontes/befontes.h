/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEFontes                                                                     |
+------------------------------------------------------------------------------+
| BEFontes � uma biblioteca livre, voc� pode redistribu�-la e/ou               |
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

#ifndef _BEFONTES_H
	#define _BEFONTES_H
	#include "../global/beglobal.h"
	#include <ft2build.h>
	#include <pango/pangoft2.h>
	#include FT_FREETYPE_H
	#include FT_GLYPH_H
	//verificar vers�o do FreeType2
	#ifdef __FREETYPE_H__
		#ifndef FREETYPE_VERSION
			#define FREETYPE_VERSION (FREETYPE_MAJOR * 10000 + FREETYPE_MINOR * 100 + FREETYPE_PATCH)
		#endif
		#if FREETYPE_VERSION < 20107
			#error "VERSAO DO FREETYPE DEVE SER >= 2.1.7"
		#endif
	#endif
	
	#define BEFONTES_BLOCOS_ARQ 1024
	#ifdef G_OS_UNIX
		#define BEFONTES_COMANDO_LOCALIZAR "ls -R"
		#define BEFONTES_ENTER "\n"
		#define BEFONTES_Font1 "/usr/share/fonts"
		#define BEFONTES_Font2 "/usr/X11R6/lib/X11/fonts"
		#define BEFONTES_PADRAO "bitstream vera sans"
	#endif
	#ifdef G_OS_WIN32
		//#define BEFONTES_COMANDO_LOCALIZAR "cmd /C dir /D"
		//#define BEFONTES_COMANDO_LOCALIZAR "dir /D"
		#define BEFONTES_COMANDO_LOCALIZAR "ls -R"
		#define BEFONTES_ENTER "\r\n"
		#define BEFONTES_Font1 "C:\\WINDOWS\\fonts"
		#define BEFONTES_Font2 "C:\\WINNT\\fonts"
		#define BEFONTES_PADRAO "arial"
	#endif
	
	typedef struct//Objeto Fonte
	{
		guchar *sNome;//nome da fonte
		guchar *sNomeInterno;//nome interno da fonte
		guchar *sEstilo;//estilo da fonte
		gchar *sEstiloUsual;//estilo usual da fonte
		gchar *sArquivo;//arquivo real da fonte
		gint nUnidade;//unidade das medidas: 2048 para TTF e 1000 para Type1
		gint nLetra[224][5];//limitado � tabela ASCII (32 - 255) estendida, pontos * nUnidade (xMin, xMax, yMin, yMax, Largura)
		gint nCaixa[4];//caixa em pontos * nUnidade (xMin, xMax, yMin, yMax)
		gint nLarguraMax;
		gint nAlturaMax;//altura da linha
		gint nObjeto;//n�mero do objeto dentro de um PDF (reservado);
		gint nEstiloFlags;// FT_STYLE_FLAG_ITALIC, FT_STYLE_FLAG_BOLD
		gint nFlags;
		gint nAscent;
		gint nDescent;
	} BEFONTES;
	
	//Prot�tipos
	void be_fontes_lista (const gchar *sArquivoFonte, void (*func_Info)(const gchar *sMensagens, gpointer user_data), gpointer user_data);
	BEFONTES *be_fontes_carregar (const gchar *sArquivoFonte, const gchar *sNome, const gchar *sEstilo);//sNome para primeira fonte localizada em sArquivoFonte; faz tentativas
	BEFONTES *_be_fontes_carregar (const gchar *sArquivoFonte, const gchar *sNome, const gchar *sEstilo);
	void be_fontes_descarregar (BEFONTES *Fonte);
	gint be_fontes_largura (BEFONTES *Fonte, const gchar *sTexto);
	void be_fontes_altura (BEFONTES *Fonte, const gchar *sTexto, gint *yMin, gint *yMax);
	//Prot�tipos reservados
	gchar *_be_fontes_extrair (FT_Library FTlib, const gchar *sArquivo);
#endif
