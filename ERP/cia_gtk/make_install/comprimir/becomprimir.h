/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEComprimir                                                                  |
+------------------------------------------------------------------------------+
| BEComprimir � uma biblioteca livre, voc� pode redistribu�-la e/ou            |
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

#ifndef _BECOMPRIMIR_H
	#define _BECOMPRIMIR_H
	#include "../global/beglobal.h"
	#include <zlib.h>

	//Verificar vers�o do zlib
	#ifdef ZLIB_H
		#if ZLIB_VERNUM < 0x1210
			#error "VERSAO DO ZLIB DEVE SER >= 1.2.1"
		#endif
	#endif

	//Prot�tipos
	guchar *be_comprimir (const gchar *sString, gulong *lComprimento);//comprimi um texto
	guchar *be_comprimir_nivel (const gchar *sString, gulong *lComprimento, gint nNivel);//comprimi um texto, com op��o de n�vel
	gchar *be_comprimir_de (const guchar *sString, gulong *lComprimento, gulong lComprimentoOri);//comprimi um texto, com op��o de n�vel
	void be_comprimir_arquivo (const gchar *sArquivoOri, const gchar *sArquivoGZ);//compacta um arquivo, em formato ZIP
	void be_comprimir_arquivo_de (const gchar *sArquivoGZ, const gchar *sArquivoOri);//descompacta um arquivo, em formato ZIP
	guchar *be_comprimir_arquivo_string (const gchar *sArquivo, gulong *lComprimento);//compacta um arquivo, em formato ZIP, para uma string
#endif
