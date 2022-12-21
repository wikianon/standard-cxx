/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEFormulario                                                                 |
+------------------------------------------------------------------------------+
| BEFormulario é uma interface livre, você pode redistribuí-la e/ou            |
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
#ifndef _BEFORMULARIO_H
	#define _BEFORMULARIO_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../relatorio/berelatorio.h"
	#include "../Mensagens/beMensagens.h"
	#include "../Lista/beLista.h"
	#include "../Salvar/beSalvar.h"
	#include "../ListaG/beListaG.h"
	#include "../Propriedades/bePropriedades.h"
	#include "../Calendario/beCalendario.h"
	#include "../Abrir/beAbrir.h"

	//definições
	#define BEFORMULARIO_LINHAS_COMBO 25
	#define BEFORMULARIO_LENSTRING_COMBO 50
	#define BEFORMULARIO_MAIS_COMBO "(mais...)"
	#define BEFORMULARIO_ALTERAR_COMBO "(alterar...)"
	#define BEFORMULARIO_NENHUM_COMBO "(nenhum)"
	#define BEFORMULARIO_INSERIR_COMBO "(inserir...)"
	#define BEFORMULARIO_RECUPERAR_COMBO "(recuperar...)"
	#define BEFORMULARIO_LENSTRING_ROTULO 15
	#define BEFORMULARIO_LENSTRING_ENTRY 256
	#define BEFORMULARIO_LENSTRING_BIN 16
	//#define BEFORMULARIO_NULO "<vazio>"
	//#define BEFORMULARIO_NULO_TAG "<span foreground='#BBBBBB'>&lt;vazio&gt;</span>"
	#define BEFORMULARIO_NULO ""
	#define BEFORMULARIO_NULO_TAG ""
	#define BEFORMULARIO_CONTROLE_ALTURA_MIN 70
	#define BEFORMULARIO_CONTROLE_TEXTOLONGO_ALTURA_MIN 100
	#define BEFORMULARIO_CONTROLE_LARGURA_MIN 100
	#define BEFORMULARIO_RELATORIO_ATUAL "relatorio_atual.pdf"
	#define BEFORMULARIO_PERFIL "Formulario"
	#define BEFORMULARIO_PERFIL_LINHAS 30
	//janela
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *formulario;
		GtkWidget *tabela;
		GtkWidget *tabulador;
		//botões
		GtkWidget *adicionar;
		GtkWidget *salvar;
		GtkWidget *remover;
		GtkWidget *dados;
		GtkWidget *relatorios;
		GtkWidget *opcoes;
		GtkWidget *perfil;
		//confirmações
		gboolean bConfAdicionar;
		gboolean bConfSalvar;
		gboolean bConfRemover;
		//valor padrão automático
		gboolean bPadraoAutomatico;
		//consulta
		BESQL_Sintaxe *Sintaxe;
		BESQL_Sintaxe *SintaxeAlterar;
		BESQL_Sintaxe *SintaxeAdicionar;
		BESQL_Sintaxe *SintaxeRemover;
		BESQL_Sintaxe *SintaxeLog;
		BESQL_Sintaxe *SintaxePerfil;
		BESQL_Campo *CampoID;
		BESQL_Campo *CampoAutoID;
		gboolean bCIA;//indica se a tabela faz parte de CIA
		//relatórios
		const gchar *sArquivoSVG_ListaPadrao;
		const gchar *sArquivoSVG_FormularioPadrao;
		const gchar *sArquivoFonte;
		const gchar *sLeitorPDF;
		const BEGLOBAL_Empresa_Cliente *Empresa;
		//lista de controles
		GSList *Controles;
		//subtabela
		GtkWidget *(*func_Subtabela)
			(BESQL_Sintaxe *Sintaxe,
			BESQL_Sintaxe *SintaxeLog,
			BESQL_Sintaxe *SintaxePerfil,
			GtkWidget *window,
			BESQL_ACESSO_ENUM nFlags,
			gboolean bOpcoes,
			gboolean bLog,
			gboolean bAtualizar,
			gpointer user_data);
		gboolean (*func_Subtabela_atualizar)
			(GtkWidget *Subtabela);
		//tabela externa
		void (*func_Tabela)
			(BESQL_Tabela *Tabela,
			gpointer user_data);
		//botão executável
		void (*func_Executavel)
			(const gchar *sComando,
			const gchar *sParametros,
			gpointer user_data);
		gpointer user_data;
	} _BEFormulario_Janela;
	//controles
	typedef struct
	{
		_BEFormulario_Janela *Janela;//relação pai
		BESQL_Campo *Campo;
		GtkWidget *label;
		GtkWidget *controle;
		GtkWidget *subcontrole1;
		GtkWidget *subcontrole2;
		GtkWidget *botao1;
		GtkWidget *botao2;
		gchar *sString;
		gboolean bSomenteLeitura;
		BESQL_Sintaxe *Sintaxe;
		BESQL_Campo *CampoFiltro;
	} _BEFormulario_Controle;
	//Protótipos
	GtkWidget *be_Formulario (
		BESQL_Sintaxe *Sintaxe,
		BESQL_Sintaxe *SintaxeLog,
		BESQL_Sintaxe *SintaxePerfil,
		GtkWidget *window,
		BESQL_ACESSO_ENUM nFlags,
		gboolean bOpcoes,
		const gchar *sArquivoSVG_ListaPadrao,
		const gchar *sArquivoSVG_FormularioPadrao,
		const gchar *sArquivoFonte,
		const gchar *sLeitorPDF,
		const BEGLOBAL_Empresa_Cliente *Empresa,
		gboolean bLog,
		GtkWidget *(*func_Subtabela) //callback da função que cria uma tabela interna (subtabela). Retorna em forma de GtkWidget
			(BESQL_Sintaxe *Sintaxe,
			BESQL_Sintaxe *SintaxeLog,
			BESQL_Sintaxe *SintaxePerfil,
			GtkWidget *window,
			BESQL_ACESSO_ENUM nFlags,
			gboolean bOpcoes,
			gboolean bLog,
			gboolean bAtualizar,
			gpointer user_data),
		gboolean (*func_Subtabela_atualizar) //callback da função que atualiza a tabela já criada
			(GtkWidget *Subtabela),
		void (*func_Tabela) //callback da função que chama uma tabela
			(BESQL_Tabela *Tabela,
			gpointer user_data),
		void (*func_Executavel) //callback para ações de botão executável
			(const gchar *sComando,
			const gchar *sParametros,
			gpointer user_data),
		gpointer user_data
	);

	//Protótipos reservados
	gboolean _be_Formulario_executar_consulta (_BEFormulario_Janela *Janela);
	void _be_Formulario_criar (_BEFormulario_Janela *Janela);
	void _be_Formulario_destruir (_BEFormulario_Janela *Janela);
	void _be_Formulario_atualizar (_BEFormulario_Janela *Janela);
	void _be_Formulario_combo_box_atualizar (_BEFormulario_Controle *Controle);
	gchar *_be_Formulario_combo_box_localizar (_BEFormulario_Controle *Controle);
	void _be_Formulario_on_combo_changed (GtkComboBox *widget, gpointer user_data);
	void _be_Formulario_on_combo_set_focus_child (GtkContainer *container, GtkWidget *widget, gpointer user_data);
	gchar *_be_Formulario_mais_combo (_BEFormulario_Janela *Janela, BESQL_Campo *Campo);
	void _be_Formulario_alterar_combo (_BEFormulario_Janela *Janela, BESQL_Campo *Campo);
	void _be_Formulario_on_calendar_clicked (GtkButton *button, gpointer user_data);
	void _be_Formulario_visualizar_binario (_BEFormulario_Controle *Controle);
	void _be_Formulario_on_binary_changed (GtkComboBox *widget, gpointer user_data);
	void _be_Formulario_on_editable_changed (GtkComboBox *widget, gpointer user_data);
	void _be_Formulario_on_toggle_button_toggled (GtkToggleButton *togglebutton, gpointer user_data);
	void _be_Formulario_on_text_view_changed (GtkTextBuffer *textbuffer, gpointer user_data);
	void _be_Formulario_on_binary_down_clicked (_BEFormulario_Controle *Controle);
	void _be_Formulario_on_binary_up_clicked (_BEFormulario_Controle *Controle);
	void _be_Formulario_on_executar_clicked (GtkButton *button, gpointer user_data);
	GtkWidget *_be_Formulario_subtabela (_BEFormulario_Controle *Controle);
	void _be_Formulario_bloquear_subtabelas (_BEFormulario_Janela *Janela, gboolean bBloquear);
	void _be_Formulario_on_destroy (GtkObject *object, gpointer user_data);
	void _be_Formulario_novos_valores (_BEFormulario_Janela *Janela, gboolean bAdicionar);
	gchar *_be_Formulario_localizar_valores (_BEFormulario_Janela *Janela, BESQL_Campo *CampoL);
	gint _be_Formulario_ordenar_lista (gconstpointer a, gconstpointer b);
	void _be_Formulario_propriedades (_BEFormulario_Janela *Janela);
	void _be_Formulario_auditoria (_BEFormulario_Janela *Janela);
	void _be_Formulario_confirmacoes (_BEFormulario_Janela *Janela);
	void _be_Formulario_exibir_campos (_BEFormulario_Janela *Janela);
	void _be_Formulario_valor_padrao (_BEFormulario_Janela *Janela);
	void _be_Formulario_info_tabela (_BEFormulario_Janela *Janela);
	void _be_Formulario_salvar_perfil (_BEFormulario_Janela *Janela);
	void _be_Formulario_excluir_perfil (_BEFormulario_Janela *Janela);
	void _be_Formulario_carregar_perfil (_BEFormulario_Janela *Janela, const gchar *sVariavel);
	void _be_Formulario_exportar (_BEFormulario_Janela *Janela);
	gboolean _be_Formulario_on_button_press (GtkWidget *event_box, GdkEventButton *event, gpointer data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonAdicionar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonRemover_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonSalvar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonDados_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonRelatorios_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonOpcoes_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowFormulario_on_toolbuttonPerfil_clicked (GtkToolButton *toolbutton, gpointer user_data);
	gboolean _be_Interface_sinal_windowFormulario_on_tableFormulario_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
#endif
