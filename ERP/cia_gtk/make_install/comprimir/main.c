#include "becomprimir.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);
	
	gchar *sTemp, *sTemp1;
	gulong lSize = 0, lSize1 = 0, lSize2 = 0;

    lSize2 = strlen ("Tem uma grave limitação de tamanho em be_comprimir_de. Se o tamanho de bytes for superior a capacidade da variável, dá overflow.");
	sTemp = (gchar *) be_comprimir_nivel ("Tem uma grave limitação de tamanho em be_comprimir_de. Se o tamanho de bytes for superior a capacidade da variável, dá overflow.", &lSize, Z_BEST_COMPRESSION);
	
	printf ("Compressão = %s\n", sTemp);
	lSize1 = lSize;
	sTemp1 = sTemp;
	sTemp = (gchar *) be_comprimir_de ((guchar *) sTemp1, &lSize1, lSize2);
	printf ("Decompressão = %s\n", sTemp);
	g_free (sTemp);
	g_free (sTemp1);
	printf ("Porcentagem = %d%% (%d %d %d)\n", (gint) (lSize*100/lSize1), (gint) lSize, (gint) lSize1, (gint) lSize2);
	
	//be_comprimir_arquivo ("erro.txt", "erro1.txt.gz");
	//be_comprimir_arquivo_de ("erro1.txt.gz", "erro1.txt");

	sTemp = (gchar *) be_comprimir_arquivo_string ("erro.txt", &lSize);
	printf ("Arquivo: %d B - %s\n", (gint) lSize, sTemp);

	printf ("\n");
	#ifdef G_OS_UNIX
		system("gzip -V");
	#endif
	#ifdef G_OS_WIN32
		system("gzip.exe -V");
	#endif
	
	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
