/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEMonitor                                                                    |
+------------------------------------------------------------------------------+
| BEMonitor � uma biblioteca livre, voc� pode redistribu�-la e/ou              |
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

#ifndef _BEMONITOR_H
	#define _BEMONITOR_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"

	//defini��es
	#ifdef G_OS_UNIX
		#define BEMONITOR_ARQ_ERRO "erro"
		#define BEMONITOR_CONEXAO_SERVIDOR "suporte"
	#endif
	#ifdef G_OS_WIN32
		#define BEMONITOR_ARQ_ERRO "erro.txt"
		#define BEMONITOR_CONEXAO_SERVIDOR "suporte.ini"
	#endif
	#define BEMONITOR_ARQ_SQL "besql_saida.sql"
	#define BEMONITOR_USUARIO_SUPORTE "suporte"
	#define BEMONITOR_SENHA_SUPORTE ""


	//Prot�tipos
	gboolean be_monitor (const gchar *sArquivo, void (*func_Info)(const gchar *sMensagens, gpointer user_data), gpointer user_data);//retorna se � necess�rio reiniciar a aplica��o
#endif
