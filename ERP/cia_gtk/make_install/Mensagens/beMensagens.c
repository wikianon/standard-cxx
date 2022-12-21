/*******************************************************************************
Janelas de mensagens informativas e interrogativas, para interação com o usuário.
*******************************************************************************/

#include "beMensagens.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_dialogEntrada.c"
#include "../Interfaces/interface_windowMensagem.c"

GtkResponseType be_Mensagens (GtkWindow *winPai,  gboolean bUTF8, const gchar *sMensagem, GtkMessageType nTipo, GtkButtonsType nBotao)
{
/*******************************************************************************
Exibe uma janela informativa.
Utiliza as funções de GtkMessageDialog.
Argumentos:
    winPai : janela parente, superior
    bUTF8 : indica se sTitulo, sMensagem e sValor, já estão em UTF8
    sMensagem : mensagem a ser exibida, aceita Pango Text Attribute Markup (quase HTML)
    nTipo : tipo de mensagem
    nBotao : tipo de botões, quais estarão visíveis
        GTK_MESSAGE_INFO, GTK_MESSAGE_WARNING, GTK_MESSAGE_QUESTION, GTK_MESSAGE_ERROR
    Retorno: um tipo de reposta
        GTK_RESPONSE_OK, GTK_RESPONSE_CANCEL, GTK_RESPONSE_CLOSE, GTK_RESPONSE_YES, GTK_RESPONSE_NO, GTK_RESPONSE_APPLY, GTK_RESPONSE_HELP
*******************************************************************************/
	GtkWidget *message = NULL;
	GtkResponseType nResp = GTK_RESPONSE_NONE;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//liberar eventos, não ficar esperando (DoEvents)
    while (gtk_events_pending ())
        gtk_main_iteration ();

	//seleção de tipos
	switch (nTipo)
	{
		case GTK_MESSAGE_INFO:
			sTemp = g_strdup_printf ("<span foreground='#000000'>%s</span>", sMensagem);//cor da mensagem é preto
			break;
		case GTK_MESSAGE_WARNING:
			sTemp = g_strdup_printf ("<span foreground='#888800'>%s</span>", sMensagem);//cor da mensagem é laranja
			break;
		case GTK_MESSAGE_QUESTION:
			sTemp = g_strdup_printf ("<span foreground='#008800'>%s</span>", sMensagem);//cor da mensagem é verde
			break;
		case GTK_MESSAGE_ERROR:
			sTemp = g_strdup_printf ("<span foreground='#FF0000'>%s</span>", sMensagem);//cor da mensagem é vermelho
			break;
		default:
			sTemp = g_strdup (sMensagem);//cor padrão
			break;
	}
    //verificar se os textos já estão em UTF8
	if (!bUTF8)
	{
		sTemp1 = sTemp;
		sTemp = BE_ISO_UTF8 (sTemp1);//conversão para UTF8
	}
	//criar janela
	message = gtk_message_dialog_new_with_markup (winPai, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, nTipo, nBotao, sTemp);//criar uma nova janela
	//configurar janela
	if (winPai)
		gtk_window_set_transient_for (GTK_WINDOW (message), winPai);
	gtk_window_set_position (GTK_WINDOW (message), GTK_WIN_POS_CENTER_ALWAYS);//window_position
	gtk_window_set_modal (GTK_WINDOW (message), TRUE);//modal
	//o botão padrão é sempre a não confirmação, evita problemas se o usuário apertar <Enter> antes de ler a mensagem
	if (nBotao == GTK_BUTTONS_YES_NO)
		gtk_dialog_set_default_response (GTK_DIALOG (message), GTK_RESPONSE_NO);//padrão é Não, se existir
	else if (nBotao == GTK_BUTTONS_OK_CANCEL)
		gtk_dialog_set_default_response (GTK_DIALOG (message), GTK_RESPONSE_CANCEL);//padrão é Cancelar, se existir

	//loop de interação com o usuário; só sai daqui quando a janela Message for fechada
	nResp = gtk_dialog_run (GTK_DIALOG (message));//recupera o botão que o usuário apertou

    //limpar
	gtk_widget_destroy (message);
	BE_free (sTemp);
	BE_free (sTemp1);

	return nResp;
}

