/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEAutenticar                                                                 |
+------------------------------------------------------------------------------+
| BEAutenticar � uma interface livre, voc� pode redistribu�-la e/ou            |
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

	//Prot�tipos
	BESQL *be_Autenticar (GtkWindow *winPai, const gchar *sIP, guint nPorta, gint nCacheMax, gboolean bAlterarSenha, const gchar *sSSLChavePrivada, const gchar *sSSLCertificado, const gchar *sSSLCA, const gchar *sSSLCACaminho, const gchar *sSSLCifra);//abre a janela de interface para o usu�rio efetuar o login
	//Prot�tipos reservados
	void _be_Interface_sinal_windowAutenticar_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
#endif
