/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BECalendario                                                                 |
+------------------------------------------------------------------------------+
| BECalendario � uma interface livre, voc� pode redistribu�-la e/ou            |
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
#ifndef _BECALENDARIO_H
	#define _BECALENDARIO_H
	#include "../global/beglobal.h"

	//janela
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *calendar;
		GDate *Data;
	} _BECalendario_Janela;
	//Prot�tipos
	GDate *be_Calendario (GtkWindow *winPai, const GDate *Data);
	//Prot�tipos reservados
	void _be_Interface_sinal_windowCalendario_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
#endif
