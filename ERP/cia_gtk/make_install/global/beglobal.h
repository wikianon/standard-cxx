/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEGlobal                                                                     |
+------------------------------------------------------------------------------+
| BEGlobal é uma biblioteca livre, você pode redistribuí-la e/ou               |
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
//Biblioteca padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <locale.h>
#include <glib.h>
#include <python2.5/Python.h>
//vesões
#define COMPANY_NAME "Luciano Trindade Pereira"
#define FILE_VERSION "8.08.349"
#define FILE_DESCRIPTION "CIA GTK+"
#define LEGAL_COPYRIGHT "(C) 2007 Luciano Trindade Pereira"
#define LEGAL_TRADEMARKS "(C) Luciano Trindade Pereira"
#define ORIGINAL_FILENAME "cia_gtk"
#define INTERNAL_NAME "Controle Integrado Administrativo"
#define PRODUCT_NAME "CIA"
#define PRODUCT_VERSION "8.08.349"
#define VER_STRING "CIA 8.08.349"
#define VER_MAJOR 8
#define VER_MINOR 08
#define VER_RELEASE 349
#define VER_BUILD 2008
#define DIRETORIO ".cia"
//Windows
#ifdef G_OS_WIN32
	#include <windows.h>
#endif
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk-pixbuf/gdk-pixdata.h>
//Linux versões
#ifdef G_OS_UNIX
	//Verificar versão do gcc
	#ifndef GCC_VERSION
		#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#endif
	#if GCC_VERSION < 30305
		#error "VERSAO DO GCC DEVE SER >= 3.3.5"
	#endif
	//Verificar versão do libc
	#if !__GLIBC_PREREQ(2,3)
		#error "VERSAO DO LIBC DEVE SER >= 6.2.3"
	#endif
#endif
//Windows versões
#ifdef G_OS_WIN32
	#if __MINGW32_MAJOR_VERSION < 3 || (__MINGW32_MAJOR_VERSION >= 3 && __MINGW32_MINOR_VERSION < 9)
		#error "VERSAO DO MINGW32 DEVE SER >= 3.9"
	#endif
#endif
//Verificar versão do glib
#ifdef __G_LIB_H__
	#if !GLIB_CHECK_VERSION (2,6,4)
		#error "VERSAO DO GLIBC DEVE SER >= 2.6.4"
	#endif
#endif
//Verificar versão do gtk
#ifdef __GTK_H__
	#if !GTK_CHECK_VERSION (2,6,4)
		#error "VERSAO DO GTK DEVE SER >= 2.6.4"
	#endif
