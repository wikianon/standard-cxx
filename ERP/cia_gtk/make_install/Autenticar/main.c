#include "beAutenticar.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);

	BESQL *SQL = NULL;
	GKeyFile *Ini = NULL;
	gchar *sTemp = NULL, *sIP = NULL;
	guint nPorta = 0;

	//carregar configuração
	Ini = g_key_file_new ();
	if (g_key_file_load_from_file (Ini, "conexao.ini", G_KEY_FILE_NONE, NULL))
	{
		sIP = g_key_file_get_string (Ini, "servidor", "ip", NULL);
		if (!sIP || strlen (sIP) == 0)
		{
			BE_free (sIP);
		}
		sTemp = g_key_file_get_string (Ini, "servidor", "porta", NULL);
		nPorta = atoi (sTemp);
	}
	g_key_file_free (Ini);

	//login
	SQL = be_Autenticar (NULL, sIP, nPorta, -1, FALSE, NULL, NULL, NULL, NULL, NULL);
	if (SQL)
	{
		printf ("OK - Conectado\n");
	}
	BE_sql_fechar_conexao (SQL);

	BE_free (sTemp);
	BE_free (sIP);

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
