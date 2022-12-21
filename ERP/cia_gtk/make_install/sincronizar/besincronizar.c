#include "besincronizar.h"

/*******************************************************************************
Contém funções para sincronizar o banco de dados atual com fontes externas.
Estas fontes externas podem ser outro banco de dados, em ODBC,
ou através de um site, via HTTP (GET e POST), HTTPS, FTP, FTPS, SFTP, Downloads,
PROXY, Cookies, Telnet, Emails (POP3 e SMTP).
Através de configurações em arquivos INI, é utilizado uma interface com as
bibliotecas beSQL, beODBC e beNet
As ações de sincronismo geram um arquivo de Log.
*******************************************************************************/

//variável estática, ponteiro para arquivo de Log de ações.
static FILE *FileLOG = NULL;

void be_sincronizar_log_iniciar (gint nQuantidade)
{
/*******************************************************************************
Função para iniciar o Log de ações.
Cria o endereço para o ponteiro estático, FileLOG.
Argumentos:
    nQuantidade : Quantidade de arquivos que devem permanecer
    Sem retorno.
*******************************************************************************/
	GDir *Diretorio = NULL;
	GError *Erros = NULL;
	GList *Lista = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	const gchar *sArquivos = NULL;
	gint nCont = 0;

	//listar os arquivos de logs
	if (nQuantidade > 0)
	{
		if ((Diretorio = g_dir_open ("./", 0, &Erros)))
		{
			//loop dos arquivos
			while ((sArquivos = g_dir_read_name (Diretorio)))
				if (g_strrstr (sArquivos, ".log"))
					Lista = g_list_append (Lista, g_strdup (sArquivos));
			//ordenar os arquivos por nome 
			Lista = g_list_sort (Lista, be_string_ordenar_string);
			for (Lista = g_list_last (Lista), nCont = 1; Lista; Lista = g_list_previous (Lista), nCont++)
			{
				if (nCont > nQuantidade)
					remove ((gchar *) Lista->data);
				//limpar nomes dos arquivos
				BE_free (Lista->data);
			}
		}
		//informar erro
		else if (Erros && Erros->message)
			printf ("Erro: %s\n", Erros->message);
	}
	//abrir arquivo, log de ações
	BE_free (sTemp);//limpar
	BE_free (sTemp1);//limpar
	sTemp = BE_tempo ("%Y-%m-%d");
	sTemp1 = g_strdup_printf ("%s_%s.log", BESINCRONIZAR_LOG, sTemp);
	if ((FileLOG = fopen (sTemp1, "ab")))//adicionar binário
	{
		//escrever início
		fwrite ("Iniciar: ", 9, 1, FileLOG);
		//incluir data e hora atual
		sTemp = BE_tempo ("%d/%m/%Y %H:%M:%S");
		fwrite (sTemp, strlen (sTemp), 1, FileLOG);
		BE_free (sTemp);//limpar
		fwrite ("\n", 1, 1, FileLOG);
	}	
	//limpar
	g_dir_close (Diretorio);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_list_free (Lista);
}

void be_sincronizar_log_fechar ()
{
/*******************************************************************************
Função para fechar o Log de ações.
Finaliza o endereço para o ponteiro estático, FileLOG.
Argumentos:
    Sem argumentos.
    Sem retorno.
*******************************************************************************/
	gchar *sTemp = NULL;

	//fechar arquivo
	if (FileLOG)
	{
		//escrever Fim
		fwrite ("Finalizar: ", 11, 1, FileLOG);
		//incluir data e hora atual
		sTemp = BE_tempo ("%d/%m/%Y %H:%M:%S");
		fwrite (sTemp, strlen (sTemp), 1, FileLOG);
		BE_free (sTemp);
		fwrite ("\n\n", 2, 1, FileLOG);
	}
	//fechar arquivo
	fclose (FileLOG);
	FileLOG = NULL;
}

void _be_sincronizar_log_informar (gchar *sInfo)
{
/*******************************************************************************
Escreve informações no arquivo de Log de ações
Somente a função BE_sincronizar_log_informar pode chamar esta.
Função reservado para uso interno.
Argumentos:
    sInfo : informação
    Sem retorno.
*******************************************************************************/
    //verifica arquivo de Log
    if (FileLOG)
    {
        fwrite (sInfo, strlen (sInfo), 1, FileLOG);
        BE_free (sInfo);
    }
}

BESQL *be_sincronizar_autenticar (const gchar *sArquivo)
{
/*******************************************************************************
Iniciar conexão com o banco de dados atual.
Argumentos:
    sArquivo : arquivo com as ações, em formato INI
    Retorno: conexão com banco de dados
*******************************************************************************/
	BESQL *SQL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sTemp = NULL, *sIP = NULL, *sUsuario = NULL, *sSenha = NULL, *sChavePrivada = NULL, *sCertificado = NULL, *sAutoridade = NULL, *sCaminhoAutoridade = NULL, *sCifras = NULL;
	gchar *sErros = NULL;
	guint nPorta = 0;

	//ler arquivo de congiruação INI
	if ((Ini = be_sincronizar_iniciar_ini (sArquivo)))
	{
		//recuperar servidor
		sIP = g_key_file_get_string (Ini, "servidor", "ip", NULL);
		if (!sIP || strlen (sIP) == 0)
		{
			BE_free (sIP);
		}
		//recuperar porta
		sTemp = g_key_file_get_string (Ini, "servidor", "porta", NULL);
		if (sTemp)
			nPorta = atoi (sTemp);
		//recuperar usuário e senha
		sUsuario = g_key_file_get_string (Ini, "servidor", "usuario", NULL);
		sSenha = g_key_file_get_string (Ini, "servidor", "senha", NULL);
		//recuperar chave privada SSL
		sChavePrivada = g_key_file_get_string (Ini, "seguranca", "chaveprivada", NULL);
		if (sChavePrivada && strlen (sChavePrivada) == 0)
		{
		    BE_free (sChavePrivada);
		}
		BE_free (sTemp);
		sTemp = sChavePrivada;
		sChavePrivada = be_arquivo_absoluto (sTemp);
		//recuperar certificado SSL
		sCertificado = g_key_file_get_string (Ini, "seguranca", "certificado", NULL);
		if (sCertificado && strlen (sCertificado) == 0)
		{
		    BE_free (sCertificado);
		}
		BE_free (sTemp);
		sTemp = sCertificado;
		sCertificado = be_arquivo_absoluto (sTemp);
		//recuperar autoridade SSL
		sAutoridade = g_key_file_get_string (Ini, "seguranca", "autoridade", NULL);
		if (sAutoridade && strlen (sAutoridade) == 0)
		{
		    BE_free (sAutoridade);
		}
		BE_free (sTemp);
		sTemp = sAutoridade;
		sAutoridade = be_arquivo_absoluto (sTemp);
		//recuperar caminho de autoridade SSL
		sCaminhoAutoridade = g_key_file_get_string (Ini, "seguranca", "caminhoautoridade", NULL);
		if (sCaminhoAutoridade && strlen (sCaminhoAutoridade) == 0)
		{
		    BE_free (sCaminhoAutoridade);
		}
		BE_free (sTemp);
		sTemp = sCaminhoAutoridade;
		sCaminhoAutoridade = be_arquivo_absoluto (sTemp);
		//recuperar cifras SSL
		sCifras = g_key_file_get_string (Ini, "seguranca", "cifras", NULL);
		if (sCifras && strlen (sCifras) == 0)
		{
		    BE_free (sCifras);
		}
		//liberar INI
		g_key_file_free (Ini);
	}

	//iniciar conexão, login
	if (sUsuario && strlen (sUsuario) > 0)
		SQL = be_sql_abrir_conexao_segura (sUsuario, sSenha, sIP, nPorta, -1, &sErros, sChavePrivada, sCertificado, sAutoridade, sCaminhoAutoridade, sCifras);

	//informar erros
	if (sErros)
	{
		BE_sincronizar_log_informar ("\t\tErro1: %s\n", sErros);
		BE_free (sErros);
		//fechar conexão se houver algum erro, e a conexão ainda estiver aberta.
		if (SQL)
		{
			BE_sql_fechar_conexao (SQL);
		}
	}

	//limpar tudo
	BE_free (sTemp);
	BE_free (sIP);
	BE_free (sUsuario);
	BE_free (sSenha);
	BE_free (sChavePrivada);
	BE_free (sCertificado);
	BE_free (sAutoridade);
	BE_free (sCaminhoAutoridade);
	BE_free (sCifras);

	return SQL;
}

