/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BELista                                                                      |
+------------------------------------------------------------------------------+
| BELista � uma interface livre, voc� pode redistribu�-la e/ou                 |
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
#ifndef _BELISTA_H
	#define _BELISTA_H
	#include "../global/beglobal.h"

	//janela
	typedef struct
	{
		gint nPos;//posi��o atual
		GtkWidget *window;
		GtkWidget *treeview;
		GSList *Lista;
		gboolean bUTF8;
	} _BELista_Janela;
	//Prot�tipos
	gint be_Lista (GtkWindow *winPai, gboolean bUTF8, const gchar *sTitulo, GSList *Lista, gint nPosicaoAtual);
	//Prot�tipos reservados
	GtkTreeModel *_be_Lista_create_model (_BELista_Janela *Janela);
	void _be_Lista_add_columns (GtkTreeView *treeview);
	void _be_Interface_sinal_windowLista_on_treeviewLista_row_activated (GtkTreeView *treeview, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data);
	void _be_Interface_sinal_windowLista_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
#endif
