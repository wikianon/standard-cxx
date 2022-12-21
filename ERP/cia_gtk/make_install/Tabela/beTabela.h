/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BETabela                                                                     |
+------------------------------------------------------------------------------+
| BETabela é uma interface livre, você pode redistribuí-la e/ou                |
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
#ifndef _BETABELA_H
	#define _BETABELA_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../relatorio/berelatorio.h"
	#include "../Mensagens/beMensagens.h"
	#include "../Lista/beLista.h"
	#include "../Salvar/beSalvar.h"
	#include "../ListaG/beListaG.h"
	#include "../Propriedades/bePropriedades.h"
	#include "../Filtro/beFiltro.h"
	#include "../Ordem/beOrdem.h"

	//definições
	#define BETABELA_LINHAS_COMBO 25
	#define BETABELA_LENSTRING_COMBO 50
	#define BETABELA_MAIS_COMBO "(mais...)"
	#define BETABELA_ALTERAR_COMBO "(alterar...)"
	#define BETABELA_ALTERADO "?"
	//#define BETABELA_NULO "<vazio>"
	//#define BETABELA_NULO_TAG "<span foreground='#BBBBBB'>&lt;vazio&gt;</span>"
	#define BETABELA_NULO ""
	#define BETABELA_NULO_TAG ""
	#define BETABELA_ALTERADO_TAG "<span background='#FFFF00' weight='bold'>"
	#define BETABELA_LINHAS 30
	#define BETABELA_LENSTRING 254
	#define BETABELA_LENSTRING_BIN 16
	#define BETABELA_AVISO_LINHAS 100
	#define BETABELA_SUPER_AVISO_LINHAS 1000
	#define BETABELA_RELATORIO_ATUAL "relatorio_atual.pdf"
	#define BETABELA_PROPRIEDADES_POR_COLUNA 4
	#define BETABELA_PERFIL "Tabela"
	#define BETABELA_COLUNAS_LARGURA_MINIMA 30
	#define BETABELA_COLUNAS_LARGURA_MINIMA_COMBO 200
	#define BETABELA_COLUNAS_LARGURA_MAXIMA 600
	//janela
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *tabela;
		GtkWidget *treeview;
		GtkWidget *label;
		GtkCellRenderer *renderer;
		GtkCellEditable *editable;
		//botões
		GtkWidget *primeiro;
		GtkWidget *anterior;
		GtkWidget *posterior;
		GtkWidget *ultimo;
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
		//localizar
		GtkTreeViewColumn *colLocalizar;
		//consulta
		BESQL_Sintaxe *Sintaxe;
		BESQL_Sintaxe *SintaxeAlterar;
		BESQL_Sintaxe *SintaxeAdicionar;
		BESQL_Sintaxe *SintaxeRemover;
		BESQL_Sintaxe *SintaxeLog;
		BESQL_Sintaxe *SintaxePerfil;
		gboolean bCIA;//indica se a tabela faz parte de CIA
		gchar *sBloqueioID;
		gint nPagina;// = 1
		//relatórios
		const gchar *sArquivoSVG_ListaPadrao;
		const gchar *sArquivoSVG_FormularioPadrao;
		const gchar *sArquivoFonte;
		const gchar *sLeitorPDF;
		const BEGLOBAL_Empresa_Cliente *Empresa;
		//log de ações
		gboolean bLog;
		//formulario
		void (*func_Formulario)(BESQL_Tabela *Tabela, GSList *Filtros, gpointer user_data);
		//tabela externa
		void (*func_Tabela)(BESQL_Tabela *Tabela, gpointer user_data);
		gpointer user_data;
	} _BETabela_Janela;
	//Protótipos
	GtkWidget *be_Tabela (
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
		gboolean bAtualizar,
		void (*func_Formulario) //callback da função que chama um formulário, para a linha selecionada
			(BESQL_Tabela *Tabela,
			GSList *Filtros,
			gpointer user_data),
		//tabela externa
		void (*func_Tabela)
			(BESQL_Tabela *Tabela,
			gpointer user_data),
		gpointer user_data
	);
	gboolean be_Tabela_atualizar (GtkWidget *Tabela);//atualizar tabela, após criação e exibição da mesma
	//Protótipos reservados
	void _be_Tabela_on_destroy (GtkObject *object, gpointer user_data);//fechar
	GtkTreeModel *_be_Tabela_criar_modelo (_BETabela_Janela *Janela);
	GtkTreeModel *_be_Tabela_modelo_combo_atualizar (_BETabela_Janela *Janela, BESQL_Campo *Campo, GtkListStore *store, gchar *path);
	gchar *_be_Tabela_mais_combo (_BETabela_Janela *Janela, BESQL_Campo *Campo, gchar *path, gchar *sTexto);
	void _be_Tabela_alterar_combo (_BETabela_Janela *Janela, BESQL_Campo *Campo);
	void _be_Tabela_adicionar_colunas (_BETabela_Janela *Janela);
	void _be_Tabela_atualizar (_BETabela_Janela *Janela, gboolean bTudo);
	gchar *_be_Tabela_localizar_valores (_BETabela_Janela *Janela, BESQL_Campo *CampoL, gchar *path);
	void _be_Tabela_cell_edited (GtkCellRendererText *cellrenderertext, gchar *arg1, gchar *arg2, gpointer user_data);
	void _be_Tabela_cell_editing_started (GtkCellRenderer *renderer, GtkCellEditable *editable, gchar *path, gpointer user_data);
	void _be_Tabela_cell_editing_canceled (GtkCellRenderer *renderer, gpointer user_data);
	void _be_Tabela_localizar (_BETabela_Janela *Janela);
	void _be_Tabela_ordem (_BETabela_Janela *Janela);
	void _be_Tabela_filtrar (_BETabela_Janela *Janela);
	void _be_Tabela_exportar (_BETabela_Janela *Janela);
	void _be_Tabela_subtotais (_BETabela_Janela *Janela);
	void _be_Tabela_auditoria (_BETabela_Janela *Janela);
	void _be_Tabela_propriedades (_BETabela_Janela *Janela);
	void _be_Tabela_confirmacoes (_BETabela_Janela *Janela);
	void _be_Tabela_exibir_campos (_BETabela_Janela *Janela);
	void _be_Tabela_valor_padrao (_BETabela_Janela *Janela);
	void _be_Tabela_info_tabela (_BETabela_Janela *Janela);
	void _be_Tabela_salvar_perfil (_BETabela_Janela *Janela);
	void _be_Tabela_excluir_perfil (_BETabela_Janela *Janela);
	void _be_Tabela_carregar_perfil (_BETabela_Janela *Janela, const gchar *sVariavel);
	void _be_Tabela_abrir_formulario (_BETabela_Janela *Janela);
	void cursor_changed (GtkTreeView *treeview, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonPrimeiro_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonVoltar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonAvancar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonUltimo_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonAdicionar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonRemover_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonSalvar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonDados_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonRelatorios_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonOpcoes_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_toolbuttonPerfil_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowTabela_on_treeviewTabela_cursor_changed (GtkTreeView *treeview, gpointer user_data);
	gboolean _be_Interface_sinal_windowTabela_on_tableTabela_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
	gboolean _be_Interface_sinal_windowTabela_on_tableTabela_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
	gboolean _be_Interface_sinal_windowTabela_on_treeviewTabela_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
	gboolean _be_Interface_sinal_windowTabela_on_treeviewTabela_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
#endif
