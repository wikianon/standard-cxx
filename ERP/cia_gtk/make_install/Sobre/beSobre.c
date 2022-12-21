#include "beSobre.h"

/*******************************************************************************
// Utiliza a fun��o gtk_about_dialog_new() para exibir uma janela padr�o sobre.
*******************************************************************************/
void be_Sobre (
	GtkWindow *winPai,
	const gchar *name,
	const gchar *version,
	const gchar *copyright,
	const gchar *website,
	const gchar *comments,
	const gchar **artists,
	const gchar **authors,
	const gchar **documenters
	)
{
/*******************************************************************************
// be_Sobre
// Exibe uma janela padr�o, sobre o programa atual.
// Uma imagem padr�o (sobre.png) ser� utilizada como fundo.
// Utiliza a fun��o gtk_about_dialog_new().
// Argumentos:
//     winPai : janela superior, principal, parente
//     name : nome do programa
//     version : vers�o do programa
//     copyright : c�pia registrada, pode ser nulo
//     website : site, pode ser nulo
//     comments : coment�rios, pode ser nulo
//     artists : endere�o de uma matriz com os nomes dos artistas, pode ser nulo
//     authors : endere�o de uma matriz com os nomes dos autores, pode ser nulo
//     documenters : endere�o de uma matriz com os nomes dos documentadores, pode ser nulo
//     Retorno: sem retorno
*******************************************************************************/
	GtkWidget *dialog = NULL;
	GdkPixbuf *pixbuf = NULL;
	GtkStyle *style = NULL;
	GdkPixmap *pixmap = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gchar *sLicense = NULL;

	sTemp = g_strdup_printf (
		"%s � um software livre,\n"
		"voc� pode redistribu�-lo e/ou modific�-lo dentro dos termos da\n"
		"Licen�a P�blica Geral GNU como publicada pela\n"
		"Funda��o do Software Livre (FSF); na vers�o 2 da Licen�a,\n"
		"ou (na sua opni�o) qualquer vers�o. Este programa � distribuido na\n"
		"esperan�a que possa ser �til, mas SEM NENHUMA GARANTIA; sem uma garantia\n"
		"implicita de ADEQUA��O a qualquer MERCADO ou APLICA��O EM PARTICULAR.\n"
		"Veja a Licen�a P�blica Geral GNU para maiores detalhes.\n"
		"Voc� deve ter recebido uma c�pia da Licen�a P�blica Geral GNU junto com\n"
		"este programa, se n�o, escreva para a Funda��o do Software Livre(FSF)\n"
		"Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.", name);//licen�a livre
	sLicense = BE_ISO_UTF8 (sTemp);//converter para UTF8

	dialog = gtk_about_dialog_new ();//criar janela sobre
	if (winPai)
		gtk_window_set_transient_for (GTK_WINDOW (dialog), winPai);//relacionar uma janela superior
	gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);//tem que fechar esta, antes de continuar
	gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);//centralizar janela
	//redimenssionamento
    pixbuf = gdk_pixbuf_new_from_file ("sobre.png", NULL);
    if (pixbuf)
    {
        gtk_widget_set_size_request (dialog, gdk_pixbuf_get_width (pixbuf), gdk_pixbuf_get_height (pixbuf));
        BE_pixbuf_free (pixbuf);
    }
    else
        gtk_widget_set_size_request (dialog, 640, 480);
	//carregar imagem de fundo
	gdk_pixbuf_render_pixmap_and_mask (gdk_pixbuf_new_from_file ("sobre.png", NULL), &pixmap, NULL, 255);
	style = gtk_style_copy (gtk_widget_get_style (dialog));
    style->bg_pixmap[GTK_STATE_NORMAL] = pixmap;
    gtk_widget_set_style (dialog, style);

    //propriedades
	gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (dialog), name);//carregar nome
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), version);//carregar vers�o
	if (copyright)
		gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog), copyright);//carregar c�pia registrada
	if (sLicense)
		gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (dialog), sLicense);
	if (website)
		gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), website);
	if (comments)
		gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), comments);
	if (artists)
		gtk_about_dialog_set_artists (GTK_ABOUT_DIALOG (dialog), artists);
	if (authors)
		gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);
	if (documenters)
		gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (dialog), documenters);
	//gtk_about_dialog_set_email_hook (_be_Sobre_email, NULL, NULL);//configurar link dos e-mails
	//gtk_about_dialog_set_url_hook (_be_Sobre_email, NULL, NULL);//configurar link de url

	//rastrear labels e mudar a cor para branco
	gtk_container_forall (GTK_CONTAINER (dialog), _be_Sobre_get_widget, NULL);
	//ativar callback da emails e urls
	gtk_about_dialog_set_email_hook (_be_Sobre_email, NULL, NULL);
	gtk_about_dialog_set_url_hook (_be_Sobre_email, NULL, NULL);

	//loop de intera��o com o usu�rio; s� sai daqui quando a janela Sobre for fechada
	gtk_dialog_run (GTK_DIALOG (dialog));

    //liberar tudo
    gtk_widget_destroy (dialog);
	BE_free (sLicense);
	BE_free (sTemp);
	BE_free (sTemp1);
}