#endif
//global
#ifndef _BEGLOBAL_H
	#define _BEGLOBAL_H
	#define BEGLOBAL_CONFIG_LOCAL "configuracaolocal.ini"
	//tamanho máximo de manipulação de arquivo
	#define BEGLOBAL_TAMANHO_MAX_ARQUIVO (G_MAXULONG / 5)
	#define BEGLOBAL_BLOCOS_ARQUIVO 1024
	//definição de cores
	#define BEGLOBAL_COR_BLOQUEADA "#FFFFEE"
	#define BEGLOBAL_COR_BLOQUEADA_FONTE "#555555"
	//conversão ISO UTF8
	#define BE_ISO_UTF8(v) g_convert(v,-1,"UTF-8","ISO-8859-1",NULL,NULL,NULL)
	//conversão UTF8 ISO
	#define BE_UTF8_ISO(v) g_convert(v,-1,"ISO-8859-1","UTF-8",NULL,NULL,NULL)
	//Limpeza eficiente da memória
	#define BE_free(m); ({g_free(m);m=NULL;})
	#define BE_list_free(m); ({g_list_free(m);m=NULL;})
	#define BE_slist_free(m); ({g_slist_free(m);m=NULL;})
	#define BE_strfreev(s); ({g_strfreev(s);s=NULL;})
	#define BE_date_free(d); ({g_date_free(d);d=NULL;})
	#define BE_key_file_free(k); ({g_key_file_free(k);k=NULL;})
	#define BE_string_free(s) g_string_free(s,FALSE);s=NULL
	//imagens
	//#define BE_pixbuf_get_len(p) (gdk_pixbuf_get_width(p)*gdk_pixbuf_get_height(p)*gdk_pixbuf_get_n_channels(p))
	#define BE_pixbuf_get_len(p) (gdk_pixbuf_get_rowstride(p)*gdk_pixbuf_get_height(p))
	#define BE_pixbuf_free(p); ({if(p){g_object_unref(p);p=NULL;}})
	//recuperar Widgets
	#define BE_interfaces_widget(o,k) GTK_WIDGET(g_object_get_data(G_OBJECT(o),k))
	// monitorar memoria
	#ifndef _BEGLOBAL_C
		//conversão
		#define malloc(n) g_malloc(n)
		#define realloc(p,n) g_realloc(p,n)
		#define free(n); ({g_free(n);})
		//libc
		/*#define malloc(n) _be_global_memoria_reservar(malloc(n),n,__FILE__,__LINE__)
		#define free(p) _be_global_memoria_liberar(p,__FILE__,__LINE__);free(p)
		#define realloc(p,n) _be_global_memoria_liberar_reservar(p,realloc(p,n),n,__FILE__,__LINE__)
		#define calloc(n,m) _be_global_memoria_reservar(calloc(n,m),n*m,__FILE__,__LINE__)
		#define valloc(n) _be_global_memoria_reservar(valloc(n),n,__FILE__,__LINE__)
		#define memalign(n,m) _be_global_memoria_reservar(memalign(n,m),n*m,__FILE__,__LINE__)
		//glib
		#define g_malloc(n) _be_global_memoria_reservar(g_malloc(n),n,__FILE__,__LINE__)
		#define g_malloc0(n) _be_global_memoria_reservar(g_malloc0(n),n,__FILE__,__LINE__)
		#define g_realloc(p,n) _be_global_memoria_liberar_reservar(p,g_realloc(p,n),n,__FILE__,__LINE__)
		#define g_free(p) _be_global_memoria_liberar(p,__FILE__,__LINE__);g_free(p)
		#define g_try_malloc(n) _be_global_memoria_reservar(g_try_malloc(n),n,__FILE__,__LINE__)
		#define g_try_malloc0(n) _be_global_memoria_reservar(g_try_malloc(n),n,__FILE__,__LINE__)
		#define g_try_realloc(p,n) _be_global_memoria_liberar_reservar(g_try_realloc(p,n),n,__FILE__,__LINE__)
		#define g_memdup(m,n) _be_global_memoria_reservar(g_memdup(m,n),n,__FILE__,__LINE__)
		//glib strings
		#define g_strdup_printf(f,...) _be_global_memoria_reservar(g_strdup_printf(f,__VA_ARGS__),1,__FILE__,__LINE__)
		#define g_strdup(f) _be_global_memoria_reservar(g_strdup(f),1,__FILE__,__LINE__)
		#define g_string_free(s,v) _be_global_memoria_reservar(g_string_free(s,v),1,__FILE__,__LINE__)
		#define g_strsplit(s,d,g) _be_global_memoria_reservar(g_strsplit(s,d,g),1,__FILE__,__LINE__)
		#define g_strfreev(s) _be_global_memoria_liberar(s,__FILE__,__LINE__);g_strfreev(s)
		#define g_convert(s,l,t,f,r,w,e) _be_global_memoria_reservar(g_convert(s,l,t,f,r,w,e),1,__FILE__,__LINE__)
		#define g_utf8_strdown(s,l) _be_global_memoria_reservar(g_utf8_strdown(s,l),1,__FILE__,__LINE__)
		//glade
		#define glade_xml_new(f,r,d) _be_global_memoria_reservar(glade_xml_new(f,r,d),1,__FILE__,__LINE__)
		//pango
		#define pango_font_description_to_string(p) _be_global_memoria_reservar(pango_font_description_to_string(p),1,__FILE__,__LINE__)*/
		//xml
		//#define g_markup_printf_escaped(f,...) _be_global_memoria_reservar(g_markup_printf_escaped(f,__VA_ARGS__),1,__FILE__,__LINE__)
		#define g_markup_printf_escaped(f,...) _be_markup_printf_escaped(g_markup_printf_escaped(f,__VA_ARGS__))
	#endif
	//definição de empresa cliente
	typedef struct
	{
		gchar *sNomeFantasia;
		gchar *sRazaoSocial;
		gchar *sCNPJ;
		gchar *sIE;
		gchar *sTelefone;
		gchar *sFax;
		gchar *sPABX;
		gchar *sEmail;
		gchar *sSite;
		gchar *sCep;
		gchar *sTipoLogradouro;
		gchar *sLogradouro;
		gchar *sComplemento;
		gchar *sBairro;
		gchar *sCidade;
		gchar *sEstado;
		gchar *sPais;
		GdkPixbuf *Logotipo;
	} BEGLOBAL_Empresa_Cliente;
	//protótipos
	void be_global_abrir (gint *argc, gchar ***argv, glong lPasso);//passo para parar a depuração; 0 não parar
	void be_global_fechar ();
	//protótipos reservados
	void *_be_global_malloc (gsize nBytes);
	void *_be_global_realloc (gpointer pAntigo, gsize nBytes);
	void _be_global_free (gpointer pAlocada);
	void __be_global_hash_localizar (gpointer key, gpointer value, gpointer user_data);
	void __be_global_hash_remover (gpointer value);
	void _be_global_marcar (gpointer pAlocada, gsize nBytes);
	void _be_global_erros_gtk (const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data);//erros de GTK
	void _be_global_parar ();


