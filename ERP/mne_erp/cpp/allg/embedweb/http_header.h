#ifndef http_header_mne
#define http_header_mne

#include <string>
#include <vector>
#include <map>

#include "http_cookie.h"
#include "http_vars.h"

class HttpHeader
{
public:
    enum Browser
    {
        B_UNKNWON,
        B_FF,
        B_MOZILLA,
        B_OPERA,
        B_NS,
        B_SAFARI,
        B_IE
    };

    enum Os
    {
        OS_UNKNWON,
        OS_WIN,
        OS_MAC,
        OS_LINUX,
        OS_IPAD,
        OS_ANDORID
    };

    static const int CONTENT_SIZE = 10240;

    typedef std::map<std::string, std::string> SetCookies;

    std::vector<std::string> serverpath;
    std::map<std::string,std::string> datapath;
    std::string dataroot;

    typedef std::vector<std::string> Header;
    Header rawheader;

    // Wird gesendet
    // =============
    HttpCookie  cookies;
    HttpVars    vars;

    std::string id;

    std::string typ;
    std::string providerpath;
    std::string dirname;
    std::string filename;
    std::string protokoll;

    std::string hostname;
    std::string port;
    std::string referer;
    std::string base;

    Browser     browser;
    Os          os;
    std::string user_agent;

    std::string user;
    std::string passwd;

    std::string post_type;
    char       *post_data;
    int         post_length;
    int         needed_postdata;

    void       *user_data;

    // Vom Provider auszufüllen
    // ========================
    int status;
    int setstatus;
    int translate;
    int age;
    int connection;
    int proxy;

    std::string location;
    std::string realm;
    std::string content_type;
    std::string charset;

    char       *content;
    long        content_maxsize;
    long        content_length;

    SetCookies set_cookies;

    std::vector<std::string> extra_header;

    std::vector<std::string> error_messages;
    std::vector<std::string> error_types;

    int error_found;
    int warning_found;
    int message_found;

    HttpHeader()
    {
        content = new char[CONTENT_SIZE];
        content_maxsize = CONTENT_SIZE;
        post_data = NULL;
        user_data = NULL;
        clear();
    }

    ~HttpHeader()
    {
        if ( content != NULL )   delete[] content;
        if ( post_data != NULL ) delete[] post_data;
        if ( user_data != NULL ) delete (char *)user_data;
    }

    int client;

private:
    void clear()
    {
        client = -1;

        rawheader.clear();

        id = "";
        serverpath.clear();
        datapath.clear();
        typ       = "";
        providerpath = "";
        dirname   = "";
        filename  = "";
        protokoll = "";

        hostname = "";
        port     = "";
        referer  = "";
        base     = "";

        browser = B_UNKNWON;
        user_agent = "";
        os = OS_UNKNWON;

        user   = "";
        passwd = "";

        *content = '\0';
        content_length = 0;
        if ( post_data != NULL ) delete[] post_data;
        if ( user_data != NULL ) delete (char*)user_data;

        post_data = NULL;
        post_length = 0;
        needed_postdata = -1;

        status = 404;
        setstatus = 0;
        translate = 0;
        age = 0;
        connection = 1;
        proxy = 0;

        realm = "";
        location = "";
        content_type   = "text/html";
        charset        = "UTF-8";

        cookies.clear();
        set_cookies.clear();
        vars.clear();

        extra_header.clear();
        error_messages.clear();
        error_types.clear();

        error_found = 0;
        warning_found = 0;
        message_found = 0;

    }


};

#endif /* http_header_mne */
