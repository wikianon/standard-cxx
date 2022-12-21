#include "besql.h"

static FILE *FileSQL = NULL;

BESQL *be_sql_abrir_conexao_segura (const gchar *sUsuario, const gchar *sSenha, const gchar *sIP, guint nPorta, gint nCacheMax, gchar **sErros, const gchar *sSSLChavePrivada, const gchar *sSSLCertificado, const gchar *sSSLCA, const gchar *sSSLCACaminho, const gchar *sSSLCifra)
{
	MYSQL *my = NULL;//conexão
	MYSQL_RES *res = NULL, *res1 = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	BESQL *SQL = NULL;
	BESQL_Tabela *Tabela = NULL;
	BESQL_BD *BD = NULL;//banco de dados
	gchar *sTemp = NULL, *sArquivoSQL = NULL;
	gint nCont = 0, nCont1 = 0;
	gchar *sValor = NULL;
	gchar *sSSL1 = NULL, *sSSL2 = NULL, *sSSL3 = NULL, *sSSL4 = NULL, *sSSL5 = NULL;
	gchar *sSQL = NULL, *sHash = NULL;
	gboolean bHashSegunda = FALSE;

	//abrir arquivo de depuração
#ifdef BESQL_DEPURACAO_SQL
	//preparar configuração local
	BE_free (sTemp);
	BE_free (sArquivoSQL);
	sTemp = be_diretorio_local (TRUE);
	sArquivoSQL = g_strdup_printf ("%s%s", sTemp, _BESQL_ARQUIVO_SAIDA);
	if ((FileSQL = fopen (sArquivoSQL, "ab")))//adicionar binário
	{
		BE_free (sTemp);
		sTemp = g_strdup_printf ("%s - ", __FILE__);
		fwrite (sTemp, strlen (sTemp), 1, FileSQL);
		BE_free (sTemp);
		sTemp = BE_tempo ("%d/%m/%Y %H:%M:%S");
		fwrite (sTemp, strlen (sTemp), 1, FileSQL);
		BE_free (sTemp);
		sTemp = BE_tempo ("\n");
		fwrite (sTemp, strlen (sTemp), 1, FileSQL);
	}
#endif

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

	//reservando memória principal
	SQL = g_new (BESQL, 1);
	SQL->sUsuario = g_strdup (sUsuario);
	if (sIP)
		SQL->sIP = g_strdup (sIP);
	else
		SQL->sIP = NULL;
    SQL->bSuper = FALSE;
	SQL->nPorta = nPorta;
	SQL->BDs = NULL;
	SQL->nCacheMax = nCacheMax;
	my = g_new (MYSQL, 1);
	SQL->_my = my;
	SQL->func_Info = NULL;
	SQL->user_data = NULL;
	//iniciando a biblioteca do mysql
	if (mysql_library_init (0, NULL, NULL) == 0)
	{
		if (mysql_init (my))//reservando memória
		{
			//preprar senha
			if (sSenha)
				sValor = be_sql_escapar (SQL, sSenha, strlen (sSenha));
            //considerar a senha padrão dentro de [client], dentro de my.cnf
			mysql_options (SQL->_my, MYSQL_READ_DEFAULT_GROUP, "client");
			//preparar conexão segura
			if (sSSLChavePrivada)
                sSSL1 = g_strdup (sSSLChavePrivada);
            if (sSSLCertificado)
                sSSL2 = g_strdup (sSSLCertificado);
            if (sSSLCA)
                sSSL3 = g_strdup (sSSLCA);
            if (sSSLCACaminho)
                sSSL4 = g_strdup (sSSLCACaminho);
            if (sSSLCifra)
                sSSL5 = g_strdup (sSSLCifra);
            mysql_ssl_set (SQL->_my, sSSL1, sSSL2, sSSL3, sSSL4, sSSL5);
			if (!mysql_real_connect (SQL->_my, SQL->sIP, SQL->sUsuario, sValor, NULL, SQL->nPorta, NULL, CLIENT_MULTI_STATEMENTS))
			{
				//falha na conexão
				if (sErros)
				{
					BE_free (*sErros);
					BE_free (sTemp);
					if ((int) mysql_errno (my) == 1226)
                        sTemp = g_strdup_printf ("Erro de conexão: O usuário %s já atingiu o máximo de conexões simultânes.", SQL->sUsuario);
					else
                        sTemp = g_strdup_printf ("Erro de conexão: (%d) %s", mysql_errno (my), mysql_error (my));

					*sErros = BE_ISO_UTF8 (sTemp);
				}
				else
				{
					if ((int) mysql_errno (my) == 1226)
                        printf ("%s:%d Erro de conexao: O usuario '%s' ja atingiu o maximo de conexoes simultaneas.\n", __FILE__, __LINE__, SQL->sUsuario);
					else
                        printf ("%s:%d Erro de conexao: (%d) %s\n", __FILE__, __LINE__, mysql_errno (my), mysql_error (my));
				}
				BE_sql_fechar_conexao (SQL);
			}
		}
		else//alocação de memória falhou
		{
			if (sErros)
			{
				BE_free (*sErros);
				BE_free (sTemp);
				sTemp = g_strdup ("Memória insuficiente.");
				*sErros = BE_ISO_UTF8 (sTemp);
			}
			else
				printf ("%s:%d Memoria insuficiente\n", __FILE__, __LINE__);
			//finalizando a biblioteca
			BE_sql_fechar_conexao (SQL);
		}
	}
	else//servidor não responde
	{
		if (sErros)
		{
			BE_free (*sErros);
			BE_free (sTemp);
			sTemp = g_strdup ("Não foi possível iniciar a conexão com o servidor MySQL.");
			*sErros = BE_ISO_UTF8 (sTemp);
		}
		else
			printf ("%s:%d Nao foi possivel iniciar a conexao com o servidor MySQL\n", __FILE__, __LINE__);
		BE_sql_fechar_conexao (SQL);
	}

	//verificar acesso de super usuário
	if (SQL)
        SQL->bSuper = _be_sql_acesso_super (SQL, sErros);

	//recuperar banco de dados disponíveis
	if (SQL)
	{
		if ((res = mysql_list_dbs (SQL->_my, NULL)))//recebe a consulta
		{
			for (nCont = 0; nCont < mysql_num_rows (res); nCont++)
			{
				//carregando bds
				row = mysql_fetch_row (res);
				if (!g_str_equal (row[0], _BESQL_INFORMACOES))//não carregar information_schema
				{
					BD = g_new (BESQL_BD, 1);
					BD->SQL = SQL;
					BD->Tabelas = NULL;
					BD->sNome = g_strdup (row[0]);
					BD->bMascara = FALSE;
					//verirficar se o BD pode ser CIA
					_be_sql_BD_CIA (BD, sErros);
					//hash na primeira tentativa
					bHashSegunda = FALSE;
					//carregando tabelas
					if (mysql_select_db (SQL->_my, BD->sNome) == 0)//selecionar bd
					{
						if ((res1 = mysql_list_tables (SQL->_my, NULL)))//recebe a consulta
						{
							for (nCont1 = 0; nCont1 < mysql_num_rows (res1); nCont1++)
							{
								//carregando tabelas
								row = mysql_fetch_row (res1);
								//obter hash da tabela/visão
								if (!bHashSegunda &&  !g_str_equal ("mysql", BD->sNome))
								{
									BE_free (sSQL);
									sSQL = g_strdup_printf ("SELECT MD5(Data) AS Hash FROM %s.`cache_tabelas` WHERE Tabela = '%s';", BD->sNome, row[0]);
									BE_free (sHash);
									sHash = g_strdup ("Hash");
									be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sHash, sErros);//se houver erros, ignorar
								}
								if ((sErros && *sErros) || bHashSegunda)
								{
									BE_free (*sErros);
									BE_free (sHash);
									bHashSegunda = TRUE;
									//segunda tentativa, principalmente para root e adm
									BE_free (sSQL);
									sSQL = g_strdup_printf ("SELECT MD5(CREATE_TIME) AS Hash FROM information_schema.`TABLES` WHERE TABLE_SCHEMA = '%s' AND TABLE_NAME= '%s';", BD->sNome, row[0]);
									BE_free (sHash);
									sHash = g_strdup ("Hash");
									be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sHash, sErros);//se houver erros, ignorar
									if (*sErros)
									{
										BE_free (*sErros);
										BE_free (sHash);
									}
								}
								//carregar informações em cache local ou do servidor
								if (!(Tabela = _be_sql_carregar_cache_local_tabela (BD, row[0], sHash)))
								{
									Tabela = g_new (BESQL_Tabela, 1);//criar estrutura tabela
									Tabela->BD = BD;
									Tabela->sNome = g_strdup (row[0]);
									Tabela->sNomeReal = NULL;//CIA
									Tabela->sNomeUsual = NULL;//CIA
									if (SQL->bSuper)
										Tabela->nFlags = BESQL_ACESSO_COMPLETO;//super acesso
									else
										Tabela->nFlags = _be_sql_acesso (Tabela, sErros);//acesso
									Tabela->sComentarios = NULL;//informação extra
									Tabela->Relatorios = NULL;//informação extra
									Tabela->bCIA = FALSE;
									Tabela->SintaxeCache = NULL;
									Tabela->nCopiasSintaxeCache = 0;
									Tabela->nTempoCache = 0;
									//carregar informações extras
									_be_sql_tabela_CIA (Tabela, sErros);
									//anexando na lista
									BD->Tabelas = g_slist_append (BD->Tabelas, Tabela);

									//salvar em disco toda estrutura de Sintaxe, e abaixo dela
									_be_sql_gravar_cache_local_tabela (Tabela, sHash);
								}
							}
							mysql_free_result (res1);
						}
						else//erro no retorno
						{
							_be_sql_erro_retorno (SQL, sErros);
							BE_sql_fechar_conexao (SQL);
						}
					}
					else//erro de consulta
					{
						_be_sql_erro_consulta (SQL, sErros, NULL);
					}
					//anexando na lista
					SQL->BDs = g_slist_append (SQL->BDs, BD);
					//verificar máscaras
					BD->bMascara = _be_sql_mascaras (SQL, BD, sErros);
				}
			}
			mysql_free_result (res);
		}
		else//erro no retorno
		{
			_be_sql_erro_retorno (SQL, sErros);
			BE_sql_fechar_conexao (SQL);
		}
	}

	BE_free (sValor);
	BE_free (sArquivoSQL);
	BE_free (sSSL1);
	BE_free (sSSL2);
	BE_free (sSSL3);
	BE_free (sSSL4);
	BE_free (sSSL5);
	BE_free (sTemp);

	return SQL;
}

BESQL_Tabela *_be_sql_carregar_cache_local_tabela (BESQL_BD *BD, gchar *sTabela, gchar *sHashMD5)
{
	BESQL_Tabela *Tabela = NULL;
	FILE *File = NULL;
	gchar *sArquivo = NULL;
	gchar *sTemp = NULL, *sHashMD5comp = NULL;
	guchar cIndicador;
	glong lComprimento = 0, lQuantidade = 0;
	gint nCont = 0;

	//verificar existência de hash
	if (!sHashMD5 || strlen (sHashMD5) != 32)
		return NULL;

	//preparar local
	BE_free (sTemp);
	sTemp = be_diretorio_local (TRUE);
	//nome do arquivo
	sArquivo = g_strdup_printf ("%scache_%s_%s_tabela.dat", sTemp, BD->sNome, sTabela);

	//abrindo arquivo
	if ((File = fopen (sArquivo, "rb")))
	{
		//obter hash MD5, comprimento fixo padrao 32
		sHashMD5comp = (gchar *) g_malloc (33); //alocar memória
		if (fread (sHashMD5comp, 32, 1, File) > 0)
		{
			//comparar hashs
			sHashMD5comp[32] = '\0';
			if (g_strstr_len (sHashMD5comp, 32, sHashMD5))
			{
				//obter tabela
				fread (&cIndicador, sizeof (cIndicador), 1, File);//indicador
				if (cIndicador == 't')
				{
					fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					//verificar comprimento da estrutura
					if (lComprimento == sizeof (BESQL_Tabela))
					{
						//obter tabela
						Tabela = g_new (BESQL_Tabela, 1);
						fread (Tabela, sizeof (BESQL_Tabela), 1, File);//dados
						Tabela->BD = BD;//integrando aos BD
						Tabela->SintaxeCache = NULL;
						BD->Tabelas = g_slist_append (BD->Tabelas, Tabela);//anexando na lista de BDs
						//obter nome, verificar lixo
						if (Tabela->sNome)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							Tabela->sNome = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (Tabela->sNome, lComprimento, 1, File);//dados
							Tabela->sNome[lComprimento] = '\0';//fim de string
						}
						//obter nome real, verificar lixo
						if (Tabela->sNomeReal)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							Tabela->sNomeReal = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (Tabela->sNomeReal, lComprimento, 1, File);//dados
							Tabela->sNomeReal[lComprimento] = '\0';//fim de string
						}
						//obter nome usual, verificar lixo
						if (Tabela->sNomeUsual)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							Tabela->sNomeUsual = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (Tabela->sNomeUsual, lComprimento, 1, File);//dados
							Tabela->sNomeUsual[lComprimento] = '\0';//fim de string
						}
						//obter comentarios, verificar lixo
						if (Tabela->sComentarios)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							Tabela->sComentarios = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (Tabela->sComentarios, lComprimento, 1, File);//dados
							Tabela->sComentarios[lComprimento] = '\0';//fim de string
						}
						//obter relatórios
						if (Tabela->Relatorios)
						{
							//obter quantidade
							fread (&lQuantidade, sizeof (lQuantidade), 1, File);
							Tabela->Relatorios = NULL;//preparar lista
							//loop nos indices
							for (nCont = 0; nCont < lQuantidade; nCont++)
							{
								fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
								sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
								fread (sTemp, lComprimento, 1, File);//dados
								sTemp[lComprimento] = '\0';//fim de string
								Tabela->Relatorios = g_slist_append (Tabela->Relatorios, sTemp);//adicionar na lista

							}
							sTemp = NULL;
						}
					}
				}
			}
		}
	}

	//limpar
	BE_free (sTemp);
	BE_free (sArquivo);
	BE_free (sHashMD5comp);

	return Tabela;
}

void _be_sql_gravar_cache_local_tabela (BESQL_Tabela *Tabela, gchar *sHashMD5)
{
	FILE *File = NULL;
	GSList *ListaL = NULL;
	gchar *sArquivo = NULL;
	gchar *sTemp = NULL;
	guchar cIndicador;
	glong lComprimento = 0, lQuantidade = 0;

	//verificar existência de hash
	if (!sHashMD5 || strlen (sHashMD5) != 32)
		return;

	//preparar local
	BE_free (sTemp);
	sTemp = be_diretorio_local (TRUE);
	//nome do arquivo
	sArquivo = g_strdup_printf ("%scache_%s_%s_tabela.dat", sTemp, Tabela->BD->sNome, Tabela->sNome);

	//Abrindo arquivo:
	if ((File = fopen (sArquivo, "wb")))
	{
		//gravar hash MD5, comprimento fixo padrao 32
		fwrite (sHashMD5, 32, 1, File);//dados
		//gravar tabela
		cIndicador = 't';
		fwrite (&cIndicador, sizeof (cIndicador), 1, File);//indicador
		lComprimento = sizeof (BESQL_Tabela);
		fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
		fwrite (Tabela, lComprimento, 1, File);//dados
		//nome
		if (Tabela->sNome)
		{
			lComprimento = sizeof (gchar) * strlen (Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Tabela->sNome, lComprimento, 1, File);//dados
		}
		//nome real
		if (Tabela->sNomeReal)
		{
			lComprimento = sizeof (gchar) * strlen (Tabela->sNomeReal);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Tabela->sNomeReal, lComprimento, 1, File);//dados
		}
		//nome usual
		if (Tabela->sNomeUsual)
		{
			lComprimento = sizeof (gchar) * strlen (Tabela->sNomeUsual);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Tabela->sNomeUsual, lComprimento, 1, File);//dados
		}
		//comentarios
		if (Tabela->sComentarios)
		{
			lComprimento = sizeof (gchar) * strlen (Tabela->sComentarios);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Tabela->sComentarios, lComprimento, 1, File);//dados
		}
		//gravar relatórios
		if (Tabela->Relatorios)
		{
			lQuantidade = g_slist_length (Tabela->Relatorios);
			fwrite (&lQuantidade, sizeof (lQuantidade), 1, File);//quantidade de ítens
			BE_free (sTemp);
			for (ListaL = Tabela->Relatorios; ListaL; ListaL = g_slist_next (ListaL))
			{
				sTemp = (gchar *) ListaL->data;
				lComprimento = sizeof (gchar) * strlen (sTemp);
				fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
				fwrite (sTemp, lComprimento, 1, File);//dados
			}
			sTemp = NULL;
		}
	}

	//limpar
	BE_free (sTemp);
	BE_free (sArquivo);
}

gboolean _be_sql_mascaras (BESQL *SQL, BESQL_BD *BD, gchar **sErros)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gint nCont = 0, nCont1 = 0;
	gboolean bMascara = FALSE;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return FALSE;

	if (!BD)
		return FALSE;

	sSQL = g_strdup_printf ("SELECT SPECIFIC_NAME FROM information_schema.ROUTINES WHERE ROUTINE_SCHEMA = '%s' AND ROUTINE_TYPE= 'FUNCTION'", BD->sNome);//pag. 1019
	_be_sql_saida (sSQL);//depuração
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			for (nCont = 0; nCont < mysql_num_rows (res); nCont++)
			{
				//próxima linha
				row = mysql_fetch_row (res);
				if (g_str_equal (row[0], BESQL_MASCARA_AMNUM))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMNUM))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMMOE))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMMOE))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMDAT))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMDAT))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMTEM))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMTEM))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMFONx))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMFONx))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMFON))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMFON))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMCNPJ))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMCNPJ))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMCEP))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMCEP))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMCPF))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMCPF))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_AMBOL))
					nCont1++;
				else if (g_str_equal (row[0], BESQL_MASCARA_RMBOL))
					nCont1++;
			}
			mysql_free_result (res);
		}
		else//erro no retorno
		{
			_be_sql_erro_retorno (SQL, sErros);
		}
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}
	//verificar quantidade
	if (nCont1 == 20)
		bMascara = TRUE;

	//limpar
	BE_free (sSQL);

	return bMascara;
}

void be_sql_callback (
	BESQL *SQL,
	gboolean (*func_Info)(struct __BESQL *SQL, gint nErro, const gchar *sErro, gpointer user_data),
	gpointer user_data
)
{
	//iniciando callback
	if (SQL)
	{
		SQL->func_Info = func_Info;
		SQL->user_data = user_data;
	}
}

void _be_sql_fechar_conexao (BESQL *SQL)
{
	BESQL_BD *BD = NULL;
	BESQL_Tabela *Tabela = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL, *ListaL2 = NULL;

	if (SQL)
	{
		//liberando memória
		mysql_close (SQL->_my);
		//finalizando a biblioteca
		mysql_library_end ();
		//liberando estrutura
		BE_free (SQL->sUsuario);
		BE_free (SQL->sIP);
		BE_free (SQL->_my);
		//liberar banco de dados
		for (ListaL = SQL->BDs; ListaL; ListaL = g_slist_next (ListaL))
		{
			BD = (BESQL_BD *) ListaL->data;
#ifdef BESQL_DEPURACAO
			printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, BD->sNome);
#endif
			//liberar tabelas
			for (ListaL1 = BD->Tabelas; ListaL1; ListaL1 = g_slist_next (ListaL1))
			{
				Tabela = (BESQL_Tabela *) ListaL1->data;
#ifdef BESQL_DEPURACAO
				printf ("DEPURACAO: %s:%d - \t%s - %s (%s) - %d - %d\n", __FILE__, __LINE__, Tabela->sNome, Tabela->sNomeReal, Tabela->sComentarios, g_slist_length (Tabela->Relatorios), Tabela->nFlags);
#endif
				BE_free (Tabela->sNome);
				BE_free (Tabela->sNomeReal);
				BE_free (Tabela->sNomeUsual);
				BE_free (Tabela->sComentarios);
				//liberar relatórios
				for (ListaL2 = Tabela->Relatorios; ListaL2; ListaL2 = g_slist_next (ListaL2))
				{
#ifdef BESQL_DEPURACAO
					printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, (gchar *) ListaL2->data);
#endif
					BE_free (ListaL2->data);
				}
				BE_sql_fechar_consulta (Tabela->SintaxeCache);
                //liberar relatórios
				BE_slist_free (Tabela->Relatorios);
			}
			BE_slist_free (BD->Tabelas);
			BE_free (BD->sNome);
			BE_free (BD);
		}
		BE_slist_free (SQL->BDs);
		//liberar estrutura principal
		BE_free (SQL);
	}

	//fechar arquivo de depuração
	if (FileSQL)
	{
		fwrite ("\n", 1, 1, FileSQL);
		fclose (FileSQL);
		FileSQL = NULL;
	}
}

