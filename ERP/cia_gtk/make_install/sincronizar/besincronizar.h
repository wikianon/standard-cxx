/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2007                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BESINCRONIZAR                                                                |
+------------------------------------------------------------------------------+
| BESINCRONIZAR é uma biblioteca livre, você pode redistribuí-la e/ou          |
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
#ifndef _BESINCRONIZAR_H
	#define _BESINCRONIZAR_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../odbc/beodbc.h"
	#include "../net/benet.h"
    #define BESINCRONIZAR_DEPURACAO

	//definições, arquivos
	#define BESINCRONIZAR_LOG "sincronizar"

	//estrutura dos dados ODBC
	typedef struct
	{
		BESQL *SQL;
        GSList *SQLAlternativas;// de gchar*; somente executada se a anterior falhar, retornar <= 0
        gint nLinhasODBC;
        gint nLinhasSQL;
        gboolean bProgresso;
        gboolean bErro;
        gboolean bLog;
	} _BESincronizarODBC;
	//estrutura dos dados NET
	typedef struct
	{
		BESQL *SQL;//const
		GKeyFile *Ini;//const
		gchar *sAcao;//const
		gchar *sNetLoop;//const
		gboolean bSiteInicial;
		BENET *Site;//const
		gboolean bUTF8;
        gint nLinhasSQL;//numeração das linhas processadas pelo callback, do SQL
        gboolean bProgresso;
        gboolean bLog;
        gchar *sNetProxy;//const
        gchar *sNetAcesso;//const
        gchar *sNetNavegador;//const
        gchar *sNetInicio;//const
	} _BESincronizarNET;

    //funções
    GKeyFile *be_sincronizar_iniciar_ini (const gchar *sArquivo);
    BESQL *be_sincronizar_autenticar (const gchar *sArquivo);//conexão com BD atual
    gboolean be_sincronizar_executar_net (BESQL *SQL, const gchar *sArquivo, gboolean bProgresso, gboolean bLog);//executar as ações do INI
    gboolean be_sincronizar_executar_odbc (BESQL *SQL, const gchar *sArquivo, gboolean bProgresso, gboolean bLog);//executar as ações do INI
    gboolean be_sincronizar_salvar_arquivo (const gchar *sArquivo, const gchar *sConteudo, gint nLen);//grava um conteúdo em um arquivo
    void be_sincronizar_log_iniciar (gint nQuantidade);//iniciar Log de ações
    void be_sincronizar_log_fechar ();//finalizar log de ações
    gchar *be_sincronizar_iso_key_file_get_string (GKeyFile *key_file, const gchar *group_name, const gchar *key);
	#define BE_sincronizar_log_informar(f,...) _be_sincronizar_log_informar(g_strdup_printf(f,__VA_ARGS__))
    //reservados
    gboolean _be_sincronizar_callback_ODBC (const BEODBC *ODBC, gint nCampos, gint nLinha, GSList *Campos, gint nTipo, gint nComp, GSList *Valores, gpointer user_data);//retorno da função be_odbc_executar
    void _be_sincronizar_log_informar (gchar *sInfo);//escrever informações no Log
    gboolean _be_sincronizar_executar_net_callback (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data);//callback da função be_sql_executar
    gchar *_be_sincronizar_executar_net (_BESincronizarNET *BESincronizarNET, GSList *Campos);//processamento real
#endif

/*
Ordem de processamento:
NetConexao
NetSQLValores
NetSQLValoresInicial
Net0_URL
Net0_Resetar
Net0_Salvar
Net0_Salvar_T
Net0_Pular0		//Net0_Pular1 é considerado lógico E
Net0_$
Net0_$__CONTROL_FINGERPRINTS_AjaxPanel1 = __CONTROL_FINGERPRINTS_AjaxPanel1","$$")
Net0_Ativo$
Net0_Validar_P
Net0_Extrair$1
Net0_SQLTravarTabela
Net0_SQLDestravarTabela
Net0_SQLConsulta0
Net0_SQLConsultas
Net0_SQLAlternativa0
*/

