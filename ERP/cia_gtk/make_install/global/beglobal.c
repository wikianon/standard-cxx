#include "beglobal.h"

/*******************************************************************************
// Aqui cont�m todas as fun��es de acesso geral, que n�o s�o especificas.
// Tem bastante fun��es que s�o extens�es para GTK e diversas defini��es de vari�veis, tipos, estruturas e macros.
//
// => Redefini��es:
// Atrav�s de macros do compilador, diversas fun��es foram modificadas.
// A maioria para proporcionar uma forma mais eficaz de limpeza de uma vari�vel alocada dinamicamente.
//
// => Rastreamento de mem�ria utilizada e n�o liberada:
// Atrav�s de uma tabela Hash foi feito uma tentativa (ainda incompleta) de rastrear todo espa�o alocado explicitamente, para no final da execu��o do programa verificar quais n�o foram liberados.
// Neste mesmo processo foi implementado um modo de paradas para breakpoints, para os erros serem rastreados de acordo com a quantidade de mem�ria alocada.
*******************************************************************************/

//rastreamento de aloca��es de mem�rias
static GHashTable *BEGLOBAL_hash = NULL;//tabela Hash, global
static gboolean bBEGLOBAL_registrar = TRUE;//ativa��o/desativa��o, tempor�ria
static glong lBEGLOBAL_passo_parar = 0;//passo que deseja parar, definido em be_global_abrir()
static glong lBEGLOBAL_passo = 0;//passo atual, definido nas aloca��es e libera��es

void be_global_abrir (gint *argc, gchar ***argv, glong lPasso)
{
/*******************************************************************************
// be_global_abrir
// Esta fun��o deve ser chamada no in�cio de qualquer fun��o main().
// Todas as fun��es inicializadoras est�o aqui.
// Argumentos:
//     argc : argc da fun��o main()
//     argv : argv da fun��o main()
//     lPasso : momento onde gerar um breakpoint (depura��o). Cada passo representa um carregamento de mem�ria (malloc, calloc, realloc)
//     Retorno: sem retorno
*******************************************************************************/

	#ifdef G_OS_UNIX
	//	system("clear");//limpar tela
	#endif
	//GMemVTable Alocador;

	bBEGLOBAL_registrar = FALSE;//pausar (parar temporariamente) o rastreamento

	lBEGLOBAL_passo_parar = lPasso;//definir parada for�ada, para depura��o

	//substituir as fun��es padr�es malloc, realloc e free pelas minhas
	//Alocador.malloc = _be_global_malloc; // ********  TA COM PROBLEMAS NA DEPURA��O  WINDOWS GDB5 E GDB6*******
	//Alocador.realloc = _be_global_realloc;
	//Alocador.free =  _be_global_free;
	//Alocador.calloc = NULL;
	//Alocador.try_malloc = NULL;
	//Alocador.try_realloc = NULL;
	//g_mem_set_vtable (&Alocador);
	//iniciar o hash
	//BEGLOBAL_hash = g_hash_table_new_full (NULL, NULL, NULL, __be_global_hash_remover);// ********  TA COM PROBLEMAS NA DEPURA��O  WINDOWS GDB5 E GDB6*******

    //toda vez que ocorrer um erro tratado no GTK (n�o inesperado), ser� chamado a fun��o _be_global_erros_gtk
	g_log_set_default_handler (_be_global_erros_gtk, (char *) *argv[0]);//repassa uma fun��o

	//inicializadores
	/*#ifdef _GDK
		gdk_init (argc, argv);//inicialidador padr�o do GDK
	#endif
	#ifdef _GTK
		gtk_init (argc, argv);//inicialidador padr�o do GTK, o do GDK est� incluso
	#endif
	#ifdef _GLADE
		gtk_init (argc, argv);//inicialidador padr�o do GTK, o do GDK est� incluso
		glade_init ();//inicialidador padr�o do GLADE
	#endif*/
	gtk_init (argc, argv);//inicializador padr�o do GTK, inclui todos os outros (GDK, Glade)
	setlocale (LC_NUMERIC, "C");//manter compatibilidade em caracteres especiais (acentua��es, etc)
	//configura��o inicial
	g_set_application_name (INTERNAL_NAME);//definir nome da aplica��o, programa
	g_set_prgname (PRODUCT_NAME);//definir nome do produto
	//manter coer�ncia nos estilos, principalmente para tabelas
	gtk_rc_parse_string (
		"style \"ciagtk\"\n"
		"{\n"
			"engine \"\"{}\n"//motor
			"GtkWidget::focus-line-pattern = \"\"\n"
			"GtkWidget::focus_line_width = 3\n"
			"GtkWidget::interior_focus = 0\n"
			//"GtkWidget::cursor-aspect-ratio = 0\n"
			//"GtkWidget::cursor-color = \"#ffff00\"\n"
			"GtkWidget::focus_padding = 0\n"
			//"GtkWidget::secondary-cursor-color = \"#ff0000\"\n"
			"GtkTreeView::allow-rules = 1\n"//ativar altern�ncia de cores
			//"GtkTreeView::even-row-color = \"#fff000\"\n"//cor de altern�ncia, clara
			//"GtkTreeView::odd-row-color = \"#000fff\"\n"//cor de altern�ncia, escuro
			//"GtkTreeView::expander-size = 1\n"
			//"GtkTreeView::horizontal-separator = 1\n"
			//"GtkTreeView::indent-expanders = 1\n"
			//"GtkTreeView::vertical-separator = 10\n"
			//"base[SELECTED] = \"#ffff00\"\n"
			//"bg[SELECTED] = \"#ffff00\"\n"
			//"text[SELECTED] = \"#0000ff\"\n"
			//"fg[SELECTED] = \"#ff0000\"\n"
		"}"
		"class \"GtkTreeView\" style \"ciagtk\"\n"
	);
	//inicializar interpretador interativo: python
	Py_Initialize ();

	bBEGLOBAL_registrar = TRUE;//continuar o rastreamento
}

void be_global_fechar ()
{
/*******************************************************************************
// be_global_fechar
// Esta fun��o deve ser chamada no fim de qualquer fun��o main().
// Todas as fun��es finalizadoras est�o aqui.
// Argumentos:
//     Retorno: sem retorno
// ****************************************************************************/
	guint nQt = 0;//contador de �tens na tabela Hash

	bBEGLOBAL_registrar = FALSE;//pausar (parar temporariamente) o rastreamento

	if (BEGLOBAL_hash)//somente se houver uma tabela Hash
	{
		//gerar comandos para depura��o
		nQt = g_hash_table_size (BEGLOBAL_hash);//recuperar tamanho da tabela
		if (nQt > 0)//somnte se for maior que zero
		{
			//exibir a mem�ria ainda alocada
			g_printerr ("**** MEMORIA PERDIDA ****\n");
			g_hash_table_foreach (BEGLOBAL_hash, __be_global_hash_localizar, NULL);//chamar um a fun��o __be_global_hash_localizar() para cada �tem da tabela
		}
		g_hash_table_destroy (BEGLOBAL_hash);//limpar
	}
	//finalizar interpretador interativo: python
	Py_Finalize ();

	bBEGLOBAL_registrar = TRUE;//continuar o rastreamento

	//#ifdef G_OS_WIN32
	//	system("pause");//pausar a sa�da do console em Windows, sen�o a janela simplesmente fecha no t�rmino da execu��o do programa
	//#endif
}

void __be_global_hash_localizar (gpointer key, gpointer value, gpointer user_data)
{
/*******************************************************************************
// __be_global_hash_localizar
// Esta fun��o � chamada toda vez (callback) que houver um �tem dentro de uma tabela Hash.
// � definida pela fun��o g_hash_table_foreach()
// Fun��o somente para uso interno.
// Argumentos:
//     key : contem a chave/�ndice da tabela Hash
//     value : cont�m o valor
//     user_data : valor extra, repassado na defini��o em g_hash_table_foreach()
//     Retorno: sem retorno
*******************************************************************************/
	//g_printerr ("\t%p - %d\n", key, (gsize) value);
	//free (key);
}

void __be_global_hash_remover (gpointer value)
{
/*******************************************************************************
// __be_global_hash_remover
// Esta fun��o � chamada toda vez que for retirar um �tem tabela Hash.
// � definida pela fun��o g_hash_table_new_full()
// Fun��o somente para uso interno.
// Argumentos:
//     value : conte�do a ser liberado
//     Retorno: sem retorno
*******************************************************************************/
	free ((glong *) value);//limpar
}

void _be_global_marcar (gpointer pAlocada, gsize nBytes)
{
/*******************************************************************************
// _be_global_marcar
// Esta fun��o � chamada toda vez que for inserir um �tem tabela Hash.
// � definida em fun��es de aloca��o de mem�ria. _be_global_malloc() e _be_global_realloc()
// Fun��o somente para uso interno.
// Argumentos:
//     pAlocada : conte�do alocado
//     nBytes : comprimento do conte�do
//     Retorno: sem retorno
*******************************************************************************/
	if (pAlocada && BEGLOBAL_hash)
	{
		lBEGLOBAL_passo++;//incrementar os passos
		if (lBEGLOBAL_passo_parar == lBEGLOBAL_passo)
			_be_global_parar ();//para com breakpoint
		//inserindo no hash
		//g_hash_table_insert (BEGLOBAL_hash, pAlocada, (gpointer *) nBytes);//marcar mem�ria
		g_hash_table_insert (BEGLOBAL_hash, pAlocada, (gpointer *) lBEGLOBAL_passo);//marcar passo
	}
}

