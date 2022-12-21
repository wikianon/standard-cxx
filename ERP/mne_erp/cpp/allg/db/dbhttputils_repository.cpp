#ifdef PTHREAD
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>

#include <utils/tostring.h>
#include <utils/process.h>
#include <argument/argument.h>

#include "dbquery.h"
#include "dbhttputils_repository.h"

#if defined(__MINGW32__) || defined(__CYGWIN__)
#define DIRSEP   "\\"
#else
#define DIRSEP   "/"
#endif

DbHttpUtilsRepository::DbHttpUtilsRepository(DbHttp *h)
                      :HttpFilesystem(h, 1),
                       DbHttpProvider(h),
                       msg("DbHttpUtilsRepository"),
                       query(h, 1),
                       table(h, 1)
{
    Argument a;

    subprovider["insert.xml"]   = &DbHttpUtilsRepository::insert_xml;
    subprovider["modify.xml"]   = &DbHttpUtilsRepository::modify_xml;
    subprovider["delete.xml"]   = &DbHttpUtilsRepository::delete_xml;
    subprovider["data.xml"]     = &DbHttpUtilsRepository::data_xml;

    subprovider["ls.xml"]       = &DbHttpUtilsRepository::ls;
    subprovider["mkdir.xml"]    = &DbHttpUtilsRepository::mkdir;
    subprovider["rmdir.xml"]    = &DbHttpUtilsRepository::rmdir;
    subprovider["addfile.xml"]  = &DbHttpUtilsRepository::addfile;
    subprovider["mkfile.html"]  = &DbHttpUtilsRepository::mkfile;
    subprovider["rmfile.xml"]   = &DbHttpUtilsRepository::rmfile;
    subprovider["mv.xml"]       = &DbHttpUtilsRepository::mv;
    subprovider["commit.xml"]   = &DbHttpUtilsRepository::commit;
    subprovider["log.xml"]      = &DbHttpUtilsRepository::log;
    subprovider["download.html"] = &DbHttpUtilsRepository::download;
    subprovider["downloadall.html"] = &DbHttpUtilsRepository::downall;

    subprovider["dblog_update.xml"] = &DbHttpUtilsRepository::dblog_update;
    subprovider["dblog.xml"]        = &DbHttpUtilsRepository::dblog;

    need_root = 0;
    gitcmd = std::string(a["gitcmd"]);

    h->add_provider(this);
}

DbHttpUtilsRepository::~DbHttpUtilsRepository()
{
}

int DbHttpUtilsRepository::request(Database *db, HttpHeader *h)
{
    SubProviderMap::iterator i;
    if ( ( i = subprovider.find(h->filename)) != subprovider.end() )
    {
        h->status = 200;
        h->content_type = "text/xml";
        read_name(db, h);
        (this->*(i->second))(db, h);
        return 1;
    }

    return 0;

}
int DbHttpUtilsRepository::exec(const CsList *cmd, const char *workdir)
{
    char buffer[1025];
    int anzahl;
    int retval;
    CsList cc = *cmd;

    execlog = "";
    Process p(DbHttpProvider::http->getServersocket(), 0);
    p.start(*cmd, "pipe", workdir);
    while( ( anzahl = p.read(buffer, sizeof(buffer) - 1)) != 0 )
    {
        if ( anzahl > 0 )
        {
            buffer[anzahl] = '\0';
            execlog += buffer;
        }
        else if ( anzahl < 0 && errno != EAGAIN ) break;
    }

    retval = p.getStatus();
    return retval;
}

void DbHttpUtilsRepository::read_name(Database *db, HttpHeader *h)
{
    DbConnect::ResultMat *r;
    DbTable::ValueMap where;
    DbTable *tab;
    CsList cols("root,name");

    tab = db->p_getTable("mne_repository", "repository");

    where["repositoryid"] = h->vars["repositoryidInput.old"];
    r = tab->select(&cols, &where);
    tab->end();
    db->release(tab);

    if ( ! r->empty() )
    {
        (*h->vars.p_getVars())["rootInput.old"] = (char*)((*r)[0][0]);
        (*h->vars.p_getVars())["nameInput.old"] = (char*)((*r)[0][1]);
    }

}

std::string DbHttpUtilsRepository::getRoot(HttpHeader *h)
{
    std::string dir = HttpFilesystem::getRoot(h);
    std::string rep = h->vars["nameInput.old"];

    if ( dir == "" || need_root )
    {
        need_root = 0;
        return dir;
    }

    if ( rep == "")
    {
        msg.perror(E_REPOSITORY, "Der Aktenordner muss einen Namen haben");
        return "";
    }

    return dir + DIRSEP + rep;
}

