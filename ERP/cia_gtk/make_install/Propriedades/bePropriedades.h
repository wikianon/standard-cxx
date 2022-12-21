/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEProprieades                                                                |
+------------------------------------------------------------------------------+
| BEPropriedades é uma interface livre, você pode redistribuí-la e/ou          |
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
#ifndef _BEPROPRIEDADES_H
	#define _BEPROPRIEDADES_H
	#include "../global/beglobal.h"
	#include "../Fonte/beFonte.h"
	#include "../Cor/beCor.h"
	#include "../Abrir/beAbrir.h"
	#include "../Diretorio/beDiretorio.h"
	#include "../Lista/beLista.h"

	//enumerações
	typedef enum
	{
		BEPROPRIEDADES_NENHUM = 0,
		BEPROPRIEDADES_ROTULO,
		BEPROPRIEDADES_TEXTO,
		BEPROPRIEDADES_NUMERO,
		BEPROPRIEDADES_LOGICO,
		BEPROPRIEDADES_FONTE,
		BEPROPRIEDADES_COR,
		BEPROPRIEDADES_ARQUIVO,
		BEPROPRIEDADES_ARQUIVO_LIVRE,
		BEPROPRIEDADES_DIRETORIO,
		BEPROPRIEDADES_LISTA
	} BEPROPRIEDADES_ENUM;
	//janela
	typedef struct
	{
		gboolean bOK;
		GtkWidget *window;
		GSList *Propriedades;
	} _BEPropriedades_Janela;
	//propriedades
	typedef struct
	{
		BEPROPRIEDADES_ENUM nTipo;
		gboolean bUTF8;
		gchar *sNome;
		gchar **Valor;
		GSList *Lista;
		GtkWidget *label;
		GtkWidget *entry;
		GtkWidget *button;
		_BEPropriedades_Janela *Janela;
	} _BEPropriedades_Tipo;
	//Protótipos
	GSList *be_Propriedades_adicionar (GSList *Propriedades, gboolean bUTF8, BEPROPRIEDADES_ENUM nTipo, const gchar *sNome, gchar **Valor, GSList *Lista);
	gboolean be_Propriedades (GtkWindow *winPai, GSList *Propriedades);
	//Protótipos reservados
	void _be_Propriedades_Liberar (GSList *Propriedades);
	void _be_Interface_sinal_windowPropriedades_on_buttonOK_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowPropriedades_on_buttonFonte_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowPropriedades_on_buttonCor_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowPropriedades_on_buttonArquivo_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowPropriedades_on_buttonDiretorio_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowPropriedades_on_buttonLista_clicked (GtkWidget *widget, gpointer user_data);
	void _be_Interface_sinal_windowPropriedades_on_checkbuttonLogico_toggled (GtkToggleButton *togglebutton, gpointer user_data);
#endif
