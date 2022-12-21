#include "beAutenticar.h"

//incluir janelas geradas automaticamente
#include "../Interfaces/interface_windowAutenticar.c"

/*******************************************************************************
// Esta módulo gera uma interface para o usuário se conectar ao BD.
// Com esta conexão ele recupera a estrutura de conexão com o BD, com somente as
// permissões que o usuário tem acesso.
*******************************************************************************/
BESQL *be_Autenticar (GtkWindow *winPai, const gchar *sIP, guint nPorta, gint nCacheMax, gboolean bAlterarSenha, const gchar *sSSLChavePrivada, const gchar *sSSLCertificado, const gchar *sSSLCA, const gchar *sSSLCACaminho, const gchar *sSSLCifra)
{
/*******************************************************************************
// be_Autenticar
// Esta função abre uma janela de interface com o usuário, para o mesmo digitar
// seu nome de usuário e senha. Também é possível alterar a sua senha.
// Se estiver correta, é retornado a estrutura de conexão com o BD.
// Todas as permissões e validações são feitas pelo servidor de BD, e não pela
// aplicação.
// Também é possível estabelecer uma conexão segura com o BD, através de SSL.
// Argumentos:
// 	winPai : janela pai, se houver
// 	sIP : IP do servidor, ou nulo para localhost
// 	nPorta : porta do servidor, ou nulo para padrão
// 	nCacheMax : quantidade de cache reservado, ver detalhes em besql
// 	bAlterarSenha : se é para alterar a senha, também
// 	sSSLChavePrivada : SSL, ver detalhes em besql
// 	sSSLCertificado : SSL, ver detalhes em besql
// 	sSSLCA : SSL, ver detalhes em besql
// 	sSSLCACaminho : SSL, ver detalhes em besql
// 	sSSLCifra : SSL, ver detalhes em besql
// 	Retorno: nova estrutura de conexão com o BD
*******************************************************************************/
	_BEAutenticar_Janela Janela;
	GtkWidget *frame = NULL;

	//estrutura janela, para compartilhamento privado de variáveis entre funções
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
	//apontar o controle frame, que contém alteração de senha
	frame = BE_interfaces_widget (Janela.window, "frameAlterarSenha");
	//se for para alterar senha, habilitar frame
	if (bAlterarSenha)
		gtk_widget_show_all (frame);
	else
		gtk_widget_hide_all (frame);

	//foco no primeiro controle
	gtk_widget_grab_focus (Janela.identidade);

	//definir uma janela pai, se possível
	if (winPai)
		gtk_window_set_transient_for (GTK_WINDOW (Janela.window), winPai);

	//loop, janela interativa está em execução
	gtk_main ();
	//devolver estrutura de conexão com o BD, ou null
	return Janela.SQL;
}

void _be_Interface_sinal_windowAutenticar_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data)
{
/*******************************************************************************
// _be_Autenticar_on_buttonOK_clicked
// Esta função é chamada dentro da função be_Autenticar, pela geração do sinal botão "OK" pressionado.
// Nesta função é definida autenticação dentro do banco de dados, fornecendo a estrutura SQL, se possível.
// Também é definido a alteração de senha.
// Função somente para uso interno.
// Argumentos:
// 	widget : botão "OK", gerador do sinal "pressionado"
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
	//definir controles como insensíveis, até que se digite alguma coisa
	gtk_widget_set_sensitive (Janela->identidade, FALSE);
	gtk_widget_set_sensitive (Janela->senha, FALSE);
	gtk_widget_set_sensitive (Janela->nova, FALSE);
	gtk_widget_set_sensitive (Janela->confirma, FALSE);
	//mouse pensando
	Mouse = be_janela_mouse_pensando (Janela->window);
	//converter nomes, pois as janelas GTK trabalham em UTF8 e o BD está definido em ISO
	sUsuario = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->identidade)));
	sSenha = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->senha)));
	sNova = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->nova)));
	sConfirma = BE_UTF8_ISO (gtk_entry_get_text (GTK_ENTRY (Janela->confirma)));
	//confirmar senha, antes de enviar ao servidor de BD
	if (sNova && sConfirma && !g_str_equal (sNova, sConfirma))
	{
		be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "A nova senha não foi confirmada.\nDigite novamente.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sUsuario);
	}
	//só conectar se for digitado pelo menos o usuário, mesmo sem senha
	if (sUsuario && strlen (sUsuario) > 0)
		SQL = be_sql_abrir_conexao_segura (sUsuario, sSenha, Janela->sIP, Janela->nPorta, Janela->nCacheMax, &sErros, Janela->sSSLChavePrivada, Janela->sSSLCertificado, Janela->sSSLCA, Janela->sSSLCACaminho, Janela->sSSLCifra);
	//mouse padrão
	BE_janela_mouse_normal (Mouse, TRUE);
	//controles voltam à serem sensíveis
	gtk_widget_set_sensitive (Janela->identidade, TRUE);
	gtk_widget_set_sensitive (Janela->senha, TRUE);
	gtk_widget_set_sensitive (Janela->nova, TRUE);
	gtk_widget_set_sensitive (Janela->confirma, TRUE);
	//alterar senha se o botão de alterar estiver visível
	if (SQL && sNova && sConfirma && GTK_WIDGET_VISIBLE (Janela->nova))
	{
		//tentar alterar senha
		if (be_sql_alterar_senha (SQL, sNova, &sErros))
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Senha alterada com sucesso.", GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
		}
		//não alterou
		else
		{
			be_Mensagens (GTK_WINDOW (Janela->window), FALSE, "Não foi possível alterar a senha.\nTente novamente.", GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
			BE_sql_fechar_conexao (SQL);
		}
	}
	//informar erros gerados pelo BD
	if (sErros)
	{
		be_Mensagens (GTK_WINDOW (Janela->window), TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
	}
	//repassar conexão, se estiver aberta e fechar esta janela
	if (SQL)
	{
		Janela->SQL = SQL;//apontar conexão
		gtk_widget_destroy (Janela->window);//fechar janela
	}
	//conexão não foi aberta, então limpar os campos de textos, para uma nova tentativa
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