gchar *be_sincronizar_iso_key_file_get_string (GKeyFile *key_file, const gchar *group_name, const gchar *key)
{
	gchar *sTemp = NULL, *sTemp1 = NULL;

	sTemp = g_key_file_get_string (key_file, group_name, key, NULL);
	if (sTemp)
	{
		sTemp1 = BE_UTF8_ISO (sTemp);
		BE_free (sTemp);
		return sTemp1;
	}

	BE_free (sTemp);
	return NULL;
}

gboolean be_sincronizar_executar_odbc (BESQL *SQL, const gchar *sArquivo, gboolean bProgresso, gboolean bLog)
{
/*******************************************************************************
Realizar ações de um arquivo INI
Este arquivo INI conterá ações para ODBC.

Argumentos:
    SQL : conexão com o banco de dados atual
    sArquivo : arquivo com as ações, em formato INI
    bProgresso : informar o progresso das ações, sacrificando o processamento
    bLog : informar se o arquivo de Log será gerado
    Retorno: se foi realizada todas as ações
*******************************************************************************/
	BEODBC *ODBC = NULL;
	_BESincronizarODBC *BESincronizarODBC = NULL;
	GKeyFile *Ini = NULL;
	GSList *ListaL = NULL;//lista
	gchar *sTemp = NULL, *sTemp1 = NULL, *sAcao = NULL, *sConexao = NULL, *sAlternativa = NULL;
	gchar *sSQLTravarTabela = NULL, *sSQLDestravarTabela = NULL;
	gchar *sODBCConexao = NULL, *sODBCCatalogo = NULL, *sODBCEsquema = NULL, *sODBCTabela = NULL, *sODBCTipo = NULL, *sODBCConsulta = NULL;
	gchar *sErros = NULL;
    gint nAcao = 1, nAlternativa = 1;
    gboolean bErro = FALSE;

	//carregar configuração de um INI
	if ((Ini = be_sincronizar_iniciar_ini (sArquivo)))
	{
		//ação inicial
		BE_free (sAcao);
		sAcao = g_strdup_printf ("%d", nAcao);//ação
		//loop até todas sincronizações ou algum erro
		while ((sConexao = g_key_file_get_string (Ini, sAcao, "ODBCConexao", NULL)) && !sErros)
		{
			//recuperar informações de consultas ODBC e Local
			sODBCConexao = g_key_file_get_string (Ini, sAcao, "ODBCConexao", NULL);
			sODBCConsulta = be_sincronizar_iso_key_file_get_string (Ini, sAcao, "ODBCConsulta");
			sSQLTravarTabela = be_sincronizar_iso_key_file_get_string (Ini, sAcao, "SQLTravarTabela");
			sSQLDestravarTabela = be_sincronizar_iso_key_file_get_string (Ini, sAcao, "SQLDestravarTabela");
			//carregar nova estrutura, com as consultas SQL
			BESincronizarODBC = g_new (_BESincronizarODBC, 1);
			BESincronizarODBC->SQL = SQL;
			BESincronizarODBC->SQLAlternativas = NULL;
			BESincronizarODBC->SQLAlternativas = g_slist_append (BESincronizarODBC->SQLAlternativas, be_sincronizar_iso_key_file_get_string (Ini, sAcao, "SQLConsulta"));//adicionar na lista
			BESincronizarODBC->nLinhasODBC = 0;
			BESincronizarODBC->nLinhasSQL = 0;
			BESincronizarODBC->bProgresso = bProgresso;
			BESincronizarODBC->bLog = bLog;
			BESincronizarODBC->bErro = FALSE;
			//preparar contagem de ações SQL alternativas
			nAlternativa = 1;
			BE_free (sAlternativa);
			sAlternativa = g_strdup_printf ("SQLAlternativa%d", nAlternativa);
			//loop para carregar as ações alternativas
			BE_free (sTemp);
			while ((sTemp = be_sincronizar_iso_key_file_get_string (Ini, sAcao, sAlternativa)))
			{
				BESincronizarODBC->SQLAlternativas = g_slist_append (BESincronizarODBC->SQLAlternativas, sTemp);//adicionar na lista, alternativas
				nAlternativa++;
				BE_free (sAlternativa);
				sAlternativa = g_strdup_printf ("SQLAlternativa%d", nAlternativa);
				//simplesmente anular. Não pode limpar da memória, pois o conteúdo está sendo apontado pela lista SQLAlternativas
				sTemp = NULL;
			}
			//preparar conexão ODBC
			if (sODBCConexao)
			{
				//recuperar informações de conexão ODBC
				BE_free (sTemp);
				sTemp = sODBCConexao;
				sODBCConexao = g_key_file_get_string (Ini, sTemp, "conexao", NULL);
				sODBCCatalogo = g_key_file_get_string (Ini, sTemp, "catalogo", NULL);
				sODBCEsquema = g_key_file_get_string (Ini, sTemp, "esquema", NULL);
				sODBCTabela = g_key_file_get_string (Ini, sTemp, "tabela", NULL);
				sODBCTipo = g_key_file_get_string (Ini, sTemp, "tipo", NULL);
				//iniciar conexão ODBC
				ODBC = be_odbc_abrir_conexao (sODBCConexao, sODBCCatalogo, sODBCEsquema, sODBCTabela, sODBCTipo, &sErros);
				if (ODBC && BESincronizarODBC)
				{
					//informar LOG
					if (BESincronizarODBC->bLog)
						BE_sincronizar_log_informar ("\tConsulta ODBC: %s\n", sODBCConsulta);
					//travar tabelas SQL local, para ficar mais rápido
					if (sSQLTravarTabela)
						be_sql_executar (BESincronizarODBC->SQL, sSQLTravarTabela, NULL, NULL, &sErros);
					//conexão direta ODBC, loop em _be_sincronizar_callback_ODBC
					be_odbc_executar (ODBC, sODBCConsulta, _be_sincronizar_callback_ODBC, BESincronizarODBC, &sErros);
					//destravar tabelas SQL local
					if (sSQLDestravarTabela)
						be_sql_executar (BESincronizarODBC->SQL, sSQLDestravarTabela, NULL, NULL, &sErros);
					//informar LOG
					if (BESincronizarODBC->bLog)
						BE_sincronizar_log_informar ("\tLinhas ODBC: %d\t\tLinhas SQL: %d\n", BESincronizarODBC->nLinhasODBC, BESincronizarODBC->nLinhasSQL);
				}
				//fechar conexão ODBC
				BE_odbc_fechar_conexao (ODBC);
			}
			//veriricar erros
			if (BESincronizarODBC->bErro)
				bErro = TRUE;
			//limpar
			BE_free (sSQLTravarTabela);
			BE_free (sSQLDestravarTabela);
			BE_free (sODBCConsulta);
			BE_free (sODBCConexao);
			BE_free (sODBCCatalogo);
			BE_free (sODBCEsquema);
			BE_free (sODBCTabela);
			if (BESincronizarODBC)
			{
				for (ListaL = BESincronizarODBC->SQLAlternativas; ListaL; ListaL = g_slist_next (ListaL))
				{
					sTemp = (gchar *) ListaL->data;
					BE_free (sTemp);
				}
				BE_slist_free (BESincronizarODBC->SQLAlternativas);
				BE_free (BESincronizarODBC);
			}
            //próximas ações
            nAcao++;
            BE_free (sAcao);
            sAcao = g_strdup_printf ("%d", nAcao);
            //limpar
            BE_free (sConexao);
		}
		//liberar
		g_key_file_free (Ini);
	}

	//informar erros
	if (sErros)
	{
        BE_sincronizar_log_informar ("\t\tErro2: %s\n", sErros);
        BE_free (sErros);
        bErro = TRUE;
	}

	//limpar
	BE_free (sConexao);
    BE_free (sAcao);
    BE_free (sAlternativa);
    BE_free (sTemp);
    BE_free (sTemp1);

	return !bErro;
}

