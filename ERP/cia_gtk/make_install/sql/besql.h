/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2007                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BESQL - (para MySQL)                                                         |
+------------------------------------------------------------------------------+
| BESQL é uma biblioteca livre, você pode redistribuí-la e/ou                  |
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
#ifndef _BESQL_H
	#define _BESQL_H
	#include "../global/beglobal.h"
	#include "../comprimir/becomprimir.h"
	//MySQL
	#include <mysql.h>
	//verificar a versão do MySQL
	#if MYSQL_VERSION_ID < 50005
		#error "VERSAO DO MYSQL DEVE SER >= 5.0.05"
	#endif
	//depuração
	#define _BESQL_ARQUIVO_SAIDA "besql_saida.sql"
	//MySQL-refman-5.0-en.a4.pdf pág. 1052
	#define _BESQL_INFORMACOES "information_schema"
	//MySQL-refman-5.0-en.a4.pdf pág. 197
	#define BESQL_CURINGA_TODOS 		"%"
	#define BESQL_CURINGA_PELOMENOSUM 	"!"
	#define BESQL_CURINGA_SOMENTEUM 	"_"
	//páginas, cláusula LIMIT padrão
	#define BESQL_MAX_LINHAS_POR_PAGINA 100
	//Limpeza eficiente da memória
	#define mysql_free_result(r); ({mysql_free_result(r);r=NULL;})
	//Tipos de comparações
	//MySQL-refman-5.0-en.a4.pdf pág. 620
	#define BESQL_COMPARACAO_IGUAL 			"="
	#define BESQL_COMPARACAO_IGUAL_SEMNULO 	"<=>"
	#define BESQL_COMPARACAO_MAIOR 			">"
	#define BESQL_COMPARACAO_MENOR 			"<"
	#define BESQL_COMPARACAO_MAIOR_IGUAL 	">="
	#define BESQL_COMPARACAO_MENOR_IGUAL 	"<="
	#define BESQL_COMPARACAO_DIFERENTE 		"<>"
	#define BESQL_COMPARACAO_PARECIDO 		"LIKE"
	#define BESQL_COMPARACAO_NULO 			"IS NULL"
	#define BESQL_COMPARACAO_NAO_NULO 		"IS NOT NULL"
	#define BESQL_COMPARACAO_VERDADEIRO 	"IS TRUE"
	#define BESQL_COMPARACAO_FALSO 			"IS FALSE"
	//função de mascaramento de valores
	#define BESQL_MASCARA_AMNUM			"AMNUM"
	#define BESQL_MASCARA_RMNUM			"RMNUM"
	#define BESQL_MASCARA_AMMOE			"AMMOE"
	#define BESQL_MASCARA_RMMOE			"RMMOE"
	#define BESQL_MASCARA_AMDAT			"AMDAT"
	#define BESQL_MASCARA_RMDAT			"RMDAT"
	#define BESQL_MASCARA_AMTEM			"AMTEM"
	#define BESQL_MASCARA_RMTEM			"RMTEM"
	#define BESQL_MASCARA_AMFONx		"AMFONx"
	#define BESQL_MASCARA_RMFONx		"RMFONx"
	#define BESQL_MASCARA_AMFON			"AMFON"
	#define BESQL_MASCARA_RMFON			"RMFON"
	#define BESQL_MASCARA_AMCNPJ		"AMCNPJ"
	#define BESQL_MASCARA_RMCNPJ		"RMCNPJ"
	#define BESQL_MASCARA_AMCEP			"AMCEP"
	#define BESQL_MASCARA_RMCEP			"RMCEP"
	#define BESQL_MASCARA_AMCPF			"AMCPF"
	#define BESQL_MASCARA_RMCPF			"RMCPF"
	#define BESQL_MASCARA_AMBOL			"AMBOL"
	#define BESQL_MASCARA_RMBOL			"RMBOL"
	//auto incremento
	#define BESQL_AUTO_ID 			"auto_increment"
	//campos do sistema CIA
	#define BESQL_CAMPO_ID "ID"
	//formato binário
	#define BESQL_BIN_COMPRIMENTO_NOME 60
	//Tipos de relações
	//MySQL-refman-5.0-en.a4.pdf pág. 624
	/*
	- Através do login atual (be_sql_abrir_conexao), localizar por todas as tabelas que conseguir acesso.
	- Quando necessário carregar (be_sql_abrir_consulta) todas as informações dos campos, e se possível os tipos especiais (`tabelas` e `campos`).
	*/
	//Tipos de dados, MySQL
	/*file:/usr/include/mysql/mysql_com.h
	MYSQL_TYPE_DECIMAL
	MYSQL_TYPE_TINY,
	MYSQL_TYPE_SHORT,
	MYSQL_TYPE_LONG,
	MYSQL_TYPE_FLOAT,
	MYSQL_TYPE_DOUBLE,
	MYSQL_TYPE_NULL,
	MYSQL_TYPE_TIMESTAMP,
	MYSQL_TYPE_LONGLONG,
	MYSQL_TYPE_INT24,
	MYSQL_TYPE_DATE,
	MYSQL_TYPE_TIME,
	MYSQL_TYPE_DATETIME,
	MYSQL_TYPE_YEAR,
	MYSQL_TYPE_NEWDATE,
	MYSQL_TYPE_VARCHAR,
	MYSQL_TYPE_BIT,
	MYSQL_TYPE_NEWDECIMAL=246,
	MYSQL_TYPE_ENUM=247,
	MYSQL_TYPE_SET=248,
	MYSQL_TYPE_TINY_BLOB=249,
	MYSQL_TYPE_MEDIUM_BLOB=250,
	MYSQL_TYPE_LONG_BLOB=251,
	MYSQL_TYPE_BLOB=252,
	MYSQL_TYPE_VAR_STRING=253,
	MYSQL_TYPE_STRING=254,
	MYSQL_TYPE_GEOMETRY=255
	*/
	//Tipos de dados, CIA
	typedef enum
	{
		BESQL_TIPO_SEL = MYSQL_TYPE_GEOMETRY + 1,//SEL, 256
		BESQL_TIPO_OPC,//OPC
		BESQL_TIPO_HIE,//HIE
		BESQL_TIPO_SUB,//SUB
		BESQL_TIPO_EXE,//EXE
		BESQL_TIPO_RTF,//RTF
		BESQL_TIPO_DUP,//DUP
		BESQL_TIPO_NUM,//NUM
		BESQL_TIPO_MOE,//MOE
		BESQL_TIPO_DAT,//DAT
		BESQL_TIPO_TEM,//TEM
		BESQL_TIPO_FON,//FONE
		BESQL_TIPO_FONx,//FONEx
		BESQL_TIPO_CNPJ,//CNPJ
		BESQL_TIPO_CPF,//CPF
		BESQL_TIPO_CEP,//CEP
		BESQL_TIPO_BOL,//BOL
		BESQL_TYPE_TINY_TEXT,//tinytext
		BESQL_TYPE_MEDIUM_TEXT,//mediumtext
		BESQL_TYPE_LONG_TEXT,//longtext
		BESQL_TYPE_TEXT//text
	} BESQL_TIPO_ENUM;
	//Tipos de indices
	typedef enum
	{
		BESQL_INDICE_SIMPLES = 0,
		BESQL_INDICE_UNICA,
		BESQL_INDICE_PRIMARIA
	} BESQL_RELACAO_INDICE;
	//Tipos de exportação
	typedef enum
	{
		BESQL_EXPORTAR_HTML = 0,
	} BESQL_EXPORTAR;

	//Tipos de acesso
	typedef enum
	{
		BESQL_ACESSO_NENHUM = 0,		//Nenhum
		BESQL_ACESSO_SELECIONAR = 1,	//SELECT
		BESQL_ACESSO_INSERIR = 2,		//INSERT
		BESQL_ACESSO_ALTERAR = 4,		//UPDATE
		BESQL_ACESSO_APAGAR = 8,		//DELETE
		BESQL_ACESSO_COMPLETO = BESQL_ACESSO_SELECIONAR + BESQL_ACESSO_INSERIR +  BESQL_ACESSO_ALTERAR + BESQL_ACESSO_APAGAR //Completo
	} BESQL_ACESSO_ENUM;
	//Tipos de relação
	typedef enum
	{
		BESQL_RELACAO_OU = 0,	// OR
		BESQL_RELACAO_E			// AND
	} BESQL_RELACAO_ENUM;
	//Tipos de ordenação
	typedef enum
	{
		BESQL_ORDENACAO_ASC = 0,	// ASC
		BESQL_ORDENACAO_DESC		// DESC
	} BESQL_ORDENACAO_ENUM;

	//Tipos de informações
	typedef enum
	{
		BESQL_INFO_SERVIDOR_IP = 0,
		BESQL_INFO_SERVIDOR_PORTA,
		BESQL_INFO_SERVIDOR_VERSAO,
		BESQL_INFO_CLIENTE_IP,
		BESQL_INFO_CLIENTE_VERSAO,
		BESQL_INFO_PROTOCOLO,
		BESQL_INFO_LINGUAGEM,
		BESQL_INFO_FUSO_HORARIO,
		BESQL_INFO_DIFERENCA_HORARIO,
		BESQL_INFO_CONEXAO,
		BESQL_INFO_USUARIO,
		BESQL_INFO_DESEMPENHO, //em ms
		BESQL_INFO_COMPILACAO,
		BESQL_INFO_EXPIRACAO,
		BESQL_INFO_STATUS,
		BESQL_INFO_ULTIMO_ID,
		BESQL_INFO_ALTERACAO,
		BESQL_INFO_AVISO,
		BESQL_INFO_SEGURO
	} BESQL_INFO_ENUM;

	//acesso inicial: SQL
	typedef struct __BESQL
	{
		MYSQL *_my;//conexão com MySQL
		gchar *sUsuario;//usuário atual
		gboolean bSuper;//super usuário; super administrador
		gchar *sIP;//usuário atual
		guint nPorta;//porta de conexão
		GSList *BDs;//lista de banco de dados
		gboolean (*func_Info)(struct __BESQL *SQL, gint nErro, const gchar *sErro, gpointer user_data);//callback de informações
		gpointer user_data;//dados do usuário
		gint nCacheMax;//máximo de cache das tabelas
	} BESQL;

	//tipo banco de dados: DATABASE
	typedef struct
	{
		BESQL *SQL;//comunicação desta tabela (relação pai)
		gchar *sNome;//nome do BD
		GSList *Tabelas;
		gboolean bMascara;
		gboolean bCIA;//conectado ao sistema CIA
	} BESQL_BD;

	//tipo tabelas: TABLE e VIEW
	typedef struct
	{
		BESQL_BD *BD;//bd desta tabela (relação pai)
		gchar *sNome;//nome da tabela ou view
		gchar *sNomeReal;//nome real da tabela, para CIA
		gchar *sNomeUsual;//nome usual da tabela, para CIA
		BESQL_ACESSO_ENUM nFlags;//acesso
		gchar *sComentarios;//comentários extra, CIA
		GSList *Relatorios;//lista de relatórios (gchar *), extra, CIA.
		gboolean bCIA;//conectado ao sistema CIA
		//cache
		gpointer SintaxeCache;//sintaxe em cache de cada tabela
		gint nCopiasSintaxeCache;//quantidade de cópias atualmente em uso
		gint nTempoCache;//tempo de descarregamento da última cópia do cache
		//ìcones ???
	} BESQL_Tabela;

	//síntaxe SELECT
	typedef struct
	{
		BESQL_Tabela *Tabela;//relação pai, quase constante
		MYSQL_RES *res;//resultado da consulta do MySQL, temporário
		gchar *sConsulta;//consulta atual, temporária
		GSList *Campos;//do tipo BESQL_Campo
		GSList *Indices;//do tipo BESQL_Indice
		GSList *Filtros;//do tipo BESQL_Filtro: padrão
		GSList *FiltrosUniao;//do tipo BESQL_Filtro: união com o padrão. Operação AND entre eles.
		GSList *Ordens;//do tipo BESQL_Ordem
		glong lLinhasPaginaMax;//padrão é BESQL_MAX_LINHAS_POR_PAGINA
		glong lLinhasPagina;//quantidade de linhas da página atual
		glong lLinhas;//quantidade de linhas, -1 para indefinido
		glong lLinha;//linha atual, relativa
		//conjunto CIA
		BESQL_Tabela *vis_Tabela;//view principal
		BESQL_Tabela *blo_Tabela;//view dos dados bloqueados
		BESQL_Tabela *adi_Tabela;//view para adição de dados
		BESQL_Tabela *alt_Tabela;//view para alteração de dados
		BESQL_Tabela *mex_Tabela;//view para marcar para excluir
		BESQL_Tabela *exc_Tabela;//view para excluir
		gboolean bForcarNulos;//converter comprimento zero em nulos
	} BESQL_Sintaxe;//utilizar o nome padrão be_SQL

	//tipo campo: FIELD
	typedef struct
	{
		BESQL_Sintaxe *Sintaxe;//relação pai
		gchar *sNome;//nome do campo
		gint nCampoPos;//posição do campo na consulta atual
		const gchar *sValor;//valor atual do campo, temporário, disponível somente após uma execução de uma consulta
		gint nComprimento;//comprimento do valor atual, temporário
		gint nComprimentoMax;//comprimento maximo do campo
		gint nComprimentoTop;//comprimento maximo do campo na consulta atual, disponível somente após be_sql_comprimento_linhas
		gboolean bAnulavel;//possível ter valor nulo.
		gboolean bAtivo;//visível nas consultas SELECT
		gboolean bAlteravel;//possível de alterações
		gboolean bAutoID;//AUTO_INCREMENT
		gchar *sTipo;//tipo em formato texto
		gint nTipo;//tipo do campo, -1 para indefinido, pode haver informações extras de CIA
		gchar *sParametros;//parâmetro extra, CIA
		gchar *sGrupo;//grupo extra, CIA
		gchar *sComentarios;//comentários extra, CIA
		gchar *sValorPadrao;//valor default campo, extra, CIA
		gchar *sTotal;//total deste campo, CIA, disponível somente após be_sql_totalizar_linhas
		//alterações: UPDATE e INSERT
		gchar *sValorNovo;//valor utilizado para alterações e adições
		gint nComprimentoNovo;//valor utilizado para alterações e adições
		gboolean bAfetar;//utilizar o sValorNovo, mesmo se for nulo
	} BESQL_Campo;

	//tipo índice: INDEX
	typedef struct
	{
		BESQL_Sintaxe *Sintaxe;//relação pai
		gchar *sNome;//nome do índice
		BESQL_RELACAO_INDICE nTipo;
		gint nSequencia;//numeração para vários campos
		BESQL_Campo *Campo;//relação irmão
		//alterações: UPDATE e DELETE
		gchar *sValorFiltro;//valor utilizado para alterações e exclusões
		gint nComprimentoFiltro;//valor utilizado para alterações e exclusões
		gboolean bAfetar;//utilizar o sValorFiltro, mesmo se for nulo
	} BESQL_Indice;

	//tipo filtro: WHERE
	typedef struct
	{
		BESQL_Campo *Campo;//campo
		gchar *sComparacao;//sinal de comparacao
		gchar *sValor;//valor
		BESQL_RELACAO_ENUM nRelacao;//relacao com o campo anterior, ou grupo anterior
		gint nGrupo;//grupo dentro de um parênteses
		gboolean bAtivo;
	} BESQL_Filtro;

	//tipo ordem: ORDER BY
	typedef struct
	{
		BESQL_Campo *Campo;//campo
		BESQL_ORDENACAO_ENUM nOrdem;//ordem
		gboolean bAtivo;
	} BESQL_Ordem;

	//SQL
	#define BE_sql_abrir_conexao(u,se,ip,p,c,e) be_sql_abrir_conexao_segura(u,se,ip,p,c,e,NULL,NULL,NULL,NULL,NULL)
	BESQL *be_sql_abrir_conexao_segura (const gchar *sUsuario, const gchar *sSenha, const gchar *sIP, guint nPorta, gint nCacheMax, gchar **sErros, const gchar *sSSLChavePrivada, const gchar *sSSLCertificado, const gchar *sSSLCA, const gchar *sSSLCACaminho, const gchar *sSSLCifra);//nCacheMax<0 para cache ilimitado
	void be_sql_callback (BESQL *SQL, gboolean (*func_Info)(struct __BESQL *SQL, gint nErro, const gchar *sErro, gpointer user_data), gpointer user_data);
	#define BE_sql_fechar_conexao(s) _be_sql_fechar_conexao(s);s=NULL
	BESQL_BD *be_sql_selecionar_bd (BESQL *SQL, const gchar *sNome, gchar **sErros);
	BESQL_Tabela *be_sql_selecionar_tabela (BESQL_BD *BD, const gchar *sNome);
	gboolean be_sql_alterar_senha (BESQL *SQL, const gchar *sNovaSenha, gchar **sErros);
	void be_sql_ping (BESQL *SQL, gchar **sErros);
	//SQL reservados
	void _be_sql_fechar_conexao (BESQL *SQL);
	BESQL_ACESSO_ENUM _be_sql_acesso (BESQL_Tabela *Tabela, gchar **sErros);
	gboolean _be_sql_acesso_super (BESQL *SQL, gchar **sErros);
	void _be_sql_BD_CIA (BESQL_BD *BD, gchar **sErros);
	void _be_sql_tabela_CIA (BESQL_Tabela *Tabela, gchar **sErros);
	gchar *be_sql_escapar (BESQL *SQL, const gchar *sValor, glong lBytes);
	gboolean _be_sql_mascaras (BESQL *SQL, BESQL_BD *BD, gchar **sErros);

	//tabela cache
	void _be_sql_gravar_cache_local_tabela (BESQL_Tabela *Tabela, gchar *sHashMD5);
	BESQL_Tabela *_be_sql_carregar_cache_local_tabela (BESQL_BD *BD, gchar *sTabela, gchar *sHashMD5);
	//sintaxe cache
	BESQL_Sintaxe *be_sql_abrir_consulta_cache (BESQL_Tabela *Tabela, gchar **sErros);
	void _be_sql_gravar_cache_local_sintaxe (BESQL_Sintaxe *Sintaxe, gchar *sHashMD5);
	BESQL_Sintaxe *_be_sql_carregar_cache_local_sintaxe (BESQL_Tabela *Tabela, gchar *sHashMD5);
	void be_sql_abrir_consulta_cache_todas (BESQL_BD *BD, gchar **sErros);
	GSList *be_sql_campo_duplicar_lista (GSList *Campos, BESQL_Sintaxe *Sintaxe);//duplicar a lista de campos
    GSList *be_sql_indice_duplicar_lista (GSList *Indices, BESQL_Sintaxe *Sintaxe, GSList *Campos);//duplicar a lista de índices

	//sintaxe
	BESQL_Sintaxe *be_sql_abrir_consulta (BESQL_Tabela *Tabela, gboolean bForcarNulos, gchar **sErros);
	#define BE_sql_fechar_consulta(s) _be_sql_fechar_consulta(s);s=NULL
	//sintaxe reservados
	void _be_sql_fechar_consulta (BESQL_Sintaxe *Sintaxe);

    //sintaxe perfil (configurações personalizadas)
    gchar *be_sql_perfil_ler (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, gchar **sErros);
    GKeyFile *be_sql_perfil_ler_ini (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, gchar **sErros);
    gboolean be_sql_perfil_salvar (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, const gchar *sValor, gboolean bPublico, gchar **sErros);
    gboolean be_sql_perfil_salvar_ini (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, GKeyFile *Valor, gboolean bPublico, gchar **sErros);
    gboolean be_sql_perfil_excluir (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, gchar **sErros);
    GSList *be_sql_perfil_variaveis (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, gchar **sErros);

	//sintaxe direto, aceita comandos separados por ";"
	gint be_sql_executar (BESQL *SQL, const gchar *sSintaxe, gboolean (*func_Ret)(BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data), gpointer user_data, gchar **sErros);//retorna quantidade de linhas retornadas ou alteradas (0 = não encontrados, -1 = erros)
	gboolean be_sql_informacoes_callback (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data);

	//campos
	gint be_sql_campo_quantidade_ativos (BESQL_Sintaxe *Sintaxe);
	gchar *be_sql_campo_valor_limitado (BESQL_Campo *Campo, gint nComprimentoMax);//Ilimitado => nComprimentoMax = -1
	typedef enum
	{
		BESQL_CAMPOS_LIMITAR_FORMATO_NENHUM = 0,
		BESQL_CAMPOS_LIMITAR_FORMATO_UTF8 = 1,
		BESQL_CAMPOS_LIMITAR_FORMATO_XML = 2,
	} BESQL_CAMPOS_LIMITAR_FORMATO;
	gchar *be_sql_campo_valor_limitado_formatado (BESQL_Campo *Campo, gint nComprimentoMax, BESQL_CAMPOS_LIMITAR_FORMATO nFlags);//Ilimitado => nComprimentoMax = -1
	BESQL_Campo *be_sql_selecionar_campo (BESQL_Sintaxe *Sintaxe, const gchar *sNome);
	BESQL_Campo *be_sql_selecionar_campo_auto_id (BESQL_Sintaxe *Sintaxe);
	BESQL_Campo *be_sql_selecionar_campo_id (BESQL_Sintaxe *Sintaxe);
	BESQL_Campo *be_sql_selecionar_campo_indice (BESQL_Sintaxe *Sintaxe, BESQL_RELACAO_INDICE nTipo);
	gboolean be_sql_inserir_arquivo (BESQL_Campo *Campo, const gchar *sArquivo, gchar cTipo);//sArquivo em UTF8
	gboolean be_sql_recuperar_arquivo (BESQL_Campo *Campo, const gchar *sArquivo, gchar cTipo);//salvar em disco
	GdkPixbuf *be_sql_recuperar_arquivo_pixbuf (BESQL_Campo *Campo, gchar cTipo);//carregar imagem
	//gboolean be_sql_inserir_imagem (BESQL_Campo *Campo, const gchar *sNome, GdkPixbuf *Imagem);
	//GdkPixbuf *be_sql_recuperar_imagem (const BESQL_Campo *Campo);
	gboolean be_sql_campo_texto (BESQL_Campo *Campo);
	gboolean be_sql_campo_binario (BESQL_Campo *Campo);
	gboolean be_sql_campo_numerico (BESQL_Campo *Campo);
	gboolean be_sql_campo_logico (BESQL_Campo *Campo);
	//campos reservados
	void _be_sql_campo_CIA (BESQL_Campo *Campo, gchar **sErros);
	void _be_sql_campo_valores_limpar (BESQL_Sintaxe *Sintaxe);
	void _be_sql_mascarar_valor_padrao (BESQL_Sintaxe *SintaxeCache, BESQL_Tabela *Tabela, gchar **sErros);

	//índices
	BESQL_Indice *be_sql_selecionar_indice (BESQL_Sintaxe *Sintaxe, const gchar *sNome, const gchar *sNomeCampo);

	//ordem
	BESQL_Ordem *be_sql_ordem_adicionar (BESQL_Sintaxe *Sintaxe, BESQL_Campo *Campo, BESQL_ORDENACAO_ENUM nOrdem);
	#define BE_sql_ordem_excluir(s,o) _be_sql_ordem_excluir(s,o);o=NULL
	void be_sql_ordem_limpar (BESQL_Sintaxe *Sintaxe);
	//ordem reservados
	void _be_sql_ordem_excluir (BESQL_Sintaxe *Sintaxe, BESQL_Ordem *Ordem);

	//filtros
	BESQL_Filtro *be_sql_filtro_adicionar (BESQL_Sintaxe *Sintaxe, BESQL_Campo *Campo, const gchar *sComparacao, const gchar *sValor, BESQL_RELACAO_ENUM nRelacao, gint nGrupo, gboolean bUniao);
	#define BE_sql_filtro_excluir(s,f,u) _be_sql_filtro_excluir(s,f,u);f=NULL
	void be_sql_filtro_limpar (BESQL_Sintaxe *Sintaxe, gboolean bUniao);
	GSList *be_sql_filtro_duplicar_lista (GSList *Filtros, BESQL_Sintaxe *SintaxeCampos);//duplicar lista de filtros, utilizando uma nova lista de campos
	#define BE_sql_filtro_limpar_lista(l) _be_sql_filtro_limpar_lista(l);l=NULL
	//filtros reservados
	void _be_sql_filtro_excluir (BESQL_Sintaxe *Sintaxe, BESQL_Filtro *Filtro, gboolean bUniao);
	gint _be_sql_filtro_ordenar_grupo (gconstpointer a, gconstpointer b);
	gchar *_be_sql_filtro_sintaxe (BESQL_Sintaxe *Sintaxe, GSList *Filtros);
	void _be_sql_filtro_limpar_lista (GSList *Filtros);

	//navegação
	void be_sql_contar_linhas (BESQL_Sintaxe *Sintaxe, gchar **sErros);
	gint be_sql_paginas (BESQL_Sintaxe *Sintaxe);
	glong be_sql_selecionar (BESQL_Sintaxe *Sintaxe, gint nPagina, gchar **sErros);//execução inicial da consulta
	void be_sql_totalizar_linhas (BESQL_Sintaxe *Sintaxe, gchar **sErros);
	void be_sql_comprimento_linhas (BESQL_Sintaxe *Sintaxe, gchar **sErros);//todos os campos são considerados strings
	glong be_sql_selecionar_proxima_linha (BESQL_Sintaxe *Sintaxe);//execução inicial da consulta
	gint be_sql_linhas_pagina (BESQL_Sintaxe *Sintaxe);

	//alteração nos dados
	glong be_sql_adicionar (BESQL_Sintaxe *Sintaxe, gchar **sErros);//adicionar
	glong be_sql_alterar (BESQL_Sintaxe *Sintaxe, gchar **sErros);//alterar pelo filtro
	glong be_sql_substituir (BESQL_Sintaxe *Sintaxe, gchar **sErros);//substituir pelo filtro
	glong be_sql_excluir (BESQL_Sintaxe *Sintaxe, gchar **sErros);//excluir pelo filtro
	//alteração pelo índice
	glong be_sql_alterar_indice (BESQL_Sintaxe *Sintaxe, gchar **sErros);//alterar pelo índice
	glong be_sql_excluir_indice (BESQL_Sintaxe *Sintaxe, gchar **sErros);//excluir pelo índice
	//alteração reservada
	glong _be_sql_alterar (BESQL_Sintaxe *Sintaxe, const gchar *sFiltro, gchar **sErros);

	//alteração nos dados, CIA
	gboolean be_sql_obter_bloqueio_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);
	gboolean be_sql_obter_bloqueio_excluir_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);
	gboolean be_sql_liberar_bloqueio_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);
	gint be_sql_usuario_bloqueando_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);//usuário bloqueando alterações no sID
	gboolean be_sql_adicionar_CIA (BESQL_Sintaxe *SintaxeAdicionar, gchar **sErros);//SintaxeAdicionar é nova estrutura
	gboolean be_sql_alterar_CIA (BESQL_Sintaxe *SintaxeAlterar, const gchar *sID, gchar **sErros);//alterar pelo sID. SintaxeAlterar é nova estrutura
	gboolean be_sql_excluir_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros);//pelo sID. mex_Tabela e exc_Tabela são verdadeiros

	//informações, status, variáveis do servidor
	gchar *be_sql_informacoes (BESQL *SQL, BESQL_INFO_ENUM nOpcoes, gchar **sErros);
	gboolean be_sql_exportar_dados (BESQL_Sintaxe *Sintaxe, const gchar *sArquivo, const gchar *sNome, BESQL_EXPORTAR nFormato, gchar **sErros);

	//erros reservados
	#define _be_sql_erro_retorno(c,e) __be_sql_erro_retorno(__FILE__, __LINE__,c,e)
	void __be_sql_erro_retorno (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros);
	#define _be_sql_erro_alteracao(c,e) __be_sql_erro_alteracao(__FILE__, __LINE__,c,e)
	void __be_sql_erro_alteracao (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros);
	#define _be_sql_erro_consulta(c,e,s) __be_sql_erro_consulta(__FILE__, __LINE__,c,e,s)
	void __be_sql_erro_consulta (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros, const gchar *sConsulta);
	#define _be_sql_saida(s) __be_sql_saida(__LINE__,s)
	void __be_sql_saida (gint nLinha, const gchar *sConsulta);
	//avisos reservados
	#define _be_sql_aviso_retorno(c,e) __be_sql_aviso_retorno(__FILE__, __LINE__,c,e)
	void __be_sql_aviso_retorno (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros);

	//#define BESQL_DEPURACAO
	#define BESQL_DEPURACAO_SQL
        //eficiente para depuração de erros
#endif
