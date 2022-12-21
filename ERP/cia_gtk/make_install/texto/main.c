#include "betexto.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca extendida
	be_global_abrir (&argc, &argv, 0);
	
	BETEXTO *Texto = NULL;
	GNode *No = NULL;
	#ifdef G_OS_UNIX
		const gchar *sArquivoFonte = "../fontes/suporte/fontes";
	#endif
	#ifdef G_OS_WIN32
		const gchar *sArquivoFonte = "../fontes/suporte/fontes.txt";
	#endif
	
	const gchar *sExpressao = {
			"OK útil <font indice='1'>aas</font>asdaskdjahkajshddfjfdhfg123456789qwertyu iopasdfghjklzxcvbnm56665dfdsdfsdfsdfklsjdhflkjshflks 877 78<font color='CC3301' bgcolor='0033FF' face='courier new' size='20' style='normal'>Eu quero. tição<font size='8'>Ainda sim.</font>Please !</font>KO "
			"<space largura='30' altura='18'/>"
			"<sub>sub-escrito.</sub>"
			"<sup>sobre-escrito.</sup>"
			"<br> ou."
			"<b>negrito.</b>"
			"<i>italico.<b>negrito e italico ao mesmo tempo</b></i>"
			"<u>sublinhado.</u>"
			"a\n"
			"Vamos aumentar bastante este texto merreca. Muito curto!"};
	//const gchar *sExpressao2 = {"Vamos aumentar bastante este. <i>Muito curto!</i><space largura='100'/>Quero aumentar agora, <font face='courier new' style='normal' color='000000'>sem perder</font> muito tempo. OK ?"};
	Texto = be_texto_abrir (sArquivoFonte, NULL, "italico", 15.0);//uilizando a primeira fonte
	if (Texto)
	{
		//texto 1
		No = be_texto_analisar (Texto, sExpressao, 200.0, 100.0, TRUE, FALSE);
		if (No)
			g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);//exibir
		printf ("\n");
		//texto 2
		/*No = be_texto_analisar (Texto, sExpressao2, 400.0, 100.0, TRUE, FALSE);
		if (No)
			g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);//exibir*/
	}
	BE_texto_fechar (Texto);
	
	//finalizar biblioteca extendida
	be_global_fechar ();
	return 0;
}
