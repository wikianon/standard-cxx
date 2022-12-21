/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEFiltro                                                                     |
+------------------------------------------------------------------------------+
| BEFiltro � uma interface livre, voc� pode redistribu�-la e/ou                |
| modific�-la dentro dos termos da Licen�a P�blica Geral GNU como              |
| publicada pela Funda��o do Software Livre (FSF); na vers�o 2 da              |
| Licen�a, ou (na sua opni�o) qualquer vers�o.                                 |
| Este programa � distribuido na esperan�a que possa ser �til,                 |
| mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUA��O a qualquer |
| MERCADO ou APLICA��O EM PARTICULAR.                                          |
| Veja a Licen�a P�blica Geral GNU para maiores detalhes.                      |
| Voc� deve ter recebido uma c�pia da Licen�a P�blica Geral GNU                |
| junto com este programa, se n�o, escreva para a Funda��o do Software         |
| Livre(FSF) Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     |
+------------------------------------------------------------------------------+
*/
#ifndef _BEFILTRO_H
	#define _BEFILTRO_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../Lista/beLista.h"
	#include "../Mensagens/beMensagens.h"

	//defini��es
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
	//Prot�tipos reservados
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