gboolean be_sql_alterar_senha (BESQL *SQL, const gchar *sNovaSenha, gchar **sErros)
{
	gchar *sSQL = NULL;
	glong lAfetados = -1;
	gchar *sValor = NULL;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return FALSE;

	//não alterar senha inválida
	if (!sNovaSenha)
		return FALSE;

	//preparando valor
	sValor = be_sql_escapar (SQL, sNovaSenha, strlen (sNovaSenha));
	//preparar consulta
	if (sValor)
		sSQL = g_strdup_printf ("SET PASSWORD = PASSWORD('%s')", sValor);
	else
		sSQL = g_strdup_printf ("SET PASSWORD = PASSWORD('')");
    _be_sql_saida (sSQL);//depuração
	//executar
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
		//erro no retorno
		if (lAfetados < 0)
			_be_sql_erro_alteracao (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	BE_free (sSQL);
	BE_free (sValor);

	if (lAfetados == 0)
		return TRUE;
	else
		return FALSE;
}

gchar *be_sql_escapar (BESQL *SQL, const gchar *sValor, glong lBytes)
{
	gchar *sNovoValor = NULL;

	if (!sValor || lBytes < 0)
		return NULL;
	else if (lBytes == 0)
		return g_strdup ("");

	sNovoValor = (gchar *) g_malloc (lBytes * sizeof (gchar) * 2 + 1);//reservando espaço
	mysql_real_escape_string (SQL->_my, sNovoValor, sValor, lBytes);//verificando valor

	return sNovoValor;
}

void __be_sql_erro_retorno (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros)
{
	gchar *sTemp = NULL;
	gchar *sErro = NULL;
	gboolean bDepurar = TRUE;
	gint nErro = 0;

	//recuperar erros
	nErro = (gint) mysql_errno (SQL->_my);
	sErro = g_strdup (mysql_error (SQL->_my));
	//callback
	if (SQL->func_Info)
		bDepurar = SQL->func_Info (SQL, nErro, sErro, SQL->user_data);

	if (bDepurar)
	{
		if (sErros)
		{
			BE_free (*sErros);
			sTemp = g_strdup_printf ("Não há dados para retornar.\n\n(%d) %s", nErro, sErro);
			*sErros = BE_ISO_UTF8 (sTemp);
			BE_free (sTemp);
		}
		else
			printf ("%s:%d Não há dados para retornar: (%d) %s\n", sArquivo, nLinha, nErro, sErro);
	}
	//saida
	sTemp = g_strdup_printf ("(%d) %s", nErro, sErro);
	__be_sql_saida (nLinha, sTemp);

    BE_free (sTemp);
	BE_free (sErro);
}

void __be_sql_erro_alteracao (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros)
{
	gchar *sTemp = NULL;
	gchar *sErro = NULL;
	gboolean bDepurar = TRUE;
	gint nErro = 0;

	//recuperar erros
	nErro = (gint) mysql_errno (SQL->_my);
	sErro = g_strdup (mysql_error (SQL->_my));
	//callback
	if (SQL->func_Info)
		bDepurar = SQL->func_Info (SQL, nErro, sErro, SQL->user_data);

	if (bDepurar)
	{
		if (sErros)
		{
			BE_free (*sErros);
			sTemp = g_strdup_printf ("Não foi possível alterar os dados.\n\n(%d) %s", nErro, sErro);
			*sErros = BE_ISO_UTF8 (sTemp);
			BE_free (sTemp);
		}
		else
			printf ("%s:%d Não foi possível alterar os dados: (%d) %s\n", sArquivo, nLinha, nErro, sErro);
	}
	//saida
	sTemp = g_strdup_printf ("(%d) %s", nErro, sErro);
	__be_sql_saida (nLinha, sTemp);

    BE_free (sTemp);
	BE_free (sErro);
}

void __be_sql_erro_consulta (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros, const gchar *sConsulta)
{
	gchar *sTemp = NULL;
	gchar *sErro = NULL;
	gboolean bDepurar = TRUE;
	gint nErro = 0;

	nErro = (gint) mysql_errno (SQL->_my);
	//tradução
	switch (nErro)
	{
		case 2003://Can't connect to MySQL server on 'localhost'
			sErro = g_strdup_printf ("%s\n\n%s", "Não foi possível conectar com o servidor.", mysql_error (SQL->_my));
			break;
		case 2006://MySQL server has gone away
			sErro = g_strdup_printf ("%s\n\n%s", "A conexão com o servidor foi perdida.", mysql_error (SQL->_my));
			break;
		case 2013://Lost connection to MySQL server during query
            sErro = g_strdup_printf ("%s\n\n%s", "O servidor não responde.", mysql_error (SQL->_my));
			break;
		default:
			sErro = g_strdup (mysql_error (SQL->_my));
			break;
	}

	//incluir consulta atual
	if (sConsulta)
	{
	    sTemp = sErro;
	    sErro = g_strdup_printf ("%s\n{ %s }", sTemp, sConsulta);
	    BE_free (sTemp);
	}

	//callback
	if (SQL->func_Info)
		bDepurar = SQL->func_Info (SQL, nErro, sErro, SQL->user_data);

	if (bDepurar)
	{
		if (sErros)
		{
			BE_free (*sErros);
			sTemp = g_strdup_printf ("Erro de consulta: (%d) %s", nErro, sErro);
			*sErros = BE_ISO_UTF8 (sTemp);
			BE_free (sTemp);
		}
		else
			printf ("%s:%d Erro de consulta: (%d) %s\n", sArquivo, nLinha, nErro, sErro);
	}
	//saida
	sTemp = g_strdup_printf ("(%d) %s", nErro, sErro);
	__be_sql_saida (nLinha, sTemp);

    BE_free (sTemp);
	BE_free (sErro);
}

void __be_sql_saida (gint nLinha, const gchar *sConsulta)
{
	gchar *sTemp = NULL;

	//gravar arquivo
	if (FileSQL)
	{
        sTemp = BE_tempo ("%H:%M:%S");
        fwrite (sTemp, strlen (sTemp), 1, FileSQL);
        BE_free (sTemp);
		sTemp = g_strdup_printf (" %d  %s\n", nLinha, sConsulta);
		fwrite (sTemp, strlen (sTemp), 1, FileSQL);
		BE_free (sTemp);
	}
}

void __be_sql_aviso_retorno (const gchar *sArquivo, gint nLinha, BESQL *SQL, gchar **sErros)
{
	gchar *sTemp = NULL;
	gchar *sAviso = NULL;

	//verificar avisos (warnings)
	if (mysql_warning_count (SQL->_my) > 0)
	{
        //recuperar avisos, pela informação
        sAviso = be_sql_informacoes (SQL, BESQL_INFO_ALTERACAO, sErros);
        //recuperar avisos, pela consulta
        if (!sAviso)
            sAviso = be_sql_informacoes (SQL, BESQL_INFO_AVISO, sErros);
        //informar
        if (sAviso)
        {
            if (sErros)
            {
                BE_free (*sErros);
                sTemp = g_strdup_printf ("AVISO: %s", sAviso);
                *sErros = BE_ISO_UTF8 (sTemp);
                BE_free (sTemp);
            }
            else
                printf ("%s:%d AVISO: %s\n", sArquivo, nLinha, sAviso);
            //saida
            sTemp = g_strdup_printf ("AVISO: %s", sAviso);
            __be_sql_saida (nLinha, sTemp);
        }

        BE_free (sTemp);
        BE_free (sAviso);
	}
}

BESQL_ACESSO_ENUM _be_sql_acesso (BESQL_Tabela *Tabela, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	BESQL_ACESSO_ENUM nFlag = BESQL_ACESSO_NENHUM;
	gchar *sSQL = NULL;
	gint nCont = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return BESQL_ACESSO_NENHUM;

	//comunicação
	SQL = Tabela->BD->SQL;
	//verificar acesso
	sSQL = g_strdup_printf ("SELECT `PRIVILEGE_TYPE` FROM `information_schema`.`TABLE_PRIVILEGES` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='%s'", Tabela->BD->sNome, Tabela->sNome);
	_be_sql_saida (sSQL);//depuração
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			for (nCont = 0; nCont < mysql_num_rows (res); nCont++)
			{
				//recuperar campo
				row = mysql_fetch_row (res);
				if (g_str_equal (row[0], "SELECT"))
					nFlag += BESQL_ACESSO_SELECIONAR;
				else if (g_str_equal (row[0], "INSERT"))
					nFlag += BESQL_ACESSO_INSERIR;
				else if (g_str_equal (row[0], "UPDATE"))
					nFlag += BESQL_ACESSO_ALTERAR;
				else if (g_str_equal (row[0], "DELETE"))
					nFlag += BESQL_ACESSO_APAGAR;
			}
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

    //liberar memória
	BE_free (sSQL);

	return nFlag;
}

gboolean _be_sql_acesso_super (BESQL *SQL, gchar **sErros)
{
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gboolean bSuper = FALSE;
	gchar *sSQL = NULL;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return BESQL_ACESSO_NENHUM;
	//verificar acesso de super usuário
	BE_free (sSQL);
	sSQL = g_strdup_printf ("SHOW GRANTS");
	_be_sql_saida (sSQL);//depuração
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			row = mysql_fetch_row (res);
			if (g_strrstr (row[0], "GRANT ALL PRIVILEGES ON *.* TO"))
				bSuper = TRUE;
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}
	//liberar memória
    BE_free (sSQL);

    return bSuper;
}

void _be_sql_BD_CIA (BESQL_BD *BD, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gint nCont = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;
	//comunicação
	SQL = BD->SQL;
	BD->bCIA = FALSE;//inicial
	//localizar a existência de sis_tabela
	sSQL = g_strdup_printf ("SELECT `PRIVILEGE_TYPE` FROM `information_schema`.`TABLE_PRIVILEGES` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='sis_tabelas'", BD->sNome);
	_be_sql_saida (sSQL);//depuração
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			for (nCont = 0; nCont < mysql_num_rows (res); nCont++)
			{
				//recuperar campo
				row = mysql_fetch_row (res);
				if (g_str_equal (row[0], "SELECT"))
                    BD->bCIA = TRUE;
			}
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}
}

void _be_sql_tabela_CIA (BESQL_Tabela *Tabela, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gchar **aLista = NULL;
	gint nCont = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;

	//comunicação
	SQL = Tabela->BD->SQL;
	//continuar somente se sis_tabelas existir
	if (Tabela->BD->bCIA)
	{
        //verificar acesso
        BE_free (sSQL);
        sSQL = g_strdup_printf ("SELECT `Comentarios`, `Relatorios`, `Tabela`, `Usual`, `Icone` FROM `%s`.`sis_tabelas` WHERE `Visao`='vis_%s'", Tabela->BD->sNome, Tabela->sNome+4);
        _be_sql_saida (sSQL);//depuração
        if (mysql_query (SQL->_my, sSQL) == 0)
        {
            if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
            {
                if (mysql_num_rows (res) > 0)
                {
                    //recuperar campos
                    row = mysql_fetch_row (res);
                    //comentários
                    Tabela->sComentarios = g_strdup (row[0]);
                    //relatórios
                    if (row[1] && strlen (row[1]) > 0)
                    {
                        aLista = g_strsplit (row[1], ";", -1);
                        for (nCont = 0; nCont < g_strv_length (aLista); nCont++)
                        {
                            Tabela->Relatorios = g_slist_append (Tabela->Relatorios, g_strdup (aLista[nCont]));
                        }
                        BE_strfreev (aLista);
                    }
                    Tabela->sNomeReal = g_strdup (row[2]);
                    Tabela->sNomeUsual = g_strdup (row[3]);
                    //ícones
                    //g_strdup (row[4]);
                    //ativar sistema CIA
                    Tabela->bCIA = TRUE;
                }
                mysql_free_result (res);
            }
            else//erro no retorno
            {
                _be_sql_erro_retorno (SQL, sErros);
            }
        }
        else//erro na consulta
        {
            _be_sql_erro_consulta (SQL, sErros, sSQL);
        }
	}
	//limpar
	BE_free (sSQL);
}

BESQL_BD *be_sql_selecionar_bd (BESQL *SQL, const gchar *sNome, gchar **sErros)
{
	BESQL_BD *BD = NULL;
	GSList *ListaL = NULL;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

	if (sNome && SQL)
	{
		for (ListaL = SQL->BDs; ListaL; ListaL = g_slist_next (ListaL))
		{
			BD = (BESQL_BD *) ListaL->data;
			if (g_str_equal (BD->sNome, sNome))
			{
				//selecionar bd: USE DATABASES
				if (mysql_select_db (SQL->_my, BD->sNome) == 0)
					return BD;
				else//erro de consulta
				{
					_be_sql_erro_consulta (SQL, sErros, NULL);
				}
			}
		}
	}
	return NULL;
}

BESQL_Tabela *be_sql_selecionar_tabela (BESQL_BD *BD, const gchar *sNome)
{
	BESQL_Tabela *Tabela = NULL;
	GSList *ListaL = NULL;

	if (sNome && BD)
	{
		for (ListaL = BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
		{
			Tabela = (BESQL_Tabela *) ListaL->data;
			if (g_str_equal (Tabela->sNome, sNome))
				return Tabela;
		}
	}
	return NULL;
}

BESQL_Campo *be_sql_selecionar_campo (BESQL_Sintaxe *Sintaxe, const gchar *sNome)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;

	if (sNome && Sintaxe)
	{
		for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (g_str_equal (Campo->sNome, sNome))
				return Campo;
		}
	}
	return NULL;
}

BESQL_Campo *be_sql_selecionar_campo_id (BESQL_Sintaxe *Sintaxe)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;

	if (Sintaxe)
	{
		for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (g_str_equal (Campo->sNome, BESQL_CAMPO_ID))
				return Campo;
		}
	}
	return NULL;
}

BESQL_Campo *be_sql_selecionar_campo_auto_id (BESQL_Sintaxe *Sintaxe)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;

	if (Sintaxe)
	{
		for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAutoID)
				return Campo;
		}
	}
	return NULL;
}

BESQL_Campo *be_sql_selecionar_campo_indice (BESQL_Sintaxe *Sintaxe, BESQL_RELACAO_INDICE nTipo)
{
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL;

	if (Sintaxe)
	{
		for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
		{
			Indice = (BESQL_Indice *) ListaL->data;
			if (Indice->nTipo == nTipo)
				return Indice->Campo;
		}
	}
	return NULL;
}

BESQL_Indice *be_sql_selecionar_indice (BESQL_Sintaxe *Sintaxe, const gchar *sNome, const gchar *sNomeCampo)
{
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL;

	if (sNome && Sintaxe)
	{
		for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
		{
			Indice = (BESQL_Indice *) ListaL->data;
			if (g_str_equal (Indice->sNome, sNome))
			{
				if (sNomeCampo)
				{
					if (g_str_equal (Indice->Campo->sNome, sNomeCampo))
						return Indice;
				}
				else
					return Indice;
			}
		}
	}
	return NULL;
}

gboolean be_sql_inserir_arquivo (BESQL_Campo *Campo, const gchar *sArquivo, gchar cTipo)
{
	GString *Buffer = NULL;
	gulong lBytes = 0;
	guchar *cArquivo = NULL;
	gchar *sNome = NULL, *sTemp = NULL;
	const gint nBufferSIZE = BEGLOBAL_BLOCOS_ARQUIVO;
	guchar sBuffer[nBufferSIZE];
	gsize nSize = nBufferSIZE;
	gboolean bOK = FALSE;
	FILE *File = NULL;
	struct
	{
		gchar sNome[BESQL_BIN_COMPRIMENTO_NOME];
		gulong lBytesO;//comprimento em bytes, original
		gulong lBytes;//comprimento em bytes, compactado
		gchar cTipo;//tipo de arquivo
	} Bin;

	if (be_sql_campo_binario (Campo) && sArquivo)
	{
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
			lBytes = Buffer->len;
			if (lBytes > 0)
			{
                Bin.lBytesO = lBytes;//comprimento original
				//comprimir arquivo
				sTemp = BE_string_free (Buffer);
				cArquivo = be_comprimir_nivel (sTemp, &lBytes, 9);
				if (cArquivo)
				{
					//preparar nome
					sNome = g_path_get_basename (sArquivo);
					g_strlcpy (Bin.sNome, sNome, strlen (sNome) > (BESQL_BIN_COMPRIMENTO_NOME - 1) ? BESQL_BIN_COMPRIMENTO_NOME - 1 : strlen (sNome) + 1);
					Bin.sNome[BESQL_BIN_COMPRIMENTO_NOME - 1] = '\0';
					Bin.lBytes = lBytes;//comprimento compactado
					Bin.cTipo = cTipo;
					//reservando memória
					Campo->nComprimentoNovo = (sizeof (Bin) + lBytes * sizeof (guchar));
					Campo->sValorNovo = (gchar *) g_malloc (Campo->nComprimentoNovo);
					if (Campo->sValorNovo)
					{
						//copiar valores
						memcpy (Campo->sValorNovo, &Bin, sizeof (Bin));
						memcpy (Campo->sValorNovo + sizeof (Bin), cArquivo, lBytes);
						//marcar para alterar
						Campo->bAfetar = TRUE;

						bOK = TRUE;
					}
				}
			}
			else
			{
				g_string_free (Buffer, TRUE);
				Buffer = NULL;
			}
		}
	}

	BE_free (sNome);
	BE_free (cArquivo);
	BE_free (sTemp);

	return bOK;
}

gboolean be_sql_recuperar_arquivo (BESQL_Campo *Campo, const gchar *sArquivo, gchar cTipo)
{
	guchar *cArquivo = NULL;
	FILE *File = NULL;
	gboolean bOK = FALSE;
	struct
	{
		gchar sNome[BESQL_BIN_COMPRIMENTO_NOME];
		gulong lBytesO;//comprimento em bytes, original
		gulong lBytes;//comprimento em bytes, compactado
		gchar cTipo;//tipo de arquivo
	} Bin;

	if (Campo && Campo->sValor && sArquivo && be_sql_campo_binario (Campo))
	{
		//verificar formato
		if (Campo->sValor[sizeof (Bin.sNome) + sizeof (Bin.lBytesO) + sizeof (Bin.lBytes)] == cTipo)
		{
			//Abrindo arquivo:
			if ((File = fopen (sArquivo, "wb")))
			{
				//carregar estrutura
				memcpy (&Bin, Campo->sValor, sizeof (Bin));
				//decomprimindo
				if (Bin.lBytes > 0)
					cArquivo = (guchar *) be_comprimir_de ((guchar *) Campo->sValor + sizeof (Bin), &Bin.lBytes, Bin.lBytesO);

				if (cArquivo)
				{
					//gravar arquivo
					if (fwrite (cArquivo, Bin.lBytes, 1, File) == 1)
						bOK = TRUE;
				}
				fclose (File);
			}
		}
	}
	BE_free (cArquivo);

	return bOK;
}

GdkPixbuf *be_sql_recuperar_arquivo_pixbuf (BESQL_Campo *Campo, gchar cTipo)
{
	GdkPixbufLoader *Loader = NULL;
	GdkPixbuf *Imagem = NULL;
	guchar *cArquivo = NULL;
	struct
	{
		gchar sNome[BESQL_BIN_COMPRIMENTO_NOME];
		gulong lBytesO;//comprimento em bytes, original
		gulong lBytes;//comprimento em bytes, compactado
		gchar cTipo;//tipo de arquivo
	} Bin;

	if (Campo && Campo->sValor && be_sql_campo_binario (Campo))
	{
		//verificar formato
		if (Campo->sValor[sizeof (Bin.sNome) + sizeof (Bin.lBytesO) + sizeof (Bin.lBytes)] == cTipo)
		{
			//criando estrutura
			if ((Loader = gdk_pixbuf_loader_new ()))
			{
				//carregar estrutura
				memcpy (&Bin, Campo->sValor, sizeof (Bin));
				//decomprimindo imagem
				if (Bin.lBytes > 0)
					cArquivo = (guchar *) be_comprimir_de ((guchar *) Campo->sValor + sizeof (Bin), &Bin.lBytes, Bin.lBytesO);

				if (cArquivo)
				{
					//carregar imagem
					if (gdk_pixbuf_loader_write (Loader, cArquivo, (gsize) Bin.lBytes, NULL))
						Imagem = gdk_pixbuf_loader_get_pixbuf (Loader);
				}
				//limpar estrutura
				gdk_pixbuf_loader_close (Loader, NULL);
			}
		}
	}
	BE_free (cArquivo);

	return Imagem;
}

/*gboolean be_sql_inserir_imagem (BESQL_Campo *Campo, const gchar *sNome, GdkPixbuf *Imagem)
{
	gulong lBytes = 0;
	guchar *cImagem = NULL, *cPixels = NULL;
	gboolean bOK = FALSE;
	struct
	{
		gchar sNome[BESQL_BIN_COMPRIMENTO_NOME];
		glong lBytes;//comprimento em bytes
		gchar cTipo;//tipo de arquivo
		GdkColorspace nSeparacao;
		guint nCanal;//canais de cores (2=Cinza, 3=RGB, 4=YCbCr ou YUV, 5=CMYK, 6=YCbCrK)
		gboolean bAlfa;//tem canal transparenfia
		guint nBits;//bits por cores (normalmente 8)
		gint nLinhas;
		gint nLargura;//altura em pixels (em bits)
		gint nAltura;//largura em pixels (em bits)
	} Bin;

	if (be_sql_campo_binario (Campo) && Imagem && sNome)
	{
		//comprimento
		lBytes = BE_pixbuf_get_len (Imagem);
		if (lBytes > 0)
		{
			//comprimir imagem
			//cPixels = be_pixbuf_get_pixels (Imagem);
			cPixels = gdk_pixbuf_get_pixels (Imagem);
			cImagem = be_comprimir_nivel (cPixels, &lBytes, 9);
			if (cImagem)
			{
				//valores
				g_strlcpy (Bin.sNome, sNome, strlen (sNome) > (BESQL_BIN_COMPRIMENTO_NOME - 1) ? BESQL_BIN_COMPRIMENTO_NOME - 1 : strlen (sNome) + 1);
				Bin.sNome[BESQL_BIN_COMPRIMENTO_NOME - 1] = '\0';
				Bin.lBytes = lBytes;
				Bin.cTipo = 'I';//imagem
				Bin.nSeparacao = gdk_pixbuf_get_colorspace (Imagem);
				Bin.nCanal = gdk_pixbuf_get_n_channels (Imagem);
				Bin.bAlfa = gdk_pixbuf_get_has_alpha (Imagem);
				Bin.nBits = gdk_pixbuf_get_bits_per_sample (Imagem);
				Bin.nLinhas = gdk_pixbuf_get_rowstride (Imagem);
				Bin.nLargura = gdk_pixbuf_get_width (Imagem);
				Bin.nAltura = gdk_pixbuf_get_height (Imagem);
				//reservando memória
				Campo->nComprimentoNovo = (sizeof (Bin) + lBytes * sizeof (guchar));
				Campo->sValorNovo = (gchar *) g_malloc (Campo->nComprimentoNovo);
				if (Campo->sValorNovo)
				{
					//copiar valores
					memcpy (Campo->sValorNovo, &Bin, sizeof (Bin));
					memcpy (Campo->sValorNovo + sizeof (Bin), cImagem, lBytes);
					//marcar para alterar
					Campo->bAfetar = TRUE;

					bOK = TRUE;
				}
			}
		}
	}
	BE_free (cImagem);
	//BE_free (cPixels);//não limpar, o BE_free_pixbuf é quem limpa

	return bOK;
}

GdkPixbuf *be_sql_recuperar_imagem (const BESQL_Campo *Campo)
{
	GdkPixbuf *Imagem = NULL;
	guchar *cImagem = NULL;
	struct
	{
		gchar sNome[BESQL_BIN_COMPRIMENTO_NOME];
		glong lBytes;//comprimento em bytes
		gchar cTipo;//tipo de arquivo
		GdkColorspace nSeparacao;
		guint nCanal;//canais de cores (2=Cinza, 3=RGB, 4=YCbCr ou YUV, 5=CMYK, 6=YCbCrK)
		gboolean bAlfa;//tem canal transparenfia
		guint nBits;//bits por cores (normalmente 8)
		gint nLinhas;
		gint nLargura;//altura em pixels (em bits)
		gint nAltura;//largura em pixels (em bits)
	} Bin;

	if (Campo && Campo->sValor && be_sql_campo_binario (Campo))
	{
		//tipo imagem
		if (Campo->sValor[sizeof (Bin.sNome) + sizeof (Bin.lBytes)] == 'I')
		{
			//carregar estrutura
			memcpy (&Bin, Campo->sValor, sizeof (Bin));
			//decomprimindo imagem
			if (Bin.lBytes > 0)
				cImagem = be_comprimir_de (Campo->sValor + sizeof (Bin), &Bin.lBytes);
			//criar pixbuf
			if (cImagem)
			{
				Imagem = gdk_pixbuf_new_from_data (cImagem, Bin.nSeparacao, Bin.bAlfa, Bin.nBits, Bin.nLargura, Bin.nAltura, Bin.nLinhas, NULL, NULL);
			}
		}
	}
	//BE_free (cImagem);//não limpar, o BE_free_pixbuf é quem limpa

	return Imagem;
}*/

