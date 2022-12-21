#ifdef PTHREAD
#include <pthread.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include <utils/tostring.h>
#include "xmltext_htmledit.h"

XmlTextHtmlEdit::XmlTextHtmlEdit(std::string editor)
{
    this->editor = editor;
}

XmlTextHtmlEdit::~XmlTextHtmlEdit()
{
}

void XmlTextHtmlEdit::mk_start()
{ 
    add_content("%s.clear()\n", editor.c_str());
}


void XmlTextHtmlEdit::mk_text(XmlParseNode *node, int num)
{ 
    const char *e = editor.c_str();

    add_content("%s.setSize('%s')\n",   e, node->getAttr("size").c_str());
    add_content("%s.setWeight('%s')\n", e, node->getAttr("weight").c_str());
    add_content("%s.setStyle('%s')\n",  e, node->getAttr("style").c_str());
    add_content("%s.addText('%s')\n",   e,
                ToString::mascarade(node->getData().c_str(),"'").c_str());
}


void XmlTextHtmlEdit::mk_part(XmlParseNode *node, int num)
{
    add_content("%s.addPart()\n", editor.c_str());
    add_content("%s.setAlign('%s')\n",  editor.c_str(),
                                        node->getAttr("align").c_str());
}


void XmlTextHtmlEdit::mk_itemize(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.addItemize()\n",e);
}


void XmlTextHtmlEdit::mk_enumerate(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.addEnumerate()\n", e);
}


void XmlTextHtmlEdit::mk_itemize_end(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.partCount = 3\n",e);
    add_content("%s.addPart()\n", e);
}


void XmlTextHtmlEdit::mk_enumerate_end(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.partCount = 3\n",e);
    add_content("%s.addPart()\n", e);
}


void XmlTextHtmlEdit::mk_item(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.addItem()\n", e);
}


void XmlTextHtmlEdit::mk_table(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.addTable()\n", e);

    add_content("%s.setAttr('TABLE','padding', '%s')\n",
                e, node->getAttr("padding").c_str());
    add_content("%s.setAttr('TABLE', 'border', '%s')\n",
                 e, node->getAttr("border").c_str());
    add_content("%s.setAttr('TABLE', 'align', '\"%s\"')\n", 
                 e, node->getAttr("align").c_str());
}


void XmlTextHtmlEdit::mk_table_end(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.partCount = 3\n",e);
    add_content("%s.addPart()\n", e);
}

void XmlTextHtmlEdit::mk_tabrow(XmlParseNode *node, int num)
{
    add_content("%s.addRow()\n", editor.c_str());
}


void XmlTextHtmlEdit::mk_tabcol(XmlParseNode *node, int num)
{
    const char *e = editor.c_str();
    add_content("%s.addColumn()\n",   e);
    if ( node->getAttr("relwidth") != "" )
	add_content("%s.setAttr('TD', 'relwidth', '%s')\n",
		e,
		node->getAttr("relwidth").c_str());

    if ( node->getAttr("padding") != "" )
	add_content("%s.setAttr('TD','padding', '%s')\n",
                e,
		node->getAttr("padding").c_str());

    if ( node->getAttr("valign") != "" )
	add_content("%s.setAttr('TD', 'valign', '\"%s\"')\n",
		e, node->getAttr("valign").c_str());
}

