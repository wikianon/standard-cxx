#include "beAutenticar.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowAutenticar.c"

/*******************************************************************************
// Esta m�dulo gera uma interface para o usu�rio se conectar ao BD.
// Com esta conex�o ele recupera a estrutura de conex�o com o BD, com somente as
// permiss�es que o usu�rio tem acesso.
*******************************************************************************/
BESQL *be_Autenticar (GtkWindow *winPai, const gchar *sIP, guint nPorta, gint nCacheMax, gboolean bAlterarSenha, const gchar *sSSLChavePrivada, const gchar *sSSLCertificado, const gchar *sSSLCA, const gchar *sSSLCACaminho, const gchar *sSSLCifra)
{
/*******************************************************************************
// be_Autenticar
// Esta fun��o abre uma janela de interface com o usu�rio, para o mesmo digitar
// seu nome de usu�rio e senha. Tamb�m � poss�vel alterar a sua senha.
// Se estiver correta, � retornado a estrutura de conex�o com o BD.
// Todas as permiss�es e valida��es s�o feitas pelo servidor de BD, e n�o pela
// aplica��o.
// Tamb�m � poss�vel estabelecer uma conex�o segura com o BD, atrav�s de SSL.
// Argumentos:
// 	winPai : janela pai, se houver
// 	sIP : IP do servidor, ou nulo para localhost
// 	nPorta : porta do servidor, ou nulo para padr�o
// 	nCacheMax : quantidade de cache reservado, ver detalhes em besql
// 	bAlterarSenha : se � para alterar a senha, tamb�m
// 	sSSLChavePrivada : SSL, ver detalhes em besql
// 	sSSLCertificado : SSL, ver detalhes em besql
// 	sSSLCA : SSL, ver detalhes em besql
// 	sSSLCACaminho : SSL, ver detalhes em besql
// 	sSSLCifra : SSL, ver detalhes em besql
// 	Retorno: nova estrutura de conex�o com o BD
*******************************************************************************/
	_BEAutenticar_Janela Janela;
	GtkWidget *frame = NULL;

	//estrutura janela, para compartilhamento privado de vari�veis entre fun��es
	Janela.SQL = NULL;
	Janela.window = be_Interface_criar_windowAutenticar (winPai, NULL, NULL, &Janela);
	Janela.identidade = BE_interfaces_widget (Janela.window, "entryIdentidade");
	Janela.senha = BE_interfaces_widget (Janela.window, "entrySenha");
	Janela.nova = BE_interfaces_widget (Janela.window, "entryNova");
	Janela.confirma = BE_interfaces_widget (Janela.window, "entryConfirma");
	Janela.sIP = sIP;
	Janela.nPorta = nPorta;
	Janela.nCacheMax = nCacheMax;
    Janela.sSSLChavePrivada = sSSLChavePrivada;
    Janela.sSSLCertificado = sSSLCertificado;
    Janela.sSSLCA = sSSLCA;
    Janela.sSSLCACaminho = sSSLCACaminho;
    Janela.sSSLCifra = sSSLCifra;
	//apontar o controle frame, que cont�m altera��o de senha
	frame = BE_interfaces_widget (Janela.window, "frameAlterarSenha");
	//se for para alterar senha, habilitar frame
	if (bAlterarSenha)
		gtk_widget_show_all (frame);
	else
		gtk_widget_hide_all (frame);

	//foco no primeiro controle
	gtk_widget_grab_focus (Janela.identidade);

	//definir uma janela pai, se poss�vel
	if (winPai)
		gtk_window_set_transient_for (GTK_WINDOW (Janela.window), winPai);

	//loop, janela interativa est� em execu��o
	gtk_main ();
	//devolver estrutura de conex�o com o BD, ou null
	return Janela.SQL;
}