void DbHttpUtilsRepository::insert_xml (Database *db, HttpHeader *h)
{
    std::string str;
    std::string val;
    std::string root;
    std::string templ;
    CsList cmd;
    int result;

    DbTable::ColumnMap::iterator ci;
    DbTable::ValueMap vals;
    DbTable::ValueMap orig_vals;
    DbTable::ValueMap where;
    DbTable::ValueMap::iterator wi;

    h->status = 200;
    h->content_type = "text/xml";

    (*h->vars.p_getVars())["rootInput.old"] = h->vars["rootInput"];
    if ( (root = HttpFilesystem::getRoot(h)) == "" )
    {
        msg.perror(E_MKREPOSITORY,"Der Aktenschrank <%s> existiert nicht", h->vars["rootInput"].c_str());
        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
        return;
    }

    if ( check_path(root.c_str(), h->vars["nameInput"].c_str(), 1, 0) != "" )
    {
        msg.pwarning(E_MKREPOSITORY,"Der Aktenordner <%s> existiert schon", h->vars["nameInput"].c_str());
    }
    else
    {
        cmd.add(gitcmd.c_str());
        cmd.add("init");
        cmd.add("--quiet");
        cmd.add("--shared=group");
        cmd.add(h->vars["nameInput"]);

        Process p(DbHttpProvider::http->getServersocket());
        p.start(cmd, NULL, root.c_str());
        result = p.wait();

        if ( result != 0 )
        {
            msg.perror(E_MKREPOSITORY,"Fehler während des Erzeugens eines Aktenordners");
            DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
            return;
        }

        mode_t mask = umask(0);
        umask(mask);
        chmod( (root + DIRSEP + h->vars["nameInput"]).c_str(), (02777 & ~ mask) );

        templ = msg.get("RepositoryVorlage");
        if ( check_path(root.c_str(), templ.c_str(), 1, 0) != "" && h->vars["nameInput"] != templ && templ != "" )
        {
            cmd.setString("");
            cmd.add("sh");
            cmd.add("-c");
            cmd.add("( cd \"" + templ + "\"; tar -cf - --exclude .git .) | ( cd \"" + h->vars["nameInput"] + "\"; tar -xf - )");

            p.start(cmd, NULL, root.c_str());
            result = p.wait();

            if ( result != 0 )
            {
                msg.perror(E_MKREPOSITORY,"Fehler während des Erzeugens eines Aktenordners");
                DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
                return;
            }

            cmd.setString("");
            cmd.add(gitcmd.c_str());
            cmd.add("add");
            cmd.add(".");

            p.start(cmd, NULL, ( root + DIRSEP + h->vars["nameInput"]).c_str());
            result = p.wait();

            if ( result != 0 )
            {
                msg.perror(E_MKREPOSITORY,"Fehler während des Erzeugens eines Aktenordners");
                DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
                return;
            }

            DbHttpAnalyse::Client::Userprefs userprefs = DbHttpProvider::http->getUserprefs();
            cmd.setString("");
            cmd.add(gitcmd.c_str());
            cmd.add("commit");
            cmd.add("-m");
            cmd.add(msg.get("Initialversion aus Vorlage"));
            cmd.add("--author=\"" + userprefs["fullname"] + " <" + userprefs["email"] + ">\"" );

            p.start(cmd, NULL, ( root + DIRSEP + h->vars["nameInput"]).c_str());
            result = p.wait();

            if ( result != 0 )
            {
                msg.perror(E_MKREPOSITORY,"Fehler während des Erzeugens eines Aktenordners");
                DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
                return;
            }
        }
    }

    (*h->vars.p_getVars()).erase("rootInput.old");
    (*h->vars.p_getVars()).erase("nameInput.old");

    table.insert_xml(db, h);
    return;

}

