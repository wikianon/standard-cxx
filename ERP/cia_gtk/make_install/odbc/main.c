#include "beodbc.h"

gboolean executar_callback (const BEODBC *ODBC, gint nCampos, gint nLinha, GSList *Campos, gint nTipo, gint nComp, GSList *Valores, gpointer user_data)
{
	GSList *ListaL = NULL, *ListaL1 = NULL;
	gchar *sBuffer = NULL;

    printf ("Linha: %d - Colunas: %d\n", nLinha, nCampos);
    //loop nas campos e valores
    for (ListaL = Campos, ListaL1 = Valores; ListaL; ListaL = g_slist_next (ListaL), ListaL1 = g_slist_next (ListaL1))
    {
        //exibir campo
        sBuffer = (gchar *) ListaL->data;
        printf ("%s = ", sBuffer);
        //exibir valor
        sBuffer = (gchar *) ListaL1->data;
        printf ("%s - (%d) - %d", sBuffer, nTipo, nComp);
        //próxmia linha
        printf ("\n");
    }

    return TRUE;//FALSE para parar o loop.
}

int main (int argc, char *argv[])
{
	//inicializar biblioteca extendida
	be_global_abrir (&argc, &argv, 0);

	BEODBC *ODBC = NULL;
	//BEODBC_Sintaxe *Sintaxe = NULL;
	gchar *sErros = NULL;

	//ODBC = be_odbc_abrir_conexao ("Driver={SQL Server};Server=sfferra003;Uid=rede;Pwd=pierre3000;", "%", NULL, NULL, NULL, &sErros);
	ODBC = be_odbc_abrir_conexao ("Driver={SQL Server};Server=sfferra003;Uid=rede;Pwd=pierre3000;", "DO", "dbo", "RECLAMACAO", NULL, NULL);
    //ODBC = be_odbc_abrir_conexao ("Driver={Oracle ODBC Driver};DBQ=sg_lpd;Uid=vag;Pwd=vag;DBA=R;MTS=T", NULL, NULL, NULL, NULL, NULL);
    //ODBC = be_odbc_abrir_conexao ("Driver={Oracle ODBC Driver};DBQ=sg_lpd;Uid=vag;Pwd=vag;DBA=R;MTS=T", NULL, "USU_EXT", "ORDENSDESERVIGOLP", NULL, NULL);//especificando o banco de dados e a tabela; lembrando que o Oracle não utiliza catalogo
    //ODBC = be_odbc_abrir_conexao ("Driver={MySQL Connector/ODBC v5};Server=localhost;Uid=root;Pwd=prodpp;", "%", NULL, "%", NULL, NULL);
    //ODBC = be_odbc_abrir_conexao ("Driver={MySQL Connector/ODBC v5};Server=localhost;Uid=root;Pwd=prodpp;", "cia_demo", NULL, "cli_insumos", NULL, NULL);//especificando o banco de dados e a tabela; lembrando que o MySQL não utiliza schema
    if (ODBC)
    {
        printf ("Conectado\n");
        //Sintaxe = be_odbc_abrir_consulta (be_odbc_selecionar_tabela (ODBC, "ORDENSDESERVIGOLP", NULL, "USU_EXT"), FALSE, NULL);//Oracle
        //Sintaxe = be_odbc_abrir_consulta (be_odbc_selecionar_tabela (ODBC, "cli_insumos", "cia_demo", NULL), FALSE, NULL);//MySQL
        //Sintaxe = be_odbc_abrir_consulta (be_odbc_selecionar_tabela (ODBC, "RECLAMACAO", "DO", "dbo"), FALSE, NULL);//MSSQL
        //if (Sintaxe)
        //    printf ("Tabela atual = %s\n", Sintaxe->Tabela->sNome);
		//BE_odbc_fechar_consulta (Sintaxe);

		//conexão direta
		//be_odbc_executar (ODBC, "SELECT * FROM `cia_demo`.`cli_insumos`;", executar_callback, NULL, NULL);
		be_odbc_executar (ODBC, "SELECT * FROM [DO].[dbo].[RECLAMACAO] WHERE BA=644550;", executar_callback, NULL, NULL);
        //be_odbc_executar (ODBC, "SELECT * FROM USU_EXT.ORDENSDESERVIGOLP WHERE LO_NOCSO = '103239910';", executar_callback, NULL, NULL);
    }
    BE_odbc_fechar_conexao (ODBC);

	if (sErros)
		printf ("%s\n", sErros);
	BE_free (sErros);


	//finalizar biblioteca extendida
	be_global_fechar ();
	return 0;
}