GtkResponseType be_Mensagens_Ext (GtkWindow *winPai,  gboolean bUTF8, const gchar *sTitulo, const gchar *sMensagem, GtkMessageType nTipo, GtkButtonsType nBotao)
{
/*******************************************************************************
Exibe uma janela informativa, com mais espaço para o informação.
Tem opção de definir título da janela.
Argumentos:
    winPai : janela parente, superior
    bUTF8 : indica se sTitulo, sMensagem e sValor, já estão em UTF8
    sTitulo : título da mensagem
    sMensagem : mensagem a ser exibida, aceita Pango Text Attribute Markup (quase HTML)
    nTipo : tipo de mensagem
    nBotao : tipo de botões, quais estarão visíveis
        GTK_MESSAGE_INFO, GTK_MESSAGE_WARNING, GTK_MESSAGE_QUESTION, GTK_MESSAGE_ERROR
    Retorno: um tipo de reposta
        GTK_RESPONSE_OK, GTK_RESPONSE_CANCEL, GTK_RESPONSE_CLOSE, GTK_RESPONSE_YES, GTK_RESPONSE_NO, GTK_RESPONSE_APPLY, GTK_RESPONSE_HELP
*******************************************************************************/
	_BEMensagem_Janela Janela;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	//liberar eventos, não ficar esperando (DoEvents)
    while (gtk_events_pending ())
        gtk_main_iteration ();

	//criar janela
	Janela.window = be_Interface_criar_windowMensagem (winPai, NULL, NULL, &Janela);
	//título da janela
	if (sTitulo)
	{
		if (bUTF8)
			gtk_window_set_title (GTK_WINDOW (Janela.window), sTitulo);//preparar título
		else
		{
			BE_free (sTemp);
			sTemp = BE_ISO_UTF8 (sTitulo);
			gtk_window_set_title (GTK_WINDOW (Janela.window), sTemp);//preparar título convertido para UTF8
		}
	}
	Janela.image = BE_interfaces_widget (Janela.window, "imageImagem");//preparar ícone ao lado da mensagem
	Janela.label = BE_interfaces_widget (Janela.window, "labelMensagem");//preparar mensagem
	//seleção de tipos
	BE_free (sTemp);
	switch (nTipo)
	{
		case GTK_MESSAGE_INFO://mensagem informativa
            gtk_image_set_from_stock (GTK_IMAGE (Janela.image), "gtk-dialog-info", GTK_ICON_SIZE_DIALOG);//carregar ícone informativo
			sTemp = g_strdup_printf ("<span foreground='#000000'>%s</span>", sMensagem);//cor da mensagem é preto
			break;
		case GTK_MESSAGE_WARNING://mensagem de aviso
            gtk_image_set_from_stock (GTK_IMAGE (Janela.image), "gtk-dialog-warning", GTK_ICON_SIZE_DIALOG);//carregar ícone de aviso
			sTemp = g_strdup_printf ("<span foreground='#888800'>%s</span>", sMensagem);//cor da mensagem é laranja
			break;
		case GTK_MESSAGE_QUESTION://pergunta
			gtk_image_set_from_stock (GTK_IMAGE (Janela.image), "gtk-dialog-question", GTK_ICON_SIZE_DIALOG);//carregar ícone de pergunta
			sTemp = g_strdup_printf ("<span foreground='#008800'>%s</span>", sMensagem);//cor da mensagem é verde
			break;
		case GTK_MESSAGE_ERROR://mensagem de erro
            gtk_image_set_from_stock (GTK_IMAGE (Janela.image), "gtk-dialog-error", GTK_ICON_SIZE_DIALOG);//carregar ícone de erro
			sTemp = g_strdup_printf ("<span foreground='#FF0000'>%s</span>", sMensagem);//cor da mensagem é vermelho
			break;
		default://sem mensagem padrão
            gtk_widget_hide (Janela.image);//sem ícones
			sTemp = g_strdup (sMensagem);//preparar mensagem
			break;
	}
	//exibir mensagem
	if (!bUTF8)
	{
		BE_free (sTemp1);
		sTemp1 = sTemp;
		sTemp = BE_ISO_UTF8 (sTemp1);//converter mensagem para UTF8
	}
	gtk_label_set_label (GTK_LABEL (Janela.label), sTemp);//inserir mensagem
    //carregar estrutura
	Janela.botaoOK = BE_interfaces_widget (Janela.window, "buttonOK");
	Janela.botaoCancelar = BE_interfaces_widget (Janela.window, "buttonCancelar");
	Janela.botaoFechar = BE_interfaces_widget (Janela.window, "buttonFechar");
	Janela.botaoSim = BE_interfaces_widget (Janela.window, "buttonSim");
	Janela.botaoNao = BE_interfaces_widget (Janela.window, "buttonNao");
    //selecionar botões
    //o botão padrão é sempre a não confirmação, evita problemas se o usuário apertar <Enter> antes de ler a mensagem
    Janela.nBotao = GTK_RESPONSE_NONE;
	switch (nBotao)
	{
		case GTK_BUTTONS_OK:
            gtk_widget_grab_focus (Janela.botaoOK);//foco no botão
            gtk_widget_hide (Janela.botaoCancelar);//esconder botão
            gtk_widget_hide (Janela.botaoFechar);//esconder botão
            gtk_widget_hide (Janela.botaoSim);//esconder botão
            gtk_widget_hide (Janela.botaoNao);//esconder botão
			break;
		case GTK_BUTTONS_CLOSE:
            gtk_widget_grab_focus (Janela.botaoFechar);
            gtk_widget_hide (Janela.botaoCancelar);
            gtk_widget_hide (Janela.botaoOK);
            gtk_widget_hide (Janela.botaoSim);
            gtk_widget_hide (Janela.botaoNao);
			break;
		case GTK_BUTTONS_CANCEL:
            gtk_widget_grab_focus (Janela.botaoCancelar);
            gtk_widget_hide (Janela.botaoOK);
            gtk_widget_hide (Janela.botaoFechar);
            gtk_widget_hide (Janela.botaoSim);
            gtk_widget_hide (Janela.botaoNao);
			break;
		case GTK_BUTTONS_YES_NO:
            gtk_widget_grab_focus (Janela.botaoNao);
            gtk_widget_hide (Janela.botaoOK);
            gtk_widget_hide (Janela.botaoCancelar);
            gtk_widget_hide (Janela.botaoFechar);
			break;
		case GTK_BUTTONS_OK_CANCEL:
            gtk_widget_grab_focus (Janela.botaoCancelar);
            gtk_widget_hide (Janela.botaoFechar);
            gtk_widget_hide (Janela.botaoSim);
            gtk_widget_hide (Janela.botaoNao);
			break;
        default:
            break;
	}

    //loop de interação com o usuário; só sai daqui quando a janela Message_ext for fechada
    gtk_main ();

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);

	return Janela.nBotao;
}