//+----------------------------------------------------------------------------+
//| BEStrings                                                                  |
//+----------------------------------------------------------------------------+
	// Dicas:
	//- Converter números em sString: g_strdup_printf
	//- Converter sString em números: g_strtod
	//- Pegar saída de printf e colocar dentro de outra sString: g_strdup_printf

	//Tipos de conversões
	typedef enum
	{
		BEGLOBAL_CONVERSAO_ALFA = 0,//somente os caractéres alfanuméricos
		BEGLOBAL_CONVERSAO_LETRAS,//somente as letras do alfabeto atual
		BEGLOBAL_CONVERSAO_CONTROLE,//somente teclas de controle (Ctrl, Shift, Alt)
		BEGLOBAL_CONVERSAO_DIGITOS,//somente dígitos de 0 à 9
		BEGLOBAL_CONVERSAO_IMPRESSOS,//somente caractéres de impressão sem espaço
		BEGLOBAL_CONVERSAO_MINUSCULAS,//somente letras minúsculas
		BEGLOBAL_CONVERSAO_IMPRESSOS_ESPACO,//somente caractéres de impressão com espaço
		BEGLOBAL_CONVERSAO_PONTUACAO,//somente caractéres de pontuação
		BEGLOBAL_CONVERSAO_ESPECIAIS,//somente espaço, tabulações, retorno de carro ou nova linha
		BEGLOBAL_CONVERSAO_MAIUSCULAS,//somente letras maiúsculas
		BEGLOBAL_CONVERSAO_HEXADECIMAIS,//somente dígitos hexadecimais
		BEGLOBAL_CONVERSAO_SEM_ACENTOS,//conversão de letras acentuadas para não acentuadas
		BEGLOBAL_CONVERSAO_FONETICAS//conversão fonética. Ex: cx=s, ci=si, rr=r, ss=s, ka=ca.
	} BEGLOBAL_CONVERSOES;
	//Tipos de aplicativos
	typedef enum
	{
		BEGLOBAL_APLICATIVO_PDF = 0,//leitor de PDF
		BEGLOBAL_APLICATIVO_NAVEGADOR,//navegador de internet
		BEGLOBAL_APLICATIVO_EMAIL,//enviar e-mail
		BEGLOBAL_APLICATIVO_PNG,//leitor de PNG
		BEGLOBAL_APLICATIVO_CALCULADORA,//calculadora
	} BEGLOBAL_APLICATIVOS;
	//Protótipos
	gchar *be_strreplace (const gchar *sString, const gchar *sLocalizar, const gchar *sSubstituir); //retorna sString com todos os sLocalizar trocados por sSubstituir.
	gchar *be_strreplace_pos (const gchar *sString, const gchar *sLocalizar, const gchar *sSubstituir, gint nInicio); //retorna sString com todos os sLocalizar trocados por sSubstituir.
	gchar *be_substr (const gchar *sString, gint nInicio, gint nComprimento);//retorna a parte de sString especificada pelo parâmetro nInicio e nComprimento.
	gint be_strpos (const gchar *sString, const gchar *sLocalizar, const gint nInicio);//retorna a posição numérica da primeira ocorrência de sLocalizar dentro de sString
	gint be_strrpos (const gchar *sString, const gchar *sLocalizar, const gint nInicio);//retorna a posição numérica da última ocorrência de sLocalizar dentro de sString
	gint be_strpos_ascii (const gchar *sString, const gchar *sLocalizar, const gint nInicio);//retorna a posição numérica da primeira ocorrência de sLocalizar dentro de sString, ignorando CASE das letras
	gchar *be_strrepeat (const gchar *sString, const gint nQt);//retorna sString repetida count vezes
	gint be_strcount (const gchar *sString, const gchar *sLocalizar);//retorna a quantidade de ocorrências de sLocalizar dentro de sString.
	gchar *be_strextract (const gchar *sString, const gchar *sLocalizar1, const gchar *sLocalizar2, gint nInicio);//Retorna um ponteiro para uma nova sString dentro do intervalo localizado em sLocalizar1 e sLocalizar2
	gchar *be_strextract_ascii (const gchar *sString, const gchar *sLocalizar1, const gchar *sLocalizar2, gint nInicio);//Retorna um ponteiro para uma nova sString dentro do intervalo localizado em sLocalizar1 e sLocalizar2, ignorando CASE das letras
	GSList *be_strsplit (gchar **sString, const gchar *sIndicador);//quebra uma sString em várias partes
	gchar *be_strclear (const gchar *sString, BEGLOBAL_CONVERSOES nOpc);//devolve uma string, dependendo das seguintes opção de conversão de BEGLOBAL_CONVERSOES
	glong be_cast_hex_long (const gchar *sString);//converte um número hexadecimal que está em forma de sString em um long
	gint be_rgb_vermelho (glong lCorRGB);//extrai a cor vermelho de um número RGB
	gint be_rgb_verde (glong lCorRGB);//extrai a cor vermelho de um número RGB
	gint be_rgb_azul (glong lCorRGB);//extrai a cor azul de um número RGB
	gint be_string_ordenar_string (gconstpointer a, gconstpointer b);//Retorna qual das duas strings vem primeiro numa ordem alfabética, pela tabela ascii.
	gdouble be_string_calculadora (gchar *sExpressao);//calcula uma expressão matemática e retorna seu valor
	gchar *be_markup_text (const gchar *sTexto);//desconverte g_markup_printf_escaped
	void be_redimensionar_proporcional (gfloat *fLarguraOriginal, gfloat *fAlturaOriginal, gfloat Largura, gfloat Altura, gboolean bAumentar);//redimensiona um retângulo, mantendo suas porporções
	gchar *_be_markup_printf_escaped (gchar *sTexto);//converte '%' em marcação HTML
	gchar *be_urlencode (const gchar *sTexto);//converte o texto para urlencode, de acordo com RFC2396; usar curl_easy_escape se possivel
	gchar *be_urldecode (const gchar *sTexto);//converte o texto de urlencode, de acordo com RFC2396; usar curl_easy_unescape se possivel
	gchar *be_htmldecode (const gchar *sTexto);