void DbHttpUtilsRepository::modify_xml (Database *db, HttpHeader *h)
{
    std::string root;
    std::string name;

    CsList cmd;
    int result;

    h->status = 200;
    h->content_type = "text/xml";
    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());

    root = h->vars["rootInput.old"];
    name = h->vars["nameInput.old"];

    if ( HttpFilesystem::getRoot(h) == "" || check_path(HttpFilesystem::getRoot(h).c_str(), name.c_str()) == "" )
    {
        msg.perror(E_MKREPOSITORY,"Der Aktenordner <%s> existiert nicht", (root + ":" + name).c_str());
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    if ( name == "" || h->vars["nameInput"] == "" )
    {
        msg.perror(E_MKREPOSITORY,"Die Aktenordner müssen eine Namen haben");
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    if ( name != h->vars["nameInput"] )
    {
        cmd.add("mv");
        cmd.add(name);
        cmd.add(h->vars["nameInput"]);

        Process p(DbHttpProvider::http->getServersocket());
        p.start(cmd, NULL, HttpFilesystem::getRoot(h).c_str());
        result = p.wait();

        if ( result != 0 )
        {
            std::string str = msg.getSystemerror(errno);
            msg.perror(E_MKREPOSITORY,"Fehler während des Umbenennen eines Aktenordners");
            msg.line("%s", str.c_str());
            DbHttpProvider::add_content(h,  "<body>error</body>");
            return;
        }
    }

    (*h->vars.p_getVars()).erase("rootInput.old");
    (*h->vars.p_getVars()).erase("nameInput.old");
    DbHttpProvider::del_content(h);
    table.modify_xml(db,h);
}

void DbHttpUtilsRepository::delete_xml (Database *db, HttpHeader *h)
{
    std::string root;

     time_t rawtime;
     struct tm * timeinfo;
     char buffer [80];

     time (&rawtime);
     timeinfo = gmtime (&rawtime);

     strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);

    if ( (root = HttpFilesystem::getRoot(h)) == "" )
    {
        msg.perror(E_MKREPOSITORY,"Der Aktenordner <%s> existiert nicht", (h->vars["rootInput.old"] + ":" + h->vars["nameInput.old"]).c_str());
        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
        return;
    }

    if ( h->vars["nameInput.old"] == "" )
     {
         msg.perror(E_DELREPOSITORY,"Der Aktenordner muss eine Namen haben");
         DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
         return;
     }

    if ( check_path(root.c_str(), h->vars["nameInput.old"].c_str(), 1, 0) == "" )
    {
        msg.pwarning(E_DELREPOSITORY,"Der Aktenordner <%s> existiert nicht", h->vars["nameInput.old"].c_str());
    }
#if defined(__MINGW32__) || defined(__CYGWIN__)
    else
    {
        SetFileAttributes(( root + DIRSEP + h->vars["nameInput.old"] ).c_str(), FILE_ATTRIBUTE_NORMAL);
        if ( ! MoveFile(( root + DIRSEP + h->vars["nameInput.old"] ).c_str(), ( root + DIRSEP + ".trash" + DIRSEP + h->vars["nameInput.old"] + "_" + buffer).c_str()) )
        {
            std::string str = msg.getSystemerror(GetLastError());
            msg.perror(E_DELREPOSITORY,"Fehler während des Löschens eines Aktenordners");
            msg.line("%s", str.c_str());
            DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
            return;
        }
    }
#else
    else if ( rename( ( root + DIRSEP + h->vars["nameInput.old"] ).c_str(), ( root + DIRSEP + ".trash" + DIRSEP + h->vars["nameInput.old"] + "_" + buffer).c_str()) != 0 )
    {
        std::string str = msg.getSystemerror(errno);
        msg.perror(E_DELREPOSITORY,"Fehler während des Löschens eines Aktenordners");
        msg.line("%s", str.c_str());
        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
        return;
    }
#endif
    (*h->vars.p_getVars()).erase("rootInput.old");
    (*h->vars.p_getVars()).erase("nameInput.old");

    (*h->vars.p_getVars())["table"] = "filedata";
    table.delete_xml(db,h);

    (*h->vars.p_getVars())["table"] = "fileinterests";
    table.delete_xml(db,h);

    DbHttpProvider::del_content(h);
    (*h->vars.p_getVars())["table"] = "repository";
    table.delete_xml(db,h);

}

void DbHttpUtilsRepository::dbdata_update ( Database *db, HttpHeader *h)
{
    std::vector<FileData>::iterator is;
    DbTable *tab;
    DbTable::ValueMap where,vals;
    DbConnect::ResultMat *r;
    std::string root;


    std::string schema;
    std::string table;

    if ( h->vars["wcol"] != "" )
    {
        CsList wcol(h->vars["wcol"]);
        CsList wval(h->vars["wval"]);
        unsigned int i;
        for ( i = 0; i< wcol.size(); i++)
        {
            if ( wcol[i] == "root")
            {
                (*h->vars.p_getVars())["rootInput.old"] = wval[i];
                break;
            }
        }
    }

    if ( h->vars["rootInput.old"] == "" )
        return;

    schema = h->vars["schema"];
    table = h->vars["table"];
    tab = db->p_getTable(schema, table);

    root = HttpFilesystem::getRoot(h);

    need_root = 1;
    HttpFilesystem::readdir(h);
    for ( is = dirs.begin(); is != dirs.end(); ++is )
    {
        vals["repositoryid"] = "################";
        where["root"] = vals["root"] = h->vars["rootInput.old"];
        where["name"] = vals["name"] = (*is).name;

        r = tab->select(&vals, &where);
         if (r->empty() )
             tab->insert(&vals);

         if ( check_path(root.c_str(), ((*is).name + "/.git").c_str(), 1, 0) == "" )
         {
             CsList cmd;
             int result;

             HttpVars vars = h->vars;

             cmd.add(gitcmd.c_str());
             cmd.add("init");
             cmd.add("--quiet");
             cmd.add("--shared=group");
             cmd.add((*is).name);

             Process p(DbHttpProvider::http->getServersocket());
             p.start(cmd, NULL, root.c_str());
             result = p.wait();

             if ( result != 0 )
                 msg.perror(E_MKREPOSITORY,"Fehler während des Erzeugens der Versionsverwaltung");

             cmd.clear();
             cmd.add(gitcmd.c_str());
             cmd.add("add");
             cmd.add(".");

             p.start(cmd, NULL, ( root + DIRSEP + (*is).name).c_str());
             result = p.wait();

             if ( result != 0 )
             {
                 msg.perror(E_MKREPOSITORY,"Fehler während des Erzeugens der Versionsverwaltung");
                 return;
             }

             (*h->vars.p_getVars())["autocommitInput"] = "1";
             (*h->vars.p_getVars())["commitmessageInput"] = "Initialversion";
             (*h->vars.p_getVars())["nameInput.old"] = (*is).name;

             commit(db, h);
             h->vars = vars;
         }
    }

    db->release(tab);


}

void DbHttpUtilsRepository::data_xml(Database *db, HttpHeader *h)
{
    dbdata_update(db,h);
    query.data_xml(db, h);
}

