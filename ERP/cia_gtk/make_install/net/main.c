#include "benet.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extentida
	be_global_abrir (&argc, &argv, 0);
	BENET *Site = NULL;

	Site = be_net_iniciar ("http://www.tgestiona.com.br/ManutPredial/login.aspx", "proxy.telesp.com.br:8080", "TELEFONICASP\\e158738:654cxz*", "Mozilla/5.0 (Windows; U; Windows NT 5.0; pt-BR; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1");
	if (be_net_receber (Site))
	{
		//localizar campos e alterar
		be_net_campo (Site, "Login1:txtUsuario", "pm_estacao");
		be_net_campo (Site, "Login1:txtSenha", "pm_estacao");
		//be_net_campo (Site, "Login1:chkSalvar", "TRUE");
		be_net_botao (Site, "Login1:btnOK", TRUE);

		if (be_net_receber (Site))
		{
			BE_free (Site->sURL);
			Site->sURL = "http://www.tgestiona.com.br/ManutPredial/default.aspx?opcao=1&Us=PM_ESTACAO";//alterar site

			if (be_net_receber (Site))
			{
				//localizar campos e alterar
				be_net_campo (Site, "_ctl0:txtNumOrdem", "5495492");
				be_net_botao (Site, "_ctl0:cmdSelecionar", TRUE);

				if (be_net_receber (Site))
					printf ("%s\n%s\n%s\n", Site->sURL, Site->sCabecalho, Site->sConteudo);//exibir conteúdo
			}
		}
	}

	//download
	/*if (be_net_downloadCURL ("banana.wav", "http://www.yourdictionary.com/ahd/pron/B0052000.wav", "proxy.telesp.com.br:8080", "TELEFONICASP\\e158738:987dsa&", "Mozilla/5.0 (Windows; U; Windows NT 5.0; pt-BR; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1", NULL))
		printf ("OK\n");
	else
		printf ("Nao baixou o arquivo.\n");*/

	BE_net_finalizar (Site);

	be_global_fechar ();
	return 0;
}
