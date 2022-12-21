/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEListaGigante                                                               |
+------------------------------------------------------------------------------+
| BEListaG � uma interface livre, voc� pode redistribu�-la e/ou                |
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
#ifndef _BELISTAG_H
	#define _BELISTAG_H
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"

	//defini��es
	#define BELISTAG_LENSTRING 49
	#define BELISTAG_LINHAS 50
	#define BELISTAG_NULO "<nulo>"
	//janela
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *treeview;
		GtkWidget *entry;
		GtkWidget *label;
		GtkWidget *primeiro;
		GtkWidget *anterior;
		GtkWidget *posterior;
		GtkWidget *ultimo;
		BESQL_Sintaxe *Sintaxe;
		GTimer *cronometro;
		gboolean bErros;
		gint nPagina;// = 1
	} _BEListaG_Janela;
	//Prot�tipos
	gboolean be_ListaG (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe, const gchar *sSelAtual);
	//Prot�tipos reservados
	GtkTreeModel *_be_ListaG_criar_modelo (_BEListaG_Janela *Janela);
	void _be_ListaG_adicionar_colunas (_BEListaG_Janela *Janela);
	void _be_ListaG_atualizar (_BEListaG_Janela *Janela);
	void _be_Interface_sinal_windowListaG_on_treeviewLista_row_activated (GtkTreeView *treeview, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data);
	void _be_Interface_sinal_windowListaG_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowListaG_on_toolbuttonPrimeiro_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowListaG_on_toolbuttonVoltar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowListaG_on_toolbuttonAvancar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowListaG_on_toolbuttonUltimo_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowListaG_on_entryLocalizar_changed (GtkEntry *entry, gpointer user_data);
	gboolean _be_ListaG_timeout (gpointer data);
	/*
	Obs1:
		A resposta � uma string referente a primeira coluna.
		O filtro � referente a segunda coluna.
		Ou seja, s�o necess�rias pelo menos 2 colunas.
	*/
#endif
