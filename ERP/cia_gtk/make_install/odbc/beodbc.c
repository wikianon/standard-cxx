#include "beodbc.h"

static FILE *FileODBC = NULL;

BEODBC *be_odbc_abrir_conexao (const gchar *sConexao, gchar *sCatalogo, gchar *sEsquema, gchar *sTabela, gchar *sTypes, gchar **sErros)
{
	/*
	STMT concorentes não funciona. Somente um de cada vêz e tem que alocar e liberar. Ele deve ser fechado antes de ENV e DBC
	Recuperar informações por colunas somente na mesma ordem. Se pular não é possível retornar para coluna anterior.
	*/
	BEODBC *ODBC = NULL;
	BEODBC_Tabela *Tabela = NULL;
	SQLHSTMT stmt;//consultas
	gchar sBuffer1[512], sBuffer2[512], sBuffer3[512], sBuffer4[512];
	gchar *sTemp = NULL;
	gboolean bErro = FALSE;
	//GSList *ListaL = NULL;

	//abrir arquivo de depuração
	if ((FileODBC = fopen (_BEODBC_ARQUIVO_SAIDA, "ab")))//adicionar binário
	{
		sTemp = g_strdup_printf ("%s - ", __FILE__);
		fwrite (sTemp, strlen (sTemp), 1, FileODBC);
		BE_free (sTemp);
		sTemp = BE_tempo ("%d/%m/%Y %H:%M:%S");
		fwrite (sTemp, strlen (sTemp), 1, FileODBC);
		BE_free (sTemp);
		sTemp = BE_tempo ("\n");
		fwrite (sTemp, strlen (sTemp), 1, FileODBC);
		BE_free (sTemp);
	}

    //reservando memória principal
    ODBC = g_new (BEODBC, 1);
    ODBC->sConexao = g_strdup (sConexao);
    ODBC->Tabelas = NULL;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

    //iniciando a biblioteca ODBC
	if (SQL_SUCCEEDED (SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &ODBC->env)))//alocar handle de ambiente
	{
        if (SQL_SUCCEEDED (SQLSetEnvAttr (ODBC->env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0)))//ativar suporte a ODBC 3.xx
        {
            if (SQL_SUCCEEDED (SQLAllocHandle (SQL_HANDLE_DBC, ODBC->env, &ODBC->dbc)))//alocar handle de conexão
            {
                //conexão
                if (SQL_SUCCEEDED (SQLDriverConnect (ODBC->dbc, NULL, (SQLCHAR *) ODBC->sConexao, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT)))
                {
/*#ifdef BEODBC_DEPURACAO
                    //obter informações do driver
                    printf ("Informacoes:\n");
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ACTIVE_ENVIRONMENTS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ACTIVE_ENVIRONMENTS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_GETDATA_EXTENSIONS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_GETDATA_EXTENSIONS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ASYNC_MODE, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ASYNC_MODE = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_INFO_SCHEMA_VIEWS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_INFO_SCHEMA_VIEWS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_BATCH_ROW_COUNT, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_BATCH_ROW_COUNT = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_KEYSET_CURSOR_ATTRIBUTES1, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_KEYSET_CURSOR_ATTRIBUTES1 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_BATCH_SUPPORT, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_BATCH_SUPPORT = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_KEYSET_CURSOR_ATTRIBUTES2, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_KEYSET_CURSOR_ATTRIBUTES2 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_MAX_ASYNC_CONCURRENT_STATEMENTS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_MAX_ASYNC_CONCURRENT_STATEMENTS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_HDBC, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_HDBC = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_MAX_CONCURRENT_ACTIVITIES, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_MAX_CONCURRENT_ACTIVITIES = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_HDESC, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_HDESC = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_MAX_DRIVER_CONNECTIONS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_MAX_DRIVER_CONNECTIONS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_HENV, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_HENV = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ODBC_INTERFACE_CONFORMANCE, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ODBC_INTERFACE_CONFORMANCE = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_HLIB, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_HLIB = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_HSTMT, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_HSTMT = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ODBC_VER, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ODBC_VER = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_NAME, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_NAME = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_PARAM_ARRAY_ROW_COUNTS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_PARAM_ARRAY_ROW_COUNTS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_ODBC_VER, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_ODBC_VER = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_PARAM_ARRAY_SELECTS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_PARAM_ARRAY_SELECTS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DRIVER_VER, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DRIVER_VER = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ROW_UPDATES, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ROW_UPDATES = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DYNAMIC_CURSOR_ATTRIBUTES1, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DYNAMIC_CURSOR_ATTRIBUTES1 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_SEARCH_PATTERN_ESCAPE, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_SEARCH_PATTERN_ESCAPE = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DYNAMIC_CURSOR_ATTRIBUTES2, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DYNAMIC_CURSOR_ATTRIBUTES2 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_SERVER_NAME, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_SERVER_NAME = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_STATIC_CURSOR_ATTRIBUTES2, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_STATIC_CURSOR_ATTRIBUTES2 = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_FILE_USAGE, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_FILE_USAGE = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DATABASE_NAME, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DATABASE_NAME = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DBMS_NAME, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DBMS_NAME = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DBMS_VER, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DBMS_VER = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ACCESSIBLE_PROCEDURES, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ACCESSIBLE_PROCEDURES = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_MULT_RESULT_SETS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_MULT_RESULT_SETS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_ACCESSIBLE_TABLES, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_ACCESSIBLE_TABLES = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_MULTIPLE_ACTIVE_TXN, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_MULTIPLE_ACTIVE_TXN = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_BOOKMARK_PERSISTENCE, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_BOOKMARK_PERSISTENCE = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_NEED_LONG_DATA_LEN, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_NEED_LONG_DATA_LEN = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_CATALOG_TERM, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_CATALOG_TERM = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_NULL_COLLATION, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_NULL_COLLATION = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_COLLATION_SEQ, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_COLLATION_SEQ = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_PROCEDURE_TERM, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_PROCEDURE_TERM = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_CONCAT_NULL_BEHAVIOR, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_CONCAT_NULL_BEHAVIOR = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_SCHEMA_TERM, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_SCHEMA_TERM = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_CURSOR_COMMIT_BEHAVIOR, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_CURSOR_COMMIT_BEHAVIOR = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_SCROLL_OPTIONS, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_SCROLL_OPTIONS = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_CURSOR_ROLLBACK_BEHAVIOR, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_CURSOR_ROLLBACK_BEHAVIOR = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_TABLE_TERM, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_TABLE_TERM = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_CURSOR_SENSITIVITY, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_CURSOR_SENSITIVITY = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_TXN_CAPABLE, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_TXN_CAPABLE = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DATA_SOURCE_READ_ONLY, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DATA_SOURCE_READ_ONLY = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_TXN_ISOLATION_OPTION, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_TXN_ISOLATION_OPTION = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DEFAULT_TXN_ISOLATION, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DEFAULT_TXN_ISOLATION = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_USER_NAME, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_USER_NAME = %s\n", sBuffer1);
                    if (SQL_SUCCEEDED (SQLGetInfo (ODBC->dbc, SQL_DESCRIBE_PARAMETER, (SQLPOINTER) sBuffer1, sizeof (sBuffer1), NULL))) printf ("SQL_DESCRIBE_PARAMETER = %s\n", sBuffer1);
                    printf ("\n");
#endif*/
					//preparar consulta pré-definida
					if (SQL_SUCCEEDED (SQLAllocHandle (SQL_HANDLE_STMT, ODBC->dbc, &stmt)))
					{
						//obter a lista de tabelas
						if (SQL_SUCCEEDED (SQLTables (stmt, (SQLCHAR *) sCatalogo, sCatalogo ? SQL_NTS : 0, (SQLCHAR *) sEsquema, sEsquema ? SQL_NTS : 0, (SQLCHAR *) sTabela, sTabela ? SQL_NTS : 0, (SQLCHAR *) sTypes, sTypes ? SQL_NTS : 0)))
						{
							//loop nos registros para retornar todas as tabelas
							while (SQL_SUCCEEDED (SQLFetch (stmt)))//retornar linhas de registros
							{
								sBuffer1[0] = '\0';
								sBuffer2[0] = '\0';
								sBuffer3[0] = '\0';
								sBuffer4[0] = '\0';
								SQLGetData (stmt, 1, SQL_C_CHAR, sBuffer1, sizeof (sBuffer1), NULL);//BD/Catalogo
								SQLGetData (stmt, 2, SQL_C_CHAR, sBuffer2, sizeof (sBuffer2), NULL);//Schema
								SQLGetData (stmt, 3, SQL_C_CHAR, sBuffer3, sizeof (sBuffer3), NULL);//Tabela
								SQLGetData (stmt, 4, SQL_C_CHAR, sBuffer4, sizeof (sBuffer4), NULL);//Tipo
								//adicionar tabelas
								if (strlen (sBuffer3))
								{
                                    //criando estrutura de base de dados
                                    Tabela = g_new (BEODBC_Tabela, 1);
                                    Tabela->ODBC = ODBC;
                                    Tabela->sNome = g_strdup (sBuffer3);
                                    Tabela->sTipo = g_strdup (sBuffer4);
                                    if (strlen (sBuffer1))
                                        Tabela->sCatalogo = g_strdup (sBuffer1);
                                    else
                                        Tabela->sCatalogo = NULL;
                                    if (strlen (sBuffer2))
                                        Tabela->sEsquema = g_strdup (sBuffer2);
                                    else
                                        Tabela->sEsquema = NULL;
                                    Tabela->nFlags = 0;//acesso
                                    //anexando na lista
                                    ODBC->Tabelas = g_slist_append (ODBC->Tabelas, Tabela);
                                }
                                /*if (SQL_SUCCEEDED (SQLGetData (stmt, 1, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                                    printf ("%s - ", sBuffer);
                                if (SQL_SUCCEEDED (SQLGetData (stmt, 2, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                                    printf ("%s - ", sBuffer);
                                if (SQL_SUCCEEDED (SQLGetData (stmt, 3, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                                    printf ("%s - ", sBuffer);
                                if (SQL_SUCCEEDED (SQLGetData (stmt, 4, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                                    printf ("%s\n", sBuffer);
                                */
                            }
                        }
                        else
                        {
                            _be_odbc_erro (sErros, "SQLTables", stmt, SQL_HANDLE_STMT);
                            bErro = TRUE;
                        }
                        //liberando memória
                        SQLFreeHandle (SQL_HANDLE_STMT, stmt);
                    }
                    else
                    {
                        _be_odbc_erro (sErros, "SQLAllocHandle", stmt, SQL_HANDLE_STMT);
                        bErro = TRUE;
                    }

                    //preparar consulta pré-definida, Flags das tabelas
                    /*
                        -No MSSQL funciona normalmente.
                        -No Oracle 8 fica num loop infinito.
                        -No MySQL não há resposta nenhuma.
                    */
                    /*if (!bErro && ODBC->Tabelas)
                    {
                        //liberar banco de dados
                        for (ListaL = ODBC->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
                        {
                            Tabela = (BEODBC_Tabela *) ListaL->data;
                            //alocalr memória
                            if (SQL_SUCCEEDED (SQLAllocHandle (SQL_HANDLE_STMT, ODBC->dbc, &stmt)))
                            {
                                //obter mais informações das tabelas
                                if (SQL_SUCCEEDED (SQLTablePrivileges (stmt, Tabela->sCatalogo, Tabela->sCatalogo ? SQL_NTS : 0, Tabela->sEsquema, Tabela->sEsquema ? SQL_NTS : 0, Tabela->sNome, Tabela->sNome ? SQL_NTS : 0)))
                                {
                                    //loop nos registros para retornar todas as iknformações extras
                                    while (SQL_SUCCEEDED (SQLFetch (stmt)))//retornar linhas de registros
                                    {
                                        if (SQL_SUCCEEDED (SQLGetData (stmt, 6, SQL_C_CHAR, sBuffer1, sizeof (sBuffer1), NULL)))
                                        {
                                            if (!(Tabela->nFlags & BEODBC_ACESSO_SELECIONAR) && g_str_equal (sBuffer1, "SELECT"))
                                                Tabela->nFlags += BEODBC_ACESSO_SELECIONAR;
                                            if (!(Tabela->nFlags & BEODBC_ACESSO_INSERIR) && g_str_equal (sBuffer1, "INSERT"))
                                                Tabela->nFlags += BEODBC_ACESSO_INSERIR;
                                            if (!(Tabela->nFlags & BEODBC_ACESSO_ALTERAR) && g_str_equal (sBuffer1, "UPDATE"))
                                                Tabela->nFlags += BEODBC_ACESSO_ALTERAR;
                                            if (!(Tabela->nFlags & BEODBC_ACESSO_APAGAR) && g_str_equal (sBuffer1, "DELETE"))
                                                Tabela->nFlags += BEODBC_ACESSO_APAGAR;
                                        }
                                    }
                                }
                                else
                                {
                                    _be_odbc_erro (sErros, "SQLTablePrivileges", stmt, SQL_HANDLE_STMT);
                                    bErro = TRUE;
                                }
                                //liberando memória
                                SQLFreeHandle (SQL_HANDLE_STMT, stmt);
                            }
                        }
                    }*/
                }
                else
                {
                    _be_odbc_erro (sErros, "SQLDriverConnect", ODBC->dbc, SQL_HANDLE_DBC);
                    bErro = TRUE;
                }
            }
            else
            {
                _be_odbc_erro (sErros, "SQLAllocHandle", ODBC->dbc, SQL_HANDLE_DBC);
                bErro = TRUE;
            }
        }
        else
        {
            _be_odbc_erro (sErros, "SQLSetEnvAttr", ODBC->dbc, SQL_HANDLE_DBC);
            bErro = TRUE;
        }
    }
    else
    {
        _be_odbc_erro (sErros, "SQLAllocHandle", ODBC->env, SQL_HANDLE_ENV);
        bErro = TRUE;
    }

    if (bErro)
    {
        BE_odbc_fechar_conexao (ODBC);
    }

	return ODBC;
}

