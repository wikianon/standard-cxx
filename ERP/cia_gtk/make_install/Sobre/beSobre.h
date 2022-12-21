/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BESobre                                                                      |
+------------------------------------------------------------------------------+
| BESobre é uma interface livre, você pode redistribuí-la e/ou                 |
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
#ifndef _BESOBRE_H
	#define _BESOBRE_H
	#include "../global/beglobal.h"

	//Protótipos
	void be_Sobre (
		GtkWindow *winPai,//janela superior, principal, parente
		const gchar *name,//nome do programa
		const gchar *version,//versão do programa
		const gchar *copyright,//cópia registrada, pode ser nulo
		const gchar *website,//site, pode ser nulo
		const gchar *comments,//comentários, pode ser nulo
		const gchar **artists,//endereço de uma matriz com os nomes dos artistas, pode ser nulo
		const gchar **authors,//endereço de uma matriz com os nomes dos autores, pode ser nulo
		const gchar **documenters//endereço de uma matriz com os nomes dos documentadores, pode ser nulo
		);//exibe uma janela padrão, sobre o programa atual
	//Protótipos reservado
	void _be_Sobre_email (GtkAboutDialog *about, const gchar *link, gpointer data);//implementação futura
	void _be_Sobre_url (GtkAboutDialog *about, const gchar *link, gpointer data);//implementação futura
	void _be_Sobre_get_widget (GtkWidget *widget, gpointer data);//rastreamento recursivo de objetos
#endif
