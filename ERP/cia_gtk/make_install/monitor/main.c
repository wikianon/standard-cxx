#include "bemonitor.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);
	
	#ifdef G_OS_UNIX
		be_monitor ("saida", NULL, NULL);
	#endif
	#ifdef G_OS_WIN32
		be_monitor ("saida.txt", NULL, NULL);
	#endif
	
	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
