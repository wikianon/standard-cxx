/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEGrupos                                                                     |
+------------------------------------------------------------------------------+
| BEGrupos � uma interface livre, voc� pode redistribu�-la e/ou                |
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
	//Prot�tipos
	gboolean be_Grupos (GtkWindow *winPai, GSList **Disponivel, GSList **Selecionado);//abre a janela de grupos
	//Prot�tipos reservados
	GtkTreeModel *_be_Grupos_create_model (GSList *Lista);//cria��o do modelo de listas
	void _be_Grupos_add_columns (GtkTreeView *treeview, gchar *sTitulo);//cria��o de modelo de colunas
	void _be_Interface_sinal_windowGrupos_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);//quando o usu�rio clicar no bot�o OK, esta fun��o � chamada
	void _be_Interface_sinal_windowGrupos_on_buttonAdicionar_clicked (GtkWidget *widget, gpointer user_data);//move o �tem selecionado na coluna dos Dispon�veis para a coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonRemover_clicked (GtkWidget *widget, gpointer user_data);//move o �tem selecionado na coluna dos Selecionados para a coluna dos Dispon�veis
	void _be_Interface_sinal_windowGrupos_on_buttonAcima_clicked (GtkWidget *widget, gpointer user_data);//eleva o �tem selecionado na coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonAbaixo_clicked (GtkWidget *widget, gpointer user_data);//rebaixa o �tem selecionado na coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonTodos_clicked (GtkWidget *widget, gpointer user_data);//move todos �tens da coluna dos Dispon�veis para a coluna dos Selecionados
	void _be_Interface_sinal_windowGrupos_on_buttonNenhum_clicked (GtkWidget *widget, gpointer user_data);//move todos �tens da coluna dos Selecionados para a coluna dos Dispon�veis
#endif
