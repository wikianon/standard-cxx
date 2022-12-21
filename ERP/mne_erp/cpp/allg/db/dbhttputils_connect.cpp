#ifdef PTHREAD
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <utils/tostring.h>

#include "dbconnect.h"
#include "dbhttputils_connect.h"

DbHttpUtilsConnect::DbHttpUtilsConnect(DbHttp *h)
          :DbHttpProvider(h),
	   msg("DbHttpUtilsConnect")
{
	subprovider["//start.xml"]         = &DbHttpUtilsConnect::start;
	subprovider["//end.xml"]           = &DbHttpUtilsConnect::end;
	subprovider["//reload.xml"]        = &DbHttpUtilsConnect::reload;

	subprovider["/func/execute.xml"]   = &DbHttpUtilsConnect::func_execute_xml;
	subprovider["/func/mod.xml"]       = &DbHttpUtilsConnect::func_mod_xml;
	subprovider["/func/del.xml"]       = &DbHttpUtilsConnect::func_del_xml;

	subprovider["/sql/execute.xml"]    = &DbHttpUtilsConnect::sql_execute_xml;

	h->add_provider(this);
}

DbHttpUtilsConnect::~DbHttpUtilsConnect()
{
}

int DbHttpUtilsConnect::request(Database *db, HttpHeader *h)
    {

    SubProviderMap::iterator i;

    if ( ( i = subprovider.find(h->dirname + "/" + h->filename)) != subprovider.end() )
    {
    if ( h->filename.find(".xml") == strlen(h->filename.c_str()) - 4 )
	{
		h->status = 200;
		h->content_type = "text/xml";
		add_content(h, 
				"<?xml version=\"1.0\" encoding=\"%s\"?><result>",
				h->charset.c_str());
	    (this->*(i->second))(db, h);
	}
	else
	{
		h->status = 200;
	    (this->*(i->second))(db, h);
	}

	return 1;
    }

    return 0;

}

void DbHttpUtilsConnect::start( Database *db, HttpHeader *h)
{
	if ( db->p_getConnect()->start() )
		if ( h->vars["rollback"] != "" )
			db->p_getConnect()->rollback();
	add_content(h,  "<body>ok</body>");
}

void DbHttpUtilsConnect::end( Database *db, HttpHeader *h)
{
	if ( h->vars["rollback"] != "" )
		db->p_getConnect()->rollback();
	else
		db->p_getConnect()->end();
	add_content(h,  "<body>ok</body>");
}