void *_be_global_malloc (gsize nBytes)
{
/*******************************************************************************
// _be_global_malloc
// Esta fun��o substitui, invis�vel, a fun��o padr�o malloc() ou g_malloc().
// Est� definida dentro de uma estrutura GMemVTable.
// Fun��o somente para uso interno.
// Argumentos:
//     nBytes : comprimento do conte�do � ser alocado
//     Retorno: endere�o do conte�do alocado
*******************************************************************************/
	gpointer pAlocada = NULL;//ponteiro gen�rico, sem tipo

	pAlocada = malloc (nBytes);//aloca��o normal, mas sem tipo

	if (bBEGLOBAL_registrar)
	{
		if (pAlocada)
		{
#if BEDEPURACAO == 1
			printf ("%s:%d - MALLOC %p %db\n", __FILE__, __LINE__, pAlocada, nBytes);
#endif
			bBEGLOBAL_registrar = FALSE;//pausar (parar temporariamente) o rastreamento
			_be_global_marcar (pAlocada, nBytes);//inserir um �tem na tabela Hash
			bBEGLOBAL_registrar = TRUE;//continuar o rastreamento
		}
		else
		//informar falta de mem�ria
		{
			printf ("Erro. Nao ha memoria suficiente para continuar.\n");
			_be_global_parar ();//parar tudo, breakpoint
		}
	}

	return pAlocada;
}

void *_be_global_realloc (gpointer pAntigo, gsize nBytes)
{
/*******************************************************************************
// _be_global_realloc
// Esta fun��o substitui, invis�vel, a fun��o padr�o realloc() ou g_realloc().
// Est� definida dentro de uma estrutura GMemVTable.
// Fun��o somente para uso interno.
// Argumentos:
//     pAntigo : conte�do antigo, alocado
//     nBytes : comprimento do conte�do � ser alocado
//     Retorno: endere�o do conte�do alocado
*******************************************************************************/
	gpointer pAlocada = NULL;//ponteiro gen�rico, sem tipo

	pAlocada = realloc (pAntigo, nBytes);//aloca��o normal, mas sem tipo

	if (bBEGLOBAL_registrar)
	{
		if (pAlocada)
		{
#if BEDEPURACAO == 1
			printf ("%s:%d - REALLOC %p > %p %db\n", __FILE__, __LINE__, pAntigo, pAlocada, nBytes);
#endif
			bBEGLOBAL_registrar = FALSE;//pausar (parar temporariamente) o rastreamento
			_be_global_marcar (pAlocada, nBytes);//inserir um �tem na tabela Hash
			bBEGLOBAL_registrar = TRUE;//continuar o rastreamento
		}
		else
		//informar falta de mem�ria
		{
			printf ("Erro. Nao ha memoria suficiente para continuar.\n");
			_be_global_parar ();//parar tudo, breakpoint
		}
	}

	return pAlocada;
}

void _be_global_free (gpointer pAlocada)
{
/*******************************************************************************
// _be_global_free
// Esta fun��o substitui, invis�vel, a fun��o padr�o free() ou g_free().
// Est� definida dentro de uma estrutura GMemVTable.
// Fun��o somente para uso interno.
// Argumentos:
//     pAntigo : conte�do antigo, alocado
//     Retorno: sem retorno
*******************************************************************************/
	if (bBEGLOBAL_registrar && pAlocada && BEGLOBAL_hash)
	{
		bBEGLOBAL_registrar = FALSE;//pausar (parar temporariamente) o rastreamento
        lBEGLOBAL_passo--;//decremento dos passos
		g_hash_table_remove (BEGLOBAL_hash, pAlocada);//retirando �tem do Hash

#if BEDEPURACAO == 1
		printf ("%s:%d - FREE %p\n", __FILE__, __LINE__, pAlocada);
#endif
		bBEGLOBAL_registrar = TRUE;//continuar o rastreamento
	}
    //limpar
	free (pAlocada);
}