GKeyFile *be_sincronizar_iniciar_ini (const gchar *sArquivo)
{
	GKeyFile *Ini = NULL;
	FILE *File = NULL;
	GString *Buffer = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	const gint nBufferSIZE = BEGLOBAL_BLOCOS_ARQUIVO;
	guchar sBuffer[nBufferSIZE];
	gsize nSize = nBufferSIZE;

	Ini = g_key_file_new ();

	//abrindo arquivo
	if ((File = fopen (sArquivo, "rb")))
	{
		//preparando buffer
		Buffer = g_string_new ("");
		//loop nos bytes
		while (nSize == nBufferSIZE)
		{
			nSize = fread (&sBuffer, sizeof (guchar), nBufferSIZE, File);
			if (nSize > 0)
				g_string_append_len (Buffer, (gchar *) sBuffer, nSize);
		}
		fclose (File);
		//comprimento final
		nSize = Buffer->len;
		if (nSize > 0)
		{
			//conversão para UTF8
			sTemp = BE_string_free (Buffer);
			sTemp1 = BE_ISO_UTF8 (sTemp);
			BE_free (sTemp);
			nSize = strlen (sTemp1);//novo tamanho
			if (g_key_file_load_from_data (Ini, sTemp1, nSize, G_KEY_FILE_NONE, NULL))
				return Ini;
		}
		else
		{
			g_string_free (Buffer, TRUE);
			Buffer = NULL;
		}
	}
	//limpar
	g_key_file_free (Ini);
	return NULL;

	/*if (g_key_file_load_from_file (Ini, sArquivo, G_KEY_FILE_NONE, NULL))
		return Ini;
	else
		return NULL;*/
}

gboolean be_sincronizar_executar_net (BESQL *SQL, const gchar *sArquivo, gboolean bProgresso, gboolean bLog)
{
/*******************************************************************************
Realizar ações de um arquivo INI
Este arquivo INI conterá ações para NET.
O processamento mais pessado está dentro de _be_sincronizar_executar_net

Argumentos:
    SQL : conexão com o banco de dados atual
    sArquivo : arquivo com as ações, em formato INI
    bProgresso : informar o progresso das ações, sacrificando o processamento
    bLog : informar se o arquivo de Log será gerado
    Retorno: se foi realizada todas as ações
*******************************************************************************/
	GKeyFile *Ini = NULL;
	_BESincronizarNET *BESincronizarNET = NULL;
	BENET *Site = NULL;
	gchar *sAcao = NULL, *sConexao = NULL;
	gchar *sNetProxy = NULL, *sNetAcesso = NULL, *sNetNavegador = NULL, *sNetInicio = NULL;
	gchar *sSQLValores = NULL;
	gchar *sErros = NULL;
	gchar *sTemp = NULL;
    gint nAcao = 1;
    gint nAfetadas = 0;
    gboolean bErro = FALSE, bUTF8 = FALSE;

	//carregar configuração de um INI
	if ((Ini = be_sincronizar_iniciar_ini (sArquivo)))
	{
		//ação inicial
		BE_free (sAcao);
		sAcao = g_strdup_printf ("%d", nAcao);//ação
		//loop até todas sincronizações ou algum erro
		while ((sConexao = g_key_file_get_string (Ini, sAcao, "NetConexao", NULL)) && !sErros)
		{
			//recuperar informações de conexão
			BE_free (sNetProxy);
			BE_free (sNetAcesso);
			BE_free (sNetNavegador);
			BE_free (sNetInicio);
			sNetProxy = g_key_file_get_string (Ini, sConexao, "proxy", NULL);
			sNetAcesso = g_key_file_get_string (Ini, sConexao, "acesso", NULL);
			sNetNavegador = g_key_file_get_string (Ini, sConexao, "navegador", NULL);
			sNetInicio = g_key_file_get_string (Ini, sConexao, "inicio", NULL);
			bUTF8 = g_key_file_get_boolean (Ini, sConexao, "utf8", NULL);
			//iniciar conexão com site inicial
			Site = be_net_iniciar (sNetInicio, sNetProxy, sNetAcesso, sNetNavegador);
			//obter site inicial
			if (Site && be_net_receber (Site))
			{
				//carregar nova estrutura, com as consultas SQL
				BESincronizarNET = g_new (_BESincronizarNET, 1);
				BESincronizarNET->SQL = SQL;
				BESincronizarNET->Ini = Ini;
				BESincronizarNET->sAcao = sAcao;
				BESincronizarNET->bUTF8 = bUTF8;
				BESincronizarNET->nLinhasSQL = 0;
				BESincronizarNET->bProgresso = bProgresso;
				BESincronizarNET->bLog = bLog;
				BESincronizarNET->Site = Site;
				BESincronizarNET->sNetLoop = NULL;
				BESincronizarNET->bSiteInicial = FALSE;
				BESincronizarNET->sNetProxy = sNetProxy;
				BESincronizarNET->sNetAcesso = sNetAcesso;
				BESincronizarNET->sNetNavegador = sNetNavegador;
				BESincronizarNET->sNetInicio = sNetInicio;

				//recuperar valores de um loop SQL
				BE_free (sSQLValores);
				sSQLValores = be_sincronizar_iso_key_file_get_string (Ini, sAcao, "NetSQLValores");
				if (sSQLValores && strlen (sSQLValores) > 0)
				{
					//informar Log
					if (bLog)
						BE_sincronizar_log_informar ("\tLoop SQL: %s\n", sSQLValores);
					//recuperar apartir de qual sequencia será o loop
					BESincronizarNET->sNetLoop = be_sincronizar_iso_key_file_get_string (Ini, sAcao, "NetSQLValoresInicial");
					//executar loop
					nAfetadas = be_sql_executar (SQL, sSQLValores, _be_sincronizar_executar_net_callback, BESincronizarNET, &sErros);
					//informar Log
					if (bLog && sErros)
						BE_sincronizar_log_informar ("\t\tErro3: %s\n", sErros);
					BE_free (sErros);
				}
				//recuperar valores que não estão dentro de um loop
				else
					sErros = _be_sincronizar_executar_net (BESincronizarNET, NULL);
				//finalizar conexão com site. Não limpar diretamente 'Site', pois BESincronizarNET->Site pode apontar para outro endereço num Reset.
				BE_net_finalizar (BESincronizarNET->Site);
				//limpar estrutura. os outros ítens são constantes
				BE_free (BESincronizarNET);
			}
            //próximas ações
            nAcao++;
            BE_free (sAcao);
            sAcao = g_strdup_printf ("%d", nAcao);
            //limpar
            BE_free (sConexao);
		}
		//liberar
		g_key_file_free (Ini);
	}

	//informar erros
	if (sErros)
	{
		BE_sincronizar_log_informar ("\t\tErro4: %s\n", sErros);
        BE_free (sErros);
        bErro = TRUE;
	}

	//limpar
	BE_free (sTemp);
	BE_free (sConexao);
	BE_free (sSQLValores);
    BE_free (sAcao);
	BE_free (sNetProxy);
	BE_free (sNetAcesso);
	BE_free (sNetNavegador);
	BE_free (sNetInicio);

	return !bErro;
}

