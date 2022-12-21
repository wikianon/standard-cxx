#include "benet.h"

/*******************************************************************************
Contém funções para facilitar a integração da biblioteca cURl (http://curl.haxx.se/) com o programa.
Em muitos casos é mais fácil fazer o uso direto do arquivo executável "curl".
Interação com sites, envio e recebimento de informações: HTTP (GET e POST), HTTPS, FTP, FTPS, SFTP, Downloads, PROXY, Cookies, Telnet, Emails (POP3 e SMTP).
*******************************************************************************/

BENET *be_net_iniciar (const gchar *sURL, const gchar *sProxy, const gchar *sUsuarioSenha, const gchar *sNavegador)
{
/*******************************************************************************
Retorna uma estrutura para comunicação entre sites, numa mesma sessão.
Progresso exibido na saída padrão.
Argumentos:
	sProxy : caminho:porta do servidor proxy, ou NULL
	sUsuarioSenha : usuário:senha se houver, ou NULL
	sNavegador : nome User-Agent do navegador, ou NULL
	Retorno: estrutura
*******************************************************************************/
	BENET *Site = NULL;//ponteiro de estrutura

	//iniciar biblioteca cURL
	if ((curl_global_init (CURL_GLOBAL_ALL) == CURLE_OK))
	{
		//criar nova estrutura
		Site = g_new (BENET, 1);
		Site->URL = NULL;
		Site->sURL = g_strdup (sURL);//site inicial
		Site->sCabecalho = NULL;
		Site->sConteudo = NULL;
		Site->sErro = NULL;
		Site->Formularios = NULL;
		Site->bSubmiter = FALSE;

		//iniciar biblioteca cURL
		if ((Site->URL = curl_easy_init ()))
		{
			//esperar até 10 min para timeout
			curl_easy_setopt (Site->URL, CURLOPT_TIMEOUT, 10 * 60);
			//seguir redirecionamentos
			curl_easy_setopt (Site->URL, CURLOPT_AUTOREFERER, 1);
			curl_easy_setopt (Site->URL, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt (Site->URL, CURLOPT_MAXREDIRS, -1);
			//preparar POST
			curl_easy_setopt (Site->URL, CURLOPT_POST, 0);
			//sempre aceitar SSL, mesmo sem validade
			curl_easy_setopt (Site->URL, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt (Site->URL, CURLOPT_SSL_VERIFYPEER, 0);
			//verificar proxy
			if (sProxy)
			{
				curl_easy_setopt (Site->URL, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
				curl_easy_setopt (Site->URL, CURLOPT_PROXY, sProxy);
				//verificar senha do proxy
				if (sUsuarioSenha)
					curl_easy_setopt (Site->URL, CURLOPT_PROXYUSERPWD, sUsuarioSenha);
			}
			//verificar senha de autenticação
			else if (sUsuarioSenha)
				curl_easy_setopt (Site->URL, CURLOPT_USERPWD, sUsuarioSenha);
			//sempre enviar senhas se houver
			curl_easy_setopt (Site->URL, CURLOPT_UNRESTRICTED_AUTH, 1);
			//definir navegador
			if (sNavegador)
				curl_easy_setopt (Site->URL, CURLOPT_USERAGENT, sNavegador);
			//definir função de retorno das partes, dados e cabeçalho
			curl_easy_setopt (Site->URL, CURLOPT_WRITEFUNCTION, _be_net_getURL_download);
			curl_easy_setopt (Site->URL, CURLOPT_HEADERFUNCTION, _be_net_getURL_download);
			//cookies
			curl_easy_setopt (Site->URL, CURLOPT_COOKIESESSION, 1);
			curl_easy_setopt (Site->URL, CURLOPT_COOKIEFILE, "cookies.txt");
			curl_easy_setopt (Site->URL, CURLOPT_COOKIEJAR, "cookies.txt");
			//progresso dao download e uploads
			curl_easy_setopt (Site->URL, CURLOPT_NOPROGRESS, 0);
			curl_easy_setopt (Site->URL, CURLOPT_PROGRESSFUNCTION, _be_net_progresso);
			curl_easy_setopt (Site->URL, CURLOPT_PROGRESSDATA, Site);
		}
	}

	return Site;//estrutura inicial
}

int _be_net_progresso (void *Dados, double dltotal, double dlnow, double ultotal, double ulnow)
{
//http://curl.haxx.se/lxr/source/docs/examples/curlgtk.c
/*******************************************************************************
Esta função é somente um callback da função curl_easy_setopt().
Ela é chamada na operação de transferência de qualquer informação.
Utilizada somente na exibição em saída padrão.
Argumentos:
    sStream : conteúdo da informação
    nVezes : quantidade de pedaços que a informação foi dividida
    nStream : comprimento do conteúdo
    Buffer : informação extra, repassada pela função que chamou esta
    Retorno: quantidade de bytes processados
*******************************************************************************/
	BENET *Site = NULL;//ponteiro de estrutura
	gchar *sTemp = NULL;

	if (Dados)
	{
		Site = (BENET *) Dados;
		printf ("Site: %s\n", Site->sURL);
	}
	sTemp = BE_tempo ("%d/%m/%Y %H:%M:%S");
	printf ("\tTempo: %s Bytes: %0.f de %0.f, Tempo Estimado: %0.f de %0.f\n", sTemp, dlnow, dltotal, ulnow, ultotal);

	//limpar
	BE_free (sTemp);

	return 0;//0 para continuar
}

void _be_net_finalizar (BENET *Site)
{
/*******************************************************************************
Limpa a estrutura BENET da memória
Argumentos:
	Site : estrutura carregada por be_net_receberCURL
	Sem retorno.
*******************************************************************************/
	//sair se Site for inválido
	if (!Site)
		return;

	//finalizar conexão
	if (Site->URL)
		curl_easy_cleanup (Site->URL);
	//liberar biblioteca
	curl_global_cleanup ();

	//limpar da memória
	BE_free (Site->sURL);
	BE_free (Site->sCabecalho);
	BE_free (Site->sConteudo);
	BE_free (Site->sErro);

	//limpar listas
	_BE_net_limpar_formularios (Site->Formularios);

	BE_free (Site);
}

void _be_net_limpar_formularios (GSList *Formularios)
{
	BENET_formulario *Formulario = NULL;//ponteiro de estrutura
	BENET_campo *Campo = NULL;//ponteiro de estrutura
	GSList *ListaL = NULL, *ListaL1 = NULL;//lista

	//loop nos formulários
	for (ListaL = Formularios; ListaL; ListaL = g_slist_next (ListaL))
	{
		Formulario = (BENET_formulario *) ListaL->data;
			//printf ("%s, %s, %s\n", Formulario->sNome, Formulario->sMetodo, Formulario->sAcao);//testes
		BE_free (Formulario->sNome);
		BE_free (Formulario->sMetodo);
		BE_free (Formulario->sAcao);
		//loop nos campos
		for (ListaL1 = Formulario->Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Campo = (BENET_campo *) ListaL1->data;
				//printf ("\t%s = %s [%s]\n", Campo->sNome, Campo->sValor, Campo->sTipo);//testes
			BE_free (Campo->sNome);
			BE_free (Campo->sTipo);
			BE_free (Campo->sValor);
			BE_free (Campo);
		}
		BE_free (Formulario);
	}
	BE_slist_free (Formularios);
}

gboolean be_net_receber (BENET *Site)
{
	BENET_formulario *Formulario = NULL;//ponteiro de estrutura
	BENET_campo *Campo = NULL;//ponteiro de estrutura
	CURLcode cCode;
	GString *Conteudo = NULL, *Cabecalho = NULL, *MetodoPOST = NULL, *MetodoGET = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL;//lista
	gchar *sTemp = NULL, *sTemp1 = NULL, *sConteudo = NULL, *sTagFormulario = NULL, *sPost = NULL;
	gint nComp = 0;
	gint nPos = 0, nPos1 = 0, nPosI = 0, nPosF = 0;

	if (Site && Site->URL && Site->sURL)
	{
		//limpar erros
		BE_free (Site->sErro);
		//repasar buffer para receber dados
		Conteudo = g_string_new ("");//preparar buffer
		curl_easy_setopt (Site->URL, CURLOPT_WRITEDATA, Conteudo);
		//repasar buffer para receber dados
		Cabecalho = g_string_new ("");//preparar buffer Cabecalho
		curl_easy_setopt (Site->URL, CURLOPT_HEADERDATA, Cabecalho);

		MetodoGET = g_string_new ("");//preparar buffer, AINDA NÃO IMPLEMENTADO
		//enviar POST
		if (Site->bSubmiter)
		{
			MetodoPOST = g_string_new ("");//preparar buffer
			//loop nos formulários existentes
			for (ListaL = Site->Formularios; ListaL; ListaL = g_slist_next (ListaL))
			{
				Formulario = (BENET_formulario *) ListaL->data;
				printf ("\t\tFormulario: %s=%s\n", Formulario->sNome, Formulario->sAcao);//informar andamento
				//verificar método POST
				if (Formulario->sMetodo && be_strpos_ascii (Formulario->sMetodo, "POST", 0) == 0)
				{
					//loop campos existentes
					for (ListaL1 = Formulario->Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						//recuperar nomes=valores
						Campo = (BENET_campo *) ListaL1->data;
						//somente campos ativos
						if (Campo->bAtivo)
						{
							//adicionar nome, se houver
							if (Campo->sNome)
							{
								BE_free (sTemp);
								sTemp = curl_easy_escape (Site->URL, Campo->sNome, 0);
								MetodoPOST = g_string_append (MetodoPOST, sTemp);
								//adicionar = (igual)
								MetodoPOST = g_string_append (MetodoPOST, "=");
								//adicionar valor, se houver
								if (Campo->sValor)
								{
									BE_free (sTemp);
									sTemp = curl_easy_escape (Site->URL, Campo->sValor, 0);
									MetodoPOST = g_string_append (MetodoPOST, sTemp);
								}
								//adicionar &
								MetodoPOST = g_string_append (MetodoPOST, "&");
							}
						}
						printf ("\t\t\tCampo: %s=%s[%d] (%s)\n", Campo->sNome, Campo->sValor, Campo->bAtivo, Campo->sTipo);//informar andamento
					}
					//excluir o último caractére
					if (MetodoPOST->len > 0)
						MetodoPOST = g_string_truncate (MetodoPOST, MetodoPOST->len - 1);
				}
			}
			//enviar método post
			if (MetodoPOST->len > 0)
			{
				BE_free (sPost);
				sPost = BE_string_free (MetodoPOST);
				printf ("\tEnviar POST >>>>>>>>>:\n%s\n", sPost);//informar andamento
				curl_easy_setopt (Site->URL, CURLOPT_POSTFIELDS, sPost);
			}
			else
				g_string_free (MetodoPOST, TRUE);
		}
		//substituir &amp; por & nos links
		BE_free (sTemp);
		sTemp = Site->sURL;
		Site->sURL = be_strreplace (sTemp, "&amp;", "&");
		//site
		curl_easy_setopt (Site->URL, CURLOPT_URL, Site->sURL);
		//**********************************************************************
		cCode = curl_easy_perform (Site->URL);//ação, receber site
		//**********************************************************************
		//verificar se funcionou
		if(cCode == CURLE_OK)
		{
			//desmarcar submeter
			Site->bSubmiter = FALSE;
			//preparar conteúdo
			nComp = Conteudo->len;//comprimento do Conteudo
			BE_free (Site->sConteudo);
			BE_free (sTemp);
			BE_free (sTemp1);
			sTemp = BE_string_free (Conteudo);//limpar
			sTemp1 = curl_easy_unescape (Site->URL, sTemp, nComp, NULL);//decodificar os caractesres em hexadecimal (HTML) para normais, %
			Site->sConteudo = be_htmldecode (sTemp1);//decodificar os caractesres em hexadecimal (HTML) para normais, &#
			//preparar cabeçalho
			BE_free (Site->sCabecalho);
			Site->sCabecalho = BE_string_free (Cabecalho);//limpar
			//limpar estrutrura de formulários
			_BE_net_limpar_formularios (Site->Formularios);

			//preparar nova estrutura, de formulários
			//preparar saída
			BE_free (sConteudo);
			sConteudo = g_strdup (Site->sConteudo);
			BE_free (sTemp);
			sTemp = sConteudo;
			sConteudo = be_strreplace (sTemp, "= ", "=");
			BE_free (sTemp);
			sTemp = sConteudo;
			sConteudo = be_strreplace (sTemp, " =", "=");
			BE_free (sTemp);
			sTemp = sConteudo;
			sConteudo = be_strreplace (sTemp, "\"", "'");
			BE_free (sTemp);
			sTemp = sConteudo;
			sConteudo = be_strreplace (sTemp, ">", " >");
			//extrair formulários
			nPosI = be_strpos_ascii (sConteudo, "<form", 0);
			//loop de novos formulários
			while (nPosI >= 0)
			{
				//posição final, do formulário
				nPosF = be_strpos_ascii (sConteudo, "</form", nPosI);
				//extrair tag formulário
				BE_free (sTagFormulario);
				sTagFormulario = be_strextract_ascii (sConteudo, " ", ">", nPosI);
				//criar nova estrutura, para formulário
				Formulario = g_new (BENET_formulario, 1);
				Formulario->sNome = be_strextract_ascii (sTagFormulario, "name='", "'", 0);//recuperar nome
				//segunda tentativa
				if (!Formulario->sNome)
					Formulario->sNome = be_strextract_ascii (sTagFormulario, "name=", " ", 0);//recuperar nome
				Formulario->sMetodo = be_strextract_ascii (sTagFormulario, "method='", "'", 0);//recuperar método
				//segunda tentativa
				if (!Formulario->sMetodo)
					Formulario->sMetodo = be_strextract_ascii (sTagFormulario, "method=", " ", 0);//recuperar método
				Formulario->sAcao = be_strextract_ascii (sTagFormulario, "action='", "'", 0);//recuperar ação
				//segunda tentativa
				if (!Formulario->sAcao)
					Formulario->sAcao = be_strextract_ascii (sTagFormulario, "action=", " ", 0);//recuperar ação
				Formulario->Campos = NULL;
				//anexar estrutura na lista
				Site->Formularios = g_slist_append (Site->Formularios, Formulario);
				//limpar
				BE_free (sTagFormulario);
				//preparar formulario
				sTagFormulario = be_substr (sConteudo, nPosI, nPosF - nPosI);
				//extrair campos INPUT
				nPos = 0;
				BE_free (sTemp);
				sTemp = be_strextract_ascii (sTagFormulario, "<input ", ">", nPos);
					//printf ("%s\n", sTagFormulario);//testes
				//loop de novos campos
				while (sTemp)
				{
						//printf ("%s\n", sTemp);//testes
					//criar campo
					Campo = g_new (BENET_campo, 1);
					Campo->bAtivo = TRUE;
					Campo->sNome = be_strextract_ascii (sTemp, "name='", "'", 0);
					//segunda tentativa
					if (!Campo->sNome)
						Campo->sNome = be_strextract_ascii (sTemp, "name=", " ", 0);
					Campo->sTipo = be_strextract_ascii (sTemp, "type='", "'", 0);
					//segunda tentativa
					if (!Campo->sTipo)
						Campo->sTipo = be_strextract_ascii (sTemp, "type=", " ", 0);
					//verificar botão
					if (Campo->sTipo && be_strpos_ascii (Campo->sTipo, "submit", 0) == 0)
						Campo->bAtivo = FALSE;//desativar por padrão
					Campo->sValor = be_strextract_ascii (sTemp, "value='", "'", 0);
					//segunda tentativa
					if (!Campo->sValor)
						Campo->sValor = be_strextract_ascii (sTemp, "value=", " ", 0);
					//verificar tipo file
					if (!Campo->sValor)
					{
						Campo->sValor = be_strextract_ascii (sTemp, "accept='", "'", 0);
						//segunda tentativa
						if (!Campo->sValor)
							Campo->sValor = be_strextract_ascii (sTemp, "accept=", " ", 0);
					}
					//verificar tipo imagem
					if (!Campo->sValor)
					{
						Campo->sValor = be_strextract_ascii (sTemp, "src='", "'", 0);
						//segunda tentativa
						if (!Campo->sValor)
							Campo->sValor = be_strextract_ascii (sTemp, "src=", " ", 0);
					}
					//anexar estrutura na lista
					Formulario->Campos = g_slist_append (Formulario->Campos, Campo);
					//avançar posição
					nPos = be_strpos_ascii (sTagFormulario, "<input", nPos) + 1;
					//extrair campos
					BE_free (sTemp);
					sTemp = be_strextract_ascii (sTagFormulario, "<input ", ">", nPos);					
				}
				//extrair campos TEXTAREA
				nPos = 0;
				BE_free (sTemp);
				sTemp = be_strextract_ascii (sTagFormulario, "<textarea ", "/textarea", nPos);
				//loop de campos
				while (sTemp)
				{
						//printf ("%s\n", sTemp);//testes
					//criar campo
					Campo = g_new (BENET_campo, 1);
					Campo->bAtivo = TRUE;
					Campo->sNome = be_strextract_ascii (sTemp, "name='", "'", 0);
					//segunda tentativa
					if (!Campo->sNome)
						Campo->sNome = be_strextract_ascii (sTemp, "name=", " ", 0);
					Campo->sTipo = g_strdup ("textarea");
					Campo->sValor = be_strextract_ascii (sTemp, ">", "<", 0);
					//anexar estrutura na lista
					Formulario->Campos = g_slist_append (Formulario->Campos, Campo);
					//avançar posição
					nPos = be_strpos_ascii (sTagFormulario, "<textarea", nPos) + 1;
					//extrair campos
					BE_free (sTemp);
					sTemp = be_strextract_ascii (sTagFormulario, "<textarea ", "/textarea", nPos);
				}
				//extrair campos SELECT
				nPos = 0;
				BE_free (sTemp);
				sTemp = be_strextract_ascii (sTagFormulario, "<select ", "/select", nPos);
				//loop de campos
				while (sTemp)
				{
						//printf ("%s\n", sTemp);//testes
					//criar campo
					Campo = g_new (BENET_campo, 1);
					Campo->bAtivo = TRUE;
					Campo->sNome = be_strextract_ascii (sTemp, "name='", "'", 0);
					//segunda tentativa
					if (!Campo->sNome)
						Campo->sNome = be_strextract_ascii (sTemp, "name=", " ", 0);
					Campo->sTipo = g_strdup ("select");
					//extrair valor selecionado, se houver
					Campo->sValor = NULL;
					nPos1 = 0;
					BE_free (sTemp1);
					sTemp1 = be_strextract_ascii (sTemp, "<option", ">", nPos1);
					//loop de "option"
					while (sTemp1)
					{
						//verificar qual está selecionado
						if (be_strpos_ascii (sTemp1, "selected", 0) > 0)
						{
							Campo->sValor = be_strextract_ascii (sTemp1, "value='", "'", 0);
							//segunda tentativa
							if (!Campo->sValor)
								Campo->sValor = be_strextract_ascii (sTemp1, "value=", " ", 0);
						}
						//avançar posição
						nPos1 = be_strpos_ascii (sTemp, "<option", nPos1) + 1;
						//localizar próximo
						BE_free (sTemp1);
						sTemp1 = be_strextract_ascii (sTemp, "<option", ">", nPos1);
					}
					//anexar estrutura na lista
					Formulario->Campos = g_slist_append (Formulario->Campos, Campo);
					//avançar posição
					nPos = be_strpos_ascii (sTagFormulario, "<select", nPos) + 1;
					//extrair campos
					BE_free (sTemp);
					sTemp = be_strextract_ascii (sTagFormulario, "<select ", "/select", nPos);
				}
				//nova posição inicial
				nPosI = be_strpos_ascii (sConteudo, "<form", nPosF);
			}
		}
		//informar erro
		else
			Site->sErro = g_strdup_printf ("Erro: %d - %s\n", cCode, curl_easy_strerror (cCode));
	}
	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sConteudo);
	BE_free (sTagFormulario);
	BE_free (sPost);

	return Site->sErro ? FALSE : TRUE;
}

int _be_net_getURL_download (void *sStream, size_t nVezes, size_t nStream, void *Buffer)
{
/*******************************************************************************
Esta função é somente um callback da função curl_easy_setopt().
Ela é chamada na operação de download de qualquer informação.
Função somente para uso interno.
Argumentos:
    sStream : conteúdo da informação
    nVezes : quantidade de pedaços que a informação foi dividida
    nStream : comprimento do conteúdo
    Buffer : informação extra, repassada pela função que chamou esta
    Retorno: quantidade de bytes processados
*******************************************************************************/
	GString *BufferPtr = NULL;//ponteiro do mesmo tipo que o de Buffer

	BufferPtr = (GString *) Buffer;//apontando
	BufferPtr = g_string_append (BufferPtr, (gchar *) sStream);//concatenando o sStream ao final do Buffer

	return nStream;//bytes processados, todos
}

BENET_campo *be_net_campo (BENET *Site, const gchar *sNome, const gchar *sNovoValor)
{
/*******************************************************************************
Localiza um campo pelo seu nome, apartir de um site, em qualquer formulário.
sNome não considera caixa dos caractéres, não é CASE SENSITIVE
Argumentos:
	Site : estrutura carregada por be_net_receberCURL
	sNome : nome do campo a ser retornado
	sNovoValor : novo valor para ser substituído pelo antigo, ou NULL para manter o mesmo
	Retorno : o campo localizado, ou NULL.
*******************************************************************************/
	GSList *ListaL = NULL, *ListaL1 = NULL;//lista
	BENET_formulario *Formulario = NULL;//ponteiro de estrutura
	BENET_campo *Campo = NULL;//ponteiro de estrutura
	gchar *sTemp = NULL;

	//loop dos formulários
	for (ListaL = Site->Formularios; ListaL; ListaL = g_slist_next (ListaL))
	{
		Formulario = (BENET_formulario *) ListaL->data;
		//loop dos campos
		for (ListaL1 = Formulario->Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Campo = (BENET_campo *) ListaL1->data;
			//comparar nome do campo
			if (Campo->sNome && be_strpos_ascii (Campo->sNome, sNome, 0) == 0)
			{
				//verificar alteração de valor
				if (sNovoValor)
				{
					sTemp = Campo->sValor;
					Campo->sValor = g_strdup (sNovoValor);
					BE_free (sTemp);
				}
				return Campo;
			}
		}
	}

	//não localizou
	return NULL;
}

BENET_campo *be_net_botao (BENET *Site, const gchar *sNome, gboolean bSubmiter)
{
/*******************************************************************************
Localiza um campo pelo seu nome, apartir de um site, em qualquer formulário.
sNome não considera caixa dos caractéres, não é CASE SENSITIVE
Argumentos:
	Site : estrutura carregada por be_net_receberCURL
	sNome : nome do campo a ser retornado
	bSubmiter : se é para alterar o URL
	Retorno : o campo localizado, ou NULL.
*******************************************************************************/
	GSList *ListaL = NULL, *ListaL1 = NULL;//lista
	BENET_formulario *Formulario = NULL;//ponteiro de estrutura
	BENET_campo *Campo = NULL;//ponteiro de estrutura
	gchar *sTemp = NULL;
	gint nPosF = 0;

	//loop dos formulários
	for (ListaL = Site->Formularios; ListaL; ListaL = g_slist_next (ListaL))
	{
		Formulario = (BENET_formulario *) ListaL->data;
		//loop dos campos
		for (ListaL1 = Formulario->Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Campo = (BENET_campo *) ListaL1->data;
			//comparar nome do campo
			if (Campo->sNome && be_strpos_ascii (Campo->sNome, sNome, 0) == 0)
			{
				//definir nova URL
				if (bSubmiter)
				{
					BE_free (sTemp);
					sTemp = g_strdup (Site->sURL);
					nPosF = be_strrpos (sTemp, "/", 0);
					sTemp[nPosF] = '\0';
					BE_free (Site->sURL);
					if (Formulario->sAcao[0] == '/')
						Site->sURL = g_strdup_printf ("%s/%s", sTemp, Formulario->sAcao+1);//novo site
					else
						Site->sURL = g_strdup_printf ("%s/%s", sTemp, Formulario->sAcao);//novo site
					//informar para submeter o POST
					Site->bSubmiter = TRUE;
				}
				Campo->bAtivo = TRUE;

				return Campo;
			}
		}
	}

	//não localizou
	return NULL;
}

BENET_campo *be_net_campo_valor (BENET *Site, const gchar *sValor, const gchar *sNovoValor)
{
/*******************************************************************************
Localiza um campo pelo seu valor, apartir de um site, em qualquer formulário.
sValor não considera caixa dos caractéres, não é CASE SENSITIVE
Geralmente é utilizado para retorna campos 'submit' (botão)
Argumentos:
	Site : estrutura carregada por be_net_receberCURL
	sValor : valor atual do campo a ser retornado
	sNovoValor : novo valor para ser substituído pelo antigo, ou NULL para manter o mesmo
	Retorno : o campo localizado, ou NULL.
*******************************************************************************/
	GSList *ListaL = NULL, *ListaL1 = NULL;//lista
	BENET_formulario *Formulario = NULL;//ponteiro de estrutura
	BENET_campo *Campo = NULL;//ponteiro de estrutura
	gchar *sTemp = NULL;

	//loop dos formulários
	for (ListaL = Site->Formularios; ListaL; ListaL = g_slist_next (ListaL))
	{
		Formulario = (BENET_formulario *) ListaL->data;
		//loop dos campos
		for (ListaL1 = Formulario->Campos; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Campo = (BENET_campo *) ListaL1->data;
			//comparar nome do campo
			if (Campo->sValor && be_strpos_ascii (Campo->sValor, sValor, 0) == 0)
			{
				//verificar alteração de valor
				if (sNovoValor)
				{
					sTemp = Campo->sValor;
					Campo->sValor = g_strdup (sNovoValor);
					BE_free (sTemp);
				}
				return Campo;
			}
		}
	}
	//não localizou
	return NULL;
}

int _be_net_downloadCURL_download (void *sStream, size_t nVezes, size_t nStream, void *Arquivo)
{
/*******************************************************************************
Esta função é somente um callback da função curl_easy_setopt().
Ela é chamada na operação de download de qualquer informação.
Função somente para uso interno.
Argumentos:
    sStream : conteúdo da informação
    nVezes : quantidade de pedaços que a informação foi dividida
    nStream : comprimento do conteúdo
    Arquivo : informação extra, repassada pela função que chamou esta
    Retorno: quantidade de bytes processados
*******************************************************************************/
	FILE *pFile = NULL;//ponteiro do mesmo tipo que o de Arquivo

	pFile = (FILE *) Arquivo;//apontando

	//gravando conteúdo baixado no arquvio
	fwrite (sStream, nStream, 1, pFile);

	return nStream;//bytes processados, todos
}

gboolean be_net_downloadCURL (const gchar *sArquivo, const gchar *sURL, const gchar *sProxy, const gchar *sUsuarioSenha, const gchar *sNavegador, const gchar *sPost)
{
/*******************************************************************************
Faz o download de um site ou arquivo. Somente do caminho indicado por sURL.
Não segue links, imagens, CSS, JSP, ou qualquer arquivo que possa fazer parte do site.
Argumentos:
	sArquivo : arquivo local
	sURL : site/arquivo hospedado em qualquer local com acesso pelo navegador
	sProxy : caminho:porta do servidor proxy, ou NULL
	sUsuarioSenha : suuário:senha se houver, ou NULL
	sNavegador : nome User-Agent do navegador, ou NULL
	sPost : campos e valores para método POST
	Retorno: se foi ou não baixado com sucesso
*******************************************************************************/
	CURL *URL = NULL;
	CURLcode cCode;
	gboolean bOK = FALSE;
	FILE *pFile = NULL;
	gchar *sTemp = NULL;

	//iniciar biblioteca cURL
	if ((curl_global_init (CURL_GLOBAL_ALL) == CURLE_OK))
	{
		//iniciar biblioteca cURL
		if ((URL = curl_easy_init ()))
		{
			if ((pFile = fopen (sArquivo, "wb")))
			{
				//sempre aceitar SSL, mesmo sem validade
				curl_easy_setopt (URL, CURLOPT_SSL_VERIFYHOST, 0);
				curl_easy_setopt (URL, CURLOPT_SSL_VERIFYPEER, 0);
				//verificar proxy
				if (sProxy)
				{
					curl_easy_setopt (URL, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
					curl_easy_setopt (URL, CURLOPT_PROXY, sProxy);
					//verificar senha do proxy
					if (sUsuarioSenha)
						curl_easy_setopt (URL, CURLOPT_PROXYUSERPWD, sUsuarioSenha);
				}
				//verificar senha de autenticação
				else if (sUsuarioSenha)
					curl_easy_setopt (URL, CURLOPT_USERPWD, sUsuarioSenha);
				//verificar POST
				if (sPost)
					curl_easy_setopt (URL, CURLOPT_POSTFIELDS, sPost);
				//substituir &amp; por & nos links
				BE_free (sTemp);
				sTemp = be_strreplace (sURL, "&amp;", "&");
				//site
				curl_easy_setopt (URL, CURLOPT_URL, sTemp);
				//definir função  de retorno das partes
				curl_easy_setopt (URL, CURLOPT_WRITEFUNCTION, _be_net_downloadCURL_download);
				//definir informação extra, repassada com _be_net_downloadCURL_download()
				curl_easy_setopt (URL, CURLOPT_WRITEDATA, pFile);
				//progresso
				curl_easy_setopt (URL, CURLOPT_NOPROGRESS, 0);
				curl_easy_setopt (URL, CURLOPT_PROGRESSFUNCTION, _be_net_progresso);
				curl_easy_setopt (URL, CURLOPT_PROGRESSDATA, NULL);
				//ação
				cCode = curl_easy_perform (URL);
				//limpar
				curl_easy_cleanup (URL);
				//verificar se funcionou
				if(cCode == CURLE_OK)
				{
					//verificar se o arquivo foi realmente baixado
					if (g_file_test (sArquivo, G_FILE_TEST_EXISTS))
						bOK = TRUE;
				}
				//fechar arquivo
				fclose (pFile);
			}
		}
		//liberar biblioteca
		curl_global_cleanup ();
	}
	//limpar
	BE_free (sTemp);

	return bOK;//conteúdo
}
