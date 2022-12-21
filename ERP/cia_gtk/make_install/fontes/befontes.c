#include "befontes.h"

void be_fontes_lista (const gchar *sArquivoFonte, void (*func_Info)(const gchar *sMensagens, gpointer user_data), gpointer user_data)
{
	FILE *pFile = NULL;
	FT_Library FTlib;
	GSList *Fontes = NULL, *FontesL = NULL;
	gchar *sComando = NULL, *sSaida = NULL, *sErro = NULL;
	glong lSaida = 0;
	gchar *sFonte = NULL;
	gchar *sCaminhoFontes = NULL, *sArquivo = NULL;
	gint nCont = 0;
	gchar **Lista = NULL;
/*#ifdef G_OS_UNIX
	const gchar *sArquivoX[2] = {BEFONTES_XF86Config, BEFONTES_Xorg};
	gchar sLinha[BEFONTES_BLOCOS_ARQ];
	gint nPos = 0, nLen = 0, nArquivoX = 0;
#endif*/
#ifdef G_OS_WIN32
	gchar *sTemp = NULL;
#endif
	//iniciar FreeType2
	FT_Init_FreeType (&FTlib);

#ifdef G_OS_UNIX
	//cami	nho específico
	if (g_file_test (BEFONTES_Font1, G_FILE_TEST_IS_DIR))
		sCaminhoFontes = g_strdup (BEFONTES_Font1);
	else if (g_file_test (BEFONTES_Font2, G_FILE_TEST_IS_DIR))
		sCaminhoFontes = g_strdup (BEFONTES_Font2);
	//localizar
	sComando = g_strdup_printf ("%s %s", BEFONTES_COMANDO_LOCALIZAR, sCaminhoFontes);
#endif
#ifdef G_OS_WIN32
	//caminho específico
	if (g_file_test (BEFONTES_Font1, G_FILE_TEST_IS_DIR))
		sCaminhoFontes = g_strdup (BEFONTES_Font1);
	else
		sCaminhoFontes = g_strdup (BEFONTES_Font2);
	//localizar
	sComando = g_strdup_printf ("%s \"%s\"", BEFONTES_COMANDO_LOCALIZAR, sCaminhoFontes);
#endif

	if (g_spawn_command_line_sync (sComando, &sSaida, &sErro, NULL, NULL))
	{
		lSaida = strlen (sSaida);
		if (strlen (sErro) == 0 && lSaida > 0)
		{
#ifdef G_OS_WIN32
			//limpeza específica do Windows
			sTemp = sSaida;
			sSaida = be_strreplace (sTemp, BEFONTES_ENTER, "  ");
			BE_free (sTemp);
			sTemp = sSaida;
			sSaida = be_strreplace (sTemp, "  ", " ");
			BE_free (sTemp);
			sTemp = sSaida;
			sSaida = be_strreplace (sTemp, " ", BEFONTES_ENTER);
			BE_free (sTemp);
#endif
			//lista de arquivos localizados
			Lista = g_strsplit (sSaida, BEFONTES_ENTER, -1);
			for (nCont = 0; Lista[nCont] && lSaida > 0; nCont++)
			{
				lSaida -= strlen (Lista[nCont]);
				if (g_path_is_absolute (Lista[nCont]))
				{
					BE_free (sCaminhoFontes);
					sCaminhoFontes = g_strdup (Lista[nCont]);
					sCaminhoFontes[strlen (sCaminhoFontes) -1] = '\0';
					//printf ("-%s\n", sCaminhoFontes);
				}
				else
				{
					sArquivo = g_build_filename (sCaminhoFontes, Lista[nCont], NULL);
					//printf ("%s\n", sArquivo);
					if (!g_file_test (sArquivo, G_FILE_TEST_IS_DIR))
					{
						sFonte = _be_fontes_extrair (FTlib, sArquivo);					
						if (sFonte)
						{
							//printf ("%s\n", sFonte);
							Fontes = g_slist_append (Fontes, sFonte);
							//chamar callback
							if (func_Info)
								func_Info (sFonte, user_data);
						}
					}
					BE_free (sArquivo);
				}
			}
			BE_strfreev (Lista);
		}
	}
	else
	{
		g_warning ("Comando invalido: %s\n", sComando);
		if (sErro)
			g_warning ("Erro: %s\n", sErro);
	}
	BE_free (sComando);
	BE_free (sSaida);
	BE_free (sErro);
	BE_free (sCaminhoFontes);
/*#ifdef G_OS_UNIX
	//procurar dentro do arquivo X
	for (nArquivoX = 0; nArquivoX < 2; nArquivoX++)
	{
		if ((pFile = fopen (sArquivoX[nArquivoX], "r")))
		{
			while (fgets (sLinha, BEFONTES_BLOCOS_ARQ, pFile))
			{
				if ((nPos = be_strpos (sLinha, BEFONTES_FontPath, 0)) >= 0)
				{
					//limpando
					nLen = strlen (sLinha);
					if (sLinha[nLen - 1] == '\n' || sLinha[nLen - 1] == '\r')//retirando o enter final
						sLinha[nLen - 1] = '\0';
					for (nCont = nPos; nCont < (nPos + strlen (BEFONTES_FontPath)); nCont++)
						sLinha[nCont] = ' ';
					//novo caminho
					sCaminhoFontes = g_strdup (g_strstrip (g_strdelimit (sLinha, "\"#", ' ')));
					//localizar
					sComando = g_strdup_printf ("%s %s", BEFONTES_COMANDO_LOCALIZAR, sCaminhoFontes);
					printf ("%s\n", sComando);
					if (g_spawn_command_line_sync (sComando, &sSaida, &sErro, NULL, NULL))
					{
						lSaida = strlen (sSaida);
						if (strlen (sErro) == 0 && lSaida > 0)
						{
							//printf ("%s\n", sSaida);
							//lista de arquivos localizados
							Lista = g_strsplit (sSaida, BEFONTES_ENTER, -1);
							for (nCont = 0; Lista[nCont] && lSaida > 0; nCont++)
							{
								lSaida -= strlen (Lista[nCont]);
								if (g_path_is_absolute (Lista[nCont]))
								{
									BE_free (sCaminhoFontes);
									sCaminhoFontes = g_strdup (Lista[nCont]);
									sCaminhoFontes[strlen (sCaminhoFontes) -1] = '\0';
									//printf ("-%s\n", sCaminhoFontes);
								}
								else
								{
									sArquivo = g_build_filename (sCaminhoFontes, Lista[nCont], NULL);
									if (!g_file_test (sArquivo, G_FILE_TEST_IS_DIR))
									{
										//printf ("%s\n", sArquivo);
										sFonte = _be_fontes_extrair (FTlib, sArquivo);
										if (sFonte)
										{
											Fontes = g_slist_append (Fontes, sFonte);
											//chamar callback
											if (func_Info)
												func_Info (sFonte, user_data);
										}
									}
									BE_free (sArquivo);
								}
							}
							BE_strfreev (Lista);
						}
					}
					else
					{
						g_warning ("Comando invalido: %s\n", sComando);
					}
					BE_free (sComando);
					BE_free (sSaida);
					BE_free (sErro);
					BE_free (sCaminhoFontes);
				}
			}
			fclose(pFile);
		}
	}
#endif*/
	//ordenar
	Fontes = g_slist_sort (Fontes, be_string_ordenar_string);
	if ((pFile = fopen (sArquivoFonte, "wb")))
	{
		for (FontesL = Fontes, nCont = 0; FontesL; FontesL = g_slist_next (FontesL), nCont++)
		{
			sFonte = FontesL->data;
			fprintf (pFile, "%s %d%s", sFonte, nCont + 1, BEFONTES_ENTER);
			//printf ("%d %s\n", nCont, sFonte);
			BE_free (sFonte);//limpando
		}
		fclose(pFile);
	}
	else
	{
		g_warning ("Arquivo invalido: %s\n", sArquivoFonte);
	}

	//limpando
	BE_slist_free (Fontes);
	//fechar
	FT_Done_FreeType (FTlib);
}