gboolean _be_sincronizar_executar_net_callback (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data)
{
	gchar *sErros = NULL;
	_BESincronizarNET *BESincronizarNET = NULL;

    //recuperar estrutura
    BESincronizarNET = (_BESincronizarNET *) user_data;

    //processamento real, para cada linha de dados
    sErros = _be_sincronizar_executar_net (BESincronizarNET, Campos);

    //informar progresso, somente na tela
    if (BESincronizarNET->bProgresso)
        printf ("Linha: %d\n", nLinha);

	//informar erros
	if (sErros)
	{
		BE_sincronizar_log_informar ("\t\tErro5: %s\n", sErros);
        BE_free (sErros);
        return FALSE;//FALSE para parar o loop.
	}
	else
		return TRUE;//FALSE para parar o loop.

}

gchar *_be_sincronizar_executar_net (_BESincronizarNET *BESincronizarNET, GSList *Campos)
{
/*******************************************************************************
Função reservada
Realiza processamento das ações de um arquivo INI.
Esta função é chamada pelo _be_sincronizar_executar_net_callback,
ou diretamente pelo be_sincronizar_executar_net
Argumentos:
    BESincronizarNET : estrutura de dados NET
    Campos : informações de um loop SQL
    Retorno: erros

Exemplos:
	Net2_SQLConsulta =
	Net2_SQLAlternativa =
    ou
	Net2_SQLConsulta1 =
	Net2_SQLConsulta2 =
*******************************************************************************/
	BENET_formulario *Formulario = NULL;//ponteiro de estrutura
	BENET_campo *CampoNet = NULL;//ponteiro de estrutura
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL, *ListaL2 = NULL;//lista
	GSList *SQLConsultas = NULL, *SQLAlternativas = NULL, *Variaveis = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL, *sTemp3 = NULL;
	gchar *sConsulta = NULL, *sAlternativa = NULL, *sVariavel = NULL;
	gchar *sSQLConsulta = NULL, *sSQLTravarTabela = NULL, *sSQLDestravarTabela = NULL;
	gchar *sNet = NULL, *sNetCampo = NULL, *sNetArquivo = NULL, *sNetArquivoT = NULL, *sPular = NULL;
	gchar *sURL = NULL;
	gchar *sErros = NULL;
    gint nConsulta = 1, nAlternativa = 1, nVariavel = 2, nNet = 0, nPular = 0;
    gint nAfetadas = 0;
    gboolean bValidar = TRUE, bPular = FALSE;
	//preparar contagem de ações Net
	if (BESincronizarNET->bSiteInicial && BESincronizarNET->sNetLoop)
		nNet = atoi (BESincronizarNET->sNetLoop);
	else
		nNet = 0;
	if (nNet < 0)
		nNet = 0;
	BE_free (sNet);
	sNet = g_strdup_printf ("Net%d_URL", nNet);//Net0_URL
	//loop para executar os sites URL
	BE_free (sURL);
	while ((sURL = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sNet, NULL)))
	{
		//informar que primeira tela
		BESincronizarNET->bSiteInicial = TRUE;

		printf ("%s = %s\n", sNet, sURL);//informar na tela
		//verificar reinício na conexão: Reset
		BE_free (sTemp);
		sTemp = g_strdup_printf ("Net%d_Resetar", nNet);//Net0_Resetar
		BE_free (sTemp1);
		sTemp1 = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp, NULL);
		if (sTemp1)
		{
			//finalizar conexão com site
			BE_net_finalizar (BESincronizarNET->Site);
			//iniciar conexão com site inicial
			BESincronizarNET->Site = be_net_iniciar (BESincronizarNET->sNetInicio, BESincronizarNET->sNetProxy, BESincronizarNET->sNetAcesso, BESincronizarNET->sNetNavegador);
			//obter site inicial, novamente
			be_net_receber (BESincronizarNET->Site);
		}
		//recuperar arquivo para download
		BE_free (sTemp);
		sTemp = g_strdup_printf ("Net%d_Salvar", nNet);//Net0_Salvar
		BE_free (sNetArquivo);
		sNetArquivo = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp, NULL);
		//recuperar arquivo para download, com tempo
		BE_free (sTemp);
		sTemp = g_strdup_printf ("Net%d_Salvar_T", nNet);//Net0_Salvar_T
		BE_free (sNetArquivoT);
		sNetArquivoT = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp, NULL);
		//verificar necessidade de pular esta contagem no INI
		if (BESincronizarNET->Site && BESincronizarNET->Site->sConteudo)
		{
			//preparar a contagem
			bPular = TRUE;
			nPular = 0;
			BE_free (sTemp1);
			BE_free (sPular);
			sPular = g_strdup_printf ("Net%d_Pular%d", nNet, nPular);//Net0_Pular0
			//verificar se o valor procurado existe
			while (bPular && (sTemp1 = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sPular, NULL)))
			{
				//garantir alteração proprosital
				bPular = FALSE;
				//verificar valores provenientes de campos de um loop SQL
				for (ListaL2 = Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
				{
					Campo = (BESQL_Campo *) ListaL2->data;
					//recuperar nome para comparação
					BE_free (sTemp2);
					BE_free (sTemp3);
					sTemp3 = BE_ISO_UTF8 (Campo->sNome);
					sTemp2 = g_strdup_printf ("$`%s`$", sTemp3);
					//localizar campo e alterar pelo seu valor
					if (strstr (sTemp1, sTemp2))
					{
						//substituir valor encontrado
						BE_free (sTemp1);
						if (Campo->sValor)
							sTemp1 = BE_ISO_UTF8 (Campo->sValor);
						else
							sTemp1 = g_strdup ("");
					}
				}
				//verificar
				printf ("%s = %s\n", sPular, sTemp1);//informar na tela
				BE_free (sTemp);
				sTemp = BE_UTF8_ISO (sTemp1);//conversão para ISO
				//verificar, localizar
				if (strstr (BESincronizarNET->Site->sConteudo, sTemp))
					bPular = TRUE;
				else
					bPular = FALSE;
				//avançar contagem
				nPular++;
				BE_free (sTemp1);
				BE_free (sPular);
				sPular = g_strdup_printf ("Net%d_Pular%d", nNet, nPular);//Net0_Pular0
			}
			//analizar resultados
			if (!nPular)//igual à 0
				bPular = FALSE;
		}				
		//processamento dos campos do site anterior, método POST
		if (!bPular && BESincronizarNET->Site)
		{
			//loop nos formularios
			for (ListaL = BESincronizarNET->Site->Formularios; ListaL; ListaL = g_slist_next (ListaL))
			{
				Formulario = (BENET_formulario *) ListaL->data;
				//loop campos
				for (ListaL1 = Formulario->Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
				{
					CampoNet = (BENET_campo *) ListaL1->data;
					//verificar pelo nome do campo
					if (CampoNet->sNome)//primeira tentativa
					{
						BE_free (sNetCampo);
						sNetCampo = g_strdup_printf ("Net%d_$%s", nNet, CampoNet->sNome);//Net0_$
						//recuperar o novo valor
						BE_free (sTemp);
						sTemp = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sNetCampo, NULL);
					}
					//segunda tentativa; verificar pelo valor do campo e não pelo nome
					if (!sTemp && CampoNet->sValor)
					{
						BE_free (sNetCampo);
						sNetCampo = g_strdup_printf ("Net%d_$%s", nNet, CampoNet->sValor);//Net0_$
						//recuperar o novo valor
						BE_free (sTemp);
						sTemp = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sNetCampo, NULL);
					}
					if (sTemp)
					{
						//verificar se o valor é para ser extraído do site
						if (strstr (sTemp, "$$"))
						{
							//obter o primeiro parâmetro da comparação
							BE_free (sTemp1);
							sTemp1 = be_substr (sTemp, 0, be_strpos (sTemp, "$$", 0));
							//obter o segundo parâmetro da comparação
							BE_free (sTemp2);
							sTemp2 = be_substr (sTemp, be_strpos (sTemp, "$$", 0) + 2, 0);
							//obter o valor da variável
							if (sTemp1 && sTemp2)
							{
								BE_free (sVariavel);
								sVariavel = be_strextract (BESincronizarNET->Site->sConteudo, sTemp1, sTemp2, 0);
								//substituir variavel por seu valor em cada consulta
								if (sVariavel)
								{
									BE_free (sTemp);
									sTemp = g_strdup (sVariavel);
								}
							}
						}
						printf ("%s = %s\n", sNetCampo, sTemp);//informar na tela
						//verificar valores provenientes de campos de um loop SQL
						for (ListaL2 = Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
						{
							Campo = (BESQL_Campo *) ListaL2->data;
							//recuperar nome para comparação
							BE_free (sTemp1);
							BE_free (sTemp2);
							sTemp2 = BE_ISO_UTF8 (Campo->sNome);
							sTemp1 = g_strdup_printf ("$`%s`$", sTemp2);
							//localizar campo e alterar pelo seu valor
							if (strstr (sTemp, sTemp1))
							{
								//substituir valor encontrado
								BE_free (sTemp);
								if (Campo->sValor)
									sTemp = BE_ISO_UTF8 (Campo->sValor);
								else
									sTemp = g_strdup ("");
							}
						}
						//substituir valor antigo pelo novo, em UTF8
						if (BESincronizarNET->bUTF8)
							CampoNet->sValor = g_strdup (sTemp);
						//substituir valor antigo pelo novo, em ISO
						else
							CampoNet->sValor = BE_UTF8_ISO (sTemp);
					}

					//recuperar novas ativações
					BE_free (sNetCampo);
					if (CampoNet->sNome)//primeir tentativa
						sNetCampo = g_strdup_printf ("Net%d_Ativo$%s", nNet, CampoNet->sNome);//Net0_Ativo$
					else if (CampoNet->sValor)//segunda tentativa
						sNetCampo = g_strdup_printf ("Net%d_Ativo$%s", nNet, CampoNet->sValor);//Net0_Ativo$
					//recuperar o novo valor
					BE_free (sTemp);
					sTemp = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sNetCampo, NULL);
					if (sTemp)
					{
						printf ("%s = %s\n", sNetCampo, sTemp);//informar na tela
						//substituir valor antigo pelo novo, para botões submit
						be_net_botao (BESincronizarNET->Site, CampoNet->sNome, TRUE);
						printf ("\tSite novo: %s\n", BESincronizarNET->Site->sURL);
					}
				}
			}
		}
		//recuperar o conteúdo, e tentar enviar o método POST
		if (!bPular && strlen (sURL) > 1)
		{
			BE_free (BESincronizarNET->Site->sURL);
			BESincronizarNET->Site->sURL = sURL;//alterar site
		}
		//obter site
		if (!bPular && be_net_receber (BESincronizarNET->Site))
		{
			//informar Log
			if (BESincronizarNET->bLog)
				BE_sincronizar_log_informar ("\tSite: %s\n", BESincronizarNET->Site->sURL);
			//verificar conversão UTF8
			BE_free (sTemp);
			if (BESincronizarNET->bUTF8)
				sTemp = BE_UTF8_ISO (BESincronizarNET->Site->sConteudo);//sites está em UTF8, converter para ISO
			else
				sTemp = g_strdup (BESincronizarNET->Site->sConteudo);//site já está em ISO
			//informar erro de codificação
			if (!sTemp)
				printf ("Erro de codificaao UTF8. Altere UTF8 = 0\n");
			//salvar conteúdo
			if (sNetArquivo)
			{
				//converter nome
				BE_free (sTemp1);
				sTemp1 = sNetArquivo;
				sNetArquivo = be_arquivo_nome_sugerido (sTemp1, BEARQUIVO_COMPRIMENTO_MAX, FALSE);
				//gravar
				if (be_sincronizar_salvar_arquivo (sNetArquivo, sTemp, strlen (BESincronizarNET->Site->sConteudo)))
				{
					if (BESincronizarNET->bLog)
						BE_sincronizar_log_informar ("\tGravado em arquivo: %s\n", sNetArquivo);
				}
			}
			//salvar conteúdo num arquivo sem igual, data e horas atuais
			if (sNetArquivoT)
			{
				//converter nome
				BE_free (sTemp1);
				sTemp1 = sNetArquivoT;
				sNetArquivoT = be_arquivo_nome_sugerido (sTemp1, BEARQUIVO_COMPRIMENTO_MAX, TRUE);
				//gravar
				if (be_sincronizar_salvar_arquivo (sNetArquivoT, sTemp, strlen (BESincronizarNET->Site->sConteudo)))
				{
					if (BESincronizarNET->bLog)
						BE_sincronizar_log_informar ("\tGravado em arquivo unico: %s\n", sNetArquivoT);
				}
			}
			//verificar validade da página
			BE_free (sTemp);
			BE_free (sTemp1);
			sTemp = g_strdup_printf ("Net%d_ValidarP", nNet);//Net0_Validar_P
			sTemp1 = g_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp, NULL);
			//verificar se o valor procurado existe
			if (sTemp1)
			{
				//verificar valores provenientes de campos de um loop SQL
				for (ListaL2 = Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
				{
					Campo = (BESQL_Campo *) ListaL2->data;
					//recuperar nome para comparação
					BE_free (sTemp2);
					BE_free (sTemp3);
					sTemp3 = BE_ISO_UTF8 (Campo->sNome);
					sTemp2 = g_strdup_printf ("$`%s`$", sTemp3);
					//localizar campo e alterar pelo seu valor
					if (strstr (sTemp1, sTemp2))
					{
						//substituir valor encontrado
						BE_free (sTemp1);
						if (Campo->sValor)
							sTemp1 = g_strdup (Campo->sValor);
						else
							sTemp1 = g_strdup ("");
					}
				}

				printf ("%s = %s\n", sTemp, sTemp1);//informar na tela
				BE_free (sTemp);
				sTemp = BE_UTF8_ISO (sTemp1);//conversão para ISO
				//verificar
				if (strstr (BESincronizarNET->Site->sConteudo, sTemp))
					bValidar = TRUE;
				else
					bValidar = FALSE;
			}
			else
				bValidar = TRUE;
			//somente páginas válidas
			if (bValidar)
			{
				//recuperar informações de consultas local
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Net%d_SQLTravarTabela", nNet);//Net0_SQLTravarTabela
				sSQLTravarTabela = be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp);
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Net%d_SQLDestravarTabela", nNet);//Net0_SQLDestravarTabela
				sSQLDestravarTabela = be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp);

				//preparar lista de consultas
				nConsulta = 1
				BE_free (sConsulta);
				sConsulta = g_strdup_printf ("Net%d_SQLConsulta%d", nNet, nConsulta);//Net0_SQLConsulta0
				//loop para carregar as consultas
				BE_free (sTemp);
				while ((sTemp = be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sConsulta)))
				{
					printf ("%s = %s\n", sConsulta, sTemp);//informar na tela
					SQLConsultas = g_slist_append (SQLConsultas, sTemp);//adicionar na lista, consultas
					nConsulta++;
					BE_free (sConsulta);
					sConsulta = g_strdup_printf ("Net%d_SQLConsulta%d", nNet, nConsulta);//Net0_SQLConsulta0
					//simplesmente anular. Não pode limpar da memória, pois o conteúdo está sendo apontado pela lista SQLConsultas
					sTemp = NULL;
				}

				//preparar lista de consultas
				BE_free (sAlternativa);
				BE_free (sTemp);
				sAlternativa = g_strdup_printf ("Net%d_SQLConsultas", nNet);//Net0_SQLConsultas
				if (sAlternativa)
				{
					sTemp = be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sAlternativa);
					if (sTemp)
						SQLAlternativas = g_slist_append (SQLAlternativas, sTemp);//adicionar na lista
				}
				//preparar contagem de ações SQL alternativas
				nAlternativa = 1;
				BE_free (sAlternativa);
				sAlternativa = g_strdup_printf ("Net%d_SQLAlternativa%d", nNet, nAlternativa);//Net0_SQLAlternativa0
				//loop para carregar as ações alternativas
				BE_free (sTemp);
				while ((sTemp = be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sAlternativa)))
				{
					printf ("%s = %s\n", sAlternativa, sTemp);//informar na tela
					SQLAlternativas = g_slist_append (SQLAlternativas, sTemp);//adicionar na lista, alternativas
					nAlternativa++;
					BE_free (sAlternativa);
					sAlternativa = g_strdup_printf ("Net%d_SQLAlternativa%d", nNet, nAlternativa);//Net0_SQLAlternativa0
					//simplesmente anular. Não pode limpar da memória, pois o conteúdo está sendo apontado pela lista SQLAlternativas
					sTemp = NULL;
				}

				//preparar lista de variáveis
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Net%d_Extrair$1", nNet);//Net0_Extrair$1
				if (sTemp)
					Variaveis = g_slist_append (Variaveis, be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sTemp));//adicionar na lista
				//preparar contagem de mais variáveis
				nVariavel = 2;
				BE_free (sVariavel);
				sVariavel = g_strdup_printf ("Net%d_Extrair$%d", nNet, nVariavel);//Net0_Extrair$1
				//loop para carregar mais variáveis
				BE_free (sTemp);
				while ((sTemp = be_sincronizar_iso_key_file_get_string (BESincronizarNET->Ini, BESincronizarNET->sAcao, sVariavel)))
				{
					printf ("%s = %s\n", sVariavel, sTemp);//informar na tela
					Variaveis = g_slist_append (Variaveis, sTemp);//adicionar na lista, alternativas
					nVariavel++;//avançar próxima variável
					BE_free (sVariavel);
					sVariavel = g_strdup_printf ("Net%d_Extrair$%d", nNet, nVariavel);//Net0_Extrair$1
					//simplesmente anular. Não pode limpar da memória, pois o conteúdo está sendo apontado pela lista Variaveis
					sTemp = NULL;
				}

				//travar tabelas SQL local, para ficar mais rápido
				if (sSQLTravarTabela)
				{
					be_sql_executar (BESincronizarNET->SQL, sSQLTravarTabela, NULL, NULL, &sErros);
					//informar Log
					if (BESincronizarNET->bLog)
						BE_sincronizar_log_informar ("\tConsulta SQL: %s\n", sSQLTravarTabela);
					BE_free (sErros);
				}
				//verificar alterações; tentar executar todas consultas
				if (SQLConsultas)
				{
					//executar listas de consultas
					for (ListaL = SQLConsultas, nAfetadas = 0; ListaL; ListaL = g_slist_next (ListaL))
					{
						//sTemp = (gchar *) ListaL->data;
						//convertendo o valor
						if (BESincronizarNET->bUTF8)
						{
							BE_free (sTemp1);
							sTemp1 = BE_UTF8_ISO (BESincronizarNET->Site->sConteudo);
							//informar erro de codificação
							if (!sTemp1)
								printf ("Erro de codificaao UTF8. Altere UTF8 = 0\n");
							//escapando o valor
							BE_free (sTemp);
							sTemp = be_sql_escapar (BESincronizarNET->SQL, sTemp1, strlen (sTemp1));
						}
						else
						{
							//escapando o valor
							BE_free (sTemp);
							sTemp = be_sql_escapar (BESincronizarNET->SQL, BESincronizarNET->Site->sConteudo, strlen (BESincronizarNET->Site->sConteudo));
						}
						//substituir conteúdo
						BE_free (sTemp1);
						sTemp1 = g_strdup_printf ("'%s'", sTemp);//valor
						sSQLConsulta = be_strreplace ((gchar *) ListaL->data, "$Conteudo$", sTemp1);
						//loop na lista de variáveis, substituir
						for (ListaL1 = Variaveis, nVariavel = 1; ListaL1; ListaL1 = g_slist_next (ListaL1), nVariavel++)
						{
							BE_free (sTemp);
							BE_free (sTemp1);
							if (ListaL1->data)
							{
								//obter o primeiro parâmetro da comparação
								sTemp = be_substr ((gchar *) ListaL1->data, 0, be_strpos ((gchar *) ListaL1->data, "$$", 0));
								//obter o segundo parâmetro da comparação
								sTemp1 = be_substr ((gchar *) ListaL1->data, be_strpos ((gchar *) ListaL1->data, "$$", 0) + 2, 0);
							}
							//obter o valor da variável
							if (sTemp && sTemp1)
							{
								BE_free (sVariavel);
								sVariavel = be_strextract (BESincronizarNET->Site->sConteudo, sTemp, sTemp1, 0);
								//substituir variavel por seu valor em cada consulta
								if (sVariavel)
								{
									//convertendo o valor
									if (BESincronizarNET->bUTF8)
									{
										BE_free (sTemp);
										sTemp = sVariavel;
										sVariavel = BE_UTF8_ISO (sTemp);
									}
									//escapando o valor
									BE_free (sTemp);
									sTemp = sVariavel;
									sVariavel = be_sql_escapar (BESincronizarNET->SQL, sTemp, strlen (sTemp));
									BE_free (sTemp);
									sTemp = sVariavel;
									sVariavel = g_strdup_printf ("'%s'", sTemp);//valor
									//substituir conteúdo
									BE_free (sTemp);
									BE_free (sTemp1);
									sTemp = sSQLConsulta;
									sTemp1 = g_strdup_printf ("$%d$", nVariavel);
									sSQLConsulta = be_strreplace (sTemp, sTemp1, sVariavel);
								}
							}
						}
						//loop nos campos, verificar valores provenientes de campos de um loop SQL
						for (ListaL2 = Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
						{
							Campo = (BESQL_Campo *) ListaL2->data;
							//preparando valores
							BE_free (sVariavel);
							if (Campo->sValor)
							{
								//escapando o valor
								sVariavel = be_sql_escapar (BESincronizarNET->SQL, Campo->sValor, strlen (Campo->sValor));
								BE_free (sTemp);
								sTemp = sVariavel;
								sVariavel = g_strdup_printf ("'%s'", sTemp);//valor
							}
							else
								sVariavel = g_strdup ("NULL");
							//recuperar nome para comparação
							BE_free (sTemp);
							sTemp = g_strdup_printf ("$`%s`$", Campo->sNome);							
							//substituir campos por valores
							BE_free (sTemp1);
							sTemp1 = sSQLConsulta;
							sSQLConsulta = be_strreplace (sTemp1, sTemp, sVariavel);
							//printf ("%s = %s\n", Campo->sNome, Campo->sValor);
						}
						//informar Log
						if (BESincronizarNET->bLog)
							BE_sincronizar_log_informar ("\tConsulta SQL: %s\n", sSQLConsulta);
						//realizar as alterações no BD
						nAfetadas = be_sql_executar (BESincronizarNET->SQL, sSQLConsulta, NULL, NULL, &sErros);
						//informar Log
						if (BESincronizarNET->bLog && sErros)
							BE_sincronizar_log_informar ("\t\tErro6: %s\n", sErros);
						BE_free (sErros);
					}
				}
				//verificar alterações alternativas; se a primeira falhar, tentar a segunda, e assim sucessivamente
				if (SQLAlternativas)
				{
					//executar listas de consultas
					for (ListaL = SQLAlternativas, nAfetadas = 0; ListaL && (nAfetadas == 0); ListaL = g_slist_next (ListaL))
					{
						//sTemp = (gchar *) ListaL->data;
						//convertendo o valor
						if (BESincronizarNET->bUTF8)
						{
							BE_free (sTemp1);
							sTemp1 = BE_UTF8_ISO (BESincronizarNET->Site->sConteudo);
							//informar erro de codificação
							if (!sTemp1)
								printf ("Erro de codificaao UTF8. Altere UTF8 = 0\n");
							//escapando o valor
							BE_free (sTemp);
							sTemp = be_sql_escapar (BESincronizarNET->SQL, sTemp1, strlen (sTemp1));
						}
						else
						{
							//escapando o valor
							BE_free (sTemp);
							sTemp = be_sql_escapar (BESincronizarNET->SQL, BESincronizarNET->Site->sConteudo, strlen (BESincronizarNET->Site->sConteudo));
						}
						//substituir conteúdo
						BE_free (sTemp1);
						sTemp1 = g_strdup_printf ("'%s'", sTemp);//valor
						sSQLConsulta = be_strreplace ((gchar *) ListaL->data, "$Conteudo$", sTemp1);
						//loop na lista de variáveis, substituir
						for (ListaL1 = Variaveis, nVariavel = 1; ListaL1; ListaL1 = g_slist_next (ListaL1), nVariavel++)
						{
							//obter o primeiro parâmetro da comparação
							BE_free (sTemp);
							sTemp = be_substr ((gchar *) ListaL1->data, 0, be_strpos ((gchar *) ListaL1->data, "$$", 0));
							//obter o segundo parâmetro da comparação
							BE_free (sTemp1);
							sTemp1 = be_substr ((gchar *) ListaL1->data, be_strpos ((gchar *) ListaL1->data, "$$", 0) + 2, 0);
							//obter o valor da variável
							if (sTemp && sTemp1)
							{
								BE_free (sVariavel);
								sVariavel = be_strextract (BESincronizarNET->Site->sConteudo, sTemp, sTemp1, 0);
								//substituir variavel por seu valor em cada consulta
								if (sVariavel)
								{
									//convertendo o valor
									if (BESincronizarNET->bUTF8)
									{
										BE_free (sTemp);
										sTemp = sVariavel;
										sVariavel = BE_UTF8_ISO (sTemp);
									}
									//escapando o valor
									BE_free (sTemp);
									sTemp = sVariavel;
									sVariavel = be_sql_escapar (BESincronizarNET->SQL, sTemp, strlen (sTemp));
									BE_free (sTemp);
									sTemp = sVariavel;
									sVariavel = g_strdup_printf ("'%s'", sTemp);//valor
									//substituir conteúdo
									BE_free (sTemp);
									BE_free (sTemp1);
									sTemp = sSQLConsulta;
									sTemp1 = g_strdup_printf ("$%d$", nVariavel);
									sSQLConsulta = be_strreplace (sTemp, sTemp1, sVariavel);
								}
							}
						}
						//loop nos campos, verificar valores provenientes de campos de um loop SQL
						for (ListaL2 = Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
						{
							Campo = (BESQL_Campo *) ListaL2->data;
							//preparando valores
							BE_free (sVariavel);
							if (Campo->sValor)
							{
								//escapando o valor
								sVariavel = be_sql_escapar (BESincronizarNET->SQL, Campo->sValor, strlen (Campo->sValor));
								BE_free (sTemp);
								sTemp = sVariavel;
								sVariavel = g_strdup_printf ("'%s'", sTemp);//valor
							}
							else
								sVariavel = g_strdup ("NULL");
							//recuperar nome para comparação
							BE_free (sTemp);
							sTemp = g_strdup_printf ("$`%s`$", Campo->sNome);
							//substituir campos por valores
							BE_free (sTemp1);
							sTemp1 = sSQLConsulta;
							sSQLConsulta = be_strreplace (sTemp1, sTemp, sVariavel);
							//printf ("%s = %s\n", Campo->sNome, Campo->sValor);
						}
						//informar Log
						if (BESincronizarNET->bLog)
							BE_sincronizar_log_informar ("\tConsulta SQL: %s\n", sSQLConsulta);
						//realizar as alterações no BD
						nAfetadas = be_sql_executar (BESincronizarNET->SQL, sSQLConsulta, NULL, NULL, &sErros);
						//informar Log
						if (BESincronizarNET->bLog && sErros)
							BE_sincronizar_log_informar ("\t\tErro7: %s\n", sErros);
						BE_free (sErros);
					}
				}
				//destravar tabelas SQL local
				if (sSQLDestravarTabela)
				{
					be_sql_executar (BESincronizarNET->SQL, sSQLDestravarTabela, NULL, NULL, &sErros);
					//informar Log
					if (BESincronizarNET->bLog)
						BE_sincronizar_log_informar ("\tConsulta SQL: %s\n", sSQLDestravarTabela);
					BE_free (sErros);
				}
			}
			else if (BESincronizarNET->bLog)
				BE_sincronizar_log_informar ("\tSite %s esta invalido\n", BESincronizarNET->Site->sURL);

			//limpar lista de consultas
			BE_free (sSQLTravarTabela);
			BE_free (sSQLDestravarTabela);
			for (ListaL = SQLConsultas; ListaL; ListaL = g_slist_next (ListaL))
			{
				sTemp = (gchar *) ListaL->data;
				BE_free (sTemp);
			}
			BE_slist_free (SQLConsultas);
			for (ListaL = SQLAlternativas; ListaL; ListaL = g_slist_next (ListaL))
			{
				sTemp = (gchar *) ListaL->data;
				BE_free (sTemp);
			}
			BE_slist_free (SQLAlternativas);
			for (ListaL = Variaveis; ListaL; ListaL = g_slist_next (ListaL))
			{
				sTemp = (gchar *) ListaL->data;
				BE_free (sTemp);
			}
			BE_slist_free (Variaveis);
		}
		//limpar
		BE_free (sNetArquivo);
		BE_free (sNetArquivoT);
		//avançar próximo site
		nNet++;
		BE_free (sNet);
		sNet = g_strdup_printf ("Net%d_URL", nNet);//Net0_URL
		BE_free (sTemp);
	}
	//limpar
	BE_free (sNetCampo);
    BE_free (sConsulta);
    BE_free (sAlternativa);
    BE_free (sVariavel);
    BE_free (sNet);
    BE_free (sPular);
    BE_free (sURL);
    BE_free (sTemp);
    BE_free (sTemp1);
    BE_free (sTemp2);
    BE_free (sTemp3);
    
	return sErros;
}