gboolean be_sql_campo_texto (BESQL_Campo *Campo)
{
	if (!Campo)
		return FALSE;
	if (Campo->nTipo == MYSQL_TYPE_STRING || Campo->nTipo == MYSQL_TYPE_VAR_STRING || Campo->nTipo == BESQL_TYPE_TINY_TEXT || Campo->nTipo == BESQL_TYPE_MEDIUM_TEXT || Campo->nTipo == BESQL_TYPE_LONG_TEXT || Campo->nTipo == BESQL_TYPE_TEXT || Campo->nTipo == BESQL_TIPO_SEL)
		return TRUE;
	else
		return FALSE;
}

gboolean be_sql_campo_binario (BESQL_Campo *Campo)
{
	if (!Campo)
		return FALSE;
	if (Campo->nTipo == MYSQL_TYPE_BLOB || Campo->nTipo == MYSQL_TYPE_TINY_BLOB || Campo->nTipo == MYSQL_TYPE_MEDIUM_BLOB || Campo->nTipo == MYSQL_TYPE_LONG_BLOB)
		return TRUE;
	else
		return FALSE;
}

gboolean be_sql_campo_numerico (BESQL_Campo *Campo)
{
	if (!Campo)
		return FALSE;
	if (Campo->nTipo == MYSQL_TYPE_TINY || Campo->nTipo == MYSQL_TYPE_SHORT || Campo->nTipo == MYSQL_TYPE_INT24 || Campo->nTipo == MYSQL_TYPE_LONGLONG || Campo->nTipo == MYSQL_TYPE_LONG || Campo->nTipo == MYSQL_TYPE_LONG || Campo->nTipo == BESQL_TIPO_NUM || Campo->nTipo == MYSQL_TYPE_FLOAT || Campo->nTipo == MYSQL_TYPE_DOUBLE || Campo->nTipo == BESQL_TIPO_MOE || Campo->nTipo == MYSQL_TYPE_DECIMAL)
		return TRUE;
	else
		return FALSE;
}

gboolean be_sql_campo_logico (BESQL_Campo *Campo)
{
	if (!Campo)
		return FALSE;
	if (Campo->nTipo == BESQL_TIPO_BOL || Campo->nTipo == MYSQL_TYPE_BIT)
		return TRUE;
	else
		return FALSE;
}

GSList *be_sql_campo_duplicar_lista (GSList *Campos, BESQL_Sintaxe *Sintaxe)
{
    BESQL_Campo *CampoO = NULL, *CampoD = NULL;
    GSList *CamposD = NULL, *ListaL = NULL;

	//campos
	for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		CampoO = (BESQL_Campo *) ListaL->data;
		CampoD = g_new (BESQL_Campo, 1);
        //copiar tudo
		CampoD->Sintaxe = Sintaxe;
		CampoD->sNome = g_strdup (CampoO->sNome);
		CampoD->nCampoPos = CampoO->nCampoPos;
		CampoD->sValor = NULL;
		CampoD->nComprimento = CampoO->nComprimento;
		CampoD->nComprimentoMax = CampoO->nComprimentoMax;
		CampoD->nComprimentoTop = CampoO->nComprimentoTop;
		CampoD->bAnulavel = CampoO->bAnulavel;
		CampoD->bAtivo = CampoO->bAtivo;
		CampoD->bAlteravel = CampoO->bAlteravel;
		CampoD->bAutoID = CampoO->bAutoID;
		CampoD->sTipo = CampoO->sTipo ? g_strdup (CampoO->sTipo) : NULL;
		CampoD->nTipo = CampoO->nTipo;
		CampoD->sParametros = CampoO->sParametros ? g_strdup (CampoO->sParametros) : NULL;
		CampoD->sGrupo = CampoO->sGrupo ? g_strdup (CampoO->sGrupo) : NULL;
		CampoD->sComentarios = CampoO->sComentarios ? g_strdup (CampoO->sComentarios) : NULL;
		CampoD->sValorPadrao = CampoO->sValorPadrao ? g_strdup (CampoO->sValorPadrao) : NULL;
		CampoD->sTotal = NULL;
		CampoD->sValorNovo = NULL;
		CampoD->nComprimentoNovo = 0;
		CampoD->bAfetar = FALSE;
		//anexar na lista
		CamposD = g_slist_append (CamposD, CampoD);
	}

	return CamposD;
}

GSList *be_sql_indice_duplicar_lista (GSList *Indices, BESQL_Sintaxe *Sintaxe, GSList *Campos)
{
    BESQL_Indice *IndiceO = NULL, *IndiceD = NULL;
    BESQL_Campo *CampoO = NULL;
    GSList *IndicesD = NULL, *ListaL = NULL, *ListaL1 = NULL;

	//indices
	for (ListaL = Indices; ListaL; ListaL = g_slist_next (ListaL))
	{
		IndiceO = (BESQL_Indice *) ListaL->data;
		IndiceD = g_new (BESQL_Indice, 1);
        //copiar tudo
		IndiceD->Sintaxe = Sintaxe;
		IndiceD->sNome = g_strdup (IndiceO->sNome);
		IndiceD->nTipo = IndiceO->nTipo;
		IndiceD->nSequencia = IndiceO->nSequencia;
        IndiceD->Campo = NULL;
        for (ListaL1 = Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
        {
            CampoO = (BESQL_Campo *) ListaL1->data;
            if (g_str_equal (IndiceO->Campo->sNome, CampoO->sNome))
                IndiceD->Campo = CampoO;
        }
		IndiceD->sValorFiltro = NULL;
		IndiceD->nComprimentoFiltro = 0;
		IndiceD->bAfetar = FALSE;
		//anexar na lista
		IndicesD = g_slist_append (IndicesD, IndiceD);
	}

	return IndicesD;
}

BESQL_Sintaxe *be_sql_abrir_consulta (BESQL_Tabela *Tabela, gboolean bForcarNulos, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_BD *BD = NULL;
	BESQL_Sintaxe *Sintaxe = NULL, *SintaxeCache = NULL;
	BESQL_Tabela *Tabela1 = NULL;
	GSList *ListaL = NULL;
	gint nCont = 0, nTempo = G_MAXINT;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

	if (!Tabela)
		return NULL;

	//comunicação
	BD = Tabela->BD;
	SQL = BD->SQL;

	//criar estrutura
	Sintaxe = g_new (BESQL_Sintaxe, 1);
	Sintaxe->Tabela = Tabela;
	Sintaxe->vis_Tabela = NULL;//view principal
	Sintaxe->blo_Tabela = NULL;//view dos dados bloqueados
	Sintaxe->adi_Tabela = NULL;//view para adição de dados
	Sintaxe->alt_Tabela = NULL;//view para alteração de dados
	Sintaxe->mex_Tabela = NULL;//view para marcar para excluir
	Sintaxe->exc_Tabela = NULL;//view para exclui
	Sintaxe->res = NULL;
	Sintaxe->sConsulta = NULL;
	Sintaxe->Campos = NULL;
	Sintaxe->Indices = NULL;
	Sintaxe->Filtros = NULL;
	Sintaxe->FiltrosUniao = NULL;
	Sintaxe->Ordens = NULL;
	Sintaxe->lLinhasPaginaMax = BESQL_MAX_LINHAS_POR_PAGINA;
	Sintaxe->lLinhasPagina = 0;
	Sintaxe->lLinhas = -1;//desconhecido
	Sintaxe->lLinha = 0;
	Sintaxe->bForcarNulos = bForcarNulos;//converter comprimento zero em nulos

	//verificar se não existe cache da Sintaxe
	if (!Tabela->SintaxeCache)
		Tabela->SintaxeCache = be_sql_abrir_consulta_cache (Tabela, sErros);
	//preparar
	SintaxeCache = (BESQL_Sintaxe *) Tabela->SintaxeCache;
	Tabela->nCopiasSintaxeCache++;//incrementar contagem de utilização da cópia
	Tabela->nTempoCache = 0;
	//copiar para Sintaxe
	Sintaxe->Campos = be_sql_campo_duplicar_lista (SintaxeCache->Campos, Sintaxe);
	Sintaxe->Indices = be_sql_indice_duplicar_lista (SintaxeCache->Indices, Sintaxe, Sintaxe->Campos);
	Sintaxe->vis_Tabela = SintaxeCache->vis_Tabela;
	Sintaxe->blo_Tabela = SintaxeCache->blo_Tabela;
	Sintaxe->adi_Tabela = SintaxeCache->adi_Tabela;
	Sintaxe->alt_Tabela = SintaxeCache->alt_Tabela;
	Sintaxe->mex_Tabela = SintaxeCache->mex_Tabela;
	Sintaxe->exc_Tabela = SintaxeCache->exc_Tabela;

	//limpar o cache, modo inteligente
	//localizar o mais antigo não utilizado, loop das tabelas
	if (SQL->nCacheMax >= 0)
	{
		for (ListaL = BD->Tabelas, nCont = 0; ListaL; ListaL = g_slist_next (ListaL))
		{
			Tabela1 = (BESQL_Tabela *) ListaL->data;
			if (Tabela1->SintaxeCache)
			{
				if (Tabela1->nCopiasSintaxeCache <= 0 && Tabela1->nTempoCache < nTempo)
					nTempo = Tabela1->nTempoCache;
				nCont++;//contar a quantidade carregada
			}
		}
		//limpar o mais antigo, loop das tabelas
		if (nCont > SQL->nCacheMax)//limpar somente se atingir o limite
		{
			for (ListaL = BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (Tabela1->SintaxeCache && Tabela1->nCopiasSintaxeCache <= 0 && Tabela1->nTempoCache == nTempo)
				{
					BE_sql_fechar_consulta (Tabela1->SintaxeCache);
					Tabela1->nCopiasSintaxeCache = 0;
					Tabela1->nTempoCache = 0;
				}
			}
		}
	}

	return Sintaxe;
}

void be_sql_abrir_consulta_cache_todas (BESQL_BD *BD, gchar **sErros)
{
	BESQL_Tabela *Tabela = NULL;
	GSList *ListaL = NULL;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;

	//loop nas tabelas
	for (ListaL = BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
	{
		Tabela = (BESQL_Tabela *) ListaL->data;
		if (!Tabela->SintaxeCache)
		{
			//carregar
			Tabela->SintaxeCache = be_sql_abrir_consulta_cache (Tabela, sErros);
			//preparar
			Tabela->nCopiasSintaxeCache = 0;
			Tabela->nTempoCache = 0;
		}
	}
}

BESQL_Sintaxe *be_sql_abrir_consulta_cache (BESQL_Tabela *Tabela, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Tabela *Tabela1 = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	MYSQL_ROW row = NULL;//linhas
	GSList *ListaL = NULL;
	gchar **aLista = NULL;
	gchar *sSQL = NULL, *sHash = NULL;
	gint nCont = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

	//verificar tabela
	if (!Tabela)
		return NULL;
	//comunicação
	SQL = Tabela->BD->SQL;

	//obter hash da tabela/visão
	BE_free (sSQL);
	sSQL = g_strdup_printf ("SELECT MD5(Data) AS Hash FROM %s.`cache_tabelas` WHERE Tabela = '%s';", Tabela->BD->sNome, Tabela->sNome);
	BE_free (sHash);
	sHash = g_strdup ("Hash");
	be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sHash, sErros);
	if (sErros && *sErros)
	{
		BE_free (*sErros);
		BE_free (sHash);
		//segunda tentativa, principalmente para root e adm
		BE_free (sSQL);
		sSQL = g_strdup_printf ("SELECT MD5(CREATE_TIME) AS Hash FROM information_schema.`TABLES` WHERE TABLE_SCHEMA = '%s' AND TABLE_NAME= '%s';", Tabela->BD->sNome, Tabela->sNome);
		BE_free (sHash);
		sHash = g_strdup ("Hash");
		be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sHash, sErros);//se houver erros, ignorar
		if (*sErros)
		{
			BE_free (*sErros);
			BE_free (sHash);
		}
	}

	//carregar informações em cache local ou do servidor
	if (!(Sintaxe = _be_sql_carregar_cache_local_sintaxe (Tabela, sHash)))
	{
		//criar estrutura
		Sintaxe = g_new (BESQL_Sintaxe, 1);
		Sintaxe->Tabela = Tabela;
		Sintaxe->vis_Tabela = NULL;//view principal
		Sintaxe->blo_Tabela = NULL;//view dos dados bloqueados
		Sintaxe->adi_Tabela = NULL;//view para adição de dados
		Sintaxe->alt_Tabela = NULL;//view para alteração de dados
		Sintaxe->mex_Tabela = NULL;//view para marcar para excluir
		Sintaxe->exc_Tabela = NULL;//view para exclui
		Sintaxe->res = NULL;
		Sintaxe->sConsulta = NULL;
		Sintaxe->Campos = NULL;
		Sintaxe->Indices = NULL;
		Sintaxe->Filtros = NULL;
		Sintaxe->FiltrosUniao = NULL;
		Sintaxe->Ordens = NULL;
		Sintaxe->lLinhasPaginaMax = BESQL_MAX_LINHAS_POR_PAGINA;
		Sintaxe->lLinhasPagina = 0;
		Sintaxe->lLinhas = -1;//desconhecido
		Sintaxe->lLinha = 0;
		Sintaxe->bForcarNulos = FALSE;//converter comprimento zero em nulos

		//carregar todos os campos disponíveis
		BE_free (sSQL);
		sSQL = g_strdup_printf ("SHOW COLUMNS FROM `%s`.`%s`", Tabela->BD->sNome, Tabela->sNome);//EXPLAIN `coluna`
		_be_sql_saida (sSQL);//depuração
		if (mysql_query (SQL->_my, sSQL) == 0)
		{
			if ((Sintaxe->res = mysql_store_result (SQL->_my)))//recebe a consulta
			{
				for (nCont = 0; nCont < mysql_num_rows (Sintaxe->res); nCont++)
				{
					//criando campo
					row = mysql_fetch_row (Sintaxe->res);
					Campo = g_new (BESQL_Campo, 1);
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
					if (row[5] && g_str_equal (row[5], BESQL_AUTO_ID))
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
						Campo->nTipo = BESQL_TYPE_TINY_TEXT;
					else if (g_strrstr (Campo->sTipo, "mediumtext"))
						Campo->nTipo = BESQL_TYPE_MEDIUM_TEXT;
					else if (g_strrstr (Campo->sTipo, "longtext"))
						Campo->nTipo = BESQL_TYPE_LONG_TEXT;
					else if (g_strrstr (Campo->sTipo, "text"))
						Campo->nTipo = BESQL_TYPE_TEXT;
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
				_be_sql_erro_retorno (SQL, sErros);
			}
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (SQL, sErros, sSQL);
		}

		//mascarar valor padrão
		_be_sql_mascarar_valor_padrao (Sintaxe, Tabela, sErros);

		//index para CIA
		if (Tabela->bCIA)
		{
			for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				if (Campo)
				{
					BE_free (sSQL);
					sSQL = g_strdup_printf ("SELECT `%s`.INDICE('vis_%s', '%s')", Tabela->BD->sNome, Tabela->sNome+4, Campo->sNome);
					_be_sql_saida (sSQL);//depuração
					//excecutar consulta
					if (mysql_query (SQL->_my, sSQL) == 0)
					{
						if ((Sintaxe->res = mysql_store_result (SQL->_my)))//recebe a consulta
						{
							if (mysql_num_rows (Sintaxe->res) > 0)
							{
								//recuperar campos
								row = mysql_fetch_row (Sintaxe->res);
								//lista de parâmetros
								if (row[0] && strlen (row[0]) > 0)
								{
									aLista = g_strsplit (row[0], "; ", -1);
									//criar novo indice
									Indice = g_new (BESQL_Indice, 1);
									Indice->Sintaxe = NULL;
									Indice->sNome = g_strdup (aLista[0]);
									Indice->nSequencia = (gint) atol (aLista[2]);
									Indice->sValorFiltro = NULL;
									Indice->nComprimentoFiltro = 0;
									Indice->bAfetar = FALSE;
									//relacionar com campo
									Indice->Campo = Campo;
									//verificar tipo
									if (g_str_equal (Indice->sNome, "PRIMARY"))
										Indice->nTipo = BESQL_INDICE_PRIMARIA;
									else if (atol (aLista[1]) == 0)
										Indice->nTipo = BESQL_INDICE_UNICA;
									else
										Indice->nTipo = BESQL_INDICE_SIMPLES;
									//anexando na lista
									Sintaxe->Indices = g_slist_append (Sintaxe->Indices, Indice);
									BE_strfreev (aLista);
								}
							}
							mysql_free_result (Sintaxe->res);
						}
						else//erro no retorno
						{
							_be_sql_erro_retorno (SQL, sErros);
						}
					}
					else//erro na consulta
					{
						_be_sql_erro_consulta (SQL, sErros, sSQL);
					}
				}
			}
		}
		//carregar todos os índices disponíveis
		else
		{
			BE_free (sSQL);
			sSQL = g_strdup_printf ("SHOW INDEX FROM `%s`.`%s`", Tabela->BD->sNome, Tabela->sNome);
			_be_sql_saida (sSQL);//depuração
			if (mysql_query (SQL->_my, sSQL) == 0)
			{
				if ((Sintaxe->res = mysql_store_result (SQL->_my)))//recebe a consulta
				{
					for (nCont = 0; nCont < mysql_num_rows (Sintaxe->res); nCont++)
					{
						//recuperar campos
						row = mysql_fetch_row (Sintaxe->res);
						//criar novo indice
						Indice = g_new (BESQL_Indice, 1);
						Indice->Sintaxe = NULL;
						Indice->sNome = g_strdup (row[2]);
						Indice->nSequencia = (gint) atol (row[3]);
						Indice->sValorFiltro = NULL;
						Indice->nComprimentoFiltro = 0;
						Indice->bAfetar = FALSE;
						//relacionar com campo
						Indice->Campo = be_sql_selecionar_campo (Sintaxe, row[4]);
						//verificar tipo
						if (g_str_equal (Indice->sNome, "PRIMARY"))
							Indice->nTipo = BESQL_INDICE_PRIMARIA;
						else if (atol (row[1]) == 0)
							Indice->nTipo = BESQL_INDICE_UNICA;
						else
							Indice->nTipo = BESQL_INDICE_SIMPLES;
						//anexando na lista
						Sintaxe->Indices = g_slist_append (Sintaxe->Indices, Indice);
					}
					mysql_free_result (Sintaxe->res);
				}
				else//erro no retorno
				{
					_be_sql_erro_retorno (SQL, sErros);
				}
			}
			else//erro na consulta
			{
				_be_sql_erro_consulta (SQL, sErros, sSQL);
			}
		}

		//verificar permissões por campo
		for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			//excutar consulta de verificação
			if (SQL->bSuper)
				Campo->bAlteravel = TRUE;
			else
			{
				BE_free (sSQL);
				sSQL = g_strdup_printf ("SELECT `PRIVILEGE_TYPE` FROM `information_schema`.`COLUMN_PRIVILEGES` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='%s' AND COLUMN_NAME='%s'", Tabela->BD->sNome, Tabela->sNome, Campo->sNome);
				_be_sql_saida (sSQL);//depuração
				if (mysql_query (SQL->_my, sSQL) == 0)
				{
					if ((Sintaxe->res = mysql_store_result (SQL->_my)))//recebe a consulta
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
							Campo->bAlteravel = (Tabela->nFlags & BESQL_ACESSO_ALTERAR) == BESQL_ACESSO_ALTERAR;
					}
					else//erro no retorno
					{
						_be_sql_erro_retorno (SQL, sErros);
					}
				}
				else//erro na consulta
				{
					_be_sql_erro_consulta (SQL, sErros, sSQL);
				}
			}
		}
		BE_free (sSQL);

		//verificar CIA, relações entre tabelas, mas não se o acesso for diretamente em cli_Tabela (somente root)
		if (strlen (Tabela->sNome) > 4 && Tabela->sNome[3] == '_' && !g_strrstr_len (Tabela->sNome, 4, "cli_"))
		{
			//procurar por vis_Tabela
			BE_free (sSQL);
			sSQL = g_strdup_printf ("vis_%s", Tabela->sNome+4);
			for (ListaL = Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (g_str_equal (Tabela1->sNome, sSQL))
				{
					Sintaxe->vis_Tabela = Tabela1;
					Tabela1->bCIA = TRUE;
					break;
				}
			}
			//procurar por blo_Tabela
			BE_free (sSQL);
			sSQL = g_strdup_printf ("blo_%s", Tabela->sNome+4);
			for (ListaL = Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (g_str_equal (Tabela1->sNome, sSQL))
				{
					Sintaxe->blo_Tabela = Tabela1;
					Tabela1->bCIA = TRUE;
					break;
				}
			}
			//procurar por adi_Tabela
			BE_free (sSQL);
			sSQL = g_strdup_printf ("adi_%s", Tabela->sNome+4);
			for (ListaL = Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (g_str_equal (Tabela1->sNome, sSQL))
				{
					Sintaxe->adi_Tabela = Tabela1;
					Tabela1->bCIA = TRUE;
					break;
				}
			}
			//procurar por alt_Tabela
			BE_free (sSQL);
			sSQL = g_strdup_printf ("alt_%s", Tabela->sNome+4);
			for (ListaL = Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (g_str_equal (Tabela1->sNome, sSQL))
				{
					Sintaxe->alt_Tabela = Tabela1;
					Tabela1->bCIA = TRUE;
					break;
				}
			}
			//procurar por mex_Tabela
			BE_free (sSQL);
			sSQL = g_strdup_printf ("mex_%s", Tabela->sNome+4);
			for (ListaL = Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (g_str_equal (Tabela1->sNome, sSQL))
				{
					Sintaxe->mex_Tabela = Tabela1;
					Tabela1->bCIA = TRUE;
					break;
				}
			}
			//procurar por exc_Tabela
			BE_free (sSQL);
			sSQL = g_strdup_printf ("exc_%s", Tabela->sNome+4);
			for (ListaL = Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
			{
				Tabela1 = (BESQL_Tabela *) ListaL->data;
				if (g_str_equal (Tabela1->sNome, sSQL))
				{
					Sintaxe->exc_Tabela = Tabela1;
					Tabela1->bCIA = TRUE;
					break;
				}
			}
		}

		//salvar em disco toda estrutura de Sintaxe, e abaixo dela
		_be_sql_gravar_cache_local_sintaxe (Sintaxe, sHash);
	}

	//limpar
	BE_free (sSQL);
	BE_free (sHash);

	return Sintaxe;
}

