/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEListaGigante                                                               |
+------------------------------------------------------------------------------+
| BEListaG é uma interface livre, você pode redistribuí-la e/ou                |
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
#ifndef _BELISTAG_H
	#define _BELISTAG_H
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"

	//definições
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
	//Protótipos
	gboolean be_ListaG (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe, const gchar *sSelAtual);
	//Protótipos reservados
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
		A resposta é uma string referente a primeira coluna.
		O filtro é referente a segunda coluna.
		Ou seja, são necessárias pelo menos 2 colunas.
	*/
#endif
