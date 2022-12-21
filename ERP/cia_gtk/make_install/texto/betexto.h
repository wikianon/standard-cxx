/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BETexto                                                                      |
+------------------------------------------------------------------------------+
| BETexto � uma biblioteca livre, voc� pode redistribu�-la e/ou                |
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
#ifndef _BETEXTO_H
	#define _BETEXTO_H
	#include "../global/beglobal.h"
	#include "../fontes/befontes.h"
	#include "../xml/bexml.h"
	//defini��es
	#define BETEXTO_SUBSOB_ESCRITO 0.58
	#define BETEXTO_SOBRESCRITO 0.33
	#define BETEXTO_SUBESCRITO 0.20
	#define BETEXTO_LARGURA_MIN 4
	#define BETEXTO_ALTURA_MIN 50
		//em caract�res
	//tipos
	typedef struct
	{
		GSList *NoXMLs;//texto estruturado
		GSList *Fontes;//lista de fontes
		gchar *sArquivoFonte;//arquivo com a lista das fontes
		gfloat fTamanho;//tamanho da fonte padr�o
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
	//Prot�tipos
	BETEXTO *be_texto_abrir (const gchar *sArquivoFonte, const gchar *sFontePadrao, const gchar *sEstilo, const gfloat fTamanhoPadrao);
	#define BE_texto_fechar(var); _be_texto_fechar(var);var=NULL;
	GNode *be_texto_analisar (//as respostas da an�lise estar�o como atributos extras
		BETEXTO *Texto, 
		const gchar *sExpressao, //texto a ser analisado, com tags v�lidas
		gfloat fLarguraMax, //largura m�xima do texto; o valor m�nimo � BETEXTO_LARGURA_MIN
		gfloat fAlturaLinha, //altura das linhas em porcentagem; um valor < BETEXTO_ALTURA_MIN ser� considerado o valor padr�o: 100%
		gboolean bQuebrar, //quebrar texto automaticamente
		gboolean bUTF8 //informar se o texto est� em UTF8
		);
	gint be_texto_localizar_fonte (const BETEXTO *Texto, const gchar *sNome, const gchar *sEstilo);
	//const valor = be_xml_no_atributos_valor (Atribuos, Nome);
	//Prot�tipos reservados
	void _be_texto_fechar (BETEXTO *Texto);
	gboolean _be_texto_preparar_texto (GNode *node, gpointer data);
	gboolean _be_texto_preparar_negrito_italico (GNode *node, gpointer data);
	gboolean _be_texto_carregar_fontes (GNode *node, gpointer data);
	gboolean _be_texto_preparar_fontes (GNode *node, gpointer data);
	gboolean _be_texto_dimensoes (GNode *node, gpointer data);
	void _be_texto_adicionar_linha (GNode *Root, BETEXTO *Texto);
	//depura��o
	//#define BETEXTO_DEPURAR
#endif