gchar *_be_fontes_extrair (FT_Library FTlib, const gchar *sArquivo)
{
	FT_Face FTface;
	FT_Error FTerror;
	gchar *sFonte = NULL;
	gchar *sNome = NULL, *sEstilo = NULL;

	FTerror = FT_New_Face (FTlib, sArquivo, 0, &FTface);
	if (!FTerror)
	{
		if (FTface->face_flags & FT_FACE_FLAG_SCALABLE)
		{
			
			sNome = g_utf8_strdown (FTface->family_name, -1);
			sEstilo = g_utf8_strdown (FTface->style_name, -1);
			if ((FTface->style_flags & FT_STYLE_FLAG_BOLD) && (FTface->style_flags & FT_STYLE_FLAG_ITALIC))
				sFonte = g_strdup_printf ("%s  %d (%s negrito italico negrito) \"%s\"", sNome, (gint) FTface->style_flags, sEstilo, sArquivo);
			else if (FTface->style_flags & FT_STYLE_FLAG_ITALIC)
				sFonte = g_strdup_printf ("%s  %d (%s italico) \"%s\"", sNome, (gint) FTface->style_flags, sEstilo, sArquivo);
			else if (FTface->style_flags & FT_STYLE_FLAG_BOLD)
				sFonte = g_strdup_printf ("%s  %d (%s negrito) \"%s\"", sNome, (gint) FTface->style_flags, sEstilo, sArquivo);
			else
				sFonte = g_strdup_printf ("%s  %d (%s normal) \"%s\"", sNome, (gint) FTface->style_flags, sEstilo, sArquivo);
			BE_free (sEstilo);
			BE_free (sNome);
		}
		FT_Done_Face (FTface);
	}

	return sFonte;
}