void _be_Interface_sinal_windowAutenticar_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
// _be_Autenticar_on_buttonOK_clicked
// Esta fun��o � chamada dentro da fun��o be_Autenticar, pela gera��o do sinal bot�o "OK" pressionado.
// Nesta fun��o � definida autentica��o dentro do banco de dados, fornecendo a estrutura SQL, se poss�vel.
// Tamb�m � definido a altera��o de senha.
// Fun��o somente para uso interno.
// Argumentos:
// 	widget : bot�o "OK", gerador do sinal "pressionado"
// 	user_data : ponteiro para a estrutura Janela, criado em  be_Autenticar
// 	Retorno: sem retorno
*******************************************************************************/
	_BEAutenticar_Janela *Janela = NULL;
	BESQL *SQL = NULL;
	BEGlobal_Janela *Mouse = NULL;
	gchar *sErros = NULL;
	gchar *sUsuario = NULL, *sSenha = NULL, *sNova = NULL, *sConfirma = NULL;

	//obter a estrutura existente
	Janela = (_BEAutenticar_Janela *) user_data;
	//definir controles como insens�veis, at� que se digite alguma coisa
	gtk_widget_set_sensitive (Janela->identidade, FALSE);
	gtk_widget_set_sensitive (Janela->senha, FALSE);
	gtk_widget_set_sensitive (Janela->nova, FALSE);
	gtk_widget_set_sensitive (Janela->confirma, FALSE);
	//mouse pensando
	Mouse = be_janela_mouse_pensando (Janela->window);
	//converter nomes, pois as janelas GTK trabalham em UTF8 e o BD est� definido em ISO
	sUsuario = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->identidade)));
	sSenha = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->senha)));
	sNova = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->nova)));
	sConfirma = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->confirma)));
	//confirmar senha, antes de enviar ao servidor de BD
	if (sNova && sConfirma && !g_str_equal (sNova, sConfirma))
	{
		be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "A nova senha n�o foi confirmada.\nDigite novamente.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sUsuario);
	}
	//s� conectar se for digitado pelo menos o usu�rio, mesmo sem senha
	if (sUsuario && strlen (sUsuario) > 0)
		SQL = be_sql_abrir_conexao_segura (sUsuario, sSenha, Janela->sIP, Janela->nPorta, Janela->nCacheMax, &sErros, Janela->sSSLChavePrivada, Janela->sSSLCertificado, Janela->sSSLCA, Janela->sSSLCACaminho, Janela->sSSLCifra);
	//mouse padr�o
	BE_janela_mouse_normal (Mouse, TRUE);
	//controles voltam � serem sens�veis
	gtk_widget_set_sensitive (Janela->identidade, TRUE);
	gtk_widget_set_sensitive (Janela->senha, TRUE);
	gtk_widget_set_sensitive (Janela->nova, TRUE);
	gtk_widget_set_sensitive (Janela->confirma, TRUE);
	//alterar senha se o bot�o de alterar estiver vis�vel
	if (SQL && sNova && sConfirma && GTK_WIDGET_VISIBLE (Janela->nova))
	{
		//tentar alterar senha
		if (be_sql_alterar_senha (SQL, sNova, &sErros))
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Senha alterada com sucesso.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		}
		//n�o alterou
		else
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "N�o foi poss�vel alterar a senha.\nTente novamente.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_sql_fechar_conexao (SQL);
		}
	}
	//informar erros gerados pelo BD
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	//repassar conex�o, se estiver aberta e fechar esta janela
	if (SQL)
	{
		Janela->SQL = SQL;//apontar conex�o
		gtk_widget_destroy (Janela->window);//fechar janela
	}
	//conex�o n�o foi aberta, ent�o limpar os campos de textos, para uma nova tentativa
	else
	{
		gtk_entry_set_text (GTK_ENTRY (Janela->identidade), "");
		gtk_entry_set_text (GTK_ENTRY (Janela->senha), "");
		gtk_entry_set_text (GTK_ENTRY (Janela->nova), "");
		gtk_entry_set_text (GTK_ENTRY (Janela->confirma), "");
		//foco no primeiro campo de texto
		if (Janela->identidade && GTK_IS_WIDGET (Janela->identidade))
            gtk_widget_grab_focus (Janela->identidade);
	}
	//limpar
	BE_free (sUsuario);
	BE_free (sSenha);
	BE_free (sNova);
	BE_free (sConfirma);
}