void DbHttpUtilsRepository::ls(Database *db, HttpHeader *h)
{
    unsigned int i;
    std::string str;
    CsList cmd;
    int result;
    std::string dir;
    int first;
    std::vector<FileData>::iterator is;

    std::string rootname = h->vars["rootname"];
    std::string idname = h->vars["idname"];

    int onlydir = ( h->vars["noleaf"] != "" );
    int singledir = ( h->vars["singledir"] != "" );

    idname = ( idname == "" ) ? "name" : idname;
    rootname = ( rootname == "" ) ? "root" : rootname;

    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());

    readdir(h);
    if ( h->error_found )
    {
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    cmd.add(gitcmd.c_str());
    cmd.add("status");
    cmd.add("-s");

    result = exec(&cmd, getRoot(h).c_str());

    if ( result != 0 )
    {
        msg.perror(E_MKREPOSITORY,"Fehler während des Listens eines Ordners");
        msg.line("%s", execlog.c_str());
        DbHttpProvider::add_content(h,  "error");
        return;
    }

    CsList s(execlog, '\n', 1);
    std::map<std::string, std::string> status;
    for ( i = 0; i<s.size(); ++i)
    {
        std::string name = s[i].substr(3);
        std::string st = s[i].substr(0,2);
        std::string::size_type n;

        if ( (n = name.find(" -> ")) != std::string::npos ) name = name.substr(n + 4);
        if ( name[0] == '"') name = name.substr(1,name.find('"', 1) - 1);

        if ( st == "??" ) st = "Y";
        st.erase( std::remove_if( st.begin(), st.end(), ::isspace ), st.end() );

        status.insert(std::make_pair(name, st));

        dir = name;
        first = 1;
        while ( ( n = dir.rfind('/') ) != std::string::npos )
        {
            name = dir.substr(n + 1);
            dir = dir.substr(0,n);

            if ( status.find(dir) == status.end() )
                status.insert(std::make_pair(dir, "M"));


            for ( is = files.begin(); is != files.end(); ++is) if ( (*is).name == name ) break;
            if ( first && dir == this->dir && is == files.end() && st == "D" )
            {
                FileData d;
                d.name = name;
                files.push_back(d);
            }
            first = 0;
        }
        for ( is = files.begin(); is != files.end(); ++is) if ( (*is).name == name ) break;
        if ( first == 1 && this->dir == "" && is == files.end() && st == "D" )
        {
            FileData d;
            d.name = name;
            files.push_back(d);
        }
    }

    DbHttpProvider::add_content(h, "<head>");

    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "menuid",   "menuid");
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "item",   "item");
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "action", "action");
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "typ",    "typ");
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "pos",    "pos");
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "status",  "status");
    DbHttpProvider::add_content(h, "</head><body>");

    dir = this->dir;
    if ( dir != "" && dir.substr(dir.length() - 2) != DIRSEP )
        dir = dir + DIRSEP;

    i = 0;
    for ( is= dirs.begin(); is != dirs.end(); ++is )
    {
        std::string fullname = dir + (*is).name;
        std::string st =  ((status.find(fullname) != status.end()) ? status.find(fullname)->second : "");
        fullname = ToString::substitute(fullname, DIRSEP, "/");

        if ( singledir )
            DbHttpProvider::add_content(h, "<r><v>%s</v><v>%s</v><v>%s</v><v>%s</v><v>%d</v><v>%s</v></r>", fullname.c_str(), (*is).name.c_str(), ("setValue( \"repositoryid : '" + h->vars["repositoryidInput.old"] + "'," + rootname + " : '" + root + "', " + idname + ": '" + ToString::mascarade(fullname.c_str(),"'") + "', status : '" + st + "'\")").c_str(), "leaf", i++, st.c_str() );
        else
            DbHttpProvider::add_content(h, "<r><v>%s</v><v>%s</v><v>%s</v><v>%s</v><v>%d</v><v>%s</v></r>", fullname.c_str(), (*is).name.c_str(), "submenu", "", i++, st.c_str() );
    }

    for ( is= files.begin(); !onlydir && is != files.end(); ++is )
    {
        std::string fullname = dir + (*is).name;
        std::string st =  ((status.find(fullname) != status.end()) ? status.find(fullname)->second : "");
        fullname = ToString::substitute(fullname, DIRSEP, "/");
        DbHttpProvider::add_content(h, "<r><v>%s</v><v>%s</v><v>%s</v><v>%s</v><v>%d</v><v>%s</v></r>", fullname.c_str(), (*is).name.c_str(), ("setValue( \"repositoryid : '" + h->vars["repositoryidInput.old"] + "'," + rootname + " : '" + root + "'," +  idname + ": '" + ToString::mascarade(fullname.c_str(),"'") + "', status : '" + st + "'\")").c_str(), "leaf", i++, st.c_str() );
    }

    DbHttpProvider::add_content(h, "</body>");
    return;

}
void DbHttpUtilsRepository::log(Database *db, HttpHeader *h)
{
    unsigned int i;
    std::string str;
    CsList cmd;
    int result;

    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());
    if ( h->vars["no_vals"] == "" || h->vars["no_vals"] == "false" )
    {

        if ( getDir(h) == "" )
        {
            DbHttpProvider::add_content(h,  "<body>error</body>");
            return;
        }

        cmd.add(gitcmd.c_str());
        cmd.add("log");
        cmd.add("--pretty=%H@%an@%at@%s");
        cmd.add(path + DIRSEP + h->vars["filenameInput.old"]);

        result = exec(&cmd, getRoot(h).c_str());

        if ( result != 0 )
        {
            msg.perror(E_MKREPOSITORY,"Fehler während des Listens der Änderungsnotizen");
            msg.line("%s", execlog.c_str());
            DbHttpProvider::add_content(h,  "<body>error</body>");
            return;
        }
    }
    else
        execlog = "";

    DbHttpProvider::add_content(h, "<head>");

    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "author", msg.get("geändert von").c_str());
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>1000</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "comitdate", msg.get("geändert am").c_str());
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "notiz",    msg.get("Änderungsnotiz").c_str());
    DbHttpProvider::add_content(h, "<d><id>%s</id><typ>2</typ><format></format><name>%s</name><regexp><reg></reg><help></help><mod></mod></regexp></d>\n", "hash",   "hash");

    DbHttpProvider::add_content(h, "</head><body>");

    if ( h->vars["status"] != "" )
        DbHttpProvider::add_content(h, "<r><v>%s</v><v>%ld</v><v>%s</v><v>%s</v></r>", "",time(NULL), h->vars["status"].c_str(), "" );

    CsList l(execlog, '\n');
    for ( i=0; i<l.size(); ++i )
    {
        if ( l[i] != "" )
        {
            CsList ele(l[i],'@');
            DbHttpProvider::add_content(h, "<r><v>%s</v><v>%s</v><v>%s</v><v>%s</v></r>", ele[1].c_str(),ele[2].c_str(),ele[3].c_str(),ele[0].c_str() );
        }
    }

    DbHttpProvider::add_content(h,  "</body>");
}