BEFONTES *be_fontes_carregar (const gchar *sArquivoFonte, const gchar *sNome, const gchar *sEstilo)
{
	//realiza tentativas de carregar a fonte
	BEFONTES *Fonte = NULL;

	//primeira tentativa
	Fonte = _be_fontes_carregar (sArquivoFonte, sNome, sEstilo);
	//segunda tentativa, utiliza fonte padrão
	if (!Fonte)
		Fonte = _be_fontes_carregar (sArquivoFonte, BEFONTES_PADRAO, sEstilo);
	//terceira tentativa, utiliza a primeira fonte
	if (!Fonte)
		Fonte = _be_fontes_carregar (sArquivoFonte, NULL, sEstilo);
	
	return Fonte;
}

BEFONTES *_be_fontes_carregar (const gchar *sArquivoFonte, const gchar *sNome, const gchar *sEstilo)
{
	FT_Face FTface;
	FT_Error FTerror;
	FT_Library FTlib;
	FT_Glyph FTglyph;
	FT_BBox FTbbox;
	FILE *pFile = NULL;
	gchar sLinha[BEFONTES_BLOCOS_ARQ];
	gchar *sNome_m = NULL, *sEstilo_m = NULL;
	gchar *sEstiloExistente = NULL;
	gchar *sArquivo = NULL;
	gchar *sTemp = NULL;
	gint nCont = 0, nIndex = 0;
	BEFONTES *Fonte = NULL;

	//converter para minúsculas
	if (sNome)
	{
		sTemp = g_utf8_strdown (sNome, -1);
		sNome_m = g_strdup_printf ("%s  ", sTemp);
		BE_free (sTemp);
	}
	if (sEstilo)
		sEstilo_m = g_utf8_strdown (sEstilo, -1);
	if ((pFile = fopen (sArquivoFonte, "rb")))
	{
		while ((fgets (sLinha, BEFONTES_BLOCOS_ARQ, pFile)))
		{
			if (!sNome_m || sLinha[0] == sNome_m[0])
			{
				if (!sNome_m || sLinha[1] == sNome_m[1])
				{
					if (!sNome_m || g_strrstr_len (sLinha, strlen (sNome_m), sNome_m))
					{
						BE_free (sEstiloExistente);
						sEstiloExistente = be_strextract (sLinha, "(", ")", 0);
						if (sEstilo_m)
						{
							if (g_strrstr (sEstiloExistente, sEstilo_m))
							{
								sArquivo = be_strextract (sLinha, "\"", "\"", 0);
								break;
							}
						}
						else
						{
							sArquivo = be_strextract (sLinha, "\"", "\"", 0);
							break;
						}
					}
				}
			}
		}
		fclose(pFile);
	}
	else
	{
		g_warning ("Arquivo invalido: %s\n", sArquivoFonte);
	}
	//abrir arquivo valido
	if (sArquivo)
	{
    	if (g_file_test (sArquivo, G_FILE_TEST_EXISTS))
    	{
    		//iniciar FreeType2
    		FT_Init_FreeType (&FTlib);
    		//abrir fonte
    		FTerror = FT_New_Face (FTlib, sArquivo, 0, &FTface);
    		if (!FTerror)
    		{
				Fonte = g_new (BEFONTES, 1);
				Fonte->sNome = (guchar *) g_strdup (FTface->family_name);
				Fonte->sNomeInterno = (guchar *) g_strdup (FT_Get_Postscript_Name (FTface));
				Fonte->sEstilo = (guchar *) g_strdup (FTface->style_name);
				if (sEstiloExistente)
					Fonte->sEstiloUsual = g_strdup (sEstiloExistente);
				else
					Fonte->sEstiloUsual = NULL;
				Fonte->sArquivo = g_strdup (sArquivo);
				Fonte->nCaixa[0] = FTface->bbox.xMin;
				Fonte->nCaixa[1] = FTface->bbox.xMax;
				Fonte->nCaixa[2] = FTface->bbox.yMin;
				Fonte->nCaixa[3] = FTface->bbox.yMax;
				Fonte->nLarguraMax = FTface->max_advance_width;
				Fonte->nAlturaMax = FTface->max_advance_height;
				Fonte->nUnidade = FTface->units_per_EM;
				Fonte->nEstiloFlags = FTface->style_flags;
				Fonte->nFlags = FTface->face_flags;
				Fonte->nAscent = FTface->ascender;
				Fonte->nDescent = FTface->descender;
				Fonte->nObjeto = 0;
    			//letras
    			for (nCont = 32; nCont < 256; nCont++)
    			{
    				nIndex = FT_Get_Char_Index(FTface, nCont);
    				if (nIndex > 0)
    				{
    					FTerror = FT_Load_Glyph (FTface, nIndex, FT_LOAD_NO_SCALE);
    					FTerror = FT_Get_Glyph (FTface->glyph, &FTglyph);
    					FT_Glyph_Get_CBox(FTglyph, FT_GLYPH_BBOX_UNSCALED, &FTbbox);
    					Fonte->nLetra[nCont - 32][0] = FTbbox.xMin;
    					Fonte->nLetra[nCont - 32][1] = FTbbox.xMax;
    					Fonte->nLetra[nCont - 32][2] = FTbbox.yMin;
    					Fonte->nLetra[nCont - 32][3] = FTbbox.yMax;
    					Fonte->nLetra[nCont - 32][4] = FTface->glyph->advance.x;
    					//fechar
    					FT_Done_Glyph (FTglyph);
    				}
    				else
    				{
    					Fonte->nLetra[nCont - 32][0] = -1;
    					Fonte->nLetra[nCont - 32][1] = -1;
    					Fonte->nLetra[nCont - 32][2] = -1;
    					Fonte->nLetra[nCont - 32][3] = -1;
    					Fonte->nLetra[nCont - 32][4] = -1;
    				}
    			}
    			//fechar
    			FT_Done_Face (FTface);
    		}
    		//fechar
    		FT_Done_FreeType (FTlib);
    	}
	}
	BE_free (sArquivo);
	BE_free (sEstilo_m);
	BE_free (sEstiloExistente);
	BE_free (sNome_m);

	return Fonte;
}