gboolean _be_sincronizar_callback_ODBC (const BEODBC *ODBC, gint nCampos, gint nLinha, GSList *Campos, gint nTipo, gint nComp, GSList *Valores, gpointer user_data)
{
/*******************************************************************************
Esta função é um retorno de outra função, be_odbc_executar. Callback.
Para cada linha de registro retornada, esta função é automaticamente chamada.
Nas consultas SQL local, são substituídas as variáveis pelos valores de ODBC, e
então executado uma consulta local.
Argumentos:
    ODBC : conexão ODBC
    nCampos : quantidade de campos
    nLinha : número da linha atual
	Campos : lista com os campos, cada um é do tipo *gchar
	Valores : lista com os valores, cada um é do tipo *gchar
	user_data : informação extra, ponteiro para a estrutura _BESincronizarODBC
    Retorno: se é para continuar o loop das linhas, do ODBC
*******************************************************************************/
	_BESincronizarODBC *BESincronizarODBC = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL, *Consulta = NULL;
	gchar *sTemp = NULL, *sConsulta = NULL, *sCampo = NULL, *sValor = NULL;
	gchar *sErros = NULL;
	gint nAfetadas = 0;
	//recuperar estrutura previamente criada em be_sincronizar_executar_odbc
    BESincronizarODBC = (_BESincronizarODBC *) user_data;
    //contar linhas ODBC
    BESincronizarODBC->nLinhasODBC++;
    //loop das Consulta
    for (Consulta = BESincronizarODBC->SQLAlternativas; Consulta && (nAfetadas <= 0); Consulta = g_slist_next (Consulta))
    {
        //preparar consulta
        BE_free (sConsulta);
        sConsulta = g_strdup ((gchar *) Consulta->data);
        //verificar consultas
        if (sConsulta && BESincronizarODBC->SQL)
        {
            //loop nas campos e valores ODBC
            for (ListaL = Campos, ListaL1 = Valores; ListaL; ListaL = g_slist_next (ListaL), ListaL1 = g_slist_next (ListaL1))
            {
                //limpar
                BE_free (sTemp);
                BE_free (sCampo);
                BE_free (sValor);
                //preparar valores ODBC para substituir as variáveis da consulta SQL local
                sCampo = g_strdup_printf ("$%s$", (gchar *) ListaL->data);//campo
                sTemp = be_sql_escapar (BESincronizarODBC->SQL, (gchar *) ListaL1->data, strlen ((gchar *) ListaL1->data));//preparando valor
                sValor = g_strdup_printf ("'%s'", sTemp);//valor
                BE_free (sTemp);
                sTemp = sConsulta;
                sConsulta = be_strreplace (sTemp, sCampo, sValor);
            }
            //executar consulta SQL local, baseada nos valores de ODBC
            nAfetadas = be_sql_executar (BESincronizarODBC->SQL, sConsulta, NULL, NULL, &sErros);
			//informar Log
			if (BESincronizarODBC->bLog && sErros)
				BE_sincronizar_log_informar ("\t\tErro8: %s\n", sErros);
			BE_free (sErros);
        }
    }
    //informar progresso, somente na tela
    if (BESincronizarODBC->bProgresso)
        printf ("Linha: %d\n", nLinha);

    //informar da falha
    /*if ((nAfetadas <= 0)  && sConsulta)
    {
        BE_sincronizar_log_informar ("Nao houve alteracao. Ultima tentativa: %s\n", sConsulta);
    }*/

    //contar linhas SQL
    if (nAfetadas > 0)
        BESincronizarODBC->nLinhasSQL += nAfetadas;

	//informar erros
	if (sErros)
	{
        BE_sincronizar_log_informar ("\t\tErro9: %s\n", sErros);
        BE_free (sErros);
        BESincronizarODBC->bErro = TRUE;
	}
	//limpar
	BE_free (sErros);
	BE_free (sTemp);
	BE_free (sConsulta);
	BE_free (sCampo);
	BE_free (sValor);

    //parar o loop ODBC se a conexão SQL cair
    if (BESincronizarODBC->SQL)
        return TRUE;
    else
        return FALSE;
}

gboolean be_sincronizar_salvar_arquivo (const gchar *sArquivo, const gchar *sConteudo, gint nLen)
/*******************************************************************************
Esta função salva o conteúdo de uma string em um arquivo.
Argumentos:
    sArquivo : nome e caminho completo do arquivo
    sConteudo : conteúdo à ser gravado
    nLen : comprimento de sConteudo
    Retorno: se foi gravado com sucesso
*******************************************************************************/
{
	FILE *pFile = NULL;
	gboolean bOK = FALSE;

	//somente com conteúdo
	if (sConteudo)
	{
		//criar arquivo, binário
		if ((pFile = fopen (sArquivo, "wb")))
		{
			//gravando conteúdo baixado no arquvio
			fwrite (sConteudo, nLen, 1, pFile);
			//fechar arquivo
			fclose (pFile);
			//verificar se o arquivo foi realmente gravado
			if (g_file_test (sArquivo, G_FILE_TEST_EXISTS))
				bOK = TRUE;
		}
	}

	return bOK;
}
