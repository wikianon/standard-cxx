/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BENet                                                                        |
+------------------------------------------------------------------------------+
| BENet é uma biblioteca livre, você pode redistribuí-la e/ou                  |
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
#ifndef _BENET_H
	#define _BENET_H
	#include "../global/beglobal.h"
	#include <curl/curl.h>
	//Verificar versão do curl
	#ifdef __CURL_CURL_H
		#if LIBCURL_VERSION_NUM < 0x070d02
			#error "VERSAO DO LIBCURL DEVE SER >= 7.13.2"
		#endif
	#endif

	//definições
	#define BENET_LEN_MAX CURL_MAX_WRITE_SIZE
	#ifdef G_OS_UNIX
		#define BENET_ENTER "\n"
	#endif
	#ifdef G_OS_WIN32
		#define BENET_ENTER "\r\n"
	#endif
	//estrutura dos campos
	typedef struct
	{
		gchar *sNome;
		gchar *sTipo;
		gchar *sValor;
		gboolean bAtivo;
	} BENET_campo;
	//estrutura do formulário
	typedef struct
	{
		gchar *sNome;
		gchar *sMetodo;
		gchar *sAcao;
		GSList *Campos;
	} BENET_formulario;
	//estrutura site
	typedef struct
	{
		CURL *URL;
		gchar *sURL;
		gchar *sCabecalho;
		gchar *sConteudo;
		gchar *sErro;
		GSList *Formularios;
		gboolean bSubmiter;
		//GSList *Filtros;//backup
	} BENET;

	//Protótipos
	BENET *be_net_iniciar (const gchar *sURL, const gchar *sProxy, const gchar *sUsuarioSenha, const gchar *sNavegador);
	void _be_net_finalizar (BENET *Site);
	#define BE_net_finalizar(s) _be_net_finalizar(s);s=NULL
	gboolean be_net_receber (BENET *Site);
	BENET_campo *be_net_campo (BENET *Site, const gchar *sNome, const gchar *sNovoValor);//localiza e retorna o campo, baseado no nome
	BENET_campo *be_net_campo_valor (BENET *Site, const gchar *sValor, const gchar *sNovoValor);//localiza e retorna o campo, baseado seu valor
	BENET_campo *be_net_botao (BENET *Site, const gchar *sNome, gboolean bSubmiter);//localiza e retorna o campo, baseado no nome
	//Protótipos reservados
	void _be_net_limpar_formularios (GSList *Formularios);
	#define _BE_net_limpar_formularios(l) _be_net_limpar_formularios(l);l=NULL


	gboolean be_net_downloadCURL (const gchar *sArquivo, const gchar *sURL, const gchar *sProxy, const gchar *sUsuarioSenha, const gchar *sNavegador, const gchar *sPost);//faz o download de um arquivo para um arquivo local.
		//Campos suportados: INPUT, TEXTAREA, SELECT
	//Protótipos reservados
	int _be_net_getURL_download (void *sStream, size_t nVezes, size_t nStream, void *Buffer);//esta função é somente um callback da função curl_easy_setopt()
	int _be_net_downloadCURL_download (void *sStream, size_t nVezes, size_t nStream, void *Arquivo);//esta função é somente um callback da função curl_easy_setopt()
	int _be_net_progresso (void *Dados, double dltotal, double dlnow, double ultotal, double ulnow);
	void _be_net_limparCURL (BENET *Site);//limpa a estrutura

	//manual on-line:
	//  http://curl.haxx.se/libcurl/c/example.html
	//  http://curl.haxx.se/docs/manual.html

/*
lnet (curl):
===========
- Função para interpretar e recuperar numa estrutura, links (nome, valor), controles (nome, valor, método).
- A função de recuperar deverá enviar junto, se houver, controles (nome, valor, método).
- Função de download, com callback de progresso.
*/

/*
Exemplo de cabeçalho completo:

HTTP/1.1 200 Connection established
Via: 1.1 SRNEGRO186

Via: 1.1 SRNEGRO186


HTTP/1.1 200 OK
Server: Microsoft-IIS/5.0
Date: Wed, 31 Oct 2007 16:53:42 GMT
X-Powered-By: ASP.NET
X-AspNet-Version: 1.1.4322
Set-Cookie: ASP.NET_SessionId=caaxe245wgvh3w55qqxekt45; path=/
Set-Cookie: login=; expires=Tue, 12-Oct-1999 03:00:00 GMT; path=/
Cache-Control: private
Content-Type: text/html; charset=utf-8
Content-Length: 6126
*/
#endif