//+----------------------------------------------------------------------------+
//| BETempo                                                                    |
//+----------------------------------------------------------------------------+
	#define _BE_time_SIZE 256
	//Protótipos
	#define BE_tempo(s) _be_tempo(s,FALSE)
	#define BE_tempo_gmt(s) _be_tempo(s,TRUE)
	gchar *be_tempo_tm (struct tm *Time, const gchar *sFormato);//retorna a data e hora de Time, no formato
	gint be_tempo_fuso ();//retorna o fuso horário atual
	GDate *be_tempo_GDate ();//cria um GDate com o tempo atual
	gint be_tempo_sec ();//retorna os segundos
	gint be_tempo_usec ();//retorna os microsegundos
	//Protótipos reservado
	gchar *_be_tempo (const gchar *sFormato, const gboolean bGMT);
	struct tm *_be_tempo_tm (const gboolean bGMT);//retorna a data e hora do sistema, de acordo com o GTM

//+----------------------------------------------------------------------------+
//| BEArquivo                                                                  |
//+----------------------------------------------------------------------------+
	#define BEARQUIVO_COMPRIMENTO_MAX 63
	gchar *be_arquivo_nome_sugerido (const gchar *sNome, gint nComprimentoMax, gboolean bTempo);//retorna uma complementação para o sNome e algumas conversões.
	gchar *be_arquivo_extensao (const gchar *sArquivo);//retorna a extensão de um arquivo
	gint be_arquivo_tamanho (const gchar *sArquivo);//retorna o tamanho do arquivo, em bytes
	gboolean be_arquivo_localidade_UTF8 ();//retorna verdadeiro se os arquivos devem ser manipulados em UTF8
	gchar *be_arquivo_aplicativo (BEGLOBAL_APLICATIVOS nTipo);//retorna o comando para executar o aplicativo
	gchar *be_diretorio_local (gboolean bSeparador);//retorna o caminho completo do diretório de configurações locais do sistema atual
	gchar *be_diretorio_local_temp (gboolean bSeparador);//retorna o caminho completo do diretório temporário, do usuário atual
	gchar *be_arquivo_absoluto (const gchar *sArquivo);//garante um caminho completo até o arquivo sArquivo