BESQL_Sintaxe *_be_sql_carregar_cache_local_sintaxe (BESQL_Tabela *Tabela, gchar *sHashMD5)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Tabela *Tabela1 = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL;
	FILE *File = NULL;
	gchar *sArquivo = NULL, *sHashMD5comp = NULL;
	gchar *sTemp = NULL;
	guchar cIndicador;
	glong lComprimento = 0, lQuantidade = 0;
	gint nCont = 0, nCont1 = 0;

	//verificar existência de hash
	if (!sHashMD5 || strlen (sHashMD5) != 32)
		return NULL;

	//preparar local
	BE_free (sTemp);
	sTemp = be_diretorio_local (TRUE);
	//nome do arquivo
	sArquivo = g_strdup_printf ("%scache_%s_%s_sintaxe.dat", sTemp, Tabela->BD->sNome, Tabela->sNome);

	//abrindo arquivo
	if ((File = fopen (sArquivo, "rb")))
	{
		//obter hash MD5, comprimento fixo padrao 32
		sHashMD5comp = (gchar *) g_malloc (33); //alocar memória
		if (fread (sHashMD5comp, 32, 1, File) > 0)
		{
			//comparar hashs
			sHashMD5comp[32] = '\0';
			if (g_strstr_len (sHashMD5comp, 32, sHashMD5))
			{
				//obter sintaxe
				fread (&cIndicador, sizeof (cIndicador), 1, File);//indicador
				if (cIndicador == 's')
				{
					fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					//verificar comprimento da estrutura
					if (lComprimento == sizeof (BESQL_Sintaxe))
					{
						//obter sintaxe
						Sintaxe = g_new (BESQL_Sintaxe, 1);
						fread (Sintaxe, sizeof (BESQL_Sintaxe), 1, File);//dados
						Sintaxe->Tabela = Tabela;//integrando nova sintaxe a estrutura SQL
						Sintaxe->Filtros = NULL;
						Sintaxe->FiltrosUniao = NULL;
						Sintaxe->Ordens = NULL;
						//obter vis
						if (Sintaxe->vis_Tabela)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							BE_free (sTemp);
							sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (sTemp, lComprimento, 1, File);//dados
							sTemp[lComprimento] = '\0';//fim de string
							//localizar endereço de vis
							if (sTemp)
							{
								for (ListaL = Sintaxe->Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
								{
									Tabela1 = (BESQL_Tabela *) ListaL->data;
									if (g_str_equal (Tabela1->sNome, sTemp))
									{
										Sintaxe->vis_Tabela = Tabela1;
										Tabela1->bCIA = TRUE;
										break;
									}
								}
							}
						}
						//obter blo
						if (Sintaxe->blo_Tabela)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							BE_free (sTemp);
							sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (sTemp, lComprimento, 1, File);//dados
							sTemp[lComprimento] = '\0';//fim de string
							//localizar endereço de vis
							if (sTemp)
							{
								for (ListaL = Sintaxe->Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
								{
									Tabela1 = (BESQL_Tabela *) ListaL->data;
									if (g_str_equal (Tabela1->sNome, sTemp))
									{
										Sintaxe->blo_Tabela = Tabela1;
										Tabela1->bCIA = TRUE;
										break;
									}
								}
							}
						}
						//obter adi
						if (Sintaxe->adi_Tabela)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							BE_free (sTemp);
							sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (sTemp, lComprimento, 1, File);//dados
							sTemp[lComprimento] = '\0';//fim de string
							//localizar endereço de vis
							if (sTemp)
							{
								for (ListaL = Sintaxe->Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
								{
									Tabela1 = (BESQL_Tabela *) ListaL->data;
									if (g_str_equal (Tabela1->sNome, sTemp))
									{
										Sintaxe->adi_Tabela = Tabela1;
										Tabela1->bCIA = TRUE;
										break;
									}
								}
							}
						}
						//obter alt
						if (Sintaxe->alt_Tabela)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							BE_free (sTemp);
							sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (sTemp, lComprimento, 1, File);//dados
							sTemp[lComprimento] = '\0';//fim de string
							//localizar endereço de vis
							if (sTemp)
							{
								for (ListaL = Sintaxe->Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
								{
									Tabela1 = (BESQL_Tabela *) ListaL->data;
									if (g_str_equal (Tabela1->sNome, sTemp))
									{
										Sintaxe->alt_Tabela = Tabela1;
										Tabela1->bCIA = TRUE;
										break;
									}
								}
							}
						}
						//obter mex
						if (Sintaxe->mex_Tabela)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							BE_free (sTemp);
							sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (sTemp, lComprimento, 1, File);//dados
							sTemp[lComprimento] = '\0';//fim de string
							//localizar endereço de vis
							if (sTemp)
							{
								for (ListaL = Sintaxe->Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
								{
									Tabela1 = (BESQL_Tabela *) ListaL->data;
									if (g_str_equal (Tabela1->sNome, sTemp))
									{
										Sintaxe->mex_Tabela = Tabela1;
										Tabela1->bCIA = TRUE;
										break;
									}
								}
							}
						}
						//obter exc
						if (Sintaxe->exc_Tabela)
						{
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							BE_free (sTemp);
							sTemp = (gchar *) g_malloc (lComprimento+1);//alocar memória
							fread (sTemp, lComprimento, 1, File);//dados
							sTemp[lComprimento] = '\0';//fim de string
							//localizar endereço de vis
							if (sTemp)
							{
								for (ListaL = Sintaxe->Tabela->BD->Tabelas; ListaL; ListaL = g_slist_next (ListaL))
								{
									Tabela1 = (BESQL_Tabela *) ListaL->data;
									if (g_str_equal (Tabela1->sNome, sTemp))
									{
										Sintaxe->exc_Tabela = Tabela1;
										Tabela1->bCIA = TRUE;
										break;
									}
								}
							}
						}
						//obter campos
						if (Sintaxe->Campos);
						{
							Sintaxe->Campos = NULL;//preparar lista
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							//verificar comprimento da estrutura
							if (lComprimento == sizeof (BESQL_Campo))
							{
								//obter quantidade
								fread (&lQuantidade, sizeof (lQuantidade), 1, File);//quantidade
								//loop nos campos
								for (nCont = 0; nCont < lQuantidade; nCont++)
								{
									//obter campo
									Campo = g_new (BESQL_Campo, 1);
									fread (Campo, sizeof (BESQL_Campo), 1, File);//dados
									Campo->Sintaxe = Sintaxe;//relacionar campo
									Sintaxe->Campos = g_slist_append (Sintaxe->Campos, Campo);//anexando na lista
									//obter nome, verificar lixo
									if (Campo->sNome)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Campo->sNome = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Campo->sNome, lComprimento, 1, File);//dados
										Campo->sNome[lComprimento] = '\0';//fim de string
									}
									//obter tipo, verificar lixo
									if (Campo->sTipo)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Campo->sTipo = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Campo->sTipo, lComprimento, 1, File);//dados
										Campo->sTipo[lComprimento] = '\0';//fim de string
									}
									//obter parametros, verificar lixo
									if (Campo->sParametros)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Campo->sParametros = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Campo->sParametros, lComprimento, 1, File);//dados
										Campo->sParametros[lComprimento] = '\0';//fim de string
									}
									//obter grupo, verificar lixo
									if (Campo->sGrupo)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Campo->sGrupo = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Campo->sGrupo, lComprimento, 1, File);//dados
										Campo->sGrupo[lComprimento] = '\0';//fim de string
									}
									//obter comentarios, verificar lixo
									if (Campo->sComentarios)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Campo->sComentarios = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Campo->sComentarios, lComprimento, 1, File);//dados
										Campo->sComentarios[lComprimento] = '\0';//fim de string
									}
									//obter valor padrao, verificar lixo
									if (Campo->sValorPadrao)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Campo->sValorPadrao = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Campo->sValorPadrao, lComprimento, 1, File);//dados
										Campo->sValorPadrao[lComprimento] = '\0';//fim de string
									}

								}
							}

						}
						//obter indices
						if (Sintaxe->Indices)
						{
							Sintaxe->Indices = NULL;//preparar lista
							fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
							//verificar comprimento da estrutura
							if (lComprimento == sizeof (BESQL_Indice))
							{
								//obter quantidade
								fread (&lQuantidade, sizeof (lQuantidade), 1, File);//comprimento
								//loop nos indices
								for (nCont = 0; nCont < lQuantidade; nCont++)
								{
									//obter campo
									Indice = g_new (BESQL_Indice, 1);
									fread (Indice, sizeof (BESQL_Indice), 1, File);//dados
									Indice->Sintaxe = Sintaxe;//relacionar campo
									Sintaxe->Indices = g_slist_append (Sintaxe->Indices, Indice);//anexando na lista
									//obter nome, verificar lixo
									if (Indice->sNome)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Indice->sNome = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Indice->sNome, lComprimento, 1, File);//dados
										Indice->sNome[lComprimento] = '\0';//fim de string
									}
									//obter valor filtro, verificar lixo
									if (Indice->sValorFiltro)
									{
										fread (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
										Indice->sValorFiltro = (gchar *) g_malloc (lComprimento+1);//alocar memória
										fread (Indice->sValorFiltro, lComprimento, 1, File);//dados
										Indice->sValorFiltro[lComprimento] = '\0';//fim de string
									}
									//campo para indices
									if (Indice->Campo)
									{
										fread (&nCont, sizeof (nCont), 1, File);//numeração sequencial do campo
										//campo; loop nos campos
										for (ListaL = Sintaxe->Campos, nCont1 = 0; ListaL; ListaL = g_slist_next (ListaL), nCont1++)
										{
											if (nCont == nCont1)
												Indice->Campo = (BESQL_Campo *) ListaL->data;;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//fechar arquivo
		fclose (File);
	}
	//limpar
	BE_free (sArquivo);
	BE_free (sHashMD5comp);
	BE_free (sTemp);

	return Sintaxe;
}

void _be_sql_gravar_cache_local_sintaxe (BESQL_Sintaxe *Sintaxe, gchar *sHashMD5)
{
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;
	FILE *File = NULL;
	gchar *sArquivo = NULL;
	guchar cIndicador;
	gchar *sTemp = NULL;
	glong lComprimento = 0, lQuantidade = 0;
	gint nCont = 0;

	//verificar existência de hash
	if (!sHashMD5 || strlen (sHashMD5) != 32)
		return;

	//preparar local
	BE_free (sTemp);
	sTemp = be_diretorio_local (TRUE);
	//nome do arquivo
	sArquivo = g_strdup_printf ("%scache_%s_%s_sintaxe.dat", sTemp, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);

	//Abrindo arquivo:
	if ((File = fopen (sArquivo, "wb")))
	{
		//gravar hash MD5, comprimento fixo padrao 32
		fwrite (sHashMD5, 32, 1, File);//dados
		//gravar sintaxe
		cIndicador = 's';
		fwrite (&cIndicador, sizeof (cIndicador), 1, File);//indicador
		lComprimento = sizeof (BESQL_Sintaxe);
		fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
		fwrite (Sintaxe, lComprimento, 1, File);//dados
		//vis
		if (Sintaxe->vis_Tabela)
		{
			lComprimento = sizeof (gchar) * strlen (Sintaxe->vis_Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Sintaxe->vis_Tabela->sNome, lComprimento, 1, File);//dados
		}
		//blo
		if (Sintaxe->blo_Tabela)
		{
			lComprimento = sizeof (gchar) * strlen (Sintaxe->blo_Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Sintaxe->blo_Tabela->sNome, lComprimento, 1, File);//dados
		}
		//adi
		if (Sintaxe->adi_Tabela)
		{
			lComprimento = sizeof (gchar) * strlen (Sintaxe->adi_Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Sintaxe->adi_Tabela->sNome, lComprimento, 1, File);//dados
		}
		//alt
		if (Sintaxe->alt_Tabela)
		{
			lComprimento = sizeof (gchar) * strlen (Sintaxe->alt_Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Sintaxe->alt_Tabela->sNome, lComprimento, 1, File);//dados
		}
		//mex
		if (Sintaxe->mex_Tabela)
		{
			lComprimento = sizeof (gchar) * strlen (Sintaxe->mex_Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Sintaxe->mex_Tabela->sNome, lComprimento, 1, File);//dados
		}
		//exc
		if (Sintaxe->exc_Tabela)
		{
			lComprimento = sizeof (gchar) * strlen (Sintaxe->exc_Tabela->sNome);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			fwrite (Sintaxe->exc_Tabela->sNome, lComprimento, 1, File);//dados
		}
		//gravar campos
		if (Sintaxe->Campos)
		{
			lComprimento = sizeof (BESQL_Campo);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			lQuantidade = g_slist_length (Sintaxe->Campos);
			fwrite (&lQuantidade, sizeof (lQuantidade), 1, File);//quantidade de ítens
			for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				//estrutura
				Campo = (BESQL_Campo *) ListaL->data;
				fwrite (Campo, sizeof (BESQL_Campo), 1, File);//dados
				//nome
				if (Campo->sNome)
				{
					lComprimento = sizeof (gchar) * strlen (Campo->sNome);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Campo->sNome, lComprimento, 1, File);//dados
				}
				//tipo
				if (Campo->sTipo)
				{
					lComprimento = sizeof (gchar) * strlen (Campo->sTipo);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Campo->sTipo, lComprimento, 1, File);//dados
				}
				//parametros
				if (Campo->sParametros)
				{
					lComprimento = sizeof (gchar) * strlen (Campo->sParametros);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Campo->sParametros, lComprimento, 1, File);//dados
				}
				//grupo
				if (Campo->sGrupo)
				{
					lComprimento = sizeof (gchar) * strlen (Campo->sGrupo);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Campo->sGrupo, lComprimento, 1, File);//dados
				}
				//comentarios
				if (Campo->sComentarios)
				{
					lComprimento = sizeof (gchar) * strlen (Campo->sComentarios);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Campo->sComentarios, lComprimento, 1, File);//dados
				}
				//valor padrao
				if (Campo->sValorPadrao)
				{
					lComprimento = sizeof (gchar) * strlen (Campo->sValorPadrao);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Campo->sValorPadrao, lComprimento, 1, File);//dados
				}
			}
		}
		//gravar indices
		if (Sintaxe->Indices)
		{
			lComprimento = sizeof (BESQL_Indice);
			fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
			lQuantidade = g_slist_length (Sintaxe->Indices);
			fwrite (&lQuantidade, sizeof (lQuantidade), 1, File);//quantidade de ítens
			for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
			{
				//estrutura
				Indice = (BESQL_Indice *) ListaL->data;
				fwrite (Indice, sizeof (BESQL_Indice), 1, File);//dados
				//nome
				if (Indice->sNome)
				{
					lComprimento = sizeof (gchar) * strlen (Indice->sNome);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Indice->sNome, lComprimento, 1, File);//dados
				}
				//valor filtro
				if (Indice->sValorFiltro)
				{
					lComprimento = sizeof (gchar) * strlen (Indice->sValorFiltro);
					fwrite (&lComprimento, sizeof (lComprimento), 1, File);//comprimento
					fwrite (Indice->sValorFiltro, lComprimento, 1, File);//dados
				}
				//campo; loop nos campos
				if (Indice->Campo)
				{
					for (ListaL1 = Sintaxe->Campos, nCont = 0; ListaL1; ListaL1 = g_slist_next (ListaL1), nCont++)
					{
						Campo = (BESQL_Campo *) ListaL1->data;
						if (Indice->Campo == Campo)
							fwrite (&nCont, sizeof (nCont), 1, File);//numeração sequencial do campo
					}
				}
			}
		}
		//fechar arquivo
		fclose (File);
	}
	//limpar
	BE_free (sArquivo);
	BE_free (sTemp);
}

void _be_sql_mascarar_valor_padrao (BESQL_Sintaxe *Sintaxe, BESQL_Tabela *Tabela, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	GString *sSQL = NULL;
	gchar *sTemp= NULL;
	MYSQL_ROW row = NULL;//linhas
	gint nCont = 0;

	//verificar máscara
	if (!Tabela->BD->bMascara)
		return;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;

	//comunicação
	SQL = Tabela->BD->SQL;

	//campos da consulta SELECT
	sSQL = g_string_new ("SELECT ");
	//loop nos campos
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->sValorPadrao && strlen (Campo->sValorPadrao) > 0)
		{
			switch (Campo->nTipo)
			{
				case BESQL_TIPO_NUM:
				case MYSQL_TYPE_FLOAT:
				case MYSQL_TYPE_DOUBLE:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMNUM, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_MOE:
				case MYSQL_TYPE_DECIMAL:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMMOE, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_BOL:
				case MYSQL_TYPE_BIT:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMBOL, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_DAT:
				case MYSQL_TYPE_DATE:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMDAT, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_TEM:
				case MYSQL_TYPE_DATETIME:
				case MYSQL_TYPE_TIMESTAMP:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMTEM, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_FONx:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMFONx, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_FON:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMFON, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_CNPJ:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMCNPJ, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_CPF:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMCPF, Campo->sValorPadrao, Campo->sNome);
					break;
				case BESQL_TIPO_CEP:
					g_string_append_printf (sSQL, "`%s`.%s('%s') AS `%s`, ", Tabela->BD->sNome, BESQL_MASCARA_AMCEP, Campo->sValorPadrao, Campo->sNome);
					break;
				default:
					g_string_append_printf (sSQL, "'%s' AS `%s`, ", Campo->sValorPadrao, Campo->sNome);
					break;
			}
			nCont++;
		}
	}
	sSQL = g_string_truncate (sSQL, sSQL->len-2);//excluir a última vírgula e espaço
	//finalizar
	g_string_append_printf (sSQL, " ;");
	//guardando consulta
	BE_free (sTemp);
	sTemp = BE_string_free (sSQL);
	//executando nova consulta
	if (nCont > 0)
	{
        _be_sql_saida (sTemp);//depuração
		if (mysql_query (SQL->_my, sTemp) == 0)
		{
			if ((Sintaxe->res = mysql_store_result (SQL->_my)))//recebe a consulta
			{
				if (mysql_num_rows (Sintaxe->res) == 1)
				{
					//criando campo
					row = mysql_fetch_row (Sintaxe->res);
					//loop nos campos
					nCont = 0;
					for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
					{
						Campo = (BESQL_Campo *) ListaL->data;
						if (Campo->sValorPadrao && strlen (Campo->sValorPadrao) > 0)
						{
							BE_free (Campo->sValorPadrao);
							if (row[nCont])
								Campo->sValorPadrao = g_strdup (row[nCont]);
							nCont++;
						}
					}
				}
				mysql_free_result (Sintaxe->res);
			}
			else//erro no retorno
				_be_sql_erro_retorno (SQL, sErros);
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (SQL, sErros, sTemp);
		}
	}
	BE_free (sTemp);
}

void be_sql_ping (BESQL *SQL, gchar **sErros)
{
	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;

	//pag 1117
	if (mysql_ping (SQL->_my) != 0)
	{
		_be_sql_erro_consulta (SQL, sErros, NULL);//erro na consulta
	}
	//callback
	else if (SQL->func_Info)
		SQL->func_Info (SQL, 0, NULL, SQL->user_data);
}