void _be_global_erros_gtk (const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
{
/*******************************************************************************
// _be_global_erros_gtk
// Sempre que ocorrer um erro tratado no GTK (n�o inesperado), esta fun��o ser� chamada para tratamento de erros.
// Est� definida na fun��o g_log_set_default_handler()
// Fun��o somente para uso interno.
// Argumentos:
//     log_domain : nome do dom�nio
//     log_level : n�vel do erro
//     message : mensagem do erro
//     user_data : dados repassados na defini��o da chamada
//     Retorno: sem retorno
*******************************************************************************/
	if (log_level == G_LOG_LEVEL_WARNING || log_level == G_LOG_LEVEL_MESSAGE || log_level == G_LOG_LEVEL_INFO || log_level == G_LOG_LEVEL_DEBUG)
		printf ("%s - %s\n", log_domain, message);//somente informar, pois h� muitos avisos no GTK
	else
		_be_global_parar ();//em outros tipos derros, para com breakpoint
}

void _be_global_parar ()
{
/*******************************************************************************
// _be_global_parar
// Parar a execu��o do programa, gerando um breakpoint. Utilizado para depura��es.
// Fun��o somente para uso interno.
// Argumentos:
//     Retorno: sem retorno
*******************************************************************************/
	G_BREAKPOINT();//parar para depura��o
	exit (EXIT_FAILURE);//finalizar programa, ap�s depura��o
}

#undef _BEGLOBAL_C


//+----------------------------------------------------------------------------+
//| BEStrings                                                                  |
//+----------------------------------------------------------------------------+
gchar *be_strreplace_pos (const gchar *sString, const gchar *sLocalizar, const gchar *sSubstituir, gint nInicio)
{
/*******************************************************************************
// be_strreplace_pos
// Retorna sString com todos os sLocalizar trocados por sSubstituir, apartir da posi��o nInicio.
// Se nInicio for negativo, ser� considerado do fim para o in�cio.
// Se nInicio for maior que o comprimento do texto original, ser� considerado posi��o zero.
// Argumentos:
//     sString : texto original
//     sLocalizar : texto � ser localizado
//     sSubstituir : texto � ser substitu�do
//     nInicio : posi��o inicial da busca.
//     Retorno: novo texto alocado na mem�ria, com ou sem altera��es
*******************************************************************************/
	GString *String = NULL;
	gchar *sTemp = NULL;
	gint nPos = 0, nCompL = 0, nCompS = 0;

	//se for menor que zero, ser� considerado do fim para o in�cio
	if (nInicio < 0)
		nInicio = strlen (sString) + nInicio;
    //se for maior que o comprimento do texto original, ser� considerado posi��o zero
	if (nInicio > strlen (sString))
		nInicio = 0;
    //se texto original e novo texto forem iguais � s� retornar uam c�pia do mesmo
	if (g_str_equal (sLocalizar, sSubstituir))
		return g_strdup (sString);

	String = g_string_new (sString + nInicio);//iniciando tipo String, apartir de nInicio
	nCompL = strlen (sLocalizar);//comprimento
	nCompS = strlen (sSubstituir);//comprimento

    sTemp = g_strstr_len (String->str, String->len, sLocalizar);//localiza a primeira posi��o de sLocalizar dentro de String
    //atrav�s do loop ser� poss�vel localizar e alterar todos os textos
	while (sTemp)
	{
		nPos = sTemp - String->str;//posi��o inicial localizada
		String = g_string_erase (String, nPos, nCompL);//excluir o texto antigo
		String = g_string_insert (String, nPos, sSubstituir);//inserir o texto novo
        sTemp = g_strstr_len (String->str + nPos + nCompS, String->len - (nPos + nCompS), sLocalizar);//localiza a pr�xima posi��o de sLocalizar dentro de String
	}
	g_free (sTemp);//limpar
	//incluir restante inicial de sString, que n�o foi alterada por causa de nInicio
	if (nInicio > 0)
		String = g_string_prepend_len (String, sString, nInicio);

	return BE_string_free (String);//retornar c�pia da sString
}

gchar *be_strreplace (const gchar *sString, const gchar *sLocalizar, const gchar *sSubstituir)
{
/*******************************************************************************
// be_strreplace
// Retorna sString com todos os sLocalizar trocados por sSubstituir.
// Faz o mesmo que be_strreplace_pos(), mas a posi��o inicial � sempre zero
// Argumentos:
//     sString : texto original
//     sLocalizar : texto � ser localizado
//     sSubstituir : texto � ser substitu�do
//     Retorno: novo texto alocado na mem�ria, com ou sem altera��es
*******************************************************************************/
	return be_strreplace_pos (sString, sLocalizar, sSubstituir, 0);
}

gchar *be_substr (const gchar *sString, gint nInicio, gint nComprimento)
{
/*******************************************************************************
// be_substr
// Retorna parte da sString iniciando por nInicio e de comprimento nComprimento.
// Se nInicio for maior que o comprimento de sString ser� considerado posi��o zero.
// Se nInicio for negativo, o texto retornado iniciar� na posi��o nInicio menos o comprimento, ou seja do fim para o in�cio.
// Se nComprimento for negativo, esta quantidade de caracteres ser� omitida no final de sString (ap�s a posic�o de inicio ter sido calculada quando nInicio for negativo).
// Argumentos:
//     sString : texto original
//     nInicio : posi��o inicial
//     nComprimento : comprimento
//     Retorno: novo texto alocado na mem�ria, com ou sem altera��es
*******************************************************************************/
	gchar *sTemp = NULL;

	//Se nInicio for negativo, o texto retornado iniciar� na posi��o nInicio menos o comprimento, ou seja do fim para o in�cio.
	if (nInicio < 0)
		nInicio = strlen (sString) + nInicio;
    //se nInicio for maior que o comprimento de sString ser� considerado posi��o zero
	if (nInicio > strlen (sString))
		nInicio = 0;
    //se nComprimento for negativo, esta quantidade de caracteres ser� omitida no final de sString (ap�s a posic�o de inicio ter sido calculada quando nInicio for negativo).
	if (nComprimento < 0)
		nComprimento = strlen (sString) - nInicio + nComprimento;

	sTemp = g_strdup (sString + nInicio);//copiar apartir da posi��o inicial, nInicio
	if (nComprimento > 0)
		sTemp[nComprimento] = '\0';//inserir caract�r nulo, para limitar o comprimento
	return sTemp;
}

gint be_strpos (const gchar *sString, const gchar *sLocalizar, const gint nInicio)
{
/*******************************************************************************
// be_strpos
// Retorna a posi��o num�rica da primeira ocorr�ncia de sLocalizar dentro de sString, iniciando em nInicio.
// Argumentos:
//     sString : texto original
//     sLocalizar : texto � ser localizado
//     nInicio : posi��o inicial da busca
//     Retorno: posi��o do texto localizado, ou -1 para n�o localizado
*******************************************************************************/
	gchar *sTemp = NULL;

	//sString aponta para in�cio do texto. Ex: 005841h
	//sTemp aponta para posi��o localizada. Ex: 005850h
	//A diferen�a entre sString e sTemp revela a dist�ncia (posi��o) entre eles. Ex: 005850 - 005841 = 9
	if ((sTemp = strstr (sString + nInicio, sLocalizar)))
		return sTemp - sString;//uma simples conta de subtra��o define a posi��o num�rica
	else
		return -1;
}

gint be_strrpos (const gchar *sString, const gchar *sLocalizar, const gint nInicio)
{
/*******************************************************************************
// be_strrpos
// Retorna a posi��o num�rica da �ltima ocorr�ncia de sLocalizar dentro de sString, at� nInicio.
// Argumentos:
//     sString : texto original
//     sLocalizar : texto � ser localizado
//     nInicio : posi��o final da busca. 0 � at� o fim.
//     Retorno: posi��o do texto localizado, ou -1 para n�o localizado
*******************************************************************************/
	gchar *sTemp = NULL;

	//sString aponta para in�cio do texto. Ex: 005841h
	//sTemp aponta para posi��o localizada. Ex: 005850h
	//A diferen�a entre sString e sTemp revela a dist�ncia (posi��o) entre eles. Ex: 005850 - 005841 = 9
	if ((sTemp = g_strrstr (sString + nInicio, sLocalizar)))
		return sTemp - sString;//uma simples conta de subtra��o define a posi��o num�rica
	else
		return -1;
}

gint be_strpos_ascii (const gchar *sString, const gchar *sLocalizar, const gint nInicio)
{
/*******************************************************************************
// be_strpos_ascii
// Retorna a posi��o num�rica da primeira ocorr�ncia de sLocalizar dentro de sString, iniciando em nInicio.
// Tanto sString, como sLocalizar s�o ignorados a caixa das letras (Mai�scula/Min�scula)
// Argumentos:
//     sString : texto original
//     sLocalizar : texto � ser localizado
//     nInicio : posi��o inicial da busca
//     Retorno: posi��o do texto localizado, ou -1 para n�o localizado
*******************************************************************************/
	gchar *sTemp = NULL;
	gchar *sStringL = NULL, *sLocalizarL = NULL;
	gint nPos = -1;

	//sString aponta para in�cio do texto. Ex: 005841h
	//sTemp aponta para posi��o localizada. Ex: 005850h
	//A diferen�a entre sString e sTemp revela a dist�ncia (posi��o) entre eles. Ex: 005850 - 005841 = 9
	//convers�o para min�sculas
	sStringL = g_ascii_strdown (sString + nInicio, -1);
	sLocalizarL = g_ascii_strdown (sLocalizar, -1);
	//comparar
	if ((sTemp = strstr (sStringL, sLocalizarL)))
		nPos = (sTemp - sStringL) + nInicio;//uma simples conta de subtra��o define a posi��o num�rica
	//limpar
	BE_free (sStringL);
	BE_free (sLocalizarL);

	return nPos;
}

gchar *be_strrepeat (const gchar *sString, const gint nQt)
{
/*******************************************************************************
// be_strrepeat
// Retorna sString repetida nQt vezes.
// Argumentos:
//     sString : texto original
//     nQt : quantidade de vezes
//     Retorno: novo texto repetido se nQt > 0, ou nulo
*******************************************************************************/
	GString *String = NULL;
	gint nCont = 0;

	String = g_string_new ("");//iniciando tipo
	//loop para de 0 � nCont nQt para contatenar o mesmo texto
	for (nCont = 0; nCont < nQt; nCont++)
		String = g_string_append (String, sString);//adicionando texto no final

	return BE_string_free (String);//retorna c�pia
}

gint be_strcount (const gchar *sString, const gchar *sLocalizar)
{
/*******************************************************************************
// be_strcount
// Retorna a quantidade de ocorr�ncias de sLocalizar dentro de sString.
// Argumentos:
//     sString : texto original
//     sLocalizar : texto � ser localizado
//     Retorno: quantidade localizada
*******************************************************************************/
	gchar *sTemp = NULL;
	gint nQt = 0, nComp = 0;

	nComp = strlen (sString);//comprimento
	//cada vez que um texto � localizado dentro do outro, o loop avan�a, o contador soma, e o texto original vai diminuindo para n�o ser contado novamente
	while ((sTemp = g_strrstr_len (sString, nComp, sLocalizar)))//continuar at� n�o localizar mais
	{
		nComp = (sTemp - sString);
		nQt++;//contador
	}
	return nQt;
}

gchar *be_strextract (const gchar *sString, const gchar *sLocalizar1, const gchar *sLocalizar2, gint nInicio)
{
/*******************************************************************************
// be_strextract
// Retorna um ponteiro para uma nova string dentro do intervalo localizado em sLocalizar1 e sLocalizar2.
// Se nInicio for igual a 0 ser� localizado desde o in�cio de sString.
// Se sLocalizar1 ou sLocalizar2 for vazio, a sa�da � nula.
// A utilidade de nInicio est� em poder pular alguns textos que coincidem com sLocalizar1 e/ou sLocalizar2.
// Argumentos:
//     sString : texto original
//     sLocalizar1 : texto marcador da posi��o inicial
//     sLocalizar2 : texto marcador da posi��o final
//     nInicio : Posi��o inicial
//     Retorno: novo texto localizado, ou nulo
*******************************************************************************/
	gint nNovoInicio = 0, nFim = 0;

	if (sString && sLocalizar1 && sLocalizar2)//sair se alguem for nulo
	{
		nNovoInicio = be_strpos (sString, sLocalizar1, nInicio);//localiza��o da posi��o inicial
		if (nInicio >= 0 && nNovoInicio >= 0)//sair se n�o localizar a primeira posi��o
		{
			if (sLocalizar2)//sair se sLocalizar2 for nulo
			{
				nNovoInicio += strlen (sLocalizar1);//ajustar in�cio para o final do sLocalizar1, pois ele pode ter partes iguais a de sLocalizar2
				nFim = be_strpos (sString, sLocalizar2, nNovoInicio);//localiza��o da posi��o final
				if (nFim > 0)//sair se n�o localizar a segunda posi��o
				{
					if (nFim - nNovoInicio > 0)//se houver algo entre eles
						return be_substr (sString, nNovoInicio, nFim - nNovoInicio);//retornar uma c�pia
					else
						return g_strdup ("");//retornar um endere�o v�lido, mas vazio
				}
			}
		}
	}

	return NULL;
}

gchar *be_strextract_ascii (const gchar *sString, const gchar *sLocalizar1, const gchar *sLocalizar2, gint nInicio)
{
/*******************************************************************************
// be_strextract_ascii
// Retorna um ponteiro para uma nova string dentro do intervalo localizado em sLocalizar1 e sLocalizar2.
// Se nInicio for igual a 0 ser� localizado desde o in�cio de sString.
// Se sLocalizar1 ou sLocalizar2 for vazio, a sa�da � nula.
// A utilidade de nInicio est� em poder pular alguns textos que coincidem com sLocalizar1 e/ou sLocalizar2.
// Tanto sString, como sLocalizar1 e sLocalizar2 s�o ignorados a caixa das letras (Mai�scula/Min�scula)
// Argumentos:
//     sString : texto original
//     sLocalizar1 : texto marcador da posi��o inicial, ignorando CASE
//     sLocalizar2 : texto marcador da posi��o final, ignorando CASE
//     nInicio : Posi��o inicial
//     Retorno: novo texto localizado, ou nulo
*******************************************************************************/
	gint nNovoInicio = 0, nFim = 0;

	if (sLocalizar1)//sair se sLocalizar1 for nulo
	{
		nNovoInicio = be_strpos_ascii (sString, sLocalizar1, nInicio);//localiza��o da posi��o inicial
		if (nInicio >= 0 && nNovoInicio >= 0)//sair se n�o localizar a primeira posi��o
		{
			if (sLocalizar2)//sair se sLocalizar2 for nulo
			{
				nNovoInicio += strlen (sLocalizar1);//ajustar in�cio para o final do sLocalizar1, pois ele pode ter partes iguais a de sLocalizar2
				nFim = be_strpos_ascii (sString, sLocalizar2, nNovoInicio);//localiza��o da posi��o final
				if (nFim > 0)//sair se n�o localizar a segunda posi��o
				{
					if (nFim - nNovoInicio > 0)//se houver algo entre eles
						return be_substr (sString, nNovoInicio, nFim - nNovoInicio);//retornar uma c�pia
					else
						return g_strdup ("");//retornar um endere�o v�lido, mas vazio
				}
			}
		}
	}

	return NULL;
}

GSList *be_strsplit (gchar **sString, const gchar *sIndicador)
{
/*******************************************************************************
// be_strsplit
// Quebra uma sString em v�rias partes.
// Na verdade � criado uma lista que aponta para v�rias partes de mesma string.
// � importante observar que o ender�o de sString ir� mudar, assim como seu conte�do. Em todos sIndicador ser� substitu�do por \0 (fim de linha).
// S� limpar sString ap�s limpar a lista de retorno.
// Argumentos:
//     sString : endere�o do texto original
//     sIndicador : indicador de quebra. Exe.: \n, \t, ;,  " "
//     Retorno: sempre retorna uma nova lista
*******************************************************************************/
	GSList *Lista = NULL;
	gchar *sTemp = NULL;
	gint nInicio = -1, nInicioAnt = 0, nComp = 0;

	nComp = strlen (sIndicador);//comprimento do indicador
	sTemp = g_strdup (*sString);//c�pia do texto
	//loop para pegar partes do texto e incluir na lista
	do
	{
		nInicio = be_strpos (sTemp, sIndicador, nInicio + 1);//localizar o in�cio
		if (nInicio > 0)
		{
			Lista = g_slist_append (Lista, sTemp + nInicioAnt);//acrescentar ponteiro � lista
			sTemp[nInicio] = '\0';//marcar fim de linha
			nInicioAnt = nInicio + nComp;
		}
	} while (nInicio >= 0);
	//adicionar �ltima parte
	Lista = g_slist_append (Lista, sTemp + nInicioAnt);

	BE_free (*sString);//limpar string antiga
	*sString = sTemp;//apontar para nova

	return Lista;
}

gchar *be_strclear (const gchar *sString, BEGLOBAL_CONVERSOES nOpc)
{
/*******************************************************************************
// be_strclear
// Devolve uma string, dependendo das seguintes op��es de BEGLOBAL_CONVERSOES.
// Argumentos:
//     sString : texto original
//     nOpc : uma op��o de BEGLOBAL_CONVERSOES
//     Retorno: depende das op��es, mas � sempre um novo texto
*******************************************************************************/
	GString *String = NULL;
	gchar searchA[][5] = {"����", "����", "���", "���", "���", "���", "����", "����", "����", "����", "�", "�"};//mapa de acentuadas
	gchar replaceA[] = {'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U', 'c', 'C'};//mapa de n�o acentuadas
	gchar searchF[][3] = {"cx", "ce", "ci", "ch", "xs", "qu", "ge", "gi", "ka", "ko", "rr", "ss"};//mapa de para convers�o fon�tica
	gchar replaceF[][3] = {"s", "se", "si", "x", "s", "q", "je", "ji", "ca", "co", "r", "s"};//mapa de correspond�ncia fon�tica
	gint nCont = 0;

	if (nOpc <= BEGLOBAL_CONVERSAO_HEXADECIMAIS)
		String = g_string_new (NULL);//iniciando tipo
	else
		String = g_string_new (sString);//iniciando tipo

	//loop de caract�r por caract�r.
	//quando um caract� � validado o mesmo � inclu�do ao final de String
	for (nCont = 0; nCont < strlen (sString); nCont++)
	{
		switch (nOpc)
		{
			case (BEGLOBAL_CONVERSAO_ALFA)://somente os caract�res alfanum�ricos
				if (g_ascii_isalnum (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_LETRAS://somente as letras do alfabeto atual
				if (g_ascii_isalpha (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_CONTROLE://somente teclas de controle (Ctrl, Shift, Alt)
				if (g_ascii_iscntrl (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_DIGITOS://somente d�gitos de 0 � 9
				if (g_ascii_isdigit (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_IMPRESSOS://somente caract�res de impress�o sem espa�o
				if (g_ascii_isgraph (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_MINUSCULAS://somente letras min�sculas
				if (g_ascii_islower (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_IMPRESSOS_ESPACO://somente caract�res de impress�o com espa�o
				if (g_ascii_isprint (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_PONTUACAO://somente caract�res de pontua��o
				if (g_ascii_ispunct (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_ESPECIAIS://somente espa�o, tabula��es, retorno de carro ou nova linha <ENTER>
				if (g_ascii_isspace (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_MAIUSCULAS://somente letras mai�sculas
				if (g_ascii_isupper (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
			case BEGLOBAL_CONVERSAO_HEXADECIMAIS://somente d�gitos hexadecimais
				if (g_ascii_isxdigit (sString[nCont]))
					String = g_string_append_c (String, sString[nCont]);
				break;
            default://sem nada
                break;
		}
	}
	//converter todas as letras
	switch (nOpc)
	{
		case BEGLOBAL_CONVERSAO_SEM_ACENTOS://convers�o de letras acentuadas para n�o acentuadas
			for (nCont = 0; nCont < (sizeof(searchA)/5); nCont++)
				String->str = g_strdelimit (String->str, searchA[nCont], replaceA[nCont]);
			break;
		case BEGLOBAL_CONVERSAO_FONETICAS://convers�o fon�tica
			for (nCont = 0; nCont < (sizeof(searchF)/3); nCont++)
				String->str = be_strreplace (String->str, searchF[nCont], replaceF[nCont]);
			break;
        default://sem convers�o
            break;
	}

	return BE_string_free (String);
}

glong be_cast_hex_long (const gchar *sString)
{
/*******************************************************************************
// be_cast_hex_long
// Converte um n�mero hexadecimal que est� em forma de string em um long.
// Argumentos:
//     sString : n�mero hexadecimal em formato string
//     Retorno: long do hexadecimal
*******************************************************************************/
	gchar sHex[] =  {"0123456789ABCDEF"};//hexadecimais v�lidos
	gchar sHex1[] = {"0123456789abcdef"};//hexadecimais v�lidos
	gchar *sTemp = NULL;
	const glong lBase = 16;
	gint nCont = 0, nDig = 0;
	glong lNum = 0, lPos = 0;

	//loop de tr�z pra frente, de cada caract�r em sString
	for (nCont = strlen (sString) - 1; nCont >= 0; nCont--)
	{
		//primeira tentativa
		sTemp = strchr (sHex, sString[nCont]);//localizar os caract�res v�lidos
		if (sTemp)
			nDig = (sTemp - sHex);//localizar posi��o dentro dos caract�res v�lidos
		else
		{
			//segunda tentativa
			sTemp = strchr (sHex1, sString[nCont]);//localizar os caract�res v�lidos
			if (sTemp)
				nDig = (sTemp - sHex1);//localizar posi��o dentro dos caract�res v�lidos
		}
		//ignorar caract�re inv�lido
		if (nDig >= 0)
		{
			lNum += (long) nDig * powf (lBase, lPos);//recuperar d�gitos
			lPos++;//contador de posi��es
		}
		//limpar
		//BE_free (sTemp);//n�o pode ser limpado, pois strchr somente aponta
		nDig = 0;
	}

	return lNum;
}

gint be_rgb_vermelho (glong lCorRGB)
{
/*******************************************************************************
// be_rgb_vermelho
// Extrai a cor vermelho de um n�mero RGB.
// Argumentos:
//     lCorRGB : cor RGB
//     Retorno: 0 � 255
*******************************************************************************/
	if (lCorRGB > powf (16, 4))//65536
		lCorRGB /= powf (16, 4);
	else
		lCorRGB = 0;

	return (gint) lCorRGB;
}

gint be_rgb_verde (glong lCorRGB)
{
/*******************************************************************************
// be_rgb_verde
// Extrai a cor verde de um n�mero RGB.
// Argumentos:
//     lCorRGB : cor RGB
//     Retorno: 0 � 255
*******************************************************************************/
	lCorRGB -= (glong) be_rgb_vermelho (lCorRGB) * powf (16, 4);

	if (lCorRGB > powf (16, 2))//256
		lCorRGB /= powf (16, 2);
	else
		lCorRGB = 0;

	return (gint) lCorRGB;
}

gint be_rgb_azul (glong lCorRGB)
{
/*******************************************************************************
// be_rgb_azul
// Extrai a cor azul de um n�mero RGB.
// Argumentos:
//     lCorRGB : cor RGB
//     Retorno: 0 � 255
*******************************************************************************/
	lCorRGB -= (glong) be_rgb_vermelho (lCorRGB) * powf (16, 4);
	lCorRGB -= (glong) be_rgb_verde (lCorRGB) * powf (16, 2);

	return (gint) lCorRGB;
}

gint be_string_ordenar_string (gconstpointer a, gconstpointer b)
{
/*******************************************************************************
// be_string_ordenar_string
// Retorna qual das duas strings vem primeiro numa ordem alfab�tica, pela tabela ASCII.
// As diferen�as entre mai�sculas e min�sculas s�o ignoradas.
// Ideal para utiliza com a fun��o g_list_sort().
// Argumentos:
//     a : ponteiro para texto A
//     b : ponteiro para texto B
//     Retorno:    0 = os textos s�o iguais
//                 1 = a � menor (vem primeiro) que b
//                 2 = b � menor (vem primeiro) que a
*******************************************************************************/
	gint nRet = 0;
	gchar *sA = NULL, *sB = NULL;

	//verificar validade do conte�do
	if (a == NULL && b != NULL)
		nRet = -1;
	else if (a != NULL && b == NULL)
		nRet = 1;
	else if (a != NULL && b != NULL)
	{
		//cast
		sA = (gchar *) a;
		sB = (gchar *) b;
		//converter para min�sculas
		sA = g_ascii_strdown (sA, -1);
		sB = g_ascii_strdown (sB, -1);
		//comparar e retornar
		nRet = g_ascii_strcasecmp (sA, sB);
	}
	//liberando mem�ria
	BE_free (sA);
	BE_free (sB);

	return nRet;
}

gchar *_be_markup_printf_escaped (gchar *sTexto)
{
/*******************************************************************************
// _be_markup_printf_escaped
// Esta fun��o � utilizada imediatamente na sa�da da fun��o g_markup_printf_escaped(), garantida automaticamente por uma macro de mesmo nome.
// Converte o caract�r '%' tamb�m, para um formato de marca��o HTML.
// sTexto � limpo.
// Argumentos:
//     sTexto : texto original
//     Retorno: novo texto, convertido
*******************************************************************************/
	gchar *sTemp = NULL;

	sTemp = be_strreplace (sTexto, "%", "&#37;");//localiza '%' e substitui por '&#37;'
	BE_free (sTexto);

	return sTemp;
}

gchar *be_markup_text (const gchar *sTexto)
{
/*******************************************************************************
// be_markup_text
// Converte marca��o HTML em caract�res normais. Faz o inverso de g_markup_printf_escaped().
// Argumentos:
//     sTexto : texto original
//     Retorno: novo texto, convertido
*******************************************************************************/
	gchar *sTemp = NULL, *sTemp1 = NULL;

	sTemp = be_strreplace (sTexto, "&amp;", "&");//localiza '&amp;' e substitui por '&'
	sTemp1 = sTemp;
	sTemp = be_strreplace (sTemp1, "&lt;", "<");
	BE_free (sTemp1);
	sTemp1 = sTemp;
	sTemp = be_strreplace (sTemp1, "&gt;", ">");
	BE_free (sTemp1);
	sTemp1 = sTemp;
	sTemp = be_strreplace (sTemp1, "&quot;", "\"");
	BE_free (sTemp1);
	sTemp1 = sTemp;
	sTemp = be_strreplace (sTemp1, "&apos;", "'");
	BE_free (sTemp1);
	sTemp1 = sTemp;
	sTemp = be_strreplace (sTemp1, "&#37;", "%");

	BE_free (sTemp1);

	return sTemp;
}

void be_redimensionar_proporcional (gfloat *fLarguraOriginal, gfloat *fAlturaOriginal, gfloat Largura, gfloat Altura, gboolean bAumentar)
{
/*******************************************************************************
// be_redimensionar_proporcional
// Redimensiona um ret�ngulo, mantendo suas porpor��es.
// Argumentos:
//     fLarguraOriginal : endere�o da largura oiginal
//     fAlturaOriginal : endere�o da altura original
//     Largura : nova largura
//     Altura : nova altura
//     bAumentar : indica se � para considerar os maiores valores entre Largura e Altura
//     Retorno: sem retorno
*******************************************************************************/
	if (*fLarguraOriginal > Largura || bAumentar)
	{
		*fAlturaOriginal = Largura / *fLarguraOriginal * *fAlturaOriginal;
		*fLarguraOriginal = Largura;
	}
	if (*fAlturaOriginal > Altura || bAumentar)
	{
		*fLarguraOriginal = Altura / *fAlturaOriginal * *fLarguraOriginal;
		*fAlturaOriginal = Altura;
	}
}

gdouble be_string_calculadora (gchar *sExpressao)
{
/*******************************************************************************
// be_string_calculadora
// Calcula uma express�o matem�tica e retorna seu valor.
// Argumentos:
// 	sExpressao : texto da express�o matem�tica
// 	Retorno: retorna o valor da express�o
*******************************************************************************/
	/*
	- Converter todos [, { em (.
	- Converter todos ], } em ).
	- Criar uma pilha, para os par�nteses.
	- Usar XML
	*/
	/*
Public Function CalcularExpress�o(ByVal Express�o As String, Optional ByVal Passo As Boolean) As String
On Local Error GoTo TratErr
    Call DepurarErrosRastrear("CalcularExpress�o")
    'Retorna uma express�o matem�tica calculada.
    '
    Dim ContarCalc As Integer, ContarCalc1 As Integer
    Dim TempCalc As String, TempCalc1 As String
    Dim bCalcProc As Boolean
    Dim Valor1 As Double, Valor2 As Double
    If Trim$(Express�o) = "" Then Exit Function
    Express�o = Replace(Express�o, " ", "")
    'Porcentagem
    ContarCalc = InStr(Express�o, "%")
    If ContarCalc > 0 Then
        TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
        If Len(TempCalc) > 0 Then
            Valor1 = TempCalc
            Express�o = Replace(Express�o, TempCalc & "%", Valor1 / 100, , 1)
        End If
    End If
    If Negativar(Express�o) Then bCalcProc = False
    'Iniciando Loop
    Do
        If Passo = True Then Debug.Print Express�o
        bCalcProc = True
        ContarCalc = InStr(Express�o, "^") 'Pot�ncia��o
Fun��oPot�ncia��o:
        If ContarCalc > 0 And bCalcProc Then
            TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
            TempCalc1 = FindLastNumber(Express�o, ContarCalc, 1)
            If Len(TempCalc) > 0 And Len(TempCalc1) > 0 Then
                Valor1 = TempCalc: Valor2 = TempCalc1
                Express�o = Replace(Express�o, TempCalc & "^" & TempCalc1, Valor1 ^ Valor2, , 1)
                bCalcProc = False
            Else
                ContarCalc = InStr(ContarCalc + 1, Express�o, "^")
                GoTo Fun��oPot�ncia��o
            End If
        End If
        ContarCalc = InStr(Express�o, "/") 'Divis�o
Fun��oDivis�o:
        If ContarCalc > 0 And bCalcProc Then
            TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
            TempCalc1 = FindLastNumber(Express�o, ContarCalc, 1)
            If Len(TempCalc) > 0 And Len(TempCalc1) > 0 Then
                Valor1 = TempCalc: Valor2 = TempCalc1
                Express�o = Replace(Express�o, TempCalc & "/" & TempCalc1, Valor1 / Valor2, , 1)
                bCalcProc = False
            Else
                ContarCalc = InStr(ContarCalc + 1, Express�o, "/")
                GoTo Fun��oDivis�o
            End If
        End If
        ContarCalc = InStr(Express�o, "\") 'Divis�o Inteira
Fun��oDivis�oInteira:
        If ContarCalc > 0 And bCalcProc Then
            TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
            TempCalc1 = FindLastNumber(Express�o, ContarCalc, 1)
            If Len(TempCalc) > 0 And Len(TempCalc1) > 0 Then
                Valor1 = TempCalc: Valor2 = TempCalc1
                Express�o = Replace(Express�o, TempCalc & "\" & TempCalc1, Valor1 \ Valor2, , 1)
                bCalcProc = False
            Else
                ContarCalc = InStr(ContarCalc + 1, Express�o, "\")
                GoTo Fun��oDivis�oInteira
            End If
        End If
        ContarCalc = InStr(Express�o, "*") 'Multiplica��o
Fun��oMultiplica��o:
        If ContarCalc > 0 And bCalcProc Then
            TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
            TempCalc1 = FindLastNumber(Express�o, ContarCalc, 1)
            If Len(TempCalc) > 0 And Len(TempCalc1) > 0 Then
                Valor1 = TempCalc: Valor2 = TempCalc1
                Express�o = Replace(Express�o, TempCalc & "*" & TempCalc1, Valor1 * Valor2, , 1)
                bCalcProc = False
            Else
                ContarCalc = InStr(ContarCalc + 1, Express�o, "*")
                GoTo Fun��oMultiplica��o
            End If
        End If
        ContarCalc = InStr(Express�o, "-") 'Subtra��o
Fun��oSubtra��o:
        If ContarCalc > 0 And bCalcProc Then
            TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
            TempCalc1 = FindLastNumber(Express�o, ContarCalc, 1)
            If Len(TempCalc) > 0 And Len(TempCalc1) > 0 Then
                Valor1 = TempCalc: Valor2 = TempCalc1
                Express�o = Replace(Express�o, TempCalc & "-" & TempCalc1, Valor1 - Valor2, , 1)
                bCalcProc = False
            Else
                ContarCalc = InStr(ContarCalc + 1, Express�o, "-")
                GoTo Fun��oSubtra��o
            End If
        End If
        ContarCalc = InStr(Express�o, "+") 'Adi��o
Fun��oAdi��o:
        If ContarCalc > 0 And bCalcProc Then
            TempCalc = FindLastNumber(Express�o, ContarCalc, -1)
            TempCalc1 = FindLastNumber(Express�o, ContarCalc, 1)
            If Len(TempCalc) > 0 And Len(TempCalc1) > 0 Then
                Valor1 = TempCalc: Valor2 = TempCalc1
                Express�o = Replace(Express�o, TempCalc & "+" & TempCalc1, Valor1 + Valor2, , 1)
                bCalcProc = False
            Else
                ContarCalc = InStr(ContarCalc + 1, Express�o, "+") 'Adi��o
                GoTo Fun��oAdi��o
            End If
        End If
        ContarCalc = 1
    If Negativar(Express�o) Then bCalcProc = False
TirarPar�nteses: 'Tirar Par�nteses
    TempCalc = ExtrairIntervalo(Express�o, "(", ")", ContarCalc)
    If IsNumeric(TempCalc) Then
        Express�o = Replace(Express�o, "(" & TempCalc & ")", TempCalc)
    End If
    ContarCalc = ContarCalc + 1
    If ContarCalc < Len(Express�o) Then GoTo TirarPar�nteses

    If Negativar(Express�o) Then bCalcProc = False

    ContarCalc1 = ContarCalc1 + 1: If ContarCalc1 > 1000 Then Exit Function 'Seguran�a
    Loop Until bCalcProc = True
    If Passo = True Then Debug.Print Express�o
    CalcularExpress�o = Express�o

TratErr:
    Call DepurarErros("CalcularExpress�o")
End Function
	*/
	return 0.0;
}

gchar *be_urlencode (const gchar *sTexto)
{
/*******************************************************************************
// be_urlencode
// Retorna o sTexto convertido em URLencode, de acordo com RFC2396.
// Utilizado em nos m�todos POST e GET na comunica��o com sites.
// Argumentos:
// 	sTexto : texto original
// 	Retorno: novo texto, convertido
*******************************************************************************/
	GString *String = NULL;
	gchar sValidos[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_.!~*'"};//caract�res v�lidos
	gchar sHex[] = {"0123456789ABCDEF"};//hexadecimais v�lidos
	gint nCont = 0;

	//iniciando tipo
	String = g_string_new (NULL);

	//loop de caract�r por caract�r.
	//quando um caract� � validado o mesmo � inclu�do ao final de String
	for (nCont = 0; nCont < strlen (sTexto); nCont++)
	{
		//verifica se � um espa�o em branco
		if (sTexto[nCont] == ' ')
			String = g_string_append_c (String, '+');
		//verifica se est� na lista dos v�lidos
		else if (strchr (sValidos, sTexto[nCont]))
			String = g_string_append_c (String, sTexto[nCont]);
		//n�o est�
		else
		{
			String = g_string_append_c (String, '%');
			String = g_string_append_c (String, sHex[((sTexto[nCont] >> 4) & 0xF)]);
			String = g_string_append_c (String, sHex[(sTexto[nCont] & 0xF)]);
		}
	}

	return BE_string_free (String);
}

gchar *be_urldecode (const gchar *sTexto)
{
/*******************************************************************************
// be_urldecode
// Retorna o sTexto convertido de URLencode, de acordo com RFC2396.
// Utilizado em nos m�todos POST e GET na comunica��o com sites.
// Argumentos:
// 	sTexto : texto convertido
// 	Retorno: novo texto, original
*******************************************************************************/
	GString *String = NULL;
	gchar sHex[] = {"0123456789ABCDEF"};//hexadecimais v�lidos
	gint nCont = 0;
	gint nDig1 = 0, nDig2 = 0;

	//iniciando tipo
	String = g_string_new (NULL);

	//loop de caract�r por caract�r.
	//quando um caract� � validado o mesmo � inclu�do ao final de String
	for (nCont = 0; nCont < strlen (sTexto); nCont++)
	{
		//verifica se � +
		if (sTexto[nCont] == '+')
			String = g_string_append_c (String, ' ');
		else if (sTexto[nCont] == '%' && nCont < (strlen (sTexto) - 2))
		{
			//convers�o hexadecimal para decimal
			nDig2 = (strchr (sHex, sTexto[nCont + 1]) - sHex);//recuperar d�gito 2
			nDig1 = (strchr (sHex, sTexto[nCont + 2]) - sHex);//recuperar d�gito 1
			if (nDig1 >= 0 && nDig2 >= 0)
				String = g_string_append_c (String, nDig1 + nDig2 * 16);
			nCont += 2;//avan�ar 2 caract�res
		}
		//resto
		else
			String = g_string_append_c (String, sTexto[nCont]);
	}

	return BE_string_free (String);
}

gchar *be_htmldecode (const gchar *sTexto)
{
/*******************************************************************************
// be_htmldecode
// Retorna um texto convertendo os carat�res especiais. &#?;
// Argumentos:
// 	sTexto : texto com carat�res especiais
// 	Retorno: novo texto, original
*******************************************************************************/
	GString *String = NULL;
	gint nCont = 0, nCont1 = 0;
	gchar *sTemp = NULL;

	//iniciando tipo
	String = g_string_new (NULL);

	//loop de caract�r por caract�r.
	//quando um caract� � validado o mesmo � inclu�do ao final de String
	for (nCont = 0; nCont < strlen (sTexto); nCont++)
	{
		if (sTexto[nCont] == '&' && sTexto[nCont+1] == '#')
		{
			for (nCont1 = nCont+2; sTexto[nCont1] != ';' && nCont1 < strlen (sTexto); nCont1++);//localizar
			if ((nCont1 - nCont) <= 5)
			{
				BE_free (sTemp);
				sTemp = g_strdup (sTexto+nCont+2);
				sTemp[nCont1 - nCont - 2] = '\0';
				if (sTemp && atoi(sTemp))
				{
					String = g_string_append_c (String, atoi(sTemp));
					nCont = nCont1;//avan�ar caract�res
				}
			}
		}
		//resto
		else
			String = g_string_append_c (String, sTexto[nCont]);
	}
	//limpar
	BE_free (sTemp);

	return BE_string_free (String);
}

//+----------------------------------------------------------------------------+
//| BETempo                                                                    |
//+----------------------------------------------------------------------------+
gchar *be_tempo_tm (struct tm *Time, const gchar *sFormato)
{
/*******************************************************************************
// be_tempo_tm
// Retorna a data e hora de Time, no formato.
// Argumentos:
// 	Time : data e/ou hora
// 	sFormato : formato v�lido para data e/ou hora
// 	Retorno: data e hora
*******************************************************************************/
	gchar sBuffer[_BE_time_SIZE];

	strftime (sBuffer, _BE_time_SIZE, sFormato, Time);//formatando a data e hora de Time

	return g_strdup (sBuffer);
}

struct tm *_be_tempo_tm (const gboolean bGMT)
{
/*******************************************************************************
// _be_tempo_tm
// Retorna a data e hora do sistema, de acordo com o GTM.
// Argumentos:
// 	bGMT : indica se � para retorna em GMT
// 	Retorno: estrutura com a data e/ou hora atuais
*******************************************************************************/
	time_t curtime;
	struct tm *loctime = NULL;

	//Tempo atual
	curtime = time (NULL);
	loctime = bGMT ? gmtime (&curtime) : localtime (&curtime);//convertendo para apresenta��o
	return loctime;
}

gchar *_be_tempo (const gchar *sFormato, const gboolean bGMT)
{
/*******************************************************************************
// _be_tempo
// Retorna a data e hora do sistema, de acordo com o GTM, no formato sFormato.
// Argumentos:
// 	bGMT : indica se � para retorna em GMT
// 	sFormato : formato v�lido para data e/ou hora
// 	Retorno: data e hora atuais
*******************************************************************************/
	return be_tempo_tm (_be_tempo_tm (bGMT), sFormato);
}

gint be_tempo_fuso ()
{
/*******************************************************************************
// be_tempo_fuso
// Retorna o fuso hor�rio atual.
// Argumentos:
// 	Retorno: numera��o do fuso hor�rio
*******************************************************************************/
	gchar *sTemp = NULL;
	gint nFuso = 0;

	sTemp = BE_tempo_gmt ("%z");
	nFuso = atoi (sTemp) / 100;

	BE_free (sTemp);

	return nFuso;
}

GDate *be_tempo_GDate ()
{
/*******************************************************************************
// be_tempo_GDate
// Cria um GDate com o tempo atual.
// Argumentos:
// 	Retorno: novo GDate com o tempo atual
*******************************************************************************/
	GDate *Data = NULL;

	Data = g_date_new ();
	g_date_set_time (Data, time (NULL));

	return Data;
}

gint be_tempo_sec ()
{
/*******************************************************************************
// be_tempo_sec
// Retorna os segundos do sistema.
// Argumentos:
// 	Retorno: segundos em n�meros
*******************************************************************************/
	GTimeVal Tempo;

	g_get_current_time (&Tempo);

	return (gint) Tempo.tv_sec;//segundos
}

gint be_tempo_usec ()
{
/*******************************************************************************
// be_tempo_usec
// Retorna os microsegundos do sistema.
// Argumentos:
//     Retorno: microsegundos em n�meros
*******************************************************************************/
	GTimeVal Tempo;

	g_get_current_time (&Tempo);

	return (gint) Tempo.tv_usec;//microsegundos
}


/*
+------------------------------------------------------------------------------+
| BEArquivo                                                                    |
+------------------------------------------------------------------------------+
*/

gchar *be_arquivo_nome_sugerido (const gchar *sNome, gint nComprimentoMax, gboolean bTempo)
{
/*******************************************************************************
// be_arquivo_nome_sugerido
// Retorna uma complementa��o para o sNome e algumas convers�es.
// Esta complementa��o � uma concatena��o da data e hora atual.
// As convers�es s�o: substituir espa�os por underlines, tudo em min�sculo, sem acentua��es.
// Tanto faz se o arquivo tem ou n�o uma extens�o.
// Ex.: C�pia de arquivos => copia_de_arq_19-12-2007_18-05-20
//      C�pia de arquivos.txt => copia_de_arq_19-12-2007_18-05-20.txt
// Argumentos:
//     sNome : nome original
//     nComprimentoMax : comprimento m�ximo retornado, deve estar entre 1 e BEARQUIVO_COMPRIMENTO_MAX
//     bTempo : se for falso a data e hora n�o ser�o inclu�das
//     Retorno: novo nome
*******************************************************************************/
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	gchar *sExtensao = NULL;

	//recuperar extens�o
	sExtensao = be_arquivo_extensao (sNome);
	//remover extens�o, se houver
	if (sExtensao)
	{
		sTemp1 = be_substr (sNome, 0, (strlen (sExtensao) + 1) * -1);//excluir do final
		sTemp = be_strclear (sTemp1, BEGLOBAL_CONVERSAO_SEM_ACENTOS);//nome sem acentua��es
	}
	else
		sTemp = be_strclear (sNome, BEGLOBAL_CONVERSAO_SEM_ACENTOS);//nome sem acentua��es
	//verificar comprimento m�nimo, o padr�o � BEARQUIVO_COMPRIMENTO_MAX
	if (nComprimentoMax <= 0 || nComprimentoMax > BEARQUIVO_COMPRIMENTO_MAX)
		nComprimentoMax = BEARQUIVO_COMPRIMENTO_MAX;
    //descontar a data e hora ser necess�rios
	if (bTempo && nComprimentoMax > (BEARQUIVO_COMPRIMENTO_MAX - 20))
		nComprimentoMax = BEARQUIVO_COMPRIMENTO_MAX - 20;
    //verificar comprimento m�ximo, incluir fim de linha para n�o ultrapassar
	if (strlen (sTemp) > nComprimentoMax)
		sTemp[nComprimentoMax] = '\0';
	BE_free (sTemp1);
	sTemp1 = sTemp;
	sTemp = g_ascii_strdown (sTemp1, -1);//converter para min�sculas
	BE_free (sTemp1);
	sTemp1 = sTemp;
	//incluir data e hora
	if (bTempo)
	{
		sTemp2 = BE_tempo ("%d-%m-%Y_%H-%M-%S");//data e hora atuais
		sTemp = g_strdup_printf ("%s_%s", sTemp1, sTemp2);//concatenar
		BE_free (sTemp1);
		sTemp1 = sTemp;
	}
	sTemp = be_strreplace (sTemp1, " ", "_");//substituir espa�os por underlines
	//incluir extens�o, se houver
	if (sExtensao)
	{
		BE_free (sTemp1);
		sTemp1 = sTemp;
		sTemp = g_strdup_printf ("%s.%s", sTemp1, sExtensao);
	}

	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sExtensao);

	return sTemp;
}

gchar *be_arquivo_extensao (const gchar *sArquivo)
{
/*******************************************************************************
// be_arquivo_extensao
// Retorna a extens�o de um arquivo.
// Argumentos:
//     sArquivo : nome do arquivo, pode incluir o caminho tamb�m
//     Retorno: novo texto com a extens�o do arquivo, ou nulo
*******************************************************************************/
	gint nPos = 0;
	gchar *sTemp  = NULL, *sTemp1 = NULL;

	sTemp1 = g_path_get_basename (sArquivo);//recuperar somente o nome do arquivo, sem caminho
	if (sTemp1)
	{
		nPos = be_strrpos (sTemp1, ".", 0);//localizar in�cio da extens�o
		if (nPos >= 0)
			sTemp = g_strdup (sTemp1 + nPos + 1);//copiar somente a extens�o
	}
	BE_free (sTemp1);

	return sTemp;
}

gint be_arquivo_tamanho (const gchar *sArquivo)
{
/*******************************************************************************
// be_arquivo_tamanho
// Retorna o tamanho do arquivo, em bytes.
// Argumentos:
//     sArquivo : nome do arquivo, deve incluir o caminho completo
//     Retorno: tamanho em bytes
*******************************************************************************/
	struct stat Arquivo;

	if (sArquivo && stat (sArquivo, &Arquivo) == 0)//carrega estrutura a Arquivo
		return (int) Arquivo.st_size;//devolve o tamanho

	return 0;
}

gboolean be_arquivo_localidade_UTF8 ()
{
/*******************************************************************************
// be_arquivo_localidade_UTF8
// Retorna verdadeiro se os arquivos devem ser manipulados em UTF8.
// Em MS Windows sempre � verdadeiro.
// Argumentos:
//     Retorno: verdadeiro para UTF8
*******************************************************************************/
	#ifdef G_OS_WIN32
		return TRUE;
	#endif
	return g_get_charset (NULL);
}

gchar *be_arquivo_aplicativo (BEGLOBAL_APLICATIVOS nTipo)
{
/*******************************************************************************
// be_arquivo_aplicativo
// Retorna a linha de comando do aplicativo pr�-determinado para tarefas especiais.
// Os tipos pr�-determinados est�o definidos em BEGLOBAL_APLICATIVOS.
// As linhas de comandos pr�-determinadas devem estar dentro do arquivo definido em BEGLOBAL_CONFIG_LOCAL.
// Argumentos:
//     sTipo : um dos tipos em BEGLOBAL_APLICATIVOS
//     Retorno: linha de comando para execu��o do aplicativo, ou nulo
*******************************************************************************/
	GKeyFile *Ini = NULL;
	gchar *sSaida = NULL;
	#ifdef G_OS_UNIX
		const gchar *sSO = "linux";
	#endif
	#ifdef G_OS_WIN32
		const gchar *sSO = "windows";
	#endif

	//carregar configura��o local
	Ini = g_key_file_new ();
	if (g_key_file_load_from_file (Ini, BEGLOBAL_CONFIG_LOCAL, G_KEY_FILE_NONE, NULL))
	{
		switch (nTipo)
		{
            case BEGLOBAL_APLICATIVO_PDF://leitor de PDF
                sSaida = g_key_file_get_string (Ini, "lerpdf", sSO, NULL);
                break;
            case BEGLOBAL_APLICATIVO_NAVEGADOR://navegador de internet
                sSaida = g_key_file_get_string (Ini, "navegador", sSO, NULL);
                break;
            case BEGLOBAL_APLICATIVO_EMAIL://enviar e-mail
                sSaida = g_key_file_get_string (Ini, "enviaremail", sSO, NULL);
                break;
            case BEGLOBAL_APLICATIVO_PNG://leitor de PNG
                sSaida = g_key_file_get_string (Ini, "lerpng", sSO, NULL);
                break;
            case BEGLOBAL_APLICATIVO_CALCULADORA://calculadora
                sSaida = g_key_file_get_string (Ini, "calculadora", sSO, NULL);
                break;
            default://nada
                break;
		}
		//fechar INI
		g_key_file_free (Ini);
	}

	return sSaida;
}

gchar *be_diretorio_local (gboolean bSeparador)
{
/*******************************************************************************
// be_diretorio_local
// Retorna o caminho completo do diret�rio de configura��es locais do sistema atual.
// O caminho � formado pelo diret�rio padr�o do usu�rio atual + DIRETORIO.
// Se este caminho n�o existir ser� criado.
// Ex.: /home/usuario99/.cia/
//      c:\Documents and Settings\usuario99\.cia\
// Argumentos:
//     bSeparador : se no final haver� a barra separadora
//     Retorno: novo caminho completo
*******************************************************************************/
    gchar *sDir = NULL;
    gchar *sTemp = NULL;

    sDir = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, DIRETORIO);//recuperar diret�rio padr�o + diret�rio do sistema atual
    //verificar se diret�rio � v�lido
    if (!g_file_test (sDir, G_FILE_TEST_IS_DIR))
    {
        #ifdef G_OS_UNIX
            sTemp = g_strdup_printf ("mkdir %s", sDir);//comando para criar diret�rio se n�o existir
        #endif
        #ifdef G_OS_WIN32
            sTemp = g_strdup_printf ("mkdir \"%s\"", sDir);//comando para criar diret�rio se n�o existir
        #endif
        system (sTemp);//executar comando
        BE_free (sTemp);//limpar
    }

    //definir caminho
    if (bSeparador)
        sTemp = g_strdup_printf ("%s%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, DIRETORIO, G_DIR_SEPARATOR_S);//com separador
    else
        sTemp = g_strdup_printf ("%s%s%s", g_get_home_dir (), G_DIR_SEPARATOR_S, DIRETORIO);//sem separador

    return sTemp;
}

gchar *be_diretorio_local_temp (gboolean bSeparador)
{
/*******************************************************************************
// be_diretorio_local_temp
// Retorna o caminho completo do diret�rio tempor�rio, do usu�rio atual.
// Se o caminho n�o existir ser� retornado nulo.
// Ex.: /tmp/
//      C:\Temp\
// Argumentos:
//     bSeparador : se no final haver� a barra separadora
//     Retorno: novo caminho completo, ou nulo
*******************************************************************************/
    gchar *sTemp = NULL;

    //verificar se existe
    if (g_get_tmp_dir ())
    {
        //definir caminho
        if (bSeparador)
            sTemp = g_strdup_printf ("%s%s", g_get_tmp_dir (), G_DIR_SEPARATOR_S);//com separador
        else
            sTemp = g_strdup_printf ("%s", g_get_tmp_dir ());//sem separador
    }

    return sTemp;
}

gchar *be_arquivo_absoluto (const gchar *sArquivo)
{
/*******************************************************************************
// be_arquivo_absoluto
// Garante um caminho completo at� o arquivo sArquivo.
// Se n�o houver um diret�rio definido em sArquivo, ser� inclu�do o diret�rio padr�o do sistema.
// Veja be_diretorio_local().
// Argumentos:
//     sArquivo : arquivo original
//     Retorno: novo caminho completo, ou nulo
*******************************************************************************/
    gchar *sTemp = NULL, *sTemp1 = NULL;

    if (sArquivo)
    {
        sTemp = g_path_get_dirname (sArquivo);//recuperar caminho do arquivo
        if ((sTemp && strlen (sTemp) <= 1) || !sTemp)
        {
            BE_free (sTemp);
            sTemp1 = be_diretorio_local (TRUE);//recuperar caminho padr�o do sistema
            sTemp = g_strdup_printf ("%s%s", sTemp1, sArquivo);//concatenar caminho e arquivo
        }
        else
        {
            BE_free (sTemp);
            if (sArquivo)
                sTemp = g_strdup (sArquivo);//copiar caimnho e arquivo
        }
        //limpar
        BE_free (sTemp1);
    }

    return sTemp;
}

//+----------------------------------------------------------------------------+
//| BEImagens                                                                  |
//+----------------------------------------------------------------------------+
guchar *be_pixbuf_get_pixels (const GdkPixbuf *pixbuf)
{
/*******************************************************************************
// be_pixbuf_get_pixels
// Corre��o de linha de bits e remo��o de transpar�ncia, ideal para PDF.
// Argumentos:
//     pixbuf : imagem em pixbuf
//     Retorno: nova sequencia de bytes/pixels
*******************************************************************************/
	guchar *sPixelsNovo = NULL, *sPixels = NULL;
	guchar *sPixelsNovoP = NULL, *sPixelsP = NULL;
	gint nCont = 0, nCont1 = 0, nComp = 0, nColunaMax = 0, nColunas = 0, nCanais = 0, nLargura = 0, nAltura = 0;

	if (!pixbuf)
		return NULL;

	nComp = BE_pixbuf_get_len (pixbuf);//comprimento
	nCanais = gdk_pixbuf_get_n_channels (pixbuf);//canais
	nLargura = gdk_pixbuf_get_width (pixbuf);//largura
	nAltura = gdk_pixbuf_get_height (pixbuf);//altura
	nColunas = gdk_pixbuf_get_rowstride (pixbuf);//colunas
	nColunaMax = nLargura * nCanais;//colunas calculadas
	sPixels = gdk_pixbuf_get_pixels (pixbuf);//pixels
	sPixelsNovo = (guchar *) g_malloc (nComp * sizeof (guchar));//reservando espa�o

	//Verificar necessidade de corre��o de rowstride
	//Em algums imagens o retorno da fun��o gdk_pixbuf_get_pixels() � incompat�vel com imagens para PDF
	if (nColunas > nColunaMax)
	{
		if (sPixelsNovo)
		{
			sPixelsNovoP = sPixelsNovo;
			sPixelsP = sPixels;
			//loop nas linhas
			for (nCont = 0; nCont < nAltura; nCont++)
			{
				//loop nas colunas
				for (nCont1 = 0; nCont1 < nColunas; nCont1++)
				{
					//ignorar os bytes � mais de cada linha, ignorar a(s) �ltima(s) coluna(s)
					if (nCont1 < nColunaMax)
					{
						memcpy (sPixelsNovoP, sPixelsP, 1);//copiando os bytes/pixels
						sPixelsNovoP++;
					}
					sPixelsP++;
				}
			}
		}
	}
	//tudo certo, s� copiar
	else
	{
		memcpy (sPixelsNovo, sPixels, nComp * sizeof (guchar));//copiar tudo de uma vez
	}

	//verificar necessidade de corre��o de transpar�ncia
	if (nCanais == 4)//4 canais significa que h� um byte � mais para transpar�ncia
	{
		sPixels = sPixelsNovo;
		sPixelsNovo = (guchar *) g_malloc (nComp * sizeof (guchar));//reservando espa�o: o certo seria nComp * 3/4, mas teria que alterar gdk_pixbuf_get_n_channels
		if (sPixelsNovo)
		{
			sPixelsP = sPixels;
			sPixelsNovoP = sPixelsNovo;
			//loop nos bytes/pixels, de 4 em 4
			for (nCont = 0; nCont < nComp; nCont += 4)
			{
				memcpy (sPixelsNovoP, sPixelsP, 3);//copiar somente 3 bytes
				sPixelsNovoP += 3;
				sPixelsP += 4;
			}
		}
		BE_free (sPixels);//liberar
	}

	return sPixelsNovo;
}

//+----------------------------------------------------------------------------+
//| BECliente                                                                  |
//+----------------------------------------------------------------------------+
void _be_empresa_cliente_limpar (BEGLOBAL_Empresa_Cliente *Empresa)
{
/*******************************************************************************
// _be_empresa_cliente_limpar
// Limpar a estrutura Empresa
// Argumentos:
//     Empresa : estrutura BEGLOBAL_Empresa_Cliente
//     Retorno: sem retorno
*******************************************************************************/
	if (!Empresa)
		return;

	BE_free (Empresa->sNomeFantasia);
	BE_free (Empresa->sRazaoSocial);
	BE_free (Empresa->sCNPJ);
	BE_free (Empresa->sIE);
	BE_free (Empresa->sTelefone);
	BE_free (Empresa->sFax);
	BE_free (Empresa->sPABX);
	BE_free (Empresa->sEmail);
	BE_free (Empresa->sSite);
	BE_free (Empresa->sCep);
	BE_free (Empresa->sTipoLogradouro);
	BE_free (Empresa->sLogradouro);
	BE_free (Empresa->sComplemento);
	BE_free (Empresa->sBairro);
	BE_free (Empresa->sCidade);
	BE_free (Empresa->sEstado);
	BE_free (Empresa->sPais);
	BE_pixbuf_free (Empresa->Logotipo);
	BE_free (Empresa);
}

//+----------------------------------------------------------------------------+
//| BEJanelas                                                                  |
//+----------------------------------------------------------------------------+
BEGlobal_Janela *be_janela_mouse_pensando (GtkWidget *window)
{
/*******************************************************************************
// be_janela_mouse_pensando
// Ativa o mouse pensando e bloqueia todos os controles da janela, definida em window.
// Argumentos:
//     window = janela
//     Retorno: estrutura BEGlobal_Janela
*******************************************************************************/
	BEGlobal_Janela *Janela = NULL;
	GdkCursor *cursor = NULL;
	GtkWidget *foco = NULL;

	//ativar gdk
	if (!window->window)
		gtk_widget_realize (window);

	//verificar se o mouse j� n�o est� alterado
	if (GTK_WIDGET_IS_SENSITIVE (window))
	{
		foco = gtk_window_get_focus (GTK_WINDOW (window));//foco atual
		cursor = gdk_cursor_new (GDK_WATCH);//recuperar mouse
		gdk_window_set_cursor (window->window, cursor);//mouse pensando
		gtk_widget_set_sensitive (window, FALSE);//bloquear janela
		gdk_cursor_unref (cursor);

		//criar nova estrutura e apontar para janela e controle com foco
		Janela = g_new (BEGlobal_Janela, 1);
		Janela->window = window;
		Janela->foco = foco;

		//liberar eventos, n�o ficar esperando (DoEvents)
		while (gtk_events_pending ())
			gtk_main_iteration ();
	}

	return Janela;
}

void _be_janela_mouse_normal (BEGlobal_Janela *Janela, gboolean bFoco)
{
/*******************************************************************************
// _be_janela_mouse_normal
// Devolver o mouse padr�o, liberar a janela bloqueada e manter o foco no �ltimo controle ativo.
// Argumentos:
//     Janela : estrutura BEGlobal_Janela, ser� liberada
//     bFoco : se vai retornar o foco ao �ltimo controle, antes be_janela_mouse_pensando()
//     Retorno: sem retorno
*******************************************************************************/
	if (!Janela)
		return;

	if (Janela->window && GTK_IS_WIDGET (Janela->window) && GDK_IS_WINDOW (Janela->window->window))
	{
        gdk_window_set_cursor (Janela->window->window, NULL);//mouse padr�o
        gtk_widget_set_sensitive (Janela->window, TRUE);//liberar janela
	}

	//liberar eventos, n�o ficar esperando (DoEvents)
	//while (gtk_events_pending ())
	//	gtk_main_iteration ();

	//devolver o foco
	if (bFoco && Janela->foco && GTK_IS_WIDGET (Janela->foco))
		gtk_widget_grab_focus (Janela->foco);


	BE_free (Janela);//liberar
}
