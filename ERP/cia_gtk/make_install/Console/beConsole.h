/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2007                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEConsole                                                                    |
+------------------------------------------------------------------------------+
| BEConsole � uma interface livre, voc� pode redistribu�-la e/ou               |
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
#ifndef _BECONSOLE_H
	#define _BECONSOLE_H
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"
	#include "../Salvar/beSalvar.h"

	//defini��es
	#define BECONSOLE_LINHAS_X_COLUNAS_MAX 10000
	#define BECONSOLE_LENSTRING 300
	#define BECONSOLE_NULO "<nulo>"
	#define BECONSOLE_VERTICAL "\\G"
	//Utilizando exibi��o vertical o BECONSOLE_LENSTRING � ignorado. � exibido todo conte�do.

	//console
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *tabela;
		GtkWidget *textview;
		GtkWidget *treeview;
		//bot�es
		GtkWidget *executar;
		GtkWidget *exportar;
		GtkWidget *rotulo;
		//consulta
		BESQL *SQL;
		gboolean bVertical;
		//campos
		GSList *Campos;
		gint nCampos;
		gint nLinhas;
	} _BEConsole_Janela;

	//fun��es
	GtkWidget *be_Console (GtkWidget *window, BESQL *SQL);
	//reservadas
	void _be_Console_on_destroy (GtkObject *object, gpointer user_data);//fechar
	void _be_Interface_sinal_windowConsole_on_toolbuttonExecutar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	void _be_Interface_sinal_windowConsole_on_toolbuttonExportar_clicked (GtkToolButton *toolbutton, gpointer user_data);
	gboolean _be_Interface_sinal_windowConsole_on_tableConsole_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
	gboolean _be_Console_callback_SQL (BESQL *SQL, gint nLinha, GSList *Campos, gpointer user_data);
	void _be_Console_adicionar_colunas (_BEConsole_Janela *Janela);
	GtkTreeModel *_be_Console_criar_modelo (_BEConsole_Janela *Janela);
	gboolean _be_Console_exportar_dados (_BEConsole_Janela *Janela, const gchar *sArquivo, const gchar *sNome);
#endif
