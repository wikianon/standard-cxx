/*
| Luciano Trindade Pereira 2007                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEODBC                                                                       |
+------------------------------------------------------------------------------+
| BEODBC � uma biblioteca livre, voc� pode redistribu�-la e/ou                 |
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
#ifndef _BEODBC_H
	#define _BEODBC_H
	#include "../global/beglobal.h"
	#include "../comprimir/becomprimir.h"
	//ODBC
	#include <sql.h>
	#include <sqlext.h>
	//verificar a vers�o do odbc
	#if ODBCVER < 0x0351
		#error "VERSAO DO ODBC DEVE SER >= 3.51"
	#endif
	//depura��o
	#define _BEODBC_ARQUIVO_SAIDA "beodbc_saida.sql"
	//curingas para filtros
	#define BEODBC_CURINGA_TODOS 		"%" //todos
	#define BEODBC_CURINGA_PELOMENOSUM 	"!" //pelo menos um
	#define BEODBC_CURINGA_SOMENTEUM 	"_" //somente um
	//p�ginas, cl�usula LIMIT padr�o
	#define BEODBC_MAX_LINHAS_POR_PAGINA 100
	//tipos de compara��es, para filtros
	#define BEODBC_COMPARACAO_IGUAL 		"="
	#define BEODBC_COMPARACAO_IGUAL_SEMNULO "<=>"
	#define BEODBC_COMPARACAO_MAIOR 		">"
	#define BEODBC_COMPARACAO_MENOR 		"<"
	#define BEODBC_COMPARACAO_MAIOR_IGUAL 	">="
	#define BEODBC_COMPARACAO_MENOR_IGUAL 	"<="
	#define BEODBC_COMPARACAO_DIFERENTE 	"<>"
	#define BEODBC_COMPARACAO_PARECIDO 		"LIKE"
	#define BEODBC_COMPARACAO_NULO 			"IS NULL"
	#define BEODBC_COMPARACAO_NAO_NULO 		"IS NOT NULL"
	#define BEODBC_COMPARACAO_VERDADEIRO 	"IS TRUE"
	#define BEODBC_COMPARACAO_FALSO 		"IS FALSE"
	/*
	- Tipos de strings de conex�o:
        http://www.connectionstrings.com
    - Refer�ncias on-line:
        http://msdn.microsoft.com/library/default.asp?url=/library/en-us/odbc/htm/odbcsqlsetenvattr.asp?frame=true (oficial do Windows)
        http://publib.boulder.ibm.com/infocenter/dzichelp/v2r2/topic/com.ibm.db2.doc.odbc/bjnemstr123.htm (com exemplos)
        http://www.easysoft.com/developer/languages/c/odbc_tutorial.html

        GetData = http://publib.boulder.ibm.com/infocenter/dzichelp/v2r2/topic/com.ibm.db2.doc.odbc/bjnemstr88.htm
    */
    //tipos de campos
    /*
    SQL_BLOB
    SQL_BLOB_LOCATOR
    SQL_CHAR
    SQL_BINARY
    SQL_CLOB
    SQL_CLOB_LOCATOR
    SQL_TYPE_DATE
    SQL_DBCLOB
    SQL_DBCLOB_LOCATOR
    SQL_DECIMAL
    SQL_DOUBLE
    SQL_FLOAT
    SQL_GRAPHIC
    SQL_INTEGER
    SQL_LONGVARCHAR
    SQL_LONGVARBINARY
    SQL_LONGVARGRAPHIC
    SQL_NUMERIC
    SQL_REAL
    SQL_ROWID
    SQL_SMALLINT
    SQL_TYPE_TIME
    */
	//Tipos de exporta��o
	typedef enum
	{
		BEODBC_EXPORTAR_HTML = 0,
	} BEODBC_EXPORTAR;

	//Tipos de acesso
	typedef enum
	{
		BEODBC_ACESSO_NENHUM = 0,		//Nenhum
		BEODBC_ACESSO_SELECIONAR = 1,	//SELECT
		BEODBC_ACESSO_INSERIR = 2,		//INSERT
		BEODBC_ACESSO_ALTERAR = 4,		//UPDATE
		BEODBC_ACESSO_APAGAR = 8,		//DELETE
		BEODBC_ACESSO_COMPLETO = BEODBC_ACESSO_SELECIONAR + BEODBC_ACESSO_INSERIR +  BEODBC_ACESSO_ALTERAR + BEODBC_ACESSO_APAGAR //Completo
	} BEODBC_ACESSO_ENUM;
	//Tipos de rela��o
	typedef enum
	{
		BEODBC_RELACAO_OU = 0,	// OR
		BEODBC_RELACAO_E			// AND
	} BEODBC_RELACAO_ENUM;
	//Tipos de ordena��o
	typedef enum
	{
		BEODBC_ORDENACAO_ASC = 0,	// ASC
		BEODBC_ORDENACAO_DESC		// DESC
	} BEODBC_ORDENACAO_ENUM;

	//Tipos de informa��es
	typedef enum
	{
		BEODBC_INFO_SERVIDOR_IP = 0,
		BEODBC_INFO_SERVIDOR_PORTA,
		BEODBC_INFO_SERVIDOR_VERSAO,
		BEODBC_INFO_CLIENTE_IP,
		BEODBC_INFO_CLIENTE_VERSAO,
		BEODBC_INFO_PROTOCOLO,
		BEODBC_INFO_LINGUAGEM,
		BEODBC_INFO_FUSO_HORARIO,
		BEODBC_INFO_DIFERENCA_HORARIO,
		BEODBC_INFO_CONEXAO,
		BEODBC_INFO_USUARIO,
		BEODBC_INFO_DESEMPENHO, //em ms
		BEODBC_INFO_COMPILACAO,
		BEODBC_INFO_EXPIRACAO,
		BEODBC_INFO_STATUS,
		BEODBC_INFO_ULTIMO_ID
	} BEODBC_INFO_ENUM;

	//acesso inicial: ODBC
	typedef struct
	{
        SQLHENV env;//ambiente
        SQLHDBC dbc;//conex�o
		gchar *sConexao;//conex�o (Conection String)
		GSList *Tabelas;//lista de tabelas
	} BEODBC;

	//tipo tabelas: TABLE e VIEW
	typedef struct
	{
		const BEODBC *ODBC;//comunica��o desta tabela (rela��o pai)
		gchar *sNome;//nome da tabela ou view
		gchar *sTipo;//TABLE, VIEW, SYSTEM TABLE, etc.
		gchar *sCatalogo;
		gchar *sEsquema;
		BEODBC_ACESSO_ENUM nFlags;//acesso
	} BEODBC_Tabela;

	//s�ntaxe SELECT
	typedef struct
	{
		const BEODBC_Tabela *Tabela;//rela��o pai
		gchar *sConsulta;//consulta atual, tempor�ria
		GSList *Campos;// do tipo BEODBC_Campo
		GSList *Indices;// do tipo BEODBC_Indice
		GSList *Filtros;// do tipo BEODBC_Filtro: padr�o
		GSList *FiltrosUniao;// do tipo BEODBC_Filtro: uni�o com o padr�o. Opera��o AND entre eles.
		GSList *Ordens;// do tipo BEODBC_Ordem
		glong lLinhasPaginaMax;//padr�o � BEODBC_MAX_LINHAS_POR_PAGINA
		glong lLinhasPagina;//quantidade de linhas da p�gina atual
		glong lLinhas;//quantidade de linhas, -1 para indefinido
		glong lLinha;//linha atual, relativa
		gboolean bForcarNulos;//converter comprimento zero em nulos
	} BEODBC_Sintaxe;//utilizar o nome padr�o be_ODBC

	//tipo campo: FIELD
	typedef struct
	{
		const BEODBC_Sintaxe *Sintaxe;//rela��o pai
		gchar *sNome;//nome do campo
		gint nCampoPos;//posi��o do campo na consulta atual
		const gchar *sValor;//valor atual do campo, tempor�rio, dispon�vel somente ap�s uma execu��o de uma consulta
		gint nComprimento;//comprimento do valor atual, tempor�rio
		gint nComprimentoMax;//comprimento maximo do campo
		gboolean bAnulavel;//poss�vel ter valor nulo.
		gboolean bAtivo;//vis�vel nas consultas SELECT
		gboolean bAlteravel;//poss�vel de altera��es
		gboolean bAutoID;//AUTO_INCREMENT
		gchar *sTipo;//tipo em formato texto
		gint nTipo;//tipo do campo
		gchar *sValorPadrao;//valor default
		//altera��es: UPDATE e INSERT
		gchar *sValorNovo;//valor utilizado para altera��es e adi��es
		gint nComprimentoNovo;//valor utilizado para altera��es e adi��es
		gboolean bAfetar;//utilizar o sValorNovo, mesmo se for nulo
	} BEODBC_Campo;

	/*//tipo filtro: WHERE
	typedef struct
	{
		const BEODBC_Campo *Campo;//campo
		gchar *sComparacao;//sinal de comparacao
		gchar *sValor;//valor
		BEODBC_RELACAO_ENUM nRelacao;//relacao com o campo anterior, ou grupo anterior
		gint nGrupo;//grupo dentro de um par�nteses
		gboolean bAtivo;
	} BEODBC_Filtro;

	//tipo ordem: ORDER BY
	typedef struct
	{
		const BEODBC_Campo *Campo;//campo
		BEODBC_ORDENACAO_ENUM nOrdem;//ordem
		gboolean bAtivo;
	} BEODBC_Ordem;
	*/

	//ODBC
	BEODBC *be_odbc_abrir_conexao (const gchar *sConexao, gchar *sCatalogo, gchar *sEsquema, gchar *sTabela, gchar *sTypes, gchar **sErros);
	#define BE_odbc_fechar_conexao(s) _be_odbc_fechar_conexao(s);s=NULL
	const BEODBC_Tabela *be_odbc_selecionar_tabela (const BEODBC *ODBC, const gchar *sNome, const gchar *sCatalogo, const gchar *sEsquema);
	//ODBC reservados
	void _be_odbc_fechar_conexao (BEODBC *ODBC);
	//BEODBC_ACESSO_ENUM _be_odbc_acesso (const BEODBC_Tabela *Tabela, gchar **sErros);
	//gchar *_be_odbc_escapar (const BEODBC *ODBC, const gchar *sValor, glong lBytes);

	//sintaxe
	BEODBC_Sintaxe *be_odbc_abrir_consulta (const BEODBC_Tabela *Tabela, gboolean bForcarNulos, gchar **sErros);
	#define BE_odbc_fechar_consulta(s) _be_odbc_fechar_consulta(s);s=NULL
	//sintaxe reservados
	//void _be_odbc_mascarar_valor_padrao (BEODBC_Sintaxe *Sintaxe, gchar **sErros);
	void _be_odbc_fechar_consulta (BEODBC_Sintaxe *Sintaxe);

	//campos
	/*gint be_odbc_campo_quantidade_ativos (const BEODBC_Sintaxe *Sintaxe);
	gchar *be_odbc_campo_valor_limitado (const BEODBC_Campo *Campo, gint nComprimentoMax);//Ilimitado => nComprimentoMax = -1
	typedef enum
	{
		BEODBC_CAMPOS_LIMITAR_FORMATO_NENHUM = 0,
		BEODBC_CAMPOS_LIMITAR_FORMATO_UTF8 = 1,
		BEODBC_CAMPOS_LIMITAR_FORMATO_XML = 2,
	} BEODBC_CAMPOS_LIMITAR_FORMATO;
	gchar *be_odbc_campo_valor_limitado_formatado (const BEODBC_Campo *Campo, gint nComprimentoMax, BEODBC_CAMPOS_LIMITAR_FORMATO nFlags);//Ilimitado => nComprimentoMax = -1
	BEODBC_Campo *be_odbc_selecionar_campo (const BEODBC_Sintaxe *Sintaxe, const gchar *sNome);
	BEODBC_Campo *be_odbc_selecionar_campo_auto_id (const BEODBC_Sintaxe *Sintaxe);
	BEODBC_Campo *be_odbc_selecionar_campo_id (const BEODBC_Sintaxe *Sintaxe);
	gboolean be_odbc_inserir_arquivo (BEODBC_Campo *Campo, const gchar *sArquivo, gchar cTipo);//sArquivo em UTF8
	gboolean be_odbc_recuperar_arquivo (const BEODBC_Campo *Campo, const gchar *sArquivo, gchar cTipo);//salvar em disco
	GdkPixbuf *be_odbc_recuperar_arquivo_pixbuf (const BEODBC_Campo *Campo, gchar cTipo);//carregar imagem
	//gboolean be_odbc_inserir_imagem (BEODBC_Campo *Campo, const gchar *sNome, GdkPixbuf *Imagem);
	//GdkPixbuf *be_odbc_recuperar_imagem (const BEODBC_Campo *Campo);
	gboolean be_odbc_campo_texto (const BEODBC_Campo *Campo);
	gboolean be_odbc_campo_binario (const BEODBC_Campo *Campo);
	gboolean be_odbc_campo_numerico (const BEODBC_Campo *Campo);
	gboolean be_odbc_campo_logico (const BEODBC_Campo *Campo);

	//�ndices
	BEODBC_Indice *be_odbc_selecionar_indice (const BEODBC_Sintaxe *Sintaxe, const gchar *sNome, const gchar *sNomeCampo);
	//campos reservados
	void _be_odbc_campo_CIA (BEODBC_Campo *Campo, gchar **sErros);
	void _be_odbc_campo_valores_limpar (BEODBC_Sintaxe *Sintaxe);

	//ordem
	BEODBC_Ordem *be_odbc_ordem_adicionar (BEODBC_Sintaxe *Sintaxe, const BEODBC_Campo *Campo, BEODBC_ORDENACAO_ENUM nOrdem);
	#define BE_odbc_ordem_excluir(s,o) _be_odbc_ordem_excluir(s,o);o=NULL
	void be_odbc_ordem_limpar (BEODBC_Sintaxe *Sintaxe);
	//ordem reservados
	void _be_odbc_ordem_excluir (BEODBC_Sintaxe *Sintaxe, BEODBC_Ordem *Ordem);

	//filtros
	BEODBC_Filtro *be_odbc_filtro_adicionar (BEODBC_Sintaxe *Sintaxe, const BEODBC_Campo *Campo, const gchar *sComparacao, const gchar *sValor, BEODBC_RELACAO_ENUM nRelacao, gint nGrupo, gboolean bUniao);
	#define BE_odbc_filtro_excluir(s,f,u) _be_odbc_filtro_excluir(s,f,u);f=NULL
	void be_odbc_filtro_limpar (BEODBC_Sintaxe *Sintaxe, gboolean bUniao);
	GSList *be_odbc_filtro_duplicar_lista (GSList *Filtros, const BEODBC_Sintaxe *SintaxeCampos);//duplicar lista de filtros, utilizando uma nova lista de campos
	#define BE_odbc_filtro_limpar_lista(l) _be_odbc_filtro_limpar_lista(l);l=NULL
	//filtros reservados
	void _be_odbc_filtro_excluir (BEODBC_Sintaxe *Sintaxe, BEODBC_Filtro *Filtro, gboolean bUniao);
	gint _be_odbc_filtro_ordenar_grupo (gconstpointer a, gconstpointer b);
	gchar *_be_odbc_filtro_sintaxe (BEODBC_Sintaxe *Sintaxe, GSList *Filtros);
	void _be_odbc_filtro_limpar_lista (GSList *Filtros);

	//navega��o
	void be_odbc_contar_linhas (BEODBC_Sintaxe *Sintaxe, gchar **sErros);
	gint be_odbc_paginas (BEODBC_Sintaxe *Sintaxe);
	glong be_odbc_selecionar (BEODBC_Sintaxe *Sintaxe, gint nPagina, gchar **sErros);//execu��o inicial da consulta
	void be_odbc_totalizar_linhas (BEODBC_Sintaxe *Sintaxe, gchar **sErros);
	void be_odbc_comprimento_linhas (BEODBC_Sintaxe *Sintaxe, gchar **sErros);//todos os campos s�o considerados strings
	glong be_odbc_selecionar_proxima_linha (BEODBC_Sintaxe *Sintaxe);//execu��o inicial da consulta
	gint be_odbc_linhas_pagina (BEODBC_Sintaxe *Sintaxe);

	//altera��o nos dados
	glong be_odbc_adicionar (BEODBC_Sintaxe *Sintaxe, gchar **sErros);//adicionar
	glong be_odbc_alterar (BEODBC_Sintaxe *Sintaxe, gchar **sErros);//alterar pelo filtro
	glong be_odbc_excluir (BEODBC_Sintaxe *Sintaxe, gchar **sErros);//excluir pelo filtro
	//altera��o pelo �ndice
	glong be_odbc_alterar_indice (BEODBC_Sintaxe *Sintaxe, gchar **sErros);//alterar pelo �ndice
	glong be_odbc_excluir_indice (BEODBC_Sintaxe *Sintaxe, gchar **sErros);//excluir pelo �ndice
	//altera��o reservada
	glong _be_odbc_alterar (BEODBC_Sintaxe *Sintaxe, const gchar *sFiltro, gchar **sErros);

	//altera��o nos dados, CIA
	gboolean be_odbc_obter_bloqueio_CIA (BEODBC_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);
	gboolean be_odbc_obter_bloqueio_excluir_CIA (BEODBC_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);
	gboolean be_odbc_liberar_bloqueio_CIA (BEODBC_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);
	gint be_odbc_usuario_bloqueando_CIA (BEODBC_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);//usu�rio bloqueando altera��es no sID
	gboolean be_odbc_adicionar_CIA (BEODBC_Sintaxe *SintaxeAdicionar, gchar **sErros);//SintaxeAdicionar � nova estrutura
	gboolean be_odbc_alterar_CIA (BEODBC_Sintaxe *SintaxeAlterar, const gchar *sID, gchar **sErros);//alterar pelo sID. SintaxeAlterar � nova estrutura
	gboolean be_odbc_excluir_CIA (BEODBC_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);//pelo sID. mex_Tabela e exc_Tabela s�o verdadeiros*/

	//ODBC direto
	gint be_odbc_executar (const BEODBC *ODBC, const gchar *sSintaxe, gboolean (*func_Ret)(const BEODBC *ODBC, gint nCampos, gint nLinha, GSList *Campos, gint nTipo, gint nComp, GSList *Valores, gpointer user_data), gpointer user_data, gchar **sErros);

	/*//informa��es, status, vari�veis do servidor
	gchar *be_odbc_informacoes (const BEODBC *ODBC, BEODBC_INFO_ENUM nOpcoes, gchar **sErros);
	gboolean be_odbc_exportar_dados (BEODBC_Sintaxe *Sintaxe, const gchar *sArquivo, const gchar *sNome, BEODBC_EXPORTAR nFormato, gchar **sErros);

	//erros reservados
	#define _be_odbc_erro_retorno(e) __be_odbc_erro_retorno(__FILE__, __LINE__,e)
	void __be_odbc_erro_retorno (const gchar *sArquivo, gint nLinha, gchar **sErros);
	#define _be_odbc_erro_alteracao(e) __be_odbc_erro_alteracao(__FILE__, __LINE__,e)
	void __be_odbc_erro_alteracao (const gchar *sArquivo, gint nLinha, gchar **sErros);
	#define _be_odbc_erro_consulta(c,e) __be_odbc_erro_consulta(__FILE__, __LINE__,c,e)
	void __be_odbc_erro_consulta (const gchar *sArquivo, gint nLinha, const BEODBC *ODBC, gchar **sErros);
	#define _be_odbc_saida(s) __be_odbc_saida(__LINE__,s)
	void __be_odbc_saida (gint nLinha, const gchar *sConsulta);


    */
    //erros reservados
	#define _be_odbc_erro(e,s,h,t) __be_odbc_erro(__FILE__,__LINE__,e,s,h,t)
	void __be_odbc_erro (const gchar *sArquivo, gint nLinha, gchar **sErros, const gchar *sNome, SQLHANDLE handle, SQLSMALLINT type);

    void extract_error (char *fn, SQLHANDLE handle, SQLSMALLINT type);

    #define BEODBC_DEPURACAO
    
    //Refer�ncia:
    //http://www.easysoft.com
#endif
