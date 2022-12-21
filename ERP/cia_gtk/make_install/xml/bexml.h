/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2005                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BEXML                                                                        |
+------------------------------------------------------------------------------+
| BEXML � uma biblioteca livre, voc� pode redistribu�-la e/ou                  |
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
#ifndef _BEXML_H
	#define _BEXML_H
	#include "../global/beglobal.h"
	#include <libxml/xmlreader.h>
	#include <libxml/xmlwriter.h>
	//verificar a vers�o do XML
	#ifdef XML_DEFAULT_VERSION
		#if LIBXML_VERSION < 20611
			#error "VERSAO DO XML2 DEVE SER >= 2.6.11"
		#endif
	#endif
	
	//tipos j� definidos pela biblioteca libxml2
	/*enum xmlReaderTypes (unsignet int)
	{
		XML_READER_TYPE_NONE = 0
		XML_READER_TYPE_ELEMENT = 1
		XML_READER_TYPE_ATTRIBUTE = 2
		XML_READER_TYPE_TEXT = 3
		XML_READER_TYPE_CDATA = 4
		XML_READER_TYPE_ENTITY_REFERENCE = 5
		XML_READER_TYPE_ENTITY = 6
		XML_READER_TYPE_PROCESSING_INSTRUCTION = 7
		XML_READER_TYPE_COMMENT = 8
		XML_READER_TYPE_DOCUMENT = 9
		XML_READER_TYPE_DOCUMENT_TYPE = 10
		XML_READER_TYPE_DOCUMENT_FRAGMENT = 11
		XML_READER_TYPE_NOTATION = 12
		XML_READER_TYPE_WHITESPACE = 13
		XML_READER_TYPE_SIGNIFICANT_WHITESPACE = 14
		XML_READER_TYPE_END_ELEMENT = 15
		XML_READER_TYPE_END_ENTITY = 16
		XML_READER_TYPE_XML_DECLARATION = 17
	}*/
	enum
	{
		BEXML_READER_TYPE_ELEMENT_EMPTY = 18
	};
	//pesquisa
	typedef enum
	{
		BEXML_LOCALIZAR_PAI = 0,
		BEXML_LOCALIZAR_IRMAO,
		BEXML_LOCALIZAR_FILHO
	} BEXML_LOCALIZAR_ENUM;
	
	//atributos
	typedef struct
	{
		gchar *sNome;
		gchar *sValor;
	} _BEXML_Atributo;
	//elemento
	typedef struct
	{
		xmlTextReaderPtr reader;
		gchar *sID;//identifica��o
		gchar *sExpressao;
		gboolean bArquivo;
		glong lPosicao;//posi��o atual do reader
		glong lQuantidade;//elementos e valores
		GSList *Atributos;//atributos, aponta para *_BEXML_Atributo
		gint nQuantidadeAtributos;
	} BEXML;
	
	//modelo em �rvore
	//atributos
	typedef struct
	{
		const gchar *sElementoNome;
		const gchar *sAtributoNome;
		const gchar *sAtributoValor;
		GNode *No;
	} _BEXML_No_Localizar;
	//n�
	typedef struct
	{
		gchar *sNome;
		gchar *sValor;
		GSList *Atributos;//aponta para *BEXML_No_Atributo
	} BEXML_No_Elemento;
	//atributos
	typedef struct
	{
		gchar *sNome;
		gchar *sValor;
	} BEXML_No_Atributo;
	
	//Prot�tipos
	BEXML *be_xml_abrir (const gchar *sExpressao, gboolean bArquivo);
	#define BE_xml_fechar(x); _be_xml_fechar(x);x=NULL;
	glong be_xml_quantidade (BEXML *XML);
	void be_xml_mover (BEXML *XML, const glong lPosicao);
	glong be_xml_mover_id (BEXML *XML, const gchar *sID);
	glong be_xml_posicao (BEXML *XML);
	const gchar *be_xml_valor (BEXML *XML);
	const gchar *be_xml_id (BEXML *XML);//normalmente vai haver ID repetidos
	guint be_xml_tipo (BEXML *XML);
	gint be_xml_nivel (BEXML *XML);
	//Prot�tipos: atributos
	gint be_xml_atributos_quantidade (BEXML *XML);
	const gchar *be_xml_atributos_nomes (BEXML *XML, const gint nPosicao);
	const gchar *be_xml_atributos_valores (BEXML *XML, const gint nPosicao);
	gint be_xml_atributos_localizar (BEXML *XML, const char *sNome);
	//Prot�tipos reservados
	void _be_xml_fechar (BEXML *XML);
	
	//Modelo em �rvore
	GNode *be_xml_no_carregar (BEXML *XML, gboolean bUTF8);
	#define BE_xml_no_descarregar(n); _be_xml_no_descarregar(n);n=NULL;
	const gchar *be_xml_no_atributos_valor (GSList *Atributos, const gchar *sNome);
	GNode *be_xml_no_pai_nivel (GNode *No, int nNivel);//procurar recursivamente pelo pai
	GNode *be_xml_no_nome (GNode *No, const gchar *sElementoNome, BEXML_LOCALIZAR_ENUM nDirecao);
	GNode *be_xml_no_valor (GNode *No, const gchar *sElementoNome, const gchar *sValor, BEXML_LOCALIZAR_ENUM nDirecao);
	GNode *be_xml_no_atributo (GNode *No, const gchar *sElementoNome, const gchar *sAtributoNome, BEXML_LOCALIZAR_ENUM nDirecao);
	GNode *be_xml_no_atributo_valor (GNode *No, const gchar *sElementoNome, const gchar *sAtributoNome, const gchar *sAtributoValor, BEXML_LOCALIZAR_ENUM nDirecao);
	GNode *be_xml_no_localizar_elemento (GNode *No, const gchar *sElementoNome, const gchar *sAtributoNome, const gchar *sAtributoValor);
	//Modelo em �rvore: reservados
	void _be_xml_no_descarregar (GNode *No);
	gboolean _be_xml_no_descarregar_loop (GNode *node, gpointer data);
	gboolean _be_xml_no_localizar_loop (GNode *node, gpointer data);
	gboolean _be_xml_no_loop (GNode *node, gpointer data);//loop para testes de exibi��o
	
	//#define BEXML_DEPURAR
#endif