void DbHttpUtilsRepository::dblog_update(Database *db, HttpHeader *h)
{
    unsigned int i;
    std::string str;
    CsList cmd;
    int result;
    DbTable *tab;
    DbTable::ValueMap where, vals;
    DbConnect::ResultMat *r;
    std::string lasthash;
    CsList l;

    execlog = "";
    if (( h->vars["no_vals"] == "" || h->vars["no_vals"] == "false" )  )
    {

        if ( getRoot(h) == "" )
        {
            DbHttpProvider::add_content(h,  "<body>error</body>");
            return;
        }

        cmd.add(gitcmd.c_str());

        cmd.add("log");
        cmd.add("--pretty=%H");

        result = exec(&cmd, getRoot(h).c_str());

        if ( result != 0 )
        {
            msg.perror(E_MKREPOSITORY,"Fehler während des Listens der Änderungsnotizen");
            msg.line("%s", execlog.c_str());
            return;
        }
        l.setString(execlog, '\n');
        lasthash = l[0];

        if ( check_path(h, h->vars["filenameInput.old"], 0, 0 ) != "" && S_ISREG(statbuf.st_mode))
        {
            cmd.setString(gitcmd.c_str());

            cmd.add("log");
            //cmd.add("--follow");
            cmd.add("--pretty=%H@%an@%at@%s");
            cmd.add(ToString::substitute(h->vars["filenameInput.old"].c_str(), "/", DIRSEP));

            result = exec(&cmd, getRoot(h).c_str());

            if ( result != 0 )
            {
                msg.perror(E_MKREPOSITORY,"Fehler während des Listens der Änderungsnotizen");
                msg.line("%s", execlog.c_str());
                return;
            }
        }
        else
        {
            return;
        }
    }

    tab = db->p_getTable("mne_repository", "filedata");
    where["repositoryid"] = vals["repositoryid"] = h->vars["repositoryidInput.old"];
    where["filename"] = vals["filename"] = h->vars["filenameInput.old"];
    where["hash"] = vals["hash"] = "";

    vals["repauthor"] = "";
    vals["repdate"] = (long)time(NULL);
    vals["repnote"] = "";
    vals["shortrev"] = h->vars["shortrevInput"];

    tab->del(&where);
    if ( h->vars["status"] != "" )
        tab->insert(&vals);

    where.clear();
    where["repositoryid"] = vals["repositoryid"] = h->vars["repositoryidInput.old"];
    where["filename"] = vals["filename"] = h->vars["filenameInput.old"];

    l.setString (execlog, '\n');
    for ( i=0; i<l.size(); ++i )
    {
        if ( l[i] != "" )
        {
            CsList ele(l[i],'@');
            CsList svals("hash,repnote,shortrev");

            vals["hash"] = ele[0];
            vals["repauthor"] = ele[1];
            vals["repdate"] = ele[2];
            vals["repnote"] = ele[3];
            vals["shortrev"] = "";
            where["repdate"] = ele[2];

            r = tab->select(&svals, &where);
            if (r->empty() )
                tab->insert(&vals);
            else
            {
                std::string hash = (char*)(((*r)[0])[0]);
                std::string repnote = (char*)(((*r)[0])[1]);
                std::string shortrev = (char*)(((*r)[0])[2]);

                if ( i == 0 && ele[0] == lasthash && ele[3] != repnote )
                {
                    CsList ll;

                    cmd.setString(gitcmd.c_str());
                    cmd.add("commit");
                    cmd.add("--amend");
                    cmd.add("-m");
                    cmd.add(repnote.c_str());

                    exec(&cmd, getRoot(h).c_str());
                    if ( result != 0 )
                    {
                        msg.perror(E_MKREPOSITORY,"Fehler während des Korrigierens der Änderungsnotiz");
                        msg.line("%s", execlog.c_str());
                        msg.line("%s", cmd.getString().c_str());
                        return;
                    }

                    cmd.setString(gitcmd.c_str());

                    cmd.add("log");
                    cmd.add("--pretty=%H");

                    result = exec(&cmd, getRoot(h).c_str());

                    if ( result != 0 )
                    {
                        msg.perror(E_MKREPOSITORY,"Fehler während des Listens der Änderungsnotizen");
                        msg.line("%s", execlog.c_str());
                        return;
                    }
                    ll.setString(execlog, '\n');
                    hash = lasthash = ll[0];

                }

                if ( ele[0] != hash )
                {
                    vals["hash"] = hash;
                    vals["repnote"] = repnote;
                    vals["shortrev"] = shortrev;
                    tab->modify(&vals, &where);
                }

            }
        }
    }

    tab->end();
    db->release(tab);
}

