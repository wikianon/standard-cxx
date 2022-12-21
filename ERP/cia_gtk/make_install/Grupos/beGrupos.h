/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEGrupos                                                                     |
+------------------------------------------------------------------------------+
| BEGrupos é uma interface livre, você pode redistribuí-la e/ou                |
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
#ifndef _BEGRUPOS_H
	#define _BEGRUPOS_H
	#include "../global/beglobal.h"

	//janela
	typedef struct
	{
		gboolean bOK;
		GtkWidget *window;
		GtkWidget *treeviewDisp;
		GtkWidget *treeviewSel;
		GSList **Disponivel;
		GSList **Selecionado;
	} _BEGrupos_Janela;
	//Protótipos
	gboolean be_Grupos (GtkWindow *winPai, GSList **Disponivel, GSList **Selecionado);//abre a janela de grupos
	//Protótipos reservados
	GtkTreeModel *_be_Grupos_create_model (GSList *Lista);//criação do modelo de listas
	void _be_Grupos_add_columns (GtkTreeView *treeview, gchar *sTitulo);//criação de modelo de colunas
	void _be_Interface_sinal_windowGrupos_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);//quando o usuário clicar no botão OK, esta função é chamada
	void _be_Interface_sinal_windowGrupos_on_buttonAdicionar_clicked (GtkWidget *widget, gpointer user_data);//move o ítem selecionado na coluna dos Disponíveis para a coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonRemover_clicked (GtkWidget *widget, gpointer user_data);//move o ítem selecionado na coluna dos Selecionados para a coluna dos Disponíveis
	void _be_Interface_sinal_windowGrupos_on_buttonAcima_clicked (GtkWidget *widget, gpointer user_data);//eleva o ítem selecionado na coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonAbaixo_clicked (GtkWidget *widget, gpointer user_data);//rebaixa o ítem selecionado na coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonTodos_clicked (GtkWidget *widget, gpointer user_data);//move todos ítens da coluna dos Disponíveis para a coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonNenhum_clicked (GtkWidget *widget, gpointer user_data);//move todos ítens da coluna dos Selecionados para a coluna dos Disponíveis
#endif
