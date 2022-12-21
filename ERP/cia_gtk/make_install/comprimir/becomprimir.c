#include "becomprimir.h"

/*******************************************************************************
Contém funções para facilitar a integração da biblioteca zlib (http://www.zlib.net/) com o programa.
Compactador de arquivos e textos.
Para utilização com arquivos o ideal é utilizar o executável "gzip" (http://www.gzip.org/).
*******************************************************************************/

guchar *be_comprimir (const gchar *sString, gulong *lComprimento)
{
/*******************************************************************************
Comprimi um texto.
Argumentos:
    sString : texto original
    lComprimento : endereço do comprimento do texto original e do comprimido, ou zero para erros
    Retorno: novo texto comprimido, ou nulo
*******************************************************************************/
	gulong lSizeLen = *lComprimento < 1 ? strlen (sString) : *lComprimento;
	gulong lSize = compressBound (lSizeLen);
	guchar *sBuffer = NULL;

	//verificar comprimento inválido
	if (lSize > BEGLOBAL_TAMANHO_MAX_ARQUIVO)
	{
		BE_free (sBuffer);//limpar
		return NULL;
	}
	sBuffer = (guchar *) g_malloc (lSize * sizeof(guchar));//reservando memória
	if (compress (sBuffer, &lSize, (guchar *) sString, lSizeLen) != Z_OK)//comprimindo
	{
		*lComprimento = 0;//comprimento 0 para erros
		return NULL;//erro
	}
	*lComprimento = lSize;//repassar novo comprimento

	return sBuffer;
}

guchar *be_comprimir_nivel (const gchar *sString, gulong *lComprimento, gint nNivel)
{
/*******************************************************************************
Comprimi um texto, com opção de nível.
Argumentos:
    sString : texto original
    lComprimento : endereço do comprimento do texto original e do comprimido, ou zero para erros
    nNivel : 0 à 9, maior compressão menor o conteúdo final, porém mais lento
    Retorno: novo texto comprimido, ou nulo
*******************************************************************************/
	gulong lSizeLen = *lComprimento < 1 ? strlen (sString) : *lComprimento;
	gulong lSize = compressBound (lSizeLen);
	guchar *sBuffer = NULL;

	//verificar comprimento inválido
	if (lSize > BEGLOBAL_TAMANHO_MAX_ARQUIVO)
	{
		BE_free (sBuffer);//limpar
		return NULL;
	}
	sBuffer = (guchar *) g_malloc (lSize * sizeof(guchar));//reservando memória
	if (compress2 (sBuffer, &lSize, (guchar *) sString, lSizeLen, nNivel) != Z_OK)//comprimindo
	{
		*lComprimento = 0;//comprimento 0 para erros
		return NULL;//erro
	}
	*lComprimento = lSize;//repassar novo comprimento

	return sBuffer;
}

gchar *be_comprimir_de (const guchar *sString, gulong *lComprimento, gulong lComprimentoOri)
{
/*******************************************************************************
Comprimi um texto, com opção de nível.
Argumentos:
    sString : texto original
    lComprimento : endereço do comprimento do texto original e do comprimido, ou zero para erros
    lComprimentoOri : comprimento do texto original
    Retorno: novo texto comprimido, ou nulo
*******************************************************************************/
	gulong lSizeLen = *lComprimento;
	gulong lSize = lComprimentoOri;
	guchar *sBuffer = NULL;

	if (*lComprimento == 0)
		return NULL;
	//verificar comprimento inválido
	if (lSizeLen > BEGLOBAL_TAMANHO_MAX_ARQUIVO)
		return NULL;

	//reservando memória
	sBuffer = (guchar *) g_malloc (lSize * sizeof(guchar));
	//descomprimindo
	if (!sBuffer || uncompress (sBuffer, &lSize, sString, lSizeLen) != Z_OK)
	{
		*lComprimento = 0;
		if (sBuffer)
			BE_free (sBuffer);
		return NULL;//erro
	}
	//repassar novo comprimento
	*lComprimento = lSize;

	return (gchar *) sBuffer;
}

void be_comprimir_arquivo (const gchar *sArquivoOri, const gchar *sArquivoGZ)
{
/*******************************************************************************
Compacta um arquivo, em formato ZIP.
Esta função utiliza o executável "gzip", tanto da versão Linux como de Windows.
O nível de compressão é o máximo (9).
Argumentos:
    sArquivoOri : arquivo original
    sArquivoGZ : novo arquivo comprimido
    Retorno: não retorna
*******************************************************************************/
	gchar *sTemp = NULL;

	sTemp = g_strdup_printf ("gzip -c9 %s > %s", sArquivoOri, sArquivoGZ);//linha de comando para executar o gzip
	system (sTemp);//execução
	BE_free (sTemp);//limpar
}

void be_comprimir_arquivo_de (const gchar *sArquivoGZ, const gchar *sArquivoOri)
{
/*******************************************************************************
Descompacta um arquivo, em formato ZIP.
Esta função utiliza o executável "gzip", tanto da versão Linux como de Windows.
Argumentos:
    sArquivoGZ : novo arquivo comprimido
    sArquivoOri : arquivo original
    Retorno: não retorna
*******************************************************************************/
	gchar *sTemp = NULL;

	sTemp = g_strdup_printf ("gzip -cd %s > %s", sArquivoGZ, sArquivoOri);//linha de comando para executar o gzip
	system (sTemp);//execução
	BE_free (sTemp);//limpar
}

guchar *be_comprimir_arquivo_string (const gchar *sArquivo, gulong *lComprimento)
{
/*******************************************************************************
Compacta um arquivo, em formato ZIP, para uma string.
Esta função utiliza o executável "gzip", tanto da versão Linux como de Windows.
O nível de compressão é o máximo (9).
Argumentos:
    sArquivo : arquivo original
    lComprimento : endereço do comprimento do texto original e do comprimido, ou zero para erros
    Retorno: novo texto comprimido, ou nulo
*******************************************************************************/
	gchar *sTemp = NULL;
	gchar *sSaida = NULL;
	gint nSaida = 0;
	gulong lLen = 0;

	sTemp = g_strdup_printf ("gzip -c9 %s", sArquivo);//linha de comando para executar gzip
	//a função g_spawn_command_line_sync() executa a linha de comando, e espera terminar a execução
	//a resposta da linha de comando (stdout) estará nna variável sSaída
	if (g_spawn_command_line_sync (sTemp, &sSaida, NULL, &nSaida, NULL) && nSaida == 0)
	{
		//loop para contar comprimento
		BE_free (sTemp);
		sTemp = sSaida;
		while (sTemp[0] != 0 || sTemp[1] != 0)//é considerado o término do arquivo com 2 vazios seguidos
		{
			sTemp++;
			lLen++;
		}
		*lComprimento = lLen + 2;
	}
	//não deu certo, sair
	else
	{
		//limpar tudo
		BE_free (sSaida);
		BE_free (sTemp);
	}

	return (guchar *) sSaida;
}