void _be_sql_fechar_consulta (BESQL_Sintaxe *Sintaxe)
{
	BESQL_Tabela *Tabela = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Indice *Indice = NULL;
	BESQL_Filtro *Filtro = NULL;
	BESQL_Ordem *Ordem = NULL;
	GSList *ListaL = NULL;

	if (!Sintaxe)
		return;

	//decrementar referências
	Tabela = Sintaxe->Tabela;
	Tabela->nCopiasSintaxeCache--;
	Tabela->nTempoCache = be_tempo_sec ();//tempo do descarregamento

	BE_free (Sintaxe->sConsulta);
	if (Sintaxe->res)
		mysql_free_result (Sintaxe->res);
	//índices
	for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
	{
		Indice = (BESQL_Indice *) ListaL->data;
#ifdef BESQL_DEPURACAO
		printf ("DEPURACAO: %s:%d - \t\t\tINDEX: %s (%s) - %d %d\n", __FILE__, __LINE__, Indice->Campo->sNome, Indice->sNome, Indice->nTipo, Indice->nSequencia);
#endif
		BE_free (Indice->sNome);
		BE_free (Indice->sValorFiltro);
		BE_free (Indice);
	}
	BE_slist_free (Sintaxe->Indices);
	//campos
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
#ifdef BESQL_DEPURACAO
		printf ("DEPURACAO: %s:%d - \t\t%s %d(%s) %s %s %s %d = %s t:%s c:%d\n", __FILE__, __LINE__, Campo->sNome, Campo->nTipo, Campo->sTipo, Campo->sParametros, Campo->sGrupo, Campo->sComentarios, Campo->bAlteravel, Campo->sValorPadrao, Campo->sTotal, Campo->nComprimentoTop);
#endif
		BE_free (Campo->sNome);
		BE_free (Campo->sTipo);
		BE_free (Campo->sParametros);
		BE_free (Campo->sGrupo);
		BE_free (Campo->sComentarios);
		BE_free (Campo->sValorPadrao);
		BE_free (Campo->sValorNovo);
		BE_free (Campo->sTotal);
		BE_free (Campo);
	}
	BE_slist_free (Sintaxe->Campos);
	//filtros
	for (ListaL = Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
	{
		Filtro = (BESQL_Filtro *) ListaL->data;
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	BE_slist_free (Sintaxe->Filtros);
	//filtros união
	for (ListaL = Sintaxe->FiltrosUniao; ListaL; ListaL = g_slist_next (ListaL))
	{
		Filtro = (BESQL_Filtro *) ListaL->data;
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	BE_slist_free (Sintaxe->FiltrosUniao);
	//ordens
	for (ListaL = Sintaxe->Ordens; ListaL; ListaL = g_slist_next (ListaL))
	{
		Ordem = (BESQL_Ordem *) ListaL->data;
		BE_free (Ordem);
	}
	BE_slist_free (Sintaxe->Ordens);

	//tabelas relacionadas
#ifdef BESQL_DEPURACAO
	if (Sintaxe->vis_Tabela)
		printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, Sintaxe->vis_Tabela->sNome);
	if (Sintaxe->blo_Tabela)
		printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, Sintaxe->blo_Tabela->sNome);
	if (Sintaxe->adi_Tabela)
		printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, Sintaxe->adi_Tabela->sNome);
	if (Sintaxe->alt_Tabela)
		printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, Sintaxe->alt_Tabela->sNome);
	if (Sintaxe->mex_Tabela)
		printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, Sintaxe->mex_Tabela->sNome);
	if (Sintaxe->exc_Tabela)
		printf ("DEPURACAO: %s:%d - \t\t%s\n", __FILE__, __LINE__, Sintaxe->exc_Tabela->sNome);
#endif

	BE_free (Sintaxe);
}

void _be_sql_campo_CIA (BESQL_Campo *Campo, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_BD *BD = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gchar **aLista = NULL;
	gchar *sTemp = NULL;
	gboolean bContinuar = FALSE;
	gint nCont = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;

	//CIA
	if (!Campo->Sintaxe->Tabela->bCIA)
		return;
	//bd
	BD = Campo->Sintaxe->Tabela->BD;
	//comunicação
	SQL = BD->SQL;

	//localizar a existência de sis_campos
	sSQL = g_strdup_printf ("SELECT `PRIVILEGE_TYPE` FROM `information_schema`.`TABLE_PRIVILEGES` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='sis_campos'", Campo->Sintaxe->Tabela->BD->sNome);
	_be_sql_saida (sSQL);//depuração
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			for (nCont = 0; nCont < mysql_num_rows (res); nCont++)
			{
				//recuperar campo
				row = mysql_fetch_row (res);
				if (g_str_equal (row[0], "SELECT"))
                    bContinuar = TRUE;
			}
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	//continuar somente se sis_tabelas existir
	if (bContinuar)
	{
        //verificar acesso
        BE_free (sSQL);
		sSQL = g_strdup_printf ("SELECT `Tipo`, `Parametros`, `Grupo`, `Comentarios` FROM `%s`.`sis_campos` WHERE `Visao`='vis_%s' AND `Apelido`='%s'", Campo->Sintaxe->Tabela->BD->sNome, Campo->Sintaxe->Tabela->sNome+4, Campo->sNome);
        _be_sql_saida (sSQL);//depuração
        //excecutar consulta
        if (mysql_query (SQL->_my, sSQL) == 0)
        {
            if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
            {
                if (mysql_num_rows (res) > 0)
                {
                    //recuperar campos
                    row = mysql_fetch_row (res);
                    //informações extra
                    if (row[0])
                    {
						if (Campo->sTipo)
						{
							BE_free (sTemp);
							sTemp = Campo->sTipo;
							Campo->sTipo = g_strdup_printf ("%s %s", sTemp, row[0]);//concatenação
						}
						else
							Campo->sTipo = g_strdup (row[0]);
						//rever tipos especiais, do CIA
						if (g_strrstr (row[0], "SEL"))
							Campo->nTipo = BESQL_TIPO_SEL;
						else if (g_strrstr (row[0], "OPC"))
							Campo->nTipo = BESQL_TIPO_OPC;
						else if (g_strrstr (row[0], "HIE"))
							Campo->nTipo = BESQL_TIPO_HIE;
						else if (g_strrstr (row[0], "SUB"))
							Campo->nTipo = BESQL_TIPO_SUB;
						else if (g_strrstr (row[0], "EXE"))
							Campo->nTipo = BESQL_TIPO_EXE;
						else if (g_strrstr (row[0], "RTF"))
							Campo->nTipo = BESQL_TIPO_RTF;
						else if (g_strrstr (row[0], "DUP"))
							Campo->nTipo = BESQL_TIPO_DUP;
						else if (g_strrstr (row[0], "NUM"))
							Campo->nTipo = BESQL_TIPO_NUM;
						else if (g_strrstr (row[0], "MOE"))
							Campo->nTipo = BESQL_TIPO_MOE;
						else if (g_strrstr (row[0], "DAT"))
							Campo->nTipo = BESQL_TIPO_DAT;
						else if (g_strrstr (row[0], "TEM"))
							Campo->nTipo = BESQL_TIPO_TEM;
						else if (g_strrstr (row[0], "FONx"))
							Campo->nTipo = BESQL_TIPO_FONx;
						else if (g_strrstr (row[0], "FON"))
							Campo->nTipo = BESQL_TIPO_FON;
						else if (g_strrstr (row[0], "CNPJ"))
							Campo->nTipo = BESQL_TIPO_CNPJ;
						else if (g_strrstr (row[0], "CPF"))
							Campo->nTipo = BESQL_TIPO_CPF;
						else if (g_strrstr (row[0], "CEP"))
							Campo->nTipo = BESQL_TIPO_CEP;
						else if (g_strrstr (row[0], "BOL"))
							Campo->nTipo = BESQL_TIPO_BOL;
                    }
                    if (row[1])
						Campo->sParametros = g_strdup (row[1]);
					if (row[2])
						Campo->sGrupo = g_strdup (row[2]);
					if (row[3])
						Campo->sComentarios = g_strdup (row[3]);
                }
                mysql_free_result (res);
            }
            else//erro no retorno
                _be_sql_erro_retorno (SQL, sErros);
        }
        else//erro na consulta
        {
            _be_sql_erro_consulta (SQL, sErros, sSQL);
        }

        //valor padrão
        BE_free (sSQL);
		sSQL = g_strdup_printf ("SELECT `%s`.COLUNA('vis_%s', '%s')", Campo->Sintaxe->Tabela->BD->sNome, Campo->Sintaxe->Tabela->sNome+4, Campo->sNome);
        _be_sql_saida (sSQL);//depuração
        //excecutar consulta
        if (mysql_query (SQL->_my, sSQL) == 0)
        {
            if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
            {
                if (mysql_num_rows (res) > 0)
                {
                    //recuperar campos
                    row = mysql_fetch_row (res);
                    //lista de parâmetros
                    if (row[0] && strlen (row[0]) > 0)
                    {
                        aLista = g_strsplit (row[0], "; ", -1);
                        //verificar valor padrão
                        if (aLista[0])
                        {
                            BE_free (Campo->sValorPadrao);
                            Campo->sValorPadrao = g_strdup (aLista[0]);
                        }
                        //verificar auto_incremento
                        if (aLista[3] && g_str_equal (aLista[3], BESQL_AUTO_ID))
                            Campo->bAutoID = TRUE;
                        //limpar
                        BE_strfreev (aLista);
                    }
                }
                mysql_free_result (res);
            }
            else//erro no retorno
                _be_sql_erro_retorno (SQL, sErros);
        }
        else//erro na consulta
        {
            _be_sql_erro_consulta (SQL, sErros, sSQL);
        }
	}

	BE_free (sSQL);
	BE_free (sTemp);
}

gint be_sql_campo_quantidade_ativos (BESQL_Sintaxe *Sintaxe)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	gint nCont = 0;

	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)
			nCont++;
	}

	return nCont;
}

BESQL_Ordem *be_sql_ordem_adicionar (BESQL_Sintaxe *Sintaxe, BESQL_Campo *Campo, BESQL_ORDENACAO_ENUM nOrdem)
{
	BESQL_Ordem *Ordem = NULL;

	//verificar
	if (!Sintaxe || !Campo)
		return NULL;

	Ordem = g_new (BESQL_Ordem, 1);
	Ordem->Campo = Campo;
	Ordem->nOrdem = nOrdem;
	Ordem->bAtivo = TRUE;
	//anexando na lista
	Sintaxe->Ordens = g_slist_append (Sintaxe->Ordens, Ordem);

	return Ordem;
}

void _BE_sql_ordem_excluir (BESQL_Sintaxe *Sintaxe, BESQL_Ordem *Ordem)
{
	Sintaxe->Ordens = g_slist_remove (Sintaxe->Filtros, Ordem);
	BE_free (Ordem);
}

void be_sql_ordem_limpar (BESQL_Sintaxe *Sintaxe)
{
	BESQL_Ordem *Ordem = NULL;
	GSList *ListaL = NULL;

	//ordens
	for (ListaL = Sintaxe->Ordens; ListaL; ListaL = g_slist_next (ListaL))
	{
		Ordem = (BESQL_Ordem *) ListaL->data;
		BE_free (Ordem);
	}
	BE_slist_free (Sintaxe->Ordens);
}

BESQL_Filtro *be_sql_filtro_adicionar (
	BESQL_Sintaxe *Sintaxe,
	BESQL_Campo *Campo,
	const gchar *sComparacao,
	const gchar *sValor,
	BESQL_RELACAO_ENUM nRelacao,
	gint nGrupo,
	gboolean bUniao)
{
	BESQL_Filtro *Filtro = NULL;

	//verificar
	if (!Sintaxe || !Campo)
		return NULL;

	Filtro = g_new (BESQL_Filtro, 1);
	Filtro->Campo = Campo;
	Filtro->sComparacao = g_strdup (sComparacao);
	if (sValor && Campo->bAnulavel && Sintaxe->bForcarNulos && strlen (sValor) == 0)
		Filtro->sValor = NULL;
	else if (sValor)
		Filtro->sValor = g_strdup (sValor);
	else
		Filtro->sValor = NULL;
	Filtro->nRelacao = nRelacao;
	Filtro->nGrupo = nGrupo;
	Filtro->bAtivo = TRUE;
	//verificar tipo
	if (bUniao)
		Sintaxe->FiltrosUniao = g_slist_append (Sintaxe->FiltrosUniao, Filtro);//anexando na lista
	else
		Sintaxe->Filtros = g_slist_append (Sintaxe->Filtros, Filtro);//anexando na lista
	//indefinir linhas
	Sintaxe->lLinhas = -1;

	return Filtro;
}

void _be_sql_filtro_excluir (BESQL_Sintaxe *Sintaxe, BESQL_Filtro *Filtro, gboolean bUniao)
{
	if (!Filtro)
		return;

	if (bUniao)
	{
		Sintaxe->FiltrosUniao = g_slist_remove (Sintaxe->FiltrosUniao, Filtro);
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	else
	{
		Sintaxe->Filtros = g_slist_remove (Sintaxe->Filtros, Filtro);
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	//indefinir linhas
	Sintaxe->lLinhas = -1;
}

void be_sql_filtro_limpar (BESQL_Sintaxe *Sintaxe, gboolean bUniao)
{
	BESQL_Filtro *Filtro = NULL;
	GSList *ListaL = NULL;

	//verificar Sintaxe válida
	if (!Sintaxe)
        return;

	if (bUniao)
	{
		//filtros união
		for (ListaL = Sintaxe->FiltrosUniao; ListaL; ListaL = g_slist_next (ListaL))
		{
			Filtro = (BESQL_Filtro *) ListaL->data;
			BE_free (Filtro->sValor);
			BE_free (Filtro->sComparacao);
			BE_free (Filtro);
		}
		BE_slist_free (Sintaxe->FiltrosUniao);
		Sintaxe->FiltrosUniao = NULL;
	}
	else
	{
		//filtros
		for (ListaL = Sintaxe->Filtros; ListaL; ListaL = g_slist_next (ListaL))
		{
			Filtro = (BESQL_Filtro *) ListaL->data;
			BE_free (Filtro->sValor);
			BE_free (Filtro->sComparacao);
			BE_free (Filtro);
		}
		BE_slist_free (Sintaxe->Filtros);
		Sintaxe->Filtros = NULL;
	}
	//indefinir linhas
	Sintaxe->lLinhas = -1;	
}

GSList *be_sql_filtro_duplicar_lista (GSList *Filtros, BESQL_Sintaxe *SintaxeCampos)
{
	BESQL_Filtro *FiltroO = NULL, *FiltroD = NULL;
	GSList *FiltrosD = NULL, *ListaL = NULL;

	//filtros
	for (ListaL = Filtros; ListaL; ListaL = g_slist_next (ListaL))
	{
		FiltroO = (BESQL_Filtro *) ListaL->data;
		FiltroD = g_new (BESQL_Filtro, 1);
		if (SintaxeCampos && FiltroO->Campo)
			FiltroD->Campo = be_sql_selecionar_campo (SintaxeCampos, FiltroO->Campo->sNome);
		else
			FiltroD->Campo = FiltroO->Campo;
		if (FiltroO->sComparacao)
			FiltroD->sComparacao = g_strdup (FiltroO->sComparacao);
		else
			FiltroD->sComparacao = NULL;
		if (FiltroO->sValor)
			FiltroD->sValor = g_strdup (FiltroO->sValor);
		else
			FiltroD->sValor = NULL;
		FiltroD->nRelacao = FiltroO->nRelacao;
		FiltroD->nGrupo = FiltroO->nGrupo;
		FiltroD->bAtivo = FiltroO->bAtivo;
		//anexar na lista
		FiltrosD = g_slist_append (FiltrosD, FiltroD);
	}

	return FiltrosD;
}

void _be_sql_filtro_limpar_lista (GSList *Filtros)
{
	BESQL_Filtro *Filtro = NULL;
	GSList *ListaL = NULL;

	//filtros
	for (ListaL = Filtros; ListaL; ListaL = g_slist_next (ListaL))
	{
		Filtro = (BESQL_Filtro *) ListaL->data;
		BE_free (Filtro->sValor);
		BE_free (Filtro->sComparacao);
		BE_free (Filtro);
	}
	BE_slist_free (Filtros);
}

gint _be_sql_filtro_ordenar_grupo (gconstpointer a, gconstpointer b)
{
	BESQL_Filtro *FiltroA = NULL, *FiltroB = NULL;

	//cast
	FiltroA = (BESQL_Filtro *) a;
	FiltroB = (BESQL_Filtro *) b;
	//comparação
	return FiltroA->nGrupo - FiltroB->nGrupo;
}

gchar *_be_sql_filtro_sintaxe (BESQL_Sintaxe *Sintaxe, GSList *Filtros)
{
	BESQL *SQL = NULL;
	BESQL_Filtro *Filtro = NULL;
	GString *sSQL = NULL;
	GSList *ListaL = NULL;
	gchar *sValor = NULL;
	gint nGrupo = 0;
	gint nCont = 0;

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//filtros da consulta SELECT
	if (g_slist_length (Filtros) > 0)
	{
		//iniciar
		sSQL = g_string_new ("");
		//ordenar por grupo
		Filtros = g_slist_sort (Filtros, _be_sql_filtro_ordenar_grupo);
		//loop dos filtros
		for (ListaL = Filtros; ListaL; ListaL = g_slist_next (ListaL))
		{
			Filtro = (BESQL_Filtro *) ListaL->data;
			if (Filtro->bAtivo)
			{
				//relação
				if (nCont == 0)
					g_string_append_printf (sSQL, "(");
				else if (nGrupo != Filtro->nGrupo)
					g_string_append_printf (sSQL, ") OR (");//relação Ou entre grupos
				else
					g_string_append_printf (sSQL, " %s ", (Filtro->nRelacao == BESQL_RELACAO_E ? "AND" : "OR"));
				//corrigir comparação
				if (!Filtro->sValor && g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_IGUAL))
				{
					BE_free (Filtro->sComparacao);
					Filtro->sComparacao = g_strdup (BESQL_COMPARACAO_NULO);
				}
				else if (!Filtro->sValor && g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_DIFERENTE))
				{
					BE_free (Filtro->sComparacao);
					Filtro->sComparacao = g_strdup (BESQL_COMPARACAO_NAO_NULO);
				}
				if (g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_NULO) || g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_NAO_NULO) || g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_VERDADEIRO) || g_str_equal (Filtro->sComparacao, BESQL_COMPARACAO_FALSO))
				{
					g_string_append_printf (sSQL, "`%s` %s", Filtro->Campo->sNome, Filtro->sComparacao);
				}
				else
				{
					if (Filtro->sValor)
					{
						sValor = be_sql_escapar (SQL, Filtro->sValor, strlen (Filtro->sValor));//preparando valor
						//remover máscara
						if (Sintaxe->Tabela->BD->bMascara)
						{
							switch (Filtro->Campo->nTipo)
							{
								case BESQL_TIPO_NUM:
								case MYSQL_TYPE_FLOAT:
								case MYSQL_TYPE_DOUBLE:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMNUM, sValor);
									break;
								case BESQL_TIPO_MOE:
								case MYSQL_TYPE_DECIMAL:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMMOE, sValor);
									break;
								case BESQL_TIPO_BOL:
								case MYSQL_TYPE_BIT:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMBOL, sValor);
									break;
								case BESQL_TIPO_DAT:
								case MYSQL_TYPE_DATE:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMDAT, sValor);
									break;
								case BESQL_TIPO_TEM:
								case MYSQL_TYPE_DATETIME:
								case MYSQL_TYPE_TIMESTAMP:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMTEM, sValor);
									break;
								case BESQL_TIPO_FONx:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFONx, sValor);
									break;
								case BESQL_TIPO_FON:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFON, sValor);
									break;
								case BESQL_TIPO_CNPJ:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCNPJ, sValor);
									break;
								case BESQL_TIPO_CPF:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCPF, sValor);
									break;
								case BESQL_TIPO_CEP:
									g_string_append_printf (sSQL, "`%s` %s `%s`.%s('%s')", Filtro->Campo->sNome, Filtro->sComparacao, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCEP, sValor);
									break;
								default:
									g_string_append_printf (sSQL, "`%s` %s '%s'", Filtro->Campo->sNome, Filtro->sComparacao, sValor);
									break;
							}
						}
						else
							g_string_append_printf (sSQL, "`%s` %s '%s'", Filtro->Campo->sNome, Filtro->sComparacao, sValor);
					}
					else
						g_string_append_printf (sSQL, "`%s` %s NULL", Filtro->Campo->sNome, Filtro->sComparacao);
				}
				BE_free (sValor);
				nGrupo = Filtro->nGrupo;
				nCont++;
			}
		}
		g_string_append_printf (sSQL, ")");

		return BE_string_free (sSQL);
	}
	else
		return NULL;
}

void be_sql_contar_linhas (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;
	//manter se já existir um valor válido
	if (Sintaxe->lLinhas > 0)
		return;
	//indefinir
	Sintaxe->lLinhas = -1;
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return;

	//callback
	if (SQL->func_Info)
		SQL->func_Info (SQL, 0, NULL, SQL->user_data);

	//filtros da consulta SELECT
	sTemp = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->Filtros);
	sTemp1 = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->FiltrosUniao);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp1);
#endif
	if (sTemp && sTemp1)
		sSQL = g_strdup_printf ("SELECT COUNT(*) FROM `%s`.`%s` WHERE (%s) AND (%s)", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp, sTemp1);
	else if (sTemp)
		sSQL = g_strdup_printf ("SELECT COUNT(*) FROM `%s`.`%s` WHERE %s", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp);
	else if (sTemp1)
		sSQL = g_strdup_printf ("SELECT COUNT(*) FROM `%s`.`%s` WHERE %s", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp1);
	else
		sSQL = g_strdup_printf ("SELECT COUNT(*) FROM `%s`.`%s`", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			row = mysql_fetch_row (res);
			if (row[0])
				Sintaxe->lLinhas = atol (row[0]);
			else
				Sintaxe->lLinhas = 0;
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	BE_free (sSQL);
	BE_free (sTemp);
	BE_free (sTemp1);
}

