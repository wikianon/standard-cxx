#ifndef dbhttp_utils_connect_mne
#define dbhttp_utils_connect_mne

#include <map>
#include <string>

#include <embedweb/http_provider.h>

#include "dbhttp_provider.h"

class DbHttpUtilsConnect : public DbHttpProvider
{
    Message msg;

    enum ERROR_TYPE
    {

        E_TYPE = 100

    };

    typedef void (DbHttpUtilsConnect::*SubProvider)(Database *db,HttpHeader *h);
    typedef std::map<std::string, SubProvider> SubProviderMap;
    SubProviderMap subprovider;

    void end          (Database *db, HttpHeader *h);
    void start        (Database *db, HttpHeader *h);

    void func_execute_xml     (Database *db, HttpHeader *h);
    void func_mod_xml         (Database *db, HttpHeader *h);
    void func_del_xml         (Database *db, HttpHeader *h);

    void sql_execute_xml      (Database *db, HttpHeader *h);

    void reload               (Database *db, HttpHeader *h);

public:
    DbHttpUtilsConnect( DbHttp *h );
    virtual ~DbHttpUtilsConnect();

    virtual std::string getPath() { return "db/utils/connect"; }

    virtual int request (Database *db, HttpHeader *h);

};

#endif /* dbhttp_utils_connect_mne */
