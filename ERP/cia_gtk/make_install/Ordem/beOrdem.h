/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEOrdem                                                                      |
+------------------------------------------------------------------------------+
| BEOrdem é uma interface livre, você pode redistribuí-la e/ou                 |
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
	//Protótipos
	gboolean be_Ordem (GtkWindow *winPai, BESQL_Sintaxe *Sintaxe);
	//Protótipos reservados
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