void DbHttpUtilsRepository::dblog(Database *db, HttpHeader *h)
{
    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());

    if ( h->vars["repositoryidInput.old"] != "" )
        dblog_update(db, h);

    if ( h->error_found == 0 )
    {
        DbHttpProvider::del_content(h);
        query.data_xml(db, h);
    }
    else
    {
        DbHttpProvider::add_content(h,  "<body>error</body>");
    }

}

void DbHttpUtilsRepository::download(Database *db, HttpHeader *h)
{
    char buffer[10240];
    int rlen;
    int file;
    std::string name;

    Process p(DbHttpProvider::http->getServersocket());

    name = h->vars["filenameInput.old"];
    if ( name.rfind("/") != std::string::npos )
        name = name.substr(name.rfind("/")+ 1);

    h->content_type = "application/octet-stream";
    snprintf(buffer, sizeof(buffer), "Content-Disposition: attachment; filename=\"%s\"", h->vars.url_decode(name.c_str()).c_str());
    buffer[sizeof(buffer) -1] = '\0';
    h->extra_header.push_back(buffer);

    if ( getDir(h) == "" )
    {
        h->status = 404;
        return;
    }

    if ( h->vars["hash"] == "" )
    {
#if defined(__MINGW32__) || defined(__CYGWIN__)
        if ( ( file = open ((path + DIRSEP + h->vars["filenameInput.old"]).c_str(), O_RDONLY )) < 0 )
#else
        if ( ( file = open ((path + DIRSEP + h->vars["filenameInput.old"]).c_str(), O_RDONLY | O_CLOEXEC )) < 0 )
#endif
        {
            DbHttpProvider::add_content(h,  msg.get("Datei <%s> wurde nicht gefunden").c_str(), h->vars["filenameInput.old"].c_str());
            h->status = 404;
            h->content_type = "text/plain";
            return;
        }
        else
        {
            while ( ( rlen = ::read(file, buffer, sizeof(buffer))) > 0 )
                DbHttpProvider::add_contentb(h, buffer, rlen );
            close(file);
        }
    }
    else
    {
        CsList cmd;
        cmd.add(gitcmd.c_str());
        cmd.add("show");
        cmd.add(h->vars["hash"] + ":" + h->vars["filenameInput.old"]);

        h->content[0] = '\0';
        h->content_length = 0;
        p.start(cmd, "pipe", getRoot(h).c_str());
        while ( ( rlen = p.read(buffer, sizeof(buffer))) > 0 )
        {
            if ( rlen > 0 )
                DbHttpProvider::add_contentb(h, buffer, rlen );
            else
                if ( rlen < 0 && errno != EAGAIN ) break;
        }

        p.wait();
        if ( p.getStatus() != 0 )
            h->content_type = "text/plain";
    }
}

void DbHttpUtilsRepository::downall(Database *db, HttpHeader *h)
{
    char buffer[10240];
    int rlen;
    std::string personid;
    std::map<std::string,std::string>::iterator m;

    Process p(DbHttpProvider::http->getServersocket());

    personid = h->vars["personidInput.old"];


    CsList cmd;
    cmd.add("mkdrawings");
    cmd.add("-user");
    cmd.add(personid);
    cmd.add("-repositoryid");
    cmd.add(h->vars["repositoryidInput.old"]);
    for (m = h->datapath.begin(); m != h->datapath.end(); ++m )
    {
        cmd.add("-root");
        cmd.add(m->first);
        cmd.add(h->dataroot + m->second);
    }

    p.start(cmd, "pipe");

    while ( ( rlen = p.read(buffer, sizeof(buffer))) > 0 )
    {
        if ( rlen > 0 )
            DbHttpProvider::add_contentb(h, buffer, rlen );
        else
            if ( rlen < 0 && errno != EAGAIN ) break;
    }

    if ( p.getStatus() == 0 )
    {
        h->content_type = "application/zip";
        snprintf(buffer, sizeof(buffer), "Content-Disposition: attachment; filename=\"%s\"", h->vars.url_decode(( h->vars["name"] + ".zip").c_str()).c_str());
        buffer[sizeof(buffer) -1] = '\0';
        h->extra_header.push_back(buffer);
    }
    else
    {
        h->content_type = "text/plain";
        snprintf(buffer, sizeof(buffer), "Content-Disposition: attachment; filename=\"%s\"", "error.txt");
        buffer[sizeof(buffer) -1] = '\0';
        h->extra_header.push_back(buffer);
    }

}

