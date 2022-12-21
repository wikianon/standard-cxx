#include "bePrincipal.h"
#include "../Autenticar/beAutenticar.h"
#include "../monitor/bemonitor.h"

BESQL *be_cia_autenticar (GtkWindow *window, gboolean bAlterarSenha)
{
	BESQL *SQL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sTemp = NULL, *sIP = NULL, *sUsuario = NULL, *sSenha = NULL, *sChavePrivada = NULL, *sCertificado = NULL, *sAutoridade = NULL, *sCaminhoAutoridade = NULL, *sCifras = NULL;
	gchar *sErros = NULL, *sArquivo_Conexao = NULL;
	guint nPorta = 0;
	gint nCacheMax = -1;//padrão é ilimitado

	//preparar configuração local
	BE_free (sTemp);
	sTemp = be_diretorio_local (TRUE);
	sArquivo_Conexao = g_strdup_printf ("%s%s", sTemp, BEPRINCIPAL_CONEXAO_SERVIDOR);
	//verificar se arquivo conexão existe
	if (!g_file_test (sArquivo_Conexao, G_FILE_TEST_EXISTS))
	{
		//copiar de uma versão local
		if (g_file_test (BEPRINCIPAL_CONEXAO_SERVIDOR, G_FILE_TEST_EXISTS))
		{
			BE_free (sTemp);
			#ifdef G_OS_UNIX
				sTemp = g_strdup_printf ("cp %s %s", BEPRINCIPAL_CONEXAO_SERVIDOR, sArquivo_Conexao);
			#endif
			#ifdef G_OS_WIN32
				sTemp = g_strdup_printf ("cp %s \"%s\"", BEPRINCIPAL_CONEXAO_SERVIDOR, sArquivo_Conexao);
			#endif
			system (sTemp);
		}
	}
	//carregar configuração
	Ini = g_key_file_new ();
	if (g_key_file_load_from_file (Ini, sArquivo_Conexao, G_KEY_FILE_NONE, NULL))
	{
		//servidor
		sIP = g_key_file_get_string (Ini, "servidor", "ip", NULL);
		if (!sIP || strlen (sIP) == 0)
		{
			BE_free (sIP);
		}
		sTemp = g_key_file_get_string (Ini, "servidor", "porta", NULL);
		if (sTemp)
			nPorta = atoi (sTemp);
		sUsuario = g_key_file_get_string (Ini, "servidor", "usuario", NULL);
		sSenha = g_key_file_get_string (Ini, "servidor", "senha", NULL);
		//conexão segura SSL
		sChavePrivada = g_key_file_get_string (Ini, "seguranca", "chaveprivada", NULL);
		if (sChavePrivada && strlen (sChavePrivada) == 0)
		{
		    BE_free (sChavePrivada);
		}
		BE_free (sTemp);
		sTemp = sChavePrivada;
		sChavePrivada = be_arquivo_absoluto (sTemp);

		sCertificado = g_key_file_get_string (Ini, "seguranca", "certificado", NULL);
		if (sCertificado && strlen (sCertificado) == 0)
		{
		    BE_free (sCertificado);
		}
		BE_free (sTemp);
		sTemp = sCertificado;
		sCertificado = be_arquivo_absoluto (sTemp);

		sAutoridade = g_key_file_get_string (Ini, "seguranca", "autoridade", NULL);
		if (sAutoridade && strlen (sAutoridade) == 0)
		{
		    BE_free (sAutoridade);
		}
		BE_free (sTemp);
		sTemp = sAutoridade;
		sAutoridade = be_arquivo_absoluto (sTemp);

		sCaminhoAutoridade = g_key_file_get_string (Ini, "seguranca", "caminhoautoridade", NULL);
		if (sCaminhoAutoridade && strlen (sCaminhoAutoridade) == 0)
		{
		    BE_free (sCaminhoAutoridade);
		}
		BE_free (sTemp);
		sTemp = sCaminhoAutoridade;
		sCaminhoAutoridade = be_arquivo_absoluto (sTemp);

		sCifras = g_key_file_get_string (Ini, "seguranca", "cifras", NULL);
		if (sCifras && strlen (sCifras) == 0)
		{
		    BE_free (sCifras);
		}
		//local
		sTemp = g_key_file_get_string (Ini, "local", "sqlcachemax", NULL);
		if (sTemp)
			nCacheMax = atoi (sTemp);
	}
	g_key_file_free (Ini);

	//login
	if (sUsuario && strlen (sUsuario) > 0)
		SQL = be_sql_abrir_conexao_segura (sUsuario, sSenha, sIP, nPorta, nCacheMax, &sErros, sChavePrivada, sCertificado, sAutoridade, sCaminhoAutoridade, sCifras);
	else
		SQL = be_Autenticar (window, sIP, nPorta, nCacheMax, bAlterarSenha, sChavePrivada, sCertificado, sAutoridade, sCaminhoAutoridade, sCifras);

	//informar erros
	if (sErros)
	{
		be_Mensagens (window, TRUE, sErros, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
		BE_free (sErros);
		if (SQL)
		{
			BE_sql_fechar_conexao (SQL);
		}
	}

	//limpar
	BE_free (sTemp);
	BE_free (sIP);
	BE_free (sArquivo_Conexao);

	return SQL;
}

void be_cia_mensagens (const gchar *sMensagens, gpointer user_data)
{
	GtkLabel *label = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;

	label = GTK_LABEL (user_data);

	sTemp = g_strdup_printf ("<span foreground='#FFFFFF'><i>%s\n</i></span>", sMensagens);
	sTemp1 = BE_ISO_UTF8 (sTemp);
	gtk_label_set_markup (label, sTemp1);
	while (gtk_events_pending ())
		gtk_main_iteration ();

	BE_free (sTemp);
	BE_free (sTemp1);
}

int main (int argc, char *argv[])
{
	BESQL *SQL = NULL;
	GtkWidget *window = NULL;
	GtkRcStyle *style = NULL;
	GdkPixbuf *pixbuf = NULL;
	GtkWidget *label = NULL;
	gchar *sTemp = NULL, *sArquivo_Fontes = NULL, *sArquivo_Saida = NULL;
	gboolean bAlterarSenha = FALSE, bReiniciar = TRUE;
	gint nCont = 0;

	//verificar opções de linha de comando
	for (nCont = 1; nCont < argc; nCont++)
	{
		if (g_str_equal (argv[nCont], "--novasenha"))
			bAlterarSenha = TRUE;
		else if (g_str_equal (argv[nCont], "--naomonitor"))
			bReiniciar = FALSE;
		else if (g_str_equal (argv[nCont], "--help") || g_str_equal (argv[nCont], "-h"))//informar opções disponíveis
		{
			printf ("Opções disponiveis:\n"
				"--novasenha : Habilita campos para alterar a senha.\n"
				"--naomonitor : Desabilita a verificacao de erros e atualizacoes.\n"
				"\n"
			);
			return 0;
		}
	}

	//inicializar biblioteca estendida
	be_global_abrir (&argc, &argv, 0);

	//apresentar splash, criar nova janela
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint (GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	//redimenssionamento
	pixbuf = gdk_pixbuf_new_from_file (BEPRINCIPAL_ARQUIVO_SPLASH, NULL);
	if (pixbuf)
	{
		gtk_widget_set_size_request (window, gdk_pixbuf_get_width (pixbuf), gdk_pixbuf_get_height (pixbuf));
		BE_pixbuf_free (pixbuf);
	}
	else
		gtk_widget_set_size_request (window, 640, 480);
	//carregar imagem de fundo
	style = gtk_widget_get_modifier_style (window);
	style->bg_pixmap_name[GTK_STATE_NORMAL] = g_strdup (BEPRINCIPAL_ARQUIVO_SPLASH);//imagem de fundo
	//criar label
	label = gtk_label_new (NULL);
	gtk_container_add (GTK_CONTAINER (window), label);
	gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
	gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_CENTER);
	gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
	gtk_misc_set_alignment (GTK_MISC (label), 0.5, 1);
	//exibir
	gtk_widget_show_all (window);

	//carregando
	be_cia_mensagens ("Carregando...", label);

	//preparar configuração local
	BE_free (sTemp);
	sTemp = be_diretorio_local (TRUE);
	sArquivo_Fontes = g_strdup_printf ("%s%s", sTemp, BEPRINCIPAL_ARQUIVO_FONTES);
	//verificar lista de fontes
	if (!g_file_test (sArquivo_Fontes, G_FILE_TEST_EXISTS))
	{
		be_cia_mensagens ("Localizando fontes...", label);
		be_fontes_lista (sArquivo_Fontes, NULL, NULL);//preparar lista de fontes
	}

	//verificar erros e atualizações
	if (bReiniciar)
	{
		//preparar configuração local
		BE_free (sTemp);
		sTemp = be_diretorio_local_temp (TRUE);
		sArquivo_Saida = g_strdup_printf ("%s%s", sTemp, BEPRINCIPAL_ARQUIVO_SAIDA);
		bReiniciar = be_monitor (sArquivo_Saida, be_cia_mensagens, label);
	}

	if (!bReiniciar)
	{
		//iniciar autenticação
		be_cia_mensagens ("Autenticação...", label);
		SQL = be_cia_autenticar (NULL, bAlterarSenha);

		//fechar splash
		gtk_widget_destroy (window);

		//iniciar aplicação
		if (SQL)
			be_Principal (SQL);

		//fechar conexão
		if (SQL)
			BE_sql_fechar_conexao (SQL);
	}

	//limpar
	BE_free (sTemp);
	BE_free (sArquivo_Fontes);
	BE_free (sArquivo_Saida);

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