void be_sql_totalizar_linhas (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	GSList *ListaL = NULL;
	GString *sCampos = NULL;
	gchar *sSQL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	gint nCont = 0, nCampos = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return;

	//callback
	if (SQL->func_Info)
		SQL->func_Info (SQL, 0, NULL, SQL->user_data);

	//filtros da consulta SELECT
	sTemp = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->Filtros);
	sTemp1 = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->FiltrosUniao);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp1);
#endif
	sCampos = g_string_new ("");
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)
		{
			if (Sintaxe->Tabela->BD->bMascara)
			{
				switch (Campo->nTipo)
				{
					case BESQL_TIPO_NUM:
					case MYSQL_TYPE_FLOAT:
					case MYSQL_TYPE_DOUBLE:
						g_string_append_printf (sCampos, "`%s`.%s(SUM(`%s`)), ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMNUM, Campo->sNome);
						break;
					case BESQL_TIPO_MOE:
					case MYSQL_TYPE_DECIMAL:
						g_string_append_printf (sCampos, "`%s`.%s(SUM(`%s`)), ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMMOE, Campo->sNome);
						break;
					default:
						g_string_append_printf (sCampos, "COUNT(`%s`), ", Campo->sNome);
				}
			}
			else
			{
				switch (Campo->nTipo)
				{
					case BESQL_TIPO_NUM:
					case MYSQL_TYPE_FLOAT:
					case MYSQL_TYPE_DOUBLE:
						g_string_append_printf (sCampos, "SUM(`%s`), ", Campo->sNome);
						break;
					case BESQL_TIPO_MOE:
					case MYSQL_TYPE_DECIMAL:
						g_string_append_printf (sCampos, "SUM(`%s`), ", Campo->sNome);
						break;
					default:
						g_string_append_printf (sCampos, "COUNT(`%s`), ", Campo->sNome);
				}
			}
			nCampos++;
		}
	}
	sCampos = g_string_truncate (sCampos, sCampos->len-2);//excluir a última vírgula e espaço
	sTemp2 = BE_string_free (sCampos);

	if (sTemp && sTemp1)
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s` WHERE (%s) AND (%s)", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp, sTemp1);
	else if (sTemp)
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s` WHERE %s", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp);
	else if (sTemp1)
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s` WHERE %s", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp1);
	else
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s`", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
	if (nCampos > 0)
	{
        _be_sql_saida (sSQL);//depuração
		if (mysql_query (SQL->_my, sSQL) == 0)
		{
			if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
			{
				row = mysql_fetch_row (res);
				//recuperar campos
				nCont = 0;
				for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Campo = (BESQL_Campo *) ListaL->data;
					if (Campo->bAtivo)
					{
						Campo->sTotal = g_strdup (row[nCont]);
						nCont++;
					}
				}
				mysql_free_result (res);
			}
			else//erro no retorno
				_be_sql_erro_retorno (SQL, sErros);
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (SQL, sErros, sSQL);
		}
	}

	BE_free (sSQL);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
}

void be_sql_comprimento_linhas (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	GSList *ListaL = NULL;
	GString *sCampos = NULL;
	gchar *sSQL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	gint nCont = 0, nCampos = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return;
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return;

	//callback
	if (SQL->func_Info)
		SQL->func_Info (SQL, 0, NULL, SQL->user_data);

	//filtros da consulta SELECT
	sTemp = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->Filtros);
	sTemp1 = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->FiltrosUniao);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp1);
