#include "bemonitor.h"

gboolean be_monitor (const gchar *sArquivo, void (*func_Info)(const gchar *sMensagens, gpointer user_data), gpointer user_data)
{
	FILE *File = NULL;
	GKeyFile *Ini = NULL;
	BESQL *SQL = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	gchar cEntrada = '\0', sErro[] = {"No symbol table info available"};//frase para constatar o erro
	gchar *sTemp = NULL, *sIP = NULL, *sVersao = NULL, *sChavePrivada = NULL, *sCertificado = NULL, *sAutoridade = NULL, *sCaminhoAutoridade = NULL, *sCifras = NULL;
	gchar *sArquivoErro = NULL, *sArquivoSQL = NULL, *sArquivoSuporte = NULL;
	guint nPorta = 0;
	gint nErro = 0;
	gboolean bReiniciar = FALSE;

    //preparar configuração local
    BE_free (sTemp);
    sTemp = be_diretorio_local (TRUE);
    sArquivoSuporte = g_strdup_printf ("%s%s", sTemp, BEMONITOR_CONEXAO_SERVIDOR);
    sArquivoSQL = g_strdup_printf ("%s%s", sTemp, BEMONITOR_ARQ_SQL);
    //preparar temporário
    BE_free (sTemp);
    sTemp = be_diretorio_local_temp (TRUE);
    sArquivoErro = g_strdup_printf ("%s%s", sTemp, BEMONITOR_ARQ_ERRO);
    //verificar se arquivo suporte existe
    if (!g_file_test (sArquivoSuporte, G_FILE_TEST_EXISTS))
    {
        //copiar de uma versão local
        if (g_file_test (BEMONITOR_CONEXAO_SERVIDOR, G_FILE_TEST_EXISTS))
        {
            BE_free (sTemp);
            #ifdef G_OS_UNIX
                sTemp = g_strdup_printf ("cp %s %s", BEMONITOR_CONEXAO_SERVIDOR, sArquivoSuporte);
            #endif
            #ifdef G_OS_WIN32
                sTemp = g_strdup_printf ("cp %s \"%s\"", BEMONITOR_CONEXAO_SERVIDOR, sArquivoSuporte);
            #endif
            system (sTemp);
        }
    }

	//chamar callback
	if (func_Info)
		func_Info ("Verificando erros anteriores...", user_data);
	else
		printf ("Verificando erros anteriores...\n");

	//procurar por erros
	if (sArquivo && (File = fopen (sArquivo, "rb")))
	{
		while ((cEntrada = fgetc (File)) != EOF)
		{
			//verificar erro
			if (nErro < strlen (sErro))
			{
				if (sErro[nErro] == cEntrada)
					nErro++;
				else
					nErro = 0;
			}
		}
		//fechar arquivo
		fclose (File);
	}

	//abrir INI
	Ini = g_key_file_new ();

	//localizar servidor e versão atual
	if (g_key_file_load_from_file (Ini, sArquivoSuporte, G_KEY_FILE_NONE, NULL))
	{
		//servidor
		sIP = g_key_file_get_string (Ini, "servidor", "ip", NULL);
		if (!sIP || strlen (sIP) == 0)
		{
			BE_free (sIP);
		}
		sTemp = g_key_file_get_string (Ini, "servidor", "porta", NULL);
		if (sTemp)
			nPorta = atoi (sTemp);
        //versão
		sVersao = g_key_file_get_string (Ini, "cliente", "versao", NULL);
		//conexão segura SSL
		sChavePrivada = g_key_file_get_string (Ini, "seguranca", "chaveprivada", NULL);
		if (sChavePrivada && strlen (sChavePrivada) == 0)
		{
		    BE_free (sChavePrivada);
		}
		BE_free (sTemp);
		sTemp = sChavePrivada;
		sChavePrivada = be_arquivo_absoluto (sTemp);

		sCertificado = g_key_file_get_string (Ini, "seguranca", "certificado", NULL);
		if (sCertificado && strlen (sCertificado) == 0)
		{
		    BE_free (sCertificado);
		}
		BE_free (sTemp);
		sTemp = sCertificado;
		sCertificado = be_arquivo_absoluto (sTemp);

		sAutoridade = g_key_file_get_string (Ini, "seguranca", "autoridade", NULL);
		if (sAutoridade && strlen (sAutoridade) == 0)
		{
		    BE_free (sAutoridade);
		}
		BE_free (sTemp);
		sTemp = sAutoridade;
		sAutoridade = be_arquivo_absoluto (sTemp);

		sCaminhoAutoridade = g_key_file_get_string (Ini, "seguranca", "caminhoautoridade", NULL);
		if (sCaminhoAutoridade && strlen (sCaminhoAutoridade) == 0)
		{
		    BE_free (sCaminhoAutoridade);
		}
		BE_free (sTemp);
		sTemp = sCaminhoAutoridade;
		sCaminhoAutoridade = be_arquivo_absoluto (sTemp);

		sCifras = g_key_file_get_string (Ini, "seguranca", "cifras", NULL);
		if (sCifras && strlen (sCifras) == 0)
		{
		    BE_free (sCifras);
		}
	}
	if (!sVersao)
		sVersao = g_strdup ("0");

	//conectar ao servidor
	SQL = be_sql_abrir_conexao_segura (BEMONITOR_USUARIO_SUPORTE, BEMONITOR_SENHA_SUPORTE, sIP, nPorta, -1, NULL, sChavePrivada, sCertificado, sAutoridade, sCaminhoAutoridade, sCifras);

	//verificar se houve erro
	if (nErro == strlen (sErro))
	{
		//adicionar mais informações
		#ifdef G_OS_UNIX
			BE_free (sTemp); sTemp = g_strdup_printf ("cp %s %s", sArquivo, sArquivoErro); system (sTemp);//gerar arquivo
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("uname -a >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("date >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo UpTime: >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("uptime >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("dmesg >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("ifconfig >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("who -u -H >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo Hostname: >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("hostname >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("free -t >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("echo >> %s", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("cat %s >> %s", sArquivoSQL, sArquivoErro); system (sTemp);
		#endif
		#ifdef G_OS_WIN32
			BE_free (sTemp); sTemp = g_strdup_printf ("cp \"%s\" \"%s\"", sArquivo, sArquivoErro); system (sTemp);//gerar arquivo
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("uname -a >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("VER  >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gdate >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("IPCONFIG >> \"%s\"", sArquivoErro); system (sTemp);
            BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("who -u -H >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho Hostname: >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("hostname >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("gecho >> \"%s\"", sArquivoErro); system (sTemp);
			BE_free (sTemp); sTemp = g_strdup_printf ("cat \"%s\" >> \"%s\"", sArquivoSQL, sArquivoErro); system (sTemp);
		#endif
		//enviar o erro
		if (SQL)
		{
			Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "suporte", NULL), "erros"), FALSE, NULL);
			if (Sintaxe)
			{
				Campo = be_sql_selecionar_campo (Sintaxe, "Conteudo");
				if (Campo)
				{
					if (be_sql_inserir_arquivo (Campo, sArquivoErro, 'B'))
					{
						Campo->bAfetar = TRUE;
						if (be_sql_adicionar (Sintaxe,  NULL) > 0)
						{
							//apagar o arquivo com erro
							#ifdef G_OS_UNIX
								BE_free (sTemp); sTemp = g_strdup_printf ("unlink %s", sArquivo); system (sTemp);
							#endif
							#ifdef G_OS_WIN32
								BE_free (sTemp); sTemp = g_strdup_printf ("DEL \"%s\"", sArquivo); system (sTemp);
							#endif
							//mensagem informativa
							BE_free (sTemp);
							sTemp = g_strdup_printf ("Erros anteriores foram detectados.\nPor favor contate seu fornecedor para correção do mesmo.\n\n%s", VER_STRING);
							be_Mensagens (NULL, FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
						}
					}
				}
				BE_sql_fechar_consulta (Sintaxe);//fechar
			}
		}
	}

	//chamar callback
	if (func_Info)
		func_Info ("Verificando atualizações...", user_data);
	else
		printf ("Verificando atualizacaoes...\n");

	//verificar atualizações
	if (SQL)
	{
		Sintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (be_sql_selecionar_bd (SQL, "suporte", NULL), "atualizacao"), FALSE, NULL);
		if (Sintaxe)
		{
			//localizar versão necessária
			Campo = be_sql_selecionar_campo (Sintaxe, "Versao");
			if (Campo)
			{
				be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_MAIOR, sVersao, BESQL_RELACAO_OU, 0, FALSE);

				if (be_sql_selecionar (Sintaxe, 1, NULL) > 0)
				{
					//mensagem informativa
					BE_free (sTemp);
					sTemp = g_strdup ("Há atualizações disponíveis.\nDeseja executar agora ?");
					if (be_Mensagens (NULL, FALSE, sTemp, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO) == GTK_RESPONSE_YES)
					{
						//loop nos dados
						while (be_sql_selecionar_proxima_linha (Sintaxe))
						{
							//baixar conteúdo
							Campo = be_sql_selecionar_campo (Sintaxe, "Conteudo");
							be_sql_recuperar_arquivo (Campo, Campo->sValor, 'B');
							//executar ação
							Campo = be_sql_selecionar_campo (Sintaxe, "Acao");
							if (Campo->sValor)
								system (Campo->sValor);
							//marcar nova versão
							Campo = be_sql_selecionar_campo (Sintaxe, "Versao");
							if (Campo->sValor && (File = fopen (sArquivoSuporte, "wb")))
							{
								g_key_file_set_string (Ini, "cliente", "versao", Campo->sValor);
								BE_free (sTemp);
								sTemp = g_key_file_to_data (Ini, NULL, NULL);
								fwrite (sTemp, strlen (sTemp), 1, File);
								//fechando arquivo
								fclose (File);
							}
						}
						//mensagem informativa
						BE_free (sTemp);
						sTemp = g_strdup_printf ("Atualizações executadas com sucesso.\nO aplicativo será finalizado agora.\n\n%s", VER_STRING);
						be_Mensagens (NULL, FALSE, sTemp, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
						bReiniciar = TRUE;
					}
				}
			}
			BE_sql_fechar_consulta (Sintaxe);//fechar
		}
	}
	//fechar conexão
	BE_sql_fechar_conexao (SQL);
	//fechar INI
	g_key_file_free (Ini);

	//chamar callback
	if (func_Info)
		func_Info ("Verificado", user_data);
	else
		printf ("Verificado\n");

	//apagar o arquivo de depuração de SQL
	#ifdef G_OS_UNIX
		BE_free (sTemp); sTemp = g_strdup_printf ("unlink %s", sArquivoSQL); system (sTemp);
	#endif
	#ifdef G_OS_WIN32
		BE_free (sTemp); sTemp = g_strdup_printf ("DEL \"%s\"", sArquivoSQL); system (sTemp);
	#endif

	BE_free (sTemp);
	BE_free (sIP);
	BE_free (sVersao);
	BE_free (sArquivoErro);
	BE_free (sArquivoSQL);
	BE_free (sArquivoSuporte);

	return bReiniciar;
}
