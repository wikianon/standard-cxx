/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2007                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEConsole                                                                    |
+------------------------------------------------------------------------------+
| BEConsole é uma interface livre, você pode redistribuí-la e/ou               |
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
#ifndef _BECONSOLE_H
	#define _BECONSOLE_H
	#include "../sql/besql.h"
	#include "../Mensagens/beMensagens.h"
	#include "../Salvar/beSalvar.h"

	//definições
	#define BECONSOLE_LINHAS_X_COLUNAS_MAX 10000
	#define BECONSOLE_LENSTRING 300
	#define BECONSOLE_NULO "<nulo>"
	#define BECONSOLE_VERTICAL "\\G"
	//Utilizando exibição vertical o BECONSOLE_LENSTRING é ignorado. É exibido todo conteúdo.

	//console
	typedef struct
	{
		GtkWidget *window;
		GtkWidget *tabela;
		GtkWidget *textview;
		GtkWidget *treeview;
		//botões
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

	//funções
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