void DbHttpUtilsRepository::mkdir  ( Database *db, HttpHeader *h)
{
    HttpFilesystem::mkdir(h);
    if ( h->error_found == 0 )
    {
        DbHttpProvider::del_content(h);
        if ( h->vars["filenameInput.old"] == "" )
            (*h->vars.p_getVars())["filenameInput"] = h->vars["filenameInput"] + DIRSEP + ".gitignore";
        else
            (*h->vars.p_getVars())["filenameInput"] = h->vars["filenameInput.old"] + DIRSEP + h->vars["filenameInput"] + DIRSEP ".gitignore";

        std::string name = h->vars["filenameInput"];

#if defined(__MINGW32__) || defined(__CYGWIN__)
        int file = open((path + DIRSEP + name).c_str(), O_WRONLY | O_CREAT, 0666 );
#else
        int file = open((path + DIRSEP + name).c_str(), O_WRONLY | O_CREAT | O_CLOEXEC, 0666 );
#endif
        close(file);
        addfile(db, h);
    }
}

void DbHttpUtilsRepository::rmdir  ( Database *db, HttpHeader *h)
{
    int n = 0;
    DIR *d;

    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());
    if ( getDir(h) == "" )
    {
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    d = opendir(( path + + DIRSEP + h->vars["filenameInput.old"]).c_str() );
    if (d == NULL)
    {
        std::string e = msg.getSystemerror(errno);
        msg.perror(E_DEL, "Fehler während des Löschen eines Ordners");
        msg.line("%s", e.c_str());
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    while (::readdir(d) != NULL)
      if(++n > 3) break;
    closedir(d);

    if (n == 4)
    {
        msg.perror(E_DEL, "Ordner nicht leer");
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    std::string hdir = h->vars["dirInput.old"];
    std::string hname = h->vars["filenameInputfile.old"];

    (*h->vars.p_getVars())["dirInput.old"] = h->vars["dirInput.old"] + DIRSEP + h->vars["filenameInput.old"];
    (*h->vars.p_getVars())["filenameInput.old"] = ".gitignore";
    DbHttpProvider::del_content(h);

    rmfile(db, h);
    if ( h->error_found )
    {
        DbHttpProvider::del_content(h);
        h->error_found = 0;
        h->error_messages.clear();

        (*h->vars.p_getVars())["dirInput.old"] = hdir;
        (*h->vars.p_getVars())["filenameInput.old"] = hname;
        HttpFilesystem::rmdir(h);
    }
}

void DbHttpUtilsRepository::addfile ( Database *db, HttpHeader *h)
{
    CsList cmd;
    int result;

    if ( h->content_type == "text/xml")
        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());

    if ( getDir(h) == "" )
    {
        if ( h->content_type == "text/xml")
            DbHttpProvider::add_content(h, "<body>error</body>");
        return;
    }

    std::string filename = path + DIRSEP + h->vars["filenameInput"];

    cmd.add(gitcmd.c_str());
    cmd.add("add");
    cmd.add(filename.c_str());

    result = exec(&cmd, getRoot(h).c_str());

    if ( result != 0 )
    {
        msg.perror(E_MKREPOSITORY,"Fehler während des Hinzufügen einer Datei");
        msg.line("%s", execlog.c_str());
        if ( h->content_type == "text/xml")
            DbHttpProvider::add_content(h, "<body>error</body>");
        return;
    }

    if ( h->error_found == 0 && h->vars["autocommitInput"] != "" )
    {
        if ( h->vars["commitmessageInput"] == "" )
            (*h->vars.p_getVars())["commitmessageInput"] = msg.get("Neue Version hinzugefügt");
        commit(db, h);
    }

    if ( execlog != "" )
    {
       msg.pwarning(W_COMMIT,"Keine Änderungen gefunden");
       msg.line("%s", execlog.c_str());
    }

    (*h->vars.p_getVars())["dirInput.old"] = "";
    (*h->vars.p_getVars())["filenameInput.old"] = ToString::substitute(filename.substr(getRoot(h).size() + 1), DIRSEP, "/");

    dblog_update(db,h);

    if ( h->content_type == "text/xml")
        DbHttpProvider::add_content(h, "<body>ok</body>");
}

void DbHttpUtilsRepository::mkfile ( Database *db, HttpHeader *h)
{
    HttpFilesystem::mkfile(h);
    if ( msg.getErrorfound())
        return;

    h->content_type = "text/plain";
    addfile(db, h);
    h->content_type = "text/html";
}

void DbHttpUtilsRepository::rmfile ( Database *db, HttpHeader *h)
{
    CsList cmd;
    int result;

    DbTable *tab;
    DbTable::ValueMap where, vals;

    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result>", h->charset.c_str());

    if ( getDir(h) == "" )
    {
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    if ( h->vars["statusInput.old"] == "Y" )
    {
        cmd.add("rm");
        cmd.add("-f");
        cmd.add((path + DIRSEP + h->vars["filenameInput.old"]).c_str());
    }
    else
    {
        cmd.add(gitcmd.c_str());
        cmd.add("rm");
        cmd.add("--force");
        cmd.add("--quiet");
        cmd.add((path + DIRSEP + h->vars["filenameInput.old"]).c_str());
    }

    result = exec(&cmd, getRoot(h).c_str());

    if ( result != 0 )
    {
        msg.perror(E_MKREPOSITORY,"Fehler während des Löschens einer Datei");
        msg.line("%s", execlog.c_str());
        DbHttpProvider::add_content(h,  "<body>error</body>");
        return;
    }

    if ( h->vars["autocommitInput"] != "" )
    {
        if ( h->vars["commitmessageInput"] == "" )
            (*h->vars.p_getVars())["commitmessageInput"] = msg.get("gelöscht");

        commit(db, h);
        if ( execlog != "" )
        {
            msg.pwarning(W_COMMIT,"Keine Änderungen gefunden");
            msg.line("%s", execlog.c_str());
            DbHttpProvider::add_content(h,  "<body>error</body>");
            return;
        }
    }

    tab = db->p_getTable("mne_repository", "filedata");
    where["repositoryid"] = h->vars["repositoryidInput.old"];
    where["filename"] = ToString::substitute((path + "/" + h->vars["filenameInput.old"]).substr(getRoot(h).size() + 1), DIRSEP, "/");

    tab->del(&where,1);
    db->release(tab);

    DbHttpProvider::add_content(h,  "<body>ok</body>");
}

void DbHttpUtilsRepository::mv ( Database *db, HttpHeader *h)
{
    CsList cmd;
    int result;
    DbTable *tab;
    DbTable::ValueMap where, vals;

    std::string oldname = h->vars["filenameInput.old"];
    std::string newname = h->vars["filenameInput"];

    if ( oldname == "" || newname == "" )
    {
        msg.perror(E_NEEDNAME, "Der Name der Datei darf nicht leer sein");
    }

    if ( getDir(h) == "" || oldname == "" || newname == "" )
    {
        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
        return;
    }

    cmd.add(gitcmd.c_str());
    cmd.add("mv");
    cmd.add((path + DIRSEP + oldname));
    cmd.add((path + DIRSEP + newname).c_str());

    result = exec(&cmd, getRoot(h).c_str());

    if ( result != 0 )
    {
        msg.perror(E_MKREPOSITORY,"Fehler während des Umbenenens einer Datei");
        msg.line("%s", execlog.c_str());
        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
        return;
    }

    if ( h->vars["autocommitInput"] != "" )
    {
        if ( h->vars["commitmessageInput"] == "" )
            (*h->vars.p_getVars())["commitmessageInput"] = msg.get("umbenannt von: ") + h->vars["filenameInput.old"];

        commit(db, h);
        if ( execlog != "" )
        {
            msg.pwarning(W_COMMIT,"Keine Änderungen gefunden");
            msg.line("%s", execlog.c_str());
            DbHttpProvider::add_content(h,  "<body>error</body>");
            return;
        }
    }

    where["repositoryid"] = h->vars["repositoryidInput.old"];
    where["filename"] = ToString::substitute((path + "/" + h->vars["filenameInput.old"]).substr(getRoot(h).size() + 1), DIRSEP, "/");
    vals["filename"]  = ToString::substitute((path + "/" + h->vars["filenameInput"]).substr(getRoot(h).size() + 1), DIRSEP, "/");

    tab = db->p_getTable("mne_repository", "filedata");
    tab->modify(&vals, &where, 1);
    db->release(tab);

    tab = db->p_getTable("mne_repository", "fileinterests");
    tab->modify(&vals, &where, 1);
    db->release(tab);

    (*h->vars.p_getVars())["filenameInput.old"] = ToString::substitute((path + "/" + h->vars["filenameInput"]).substr(getRoot(h).size() + 1), DIRSEP, "/");
    dblog_update(db, h);

    DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>ok</body>", h->charset.c_str());
}

void DbHttpUtilsRepository::commit(Database *db, HttpHeader *h)
{
    CsList cmd;
    int result;

    DbHttpAnalyse::Client::Userprefs userprefs = DbHttpProvider::http->getUserprefs();

    cmd.add(gitcmd.c_str());
    cmd.add("commit");
    cmd.add("-a");
    cmd.add("-m");
#if defined(__MINGW32__) || defined(__CYGWIN__)
    cmd.add(ToString::from_utf8((h->vars["commitmessageInput"] == "" ) ? msg.get("Keine Notiz") : h->vars["commitmessageInput"] ));
#else
    cmd.add((h->vars["commitmessageInput"] == "" ) ? msg.get("Keine Notiz") : h->vars["commitmessageInput"] );
#endif
    cmd.add("--author=\"" + userprefs["fullname"] + " <" + userprefs["email"] + ">\"" );

    if ( ( result = exec(&cmd, getRoot(h).c_str())) == 0 ) execlog = "";

    if ( h->vars["autocommitInput"] == "" )
    {
        if ( result != 0 )
        {
            msg.perror(E_COMMIT,"Fehler während des Akzeptierens der Änderungen");
            msg.line("%s", execlog.c_str());
            DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>error</body>", h->charset.c_str());
            return;
        }

        DbHttpProvider::add_content(h,  "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>ok</body>", h->charset.c_str());
    }
}