void _be_odbc_fechar_conexao (BEODBC *ODBC)
{
	BEODBC_Tabela *Tabela = NULL;
	GSList *ListaL = NULL;

	if (ODBC)
	{
		//desconectando
		SQLDisconnect (ODBC->dbc);
		//liberando memória
        SQLFreeHandle (SQL_HANDLE_DBC, ODBC->dbc);
        SQLFreeHandle (SQL_HANDLE_ENV, ODBC->env);
		//liberando estrutura
		BE_free (ODBC->sConexao);
		//liberar tabelas
		for (ListaL = ODBC->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
		{
			Tabela = (BEODBC_Tabela *) ListaL->data;
#ifdef BEODBC_DEPURACAO
			printf ("DEPURACAO: %s:%d - \t%s.%s.%s - %s - %d\n", __FILE__, __LINE__, Tabela->sCatalogo, Tabela->sEsquema, Tabela->sNome, Tabela->sTipo, Tabela->nFlags);
#endif
            BE_free (Tabela->sNome);
            BE_free (Tabela->sTipo);
            BE_free (Tabela->sCatalogo);
            BE_free (Tabela->sEsquema);
		}
		BE_slist_free (ODBC->Tabelas);
		//liberar estrutura principal
		BE_free (ODBC);
	}

	//fechar arquivo de depuração
	if (FileODBC)
	{
		fwrite ("\n", 1, 1, FileODBC);
		fclose (FileODBC);
	}
}

const BEODBC_Tabela *be_odbc_selecionar_tabela (const BEODBC *ODBC, const gchar *sNome, const gchar *sCatalogo, const gchar *sEsquema)
{
	BEODBC_Tabela *Tabela = NULL;
	GSList *ListaL = NULL;

	if (sNome && ODBC)
	{
		for (ListaL = ODBC->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
		{
			Tabela = (BEODBC_Tabela *) ListaL->data;
			if (g_str_equal (Tabela->sNome, sNome))
                if ((!sCatalogo && !Tabela->sCatalogo) || (sCatalogo && Tabela->sCatalogo && g_str_equal (Tabela->sCatalogo, sCatalogo)))
                    if ((!sEsquema && !Tabela->sEsquema) || (sEsquema && Tabela->sEsquema && g_str_equal (Tabela->sEsquema, sEsquema)))
                        return Tabela;
		}
	}
	return NULL;
}

BEODBC_Sintaxe *be_odbc_abrir_consulta (const BEODBC_Tabela *Tabela, gboolean bForcarNulos, gchar **sErros)
{
	const BEODBC *ODBC = NULL;
	BEODBC_Sintaxe *Sintaxe = NULL;
	BEODBC_Campo *Campo = NULL;
	//BEODBC_Indice *Indice = NULL;
	//MYSQL_ROW row = NULL;//linhas
	//BEODBC_Tabela *Tabela1 = NULL;
	SQLHSTMT stmt;//resultado da consulta
	gchar sBuffer[512];
	//GSList *ListaL = NULL;
	//gchar **aLista = NULL;
	//gchar *sSQL = NULL;
	//gint nCont = 0;//, nCont1 = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

	if (!Tabela)
		return NULL;

	//comunicação
	ODBC = Tabela->ODBC;
	//criar estrutura
	Sintaxe = g_new (BEODBC_Sintaxe, 1);
	Sintaxe->Tabela = Tabela;
	Sintaxe->sConsulta = NULL;
	Sintaxe->Campos = NULL;
	Sintaxe->Indices = NULL;
	Sintaxe->Filtros = NULL;
	Sintaxe->FiltrosUniao = NULL;
	Sintaxe->Ordens = NULL;
	Sintaxe->lLinhasPaginaMax = BEODBC_MAX_LINHAS_POR_PAGINA;
	Sintaxe->lLinhasPagina = 0;
	Sintaxe->lLinhas = -1;//desconhecido
	Sintaxe->lLinha = 0;
	Sintaxe->bForcarNulos = bForcarNulos;//converter comprimento zero em nulos

    //carregar todos os campos disponíveis
    if (SQL_SUCCEEDED (SQLAllocHandle (SQL_HANDLE_STMT, ODBC->dbc, &stmt)))
    {
        //obter a lista de campos
        if (SQL_SUCCEEDED (SQLColumns (stmt, (SQLCHAR *) Tabela->sCatalogo, Tabela->sCatalogo ? SQL_NTS : 0, (SQLCHAR *) Tabela->sEsquema, Tabela->sEsquema ? SQL_NTS : 0, (SQLCHAR *) Tabela->sNome, Tabela->sNome ? SQL_NTS : 0, NULL, 0)))
        //if (SQL_SUCCEEDED (SQLColumns (stmt, Tabela->sCatalogo, Tabela->sCatalogo ? SQL_NTS : 0, Tabela->sEsquema, Tabela->sEsquema ? SQL_NTS : 0, Tabela->sNome, Tabela->sNome ? SQL_NTS : 0, "%", SQL_NTS)))
        {
            //loop nos registros para retornar todas as tabelas
            while (SQL_SUCCEEDED (SQLFetch (stmt)))//retornar linhas de registros
            {
                //reuperando as informações
                /*for (nCont = 1; nCont < 19; nCont++)
                {
                    if (SQL_SUCCEEDED (SQLGetData (stmt, nCont, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                        printf ("%s - ", sBuffer);
                }
                printf ("\n");*/
                //Nome = 4
                //Anulavel = 11
                //Alteravel
                //ComprimentoMax = 7
                //sTipo = 6
                //nTipo = 5
                //AutoIncremento
                //Valor Padrão = 13

				//criando campo
				Campo = g_new (BEODBC_Campo, 1);
				Campo->Sintaxe = Sintaxe;
				if (SQL_SUCCEEDED (SQLGetData (stmt, 4, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                    Campo->sNome = g_strdup (sBuffer);
                else
                    Campo->sNome = NULL;
				if (SQL_SUCCEEDED (SQLGetData (stmt, 5, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                    Campo->nTipo = (gint) g_strtod (sBuffer, NULL);
				else
                    Campo->nTipo = -1;//indefinido
				if (SQL_SUCCEEDED (SQLGetData (stmt, 6, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                    Campo->sTipo = g_strdup (sBuffer);//tipo
                else
                    Campo->sTipo = NULL;
				if (SQL_SUCCEEDED (SQLGetData (stmt, 7, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                    Campo->nComprimentoMax = (gint) g_strtod (sBuffer, NULL);
				else
                    Campo->nComprimentoMax = 0;
				Campo->nCampoPos = -1;//disponível somente após uma execução de uma consulta
				Campo->sValor = NULL;//disponível somente após uma execução de uma consulta
				if (SQL_SUCCEEDED (SQLGetData (stmt, 13, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                    Campo->sValorPadrao = g_strdup (sBuffer);
                else
                    Campo->sValorPadrao = NULL;
				if (SQL_SUCCEEDED (SQLGetData (stmt, 18, SQL_C_CHAR, sBuffer, sizeof (sBuffer), NULL)))
                {
                    if (g_str_equal (sBuffer, "NO"))
                        Campo->bAnulavel = FALSE;
                    else
                        Campo->bAnulavel = TRUE;
                }
                else
                    Campo->bAnulavel = FALSE;
				Campo->bAlteravel = FALSE;//*************   IMPLEMENTAÇÃO POSTERIOR
                Campo->nComprimento = 0;//disponível somente após uma execução de uma consulta
				Campo->bAtivo = TRUE;
				Campo->bAutoID = FALSE;//*************   IMPLEMENTAÇÃO POSTERIOR
				Campo->sValorNovo = NULL;
				Campo->nComprimentoNovo = 0;
				Campo->bAfetar = FALSE;
				//anexando na lista
				Sintaxe->Campos = g_slist_append (Sintaxe->Campos, Campo);
            }
        }
        else
        {
            _be_odbc_erro (sErros, "SQLTables", stmt, SQL_HANDLE_STMT);
        }
        //liberando memória
        SQLFreeHandle (SQL_HANDLE_STMT, stmt);
    }
    else
    {
        _be_odbc_erro (sErros, "SQLAllocHandle", stmt, SQL_HANDLE_STMT);
    }


	//carregar todos os campos disponíveis
	/*BE_free (sSQL);
	sSQL = g_strdup_printf ("SHOW COLUMNS FROM `%s`.`%s`", Tabela->BD->sNome, Tabela->sNome);//EXPLAIN `coluna`
	_be_sql_saida (sSQL);//depuração
	if (mysql_query (ODBC->_my, sSQL) == 0)
	{
		if ((Sintaxe->res = mysql_store_result (ODBC->_my)))//recebe a consulta
		{
			for (nCont = 0; nCont < mysql_num_rows (Sintaxe->res); nCont++)
			{
				//criando campo
				row = mysql_fetch_row (Sintaxe->res);
				Campo = g_new (BEODBC_Campo, 1);
				Campo->Sintaxe = Sintaxe;
				Campo->sNome = g_strdup (row[0]);
				Campo->nCampoPos = -1;//disponível somente após uma execução de uma consulta
				Campo->sValor = NULL;//disponível somente após uma execução de uma consulta
				if (g_str_equal (row[2], "NO"))
					Campo->bAnulavel = FALSE;
				else
					Campo->bAnulavel = TRUE;
				Campo->bAlteravel = FALSE;//verificado logo abaixo
				Campo->nComprimento = 0;//disponível somente após uma execução de uma consulta
				Campo->nComprimentoMax = 0;//disponível somente após uma execução de uma consulta
				Campo->nComprimentoTop = 0;//disponível somente após uma execução de uma consulta
				Campo->bAtivo = TRUE;
				Campo->sTipo = g_strdup (row[1]);//tipo
				Campo->bAutoID = FALSE;
				//verificar campo auto_increment
				if (row[5] && g_str_equal (row[5], BEODBC_AUTO_ID))
					Campo->bAutoID = TRUE;
				//convesão numérica de tipos
				if (g_strrstr (Campo->sTipo, "tinyint(1)") || g_strrstr (Campo->sTipo, "bit"))
					Campo->nTipo = MYSQL_TYPE_BIT;
				else if (g_strrstr (Campo->sTipo, "tinyint"))
					Campo->nTipo = MYSQL_TYPE_TINY;
				else if (g_strrstr (Campo->sTipo, "smallint"))
					Campo->nTipo = MYSQL_TYPE_SHORT;
				else if (g_strrstr (Campo->sTipo, "mediumint"))
					Campo->nTipo = MYSQL_TYPE_INT24;
				else if (g_strrstr (Campo->sTipo, "bigint"))
					Campo->nTipo = MYSQL_TYPE_LONGLONG;
				else if (g_strrstr (Campo->sTipo, "int"))
					Campo->nTipo = MYSQL_TYPE_LONG;
				else if (g_strrstr (Campo->sTipo, "dec") || g_strrstr (Campo->sTipo, "numeric"))
					Campo->nTipo = MYSQL_TYPE_DECIMAL;
				else if (g_strrstr (Campo->sTipo, "float"))
					Campo->nTipo = MYSQL_TYPE_FLOAT;
				else if (g_strrstr (Campo->sTipo, "double") || g_strrstr (Campo->sTipo, "real"))
					Campo->nTipo = MYSQL_TYPE_DOUBLE;
				else if (g_strrstr (Campo->sTipo, "timestamp"))
					Campo->nTipo = MYSQL_TYPE_TIMESTAMP;
				else if (g_strrstr (Campo->sTipo, "datetime"))
					Campo->nTipo = MYSQL_TYPE_DATETIME;
				else if (g_strrstr (Campo->sTipo, "date"))
					Campo->nTipo = MYSQL_TYPE_DATE;
				else if (g_strrstr (Campo->sTipo, "time"))
					Campo->nTipo = MYSQL_TYPE_TIME;
				else if (g_strrstr (Campo->sTipo, "year"))
					Campo->nTipo = MYSQL_TYPE_YEAR;
				else if (g_strrstr (Campo->sTipo, "char"))
					Campo->nTipo = MYSQL_TYPE_STRING;
				else if (g_strrstr (Campo->sTipo, "varchar"))
					Campo->nTipo = MYSQL_TYPE_VAR_STRING;
				else if (g_strrstr (Campo->sTipo, "tinytext"))
					Campo->nTipo = BEODBC_TYPE_TINY_TEXT;
				else if (g_strrstr (Campo->sTipo, "mediumtext"))
					Campo->nTipo = BEODBC_TYPE_MEDIUM_TEXT;
				else if (g_strrstr (Campo->sTipo, "longtext"))
					Campo->nTipo = BEODBC_TYPE_LONG_TEXT;
				else if (g_strrstr (Campo->sTipo, "text"))
					Campo->nTipo = BEODBC_TYPE_TEXT;
				else if (g_strrstr (Campo->sTipo, "tinyblob"))
					Campo->nTipo = MYSQL_TYPE_TINY_BLOB;
				else if (g_strrstr (Campo->sTipo, "mediumblob"))
					Campo->nTipo = MYSQL_TYPE_MEDIUM_BLOB;
				else if (g_strrstr (Campo->sTipo, "longblob"))
					Campo->nTipo = MYSQL_TYPE_LONG_BLOB;
				else if (g_strrstr (Campo->sTipo, "blob"))
					Campo->nTipo = MYSQL_TYPE_BLOB;
				else if (g_strrstr (Campo->sTipo, "set"))
					Campo->nTipo = MYSQL_TYPE_SET;
				else if (g_strrstr (Campo->sTipo, "enum"))
					Campo->nTipo = MYSQL_TYPE_ENUM;
				else
					Campo->nTipo = -1;

				//MYSQL_TYPE ... máximno de 255
				//MYSQL_TYPE_DECIMAL = 0

				Campo->sParametros = NULL;//parâmetros extra
				Campo->sGrupo = NULL;//grupo extra
				Campo->sComentarios = NULL;//comentários extra
				Campo->sValorPadrao = g_strdup (row[4]);//valor default, pode ser extra
				Campo->sTotal = NULL;
				Campo->sValorNovo = NULL;
				Campo->nComprimentoNovo = 0;
				Campo->bAfetar = FALSE;
				//recuperar informações extra
				_be_sql_campo_CIA (Campo, sErros);
				//anexando na lista
				Sintaxe->Campos = g_slist_append (Sintaxe->Campos, Campo);
			}
			mysql_free_result (Sintaxe->res);
		}
		else//erro no retorno
		{
			_be_sql_erro_retorno (sErros);
		}
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (ODBC, sErros);
	}

	//mascarar valor padrão
	_be_sql_mascarar_valor_padrao (Sintaxe, sErros);

	//carregar todos os índices disponíveis
	{
		BE_free (sSQL);
		sSQL = g_strdup_printf ("SHOW INDEX FROM `%s`.`%s`", Tabela->BD->sNome, Tabela->sNome);
		_be_sql_saida (sSQL);//depuração
		if (mysql_query (ODBC->_my, sSQL) == 0)
		{
			if ((Sintaxe->res = mysql_store_result (ODBC->_my)))//recebe a consulta
			{
				for (nCont = 0; nCont < mysql_num_rows (Sintaxe->res); nCont++)
				{
					//recuperar campos
					row = mysql_fetch_row (Sintaxe->res);
					//criar novo indice
					Indice = g_new (BEODBC_Indice, 1);
					Indice->Sintaxe = Sintaxe;
					Indice->sNome = g_strdup (row[2]);
					Indice->nSequencia = (gint) atol (row[3]);
					Indice->sValorFiltro = NULL;
					Indice->nComprimentoFiltro = 0;
					Indice->bAfetar = FALSE;
					//relacionar com campo
					Indice->Campo = be_sql_selecionar_campo (Sintaxe, row[4]);
					//verificar tipo
					if (g_str_equal (Indice->sNome, "PRIMARY"))
						Indice->nTipo = BEODBC_INDICE_PRIMARIA;
					else if (atol (row[1]) == 0)
						Indice->nTipo = BEODBC_INDICE_UNICA;
					else
						Indice->nTipo = BEODBC_INDICE_SIMPLES;
					//anexando na lista
					Sintaxe->Indices = g_slist_append (Sintaxe->Indices, Indice);
				}
				mysql_free_result (Sintaxe->res);
			}
			else//erro no retorno
			{
				_be_sql_erro_retorno (sErros);
			}
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (ODBC, sErros);
		}
	}

	//verificar permissões por campo
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BEODBC_Campo *) ListaL->data;
		//excutar consulta de verificação
		BE_free (sSQL);
		sSQL = g_strdup_printf ("SELECT `PRIVILEGE_TYPE` FROM `information_schema`.`COLUMN_PRIVILEGES` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='%s' AND COLUMN_NAME='%s'", Tabela->BD->sNome, Tabela->sNome, Campo->sNome);
		_be_sql_saida (sSQL);//depuração
		if (mysql_query (ODBC->_my, sSQL) == 0)
		{
			if ((Sintaxe->res = mysql_store_result (ODBC->_my)))//recebe a consulta
			{
				for (nCont = 0; nCont < mysql_num_rows (Sintaxe->res); nCont++)
				{
					//recuperar campo
					row = mysql_fetch_row (Sintaxe->res);
					if (g_str_equal (row[0], "UPDATE"))
						Campo->bAlteravel = TRUE;
				}
				mysql_free_result (Sintaxe->res);
				//valor padrão
				if (nCont == 0)
					Campo->bAlteravel = (Tabela->nFlags & BEODBC_ACESSO_ALTERAR) == BEODBC_ACESSO_ALTERAR;
			}
			else//erro no retorno
			{
				_be_sql_erro_retorno (sErros);
			}
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (ODBC, sErros);
		}
	}
	BE_free (sSQL);*/

	return Sintaxe;
}

void _be_odbc_fechar_consulta (BEODBC_Sintaxe *Sintaxe)
{
	BEODBC_Campo *Campo = NULL;
	//BEODBC_Indice *Indice = NULL;
	//BEODBC_Filtro *Filtro = NULL;
	//BEODBC_Ordem *Ordem = NULL;
	GSList *ListaL = NULL;

	if (!Sintaxe)
		return;

	BE_free (Sintaxe->sConsulta);
	//índices
	/*for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
	{
		Indice = (BEODBC_Indice *) ListaL->data;
#ifdef BEODBC_DEPURACAO
		printf ("DEPURACAO: %s:%d - \t\t\tINDEX: %s (%s) - %d %d\n", __FILE__, __LINE__, Indice->Campo->sNome, Indice->sNome, Indice->nTipo, Indice->nSequencia);
#endif
		BE_free (Indice->sNome);
		BE_free (Indice->sValorFiltro);
		BE_free (Indice);
	}
	BE_slist_free (Sintaxe->Indices);*/
	//campos
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BEODBC_Campo *) ListaL->data;
#ifdef BEODBC_DEPURACAO
		printf ("DEPURACAO: %s:%d - \t\t%s %d(%s) %d = %s c:%d\n", __FILE__, __LINE__, Campo->sNome, Campo->nTipo, Campo->sTipo, Campo->bAlteravel, Campo->sValorPadrao, Campo->nComprimentoMax);
#endif
		BE_free (Campo->sNome);
		BE_free (Campo->sTipo);
		BE_free (Campo->sValorPadrao);
		BE_free (Campo->sValorNovo);
		BE_free (Campo);
	}
	BE_slist_free (Sintaxe->Campos);
	//filtros
	/*for (ListaL = Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
	{
		Filtro = (BEODBC_Filtro *) ListaL->data;
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	BE_slist_free (Sintaxe->Filtros);
	//filtros união
	for (ListaL = Sintaxe->FiltrosUniao; ListaL; ListaL = g_slist_next (ListaL))
	{
		Filtro = (BEODBC_Filtro *) ListaL->data;
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	BE_slist_free (Sintaxe->FiltrosUniao);
	//ordens
	for (ListaL = Sintaxe->Ordens; ListaL; ListaL = g_slist_next (ListaL))
	{
		Ordem = (BEODBC_Ordem *) ListaL->data;
		BE_free (Ordem);
	}
	BE_slist_free (Sintaxe->Ordens);*/

	BE_free (Sintaxe);
}

gint be_odbc_executar (
    const BEODBC *ODBC,
    const gchar *sSintaxe,
    gboolean (*func_Ret)(const BEODBC *ODBC, gint nCampos, gint nLinha, GSList *Campos, gint nTipo, gint nComp, GSList *Valores, gpointer user_data),
    gpointer user_data,
    gchar **sErros
)
{
	/*
	FALTA IMPLEMENTAÇÃO DE RETORNO DO UPDATE, INSERT, DELETE, ETC.

	*/

	SQLHSTMT stmt;//resultado da consulta
	gchar sNome[255], sValor[1024];
	gchar *sBuffer = NULL;
	GSList *Campos = NULL, *Valores = NULL;
	GSList *ListaL = NULL;
	gchar *sConsulta = NULL;
	SQLSMALLINT nCols = 0;
	SQLSMALLINT nTipo = 0;
	SQLUINTEGER nComp = 0;
	gint nCont = 0, nLinha = 0;
	gboolean bContinuar = TRUE;

	sConsulta = g_strdup (sSintaxe);

    //carregar todos os campos disponíveis
    if (SQL_SUCCEEDED (SQLAllocHandle (SQL_HANDLE_STMT, ODBC->dbc, &stmt)))
    {
        //obter a lista de campos
        if (SQL_SUCCEEDED (SQLExecDirect (stmt, (SQLCHAR *) sConsulta, strlen (sConsulta))))
        {
            //quantidade de campos
            SQLNumResultCols (stmt, &nCols);
            //loop nos campos, nomes
            for (nCont = 0; nCont < nCols; nCont++)
            {
                sNome[0] = '\0';
                if (nLinha == 0)
                {
                    if (!SQL_SUCCEEDED (SQLDescribeCol (stmt, nCont+1, (SQLCHAR *) sNome, sizeof (sNome), NULL, &nTipo, &nComp, NULL, NULL)))
                    {
                        sNome[0] = '\0';
                    }
                    //adicionar nomes dos campos
                    Campos = g_slist_append (Campos, g_strdup (sNome));
                }
            }
            //loop nas linhas de dados
            while (SQL_SUCCEEDED (SQLFetch (stmt)) && bContinuar)
            {
                //loop nos campos, valores
                for (nCont = 0; nCont < nCols; nCont++)
                {
                    //retornar valores
                    sValor[0] = '\0';
                    if (!SQL_SUCCEEDED (SQLGetData (stmt, nCont+1, SQL_C_CHAR, sValor, sizeof (sValor), NULL)))
                    {
                        sValor[0] = '\0';
                    }
                    //adicionar valores dos campos
                    Valores = g_slist_append (Valores, g_strdup (sValor));
                }
                //retornar valores, por callback
                if (func_Ret)
                    bContinuar = func_Ret (ODBC, nCols, nLinha, Campos, nTipo, nComp, Valores, user_data);
                //contar linhas
                nLinha++;
                //limpar valores
                for (ListaL = Valores; ListaL; ListaL = g_slist_next (ListaL))
                {
                    sBuffer = (gchar *) ListaL->data;
                    BE_free (sBuffer);
                }
                BE_slist_free (Valores);
            }
            //limpar campos
            for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
            {
                sBuffer = (gchar *) ListaL->data;
                BE_free (sBuffer);
            }
            BE_slist_free (Campos);
        }
        else
        {
            _be_odbc_erro (sErros, "SQLExecDirect", stmt, SQL_HANDLE_STMT);
        }
        //liberando memória
        SQLFreeHandle (SQL_HANDLE_STMT, stmt);
    }
    else
    {
        _be_odbc_erro (sErros, "SQLAllocHandle", stmt, SQL_HANDLE_STMT);
    }

    return nLinha;
}

void __be_odbc_erro (const gchar *sArquivo, gint nLinha, gchar **sErros, const gchar *sNome, SQLHANDLE handle, SQLSMALLINT type)
{
	GString *Buffer = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar sEstado[7];
	gchar sMensagem[256];
	gint nRec = 1;

	Buffer = g_string_new ("");
	//loop dos erros
	while (SQL_SUCCEEDED (SQLGetDiagRec (type, handle, nRec, (SQLCHAR *) sEstado, NULL, (SQLCHAR *) sMensagem, sizeof (sMensagem), NULL)))
	{
        g_string_append_printf (Buffer, "Estado: %s - Mensagem: %s\n", sEstado, sMensagem);
        nRec++;
	}

	if (sErros)
	{
		BE_free (*sErros);
		sTemp1 = BE_string_free (Buffer);
		if (sTemp1 && strlen (sTemp1) > 0)
            sTemp = g_strdup_printf ("Erro: %s - %s", sNome, sTemp1);
        else
            sTemp = g_strdup_printf ("Erro: %s", sNome);
		*sErros = BE_ISO_UTF8 (sTemp)
		BE_free (sTemp);
		BE_free (sTemp1);
	}
	else
	{
        sTemp = BE_string_free (Buffer);
        if (sTemp && strlen (sTemp) > 0)
            printf ("%s:%d Erro: %s - %s", sArquivo, nLinha, sNome, sTemp);
        else
            printf ("%s:%d Erro: %s", sArquivo, nLinha, sNome);
		BE_free (sTemp);
	}
}
