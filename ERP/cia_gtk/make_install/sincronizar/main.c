#include "besincronizar.h"

int main (int argc, char *argv[])
{
/*******************************************************************************
Funcionamento é somente através de linha de comando.
*******************************************************************************/
	BESQL *SQL = NULL;
	const gchar *sArquivo = NULL;
	gboolean bProgresso = FALSE;
	gboolean bLog = TRUE;
	gchar cTipo = 0;
	gint nCont = 0;
	gint nQt = 0;

	//verificar argumentos
	if (argc <= 2)
	{
		printf ("Falta especificar o arquivo de configuracao.\n");
		return 0;
	}
	//verificar opções de linha de comando
	for (nCont = 1; nCont < argc; nCont++)
	{
		if (g_str_equal (argv[nCont], "--progresso"))
			bProgresso = TRUE;
		else if (g_str_equal (argv[nCont], "--semlog"))
			bLog = FALSE;
		else if (g_strrstr_len (argv[nCont], 2, "-q"))
			nQt = atoi (argv[nCont]+2);
		else if (g_str_equal (argv[nCont], "--help") || g_str_equal (argv[nCont], "-h"))//informar opções disponíveis
		{
			printf ("Opcoes disponiveis:\n\n"
				"-n : odbc\n"
				"-e : net\n"
				"--progresso : Informa na tela o progresso do sincronismo. Deixa mais lento.\n"
				"--semlog : Desabilita o arquivo de Log. Deixa mais rapido.\n"
				"-qx : Quantidade x de arquivos que devem permanecer.\n"
				"\n"
			);
			return 0;
		}
	}
	//o primeiro parâmetro é o arquivo de configuração ou ajuda
	sArquivo = argv[1];
	if (!g_file_test (sArquivo, G_FILE_TEST_EXISTS))
	{
		printf ("Arquivo de configuracao %s inexistente.\n", sArquivo);
		return 0;
	}
	//o segundo parâmetro é o tipo
	if (g_str_equal (argv[2], "-n"))
		cTipo = 'n';
	else if (g_str_equal (argv[2], "-e"))
		cTipo = 'e';

	//inicializar biblioteca estendida
	be_global_abrir (&argc, &argv, 0);

	//iniciar log
	be_sincronizar_log_iniciar (nQt);
	printf ("Iniciar sincronismo\n");

	//conexão
	SQL = be_sincronizar_autenticar (sArquivo);
	//executar arquivo INI
	if (SQL && cTipo == 'n' && be_sincronizar_executar_odbc (SQL, sArquivo, bProgresso, bLog))
	{
		printf ("Sincronismo realizado com sucesso.\n");
	}
	else if (SQL && cTipo == 'e' && be_sincronizar_executar_net (SQL, sArquivo, bProgresso, bLog))
	{
		printf ("Sincronismo realizado com sucesso.\n");
	}
	else
	{
		BE_sincronizar_log_informar ("%s\n", "Ocorreram erros no decorrer do sincronismo.");
		printf ("Ocorreram erros no decorrer do sincronismo.\n");
	}
	//finalizar conexão atual
	if (SQL)
	{
		BE_sql_fechar_conexao (SQL);
	}
	else
	{
		BE_sincronizar_log_informar ("%s\n", "A conexao com o servidor falhou.");
		printf ("A conexao com o servidor falhou.\n");
	}

	//finalizar log
	be_sincronizar_log_fechar ();

	//finalizar biblioteca extendida
	be_global_fechar ();
	return 0;
}
