/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEAutenticar                                                                 |
+------------------------------------------------------------------------------+
| BEAutenticar é uma interface livre, você pode redistribuí-la e/ou            |
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
#ifndef _BEAUTENTICAR_H
	#define _BEAUTENTICAR_H
	#include "../global/beglobal.h"
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"

	//janela
	typedef struct
	{
		BESQL *SQL;
		GtkWidget *window;
		GtkWidget *identidade;
		GtkWidget *senha;
		GtkWidget *nova;
		GtkWidget *confirma;
		const gchar *sIP;
        const gchar *sSSLChavePrivada;
        const gchar *sSSLCertificado;
        const gchar *sSSLCA;
        const gchar *sSSLCACaminho;
        const gchar *sSSLCifra;
		guint nPorta;
		gint nCacheMax;
	} _BEAutenticar_Janela;

	//Protótipos
	BESQL *be_Autenticar (GtkWindow *winPai, const gchar *sIP, guint nPorta, gint nCacheMax, gboolean bAlterarSenha, const gchar *sSSLChavePrivada, const gchar *sSSLCertificado, const gchar *sSSLCA, const gchar *sSSLCACaminho, const gchar *sSSLCifra);//abre a janela de interface para o usuário efetuar o login
	//Protótipos reservados
	void _be_Interface_sinal_windowAutenticar_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
#endif
