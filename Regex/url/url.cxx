#include <regex>
#include <iostream>

struct url
{
 url(std::string url): _url(url){}
 
 ~url()
  {
   if(!_url.empty())_url.clear();
  }
  
  bool isUrl()
  {
   const std::regex pattern(
                            "(?:(ftp|http[s]?:[//])?)?([w]{3}[.])?"
                            "(.*[.](com|php|net|org|br|dk|at|us|tv|info|uk|co.uk|biz|se)?)?"
                            "(.*[.](aspx|htm|html|HTM|HTML|jhtm|jhtml|JHTM|JHTML|xhtm|xhtml|XHTM|XHTML)?)?"
                           );
   
   return std::regex_match(_url, pattern);
  }
  
  void print()
  {
   std::cout << "\n\tUrl: "+_url+(isUrl()?" is Valid\n":" is Invalid\n");
  }
  
  private:
   std::string _url;
};

int main()
{
  const std::string urls[11] = {
                                "http://docs.microsoft.com/pt-br/sql/reporting-services/tools/url-examples-for-items-on-a-report-server-sharepoint-mode/",
                                "https://www.uolhost.uol.com.br/faq/v2/loja-virtual/o-que-e-um-endereco-de-url.php",
                                "https://www.uolhost.uol.com.br/index.asp",
                                "www.uolhost.uol.com.br/INDEX.htm",
                                "ftp://ftp.uolhost.uol.com.br/index.html",
                                "ftps://uolhost.uol.com.br/downloads/source",
                                "ftp.uolhost.uol.com.br/downloads/source/",
                                "ftp.uolhost.uol.com.br/downloads/source/index.jhtml",
                                "ftp.uolhost.uol.com.br/downloads/source/index.xhtml",
                                "ftp.uolhost.uol.com.br/downloads/source/index.aspx",
                                "ftp.uolhost.uol.com.br/downloads/source/index.XHTML"
                               };
  
  for(int i=0; i<11; i++)
   url(urls[i]).print();
    std::cout << "\n";
  return 0;  
}