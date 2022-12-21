#ifdef PTHREAD
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iconv.h>
#include <errno.h>

#if defined(__MINGW32__) || defined(__CYGWIN__)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#ifdef MACOS
#include <locale.h>
#endif

#include <argument/argument.h>
#include <utils/tostring.h>

#include "http_utils.h"

HttpUtils::HttpUtils(Http *h) :
	HttpProvider(h), msg("HttpUtils")
{

		subprovider["count.html"] = &HttpUtils::count;
		subprovider["uhr.html"] = &HttpUtils::uhr;
        subprovider["time.html"] = &HttpUtils::time;
        subprovider["logout.xml"] = &HttpUtils::logout;
        subprovider["file.html"] = &HttpUtils::file;
        subprovider["file.txt"] = &HttpUtils::file;
        subprovider["locale.xml"] = &HttpUtils::locale;
        subprovider["proxy.txt"] = &HttpUtils::proxy;

		if ( h != NULL )
		    h->add_provider(this);
}

HttpUtils::~HttpUtils()
{
}

int HttpUtils::request(HttpHeader *h)
{

	SubProviderMap::iterator i;

	if ( (i = subprovider.find(h->filename)) != subprovider.end() )
	{
		(this->*(i->second))(h);
		return 1;
	}

	return 0;

}

void HttpUtils::mk_window(HttpHeader *h, char *str)
{

	h->status = 200;
	h->content_type = "text/html";

	add_content(h,  "<!DOCTYPE HTML PUBLIC \"-");
	add_content(h,  "//W3C//DTD HTML 4.01 Transitional//DE\">");
	add_content(h,  "<html>");
	add_content(h,  "<head>");
	add_content(h,  "<title>Nelson technische Informatik - count</title>");
	add_content(h,  "<meta http-equiv=\"Content-Type\" ");
	add_content(h,  "content=\"text/html; ");
	add_content(h,  "charset=iso-8859-1\">");
	add_content(h,  "<link id=\"MainStyleLink\" rel=\"stylesheet\" ");
	add_content(h,  "type=\"text/css\">");
	add_content(h,  "<script language=\"JavaScript\" ");
	add_content(h,  "type=\"text/JavaScript\">");
	add_content(h,  "window.onload = function() ");
	add_content(h,  "{ ");
	add_content(h,  "top.mneMain.mneMisc.setStyle(document, ");
	add_content(h,  "\"MainStyleLink\");");
	add_content(h,  "var d = new Date(); ");
	add_content(h,  "document.getElementById('uhr').firstChild.data = ");
	add_content(h,  " top.mneMain.mneMisc.addNull(d.getHours(),2) + ':' ");
	add_content(h,  " + top.mneMain.mneMisc.addNull(d.getMinutes(),2); } ");
	add_content(h,  "</script>\n");
	add_content(h,  "</head>\r\n");
	add_content(h,  "<body class=\"MneCount\">\r\n");
	add_content(h,  "<div id=\"uhr\" class=\"MneCount\">%s</div>\r\n", str);
	add_content(h,  "</body>");
}

void HttpUtils::count(HttpHeader *h)
{
	mk_window(h, (char *)h->vars["count"].c_str());
}

void HttpUtils::uhr(HttpHeader *h)
{
	char buffer[32];
	time_t t;

	::time(&t);
	strftime(buffer, sizeof(buffer), "%H:%M", localtime(&t));
	mk_window(h, buffer);
}

void HttpUtils::time(HttpHeader *h)
{
	h->content_type = "text/plain";
	h->status = 200;
	add_content(h,  "%d", ((int)::time(NULL)));
}

void HttpUtils::logout(HttpHeader *h)
{
    Argument a;
    char str[1024];

    h->status = 200;
    h->content_type = "text/xml";

    snprintf(str, sizeof(str), "MneHttpSessionId%d", (int)a["port"]);
    str[sizeof(str) - 1] = '\0';
    h->set_cookies[str] = "Logout";
    add_content(h, "<?xml version=\"1.0\" encoding=\"%s\"?><result><body>logout</body>",h->charset.c_str());
}

