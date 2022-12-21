/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEFontes                                                                     |
+------------------------------------------------------------------------------+
| BEFontes é uma biblioteca livre, você pode redistribuí-la e/ou               |
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

#ifndef _BEFONTES_H
	#define _BEFONTES_H
	#include "../global/beglobal.h"
	#include <ft2build.h>
	#include <pango/pangoft2.h>
	#include FT_FREETYPE_H
	#include FT_GLYPH_H
	//verificar versão do FreeType2
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
		gint nLetra[224][5];//limitado à tabela ASCII (32 - 255) estendida, pontos * nUnidade (xMin, xMax, yMin, yMax, Largura)
		gint nCaixa[4];//caixa em pontos * nUnidade (xMin, xMax, yMin, yMax)
		gint nLarguraMax;
		gint nAlturaMax;//altura da linha
		gint nObjeto;//número do objeto dentro de um PDF (reservado);
		gint nEstiloFlags;// FT_STYLE_FLAG_ITALIC, FT_STYLE_FLAG_BOLD
		gint nFlags;
		gint nAscent;
		gint nDescent;
	} BEFONTES;
	
	//Protótipos
	void be_fontes_lista (const gchar *sArquivoFonte, void (*func_Info)(const gchar *sMensagens, gpointer user_data), gpointer user_data);
	BEFONTES *be_fontes_carregar (const gchar *sArquivoFonte, const gchar *sNome, const gchar *sEstilo);//sNome para primeira fonte localizada em sArquivoFonte; faz tentativas
	BEFONTES *_be_fontes_carregar (const gchar *sArquivoFonte, const gchar *sNome, const gchar *sEstilo);
	void be_fontes_descarregar (BEFONTES *Fonte);
	gint be_fontes_largura (BEFONTES *Fonte, const gchar *sTexto);
	void be_fontes_altura (BEFONTES *Fonte, const gchar *sTexto, gint *yMin, gint *yMax);
	//Protótipos reservados
	gchar *_be_fontes_extrair (FT_Library FTlib, const gchar *sArquivo);
#endif
