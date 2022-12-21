/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEMensagens                                                                  |
+------------------------------------------------------------------------------+
| BEMensagens é uma interface livre, você pode redistribuí-la e/ou             |
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
#ifndef _BEMENSAGENS_H
	#define _BEMENSAGENS_H
	#include "../global/beglobal.h"

	//O bUTF8 é para indicar se sTitulo, sMensagem e sValor, já estão em UTF8.

	/*
	Botões:
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

	//ao invés de criar variáveis globais separadas, é criado uma estrutura dinâmica com escopo local
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

	//Protótipos
	GtkResponseType be_Mensagens (GtkWindow *winPai, gboolean bUTF8, const gchar *sMensagem, GtkMessageType nTipo, GtkButtonsType nBotao);//exibe uma janela informativa
    GtkResponseType be_Mensagens_Ext (GtkWindow *winPai,  gboolean bUTF8, const gchar *sTitulo, const gchar *sMensagem, GtkMessageType nTipo, GtkButtonsType nBotao);//exibe uma janela informativa, com mais espaço para o informação
	gchar *be_Mensagens_in (GtkWindow *winPai, gboolean bUTF8, const gchar *sTitulo, const gchar *sMensagem, const gchar *sValor);//exibe uma janela para entrada de dados
    //Protótipos reservados
    void _be_Interface_sinal_windowMensagem_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);//quando o usuário clicar no botão OK, esta função é chamada
    void _be_Interface_sinal_windowMensagem_on_buttonCancelar_clicked (GtkWidget *widget, gpointer user_data);//quando o usuário clicar no botão Cancelar ou Não, esta função é chamada
    void _be_Interface_sinal_windowMensagem_on_buttonFechar_clicked (GtkWidget *widget, gpointer user_data);//quando o usuário clicar no botão fechar, esta função é chamada
    void _be_Interface_sinal_windowMensagem_on_buttonSim_clicked (GtkWidget *widget, gpointer user_data);//quando o usuário clicar no botão Sim, esta função é chamada
    void _be_Interface_sinal_windowMensagem_on_buttonNao_clicked (GtkWidget *widget, gpointer user_data);//quando o usuário clicar no botão Não, esta função é chamada
#endif
