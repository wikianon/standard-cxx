/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEMonitor                                                                    |
+------------------------------------------------------------------------------+
| BEMonitor é uma biblioteca livre, você pode redistribuí-la e/ou              |
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

#ifndef _BEMONITOR_H
	#define _BEMONITOR_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"

	//definições
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


	//Protótipos
	gboolean be_monitor (const gchar *sArquivo, void (*func_Info)(const gchar *sMensagens, gpointer user_data), gpointer user_data);//retorna se é necessário reiniciar a aplicação
#endif