void _be_Interface_sinal_windowMensagem_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usuário clicar no botão OK ou Sim, esta função é chamada.
Função chamada pelo evento do sinal on_buttonOK_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEMensagem_Janela
    Retorno: não retorna
*******************************************************************************/
    _BEMensagem_Janela *Janela = NULL;

    Janela = (_BEMensagem_Janela *) user_data;//recuperar estrutura
    Janela->nBotao = GTK_RESPONSE_OK;//selecionar botão OK
    gtk_widget_destroy (Janela->window);//fechar janela
}

void _be_Interface_sinal_windowMensagem_on_buttonCancelar_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usuário clicar no botão Cancelar ou Não, esta função é chamada.
Função chamada pelo evento do sinal on_buttonCancelar_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEMensagem_Janela
    Retorno: não retorna
*******************************************************************************/
    _BEMensagem_Janela *Janela = NULL;

    Janela = (_BEMensagem_Janela *) user_data;//recuperar estrutura
    Janela->nBotao = GTK_RESPONSE_CANCEL;//selecionar botão Cancelar
    gtk_widget_destroy (Janela->window);//fechar janela
}

void _be_Interface_sinal_windowMensagem_on_buttonFechar_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usuário clicar no botão Fechar, esta função é chamada.
Função chamada pelo evento do sinal on_buttonFechar_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEMensagem_Janela
    Retorno: não retorna
*******************************************************************************/
    _BEMensagem_Janela *Janela = NULL;

    Janela = (_BEMensagem_Janela *) user_data;//recuperar estrutura
    Janela->nBotao = GTK_RESPONSE_CLOSE;//selecionar botão fechar
    gtk_widget_destroy (Janela->window);//fechar janela
}

