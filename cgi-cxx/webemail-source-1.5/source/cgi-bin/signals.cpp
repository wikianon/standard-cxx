#include <signal.h>
#include "webemail.h"

void handlesignals()
{
	signal(SIGHUP, SIG_IGN);  //IGNOR HANG UP
	signal(SIGINT, SIG_IGN);  //  "   Keyboard int
	signal(SIGABRT, SIG_IGN); //  "   Abort
	signal(SIGPIPE, SIG_IGN); //  "   broken pipe

	//signal(SIGILL, SIG_IGN):  //  "   Illegal instruct
	//signal(SIGFPE, SIG_IGN);  //  "   Foating point error
	//signal(SIGKILL, SIG_IGN); //  "   Kill
	//signal(SIGSEGV, SIG_IGN); //  "   Segment fault
	//signal(SIGALRM, SIG_IGN); //  "   timer alarm
	//signal(SIGTERM, SIG_IGN); //  "   termination
	return;
}
