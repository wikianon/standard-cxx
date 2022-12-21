/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2007                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BESINCRONIZAR                                                                |
+------------------------------------------------------------------------------+
| BESINCRONIZAR � uma biblioteca livre, voc� pode redistribu�-la e/ou          |
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
#ifndef _BESINCRONIZAR_H
	#define _BESINCRONIZAR_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../odbc/beodbc.h"
	#include "../net/benet.h"
    #define BESINCRONIZAR_DEPURACAO

	//defini��es, arquivos
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
        gint nLinhasSQL;//numera��o das linhas processadas pelo callback, do SQL
        gboolean bProgresso;
        gboolean bLog;
        gchar *sNetProxy;//const
        gchar *sNetAcesso;//const
        gchar *sNetNavegador;//const
        gchar *sNetInicio;//const
	} _BESincronizarNET;

    //fun��es
    GKeyFile *be_sincronizar_iniciar_ini (const gchar *sArquivo);
    BESQL *be_sincronizar_autenticar (const gchar *sArquivo);//conex�o com BD atual
    gboolean be_sincronizar_executar_net (BESQL *SQL, const gchar *sArquivo, gboolean bProgresso, gboolean bLog);//executar as a��es do INI
    gboolean be_sincronizar_executar_odbc (BESQL *SQL, const gchar *sArquivo, gboolean bProgresso, gboolean bLog);//executar as a��es do INI
    gboolean be_sincronizar_salvar_arquivo (const gchar *sArquivo, const gchar *sConteudo, gint nLen);//grava um conte�do em um arquivo
    void be_sincronizar_log_iniciar (gint nQuantidade);//iniciar Log de a��es
    void be_sincronizar_log_fechar ();//finalizar log de a��es
    gchar *be_sincronizar_iso_key_file_get_string (GKeyFile *key_file, const gchar *group_name, const gchar *key);
	#define BE_sincronizar_log_informar(f,...) _be_sincronizar_log_informar(g_strdup_printf(f,__VA_ARGS__))
    //reservados
    gboolean _be_sincronizar_callback_ODBC (const BEODBC *ODBC, gint nCampos, gint nLinha, GSList *Campos, gint nTipo, gint nComp, GSList *Valores, gpointer user_data);//retorno da fun��o be_odbc_executar
    void _be_sincronizar_log_informar (gchar *sInfo);//escrever informa��es no Log
    gboolean _be_sincronizar_executar_net_callback (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data);//callback da fun��o be_sql_executar
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
Net0_Pular0		//Net0_Pular1 � considerado l�gico E
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