#endif
	sCampos = g_string_new ("");
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)
		{
			if (Sintaxe->Tabela->BD->bMascara)
			{
				switch (Campo->nTipo)
				{
					case BESQL_TIPO_NUM:
					case MYSQL_TYPE_FLOAT:
					case MYSQL_TYPE_DOUBLE:
						g_string_append_printf (sCampos, "MAX(CHAR_LENGTH(`%s`)+1), ", Campo->sNome);
						break;
					case BESQL_TIPO_MOE:
					case MYSQL_TYPE_DECIMAL:
						g_string_append_printf (sCampos, "MAX(CHAR_LENGTH(`%s`)+5), ", Campo->sNome);
						break;
					case BESQL_TIPO_BOL:
					case MYSQL_TYPE_BIT:
						g_string_append_printf (sCampos, "(3), ");
						break;
					case BESQL_TIPO_FONx:
						g_string_append_printf (sCampos, "(18), ");
						break;
					case BESQL_TIPO_FON:
						g_string_append_printf (sCampos, "(15), ");
						break;
					case BESQL_TIPO_CNPJ:
						g_string_append_printf (sCampos, "(18), ");
						break;
					case BESQL_TIPO_CPF:
						g_string_append_printf (sCampos, "(14), ");
						break;
					case BESQL_TIPO_CEP:
						g_string_append_printf (sCampos, "(10), ");
						break;
					case BESQL_TIPO_TEM:
					case MYSQL_TYPE_DATETIME:
					case MYSQL_TYPE_TIMESTAMP:
						g_string_append_printf (sCampos, "(19), ");
						break;
					case BESQL_TIPO_DAT:
					case MYSQL_TYPE_DATE:
						g_string_append_printf (sCampos, "(10), ");
						break;
					case MYSQL_TYPE_TIME:
						g_string_append_printf (sCampos, "(8), ");
						break;
					case MYSQL_TYPE_YEAR:
						g_string_append_printf (sCampos, "(4), ");
						break;
					case MYSQL_TYPE_BLOB:
					case MYSQL_TYPE_TINY_BLOB:
					case MYSQL_TYPE_MEDIUM_BLOB:
					case MYSQL_TYPE_LONG_BLOB:
						g_string_append_printf (sCampos, "(1), ");
						break;
					default:
						g_string_append_printf (sCampos, "MAX(CHAR_LENGTH(`%s`)), ", Campo->sNome);
				}
			}
			else
				g_string_append_printf (sCampos, "MAX(CHAR_LENGTH(`%s`)), ", Campo->sNome);
			nCampos++;
		}
	}
	sCampos = g_string_truncate (sCampos, sCampos->len-2);//excluir a última vírgula e espaço
	sTemp2 = BE_string_free (sCampos);

	if (sTemp && sTemp1)
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s` WHERE (%s) AND (%s)", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp, sTemp1);
	else if (sTemp)
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s` WHERE %s", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp);
	else if (sTemp1)
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s` WHERE %s", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp1);
	else
		sSQL = g_strdup_printf ("SELECT %s FROM `%s`.`%s`", sTemp2, Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
	if (nCampos > 0)
	{
        _be_sql_saida (sSQL);//depuração
		if (mysql_query (SQL->_my, sSQL) == 0)
		{
			if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
			{
				row = mysql_fetch_row (res);
				//recuperar campos
				nCont = 0;
				for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Campo = (BESQL_Campo *) ListaL->data;
					if (Campo->bAtivo)
					{
						if (row[nCont])
							Campo->nComprimentoTop = (gint) atol (row[nCont]);
						else
							Campo->nComprimentoTop = 0;
						nCont++;
					}
				}
				mysql_free_result (res);
			}
			else//erro no retorno
				_be_sql_erro_retorno (SQL, sErros);
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (SQL, sErros, sSQL);
		}
	}

	BE_free (sSQL);
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
}

gint be_sql_paginas (BESQL_Sintaxe *Sintaxe)
{
	gint nQt = 0;

	nQt = Sintaxe->lLinhas / Sintaxe->lLinhasPaginaMax;
	if ((Sintaxe->lLinhas % Sintaxe->lLinhasPaginaMax) > 0)
		nQt++;

	return nQt;
}

glong be_sql_selecionar (BESQL_Sintaxe *Sintaxe, gint nPagina, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	BESQL_Ordem *Ordem = NULL;
	MYSQL_FIELD *field;
	GString *sSQL = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nCont = 0, nCampos = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return 0;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return 0;
	//verificar página
	if (nPagina <= 0)
		return 0;
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//callback
	if (SQL->func_Info)
		SQL->func_Info (SQL, 0, NULL, SQL->user_data);

	//liberando consulta anterior
	if (Sintaxe->res)
		mysql_free_result (Sintaxe->res);
	Sintaxe->lLinha = 0;
	//campos da consulta SELECT
	sSQL = g_string_new ("SELECT ");
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAtivo)
		{
			Campo->nCampoPos = nCampos;
			if (Sintaxe->Tabela->BD->bMascara)
			{
				switch (Campo->nTipo)
				{
					case BESQL_TIPO_NUM:
					case MYSQL_TYPE_FLOAT:
					case MYSQL_TYPE_DOUBLE:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMNUM, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_MOE:
					case MYSQL_TYPE_DECIMAL:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMMOE, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_BOL:
					case MYSQL_TYPE_BIT:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMBOL, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_DAT:
					case MYSQL_TYPE_DATE:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMDAT, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_TEM:
					case MYSQL_TYPE_DATETIME:
					case MYSQL_TYPE_TIMESTAMP:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMTEM, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_FONx:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMFONx, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_FON:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMFON, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_CNPJ:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMCNPJ, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_CPF:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMCPF, Campo->sNome, Campo->sNome);
						break;
					case BESQL_TIPO_CEP:
						g_string_append_printf (sSQL, "`%s`.%s(`%s`) AS `%s`, ", Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_AMCEP, Campo->sNome, Campo->sNome);
						break;
					default:
						g_string_append_printf (sSQL, "`%s`, ", Campo->sNome);
				}
			}
			else
				g_string_append_printf (sSQL, "`%s`, ", Campo->sNome);
			nCampos++;
		}
		else
			Campo->nCampoPos = -1;
	}
	sSQL = g_string_truncate (sSQL, sSQL->len-2);//excluir a última vírgula e espaço
	//tabela da consulta SELECT
	g_string_append_printf (sSQL, " FROM `%s`.`%s`", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);

	//filtros da consulta SELECT
	sTemp = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->Filtros);
	sTemp1 = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->FiltrosUniao);
	if (sTemp && sTemp1)
	{
		g_string_append_printf (sSQL, " WHERE (%s) AND (%s)", sTemp, sTemp1);
		BE_free (sTemp);
		BE_free (sTemp1);
	}
	else if (sTemp)
	{
		g_string_append_printf (sSQL, " WHERE %s", sTemp);
		BE_free (sTemp);
	}
	else if (sTemp1)
	{
		g_string_append_printf (sSQL, " WHERE %s", sTemp1);
		BE_free (sTemp1);
	}
	//ordens da consulta SELECT
	if (g_slist_length (Sintaxe->Ordens) > 0)
	{
		//loop das ordens
		nCont = 0;
		for (ListaL = Sintaxe->Ordens; ListaL; ListaL = g_slist_next (ListaL))
		{
			Ordem = (BESQL_Ordem *) ListaL->data;
			if (Ordem->bAtivo)
			{
				if (nCont == 0)
					g_string_append_printf (sSQL, " ORDER BY ");
				else
					g_string_append_printf (sSQL, ",");
				g_string_append_printf (sSQL, "`%s` %s", Ordem->Campo->sNome, (Ordem->nOrdem == BESQL_ORDENACAO_DESC ? "DESC" : "ASC"));
				nCont++;
			}
		}
	}
	//limites da consulta SELECT
	g_string_append_printf (sSQL, " LIMIT %d, %d;", (gint) ((nPagina - 1) * Sintaxe->lLinhasPaginaMax), (gint) Sintaxe->lLinhasPaginaMax);
	//guardando consulta
	Sintaxe->sConsulta = BE_string_free (sSQL);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, Sintaxe->sConsulta);
#endif
	//executando nova consulta
	if (nCampos > 0)
	{
        _be_sql_saida (Sintaxe->sConsulta);//depuração
		if (mysql_query (SQL->_my, Sintaxe->sConsulta) == 0)
		{
			if ((Sintaxe->res = mysql_store_result (SQL->_my)))//recebe a consulta
			{
				//iniciar contagem de linhas
				Sintaxe->lLinha = (nPagina - 1) * Sintaxe->lLinhasPaginaMax;
				//tipos dos campos
				for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Campo = (BESQL_Campo *) ListaL->data;
					Campo->nComprimento = 0;
					Campo->sValor = NULL;
					if (Campo->nCampoPos >= 0)
					{
						//MySQL-refman-5.0-en.a4.pdf pág. 1084
						field = mysql_fetch_field_direct (Sintaxe->res, Campo->nCampoPos);
						//field->type;
						Campo->nComprimentoMax = (gint) field->length;
						//Campo->bAnulavel = !((field->flags & NOT_NULL_FLAG) == NOT_NULL_FLAG);
						//printf ("%s - %s\n", field->org_table, field->name);//cli
					}
				}
			}
			else//erro no retorno
				_be_sql_erro_retorno (SQL, sErros);
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (SQL, sErros, Sintaxe->sConsulta);
		}
	}
	//retornar a quantidade de linhas na página atual
	if (Sintaxe->res)
		return mysql_num_rows (Sintaxe->res);
	else
		return 0;
}

gint be_sql_linhas_pagina (BESQL_Sintaxe *Sintaxe)
{
	return mysql_num_rows (Sintaxe->res);
}

glong be_sql_selecionar_proxima_linha (BESQL_Sintaxe *Sintaxe)
{
	MYSQL_ROW row = NULL;//linhas
	BESQL_Campo *Campo = NULL;
	gulong *pLen = NULL;
	GSList *ListaL = NULL;

	//callback
	if (Sintaxe->Tabela->BD->SQL->func_Info)
		Sintaxe->Tabela->BD->SQL->func_Info (Sintaxe->Tabela->BD->SQL, 0, NULL, Sintaxe->Tabela->BD->SQL->user_data);

	//preparar os campos
	_be_sql_campo_valores_limpar (Sintaxe);
	//verificar novos valores
	if (Sintaxe->res)
	{
		//avançar linha
		row = mysql_fetch_row (Sintaxe->res);
		Sintaxe->lLinha++;
		if (row)
		{
			//comprimento
			pLen = mysql_fetch_lengths (Sintaxe->res);
			//recuperar valores
			for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				if (Campo->nCampoPos >= 0)
				{
					Campo->sValor = row[Campo->nCampoPos];
					Campo->nComprimento = pLen[Campo->nCampoPos];
				}
			}
			return Sintaxe->lLinha;
		}
		else
		{
			//definir quantidade
			if (Sintaxe->lLinha < Sintaxe->lLinhasPaginaMax && Sintaxe->lLinha > 0 && Sintaxe->lLinhasPaginaMax > 0)
				Sintaxe->lLinhas = Sintaxe->lLinha - 1;
			Sintaxe->lLinha = 0;
			mysql_free_result (Sintaxe->res);
			return Sintaxe->lLinha;
		}
	}
	else
	{
		//definir quantidade
		if (Sintaxe->lLinha < Sintaxe->lLinhasPaginaMax && Sintaxe->lLinha > 0 && Sintaxe->lLinhasPaginaMax > 0)
			Sintaxe->lLinhas = Sintaxe->lLinha - 1;
		Sintaxe->lLinha = 0;
		return Sintaxe->lLinha;
	}
}

glong be_sql_adicionar (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	GString *sSQL = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sValor = NULL;
	glong lAfetados = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return -1;
	}
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_INSERIR))
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return -1;
	}

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//campos da consulta
	sSQL = g_string_new ("INSERT ");
	g_string_append_printf (sSQL, "`%s`.`%s` SET ", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAfetar)
		{
			if (Campo->sValorNovo)
			{
				//preparando valor
				BE_free (sValor);
				if (Campo->nComprimentoNovo > 0)
					sValor = be_sql_escapar (SQL, Campo->sValorNovo, Campo->nComprimentoNovo);
				else
					sValor = be_sql_escapar (SQL, Campo->sValorNovo, strlen (Campo->sValorNovo));
				//verificar máscaras
				if (Sintaxe->Tabela->BD->bMascara)
				{
					switch (Campo->nTipo)
					{
						case BESQL_TIPO_NUM:
						case MYSQL_TYPE_FLOAT:
						case MYSQL_TYPE_DOUBLE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMNUM, sValor);
							break;
						case BESQL_TIPO_MOE:
						case MYSQL_TYPE_DECIMAL:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMMOE, sValor);
							break;
						case BESQL_TIPO_BOL:
						case MYSQL_TYPE_BIT:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMBOL, sValor);
							break;
						case BESQL_TIPO_DAT:
						case MYSQL_TYPE_DATE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMDAT, sValor);
							break;
						case BESQL_TIPO_TEM:
						case MYSQL_TYPE_DATETIME:
						case MYSQL_TYPE_TIMESTAMP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMTEM, sValor);
							break;
						case BESQL_TIPO_FONx:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFONx, sValor);
							break;
						case BESQL_TIPO_FON:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFON, sValor);
							break;
						case BESQL_TIPO_CNPJ:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCNPJ, sValor);
							break;
						case BESQL_TIPO_CPF:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCPF, sValor);
							break;
						case BESQL_TIPO_CEP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCEP, sValor);
							break;
						default:
							g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
					}
				}
				else
					g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
			}
			else
				g_string_append_printf (sSQL, "`%s` = NULL, ", Campo->sNome);
		}
		//inserir com valor padrão, se houver valor padrão
		//else if (Campo->nTipo != MYSQL_TYPE_TIMESTAMP)
		else
		{
			g_string_append_printf (sSQL, "`%s` = DEFAULT, ", Campo->sNome);
		}
	}
	sSQL = g_string_truncate (sSQL, sSQL->len-2);//excluir a última vírgula e espaço
	BE_free (sTemp);
	sTemp = BE_string_free (sSQL);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
#endif
    _be_sql_saida (sTemp);//depuração
	//executar consulta
	if (mysql_query (SQL->_my, sTemp) == 0)
	{
		lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
		//erro no retorno
		if (lAfetados < 0)
			_be_sql_erro_alteracao (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sTemp);
	}

    //verificar avisos (warnings)
    _be_sql_aviso_retorno (SQL, sErros);

	BE_free (sTemp);
	BE_free (sValor);
	_be_sql_campo_valores_limpar (Sintaxe);

	return lAfetados;
}

glong _be_sql_alterar (BESQL_Sintaxe *Sintaxe, const gchar *sFiltro, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	GString *sSQL = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sValor = NULL;
	glong lAfetados = 0;
	gint nCont = 0, nLen = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return -1;
	}
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_ALTERAR))
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return -1;
	}
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//campos da consulta
	nCont = 0;
	sSQL = g_string_new ("UPDATE ");
	g_string_append_printf (sSQL, "`%s`.`%s` SET ", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAfetar)
		{
			//verificar comprimento
			if (Campo->nComprimentoNovo > 0)
				nLen = Campo->nComprimentoNovo;
			else if (Campo->sValorNovo)
				nLen = strlen (Campo->sValorNovo);
			else nLen = 0;
			//verificar alterações
			if (Campo->sValorNovo && !(nLen == 0 && Campo->bAnulavel && Sintaxe->bForcarNulos))
			{
				//preparando valor
				BE_free (sValor);
				sValor = be_sql_escapar (SQL, Campo->sValorNovo, nLen);
				//verificar máscaras
				if (Sintaxe->Tabela->BD->bMascara)
				{
					switch (Campo->nTipo)
					{
						case BESQL_TIPO_NUM:
						case MYSQL_TYPE_FLOAT:
						case MYSQL_TYPE_DOUBLE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMNUM, sValor);
							break;
						case BESQL_TIPO_MOE:
						case MYSQL_TYPE_DECIMAL:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMMOE, sValor);
							break;
						case BESQL_TIPO_BOL:
						case MYSQL_TYPE_BIT:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMBOL, sValor);
							break;
						case BESQL_TIPO_DAT:
						case MYSQL_TYPE_DATE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMDAT, sValor);
							break;
						case BESQL_TIPO_TEM:
						case MYSQL_TYPE_DATETIME:
						case MYSQL_TYPE_TIMESTAMP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMTEM, sValor);
							break;
						case BESQL_TIPO_FONx:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFONx, sValor);
							break;
						case BESQL_TIPO_FON:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFON, sValor);
							break;
						case BESQL_TIPO_CNPJ:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCNPJ, sValor);
							break;
						case BESQL_TIPO_CPF:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCPF, sValor);
							break;
						case BESQL_TIPO_CEP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCEP, sValor);
							break;
						default:
							g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
					}
				}
				else
					g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
			}
			else
				g_string_append_printf (sSQL, "`%s` = NULL, ", Campo->sNome);
			nCont++;
		}
	}
	sSQL = g_string_truncate (sSQL, sSQL->len-2);//excluir a última vírgula e espaço
	//adicionar filtro
	if (sFiltro)
		g_string_append_printf (sSQL, " %s", sFiltro);
	sTemp = BE_string_free (sSQL);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
#endif
	if (nCont > 0)
	{
        _be_sql_saida (sTemp);//depuração
		//excecutar consulta
		if (mysql_query (SQL->_my, sTemp) == 0)
		{
			lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
			//erro no retorno
			if (lAfetados < 0)
				_be_sql_erro_alteracao (SQL, sErros);
		}
		//erro na consulta
		else
		{
			_be_sql_erro_consulta (SQL, sErros, sTemp);
		}

        //verificar avisos (warnings)
        _be_sql_aviso_retorno (SQL, sErros);
	}

	BE_free (sTemp);
	BE_free (sValor);
	_be_sql_campo_valores_limpar (Sintaxe);

	return lAfetados;
}

glong be_sql_substituir (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	GString *sSQL = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sValor = NULL;
	glong lAfetados = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return -1;
	}
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->Tabela->nFlags & BESQL_ACESSO_INSERIR))
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return -1;
	}
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//campos da consulta
	sSQL = g_string_new ("REPLACE ");
	g_string_append_printf (sSQL, "`%s`.`%s` SET ", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAfetar)
		{
			if (Campo->sValorNovo)
			{
				//preparando valor
				BE_free (sValor);
				if (Campo->nComprimentoNovo > 0)
					sValor = be_sql_escapar (SQL, Campo->sValorNovo, Campo->nComprimentoNovo);
				else
					sValor = be_sql_escapar (SQL, Campo->sValorNovo, strlen (Campo->sValorNovo));
				//verificar máscaras
				if (Sintaxe->Tabela->BD->bMascara)
				{
					switch (Campo->nTipo)
					{
						case BESQL_TIPO_NUM:
						case MYSQL_TYPE_FLOAT:
						case MYSQL_TYPE_DOUBLE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMNUM, sValor);
							break;
						case BESQL_TIPO_MOE:
						case MYSQL_TYPE_DECIMAL:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMMOE, sValor);
							break;
						case BESQL_TIPO_BOL:
						case MYSQL_TYPE_BIT:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMBOL, sValor);
							break;
						case BESQL_TIPO_DAT:
						case MYSQL_TYPE_DATE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMDAT, sValor);
							break;
						case BESQL_TIPO_TEM:
						case MYSQL_TYPE_DATETIME:
						case MYSQL_TYPE_TIMESTAMP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMTEM, sValor);
							break;
						case BESQL_TIPO_FONx:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFONx, sValor);
							break;
						case BESQL_TIPO_FON:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFON, sValor);
							break;
						case BESQL_TIPO_CNPJ:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCNPJ, sValor);
							break;
						case BESQL_TIPO_CPF:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCPF, sValor);
							break;
						case BESQL_TIPO_CEP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCEP, sValor);
							break;
						default:
							g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
					}
				}
				else
					g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
			}
			else
				g_string_append_printf (sSQL, "`%s` = NULL, ", Campo->sNome);
		}
		//inserir com valor padrão, se houver valor padrão
		//else if (Campo->nTipo != MYSQL_TYPE_TIMESTAMP)
		else
		{
			g_string_append_printf (sSQL, "`%s` = DEFAULT, ", Campo->sNome);
		}
	}
	sSQL = g_string_truncate (sSQL, sSQL->len-2);//excluir a última vírgula e espaço
	BE_free (sTemp);
	sTemp = BE_string_free (sSQL);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
#endif
    _be_sql_saida (sTemp);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sTemp) == 0)
	{
		lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
		//erro no retorno
		if (lAfetados < 0)
			_be_sql_erro_alteracao (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sTemp);
	}

    //verificar avisos (warnings)
    _be_sql_aviso_retorno (SQL, sErros);

	BE_free (sTemp);
	BE_free (sValor);
	_be_sql_campo_valores_limpar (Sintaxe);

	return lAfetados;
}

glong be_sql_alterar (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	gchar *sFiltro = NULL, *sTemp = NULL, *sTemp1 = NULL;
	glong lAfetados = 0;

	//filtros da consulta SELECT
	sTemp = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->Filtros);
	sTemp1 = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->FiltrosUniao);
	if (sTemp && sTemp1)
	{
		sFiltro = g_strdup_printf ("WHERE (%s) AND (%s)", sTemp, sTemp1);
	}
	else if (sTemp)
	{
		sFiltro = g_strdup_printf ("WHERE %s", sTemp);
	}
	else if (sTemp1)
	{
		sFiltro = g_strdup_printf ("WHERE %s", sTemp1);
	}
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sFiltro);
#endif
	//executar alteração
	lAfetados = _be_sql_alterar (Sintaxe, sFiltro, sErros)

	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sFiltro);

	return lAfetados;
}

glong be_sql_alterar_indice (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Indice *Indice = NULL;
	GString *Filtro = NULL;
	GSList *ListaL = NULL;
	gchar *sFiltro = NULL, *sValor = NULL;
	glong lAfetados = 0;
	gint nCont = 0;

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//loop nas chaves
	Filtro = g_string_new ("WHERE ");
	for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
	{
		Indice = (BESQL_Indice *) ListaL->data;
		if (Indice->bAfetar)
		{
			//verificar valor
			if (Indice->sValorFiltro)
			{
				//escapar valor
				BE_free (sValor);
				if (Indice->nComprimentoFiltro > 0)
					sValor = be_sql_escapar (SQL, Indice->sValorFiltro, Indice->nComprimentoFiltro);
				else
					sValor = be_sql_escapar (SQL, Indice->sValorFiltro, strlen (Indice->sValorFiltro));
				//adicionar filtro
				if (be_sql_campo_texto (Indice->Campo) || be_sql_campo_binario (Indice->Campo))
					g_string_append_printf (Filtro, "`%s` = BINARY '%s' AND ", Indice->Campo->sNome, sValor);
				else
					g_string_append_printf (Filtro, "`%s` = '%s' AND ", Indice->Campo->sNome, sValor);
			}
			else
			{
				g_string_append_printf (Filtro, "`%s` IS NULL AND ", Indice->Campo->sNome);
			}
			nCont++;
		}
	}
	Filtro = g_string_truncate (Filtro, Filtro->len-5);//excluir o último AND
	sFiltro = BE_string_free (Filtro);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sFiltro);
#endif
	//executar alteração
	if (nCont > 0)
		lAfetados = _be_sql_alterar (Sintaxe, sFiltro, sErros)

	BE_free (sFiltro);
	BE_free (sValor);

	return lAfetados;
}

glong be_sql_excluir (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	gchar *sSQL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	glong lAfetados = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return -1;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_APAGAR))
		return -1;
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;
	//filtros da consulta SELECT
	sTemp = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->Filtros);
	sTemp1 = _be_sql_filtro_sintaxe (Sintaxe, Sintaxe->FiltrosUniao);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp1);
#endif
	//consultas
	if (sTemp && sTemp1)
		sSQL = g_strdup_printf ("DELETE FROM `%s`.`%s` WHERE (%s) AND (%s)", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp, sTemp1);
	else if (sTemp)
		sSQL = g_strdup_printf ("DELETE FROM `%s`.`%s` WHERE %s", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp);
	else if (sTemp1)
		sSQL = g_strdup_printf ("DELETE FROM `%s`.`%s` WHERE %s", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome, sTemp1);
	else
		sSQL = g_strdup_printf ("DELETE FROM `%s`.`%s`", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//executar
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
		//erro no retorno
		if (lAfetados < 0)
			_be_sql_erro_alteracao (SQL, sErros);
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}
    //verificar avisos (warnings)
    _be_sql_aviso_retorno (SQL, sErros);

	BE_free (sSQL);
	BE_free (sTemp);
	BE_free (sTemp1);

	return lAfetados;
}

glong be_sql_excluir_indice (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Indice *Indice = NULL;
	GSList *ListaL = NULL;
	GString *Filtro = NULL;
	gchar *sSQL = NULL, *sValor = NULL;
	glong lAfetados = 0;
	gint nCont = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return -1;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_APAGAR))
		return -1;
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//loop nas chaves
	Filtro = g_string_new ("DELETE ");
	g_string_append_printf (Filtro, "FROM `%s`.`%s` WHERE ", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
	for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
	{
		Indice = (BESQL_Indice *) ListaL->data;
		if (Indice->bAfetar)
		{
			//escapar valor
			BE_free (sValor);
			if (Indice->nComprimentoFiltro > 0)
				sValor = be_sql_escapar (SQL, Indice->sValorFiltro, Indice->nComprimentoFiltro);
			else
				sValor = be_sql_escapar (SQL, Indice->sValorFiltro, strlen (Indice->sValorFiltro));
			//adicionar filtro
			if (be_sql_campo_texto (Indice->Campo) || be_sql_campo_binario (Indice->Campo))
				g_string_append_printf (Filtro, "`%s`=BINARY '%s' AND ", Indice->Campo->sNome, sValor);
			else
				g_string_append_printf (Filtro, "`%s`='%s' AND ", Indice->Campo->sNome, sValor);
			nCont++;
		}
	}
	Filtro = g_string_truncate (Filtro, Filtro->len-5);//excluir o último AND
	sSQL = BE_string_free (Filtro);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
	//executar
	if (nCont > 0)
	{
        _be_sql_saida (sSQL);//depuração
		if (mysql_query (SQL->_my, sSQL) == 0)
		{
			lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
			//erro no retorno
			if (lAfetados < 0)
				_be_sql_erro_alteracao (SQL, sErros);
		}
		else//erro na consulta
		{
			_be_sql_erro_consulta (SQL, sErros, sSQL);
		}
        //verificar avisos (warnings)
        _be_sql_aviso_retorno (SQL, sErros);
	}

	BE_free (sSQL);
	BE_free (sValor);

	return lAfetados;
}

gboolean be_sql_obter_bloqueio_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gboolean bOK = FALSE;

	//verificar para não continuar com erros
	if (sErros && *sErros)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar filtro
	if (!sID)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar se existe as tabelas relacionadas
	if (!Sintaxe->vis_Tabela)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar tabela original
	if (!Sintaxe->vis_Tabela->sNomeReal)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//obter trava para alterações
	sSQL = g_strdup_printf ("SELECT GET_LOCK(CONCAT('%s', '%s'),0)", Sintaxe->vis_Tabela->sNomeReal, sID);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			row = mysql_fetch_row (res);
			if (row[0] && (atol (row[0])) == 1)
				bOK = TRUE;
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	BE_free (sSQL);

	return bOK;
}

gboolean be_sql_obter_bloqueio_excluir_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gboolean bOK = FALSE;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return bOK;
	//verificar filtro
	if (!sID)
		return bOK;
	//verificar se existe as tabelas relacionadas
	if (!Sintaxe->vis_Tabela || !Sintaxe->exc_Tabela || !Sintaxe->mex_Tabela)
		return bOK;
	//verificar tipo de acesso
	if (!(Sintaxe->mex_Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->mex_Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return bOK;
	//verificar tipo de acesso
	if (!(Sintaxe->exc_Tabela->nFlags & BESQL_ACESSO_APAGAR) || !(Sintaxe->exc_Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return bOK;
	//verificar tabela original
	if (!Sintaxe->vis_Tabela->sNomeReal)
		return bOK;

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//obter trava para alterações
	sSQL = g_strdup_printf ("SELECT GET_LOCK(CONCAT('%s', '%s'),0)", Sintaxe->vis_Tabela->sNomeReal, sID);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			row = mysql_fetch_row (res);
			if (row[0] && (atol (row[0])) == 1)
				bOK = TRUE;
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	BE_free (sSQL);

	return bOK;
}

gboolean be_sql_liberar_bloqueio_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gboolean bOK = FALSE;

	//verificar para não continuar com erros
	if (sErros && *sErros)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar para não continuar com erros
	if (!Sintaxe)
		return bOK;
	//verificar filtro
	if (!sID)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar se existe as tabelas relacionadas
	if (!Sintaxe->vis_Tabela)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//verificar tabela original
	if (!Sintaxe->vis_Tabela->sNomeReal)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return bOK;
	}
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//liberar trava
	sSQL = g_strdup_printf ("SELECT RELEASE_LOCK(CONCAT('%s', '%s'))", Sintaxe->vis_Tabela->sNomeReal, sID);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			row = mysql_fetch_row (res);
			if (row[0] && (atol (row[0])) == 1)
				bOK = TRUE;
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	BE_free (sSQL);

	return bOK;
}

gint be_sql_usuario_bloqueando_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros)
{
	BESQL *SQL = NULL;
	MYSQL_RES *res = NULL;//resposta da consulta
	MYSQL_ROW row = NULL;//linhas
	gchar *sSQL = NULL;
	gint nUsuario = -1;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return nUsuario;
	//verificar filtro
	if (!sID)
		return nUsuario;
	//verificar se existe as tabelas relacionadas
	if (!Sintaxe->vis_Tabela)
		return nUsuario;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return nUsuario;
	//verificar tabela original
	if (!Sintaxe->vis_Tabela->sNomeReal)
		return nUsuario;

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//obter usuário que travou
	BE_free (sSQL);
	sSQL = g_strdup_printf ("SELECT IS_USED_LOCK(CONCAT('%s', '%s'))", Sintaxe->vis_Tabela->sNomeReal, sID);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
		{
			row = mysql_fetch_row (res);
			if (row[0])
				nUsuario = (gint) atol (row[0]);
			mysql_free_result (res);
		}
		else//erro no retorno
			_be_sql_erro_retorno (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	return nUsuario;
}

gboolean be_sql_adicionar_CIA (BESQL_Sintaxe *Sintaxe, gchar **sErros)
{
	glong lAfetados = 0;

	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_INSERIR) || !(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return FALSE;

	//executar a consulta
	lAfetados = be_sql_adicionar (Sintaxe, sErros);

	if (lAfetados == 1)
		return TRUE;
	else
		return FALSE;
}

gboolean be_sql_alterar_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros)
{
	BESQL *SQL = NULL;
	BESQL_Campo *Campo = NULL;
	GString *sSQL = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sValor = NULL;
	glong lAfetados = 0;
	gint nCont = 0, nLen = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return FALSE;
	}
	//verificar filtro
	if (!sID)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return FALSE;
	}
	//verificar se existe as tabelas relacionadas
	if (!Sintaxe->vis_Tabela)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return FALSE;
	}
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return FALSE;
	}
	//verificar tabela original
	if (!Sintaxe->vis_Tabela->sNomeReal)
	{
		_be_sql_campo_valores_limpar (Sintaxe);
		return FALSE;
	}
	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//campos da consulta
	nCont = 0;
	sSQL = g_string_new ("UPDATE ");
	g_string_append_printf (sSQL, "`%s`.`%s` SET ", Sintaxe->Tabela->BD->sNome, Sintaxe->Tabela->sNome);
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		if (Campo->bAfetar)
		{
			//verificar comprimento
			if (Campo->nComprimentoNovo > 0)
				nLen = Campo->nComprimentoNovo;
			else if (Campo->sValorNovo)
				nLen = strlen (Campo->sValorNovo);
			else nLen = 0;
			//verificar alterações
			if (Campo->sValorNovo && !(nLen == 0 && Campo->bAnulavel && Sintaxe->bForcarNulos))
			{
				//preparando valor
				BE_free (sValor);
				sValor = be_sql_escapar (SQL, Campo->sValorNovo, nLen);
				//verificar máscaras
				if (Sintaxe->Tabela->BD->bMascara)
				{
					switch (Campo->nTipo)
					{
						case BESQL_TIPO_NUM:
						case MYSQL_TYPE_FLOAT:
						case MYSQL_TYPE_DOUBLE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMNUM, sValor);
							break;
						case BESQL_TIPO_MOE:
						case MYSQL_TYPE_DECIMAL:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMMOE, sValor);
							break;
						case BESQL_TIPO_BOL:
						case MYSQL_TYPE_BIT:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMBOL, sValor);
							break;
						case BESQL_TIPO_DAT:
						case MYSQL_TYPE_DATE:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMDAT, sValor);
							break;
						case BESQL_TIPO_TEM:
						case MYSQL_TYPE_DATETIME:
						case MYSQL_TYPE_TIMESTAMP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMTEM, sValor);
							break;
						case BESQL_TIPO_FONx:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFONx, sValor);
							break;
						case BESQL_TIPO_FON:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMFON, sValor);
							break;
						case BESQL_TIPO_CNPJ:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCNPJ, sValor);
							break;
						case BESQL_TIPO_CPF:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCPF, sValor);
							break;
						case BESQL_TIPO_CEP:
							g_string_append_printf (sSQL, "`%s` = `%s`.%s('%s'), ", Campo->sNome, Sintaxe->Tabela->BD->sNome, BESQL_MASCARA_RMCEP, sValor);
							break;
						default:
							g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
					}
				}
				else
					g_string_append_printf (sSQL, "`%s` = '%s', ", Campo->sNome, sValor);
			}
			else
				g_string_append_printf (sSQL, "`%s` = NULL, ", Campo->sNome);
			nCont++;
		}
	}
	sSQL = g_string_truncate (sSQL, sSQL->len-2);//excluir a última vírgula e espaço
	g_string_append_printf (sSQL, " WHERE `ID` = '%s'", sID);
	BE_free (sTemp);
	sTemp = BE_string_free (sSQL);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sTemp);
#endif
	if (nCont > 0)
	{
        _be_sql_saida (sTemp);//depuração
		//excecutar consulta
		if (mysql_query (SQL->_my, sTemp) == 0)
		{
			lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
			//erro no retorno
			if (lAfetados < 0)
				_be_sql_erro_alteracao (SQL, sErros);
		}
		//erro na consulta
		else
		{
			_be_sql_erro_consulta (SQL, sErros, sTemp);
		}
	}

	BE_free (sTemp);
	BE_free (sValor);
	_be_sql_campo_valores_limpar (Sintaxe);

	if (lAfetados == 1)
		return TRUE;
	else
		return FALSE;
}

gboolean be_sql_excluir_CIA (BESQL_Sintaxe *Sintaxe, const gchar *sID, gchar **sErros)
{
	BESQL *SQL = NULL;
	gchar *sSQL = NULL;
	glong lAfetados = 0;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return FALSE;
	//verificar filtro
	if (!sID)
		return FALSE;
	//verificar se existe as tabelas relacionadas
	if (!Sintaxe->vis_Tabela || !Sintaxe->exc_Tabela || !Sintaxe->mex_Tabela)
		return FALSE;
	//verificar tipo de acesso
	if (!(Sintaxe->mex_Tabela->nFlags & BESQL_ACESSO_ALTERAR) || !(Sintaxe->mex_Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return FALSE;
	//verificar tipo de acesso
	if (!(Sintaxe->exc_Tabela->nFlags & BESQL_ACESSO_APAGAR) || !(Sintaxe->exc_Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return FALSE;
	//verificar tabela original
	if (!Sintaxe->vis_Tabela->sNomeReal)
		return FALSE;

	//comunicação
	SQL = Sintaxe->Tabela->BD->SQL;

	//marcar para excluir
	BE_free (sSQL);
	sSQL = g_strdup_printf ("UPDATE `%s`.`%s` SET `Data` = '0000-00-00 00:00:00' WHERE `ID` = '%s'", Sintaxe->mex_Tabela->BD->sNome, Sintaxe->mex_Tabela->sNome, sID);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	//excluir
	BE_free (sSQL);
	sSQL = g_strdup_printf ("DELETE FROM `%s`.`%s` WHERE `ID` = '%s'", Sintaxe->exc_Tabela->BD->sNome, Sintaxe->exc_Tabela->sNome, sID);
#ifdef BESQL_DEPURACAO
	printf ("DEPURACAO: %s:%d - %s\n", __FILE__, __LINE__, sSQL);
#endif
    _be_sql_saida (sSQL);//depuração
	//excecutar consulta
	if (mysql_query (SQL->_my, sSQL) == 0)
	{
		lAfetados = (glong) mysql_affected_rows (SQL->_my);//recebe quantidade de alterações
		//erro no retorno
		if (lAfetados < 0)
			_be_sql_erro_alteracao (SQL, sErros);
	}
	//erro na consulta
	else
	{
		_be_sql_erro_consulta (SQL, sErros, sSQL);
	}

	BE_free (sSQL);

	if (lAfetados == 1)
		return TRUE;
	else
		return FALSE;
}

void _be_sql_campo_valores_limpar (BESQL_Sintaxe *Sintaxe)
{
	BESQL_Indice *Indice = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;

	//campos
	for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Campo = (BESQL_Campo *) ListaL->data;
		Campo->sValor = NULL;
		BE_free (Campo->sValorNovo);
		Campo->bAfetar = FALSE;
		Campo->nComprimento = 0;
	}
	//índices
	for (ListaL = Sintaxe->Indices; ListaL; ListaL = g_slist_next (ListaL))
	{
		Indice = (BESQL_Indice *) ListaL->data;
		BE_free (Indice->sValorFiltro);
		Indice->nComprimentoFiltro = 0;
		Indice->bAfetar = FALSE;
	}
}

gchar *be_sql_campo_valor_limitado (BESQL_Campo *Campo, gint nComprimentoMax)
{
	gchar *sValor = NULL;

	//cópia
	if (nComprimentoMax == -1)
	{
		sValor = g_strdup (Campo->sValor);
	}
	else
	{
		//tamanhao mínimo
		if (nComprimentoMax < 5)
			nComprimentoMax = 5;
		nComprimentoMax++;// /0
		//limitar comprimento da string
		if (Campo)
		{
			if (Campo->sValor)
			{
				if (Campo->nComprimento > nComprimentoMax)
				{
					sValor = (gchar *) g_malloc (nComprimentoMax * sizeof(gchar));
					g_strlcpy (sValor, Campo->sValor, nComprimentoMax);
					sValor[nComprimentoMax - 4] = '.';
					sValor[nComprimentoMax - 3] = '.';
					sValor[nComprimentoMax - 2] = '.';
					sValor[nComprimentoMax - 1] = '\0';
				}
				else
					sValor = g_strdup (Campo->sValor);
			}
		}
	}

	return sValor;
}

gchar *be_sql_campo_valor_limitado_formatado (BESQL_Campo *Campo, gint nComprimentoMax, BESQL_CAMPOS_LIMITAR_FORMATO nFlags)
{
	gchar *sValor = NULL, *sValorTemp = NULL;
	gboolean bUTF8 = FALSE;

	sValor = be_sql_campo_valor_limitado (Campo, nComprimentoMax);
	if (sValor)
	{
		//verificar necessidade de UTF8
		if (nFlags & BESQL_CAMPOS_LIMITAR_FORMATO_UTF8 || nFlags & BESQL_CAMPOS_LIMITAR_FORMATO_XML)
			bUTF8 = TRUE;
		//conversão para UTF8
		if (bUTF8)
		{
			sValorTemp = sValor;
			sValor = BE_ISO_UTF8 (sValorTemp);
			BE_free (sValorTemp);
		}
		//conversão para XML
		if (nFlags & BESQL_CAMPOS_LIMITAR_FORMATO_XML)
		{
			sValorTemp = sValor;
			sValor = g_markup_printf_escaped ("%s", sValorTemp);
			BE_free (sValorTemp);
		}
		//desfazer UTF8 desnecessário
		if (bUTF8 && !(nFlags & BESQL_CAMPOS_LIMITAR_FORMATO_UTF8))
		{
			sValorTemp = sValor;
			sValor = BE_UTF8_ISO (sValorTemp);
			BE_free (sValorTemp);
		}
	}

	return sValor;
}

gchar *be_sql_perfil_ler (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, gchar **sErros)
{
    BESQL_Campo *CampoPerfil = NULL;

    //sair
    if (!SintaxePerfil || !sNome || !sVariavel)
        return NULL;

    //limpar filtro atual
    be_sql_filtro_limpar (SintaxePerfil, FALSE);
    //aplicar filtros
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 0);//Nome
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sNome, BESQL_RELACAO_E, 0, FALSE);
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 1);//Variavel
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sVariavel, BESQL_RELACAO_E, 0, FALSE);
    //executar consulta
    if (be_sql_selecionar (SintaxePerfil, 1, sErros) > 0 && be_sql_selecionar_proxima_linha (SintaxePerfil) > 0)
    {
        CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 2);//Valor
        if (CampoPerfil && CampoPerfil->sValor && strlen (CampoPerfil->sValor) > 0)
        {
            //retornar
            return g_strdup (CampoPerfil->sValor);
        }
    }

    return NULL;
}

GKeyFile *be_sql_perfil_ler_ini (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, gchar **sErros)
{
    GKeyFile *Ini = NULL;
    gchar *sTemp = NULL;

    //recuperar valor
    sTemp = be_sql_perfil_ler (SintaxePerfil, sNome, sVariavel, sErros);
    if (sTemp)
    {
        //preparar ini
        Ini = g_key_file_new ();
        //recuperar valor
        if (g_key_file_load_from_data (Ini, sTemp, strlen (sTemp), G_KEY_FILE_NONE, NULL))
        {
            //printf ("%s\n", g_key_file_to_data (Ini, NULL, NULL));
            //retornar
            BE_free (sTemp);
            return Ini;
        }
    }

	//limpar
	if (Ini)
	{
        BE_key_file_free (Ini);
	}
	BE_free (sTemp);

    return NULL;
}

gboolean be_sql_perfil_salvar (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, const gchar *sValor, gboolean bPublico, gchar **sErros)
{
    BESQL_Campo *CampoPerfil = NULL;

    //sair
    if (!SintaxePerfil || !sNome || !sVariavel || !sValor)
        return FALSE;

    //limpar filtro atual
    be_sql_filtro_limpar (SintaxePerfil, FALSE);
    //aplicar filtros
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 0);//Nome
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sNome, BESQL_RELACAO_E, 0, FALSE);
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 1);//Variavel
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sVariavel, BESQL_RELACAO_E, 0, FALSE);

    //preparar substituição de perfil
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 0);//Nome
    CampoPerfil->sValorNovo = g_strdup (sNome);
    CampoPerfil->bAfetar = TRUE;
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 1);//variável
    CampoPerfil->sValorNovo = g_strdup (sVariavel);
    CampoPerfil->bAfetar = TRUE;
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 2);//Valor
    CampoPerfil->sValorNovo = g_strdup (sValor);
    CampoPerfil->bAfetar = TRUE;
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 3);//Público
    CampoPerfil->sValorNovo = g_strdup (bPublico ? "1" : "0");
    CampoPerfil->bAfetar = TRUE;
    //executar consulta
    return be_sql_substituir (SintaxePerfil, sErros) > 0 ? TRUE : FALSE;
}

gboolean be_sql_perfil_salvar_ini (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, GKeyFile *Valor, gboolean bPublico, gchar **sErros)
{
    gchar *sTemp = NULL;
    gboolean bSalvar = FALSE;

    //sair
    if (!Valor)
        return FALSE;
    //valor
    sTemp = g_key_file_to_data (Valor, NULL, NULL);
    //executar consulta
    bSalvar = be_sql_perfil_salvar (SintaxePerfil, sNome, sVariavel, sTemp, bPublico, sErros);
    //limpar
    BE_free (sTemp);

    return bSalvar;
}

gboolean be_sql_perfil_excluir (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, const gchar *sVariavel, gchar **sErros)
{
    BESQL_Campo *CampoPerfil = NULL;

    //sair
    if (!SintaxePerfil || !sNome || !sVariavel)
        return FALSE;

    //limpar filtro atual
    be_sql_filtro_limpar (SintaxePerfil, FALSE);
    //aplicar filtros
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 0);//Nome
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sNome, BESQL_RELACAO_E, 0, FALSE);
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 1);//Variavel
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sVariavel, BESQL_RELACAO_E, 0, FALSE);

    return be_sql_excluir (SintaxePerfil, sErros) > 0 ? TRUE : FALSE;
}

GSList *be_sql_perfil_variaveis (BESQL_Sintaxe *SintaxePerfil, const gchar *sNome, gchar **sErros)
{
    BESQL_Campo *CampoPerfil = NULL;
    GSList *Lista = NULL;

    //sair
    if (!SintaxePerfil || !sNome)
        return NULL;

    //limpar filtro atual
    be_sql_filtro_limpar (SintaxePerfil, FALSE);
    //aplicar filtros
    CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 0);//Nome
    be_sql_filtro_adicionar (SintaxePerfil, CampoPerfil, BESQL_COMPARACAO_IGUAL, sNome, BESQL_RELACAO_E, 0, FALSE);
    //executar consulta
    if (be_sql_selecionar (SintaxePerfil, 1, sErros))
    {
        //loop
        while (be_sql_selecionar_proxima_linha (SintaxePerfil))
        {
            CampoPerfil = (BESQL_Campo *) g_slist_nth_data (SintaxePerfil->Campos, 1);//Variavel
            if (CampoPerfil && CampoPerfil->sValor && strlen (CampoPerfil->sValor) > 0)
            {
                //adicionar na lista
                Lista = g_slist_append (Lista, g_strdup (CampoPerfil->sValor));
            }
        }
        return Lista;
    }

	//limpar
	return NULL;
}

gint be_sql_executar (
    BESQL *SQL,
    const gchar *sSintaxe,
    gboolean (*func_Ret)(BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data),
    gpointer user_data,
    gchar **sErros
)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	MYSQL_FIELD *field;
	BESQL_Campo *Campo = NULL;
	GSList *Campos = NULL, *ListaL = NULL;
	gulong *pLen = NULL;
	gint nLinha = -1, nCampo = 0;
	gboolean bContinuar = TRUE;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return 0;

	//callback
	if (SQL->func_Info)
		SQL->func_Info (SQL, 0, NULL, SQL->user_data);

    //depuração
    _be_sql_saida (sSintaxe);
    //executar consulta
	if (mysql_query (SQL->_my, sSintaxe) == 0)
	{
		//processar múltiplas consultas
		do
		{
			//verificar tipo de consulta
			if (mysql_field_count (SQL->_my) > 0)
			{
				//SELECT, SHOW, DESCRIBE
				if ((res = mysql_store_result (SQL->_my)))//recebe a consulta
				{
					//loop nas linhas
					for (nLinha = 0; nLinha < mysql_num_rows (res) && bContinuar; nLinha++)
					{
						//recuperar campo
						row = mysql_fetch_row (res);
						//nomes dos campos
						if (nLinha == 0)
						{
							//loop nos campos
							for (nCampo = 0; nCampo < mysql_num_fields (res); nCampo++)
							{
								field = mysql_fetch_field_direct (res, nCampo);
								//criar campos
								Campo = g_new (BESQL_Campo, 1);
								Campo->Sintaxe = NULL;
								Campo->sNome = g_strdup (field->name);
								Campo->nCampoPos = nCampo;//disponível somente após uma execução de uma consulta
								Campo->sValor = NULL;//disponível somente após uma execução de uma consulta
								Campo->bAnulavel = field->flags & NOT_NULL_FLAG ? TRUE : FALSE;
								Campo->bAlteravel = FALSE;
								Campo->nComprimentoMax = (gint) field->length;
								Campo->nComprimentoTop = 0;
								Campo->bAtivo = TRUE;
								Campo->sTipo = NULL;
								Campo->nTipo = (gint) field->type;
								Campo->bAutoID = field->flags & AUTO_INCREMENT_FLAG ? TRUE : FALSE;;
								Campo->sParametros = NULL;//parâmetros extra
								Campo->sGrupo = NULL;//grupo extra
								Campo->sComentarios = NULL;//comentários extra
								Campo->sValorPadrao = NULL;
								Campo->sTotal = NULL;
								Campo->sValorNovo = NULL;
								Campo->nComprimentoNovo = 0;
								Campo->bAfetar = FALSE;
								//anexando na lista
								Campos = g_slist_append (Campos, Campo);
							}
						}
						//comprimento
						pLen = mysql_fetch_lengths (res);
						//loop nos campos
						for (nCampo = 0, ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL), nCampo++)
						{
							//valores atuais
							Campo = (BESQL_Campo *) ListaL->data;
							Campo->sValor = row[nCampo];//adicionar valores
							Campo->nComprimento = pLen[nCampo];
						}
						//retornar campos, por callback
						if (func_Ret)
							bContinuar = func_Ret (SQL, nLinha, Campos, user_data);
					}
					mysql_free_result (res);

					//limpar campos
					for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
					{
						Campo = (BESQL_Campo *) ListaL->data;
						BE_free (Campo->sNome);
						BE_free (Campo->sTipo);
						BE_free (Campo->sParametros);
						BE_free (Campo->sGrupo);
						BE_free (Campo->sComentarios);
						BE_free (Campo->sValorPadrao);
						BE_free (Campo->sValorNovo);
						BE_free (Campo->sTotal);
						BE_free (Campo);
					}
					BE_slist_free (Campos);
				}
				else//erro no retorno
					_be_sql_erro_retorno (SQL, sErros);
			}
			//INSERT, UPDATE, DELETE
			else
				nLinha = (gint) mysql_affected_rows (SQL->_my);
		//continuar processamento se houver mais consultas
		} while (!mysql_next_result (SQL->_my));
	}
	else//erro na consulta
	{
		_be_sql_erro_consulta (SQL, sErros, sSintaxe);
	}

    //verificar avisos (warnings)
    _be_sql_aviso_retorno (SQL, sErros);

    return nLinha;
}

gboolean be_sql_informacoes_callback (BESQL *SQL, gint nLinhas, GSList *Campos, gpointer user_data)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	gchar **sTemp = NULL;

	if (user_data)
	{
        //recuperar nome do campo
        sTemp = (gchar **) user_data;
        //loop nas campos e valores
        for (ListaL = Campos; ListaL; ListaL = g_slist_next (ListaL))
        {
            Campo = (BESQL_Campo *) ListaL->data;
            //verificar campo
            if (g_str_equal (*sTemp, Campo->sNome) || strlen (*sTemp) == 0)
            {
                //limpar comparação
                BE_free (*sTemp);
                *sTemp = g_strdup (Campo->sValor);
                break;
            }
        }
	}

    return FALSE;//somente a primeira linha
}

gchar *be_sql_informacoes (BESQL *SQL, BESQL_INFO_ENUM nOpcoes, gchar **sErros)
{
	gchar *sSQL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	gint nInicio, nFim;

	//verificar para não continuar com erros
	if (sErros && *sErros)
		return NULL;

	switch (nOpcoes)
	{
		//MySQL-refman-5.0-en.a4.pdf pág. 1104
		case BESQL_INFO_SERVIDOR_IP:
			if (SQL->sIP)
				sTemp = g_strdup (SQL->sIP);
			else
				sTemp = g_strdup ("localhost");
			break;
		case BESQL_INFO_SERVIDOR_VERSAO:
			//MYSQL_SERVER_VERSION
			sTemp = g_strdup (mysql_get_server_info (SQL->_my));//5.0.18-Debian_7-log, mysql_get_server_version(SQL->_my)
			break;
		case BESQL_INFO_SERVIDOR_PORTA://porta
            sTemp = g_strdup ("Value");//nome do campo para retornar valor
            be_sql_executar (SQL, "SHOW VARIABLES LIKE 'port'", be_sql_informacoes_callback, &sTemp, sErros);
			break;
		case BESQL_INFO_CLIENTE_IP:
			sTemp = g_strdup (mysql_get_host_info (SQL->_my));//Localhost via UNIX socket
			//sTemp = g_strdup ("Host");//nome do campo para retornar valor
			//be_sql_executar (SQL, "SHOW PROCESSLIST", be_sql_informacoes_callback, &sTemp, sErros);
			break;
		case BESQL_INFO_CLIENTE_VERSAO:
			sTemp = g_strdup (mysql_get_client_info ());//5.0.18, mysql_get_client_version()
			break;
		case BESQL_INFO_PROTOCOLO:
			sTemp = g_strdup_printf ("%d", (gint) mysql_get_proto_info (SQL->_my));//10
			break;
		case BESQL_INFO_CONEXAO://pág 1130
			sTemp = g_strdup_printf ("%d", (gint) mysql_thread_id (SQL->_my));//25
			break;
		case BESQL_INFO_USUARIO://usuário
			sTemp = g_strdup ("I");//nome do campo para retornar valor
			be_sql_executar (SQL, "SELECT USER() AS I", be_sql_informacoes_callback, &sTemp, sErros);
			break;
		case BESQL_INFO_FUSO_HORARIO:
			sTemp1 = g_strdup ("Value");//nome do campo para retornar valor
			be_sql_executar (SQL, "SHOW VARIABLES LIKE 'time_zone'", be_sql_informacoes_callback, &sTemp1, sErros);

			sTemp2 = g_strdup ("I");//nome do campo para retornar valor
			be_sql_executar (SQL, "SELECT UTC_TIMESTAMP() AS I", be_sql_informacoes_callback, &sTemp2, sErros);

			if (sTemp1 && sTemp2)
                sTemp = g_strdup_printf ("Local: %s - UTC: %s", sTemp1, sTemp2);
			break;
		case BESQL_INFO_DIFERENCA_HORARIO://diferença de minutos, entre cliente e servidor (minutos)
			sTemp1 = BE_tempo ("%Y-%m-%d %H:%M:%S");
			sSQL = g_strdup_printf ("SELECT (TIMESTAMPDIFF(SECOND, '%s', NOW())/60) AS I", sTemp1);

			sTemp = g_strdup ("I");//nome do campo para retornar valor
			be_sql_executar (SQL, sSQL, be_sql_informacoes_callback, &sTemp, sErros);

			break;
		case BESQL_INFO_DESEMPENHO://desempenho, em milisegundos
			nInicio = be_tempo_usec ();
			be_sql_executar (SQL, "SELECT BENCHMARK(100000,ENCODE('hello','goodbye'))", be_sql_informacoes_callback, NULL, sErros);
			nFim = be_tempo_usec ();
			sTemp = g_strdup_printf ("%.3f", (gfloat) (nFim - nInicio) / 1000);
			break;
		case BESQL_INFO_LINGUAGEM://linguagem
			sTemp1 = g_strdup ("I");//nome do campo para retornar valor
			be_sql_executar (SQL, "SELECT COLLATION('abc') AS I", be_sql_informacoes_callback, &sTemp1, sErros);

			sTemp2 = g_strdup ("Value");//nome do campo para retornar valor
			be_sql_executar (SQL, "SHOW VARIABLES LIKE 'language'", be_sql_informacoes_callback, &sTemp2, sErros);

			if (sTemp1 && sTemp2)
				sTemp = g_strdup_printf ("%s %s", sTemp1, sTemp2);
			break;
		case BESQL_INFO_COMPILACAO://compilação máquina
			sTemp1 = g_strdup ("Value");//nome do campo para retornar valor
			be_sql_executar (SQL, "SHOW VARIABLES LIKE 'version_compile_machine'", be_sql_informacoes_callback, &sTemp1, sErros);

			sTemp2 = g_strdup ("Value");//nome do campo para retornar valor
			be_sql_executar (SQL, "SHOW VARIABLES LIKE 'version_compile_os'", be_sql_informacoes_callback, &sTemp2, sErros);

			if (sTemp1 && sTemp2)
				sTemp = g_strdup_printf ("%s %s", sTemp1, sTemp2);
			break;
		case BESQL_INFO_EXPIRACAO://timeout
			sTemp = g_strdup ("Value");//nome do campo para retornar valor
			be_sql_executar (SQL, "SHOW VARIABLES LIKE 'wait_timeout'", be_sql_informacoes_callback, &sTemp, sErros);
			break;
		case BESQL_INFO_STATUS://pág 1130
			sTemp = g_strdup (mysql_stat (SQL->_my));//Uptime: 2461  Threads: 1  Questions: 4781  Slow queries: 0  Opens: 0  Flush tables: 1  Open tables: 17  Queries per second avg: 1.943
			break;
		case BESQL_INFO_ULTIMO_ID://pág 1108
			sTemp = g_strdup_printf ("%d", (gint) mysql_insert_id (SQL->_my));
			break;
        case BESQL_INFO_ALTERACAO://pág 1107
			sTemp = g_strdup (mysql_info (SQL->_my));
			break;
		case BESQL_INFO_AVISO://avisos, não é um erro
			sTemp = g_strdup ("Message");//nome do campo para retornar valor
			be_sql_executar (SQL, "SHOW WARNINGS", be_sql_informacoes_callback, &sTemp, sErros);
			break;
		case BESQL_INFO_SEGURO://verificar se usuário atual está numa conexão segura
			sTemp = g_strdup ("");//retornar o valor do primeiro campo
			be_sql_executar (SQL, "SHOW GRANTS", be_sql_informacoes_callback, &sTemp, sErros);
			sTemp1 = g_ascii_strup (sTemp, -1);//tudo em maiúsculo
			BE_free (sTemp);
			if (be_strpos (sTemp1, "SSL", 0) >= 0 || be_strpos (sTemp1, "ISSUER", 0) >= 0 || be_strpos (sTemp1, "SUBJECT", 0) >= 0 || be_strpos (sTemp1, "CIPHER", 0) >= 0)
                sTemp = g_strdup ("Conexão Segura");
			break;
	}

	BE_free (sSQL);
	BE_free (sTemp1);
	BE_free (sTemp2);

	return sTemp;
}

gboolean be_sql_exportar_dados (BESQL_Sintaxe *Sintaxe, const gchar *sArquivo, const gchar *sNome, BESQL_EXPORTAR nFormato, gchar **sErros)
{
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL;
	glong lLinha = 0, lLinhas = 0;
	gint nPaginaDados = 1, nRegistros = 0;
	FILE *File = NULL;
	gboolean bOK = FALSE;

	//verificar
	if (!Sintaxe || !sArquivo)
		return bOK;
	//verificar para não continuar com erros
	if (sErros && *sErros)
		return bOK;
	//verificar tipo de acesso
	if (!(Sintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
		return bOK;

	//callback
	if (Sintaxe->Tabela->BD->SQL->func_Info)
		Sintaxe->Tabela->BD->SQL->func_Info (Sintaxe->Tabela->BD->SQL, 0, NULL, Sintaxe->Tabela->BD->SQL->user_data);

	//Abrindo arquivo: Criando ou sobreescrevendo existente
	if ((File = fopen (sArquivo, "wb")))
	{
		if (nFormato == BESQL_EXPORTAR_HTML)
		{
			//início, cabeçalho
			BE_free (sTemp);
			sTemp = g_strdup_printf ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd>\n");
			fwrite (sTemp, strlen (sTemp), 1, File);

			//início
			BE_free (sTemp);
			sTemp = g_strdup_printf ("<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n<meta name=\"Author\" content=\"%s %s\">\n<title>%s</title>\n</head>\n<body>\n", INTERNAL_NAME, PRODUCT_VERSION, sNome);
			fwrite (sTemp, strlen (sTemp), 1, File);
			//tabela
			BE_free (sTemp);
			sTemp = g_strdup_printf ("<table border='1' align='center' valign='middle'>\n<caption>%s</caption>\n", sNome);
			fwrite (sTemp, strlen (sTemp), 1, File);
			//alterar página
			if (Sintaxe)
				Sintaxe->lLinhasPaginaMax = BESQL_MAX_LINHAS_POR_PAGINA;
			//loop nos dados
			do
			{
				lLinhas = be_sql_selecionar (Sintaxe, nPaginaDados, sErros);
				while (lLinhas > 0 && (lLinha = be_sql_selecionar_proxima_linha (Sintaxe)))
				{
					//cabeçalho
					if (nRegistros == 0)
					{
						//loop nos campos
						for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
						{
							Campo = (BESQL_Campo *) ListaL->data;
							if (Campo->sNome)
							{
								BE_free (sTemp);
								sTemp = g_strdup_printf ("<th>%s</th>\n", Campo->sNome);
								fwrite (sTemp, strlen (sTemp), 1, File);
							}
						}
					}
					//loop nos campos
					fwrite ("<tr>\n", strlen ("<tr>\n"), 1, File);
					for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
					{
						Campo = (BESQL_Campo *) ListaL->data;
						//dados
						BE_free (sTemp);
						if (Campo->sValor)
							sTemp = g_strdup_printf ("\t<td>%s</td>\n", Campo->sValor);
						else
							sTemp = g_strdup_printf ("\t<td>%s</td>\n", "");
						fwrite (sTemp, strlen (sTemp), 1, File);
					}
					fwrite ("</tr>\n", strlen ("</tr>\n"), 1, File);

					nRegistros++;
				}
				nPaginaDados++;
			} while (lLinhas > 0 && lLinhas >= BESQL_MAX_LINHAS_POR_PAGINA);
			//fim
			BE_free (sTemp);
			sTemp = g_strdup_printf ("</table>\n</body>\n</html>");
			fwrite (sTemp, strlen (sTemp), 1, File);
			//informar
			bOK = TRUE;
		}
		//fechar arquivo
		fclose (File);
	}

	//limpar
	BE_free (sTemp);

	return bOK;
}
