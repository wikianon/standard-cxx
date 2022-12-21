#include "befontes.h"

int main(int argc, char *argv[])
{
	//inicializar biblioteca estendida
	be_global_abrir (&argc, &argv, 0);

	BEFONTES *Fonte = NULL;
	gint nCont = 0;
	gint yMin = 0, yMax = 0;

	#ifdef G_OS_UNIX
		//preparar lista de fontes
		//be_fontes_lista ("suporte/fontes", NULL, NULL);
		//extrair informações de uma fonte
		//Fonte = be_fontes_carregar ("suporte/fontes", "bitstream vera sans", "italico");
		Fonte = be_fontes_carregar ("suporte/fontes", "bitstream charter", "negrito");
	#endif
	#ifdef G_OS_WIN32
		//preparar lista de fontes
		be_fontes_lista ("suporte/fontes.txt", NULL, NULL);
		//extrair informações de uma fonte
		//Fonte = be_fontes_carregar ("suporte/fontes.txt", "bitstream vera sans", "italico");
		//Fonte = be_fontes_carregar ("suporte/fontes.txt", "arial", "italico");
		Fonte = be_fontes_carregar ("suporte/fontes.txt", NULL, "normal");//utiliza a primeira fonte "normal"
	#endif

	if (Fonte)
	{
		printf ("%s %s (%s) %d\n%s\n", Fonte->sNome, Fonte->sEstilo, Fonte->sEstiloUsual, Fonte->nUnidade, Fonte->sArquivo);
		printf ("Maximas %d x %d\n", Fonte->nLarguraMax, Fonte->nAlturaMax);
		printf ("Caixa x=(%d %d) y=(%d %d)\n", Fonte->nCaixa[0], Fonte->nCaixa[1], Fonte->nCaixa[2], Fonte->nCaixa[3]);
		for (nCont = 0; nCont < 224; nCont++)
			if (Fonte->nLetra[nCont][4] >= 0)
				printf ("%c : x=(%d %d) y=(%d %d) L=%d\n", nCont + 32, Fonte->nLetra[nCont][0], Fonte->nLetra[nCont][1], Fonte->nLetra[nCont][2], Fonte->nLetra[nCont][3], Fonte->nLetra[nCont][4]);

		printf ("Largura: %.3f pt\n", (gfloat) be_fontes_largura (Fonte, "Testando alguma coisa") / (gfloat) Fonte->nUnidade * 18.0);
		be_fontes_altura (Fonte, "Testando aluma coisa", &yMin, &yMax);
		printf ("Altura: %.3f pt\n", (gfloat) (yMax - yMin) / (gfloat) Fonte->nUnidade * 18.0);
	}
	be_fontes_descarregar (Fonte);

    /*PangoFontMap *M = NULL;
    PangoFontFamily **F = NULL;
    PangoFontFace **Face;
    PangoFontDescription *Des = NULL;
    PangoRectangle Rec, Rec1;
    PangoLayoutLine *Line = NULL;

	//pango_font_get_metrics

	gint N = 0, A;
	gint nCont1 = 0;
	gint X = 0, Y = 0;

	M = pango_ft2_font_map_new ();
	pango_font_map_list_families (M, &F, &N);

	printf ("%d\n", N);
	for (nCont = 0; nCont < N; nCont++)
	{
		printf ("%s\n", pango_font_family_get_name (F[nCont]));
		pango_font_family_list_faces (F[nCont], &Face, &A);
		printf ("\t%d\n", A);
  		for (nCont1 = 0; nCont1 < A; nCont1++)
  		{
            printf ("\t%s\n", pango_font_face_get_face_name (Face[nCont1]));

		}
		//g_free (Face);
	}
	Des = pango_font_face_describe (Face[2]);
	printf ("Familia %s\n", pango_font_description_get_family (Des));
	printf ("Estilo %d\n", pango_font_description_get_style (Des));
	printf ("String %s\n", pango_font_description_to_string (Des));

	pango_font_description_free (Des);

	g_free (F);
	//g_object_unref (M);

	PangoLayout *Lay = NULL;
	PangoContext *Con = NULL;

	//Con = pango_win32_get_context ();
	//Con = gtk_widget_get_pango_context
	Con = pango_ft2_font_map_create_context ((PangoFT2FontMap  *) M);
	//Lay = gtk_widget_create_pango_layout
	Lay = pango_layout_new (Con);
	pango_layout_set_markup (Lay, "<span foreground='blue' size='10'>Blue text</span> \n os <i>cooo</i>.", -1);
	pango_layout_set_wrap (Lay, PANGO_WRAP_WORD);


	printf ("%d\n\n", pango_layout_get_line_count (Lay));
	pango_layout_get_pixel_size (Lay, &X, &Y);
	printf ("%d %d\n", X, Y);
	pango_layout_get_size (Lay, &X, &Y);
	printf ("(%d %d)/%d\n", X, Y, PANGO_SCALE);
	printf ("%d %d\n", pango_layout_get_spacing (Lay), pango_layout_get_width (Lay));
	printf ("%s\n", pango_layout_get_text (Lay));
	pango_layout_get_pixel_extents (Lay, &Rec, &Rec1);
	printf ("%d %d %d %d\n", Rec.x, Rec.y, Rec.width, Rec.height);
	printf ("%d %d %d %d\n", Rec1.x, Rec1.y, Rec1.width, Rec1.height);

	Line = pango_layout_get_line (Lay, 0);
	printf ("\n%d %d %d %d\n", Line->start_index, Line->length, Line->is_paragraph_start, Line->resolved_dir);
	pango_layout_line_get_pixel_extents (Line, &Rec, &Rec1);
	printf ("%d %d %d %d\n", Rec.x, Rec.y, Rec.width, Rec.height);//depende das letras
	printf ("%d %d %d %d\n", Rec1.x, Rec1.y, Rec1.width, Rec1.height);//bloco fixo das letras

	Line = pango_layout_get_line (Lay, 1);
	printf ("\n%d %d %d %d\n", Line->start_index, Line->length, Line->is_paragraph_start, Line->resolved_dir);
	pango_layout_line_get_pixel_extents (Line, &Rec, &Rec1);
	printf ("%d %d %d %d\n", Rec.x, Rec.y, Rec.width, Rec.height);//depende das letras
	printf ("%d %d %d %d\n", Rec1.x, Rec1.y, Rec1.width, Rec1.height);//bloco fixo das letras
	*/

	//finalizar biblioteca extentida
	be_global_fechar ();
	return 0;
}
