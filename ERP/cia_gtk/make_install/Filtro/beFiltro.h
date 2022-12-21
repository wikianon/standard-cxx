/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEFiltro                                                                     |
+------------------------------------------------------------------------------+
| BEFiltro é uma interface livre, você pode redistribuí-la e/ou                |
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
#ifndef _BEFILTRO_H
	#define _BEFILTRO_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../Lista/beLista.h"
	#include "../Mensagens/beMensagens.h"

	//definições
	#define BEFILTRO_PROPRIEDADES_POR_COLUNA 2
	#define BEFILTRO_LINHAS 10
	#define BEFILTRO_LENSTRING 30
	#define BEFILTRO_COR_FUNDO "#FFFFFF"
	#define BEFILTRO_COR_BORDA "#CCCCCC"
	#define BEFILTRO_CURINGA_TODOS "*"
	#define BEFILTRO_CURINGA_PELOMENOSUM "?"
	#define BEFILTRO_PERFIL "Filtro"
	#define BEFILTRO_PERFIL_LINHAS 100
	//janela
	typedef struct
	{
		gboolean bOK;
		GtkWidget *window;
		GtkWidget *treeview;
		BESQL_Sintaxe *Sintaxe;
		BESQL_Sintaxe *SintaxePerfil;
		GSList *Filtros;//backup
	} _BEFiltro_Janela;
	gboolean be_Filtro (GtkWindow *window, BESQL_Sintaxe *Sintaxe, BESQL_Sintaxe *SintaxePerfil);
	//Protótipos reservados
	GtkTreeModel *_be_Filtro_criar_modelo (_BEFiltro_Janela *Janela);
	void _be_Filtro_adicionar_colunas (_BEFiltro_Janela *Janela);
	void _be_Filtro_atualizar (_BEFiltro_Janela *Janela);
	void _be_Interface_sinal_windowFiltro_on_buttonPerfil_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowFiltro_on_buttonLimpar_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowFiltro_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Filtro_cell_editing_canceled (GtkCellRenderer *renderer, gpointer user_data);
	void _be_Filtro_cell_edited (GtkCellRendererText *cellrenderertext, gchar *arg1, gchar *arg2, gpointer user_data);
	void _be_Filtro_salvar_perfil (_BEFiltro_Janela *Janela);
	void _be_Filtro_excluir_perfil (_BEFiltro_Janela *Janela);
	void _be_Filtro_carregar_perfil (_BEFiltro_Janela *Janela, const gchar *sVariavel);
#endif
