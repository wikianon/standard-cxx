/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEComprimir                                                                  |
+------------------------------------------------------------------------------+
| BEComprimir é uma biblioteca livre, você pode redistribuí-la e/ou            |
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

#ifndef _BECOMPRIMIR_H
	#define _BECOMPRIMIR_H
	#include "../global/beglobal.h"
	#include <zlib.h>

	//Verificar versão do zlib
	#ifdef ZLIB_H
		#if ZLIB_VERNUM < 0x1210
			#error "VERSAO DO ZLIB DEVE SER >= 1.2.1"
		#endif
	#endif

	//Protótipos
	guchar *be_comprimir (const gchar *sString, gulong *lComprimento);//comprimi um texto
	guchar *be_comprimir_nivel (const gchar *sString, gulong *lComprimento, gint nNivel);//comprimi um texto, com opção de nível
	gchar *be_comprimir_de (const guchar *sString, gulong *lComprimento, gulong lComprimentoOri);//comprimi um texto, com opção de nível
	void be_comprimir_arquivo (const gchar *sArquivoOri, const gchar *sArquivoGZ);//compacta um arquivo, em formato ZIP
	void be_comprimir_arquivo_de (const gchar *sArquivoGZ, const gchar *sArquivoOri);//descompacta um arquivo, em formato ZIP
	guchar *be_comprimir_arquivo_string (const gchar *sArquivo, gulong *lComprimento);//compacta um arquivo, em formato ZIP, para uma string
#endif