gint be_fontes_largura (BEFONTES *Fonte, const gchar *sTexto)
{
	gint nLargura = 0;
	gint nCont = 0;

	for (nCont = 0; nCont < strlen (sTexto); nCont++)
		nLargura += Fonte->nLetra[((guchar) sTexto[nCont]) - 32][4];

	return nLargura;
}

void be_fontes_altura (BEFONTES *Fonte, const gchar *sTexto, gint *yMin, gint *yMax)
{
	gint nCont = 0;
	guchar cChar = 0;

	for (nCont = 0; nCont < strlen (sTexto); nCont++)
	{
		cChar = (guchar) sTexto[nCont];
		if (Fonte->nLetra[cChar - 32][2] < *yMin)
			*yMin = Fonte->nLetra[cChar - 32][2];
		if (Fonte->nLetra[cChar - 32][3] > *yMax)
			*yMax = Fonte->nLetra[cChar - 32][3];
	}
}

void be_fontes_descarregar (BEFONTES *Fonte)
{
	if (Fonte)
	{
		BE_free (Fonte->sNome);
		BE_free (Fonte->sNomeInterno);
		BE_free (Fonte->sEstilo);
		BE_free (Fonte->sEstiloUsual);
		BE_free (Fonte->sArquivo);
		BE_free (Fonte);
	}
}