//+----------------------------------------------------------------------------+
//| BEImagens                                                                  |
//+----------------------------------------------------------------------------+
	guchar *be_pixbuf_get_pixels (const GdkPixbuf *pixbuf);//correção de linha de bits e remoção de transparência, ideal para PDF

//+----------------------------------------------------------------------------+
//| BECliente                                                                  |
//+----------------------------------------------------------------------------+
	#define BE_empresa_cliente_limpar(e) _be_empresa_cliente_limpar(e);e=NULL
	void _be_empresa_cliente_limpar (BEGLOBAL_Empresa_Cliente *Empresa);//limpar a estrutura Empresa

//+----------------------------------------------------------------------------+
//| BEJanelas                                                                  |
//+----------------------------------------------------------------------------+
	//Estruturas
	typedef struct
	{
		GtkWidget *window;//janela
		GtkWidget *foco;//controle com o foco atual
	} BEGlobal_Janela;
	//Protótipos
	BEGlobal_Janela *be_janela_mouse_pensando (GtkWidget *window);//ativa o mouse pensando e bloqueia todos os controles da janela, definida em window
	#define BE_janela_mouse_normal(j,b) _be_janela_mouse_normal(j,b);j=NULL
	//Protótipos Reservados
	void _be_janela_mouse_normal (BEGlobal_Janela *Janela, gboolean bFoco);//devolver o mouse padrão, liberar a janela bloqueada e manter o foco no último controle ativo
	//ativar informações de depuração
	#define BEDEPURACAO 0
#endif
