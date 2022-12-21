/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BESobre                                                                      |
+------------------------------------------------------------------------------+
| BESobre � uma interface livre, voc� pode redistribu�-la e/ou                 |
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
#ifndef _BESOBRE_H
	#define _BESOBRE_H
	#include "../global/beglobal.h"

	//Prot�tipos
	void be_Sobre (
		GtkWindow *winPai,//janela superior, principal, parente
		const gchar *name,//nome do programa
		const gchar *version,//vers�o do programa
		const gchar *copyright,//c�pia registrada, pode ser nulo
		const gchar *website,//site, pode ser nulo
		const gchar *comments,//coment�rios, pode ser nulo
		const gchar **artists,//endere�o de uma matriz com os nomes dos artistas, pode ser nulo
		const gchar **authors,//endere�o de uma matriz com os nomes dos autores, pode ser nulo
		const gchar **documenters//endere�o de uma matriz com os nomes dos documentadores, pode ser nulo
		);//exibe uma janela padr�o, sobre o programa atual
	//Prot�tipos reservado
	void _be_Sobre_email (GtkAboutDialog *about, const gchar *link, gpointer data);//implementa��o futura
	void _be_Sobre_url (GtkAboutDialog *about, const gchar *link, gpointer data);//implementa��o futura
	void _be_Sobre_get_widget (GtkWidget *widget, gpointer data);//rastreamento recursivo de objetos
#endif