void HttpUtils::file(HttpHeader *h)
{
    std::string endtag;
    iconv_t iv;
    char *inbuf, *outbuf, *ci, *co;
    size_t innum,outnum;
    std::string str;

    h->status = 200;
    if ( h->filename == "file.html")
    {
        h->content_type = "text/html";
    }
    else
    {
        h->content_type = "text/plain";
    }

    str = h->vars["data"];
    if ( str.substr(0,10) == "##########" )
        str = h->vars.data("data", 1).c_str();

    if ( h->vars["iconv"] != "" )
    {
        ci = inbuf = (char *)str.c_str();
        innum = str.length();

        co = outbuf = new char[str.size() * 4];
        outnum = ( str.size() * 4 - 1);

        if ( ( iv = iconv_open("utf-8//TRANSLIT", h->vars["iconv"].c_str()) ) != (iconv_t)(-1))
        {
            iconv (iv, &ci, &innum, &co, &outnum);
            iconv_close(iv);

            *co = '\0';
            str = outbuf;
            delete[] outbuf;
        }
    }

    if ( h->vars["script"] != "" )
    {
        add_content(h, "<script type=\"text/javascript\">\n");
        add_content(h, "<!--\n");
        add_content(h, "%s\n", h->vars["script"].c_str());
        add_content(h, "//-->\n");
        add_content(h, "</script>\n");
        if ( h->content_type == "text/plain")
        {
            h->content_type = "text/html";
            if ( h->vars["data"].substr(0,10) == "##########" )
                add_content(h,  "<textarea id=\"data\" >%s</textarea>", str.c_str());
            else
                add_content(h,  "<textarea id=\"data\" >%s</textarea>", ToString::mkhtml(str.c_str()).c_str());
            return;
        }
    }
    add_content(h, "%s%s",h->vars["data"].c_str(), endtag.c_str());
}

void HttpUtils::locale(HttpHeader *h)
{
    h->status = 200;
    h->content_type = "text/xml";

    struct lconv *l;
    l = localeconv();

    add_content(h, "<?xml version=\"1.0\" encoding=\"%s\"?><result><head><d><id>decimal_point</id><typ>2</typ><name>decimal_point</name></d><d><id>thousands_sep</id><typ>2</typ><name>thousands_sep</name></d></head><body>",h->charset.c_str());
    add_content(h,  "<r><v>%s</v><v>%s</v></r></body>",l->decimal_point, l->thousands_sep);

    return;
}

void HttpUtils::proxy(HttpHeader *h)
{
    char tmp[10240];
#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(Darwin)
    struct hostent *hp;
#else
    struct hostent hostbuf, *hp;
    int herr;
#endif
    struct sockaddr_in s_in;
    int port = 80;
    int sock;
    std::string request;

    h->content_type = "text/plain";
    h->translate = 0;
    h->proxy = 1;

    if (h->vars["port"] != "" ) port = atoi(h->vars["port"].c_str());

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        msg.perror(E_OPEN, "Konnte Socket nicht öffnen");
        return;
    }

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(Darwin)
    hp = gethostbyname(h->vars["host"].c_str());
#else
    gethostbyname_r(h->vars["host"].c_str(), &hostbuf, tmp, sizeof(tmp), &hp, &herr);
#endif
    if (hp == NULL)
    {
        msg.perror(E_HOST, "Unbekannter Server <%s>", h->vars["host"].c_str());
        close( sock);
        sock = -1;
        return;
    }

    memset((char*) &s_in, 0, sizeof(s_in));
    memcpy(&s_in.sin_addr, hp->h_addr, hp->h_length);
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(port);

    if (::connect(sock, (sockaddr*) &s_in, sizeof(struct sockaddr_in)) < 0)
    {
        msg.perror(E_CONNECT, "kann mich nicht mit server <%s:%d> verbinden", h->vars["host"].c_str(), port);
        close( sock);
        sock = -1;
        return;
    }

    h->status = 200;
    h->translate = 0;

    HttpHeader::Header::iterator i;

    request  = "GET " + h->vars["get"] + " HTTP/1.1\r\n";
    request += "Host: " + h->vars["host"] + "\r\n";
    for ( i=h->rawheader.begin(),++i,++i; i != h->rawheader.end(); ++i)
        if ( ! ( i->find("Host:") == 0 )  && ! ( i->find("Content-Length:") == 0 ))
        request += (*i) + "\r\n";
    request += "\r\n";

    send(sock, request.c_str(), request.length(), 0);

    int l;
    while( ( l = recv(sock, tmp, sizeof(tmp), 0)) > 0 )
    {
        add_contentb(h, tmp, l);
    }

    close(sock);
    return;
}
