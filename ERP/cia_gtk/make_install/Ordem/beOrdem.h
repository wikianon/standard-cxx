/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEOrdem                                                                      |
+------------------------------------------------------------------------------+
| BEOrdem � uma interface livre, voc� pode redistribu�-la e/ou                 |
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
#ifndef _BEORDEM_H
	#define _BEORDEM_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"

	//janela
	typedef struct
	{
		gboolean bOK;
		GtkWidget *window;
		GtkWidget *treeviewDisp;
		GtkWidget *treeviewSel;
		BESQL_Sintaxe *Sintaxe;
	} _BEOrdem_Janela;
	//Prot�tipos
	gboolean be_Ordem (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe);
	//Prot�tipos reservados
	GtkTreeModel *_be_Ordem_criar_modelo_campo (BESQL_Sintaxe *Sintaxe);
	GtkTreeModel *_be_Ordem_criar_modelo_ordem (BESQL_Sintaxe *Sintaxe);
	void _be_Ordem_adicionar_colunas (GtkTreeView *treeview, gchar *sTitulo);
	void _be_Interface_sinal_windowOrdem_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowOrdem_on_buttonAdicionar_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowOrdem_on_buttonRemover_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowOrdem_on_buttonAcima_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowOrdem_on_buttonAbaixo_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowOrdem_on_buttonCrescente_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowOrdem_on_buttonDecrescente_clicked (GtkWidget *widget, gpointer user_data);
#endif
