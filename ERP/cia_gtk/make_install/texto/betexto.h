/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BETexto                                                                      |
+------------------------------------------------------------------------------+
| BETexto é uma biblioteca livre, você pode redistribuí-la e/ou                |
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
#ifndef _BETEXTO_H
	#define _BETEXTO_H
	#include "../global/beglobal.h"
	#include "../fontes/befontes.h"
	#include "../xml/bexml.h"
	//definições
	#define BETEXTO_SUBSOB_ESCRITO 0.58
	#define BETEXTO_SOBRESCRITO 0.33
	#define BETEXTO_SUBESCRITO 0.20
	#define BETEXTO_LARGURA_MIN 4
	#define BETEXTO_ALTURA_MIN 50
		//em caractéres
	//tipos
	typedef struct
	{
		GSList *NoXMLs;//texto estruturado
		GSList *Fontes;//lista de fontes
		gchar *sArquivoFonte;//arquivo com a lista das fontes
		gfloat fTamanho;//tamanho da fonte padrão
		gint nLinha;
		gint nEspacos;
		gfloat fX;
		gfloat fY;
		gfloat fYMax;
		gfloat fYMin;
		gfloat fYMinAnt;
		gfloat fLarguraMax;
		gfloat fAlturaLinha;
		gfloat fLarguraBloco;
		gfloat fAlturaBloco;
		gboolean bQuebrarAgora;
		gboolean bQuebrar;
	} BETEXTO;
	//Protótipos
	BETEXTO *be_texto_abrir (const gchar *sArquivoFonte, const gchar *sFontePadrao, const gchar *sEstilo, const gfloat fTamanhoPadrao);
	#define BE_texto_fechar(var); _be_texto_fechar(var);var=NULL;
	GNode *be_texto_analisar (//as respostas da análise estarão como atributos extras
		BETEXTO *Texto, 
		const gchar *sExpressao, //texto a ser analisado, com tags válidas
		gfloat fLarguraMax, //largura máxima do texto; o valor mínimo é BETEXTO_LARGURA_MIN
		gfloat fAlturaLinha, //altura das linhas em porcentagem; um valor < BETEXTO_ALTURA_MIN será considerado o valor padrão: 100%
		gboolean bQuebrar, //quebrar texto automaticamente
		gboolean bUTF8 //informar se o texto está em UTF8
		);
	gint be_texto_localizar_fonte (const BETEXTO *Texto, const gchar *sNome, const gchar *sEstilo);
	//const valor = be_xml_no_atributos_valor (Atribuos, Nome);
	//Protótipos reservados
	void _be_texto_fechar (BETEXTO *Texto);
	gboolean _be_texto_preparar_texto (GNode *node, gpointer data);
	gboolean _be_texto_preparar_negrito_italico (GNode *node, gpointer data);
	gboolean _be_texto_carregar_fontes (GNode *node, gpointer data);
	gboolean _be_texto_preparar_fontes (GNode *node, gpointer data);
	gboolean _be_texto_dimensoes (GNode *node, gpointer data);
	void _be_texto_adicionar_linha (GNode *Root, BETEXTO *Texto);
	//depuração
	//#define BETEXTO_DEPURAR
#endif