void _be_Sobre_email (GtkAboutDialog *about, const gchar *link, gpointer data)
{
/*******************************************************************************
// _be_Sobre_email
// Implementa��o futura
*******************************************************************************/
	/*gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;

	if (g_strrstr (link, "@"))
	{
		sTemp = be_arquivo_aplicativo ("enviaremail");
		sTemp1 = g_strdup_printf ("mailto:%s", link);
	}
	else
	{
		sTemp = be_arquivo_aplicativo ("navegador");
		sTemp1 = g_strdup_printf ("%s", link);
	}
	if (sTemp && sTemp1)
	{
		#ifdef G_OS_UNIX
			sTemp2 = g_strdup_printf ("%s %s", sTemp, sTemp1);//preparar
		#endif
		#ifdef G_OS_WIN32
			sTemp2 = g_strdup_printf ("\"%s\" %s", sTemp, sTemp1);//preparar
		#endif
		//abrir aplicativo
		g_spawn_command_line_async (sTemp2, NULL);
		//printf ("%s\n", sTemp2);
	}

	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);*/
}

void _be_Sobre_url (GtkAboutDialog *about, const gchar *link, gpointer data)
{
/*******************************************************************************
// _be_Sobre_url
// Implementa��o futura
*******************************************************************************/
	//g_print ("show url %s\n", link);
	//_be_Sobre_email (about, link, data);
}

void _be_Sobre_get_widget (GtkWidget *widget, gpointer data)
{
/*******************************************************************************
// _be_Sobre_get_widget
// Esta fun��o � um callback, chamada � por gtk_container_forall.
// Ela rastrea todos os controles r�tulos e muda a cor da fonte para branco.
// Fun��o reservada, para uso interno.
*******************************************************************************/
    gchar *sTemp = NULL;

    //se for um container e n�o um bot�o, verificar os controles filhos
    if (GTK_IS_CONTAINER (widget) && !GTK_IS_BUTTON (widget))
 		gtk_container_forall (GTK_CONTAINER (widget), _be_Sobre_get_widget, NULL);//recursividade
	//� um r�tulo
    else if (GTK_IS_LABEL (widget))
    {
 		//printf ("%s == %s\n", gtk_label_get_label (GTK_LABEL (widget)), gtk_label_get_text (GTK_LABEL (widget)));
 		//n�o alterar os labels dos bot�es
 		if (gtk_label_get_label (GTK_LABEL (widget))[0] != '_')
 			gtk_label_set_markup (GTK_LABEL (widget), sTemp = g_strdup_printf ("<span foreground='#FFFFFF'>%s</span>", gtk_label_get_label (GTK_LABEL (widget))));
    }
    //limpar
    BE_free (sTemp);
}
