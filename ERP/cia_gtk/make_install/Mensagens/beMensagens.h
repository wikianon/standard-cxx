/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEMensagens                                                                  |
+------------------------------------------------------------------------------+
| BEMensagens � uma interface livre, voc� pode redistribu�-la e/ou             |
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
#ifndef _BEMENSAGENS_H
	#define _BEMENSAGENS_H
	#include "../global/beglobal.h"

	//O bUTF8 � para indicar se sTitulo, sMensagem e sValor, j� est�o em UTF8.

	/*
	Bot�es:
	GTK_BUTTONS_OK
	GTK_BUTTONS_CLOSE
	GTK_BUTTONS_CANCEL
	GTK_BUTTONS_YES_NO
	GTK_BUTTONS_OK_CANCEL

	Tipos:
    GTK_MESSAGE_INFO:
	GTK_MESSAGE_WARNING:
	GTK_MESSAGE_QUESTION:
	GTK_MESSAGE_ERROR:

	Respostas:
	GTK_RESPONSE_OK
	GTK_RESPONSE_CANCEL
	GTK_RESPONSE_CLOSE
	GTK_RESPONSE_YES
	GTK_RESPONSE_NO
	GTK_RESPONSE_APPLY
	GTK_RESPONSE_HELP
	*/

	//ao inv�s de criar vari�veis globais separadas, � criado uma estrutura din�mica com escopo local
	//janela
	typedef struct
	{
		GtkWidget *window;
        GtkWidget *label;
        GtkWidget *image;
        GtkWidget *botaoOK;
        GtkWidget *botaoCancelar;
        GtkWidget *botaoFechar;
        GtkWidget *botaoSim;
        GtkWidget *botaoNao;
		GtkResponseType nBotao;
	} _BEMensagem_Janela;//estrutura para as mensagens

	//Prot�tipos
	GtkResponseType be_Mensagens (GtkWindow *winPai, gboolean bUTF8, const gchar *sMensagem, GtkMessageType nTipo, GtkButtonsType nBotao);//exibe uma janela informativa
    GtkResponseType be_Mensagens_Ext (GtkWindow *winPai,  gboolean bUTF8, const gchar *sTitulo, const gchar *sMensagem, GtkMessageType nTipo, GtkButtonsType nBotao);//exibe uma janela informativa, com mais espa�o para o informa��o
	gchar *be_Mensagens_in (GtkWindow *winPai, gboolean bUTF8, const gchar *sTitulo, const gchar *sMensagem, const gchar *sValor);//exibe uma janela para entrada de dados
    //Prot�tipos reservados
    void _be_Interface_sinal_windowMensagem_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);//quando o usu�rio clicar no bot�o OK, esta fun��o � chamada
    void _be_Interface_sinal_windowMensagem_on_buttonCancelar_clicked (GtkWidget *widget, gpointer user_data);//quando o usu�rio clicar no bot�o Cancelar ou N�o, esta fun��o � chamada
    void _be_Interface_sinal_windowMensagem_on_buttonFechar_clicked (GtkWidget *widget, gpointer user_data);//quando o usu�rio clicar no bot�o fechar, esta fun��o � chamada
    void _be_Interface_sinal_windowMensagem_on_buttonSim_clicked (GtkWidget *widget, gpointer user_data);//quando o usu�rio clicar no bot�o Sim, esta fun��o � chamada
    void _be_Interface_sinal_windowMensagem_on_buttonNao_clicked (GtkWidget *widget, gpointer user_data);//quando o usu�rio clicar no bot�o N�o, esta fun��o � chamada
#endif