void DbHttpUtilsConnect::func_mod_xml(Database *db, HttpHeader *h)
{
    std::string stm;
    std::string komma;

    char str[256];
    char typ[256];
    int i;

    stm = "SELECT mne_catalog.pgplsql_proc_create ( ";

    i = 0;
    sprintf(str, "par%d", i);
    sprintf(typ, "typ%d", i);
    while ( h->vars.exists(str) )
    {
	if ( h->vars[typ] == "" || h->vars[typ] == "text" )
	    stm += komma
	        + "E'"
	        +  ToString::mascarade(h->vars[str].c_str(), "'\\")
		+ "'";
	else
	    stm += komma
	        +  ToString::mascarade(h->vars[str].c_str(), "'\\");

	komma = ", ";
	i++;

	sprintf(str, "par%d", i);
	sprintf(typ, "typ%d", i);
    }

    stm += ")";
    if ( db->p_getConnect()->execute(stm.c_str()) == 0 )
    	add_content(h, "<body><schema>%s</schema><fullname>%s</fullname></body>",h->vars["par0"].c_str(), h->vars["par1"].c_str());
    else
    	add_content(h, "<body>error</body>");

    if ( h->vars["sqlend"] != "" )
        db->p_getConnect()->end();

}
void DbHttpUtilsConnect::func_execute_xml(Database *db, HttpHeader *h)
{
    std::string stm;
    std::string komma;

    char str[256];
    char typ[256];
    int i;

    stm = "SELECT " + h->vars["schema"] + "." + h->vars["name"] + "( ";

    i = 0;
    sprintf(str, "par%d", i);
    sprintf(typ, "typ%d", i);
    while ( h->vars.exists(str) )
    {
	if ( h->vars[typ] == "" || h->vars[typ] == "text" )
	    stm += komma
	        + "E'"
	        +  ToString::mascarade(h->vars[str].c_str(), "'\\")
		+ "'";
    else if ( h->vars[typ] == "double" )
    {
        stm += komma + db->p_getConnect()->getValue(DbConnect::DOUBLE, h->vars[str]);
    }
    else if ( h->vars[typ] == "bool" )
    {
        stm += komma + db->p_getConnect()->getValue(DbConnect::BOOL, h->vars[str]);
    }
    else if ( h->vars[typ] == "long" )
    {
        stm += komma + db->p_getConnect()->getValue(DbConnect::LONG, h->vars[str]);
    }
    else if ( h->vars[typ] == "array" )
    {
        stm += komma +  h->vars[str];
    }
    else
    {
        msg.perror(E_TYPE, "#mne_lang#Unbekanter typ <%s>", h->vars[typ].c_str());
        add_content(h, "<body>error</body>");
        if ( h->vars["sqlend"] != "" )
            db->p_getConnect()->end();

        return;
    }

	komma = ", ";
	i++;

	sprintf(str, "par%d", i);
	sprintf(typ, "typ%d", i);
    }

    stm += ")";
    if ( db->p_getConnect()->execute(stm.c_str()) == 0 )
    {
    	DbConnect::ResultMat *r = db->p_getConnect()->p_getResult();
    	if ( r->size() == 0 )
    	  add_content(h, "<body>ok</body>");
    	else
    		add_content(h, "<body><result>%s</result></body>",ToString::mkxml(((*r)[0][0]).format(&msg)).c_str());
    }
    else
    	add_content(h, "<body>error</body>");

    if ( h->vars["sqlend"] != "" )
        db->p_getConnect()->end();

}
void DbHttpUtilsConnect::func_del_xml(Database *db, HttpHeader *h)
{
    std::string stm;
    std::string komma;

    stm  = "SELECT mne_catalog.pgplsql_proc_drop ('";
    stm += h->vars["schemaInput.old"] + "','" + h->vars["fullnameInput.old"] + "')";

    if ( db->p_getConnect()->execute(stm.c_str()) == 0 )
    	add_content(h, "<body>ok</body>");
    else
    	add_content(h, "<body>error</body>");

    if ( h->vars["sqlend"] != "" )
        db->p_getConnect()->end();

}

void DbHttpUtilsConnect::sql_execute_xml(Database *db, HttpHeader *h)
{
    std::string stm;
    stm = h->vars["command"];
    unsigned int i;

    if ( stm != "" && db->p_getConnect()->execute(stm.c_str()) == 0 )
    {
        DbConnect::ResultMat::iterator rm;
        DbConnect::ResultVec::iterator rv, re;
        DbConnect::ResultMat *r = db->p_getConnect()->p_getResult();

        add_content(h, "<head encoding=\"%s\">", h->charset.c_str());

        if ( r->size() > 0 )
        {
            for (i=0; i < (*r)[0].size(); ++i)
            {
                add_content(h,  "<d><id>result%d</id><typ>%d</typ><name>result%d</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n",
                        i, ((*r)[0][i]).typ, i);
            }
        }

        add_content(h,  "</head>");
        add_content(h,  "<body>");

        for (rm = r->begin(); rm != r->end(); ++rm)
        {
            add_content(h,  "<r>");
            rv = (*rm).begin();
            re = (*rm).end();
            for (; rv != re; ++rv)
                add_content(h,  "<v>%s</v>\n", ToString::mkxml( rv->format(&msg)).c_str());
            add_content(h,  "</r>");
        }

        if ( r->empty() )
            add_content(h,  "<r>ok</r>");

        add_content(h,  "</body>");
    }
    else if ( stm != "" )
    {
        add_content(h, "<body>error</body>");
    }
    else
    {
        add_content(h, "<body>ok</body>");

    }

    if (h->vars["sqlend"] != "")
        db->p_getConnect()->end();

}
void DbHttpUtilsConnect::reload(Database *db, HttpHeader *h)
{
    DbTable *tab = db->p_getTable(db->getApplschema(), "update");
    tab->del_allcolumns();
    db->release(tab);

    this->read_datadir();

    msg.pmessage(0, "ok");
    add_content(h, "<body>ok</body>");
}