void _be_Interface_sinal_windowMensagem_on_buttonSim_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usuário clicar no botão Sim, esta função é chamada.
Função chamada pelo evento do sinal on_buttonSim_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEMensagem_Janela
    Retorno: não retorna
*******************************************************************************/
    _BEMensagem_Janela *Janela = NULL;

    Janela = (_BEMensagem_Janela *) user_data;//recuperar estrutura
    Janela->nBotao = GTK_RESPONSE_YES;//selecionar botão Sim
    gtk_widget_destroy (Janela->window);//fechar janela
}

void _be_Interface_sinal_windowMensagem_on_buttonNao_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
Quando o usuário clicar no botão Não, esta função é chamada.
Função chamada pelo evento do sinal on_buttonNao_clicked.
Função somente para uso interno.
Argumentos:
    widget : botão acionado
    user_data : dados repassados pela função chamadora; estrutura de _BEMensagem_Janela
    Retorno: não retorna
*******************************************************************************/
    _BEMensagem_Janela *Janela = NULL;

    Janela = (_BEMensagem_Janela *) user_data;//recuperar estrutura
    Janela->nBotao = GTK_RESPONSE_NO;//selecionar botão Não
    gtk_widget_destroy (Janela->window);//fechar janela
}

gchar *be_Mensagens_in (GtkWindow *winPai, gboolean bUTF8, const gchar *sTitulo, const gchar *sMensagem, const gchar *sValor)
{
/*******************************************************************************
Exibe uma janela para entrada de dados.
Argumentos:
    winPai : janela parente, superior
    bUTF8 : indica se sTitulo, sMensagem e sValor, já estão em UTF8
    sTitulo : título da mensagem
    sMensagem : mensagem a ser exibida, aceita Pango Text Attribute Markup (quase HTML)
    sValor : texto inicial, que o usuário poderá modificar, ou nulo
    Retorno: novo texto que o usuário digitou em UTF8, ou nulo
*******************************************************************************/
	GtkWidget *window = NULL;
	GtkWidget *label = NULL;
	GtkWidget *entry = NULL;
	gchar *sString = NULL;
	gchar *sTemp = NULL;

	//liberar eventos, não ficar esperando (DoEvents)
    while (gtk_events_pending ())
        gtk_main_iteration ();

	//criar janela de diálogo
	window = be_Interface_criar_dialogEntrada (winPai, NULL, NULL, NULL);
	//título da janela
	if (sTitulo)
	{
		if (bUTF8)
			gtk_window_set_title (GTK_WINDOW (window), sTitulo);//preparar título
		else
		{
			BE_free (sTemp);
			sTemp = BE_ISO_UTF8 (sTitulo);
			gtk_window_set_title (GTK_WINDOW (window), sTemp);//preparar título convertido para UTF8
		}
	}
	//mensagem/rótulo
	label = BE_interfaces_widget (window, "labelMensagem");
	if (bUTF8)
		gtk_label_set_markup (GTK_LABEL (label), sMensagem);//preparar mensagem
	else
	{
		BE_free (sTemp);
		sTemp = BE_ISO_UTF8 (sMensagem);
		gtk_label_set_markup (GTK_LABEL (label), sTemp);//preparar mensagem convertida para UTF8
	}
	//caixa de texto
	entry = BE_interfaces_widget (window, "entryMensagem");
	if (sValor)//valor inicial
	{
		if (bUTF8)
		{
			gtk_entry_set_text (GTK_ENTRY (entry), sValor);//preparar texto
			gtk_editable_select_region (GTK_EDITABLE (entry), 0, strlen (sValor));//permitir seleção do texto
		}
		else
		{
			BE_free (sTemp);
			sTemp = BE_ISO_UTF8 (sValor);
			gtk_entry_set_text (GTK_ENTRY (entry), sTemp);//preparar texto convertido para UTF8
			gtk_editable_select_region (GTK_EDITABLE (entry), 0, strlen (sTemp));//permitir seleção do texto
		}
	}
	//loop de interação com o usuário; só sai daqui quando a janela Message_ext for fechada
	if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_OK)
		sString = g_strdup (gtk_entry_get_text (GTK_ENTRY (entry)));//recuperar novo texto
    //liberando memória
	gtk_widget_destroy (window);
	BE_free (sTemp);

	return sString;
}
