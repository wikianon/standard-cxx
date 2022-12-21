/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BELista                                                                      |
+------------------------------------------------------------------------------+
| BELista é uma interface livre, você pode redistribuí-la e/ou                 |
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
#ifndef _BELISTA_H
	#define _BELISTA_H
	#include "../global/beglobal.h"

	//janela
	typedef struct
	{
		gint nPos;//posição atual
		GtkWidget *window;
		GtkWidget *treeview;
		GSList *Lista;
		gboolean bUTF8;
	} _BELista_Janela;
	//Protótipos
	gint be_Lista (GtkWindow *winPai, gboolean bUTF8, const gchar *sTitulo, GSList *Lista, gint nPosicaoAtual);
	//Protótipos reservados
	GtkTreeModel *_be_Lista_create_model (_BELista_Janela *Janela);
	void _be_Lista_add_columns (GtkTreeView *treeview);
	void _be_Interface_sinal_windowLista_on_treeviewLista_row_activated (GtkTreeView *treeview, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data);
	void _be_Interface_sinal_windowLista_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
#endif
