/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEPrincipal                                                                  |
+------------------------------------------------------------------------------+
| BEPrincipal é uma interface livre, você pode redistribuí-la e/ou             |
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
#ifndef _BEPRINCIPAL_H
	#define _BEPRINCIPAL_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	// #include "../cript/becript.h"
	// #include "../net/benet.h"
	#include "../fontes/befontes.h"
	#include "../Propriedades/bePropriedades.h"
	#include "../Sobre/beSobre.h"
	#include "../Console/beConsole.h"
	#include "../Tabela/beTabela.h"
	#include "../Formulario/beFormulario.h"

	//definições
	#define BEPRINCIPAL_ARQUIVO_SPLASH "splash.png"
	#ifdef G_OS_UNIX
        #define BEPRINCIPAL_CONEXAO_SERVIDOR "conexao"
        #define BEPRINCIPAL_CONFIGURACAO_LOCAL "configuracaolocal"
	#endif
	#ifdef G_OS_WIN32
        #define BEPRINCIPAL_CONEXAO_SERVIDOR "conexao.ini"
        #define BEPRINCIPAL_CONFIGURACAO_LOCAL "configuracaolocal.ini"
    #endif
	#define BEPRINCIPAL_ARQUIVO_LISTA_PADRAO "lista_padrao.svg"
	#define BEPRINCIPAL_ARQUIVO_FORMULARIO_PADRAO "formulario_padrao.svg"
	#define BEPRINCIPAL_PERFIL_MENU "menu-Principal"
	#define BEPRINCIPAL_PERFIL_MENU_INICIAL "Acesso Inicial"
	#define BEPRINCIPAL_PERFIL_SEPARADA "janela_separada-Principal"
	#define BEPRINCIPAL_PERFIL_SEPARADA_JANELA "Janela Separada"
	#ifdef G_OS_UNIX
		#define BEPRINCIPAL_ARQUIVO_FONTES "fontes"
		#define BEPRINCIPAL_ARQUIVO_SAIDA "saida"
	#endif
	#ifdef G_OS_WIN32
		#define BEPRINCIPAL_ARQUIVO_FONTES "fontes.txt"
		#define BEPRINCIPAL_ARQUIVO_SAIDA "saida.txt"
	#endif
	#define BEPRINCIPAL_TIMEOUT 60000
		//em milisegundos

	//janela
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *status;
		GtkWidget *tabulador;
		//menus
			//escritórios
			GtkWidget *empresa;
				GtkWidget *senha;
				GtkWidget *usuario;
				GtkWidget *console;
				GtkWidget *auditoria;
				GtkWidget *iniciar;
				GtkWidget *adicionar;
				GtkWidget *excluir;
			GtkWidget *sair;
			//operações
			GtkWidget *operacoes;
			GtkWidget *janelas;
			GtkWidget *acesso;
			GtkWidget *ordenada;
			GtkWidget *aberta;
			//ajuda
			GtkWidget *config;
			GtkWidget *servidor;
			GtkWidget *ajuda;
			GtkWidget *sobre;
		//sql
		BESQL *SQL;
		BESQL_BD *BD;//principal
		BESQL_Sintaxe *SintaxeLog;
		BESQL_Sintaxe *SintaxePerfil;
		gint nExpirar;//seg
		gint nExpirarAtual;//seg
		//lista de menus
		GSList *Menus;
		GSList *MenusUsuario;
		//empresa principal/cliente
		BEGLOBAL_Empresa_Cliente *Empresa;
		//usuário
		gchar *sUsuario;
		gchar *sDescricaoPessoal;
		gchar *sEmailPessoal;
		gchar *sModelo;
		//configurações local
		GKeyFile *Ini;
		gchar *sLerPDF;
		gchar *sLerPNG;
		gchar *sNavegador;
		gchar *sEnviarEmail;
		gchar *sEditarPNG;
		gchar *sEditarTexto;
		gchar *sCalculadora;
		gboolean bJanelaSeparada;
		//acesso rápido
		gchar *sAcessoRapido;
		//status
		gchar *sStatus;
	} _BEPrincipal_Janela;
	//menus
	typedef struct __BEPrincipal_Menu
	{
		_BEPrincipal_Janela *Janela;
		gchar *sTransacao;
		gchar *sVisao;
		gchar *sID;//id atual para transação
		GtkWidget *menuitem;//menu
		struct __BEPrincipal_Menu *Mae;
		struct __BEPrincipal_Menu *Filha;
	} _BEPrincipal_Menu;

	//Protótipos
	void be_Principal (BESQL *SQL);
	//Protótipos reservados
	gboolean _be_Principal_callback_sql (BESQL *SQL, gint nErro, const gchar *sErro, gpointer user_data);
	void _be_Principal_conectar (_BEPrincipal_Janela *Janela);
	void _be_Principal_desconectar (_BEPrincipal_Janela *Janela);
	void _be_Principal_status (_BEPrincipal_Janela *Janela, gboolean bAtualizar);
	void _be_Principal_status_eventual (_BEPrincipal_Janela *Janela, const gchar *sInformacao);
	gboolean _be_Principal_timeout (gpointer data);
	void _be_Principal_menus (_BEPrincipal_Janela *Janela, BESQL_Sintaxe *Sintaxe);
	void _be_Principal_configuracoes_local (_BEPrincipal_Janela *Janela, gboolean bGravar);
	void _be_Principal_sair (_BEPrincipal_Janela *Janela, gboolean bPerguntar);
	gboolean _be_Principal_on_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);//fechar
	gboolean _be_Principal_executar_acessos (_BEPrincipal_Janela *Janela, const gchar *sAcesso);
	void _be_Principal_on_menubarUsuario_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Principal_menu_usuario (_BEPrincipal_Janela *Janela);
	void _be_Principal_abrir_janela (_BEPrincipal_Janela *Janela, BESQL_Sintaxe *Sintaxe, gint nTipo);
	void _be_Principal_abrir_janela_interna (_BEPrincipal_Janela *Janela, BESQL_Sintaxe *Sintaxe, gint nTipo);
	void _be_Principal_on_menubarOperacoes_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Principal_callback_botao_executavel
		(const gchar *sComando, const gchar *sParametros, gpointer user_data);
	void _be_Principal_callback_Tabela1
		(BESQL_Tabela *Tabela, gpointer user_data);
	void _be_Principal_callback_Formulario1
		(BESQL_Tabela *Tabela, GSList *Filtros, gpointer user_data);
	GtkWidget *_be_Principal_callback_SubTabela1
		(BESQL_Sintaxe *Sintaxe, BESQL_Sintaxe *SintaxeLog, BESQL_Sintaxe *SintaxePerfil, GtkWidget *window, BESQL_ACESSO_ENUM nFlags,
		gboolean bOpcoes, gboolean bLog, gboolean bAtualizar, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarEmpresa_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarConsole_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarAuditoria_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarIniciar_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarAdicionar_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarExcluir_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarSair_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarJanelas_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarAcesso_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarOrdenada_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarFecharJanela_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarConfig_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarServidor_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarAjuda_activate (GtkMenuItem *menuitem, gpointer user_data);
	void _be_Interface_sinal_windowPrincipal_on_menubarSobre_activate (GtkMenuItem *menuitem, gpointer user_data);
#endif
