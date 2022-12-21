#include "berelatorio.h"

/*
Estrutura baseada no REC-SVG11-20030114.pdf
*/

gboolean be_relatorio_gerar (
	const gchar *sArquivoSVG,
	const gchar *sArquivoSaida,
	const gchar *sArquivoFonte,
	BESQL_Campo *Campo,
	const gchar *sRelatorio,
	const BEGLOBAL_Empresa_Cliente *EmpresaCliente,
	BESQL_Sintaxe *Sintaxe,
	gchar **sErros
)
{
	GSList *ListaDados = NULL, *ListaL = NULL;
	BESQL_Filtro *Filtrar = NULL;
	gint nPaginaDados = 1;
	glong lRegistros = 0;
	glong lLinhas = 0;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sExtensao = NULL, *sArquivo = NULL;
	gboolean bOK = FALSE;

	//veriricar campo, utilizado para completar nomes de arquivos
	if (Campo)
	{
		//loop dos dados, para gerar uma lista
		Sintaxe->lLinhasPaginaMax = BERELATORIO_MAX_LINHAS_POR_PAGINA;//alterar a quantidade exibida
		do
		{			
			lLinhas = be_sql_selecionar (Sintaxe, nPaginaDados, sErros);
			while (lLinhas > 0 && be_sql_selecionar_proxima_linha (Sintaxe))
			{
				if (Campo->sValor && !be_sql_campo_binario (Campo))
					ListaDados = g_slist_append (ListaDados, be_sql_campo_valor_limitado (Campo, BERELATORIO_LENSTRING_FILTRO));

				//contar linhas de registros
				lRegistros++;
			}
			nPaginaDados++;
		} while (lLinhas > 0 && lLinhas >= BERELATORIO_MAX_LINHAS_POR_PAGINA);
		//loop da lista, para filtrar
		if (!sErros || (sErros && !(*sErros)))
		{
			for (ListaL = ListaDados; ListaL; ListaL = g_slist_next (ListaL))
			{
				BE_free (sTemp);
				sTemp = (gchar *) ListaL->data;
				//preparar nome do arquivo
				sExtensao = be_arquivo_extensao (sArquivoSaida);
				if (sExtensao)
				{
					BE_free (sTemp1);
					BE_free (sArquivo);
					sTemp1 = g_strdup (sArquivoSaida);
					sTemp1[strlen (sTemp1) - strlen (sExtensao) - 1] = '\0';
					sArquivo = g_strdup_printf ("%s-%s.%s", sTemp1, sTemp, sExtensao);
				}
				else
				{
					BE_free (sArquivo);
					sArquivo = g_strdup_printf ("%s%s", sArquivoSaida, sTemp);
				}
				//aplicar filtro
				if (be_sql_campo_texto (Campo))
				{
					BE_free (sTemp1);
					sTemp1 = sTemp;
					sTemp = g_strdup_printf ("%s", sTemp1);
					Filtrar = be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_PARECIDO, sTemp, BESQL_RELACAO_E, 0, TRUE);
				}
				else
					Filtrar = be_sql_filtro_adicionar (Sintaxe, Campo, BESQL_COMPARACAO_IGUAL, sTemp, BESQL_RELACAO_E, 0, TRUE);
				//gerar relatório
#ifdef BERELATORIO_DEPURAR
				printf ("%s %s\n", sTemp, sArquivo);
#endif
				bOK = _be_relatorio_gerar (sArquivoSVG, sArquivo, sArquivoFonte, sRelatorio, EmpresaCliente, Sintaxe, sErros);
				//parar se não der certo
				if (!bOK)
					break;

				//excluir filtro
				BE_sql_filtro_excluir (Sintaxe, Filtrar, TRUE);
			}
			BE_slist_free (ListaDados);
		}
		BE_sql_filtro_excluir (Sintaxe, Filtrar, TRUE);//excluir filtro
		BE_free (sTemp);
		BE_free (sTemp1);
		BE_free (sExtensao);
		BE_free (sArquivo);

		return bOK;
	}
	else
	{
		bOK = _be_relatorio_gerar (sArquivoSVG, sArquivoSaida, sArquivoFonte, sRelatorio, EmpresaCliente, Sintaxe, sErros);

		return bOK;
	}
}

gboolean _be_relatorio_gerar (
	const gchar *sArquivoSVG,
	const gchar *sArquivoSaida,
	const gchar *sArquivoFonte,
	const gchar *sRelatorio,
	const BEGLOBAL_Empresa_Cliente *EmpresaCliente,
	BESQL_Sintaxe *Sintaxe,
	gchar **sErros
)
{
	BEPDF *PDF = NULL;
	BESQL_Campo *Campo = NULL, *Campo1 = NULL;
	BEPDF_Celula *Celula = NULL, *CelulaAnt = NULL;
	BEPDF_Pagina *Pagina = NULL;
	BEXML *XML = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	_BERELATORIO *Relatorio = NULL;
	_BERELATORIO_Dados *Dados = NULL, *DadosAnt = NULL;
	_BERELATORIO_SubLista *SubLista = NULL;
	BESQL_Sintaxe *SubSintaxe = NULL;
	GdkPixbuf *Imagem = NULL;
	GNode *No = NULL, *NoFilho = NULL, *NoDados = NULL, *NoForms = NULL, *NoListas = NULL, *NoRotulos = NULL, *NoIndices = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL, *ListaL2 = NULL;
	GSList *LinhaInicio = NULL;
	gchar *sTemp = NULL;
	const gchar *sValor = NULL;
	gchar **aLista = NULL;
	gint nCont = 0, nCont1 = 0;
	gfloat fX = 0.0, fY = 0.0, fAlturaLinha = 0.0, fLinhaInicial = 0.0;
	gint nPagina = 1, nPaginas = 1, nPaginaDados = 1, nComprimentoMax = 0;
	glong lRegistros = 0;
	gboolean bFundo = FALSE;
	glong lLinhas = 0;

	//capturar informações
	XML = be_xml_abrir (sArquivoSVG, TRUE);
	if (XML)
	{
		//carregando XML numa árvore
		No = be_xml_no_carregar (XML, FALSE);
#ifdef BERELATORIO_DEPURAR
		g_node_traverse (No, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_xml_no_loop, NULL);
#endif
		//iniciar arquivo pdf
		PDF = be_pdf_novo (sArquivoFonte, BEFONTES_PADRAO, NULL, 10.0);
		if (PDF)
		{
			//criar objeto
			Relatorio = g_new (_BERELATORIO, 1);
			Relatorio->PDF = PDF;
			Relatorio->Sintaxe = Sintaxe;
			Relatorio->SubSintaxe = NULL;
			Relatorio->pyModulo = NULL;
			Relatorio->pyScript = NULL;
			Relatorio->lLinha = 0;
			Relatorio->Campo = NULL;
			Relatorio->nPagina = 1;
			Relatorio->nPaginas = 0;

			Relatorio->sRelatorio = sRelatorio;
			Relatorio->sSubRelatorio = NULL;
			Relatorio->EmpresaCliente = EmpresaCliente;
			Relatorio->sArquivoSVG = sArquivoSVG;

			Relatorio->Dados = NULL;
			Relatorio->fXMinDados = 0.0;
			Relatorio->fYMinDados = 0.0;
			Relatorio->fLarguraMaxDados = 0.0;
			Relatorio->fAlturaMaxDados = 0.0;

			Relatorio->Paginas = NULL;

			Relatorio->Formularios = NULL;
			Relatorio->fXMinForms = 0.0;
			Relatorio->fYMinForms = 0.0;
			Relatorio->fLarguraMaxForms = 0.0;
			Relatorio->fAlturaMaxForms = 0.0;

			Relatorio->Rotulos = NULL;
			Relatorio->Listas = NULL;
			Relatorio->Indices = NULL;
			Relatorio->fXMinListas = 0.0;
			Relatorio->fYMinListas = 0.0;
			Relatorio->fLarguraMaxListas = 0.0;
			Relatorio->fAlturaMaxListas = 0.0;

			Relatorio->SubListas = NULL;

			Relatorio->fYRelativo = 0.0;
			Relatorio->DadosAtual = NULL;
			Relatorio->fXMinAtual = 0.0;
			Relatorio->fYMinAtual = 0.0;

			Relatorio->Imagens = NULL;
			//preparando python
			Relatorio->pyModulo = PyImport_AddModule ("__main__");
			if (Relatorio->pyModulo)
				Relatorio->pyModulo = PyModule_GetDict (Relatorio->pyModulo);

			//iniciar página padrão
			BE_pdf_pagina_padrao (PDF, sRelatorio, COMPANY_NAME, BERELATORIO_VERSAO);
			Pagina = PDF->Pagina;
			//dimensões da página
			NoElemento = (BEXML_No_Elemento *) No->data;
			//largura
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
			if (sValor)
				Pagina->fLargura = _be_relatorio_conversao (sValor);
			//altura
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "height");
			if (sValor)
				Pagina->fAltura = _be_relatorio_conversao (sValor);
			//somente página válida
			if (Pagina->fLargura > 0.0 && Pagina->fAltura > 0.0)
			{
				//localizar limites de dados
				NoFilho = be_xml_no_localizar_elemento (No, "rect", "id", "DADOS");
				if (NoFilho)
				{
					NoElemento = (BEXML_No_Elemento *) NoFilho->data;
#ifndef BERELATORIO_DEPURAR
					//alterar nome, para não aparecer
					BE_free (NoElemento->sNome);
					NoElemento->sNome = g_strdup ("rect1");
#endif
					//posições
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
					Relatorio->fXMinDados = _be_relatorio_conversao (sValor);
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
					Relatorio->fYMinDados = _be_relatorio_conversao (sValor);
					//dimensões
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
					Relatorio->fLarguraMaxDados = _be_relatorio_conversao (sValor);
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "height");
					Relatorio->fAlturaMaxDados = _be_relatorio_conversao (sValor);
				}
				//verificar limites
				if (Relatorio->fLarguraMaxDados <= 0.0)
					Relatorio->fLarguraMaxDados = Pagina->fLargura;
				if (Relatorio->fAlturaMaxDados <= 0.0)
					Relatorio->fAlturaMaxDados = Pagina->fAltura;

				//localizar limites de formulários
				NoFilho = be_xml_no_localizar_elemento (No, "rect", "id", "FORMULARIOS");
				if (NoFilho)
				{
					NoElemento = (BEXML_No_Elemento *) NoFilho->data;
#ifndef BERELATORIO_DEPURAR
					//alterar nome, para não aparecer
					BE_free (NoElemento->sNome);
					NoElemento->sNome = g_strdup ("rect1");
#endif
					//posições
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
					Relatorio->fXMinForms = _be_relatorio_conversao (sValor);
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
					Relatorio->fYMinForms = _be_relatorio_conversao (sValor);
					//dimensões
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
					Relatorio->fLarguraMaxForms = _be_relatorio_conversao (sValor);
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "height");
					Relatorio->fAlturaMaxForms = _be_relatorio_conversao (sValor);
				}
				//verificar limites
				if (Relatorio->fLarguraMaxForms <= 0.0)
					Relatorio->fLarguraMaxForms = Pagina->fLargura;
				if (Relatorio->fAlturaMaxForms <= 0.0)
					Relatorio->fAlturaMaxForms = Pagina->fAltura;

				//localizar limites de listas
				NoFilho = be_xml_no_localizar_elemento (No, "rect", "id", "LISTAS");
				if (NoFilho)
				{
					NoElemento = (BEXML_No_Elemento *) NoFilho->data;
#ifndef BERELATORIO_DEPURAR
					//alterar nome, para não aparecer
					BE_free (NoElemento->sNome);
					NoElemento->sNome = g_strdup ("rect1");
#endif
					//posições
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
					Relatorio->fXMinListas = _be_relatorio_conversao (sValor);
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
					Relatorio->fYMinListas = _be_relatorio_conversao (sValor);
					//dimensões
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
					Relatorio->fLarguraMaxListas = _be_relatorio_conversao (sValor);
					sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "height");
					Relatorio->fAlturaMaxListas = _be_relatorio_conversao (sValor);
				}
				//verificar limites
				if (Relatorio->fLarguraMaxListas <= 0.0)
					Relatorio->fLarguraMaxListas = Pagina->fLargura;
				if (Relatorio->fAlturaMaxListas <= 0.0)
					Relatorio->fAlturaMaxListas = Pagina->fAltura;
				//fundo
				NoFilho = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "FUNDO", BEXML_LOCALIZAR_IRMAO);
				if (NoFilho)
				{
					Relatorio->nLocal = BEPDF_LOC_FUNDO;
					Relatorio->fYRelativo = 0.0;
					Relatorio->DadosAtual = NULL;
					g_node_traverse (NoFilho, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
				}
				//localizar objetos de cada seção
				NoDados = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "DADOS", BEXML_LOCALIZAR_IRMAO);
				NoForms = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "FORMULARIOS", BEXML_LOCALIZAR_IRMAO);
				NoListas = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "LISTAS", BEXML_LOCALIZAR_IRMAO);
				NoRotulos = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "ROTULOS", BEXML_LOCALIZAR_IRMAO);
				NoIndices = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "INDICES", BEXML_LOCALIZAR_IRMAO);

				//loop nos dados
				if (Sintaxe)
				{
					lRegistros = 0;
					Sintaxe->lLinhasPaginaMax = BERELATORIO_MAX_LINHAS_POR_PAGINA;
					be_sql_comprimento_linhas (Sintaxe, sErros);
					be_sql_totalizar_linhas (Sintaxe, sErros);
					do
					{
						//loop nas linhas de dados
						lLinhas = be_sql_selecionar (Sintaxe, nPaginaDados, sErros);
						while (lLinhas > 0 && (Relatorio->lLinha = be_sql_selecionar_proxima_linha (Sintaxe)))
						{
							//contar linhas de registros
							lRegistros++;

							//calcular o comprimento total de cada campo, para listas
							if (lRegistros == 1)
							{
								//calcular o valor máximo de comprimento dos dados
								for (ListaL = Sintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
								{
									Campo = (BESQL_Campo *) ListaL->data;
									if (Campo->bAtivo)
									{
										//limites para uma exibição aceitável
										if (Campo->nComprimentoTop < 7)
											Campo->nComprimentoTop = 7;
										else if (Campo->nComprimentoTop > 30)
											Campo->nComprimentoTop = 30;
										//calcular total
										nComprimentoMax += Campo->nComprimentoTop;
										nCont++;
									}
								}
							}
							//criar dados
							if (NoDados)
							{
								Dados = g_new (_BERELATORIO_Dados, 1);
								Dados->Celulas = NULL;
								Dados->fLargura = 0.0;
								Dados->fAltura = 0.0;
								Dados->lLinha = Relatorio->lLinha;
								Dados->Campo = NULL;
								//anexar
								Relatorio->Dados = g_slist_append (Relatorio->Dados, Dados);
								//criar células
								Relatorio->Campo = NULL;
								Relatorio->nLocal = BEPDF_LOC_CORPO;
								Relatorio->fYRelativo = 0.0;
								Relatorio->DadosAtual = Dados;
								Relatorio->fXMinAtual = Relatorio->fXMinDados;
								Relatorio->fYMinAtual = Relatorio->fYMinDados;
								g_node_traverse (NoDados, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
								//nao tentar mais se não houver células
								if (g_slist_length (Dados->Celulas) == 0)
									NoDados = NULL;
							}
							//criar formulários
							if (NoForms)
							{
								//colunas
								for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
								{
									Campo = (BESQL_Campo *) ListaL->data;
									if (Campo->bAtivo)
									{
										//criar formulários
										Dados = g_new (_BERELATORIO_Dados, 1);
										Dados->Celulas = NULL;
										Dados->fLargura = 0.0;
										Dados->fAltura = 0.0;
										Dados->lLinha = Relatorio->lLinha;
										Dados->Campo = Campo;
										//anexar
										Relatorio->Formularios = g_slist_append (Relatorio->Formularios, Dados);
										//criar células
										Relatorio->Campo = Campo;
										Relatorio->nLocal = BERELATORIO_LOC_FORMULARIOS;
										Relatorio->fYRelativo = 0.0;
										Relatorio->DadosAtual = Dados;
										Relatorio->fXMinAtual = Relatorio->fXMinForms;
										Relatorio->fYMinAtual = Relatorio->fYMinForms;
										g_node_traverse (NoForms, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
										//nao tentar mais se não houver células
										if (g_slist_length (Dados->Celulas) == 0)
										{
											NoForms = NULL;
											break;
										}
									}
								}
							}
							//criar listas
							if (NoListas)
							{
								//colunas
								for (ListaL = Sintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
								{
									Campo = (BESQL_Campo *) ListaL->data;
									if (Campo->bAtivo)
									{
										//criar formulários
										Dados = g_new (_BERELATORIO_Dados, 1);
										Dados->Celulas = NULL;
										Dados->fLargura = 0.0;
										Dados->fAltura = 0.0;
										Dados->lLinha = Relatorio->lLinha;
										Dados->Campo = Campo;
										//anexar
										Relatorio->Listas = g_slist_append (Relatorio->Listas, Dados);
										//criar células
										Relatorio->Campo = Campo;
										Relatorio->fLarguraProporcional = (gfloat) Campo->nComprimentoTop / (gfloat) nComprimentoMax;
										Relatorio->nLocal = BERELATORIO_LOC_LISTAS;
										Relatorio->fYRelativo = 0.0;
										Relatorio->DadosAtual = Dados;
										Relatorio->fXMinAtual = Relatorio->fXMinListas;
										Relatorio->fYMinAtual = Relatorio->fYMinListas;
										g_node_traverse (NoListas, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
										//nao tentar mais se não houver células
										if (g_slist_length (Dados->Celulas) == 0)
										{
											NoListas = NULL;
											break;
										}
										nCont++;
									}
								}
							}
							//criar rótulos, somente uma vez
							if (NoRotulos && lRegistros == 1)
							{
								//colunas
								for (ListaL = Sintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
								{
									Campo = (BESQL_Campo *) ListaL->data;
									if (Campo->bAtivo)
									{
										//criar formulários
										Dados = g_new (_BERELATORIO_Dados, 1);
										Dados->Celulas = NULL;
										Dados->fLargura = 0.0;
										Dados->fAltura = 0.0;
										Dados->lLinha = Relatorio->lLinha;
										Dados->Campo = Campo;
										//anexar
										Relatorio->Rotulos = g_slist_append (Relatorio->Rotulos, Dados);
										//criar células
										Relatorio->Campo = Campo;
										Relatorio->fLarguraProporcional = (gfloat) Campo->nComprimentoTop / (gfloat) nComprimentoMax;
										Relatorio->nLocal = BERELATORIO_LOC_ROTULOS;
										Relatorio->fYRelativo = 0.0;
										Relatorio->DadosAtual = Dados;
										Relatorio->fXMinAtual = Relatorio->fXMinListas;
										Relatorio->fYMinAtual = Relatorio->fYMinListas;
										g_node_traverse (NoRotulos, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
										//não tentar mais se não houver células
										if (g_slist_length (Dados->Celulas) == 0)
										{
											NoRotulos = NULL;
											break;
										}
										nCont++;
									}
								}
							}
							//criar índices, vertical
							if (NoIndices)
							{
								Dados = g_new (_BERELATORIO_Dados, 1);
								Dados->Celulas = NULL;
								Dados->fLargura = 0.0;
								Dados->fAltura = 0.0;
								Dados->lLinha = Relatorio->lLinha;
								Dados->Campo = NULL;
								//anexar
								Relatorio->Indices = g_slist_append (Relatorio->Indices, Dados);
								//criar células
								Relatorio->Campo = NULL;
								Relatorio->nLocal = BERELATORIO_LOC_INDICES;
								Relatorio->fYRelativo = 0.0;
								Relatorio->DadosAtual = Dados;
								Relatorio->fXMinAtual = Relatorio->fXMinListas;
								Relatorio->fYMinAtual = Relatorio->fYMinListas;
								g_node_traverse (NoIndices, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
								//nao tentar mais se não houver células
								if (g_slist_length (Dados->Celulas) == 0)
									NoIndices = NULL;
							}
							//criar sublistas: loop nos campos
							for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
							{
								Campo = (BESQL_Campo *) ListaL->data;
								if (Campo->sValor && Campo->bAtivo && Campo->nTipo == BESQL_TIPO_SUB && Campo->sParametros)
								{
									//lista de parâmetros
									BE_strfreev (aLista);
									aLista = g_strsplit (Campo->sParametros, "; ", -1);
										/*
										aLista[0] = `vis_Tabela2`
										aLista[1] = `ID2`
										*/
									//preparar nome da tabela
									BE_free (sTemp);
									sTemp = g_strdup (aLista[0]+1);
									sTemp[strlen (sTemp)-1] = '\0';
									//abrir consulta relacionada
									SubSintaxe = be_sql_abrir_consulta (be_sql_selecionar_tabela (Sintaxe->Tabela->BD, sTemp), TRUE, sErros);
									if (SubSintaxe  && (SubSintaxe->Tabela->nFlags & BESQL_ACESSO_SELECIONAR))
									{
										//preparar nome do campo
										BE_free (sTemp);
										sTemp = g_strdup (aLista[1]+1);
										sTemp[strlen (sTemp)-1] = '\0';
										//preparar filtro atual
										Campo1 = be_sql_selecionar_campo (SubSintaxe, sTemp);
										if (Campo1)
										{
											Campo1->bAtivo = FALSE;
											be_sql_filtro_adicionar (SubSintaxe, Campo1, BESQL_COMPARACAO_IGUAL, Campo->sValor, BESQL_RELACAO_E, 0, FALSE);
										}
										//nome do subrelatório
										if (Campo->sGrupo)
											Relatorio->sSubRelatorio = Campo->sGrupo;
										else if (SubSintaxe->Tabela->sNomeUsual)
											Relatorio->sSubRelatorio = SubSintaxe->Tabela->sNomeUsual;
										else if (SubSintaxe->Tabela->sNomeReal)
											Relatorio->sSubRelatorio = SubSintaxe->Tabela->sNomeReal;
										else
											Relatorio->sSubRelatorio = SubSintaxe->Tabela->sNome;
										//sublista
										SubLista = _be_subrelatorio_gerar (Relatorio, SubSintaxe, Relatorio->sSubRelatorio, No, lRegistros, Campo, sErros);
										//anexar na lista
										if (SubLista)
											Relatorio->SubListas = g_slist_append (Relatorio->SubListas, SubLista);
										//limpar nome do subrelatório
										Relatorio->sSubRelatorio = NULL;
									}
									//fechar consulta
									BE_sql_fechar_consulta (SubSintaxe);
								}
							}
						}
						nPaginaDados++;
					} while (lLinhas > 0 && lLinhas >= BERELATORIO_MAX_LINHAS_POR_PAGINA);
					//marcar fim das linhas
					Relatorio->lLinha = -1;

					//criar subtotais para listas
					if (NoListas)
					{
						//colunas
						for (ListaL = Sintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
						{
							Campo = (BESQL_Campo *) ListaL->data;
							if (Campo->bAtivo)
							{
								//criar lista
								Dados = g_new (_BERELATORIO_Dados, 1);
								Dados->Celulas = NULL;
								Dados->fLargura = 0.0;
								Dados->fAltura = 0.0;
								Dados->lLinha = Relatorio->lLinha;
								Dados->Campo = Campo;
								//anexar
								Relatorio->Listas = g_slist_append (Relatorio->Listas, Dados);
								//criar células
								Relatorio->Campo = Campo;
								Relatorio->fLarguraProporcional = (gfloat) Campo->nComprimentoTop / (gfloat) nComprimentoMax;
								Relatorio->nLocal = BERELATORIO_LOC_LISTAS;
								Relatorio->fYRelativo = 0.0;
								Relatorio->DadosAtual = Dados;
								Relatorio->fXMinAtual = Relatorio->fXMinListas;
								Relatorio->fYMinAtual = Relatorio->fYMinListas;
								g_node_traverse (NoListas, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
								//nao tentar mais se não houver células
								if (g_slist_length (Dados->Celulas) == 0)
								{
									NoListas = NULL;
									break;
								}
								nCont++;
							}
						}
					}
					//criar índices, vertical
					if (NoIndices && NoListas)
					{
						Dados = g_new (_BERELATORIO_Dados, 1);
						Dados->Celulas = NULL;
						Dados->fLargura = 0.0;
						Dados->fAltura = 0.0;
						Dados->lLinha = Relatorio->lLinha;
						Dados->Campo = NULL;
						//anexar
						Relatorio->Indices = g_slist_append (Relatorio->Indices, Dados);
						//criar células
						Relatorio->Campo = NULL;
						Relatorio->nLocal = BERELATORIO_LOC_INDICES;
						Relatorio->fYRelativo = 0.0;
						Relatorio->DadosAtual = Dados;
						Relatorio->fXMinAtual = Relatorio->fXMinListas;
						Relatorio->fYMinAtual = Relatorio->fYMinListas;
						g_node_traverse (NoIndices, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
						//nao tentar mais se não houver células
						if (g_slist_length (Dados->Celulas) == 0)
							NoIndices = NULL;
					}
					//desmarcar fim das linhas
					Relatorio->lLinha = 0;

					//posicionamentos
					if (!sErros || (sErros && !(*sErros)))
					{
						//posição das células, loop dos dados
						if (NoDados)
						{
							fX = 0.0;
							fY = 0.0;
							fAlturaLinha = 0.0;
							nPagina = 1;
							for (ListaL = Relatorio->Dados; ListaL; ListaL = g_slist_next (ListaL))
							{
								Dados = (_BERELATORIO_Dados *) ListaL->data;
								//mover para próxima linha
								if (Dados->fLargura + fX > Relatorio->fLarguraMaxDados)
								{
									//iniciar valores
									fX = 0.0;
									fY += fAlturaLinha;
									fAlturaLinha = 0.0;
									if (Dados->fAltura > fAlturaLinha)
										fAlturaLinha = Dados->fAltura;
								}
								//mover para próxima página
								if (fAlturaLinha + fY > Relatorio->fAlturaMaxDados)
								{
									fX = 0.0;
									fY = 0.0;
									nPagina++;
								}
								//loop das células
								for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									Celula = (BEPDF_Celula *) ListaL1->data;
									Celula->fX += fX;
									Celula->fY += fY;
									if (Dados->fAltura > fAlturaLinha)
										fAlturaLinha = Dados->fAltura;
									Celula->nPagina = nPagina;
								}
								//mover para direita
								fX += Dados->fLargura;
							}
							//atualizar páginas
							if (nPagina > nPaginas)
								nPaginas = nPagina;
						}
						//posição das células, loop dos formulários
						if (NoForms)
						{
							fX = 0.0;
							fY = 0.0;
							fAlturaLinha = 0.0;
							nPagina = 1;
							nCont1 = be_sql_campo_quantidade_ativos (Sintaxe);
							DadosAnt = NULL;
							for (ListaL = Relatorio->Formularios, nCont = 0; ListaL; ListaL = g_slist_next (ListaL), nCont++)
							{
								Dados = (_BERELATORIO_Dados *) ListaL->data;
								//reposicionar sublistas
								if (DadosAnt && DadosAnt->lLinha != Dados->lLinha)
								{
									//avançar próximoa linha
									fY += fAlturaLinha;
									//loop por sublistas
									for (ListaL1 = Relatorio->SubListas; ListaL1; ListaL1 = g_slist_next (ListaL1))
									{
										SubLista = (_BERELATORIO_SubLista *) ListaL1->data;
										//somente mesma lista
										if (SubLista->lLinha == DadosAnt->lLinha)
										{
											//loop por campos
											for (ListaL2 = Sintaxe->Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
											{
												Campo = (BESQL_Campo *) ListaL2->data;
												//somente mesmo campo
												if (SubLista->Campo == Campo)
													_be_subrelatorio_reposicionar (Relatorio, SubLista, No, &fY, &nPagina);
											}
										}
									}
								}
								//verificar quebra de página para formulário padrão
								if (nCont > 0 && (nCont % nCont1) == 0)
								{
									//formulário
									fX = 0.0;
									fY = 0.0;
									fAlturaLinha = 0.0;
									nPagina++;
								}
								//mover para próxima linha
								if (Dados->fLargura + fX > Relatorio->fLarguraMaxForms)
								{
									//iniciar valores
									fX = 0.0;
									fY += fAlturaLinha;
									fAlturaLinha = 0.0;
									if (Dados->fAltura > fAlturaLinha)
										fAlturaLinha = Dados->fAltura;
								}
								//mover para próxima página
								if (fAlturaLinha + fY > Relatorio->fAlturaMaxForms)
								{
									fX = 0.0;
									fY = 0.0;
									fAlturaLinha = 0.0;
									nPagina++;
								}
								//loop das células
								for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									Celula = (BEPDF_Celula *) ListaL1->data;
									Celula->fX += fX;
									Celula->fY += fY;
									if (Dados->fAltura > fAlturaLinha)
										fAlturaLinha = Dados->fAltura;
									Celula->nPagina = nPagina;
									//alterar local
									Celula->nLocal = BEPDF_LOC_CORPO;
								}
								//mover para direita
								fX += Dados->fLargura;
								//marcar anterior
								DadosAnt = Dados;
							}
							//ÚLITMA LINHA: reposicionar sublistas
							if (DadosAnt)
							{
								//avançar próximoa linha
								fY += fAlturaLinha;
								//loop por sublistas
								for (ListaL1 = Relatorio->SubListas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									SubLista = (_BERELATORIO_SubLista *) ListaL1->data;
									//somente mesma lista
									if (SubLista->lLinha == DadosAnt->lLinha)
									{
										//loop por campos
										for (ListaL2 = Sintaxe->Campos; ListaL2; ListaL2 = g_slist_next (ListaL2))
										{
											Campo = (BESQL_Campo *) ListaL2->data;
											//somente mesmo campo
											if (SubLista->Campo == Campo)
												_be_subrelatorio_reposicionar (Relatorio, SubLista, No, &fY, &nPagina);
										}
									}
								}
							}
							//atualizar páginas
							if (nPagina > nPaginas)
								nPaginas = nPagina;
						}
						//posição das células, loop dos rótulos
						if (NoRotulos)
						{
							fX = 0.0;
							fLinhaInicial = 0.0;
							for (ListaL = Relatorio->Rotulos; ListaL; ListaL = g_slist_next (ListaL))
							{
								Dados = (_BERELATORIO_Dados *) ListaL->data;
								//loop das células
								for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									Celula = (BEPDF_Celula *) ListaL1->data;
									Celula->nLocal = BEPDF_LOC_CABECALHO;
									Celula->fX += fX;
								}
								//mover para direita
								fX += Dados->fLargura;
								//altura máxima, para as listas iniciarem
								if (Dados->fAltura > fLinhaInicial)
									fLinhaInicial = Dados->fAltura;
							}
							//alterar a altura dos retângulos para ficarem todos iguais
							for (ListaL = Relatorio->Rotulos; ListaL; ListaL = g_slist_next (ListaL))
							{
								Dados = (_BERELATORIO_Dados *) ListaL->data;
								//loop das células
								for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									Celula = (BEPDF_Celula *) ListaL1->data;
									Celula->fAltura = fLinhaInicial + (Relatorio->fYMinListas - Celula->fY);
								}
							}
						}
						//posição das células, loop das listas
						if (NoListas)
						{
							fX = 0.0;
							fY = fLinhaInicial;
							fAlturaLinha = 0.0;
							nPagina = 1;
							LinhaInicio = Relatorio->Listas;
							for (ListaL = Relatorio->Listas; ListaL; ListaL = g_slist_next (ListaL))
							{
								Dados = (_BERELATORIO_Dados *) ListaL->data;
								//mover para próxima linha
								if (Dados->fLargura + fX > Relatorio->fLarguraMaxListas)
								{
									//alterar a altura dos retângulos para ficarem todos iguais
									for (ListaL1 = LinhaInicio; ListaL1 != ListaL; ListaL1 = g_slist_next (ListaL1))
									{
										DadosAnt = (_BERELATORIO_Dados *) ListaL1->data;
										//loop das células
										for (ListaL2 = DadosAnt->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
										{
											Celula = (BEPDF_Celula *) ListaL2->data;
											Celula->fAltura = fAlturaLinha;
										}
									}
									LinhaInicio = ListaL;
									bFundo = !bFundo;
									//iniciar valores
									fX = 0.0;
									fY += fAlturaLinha;
									fAlturaLinha = 0.0;
									if (Dados->fAltura > fAlturaLinha)
										fAlturaLinha = Dados->fAltura;
								}
								//mover para próxima página
								if (fAlturaLinha + fY > Relatorio->fAlturaMaxListas)
								{
									//alterar posição e página das células que começaram a linha
									for (ListaL1 = LinhaInicio; ListaL1 != ListaL; ListaL1 = g_slist_next (ListaL1))
									{
										DadosAnt = (_BERELATORIO_Dados *) ListaL1->data;
										//loop das células
										for (ListaL2 = DadosAnt->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
										{
											Celula = (BEPDF_Celula *) ListaL2->data;
											Celula->nPagina = nPagina + 1;
											Celula->fY -= fY;
											Celula->fY += fLinhaInicial;
										}
									}
									fY = fLinhaInicial;
									nPagina++;
								}
								//loop das células
								for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									Celula = (BEPDF_Celula *) ListaL1->data;
									Celula->fX += fX;
									Celula->fY += fY;
									if (Dados->fAltura > fAlturaLinha)
										fAlturaLinha = Dados->fAltura;
									Celula->nPagina = nPagina;
									//alterar local
									Celula->nLocal = BEPDF_LOC_CORPO;
									if (!bFundo)
									{
										BE_free (Celula->sCorFundo);
									}
								}
								//mover para direita
								fX += Dados->fLargura;
							}
							//alterar a altura dos retângulos para ficarem todos iguais, última linha
							for (ListaL1 = LinhaInicio; ListaL1; ListaL1 = g_slist_next (ListaL1))
							{
								DadosAnt = (_BERELATORIO_Dados *) ListaL1->data;
								//loop das células
								for (ListaL2 = DadosAnt->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
								{
									Celula = (BEPDF_Celula *) ListaL2->data;
									Celula->fAltura = fAlturaLinha;
								}
							}
							//atualizar páginas
							if (nPagina > nPaginas)
								nPaginas = nPagina;
						}
						//posição das células, loop dos indices
						if (NoIndices && NoListas)
						{
							nCont = be_sql_campo_quantidade_ativos (Sintaxe);
							nCont = nCont > BERELATORIO_LISTA_MAX_COLUNAS ? BERELATORIO_LISTA_MAX_COLUNAS : nCont;//corrigir
							ListaL2 = Relatorio->Listas;
							DadosAnt = (_BERELATORIO_Dados *) ListaL2->data;
							CelulaAnt = DadosAnt->Celulas->data;
							for (ListaL = Relatorio->Indices; ListaL; ListaL = g_slist_next (ListaL))
							{
								Dados = (_BERELATORIO_Dados *) ListaL->data;//índices
								//loop das celulas
								for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
								{
									Celula = (BEPDF_Celula *) ListaL1->data;
									Celula->nLocal = BEPDF_LOC_CORPO;
									Celula->nPagina = CelulaAnt->nPagina;
									Celula->fY = CelulaAnt->fY;
									Celula->fAltura = CelulaAnt->fAltura;
								}
								for (nCont1 = 0; nCont1 < nCont; ListaL2 = g_slist_next (ListaL2), nCont1++);
								if (ListaL2)
								{
									DadosAnt = (_BERELATORIO_Dados *) ListaL2->data;
									CelulaAnt = DadosAnt->Celulas->data;
								}
							}
						}
					}
				}
				//cabecalho
				NoFilho = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "CABECALHO", BEXML_LOCALIZAR_IRMAO);
				if (NoFilho)
				{
					Relatorio->nLocal = BEPDF_LOC_CABECALHO;
					Relatorio->fYRelativo = 0.0;
					Relatorio->DadosAtual = NULL;
					g_node_traverse (NoFilho, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
				}
				//rodapé
				NoFilho = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "RODAPE", BEXML_LOCALIZAR_IRMAO);
				if (NoFilho)
				{
					Relatorio->nLocal = BEPDF_LOC_RODAPE;
					Relatorio->fYRelativo = 0.0;
					Relatorio->DadosAtual = NULL;
					g_node_traverse (NoFilho, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
				}
				//substituir variáveis de páginas
				NoFilho = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "PAGINA", BEXML_LOCALIZAR_IRMAO);
				if (NoFilho)
				{
					for (nPagina = 1; nPagina <= nPaginas; nPagina++)
					{
						//criar dados
						Dados = g_new (_BERELATORIO_Dados, 1);
						Dados->Celulas = NULL;
						Dados->fLargura = 0.0;
						Dados->fAltura = 0.0;
						Dados->lLinha = 0;
						Dados->Campo = NULL;
						//anexar
						Relatorio->Paginas = g_slist_append (Relatorio->Paginas, Dados);
						//criar células
						Relatorio->Campo = NULL;
						Relatorio->nLocal = BERELATORIO_LOC_PAGINAS;
						Relatorio->fYRelativo = 0.0;
						Relatorio->DadosAtual = Dados;
						Relatorio->fXMinAtual = 0.0;
						Relatorio->fYMinAtual = 0.0;
						Relatorio->nPagina = nPagina;
						Relatorio->nPaginas = nPaginas;
						//criar células
						g_node_traverse (NoFilho, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
					}
					//alterar local
					for (ListaL = Relatorio->Paginas; ListaL; ListaL = g_slist_next (ListaL))
					{
						Dados = (_BERELATORIO_Dados *) ListaL->data;
						//loop das células
						for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
						{
							Celula = (BEPDF_Celula *) ListaL1->data;
							//alterar local
							Celula->nLocal = BEPDF_LOC_CORPO;
						}
					}
				}
				//limpar

				//dados
				for (ListaL = Relatorio->Dados; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					BE_free (Dados);
				}
				BE_slist_free (Relatorio->Dados);
				//páginas
				for (ListaL = Relatorio->Paginas; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					BE_free (Dados);
				}
				BE_slist_free (Relatorio->Paginas);
				//formulários
				for (ListaL = Relatorio->Formularios; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					BE_free (Dados);
				}
				BE_slist_free (Relatorio->Formularios);
				//rótulos
				for (ListaL = Relatorio->Rotulos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					BE_free (Dados);
				}
				BE_slist_free (Relatorio->Rotulos);
				//indices
				for (ListaL = Relatorio->Indices; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					BE_free (Dados);
				}
				BE_slist_free (Relatorio->Indices);
				//listas
				for (ListaL = Relatorio->Listas; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					BE_free (Dados);
				}
				BE_slist_free (Relatorio->Listas);
				//listas
				for (ListaL = Relatorio->SubListas; ListaL; ListaL = g_slist_next (ListaL))
				{
					SubLista = (_BERELATORIO_SubLista *) ListaL->data;
					_be_subrelatorio_limpar (SubLista);
				}
				BE_slist_free (Relatorio->SubListas);

				//gerar PDF
				if (!sErros || (sErros && !(*sErros)))
					if (!be_pdf_gravar (PDF, sArquivoSaida))
					{
						BE_free (*sErros);
						BE_free (sTemp);
						sTemp = g_strdup_printf ("Arquivo %s não é válido.", sArquivoSaida);
						*sErros = BE_ISO_UTF8 (sTemp);
					}
				BE_pdf_fechar (PDF);

				//imagens
				for (ListaL = Relatorio->Imagens; ListaL; ListaL = g_slist_next (ListaL))
				{
					Imagem = (GdkPixbuf *) ListaL->data;
					BE_pixbuf_free (Imagem);
				}
				BE_slist_free (Relatorio->Imagens);
				BE_free (Relatorio);
			}
		}
	}
	//limpar
	BE_xml_no_descarregar (No);
	BE_xml_fechar (XML);
	BE_strfreev (aLista);
	BE_free (sTemp);

	return !sErros || (sErros && !(*sErros));
}

_BERELATORIO_SubLista *_be_subrelatorio_gerar (_BERELATORIO *Relatorio, BESQL_Sintaxe *SubSintaxe, const gchar *sSubRelatorio, GNode *No, glong lLinha, BESQL_Campo *CampoRef, gchar **sErros)
{
	_BERELATORIO_SubLista *SubLista = NULL;
	BEPDF *PDF = NULL;
	BESQL_Campo *Campo = NULL;
	BEPDF_Celula *Celula = NULL, *CelulaAnt = NULL;
	BEPDF_Pagina *Pagina = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	_BERELATORIO_Dados *Dados = NULL, *DadosAnt = NULL;
	GNode *NoFilho = NULL, *NoSubCabecalhos = NULL, *NoSubListas = NULL, *NoSubRotulos = NULL, *NoSubIndices = NULL;
	GSList *ListaL = NULL, *ListaL1 = NULL, *ListaL2 = NULL, *ListaL3 = NULL;
	GSList *LinhaInicio = NULL;
	GSList *SubLinha = NULL;
	gchar *sTemp = NULL;
	const gchar *sValor = NULL;
	gint nCont = 0, nCont1 = 0;
	gfloat fX = 0.0, fY = 0.0, fAlturaLinha = 0.0, fLinhaInicial = 0.0;
	gint nPagina = 1, nPaginas = 1, nPaginaDados = 1, nComprimentoMax = 0;
	glong lRegistros = 0;
	gboolean bFundo = FALSE;
	glong lLinhas = 0;

	//recuperar variável
	PDF = Relatorio->PDF;
	Pagina = PDF->Pagina;

	//criar objeto
	SubLista = g_new (_BERELATORIO_SubLista, 1);
	SubLista->SubCabecalhos = NULL;
	SubLista->SubRotulos = NULL;
	SubLista->SubListas = NULL;
	SubLista->SubIndices = NULL;
	SubLista->Linhas = NULL;
	SubLista->lLinha = lLinha;
	SubLista->Campo = CampoRef;//campo de referência, gerador da sublista
	SubLista->sSubRelatorio = g_strdup (sSubRelatorio);
	SubLista->fYMinSubListas = 0;

	//localizar limites de sublistas
	NoFilho = be_xml_no_localizar_elemento (No, "rect", "id", "SUBLISTAS");
	if (NoFilho)
	{
		NoElemento = (BEXML_No_Elemento *) NoFilho->data;
#ifndef BERELATORIO_DEPURAR
		//alterar nome, para não aparecer
		BE_free (NoElemento->sNome);
		NoElemento->sNome = g_strdup ("rect");
#endif
		//posições
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
		Relatorio->fXMinSubListas = _be_relatorio_conversao (sValor);
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
		Relatorio->fYMinSubListas = _be_relatorio_conversao (sValor) + Relatorio->fYRelativo;
		//dimensões
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
		Relatorio->fLarguraMaxSubListas = _be_relatorio_conversao (sValor);
		Relatorio->fAlturaMaxSubListas = Relatorio->fAlturaMaxForms;//mesma altura do formulário
	}
	//verificar limites
	if (Relatorio->fLarguraMaxSubListas <= 0.0)
		Relatorio->fLarguraMaxSubListas = Pagina->fLargura;
	if (Relatorio->fAlturaMaxSubListas <= 0.0)
		Relatorio->fAlturaMaxSubListas = Pagina->fAltura;
	//localizar objetos de cada seção
	NoSubListas = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "SUBLISTAS", BEXML_LOCALIZAR_IRMAO);
	NoSubRotulos = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "SUBROTULOS", BEXML_LOCALIZAR_IRMAO);
	NoSubIndices = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "SUBINDICES", BEXML_LOCALIZAR_IRMAO);
	NoSubCabecalhos = be_xml_no_atributo_valor (No->children, "g", "inkscape:label", "SUBCABECALHO", BEXML_LOCALIZAR_IRMAO);
	//loop nos dados
	if (SubSintaxe)
	{
		lRegistros = 0;
		SubSintaxe->lLinhasPaginaMax = BERELATORIO_MAX_LINHAS_POR_PAGINA;
		be_sql_comprimento_linhas (SubSintaxe, sErros);
		be_sql_totalizar_linhas (SubSintaxe, sErros);
		do
		{
			lLinhas = be_sql_selecionar (SubSintaxe, nPaginaDados, sErros);
			while (lLinhas > 0 && (Relatorio->lLinha = be_sql_selecionar_proxima_linha (SubSintaxe)))
			{
				//contar linhas de registros
				lRegistros++;

				//calcular o comprimento total de cada campo, para listas
				if (lRegistros == 1)
				{
					//calcular o valor máximo de comprimento dos dados
					for (ListaL = SubSintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
					{
						Campo = (BESQL_Campo *) ListaL->data;
						if (Campo->bAtivo)
						{
							//limites para uma exibição aceitável
							if (Campo->nComprimentoTop < 7)
								Campo->nComprimentoTop = 7;
							else if (Campo->nComprimentoTop > 30)
								Campo->nComprimentoTop = 30;
							//calcular total
							nComprimentoMax += Campo->nComprimentoTop;
							nCont++;
						}
					}
				}
				//criar listas
				if (NoSubListas)
				{
					//colunas
					for (ListaL = SubSintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
					{
						Campo = (BESQL_Campo *) ListaL->data;
						if (Campo->bAtivo)
						{
							//criar formulários
							Dados = g_new (_BERELATORIO_Dados, 1);
							Dados->Celulas = NULL;
							Dados->fLargura = 0.0;
							Dados->fAltura = 0.0;
							Dados->lLinha = Relatorio->lLinha;
							Dados->Campo = Campo;
							//anexar
							SubLista->SubListas = g_slist_append (SubLista->SubListas, Dados);
							//criar células
							Relatorio->Campo = Campo;
							Relatorio->fLarguraProporcional = (gfloat) Campo->nComprimentoTop / (gfloat) nComprimentoMax;
							Relatorio->nLocal = BERELATORIO_LOC_SUBLISTAS;
							Relatorio->fYRelativo = 0.0;
							Relatorio->DadosAtual = Dados;
							Relatorio->fXMinAtual = Relatorio->fXMinSubListas;
							Relatorio->fYMinAtual = Relatorio->fYMinSubListas;
							g_node_traverse (NoSubListas, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
							//nao tentar mais se não houver células
							if (g_slist_length (Dados->Celulas) == 0)
							{
								NoSubListas = NULL;
								break;
							}
							nCont++;
						}
					}
				}
				//criar rótulos, somente uma vez
				if (NoSubRotulos && lRegistros == 1)
				{
					//colunas
					for (ListaL = SubSintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
					{
						Campo = (BESQL_Campo *) ListaL->data;
						if (Campo->bAtivo)
						{
							//criar rótulos
							Dados = g_new (_BERELATORIO_Dados, 1);
							Dados->Celulas = NULL;
							Dados->fLargura = 0.0;
							Dados->fAltura = 0.0;
							Dados->lLinha = Relatorio->lLinha;
							Dados->Campo = Campo;
							//anexar
							SubLista->SubRotulos = g_slist_append (SubLista->SubRotulos, Dados);
							//anexar sublinha
							SubLinha = g_slist_append (SubLinha, Dados);
							//criar células
							Relatorio->Campo = Campo;
							Relatorio->fLarguraProporcional = (gfloat) Campo->nComprimentoTop / (gfloat) nComprimentoMax;
							Relatorio->nLocal = BERELATORIO_LOC_SUBROTULOS;
							Relatorio->fYRelativo = 0.0;
							Relatorio->DadosAtual = Dados;
							Relatorio->fXMinAtual = Relatorio->fXMinSubListas;
							Relatorio->fYMinAtual = Relatorio->fYMinSubListas;
							g_node_traverse (NoSubRotulos, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
							//não tentar mais se não houver células
							if (g_slist_length (Dados->Celulas) == 0)
							{
								NoSubRotulos = NULL;
								break;
							}
							nCont++;
						}
					}
				}
				//criar índices, vertical
				if (NoSubIndices)
				{
					Dados = g_new (_BERELATORIO_Dados, 1);
					Dados->Celulas = NULL;
					Dados->fLargura = 0.0;
					Dados->fAltura = 0.0;
					Dados->lLinha = Relatorio->lLinha;
					Dados->Campo = NULL;
					//anexar
					SubLista->SubIndices = g_slist_append (SubLista->SubIndices, Dados);
					//criar células
					Relatorio->Campo = NULL;
					Relatorio->nLocal = BERELATORIO_LOC_SUBINDICES;
					Relatorio->fYRelativo = 0.0;
					Relatorio->DadosAtual = Dados;
					Relatorio->fXMinAtual = Relatorio->fXMinSubListas;
					Relatorio->fYMinAtual = Relatorio->fYMinSubListas;
					g_node_traverse (NoSubIndices, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
					//nao tentar mais se não houver células
					if (g_slist_length (Dados->Celulas) == 0)
						NoSubIndices = NULL;
				}
			}
			nPaginaDados++;
		} while (lLinhas > 0 && lLinhas >= BERELATORIO_MAX_LINHAS_POR_PAGINA);
		//marcar fim das linhas
		Relatorio->lLinha = -1;

		//criar subtotais para listas
		if (NoSubListas && SubLista->SubListas)
		{
			//colunas
			for (ListaL = SubSintaxe->Campos, nCont = 0; ListaL && nCont < BERELATORIO_LISTA_MAX_COLUNAS; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				if (Campo->bAtivo)
				{
					//criar lista
					Dados = g_new (_BERELATORIO_Dados, 1);
					Dados->Celulas = NULL;
					Dados->fLargura = 0.0;
					Dados->fAltura = 0.0;
					Dados->lLinha = Relatorio->lLinha;
					Dados->Campo = Campo;
					//anexar
					SubLista->SubListas = g_slist_append (SubLista->SubListas, Dados);
					//criar células
					Relatorio->Campo = Campo;
					Relatorio->fLarguraProporcional = (gfloat) Campo->nComprimentoTop / (gfloat) nComprimentoMax;
					Relatorio->nLocal = BERELATORIO_LOC_SUBLISTAS;
					Relatorio->fYRelativo = 0.0;
					Relatorio->DadosAtual = Dados;
					Relatorio->fXMinAtual = Relatorio->fXMinSubListas;
					Relatorio->fYMinAtual = Relatorio->fYMinSubListas;
					g_node_traverse (NoSubListas, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
					//nao tentar mais se não houver células
					if (g_slist_length (Dados->Celulas) == 0)
					{
						NoSubListas = NULL;
						break;
					}
					nCont++;
				}
			}
		}
		//criar índices, vertical
		if (NoSubIndices && SubLista->SubListas)
		{
			Dados = g_new (_BERELATORIO_Dados, 1);
			Dados->Celulas = NULL;
			Dados->fLargura = 0.0;
			Dados->fAltura = 0.0;
			Dados->lLinha = Relatorio->lLinha;
			Dados->Campo = NULL;
			//anexar
			SubLista->SubIndices = g_slist_append (SubLista->SubIndices, Dados);
			//criar células
			Relatorio->Campo = NULL;
			Relatorio->nLocal = BERELATORIO_LOC_SUBINDICES;
			Relatorio->fYRelativo = 0.0;
			Relatorio->DadosAtual = Dados;
			Relatorio->fXMinAtual = Relatorio->fXMinSubListas;
			Relatorio->fYMinAtual = Relatorio->fYMinSubListas;
			g_node_traverse (NoSubIndices, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
			//nao tentar mais se não houver células
			if (g_slist_length (Dados->Celulas) == 0)
				NoSubIndices = NULL;
		}
		//desmarcar fim das linhas
		Relatorio->lLinha = 0;

		//criar cabeçalhos
		if (NoSubCabecalhos)
		{
			Dados = g_new (_BERELATORIO_Dados, 1);
			Dados->Celulas = NULL;
			Dados->fLargura = 0.0;
			Dados->fAltura = 0.0;
			Dados->lLinha = Relatorio->lLinha;
			Dados->Campo = NULL;
			//anexar
			SubLista->SubCabecalhos = g_slist_append (SubLista->SubCabecalhos, Dados);
			//anexar sublinha
			SubLinha = g_slist_append (SubLinha, Dados);
			//criar células
			Relatorio->Campo = NULL;
			Relatorio->nLocal = BEPDF_LOC_CABECALHO;
			Relatorio->fYRelativo = 0.0;
			Relatorio->DadosAtual = Dados;
			Relatorio->fXMinAtual = Relatorio->fXMinForms;
			Relatorio->fYMinAtual = Relatorio->fYMinForms;
			g_node_traverse (NoSubCabecalhos, G_PRE_ORDER, G_TRAVERSE_ALL, -1, _be_relatorio_loop, Relatorio);
			//descobrir o Y mínimo
			if (Dados->Celulas)
			{
				//valor inicial
				SubLista->fYMinSubListas = G_MAXFLOAT;
				//loop das células
				for (ListaL = Dados->Celulas; ListaL; ListaL = g_slist_next (ListaL))
				{
					Celula = (BEPDF_Celula *) ListaL->data;
					//verificar valor mínimo
					if (SubLista->fYMinSubListas > Celula->fY)
						SubLista->fYMinSubListas = Celula->fY;
				}
				//corrigir valor
				SubLista->fYMinSubListas -= Relatorio->fYMinForms;
			}
		}

		//posicionamentos
		if (!sErros || (sErros && !(*sErros)))
		{
			//posição das células, loop dos rótulos
			if (SubLista->SubRotulos)
			{
				fX = 0.0;
				fLinhaInicial = 0.0;
				for (ListaL = SubLista->SubRotulos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					//loop das células
					for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Celula = (BEPDF_Celula *) ListaL1->data;
						Celula->nLocal = BEPDF_LOC_CABECALHO;
						Celula->fX += fX;
					}
					//mover para direita
					fX += Dados->fLargura;
					//altura máxima, para as listas iniciarem
					if (Dados->fAltura > fLinhaInicial)
						fLinhaInicial = Dados->fAltura;
				}
				//alterar a altura dos retângulos para ficarem todos iguais
				for (ListaL = SubLista->SubRotulos; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					//loop das células
					for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Celula = (BEPDF_Celula *) ListaL1->data;
						Celula->fAltura = fLinhaInicial + (Relatorio->fYMinSubListas - Celula->fY);
					}
				}
			}
			//1ª linha
			SubLista->Linhas = g_slist_append (SubLista->Linhas, SubLinha);
			SubLinha = NULL;//iniciar próxima sublinha
			//posição das células, loop das listas
			if (SubLista->SubListas)
			{
				fX = 0.0;
				fY = fLinhaInicial;
				fAlturaLinha = 0.0;
				LinhaInicio = SubLista->SubListas;
				//loop das sublistas
				for (ListaL = SubLista->SubListas; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;
					//mover para próxima linha
					if (Dados->fLargura + fX > Relatorio->fLarguraMaxSubListas)
					{
						//alterar a altura dos retângulos para ficarem todos iguais
						for (ListaL1 = LinhaInicio; ListaL1 != ListaL; ListaL1 = g_slist_next (ListaL1))
						{
							DadosAnt = (_BERELATORIO_Dados *) ListaL1->data;
							//loop das células
							for (ListaL2 = DadosAnt->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
							{
								Celula = (BEPDF_Celula *) ListaL2->data;
								Celula->fAltura = fAlturaLinha;
							}
						}
						LinhaInicio = ListaL;
						bFundo = !bFundo;
						//iniciar valores
						fX = 0.0;
						fY += fAlturaLinha;
						fAlturaLinha = 0.0;
						if (Dados->fAltura > fAlturaLinha)
							fAlturaLinha = Dados->fAltura;
						//próximas linhas
						SubLista->Linhas = g_slist_append (SubLista->Linhas, SubLinha);
						SubLinha = NULL;//iniciar próxima sublinha
					}
					//loop das células
					for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Celula = (BEPDF_Celula *) ListaL1->data;
						Celula->fX += fX;
						Celula->fY += fY;
						if (Dados->fAltura > fAlturaLinha)
							fAlturaLinha = Dados->fAltura;
						Celula->nPagina = nPagina;
						//alterar local
						Celula->nLocal = BEPDF_LOC_CORPO;
						if (!bFundo)
						{
							BE_free (Celula->sCorFundo);
						}
					}
					//anexar sublinha
					SubLinha = g_slist_append (SubLinha, Dados);
					//mover para direita
					fX += Dados->fLargura;
				}
				//alterar a altura dos retângulos para ficarem todos iguais, última linha
				for (ListaL1 = LinhaInicio; ListaL1; ListaL1 = g_slist_next (ListaL1))
				{
					DadosAnt = (_BERELATORIO_Dados *) ListaL1->data;
					//loop das células
					for (ListaL2 = DadosAnt->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
					{
						Celula = (BEPDF_Celula *) ListaL2->data;
						Celula->fAltura = fAlturaLinha;
					}
				}
				//próximas linhas
				SubLista->Linhas = g_slist_append (SubLista->Linhas, SubLinha);
				SubLinha = NULL;//iniciar próxima sublinha
				//atualizar páginas
				if (nPagina > nPaginas)
					nPaginas = nPagina;
			}
			//posição das células, loop dos indices
			if (SubLista->SubIndices && SubLista->SubListas)
			{
				nCont = be_sql_campo_quantidade_ativos (SubSintaxe);
				nCont = nCont > BERELATORIO_LISTA_MAX_COLUNAS ? BERELATORIO_LISTA_MAX_COLUNAS : nCont;//corrigir
				ListaL2 = SubLista->SubListas;
				DadosAnt = (_BERELATORIO_Dados *) ListaL2->data;
				if (DadosAnt && DadosAnt->Celulas)
					CelulaAnt = DadosAnt->Celulas->data;

				//primeira linha
				ListaL3 = SubLista->Linhas;

				for (ListaL = SubLista->SubIndices; ListaL; ListaL = g_slist_next (ListaL))
				{
					Dados = (_BERELATORIO_Dados *) ListaL->data;//índices

					//próxima linha
					ListaL3 = g_slist_next (ListaL3);
					SubLinha = (GSList *) ListaL3->data;
					//anexar sublinha
					SubLinha = g_slist_append (SubLinha, Dados);

					//loop das celulas
					for (ListaL1 = Dados->Celulas; ListaL1; ListaL1 = g_slist_next (ListaL1))
					{
						Celula = (BEPDF_Celula *) ListaL1->data;
						Celula->nLocal = BEPDF_LOC_CORPO;
						Celula->nPagina = CelulaAnt->nPagina;
						Celula->fY = CelulaAnt->fY;
						Celula->fAltura = CelulaAnt->fAltura;
					}
					for (nCont1 = 0; nCont1 < nCont; ListaL2 = g_slist_next (ListaL2), nCont1++);
					if (ListaL2)
					{
						DadosAnt = (_BERELATORIO_Dados *) ListaL2->data;
						CelulaAnt = DadosAnt->Celulas->data;
					}
				}
			}
		}
	}

	//limpar
	BE_free (sTemp);
	if (sErros && *sErros)
	{
		_be_subrelatorio_limpar (SubLista);
		SubLista = NULL;
	}

	return SubLista;
}

void _be_subrelatorio_reposicionar (_BERELATORIO *Relatorio, _BERELATORIO_SubLista *SubLista, GNode *No, gfloat *fYRef, gint *nPagina)
{
	_BERELATORIO_Dados *Dados = NULL;
	BEPDF_Celula *Celula = NULL;
	GSList *SubLinhaL = NULL, *ListaL = NULL, *ListaL1 = NULL, *ListaL2 = NULL, *ListaAntL = NULL;
	gfloat fYMax = 0.0, fYMin = G_MAXFLOAT, fYCorrecao = 0.0;

	//corrigir espaçamentos
	*fYRef += BERELATORIO_ESPACAMENTO_SUBLISTAS;
	//loop nas linhas
	for (ListaL = SubLista->Linhas; ListaL; ListaAntL = ListaL, ListaL = g_slist_next (ListaL))
	{
		//mover para próxima página; a linha atual ultrapassou os limites
		if (fYMax > Relatorio->fAlturaMaxForms + Relatorio->fYMinForms)
		{
			//retornar linha
			ListaL = ListaAntL;
			SubLinhaL = (GSList *) ListaL->data;
			//reiniciar valores
			fYMin = G_MAXFLOAT;
			fYMax = 0.0;
			//desfazer todas alterações na linha atual; loop nas sublinhas
			for (ListaL1 = SubLinhaL; ListaL1; ListaL1 = g_slist_next (ListaL1))
			{
				Dados = (_BERELATORIO_Dados *) ListaL1->data;
				//loop das células
				for (ListaL2 = Dados->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
				{
					Celula = (BEPDF_Celula *) ListaL2->data;
					Celula->fY += SubLista->fYMinSubListas;//corrigir posição
					Celula->fY -= (*fYRef - fYCorrecao);//reposicionar
					//verificar valor máximo
					if (fYMax < (Celula->fY + Celula->fAltura))
						fYMax = Celula->fY + Celula->fAltura;
					//verificar valor mínimo
					if (fYMin > Celula->fY)
						fYMin = Celula->fY;
				}
			}
			//avançar página
			*nPagina = *nPagina + 1;
			//reposicionar linha
			*fYRef = 0.0;
			fYCorrecao = (fYMin - Relatorio->fYMinForms) - SubLista->fYMinSubListas;
			//reiniciar valores
			fYMax = 0.0;
			fYMin = G_MAXFLOAT;
		}
		SubLinhaL = (GSList *) ListaL->data;
		//loop nas sublinhas
		for (ListaL1 = SubLinhaL; ListaL1; ListaL1 = g_slist_next (ListaL1))
		{
			Dados = (_BERELATORIO_Dados *) ListaL1->data;
			//loop das células
			for (ListaL2 = Dados->Celulas; ListaL2; ListaL2 = g_slist_next (ListaL2))
			{
				Celula = (BEPDF_Celula *) ListaL2->data;
				Celula->fY -= SubLista->fYMinSubListas;//corrigir posição
				Celula->fY += (*fYRef - fYCorrecao);//reposicionar
				Celula->nPagina = *nPagina;
				//alterar local
				Celula->nLocal = BEPDF_LOC_CORPO;
				//verificar valor máximo
				if (fYMax < (Celula->fY + Celula->fAltura))
					fYMax = Celula->fY + Celula->fAltura;
				//verificar valor mínimo
				if (fYMin > Celula->fY)
					fYMin = Celula->fY;
			}
		}
	}
	//atualizar valor de Y, corrigir altura
	if (fYMax > 0)
		*fYRef += fYMax - fYMin;
	//valor original
	Relatorio->sSubRelatorio = NULL;
}

void _be_subrelatorio_limpar (_BERELATORIO_SubLista *SubLista)
{
	_BERELATORIO_Dados *Dados = NULL;
	GSList *SubLinhaL = NULL, *ListaL = NULL;
	//subrótulos
	for (ListaL = SubLista->SubCabecalhos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Dados = (_BERELATORIO_Dados *) ListaL->data;
		BE_free (Dados);
	}
	BE_slist_free (SubLista->SubCabecalhos);
	//subrótulos
	for (ListaL = SubLista->SubRotulos; ListaL; ListaL = g_slist_next (ListaL))
	{
		Dados = (_BERELATORIO_Dados *) ListaL->data;
		BE_free (Dados);
	}
	BE_slist_free (SubLista->SubRotulos);
	//subindices
	for (ListaL = SubLista->SubIndices; ListaL; ListaL = g_slist_next (ListaL))
	{
		Dados = (_BERELATORIO_Dados *) ListaL->data;
		BE_free (Dados);
	}
	BE_slist_free (SubLista->SubIndices);
	//sublistas
	for (ListaL = SubLista->SubListas; ListaL; ListaL = g_slist_next (ListaL))
	{
		Dados = (_BERELATORIO_Dados *) ListaL->data;
		BE_free (Dados);
	}
	BE_slist_free (SubLista->SubListas);
	//lista
	for (ListaL = SubLista->Linhas; ListaL; ListaL = g_slist_next (ListaL))
	{
		SubLinhaL = (GSList *) ListaL->data;
		g_slist_free (SubLinhaL);
	}
	BE_slist_free (SubLista->Linhas);

	BE_free (SubLista->sSubRelatorio);

	BE_free (SubLista);
}

BEPDF_Celula * _be_relatorio_retangulo (GNode *No, _BERELATORIO *Relatorio)
{
	BEPDF *PDF = NULL;
	BEPDF_Celula *Celula = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	const gchar *sValor = NULL;
	GSList *ListaEstilos = NULL, *ListaL = NULL;
	gchar **aValor = NULL, **aTemp = NULL;
	gchar *sTemp = NULL;
	gboolean bInvisivel = FALSE;

	NoElemento = (BEXML_No_Elemento *) No->data;
	PDF = Relatorio->PDF;
	//nova célula
	Celula = be_pdf_nova_celula (PDF);
	BE_free (Celula->sCorBorda);//valor padrão, sem borda
	//local
	Celula->nLocal = Relatorio->nLocal;
	Celula->nPagina = Relatorio->nPagina;
	//posições
	sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
	Celula->fX = _be_relatorio_conversao (sValor);
	sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
	Celula->fY = _be_relatorio_conversao (sValor) + Relatorio->fYRelativo;
	//dimensões
	sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
	Celula->fLargura = _be_relatorio_conversao (sValor);
	sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "height");
	Celula->fAltura = _be_relatorio_conversao (sValor);
	//transformações
	_be_relatorio_transformacoes (No, &Celula->fX, &Celula->fY, &Celula->fLargura, &Celula->fAltura, NULL);
	//verificar largura proporcional para lista padrão
	_be_relatorio_largura_proporcional (Relatorio, Celula);
	//lista de estilos
	sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "style");
	BE_free (sTemp);
	sTemp = g_strdup (sValor);
	ListaEstilos = be_strsplit (&sTemp,";");

	for (ListaL = ListaEstilos; ListaL; ListaL = g_slist_next (ListaL))
	{
		aValor = g_strsplit ((gchar *) ListaL->data, ":", -1);
		//espessura da borda
		if (g_str_equal ("stroke-width", aValor[0]))
		{
			Celula->fBorEspessura = _be_relatorio_conversao (aValor[1]);
		}
		//pontilhamento da borda
		else if (g_str_equal ("stroke-dasharray", aValor[0]))
		{
			if (!g_str_equal ("none", aValor[1]))
			{
				aTemp = g_strsplit (aValor[1], " ", -1);
				Celula->nBorPontilhado = _be_relatorio_conversao (aTemp[1]);
				BE_strfreev (aTemp);
			}
		}
		//cor da borda
		else if (!bInvisivel && g_str_equal ("stroke", aValor[0]))
		{
			BE_free (Celula->sCorBorda);
			if (!g_str_equal ("none", aValor[1]))
				Celula->sCorBorda = g_strdup (aValor[1]+1);//não enviar #
		}
		//cor de fundo
		else if (!bInvisivel && g_str_equal ("fill", aValor[0]))
		{
			BE_free (Celula->sCorFundo);
			if (!g_str_equal ("none", aValor[1]))
				Celula->sCorFundo = g_strdup (aValor[1]+1);//não enviar #
		}
		//verificar transparencia, não suportado, ainda
		else if (g_strrstr (aValor[0], "opacity"))
		{
#ifndef BERELATORIO_DEPURAR
			if (g_strtod (aValor[1], NULL) < 1.0)
			{
				//sem bordas ou fundo
				Celula->nBorPosicao = BEPDF_BOR_NENHUMA;
				BE_free (Celula->sCorBorda);
				BE_free (Celula->sCorFundo);
				bInvisivel = TRUE;
			}
#endif
		}
		//limpar
		BE_strfreev (aValor);
	}

	//limpar
	BE_slist_free (ListaEstilos);
	BE_free (sTemp);

	return Celula;
}

gboolean _be_relatorio_loop (GNode *node, gpointer data)
{
	GNode *NoFilho = NULL, *NoTexto = NULL, *NoCaixa = NULL, *NoScript = NULL;
	BEXML_No_Elemento *NoElemento = NULL, *NoElementoFilho = NULL, *NoElementoTexto = NULL;
	BEXML_No_Atributo *NoAtributo = NULL;
	_BERELATORIO *Relatorio = NULL;
	_BERELATORIO_Dados *Dados = NULL;
	BEPDF *PDF = NULL;
	BEPDF_Celula *Celula = NULL, *CelulaCaixa = NULL;
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	GdkPixbuf *Imagem = NULL;
	gchar *sFonte = NULL, *sCor = NULL;
	gfloat fXTransf = 0.0, fYTransf = 0.0, fLTransf = 0.0, fATransf = 0.0;
	gfloat fTamanho = 0.0, fLinha = 100.0, fY = 0.0, fLarguraTexto = 0.0, fAlturaTexto = 0.0;
	const gchar *sValor = NULL;
	GSList *ListaEstilos = NULL, *ListaL = NULL;
	gchar **aValor = NULL, **aTemp = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL;
	GString *Buffer = NULL;
	gboolean bNegrito = FALSE, bItalico = FALSE;
	gboolean bInvisivel = FALSE;
	gint nCont = 0;

	NoElemento = (BEXML_No_Elemento *) node->data;
	Relatorio = (_BERELATORIO *) data;
	PDF = Relatorio->PDF;
	Sintaxe = Relatorio->Sintaxe;
	//estrutura de modelos: pág. 96

	//linhas
	if (g_str_equal (NoElemento->sNome, "path"))
	{
		//nova célula
		Celula = be_pdf_nova_celula (PDF);
		Celula->nLocal = Relatorio->nLocal;
		Celula->nBorPosicao = BEPDF_BOR_INCLINADA1;
		Celula->nPagina = Relatorio->nPagina;
		//posições
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "d");
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp = g_strdup (sValor+2);//pular 'M '
		sTemp1 = sTemp;
		sTemp = be_strreplace (sTemp1, " L ", ",");
		aTemp = g_strsplit (sTemp, ",", -1);
		Celula->fX = _be_relatorio_conversao (aTemp[0]);
		Celula->fY = _be_relatorio_conversao (aTemp[1]) + Relatorio->fYRelativo;
		if (aTemp[2])
			fLTransf = _be_relatorio_conversao (aTemp[2]) - Celula->fX;
		else
			fLTransf = 0;
		if (aTemp[2] && aTemp[3])
			fATransf = _be_relatorio_conversao (aTemp[3]) - Celula->fY;
		else
			fATransf = 0;
		//transformações
		_be_relatorio_transformacoes (node, &Celula->fX, &Celula->fY, &fLTransf, &fATransf, NULL);
		Celula->fLargura = fLTransf;
		Celula->fAltura = fATransf;

		BE_strfreev (aTemp);
		//lista de estilos
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "style");
		BE_free (sTemp);
		sTemp = g_strdup (sValor);
		ListaEstilos = be_strsplit (&sTemp,";");
		for (ListaL = ListaEstilos; ListaL; ListaL = g_slist_next (ListaL))
		{
			aValor = g_strsplit ((gchar *) ListaL->data, ":", -1);
			//espessura da borda
			if (g_str_equal ("stroke-width", aValor[0]))
			{
				Celula->fBorEspessura = _be_relatorio_conversao (aValor[1]);
			}
			//pontihamento da borda
			else if (g_str_equal ("stroke-dasharray", aValor[0]))
			{
				if (!g_str_equal ("none", aValor[1]))
				{
					aTemp = g_strsplit (aValor[1], " ", -1);
					Celula->nBorPontilhado = _be_relatorio_conversao (aTemp[1]);
					BE_strfreev (aTemp);
				}
			}
			//cor da borda
			else if (!bInvisivel && g_str_equal ("stroke", aValor[0]))
			{
				BE_free (Celula->sCorBorda);
				if (!g_str_equal ("none", aValor[1]))
					Celula->sCorBorda = g_strdup (aValor[1]+1);//não enviar #
			}
			//cor de fundo
			else if (!bInvisivel && g_str_equal ("fill", aValor[0]))
			{
				BE_free (Celula->sCorFundo);
				if (!g_str_equal ("none", aValor[1]))
					Celula->sCorFundo = g_strdup (aValor[1]+1);//não enviar #
			}
			//verificar transparencia, não suportado
			else if (g_str_equal ("stroke-opacity", aValor[0]) || g_str_equal ("opacity", aValor[0]))
			{
#ifndef BERELATORIO_DEPURAR
				if (g_strtod (aValor[1], NULL) < 1.0)
				{
						//sem bordas ou fundo
					Celula->nBorPosicao = BEPDF_BOR_NENHUMA;
					BE_free (Celula->sCorBorda);
					BE_free (Celula->sCorFundo);
					bInvisivel = TRUE;
				}
#endif
			}
			//limpar
			BE_strfreev (aValor);
		}
		BE_slist_free (ListaEstilos);
	}
	//retângulos
	else if (g_str_equal (NoElemento->sNome, "rect"))
	{
		//procurar por um irmão texto
		NoTexto = be_xml_no_nome (node->next, "text", BEXML_LOCALIZAR_IRMAO);
		if (!NoTexto)
			Celula = _be_relatorio_retangulo (node, Relatorio);
	}
	//texto
	else if (g_str_equal (NoElemento->sNome, "text"))
	{
		//procurar por retângulo irmão, agrupado ao texto
		NoCaixa = be_xml_no_nome (node->parent, "g", BEXML_LOCALIZAR_PAI);
		NoCaixa = be_xml_no_nome (NoCaixa->children, "rect", BEXML_LOCALIZAR_IRMAO);
		if (NoCaixa)
			CelulaCaixa = _be_relatorio_retangulo (NoCaixa, Relatorio);
		//nova célula
		Celula = be_pdf_nova_celula (PDF);
		Celula->nLocal = Relatorio->nLocal;
		Celula->nPagina = Relatorio->nPagina;
#ifndef BERELATORIO_DEPURAR
		//sem bordas ou fundo
		Celula->nBorPosicao = BEPDF_BOR_NENHUMA;
		BE_free (Celula->sCorBorda);
		BE_free (Celula->sCorFundo);
#endif
		//lista de estilos
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "style");
		BE_free (sTemp);
		sTemp = g_strdup (sValor);
		ListaEstilos = be_strsplit (&sTemp,";");
		for (ListaL = ListaEstilos; ListaL; ListaL = g_slist_next (ListaL))
		{
			aValor = g_strsplit ((gchar *) ListaL->data, ":", -1);
			//nome da fonte
			if (g_str_equal ("font-family", aValor[0]))
				sFonte = g_strdup (aValor[1]);
			//negrito
			else if (g_str_equal ("font-weight", aValor[0]) && !g_str_equal ("normal", aValor[1]))
				bNegrito = TRUE;
			//itálico
			else if (g_str_equal ("font-style", aValor[0]) && !g_str_equal ("normal", aValor[1]))
				bItalico = TRUE;
			//cor da fonte
			else if (g_str_equal ("fill", aValor[0]))
			{
				if (!g_str_equal ("none", aValor[1]))
					sCor = g_strdup (aValor[1]+1);//não enviar #
			}
			//tamanho da fonte
			else if (g_str_equal ("font-size", aValor[0]))
			{
				fTamanho = _be_relatorio_conversao (aValor[1]);
				//conversão de transformações
				_be_relatorio_transformacoes (node, NULL, NULL, NULL, NULL, &fTamanho);
			}
			//proporção da linha
			else if (g_str_equal ("line-height", aValor[0]))
				fLinha = _be_relatorio_conversao (aValor[1]);
			else if (g_str_equal ("text-align", aValor[0]))
			{
				if (g_str_equal ("center", aValor[1]))
					Celula->nAlinhamento = BEPDF_ALI_CENTRO;
				else if (g_str_equal ("end", aValor[1]))
					Celula->nAlinhamento = BEPDF_ALI_DIREITO;
			}
			//limpar
			BE_strfreev (aValor);
		}
		BE_slist_free (ListaEstilos);
		//forçar alterações na fonte, para linha de totais
		if (Relatorio->lLinha == -1)
		{
			bItalico = TRUE;
			bNegrito = TRUE;
			fTamanho /= 1.30;//diminuir 30%
		}
		//verificar cor
		if (!sCor)
			sCor = g_strdup ("000000");//preto
		//verificar influência de retângulo irmão
		if (CelulaCaixa)
		{
			//posições
			Celula->fX = CelulaCaixa->fX;
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
			fXTransf = _be_relatorio_conversao (sValor);
			Celula->fY = CelulaCaixa->fY;
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
			fYTransf = _be_relatorio_conversao (sValor) + Relatorio->fYRelativo;
			//dimensões
			Celula->fLargura = CelulaCaixa->fLargura;
			Celula->fAltura = CelulaCaixa->fAltura;
			//transformações
			_be_relatorio_transformacoes (node, &fXTransf, &fYTransf, NULL, NULL, NULL);
			//margem Y do texto
			Celula->fYTexto = fYTransf - Celula->fY;
			//margem X do texto
			if (Celula->nAlinhamento & BEPDF_ALI_CENTRO)
				Celula->fXTexto = 0.0;
			else if (Celula->nAlinhamento & BEPDF_ALI_DIREITO)
				Celula->fXTexto = (fXTransf - Celula->fX) - Celula->fLargura;
			else
				Celula->fXTexto = fXTransf - Celula->fX;
		}
		else
		{
			//posições
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
			Celula->fX = _be_relatorio_conversao (sValor);
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
			Celula->fY = _be_relatorio_conversao (sValor) + Relatorio->fYRelativo;
			//transformações
			_be_relatorio_transformacoes (node, &Celula->fX, &Celula->fY, NULL, NULL, NULL);
			//verificar largura proporcional para lista padrão
			_be_relatorio_largura_proporcional (Relatorio, Celula);
		}
		//textos
		if (sFonte && fTamanho > 0.0)
		{
			//iniciar buffer
			Buffer = g_string_new ("");
			if (bNegrito && bItalico)
				g_string_append_printf (Buffer, "<font face='%s' style='%s' size='%f' color='%s'>", sFonte, "negrito italico", fTamanho, sCor);
			else if (bNegrito)
				g_string_append_printf (Buffer, "<font face='%s' style='%s' size='%f' color='%s'>", sFonte, "negrito", fTamanho, sCor);
			else if (bItalico)
				g_string_append_printf (Buffer, "<font face='%s' style='%s' size='%f' color='%s'>", sFonte, "italico", fTamanho, sCor);
			else
				g_string_append_printf (Buffer, "<font face='%s' size='%f' color='%s'>", sFonte, fTamanho, sCor);

			//linhas de textos
			NoFilho = node->children;
			if (NoFilho)
			{
				while (NoFilho)
				{
					//texto
					NoElementoFilho = (BEXML_No_Elemento *) NoFilho->data;
					//substituir por dados
					if (Relatorio->nLocal == BEPDF_LOC_CORPO || Relatorio->nLocal == BERELATORIO_LOC_FORMULARIOS || Relatorio->nLocal == BERELATORIO_LOC_ROTULOS || Relatorio->nLocal == BERELATORIO_LOC_LISTAS || Relatorio->nLocal == BERELATORIO_LOC_INDICES || Relatorio->nLocal == BERELATORIO_LOC_SUBROTULOS || Relatorio->nLocal == BERELATORIO_LOC_SUBLISTAS || Relatorio->nLocal == BERELATORIO_LOC_SUBINDICES)
					{
						BE_free (sTemp);
						//verificar imagens
						if (Relatorio->Campo && be_sql_campo_binario (Relatorio->Campo) && g_str_equal (NoElementoFilho->sValor, "$valores") && CelulaCaixa)
						{
							Imagem = be_sql_recuperar_arquivo_pixbuf (Relatorio->Campo, 'I');
							//imagem
							if (Imagem)
							{
								Relatorio->Imagens = g_slist_append (Relatorio->Imagens, Imagem);
								CelulaCaixa->Imagem = be_pdf_nova_imagem (PDF, Imagem, TRUE);
								if ((gfloat) gdk_pixbuf_get_width (Imagem) > CelulaCaixa->fLargura)
									CelulaCaixa->fAltura = (gfloat) gdk_pixbuf_get_height (Imagem) * (CelulaCaixa->fLargura / (gfloat) gdk_pixbuf_get_width (Imagem));
								else
								{
									CelulaCaixa->fLargura = (gfloat) gdk_pixbuf_get_width (Imagem);
									CelulaCaixa->fAltura = (gfloat) gdk_pixbuf_get_height (Imagem);
								}
								sTemp = g_strdup ("");//sem textos
							}
							//arquivo
							else
							{
								BE_free (sTemp1);
								sTemp1 = be_sql_campo_valor_limitado_formatado (Relatorio->Campo, BERELATORIO_LENSTRING_BIN, BESQL_CAMPOS_LIMITAR_FORMATO_XML);
								if (!sTemp1)
									sTemp1 = g_strdup ("");
								if (Relatorio->Campo->nComprimento > 1073741824)
									sTemp = g_strdup_printf ("%s (%.3f GB)", sTemp1, (gfloat) Relatorio->Campo->nComprimento / 1073741824.0);
								else if (Relatorio->Campo->nComprimento > 1048576)
									sTemp = g_strdup_printf ("%s (%.2f MB)", sTemp1, (gfloat) Relatorio->Campo->nComprimento / 1048576.0);
								else if (Relatorio->Campo->nComprimento > 1024)
									sTemp = g_strdup_printf ("%s (%.1f KB)", sTemp1, (gfloat) Relatorio->Campo->nComprimento / 1204.0);
								else
									sTemp = g_strdup_printf ("%s (%d B)", sTemp1, Relatorio->Campo->nComprimento);
							}
						}
						else//dados
						{
							sTemp = _be_relatorio_dados (Relatorio, NoElementoFilho->sValor);
						}
						//exibir
						BE_free (sTemp1);
						sTemp1 = sTemp;
						sTemp = be_strreplace (sTemp1, "\n", "<br>");//converter enter \n em <br>
						g_string_append_printf (Buffer, "%s<br>", sTemp);
					}
					//substituir páginas
					else if (Relatorio->nLocal == BERELATORIO_LOC_PAGINAS)
					{
						//página total, somente uma vez
						BE_free (sTemp);
						BE_free (sTemp1);
						sTemp = NoElementoFilho->sValor;
						sTemp1 = g_strdup_printf ("%d", Relatorio->nPaginas);
						NoElementoFilho->sValor = be_strreplace (sTemp, "$P", sTemp1);

						//página atual
						BE_free (sTemp);
						BE_free (sTemp1);
						sTemp1 = g_strdup_printf ("%d", Relatorio->nPagina);
						sTemp = be_strreplace (NoElementoFilho->sValor, "$p", sTemp1);
						//exibir
						g_string_append_printf (Buffer, "%s<br>", sTemp);
					}
					else
						//exibir
						g_string_append_printf (Buffer, "%s<br>", NoElementoFilho->sValor);

					//próximo irmão
					NoFilho = NoFilho->next;
				}
				//excluir a última quebra de linha
				Buffer = g_string_truncate (Buffer, Buffer->len - 4);
			}
			//finalizar linhas
			Buffer = g_string_append (Buffer, "</font>");
			//texto da célula
			BE_free (sTemp);
			sTemp = BE_string_free (Buffer);
#ifdef BERELATORIO_DEPURAR
			printf ("%s\n", sTemp);
#endif
			//substituir variáveis
			BE_free (sTemp1);
			sTemp1 = _be_relatorio_variaveis (Relatorio, sTemp);
			if (sTemp1)
			{
				BE_free (sTemp);
				sTemp = sTemp1;
				sTemp1 = NULL;
			}
			//texto
			if (Celula->fLargura > 0.0)
				Celula->NoTexto = be_texto_analisar (PDF->Texto, sTemp, Celula->fLargura, 100.0, TRUE, FALSE);
			else
			{
				Celula->NoTexto = be_texto_analisar (PDF->Texto, sTemp, 200.0, 100.0, TRUE, FALSE);
			}
			//dimensões reais do texto
			if (Celula->NoTexto)
			{
				NoElemento = (BEXML_No_Elemento *) Celula->NoTexto->children->data;
				fLarguraTexto = g_strtod (be_xml_no_atributos_valor (NoElemento->Atributos, "largura"), NULL);
				fAlturaTexto = g_strtod (be_xml_no_atributos_valor (NoElemento->Atributos, "altura"), NULL);
			}
			//redimensionar para tamanho do texto, se não houver irmão retângulo
			if (!CelulaCaixa)
			{
				Celula->fLargura = fLarguraTexto;
				Celula->fAltura = fAlturaTexto;
			}
			//localizar a altura da primeira linha
			if (Celula->NoTexto)
			{
				NoTexto = Celula->NoTexto->children;//bloco
				NoTexto = NoTexto->next;//font
				NoTexto = NoTexto->next;//linha
				NoElementoTexto = (BEXML_No_Elemento *) NoTexto->data;
				fY = g_strtod (be_xml_no_atributos_valor (NoElementoTexto->Atributos, "yMax"), NULL);
			}
			//correção de posições
			if (CelulaCaixa)
			{
				Celula->fYTexto -= fY;
				//dimensões: verificar se o texto cabe na caixa
				if (fAlturaTexto + Celula->fYTexto > Celula->fAltura)//não cabe
				{
					Celula->fAltura = fAlturaTexto + Celula->fYTexto;
					if (Celula->fAltura > CelulaCaixa->fAltura)
					{
						Relatorio->fYRelativo += (float) (int) ((Celula->fAltura - CelulaCaixa->fAltura) + 1.0);//marcar diferença, para empurar os outros para baixo, arredondar para inteiros
						CelulaCaixa->fAltura = (float) (int) (Celula->fAltura + 1.0);//redimensionar altura
					}
					//derconsiderar se não for para dados
					if (Relatorio->nLocal != BEPDF_LOC_CORPO)
						Relatorio->fYRelativo = 0.0;
				}
			}
			else
				Celula->fY -= fY;
		}
	}
	//imagens
	else if (g_str_equal (NoElemento->sNome, "image"))
	{
		//nova célula
		Celula = be_pdf_nova_celula (PDF);
		Celula->nLocal = Relatorio->nLocal;
		Celula->nPagina = Relatorio->nPagina;
		//posições
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "x");
		Celula->fX = _be_relatorio_conversao (sValor);
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "y");
		Celula->fY = _be_relatorio_conversao (sValor) + Relatorio->fYRelativo;
		//dimensões
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "width");
		Celula->fLargura = _be_relatorio_conversao (sValor);
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "height");
		Celula->fAltura = _be_relatorio_conversao (sValor);
		//transformações
		_be_relatorio_transformacoes (node, &Celula->fX, &Celula->fY, &Celula->fLargura, &Celula->fAltura, NULL);
		//verificar logotipo
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "id");
		if (sValor && Relatorio->EmpresaCliente && g_str_equal (sValor, "logotipo") && Relatorio->EmpresaCliente->Logotipo)
			Celula->Imagem = be_pdf_nova_imagem (PDF, Relatorio->EmpresaCliente->Logotipo, TRUE);
		else//imagem externa ou dados
		{
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "xlink:href");
			//verificar se a imagem é local
			BE_free (sTemp);
			sTemp = g_path_get_dirname (sValor);
			if (g_str_equal (sTemp, "."))
			{
				BE_free (sTemp1);
				sTemp1 = g_path_get_dirname (Relatorio->sArquivoSVG);
				if (!g_str_equal (sTemp1, "."))
				{
					BE_free (sTemp);
					sTemp = g_strdup_printf ("%s%s%s", sTemp1, G_DIR_SEPARATOR_S, sValor);
					sValor = sTemp;
				}
			}
			//dados
			Imagem = _be_relatorio_dados_imagem (Relatorio, sValor);
			//imagem externa
			if (!Imagem)
				Imagem = gdk_pixbuf_new_from_file (sValor, NULL);
			if (Imagem)
			{
				Relatorio->Imagens = g_slist_append (Relatorio->Imagens, Imagem);
				Celula->Imagem = be_pdf_nova_imagem (PDF, Imagem, TRUE);
			}
		}
		//lista de estilos
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "style");
		BE_free (sTemp);
		sTemp = g_strdup (sValor);
		ListaEstilos = be_strsplit (&sTemp,";");
		for (ListaL = ListaEstilos; ListaL; ListaL = g_slist_next (ListaL))
		{
			aValor = g_strsplit ((gchar *) ListaL->data, ":", -1);
			//cor da borda
			if (g_str_equal ("stroke", aValor[0]))
			{
				BE_free (Celula->sCorBorda);
				if (!g_str_equal ("none", aValor[1]))
					Celula->sCorBorda = g_strdup (aValor[1]+1);//não enviar #
			}
			//cor de fundo
			else if (g_str_equal ("fill", aValor[0]))
			{
				BE_free (Celula->sCorFundo);
				if (!g_str_equal ("none", aValor[1]))
					Celula->sCorFundo = g_strdup (aValor[1]+1);//não enviar #
			}
			//limpar
			BE_strfreev (aValor);
		}
		BE_slist_free (ListaEstilos);
	}
	//processar scripts em Python
	else if (g_str_equal (NoElemento->sNome, "python"))
	{
		//recuperar script
		sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "py");
		//printf ("Script original py = %s\n", sValor);
		if (sValor && Sintaxe)
		{
			BE_free (sTemp);
			sTemp = g_strdup (sValor);
			//loop de todos campos, substituir os campos por seus valores atuais
			for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				//localizar e substituir o valor do campo
				BE_free (sTemp1);
				BE_free (sTemp2);
				sTemp1 = g_strdup_printf ("[%s]", Campo->sNome);//nome do campo
				//alterar
				if (Campo->sValor)
					sTemp2 = be_strreplace (sTemp, sTemp1, Campo->sValor);
				else
					sTemp2 = be_strreplace (sTemp, sTemp1, "")
				BE_free (sTemp);
				sTemp = g_strdup (sTemp2);
				//localizar e substituir \\n por \n
				BE_free (sTemp1);
				sTemp1 = be_strreplace (sTemp, "\\n", "\n");
				//localizar e substituir \\t por \t
				BE_free (sTemp);
				sTemp = be_strreplace (sTemp1, "\\t", "\t");
			}
			//printf ("Script py = %s\n", sTemp);
			//processar script
			if (Relatorio->pyModulo)
			{
				//execução do script
				PyRun_String (sTemp, Py_single_input, Relatorio->pyModulo, Relatorio->pyModulo);
				//recuperar variáveis: loop
				nCont = 1;
				BE_free (sTemp);
				sTemp = g_strdup_printf ("Objeto%d", nCont);
				while ((Relatorio->pyScript = PyDict_GetItemString (Relatorio->pyModulo, sTemp)))
				{
					//printf ("Objeto%d %s\n", nCont, PyString_AsString (PyObject_Str (Relatorio->pyScript)));
					//verificar nome do objeto
					if (be_xml_no_atributos_valor (NoElemento->Atributos, sTemp))
					{
						//localizar objeto
						if ((NoScript = be_xml_no_localizar_elemento (node->parent, NULL, "id", be_xml_no_atributos_valor (NoElemento->Atributos, sTemp))))
						{
							NoElementoFilho = (BEXML_No_Elemento *) NoScript->data;
							//printf ("Objeto%d: %s\n", nCont, NoElementoFilho->sNome);
							//verificar atributo
							BE_free (sTemp);
							sTemp = g_strdup_printf ("Atributo%d", nCont);
							if (be_xml_no_atributos_valor (NoElemento->Atributos, sTemp))
							{
								//printf ("Atributo%d: %s\n", nCont, be_xml_no_atributos_valor (NoElemento->Atributos, sTemp));
								//loop dos atributos
								for (ListaL = NoElementoFilho->Atributos; ListaL; ListaL = g_slist_next (ListaL))
								{
									NoAtributo = (BEXML_No_Atributo *) ListaL->data;
									//localizar atributo e substituir valor
									if (g_str_equal (NoAtributo->sNome, be_xml_no_atributos_valor (NoElemento->Atributos, sTemp)))
									{
										BE_free (NoAtributo->sValor);
										NoAtributo->sValor = g_strdup (PyString_AsString (PyObject_Str (Relatorio->pyScript)));
									}
								}
							}
							//verificar estilo
							BE_free (sTemp);
							sTemp = g_strdup_printf ("Estilo%d", nCont);
							if (be_xml_no_atributos_valor (NoElemento->Atributos, sTemp))
							{
								//printf ("Propriedade%d: %s\n", nCont, be_xml_no_atributos_valor (NoElemento->Atributos, sTemp));
								//loop dos atributos
								for (ListaL = NoElementoFilho->Atributos; ListaL; ListaL = g_slist_next (ListaL))
								{
									NoAtributo = (BEXML_No_Atributo *) ListaL->data;
									//localizar estilo
									if (g_str_equal (NoAtributo->sNome, "style"))
									{
										//preparar intervalo de busca
										BE_free (sTemp1);
										sTemp1 = g_strdup_printf (";%s:", be_xml_no_atributos_valor (NoElemento->Atributos, sTemp));
										//recuperar intervalo
										BE_free (sTemp);
										sTemp = be_strextract (NoAtributo->sValor, sTemp1, ";", 0);
										//alterar propriedade
										if (sTemp)
										{
											BE_free (sTemp1);
											sTemp1 = NoAtributo->sValor;
											NoAtributo->sValor = be_strreplace (sTemp1, sTemp, PyString_AsString (PyObject_Str (Relatorio->pyScript)));
											//printf ("%s => %s\n", sTemp, PyString_AsString (PyObject_Str (Relatorio->pyScript)));
										}
									}
								}
					
							}
						}
					}
					//preparar busca da próxima variável
					nCont++;
					BE_free (sTemp);
					sTemp = g_strdup_printf ("Objeto%d", nCont);					
				}
			}
		}
	}
	//espaço dos dados, marcar para futuro posicionamento
	if (Relatorio->DadosAtual)
	{
		if (Celula)
		{
			Dados = Relatorio->DadosAtual;
			Dados->Celulas = g_slist_append (Dados->Celulas, Celula);
			//dimensões atuais
			if ((Celula->fX + Celula->fLargura) - Relatorio->fXMinAtual > Dados->fLargura)
				Dados->fLargura = (Celula->fX + Celula->fLargura) - Relatorio->fXMinAtual;
			if ((Celula->fY + Celula->fAltura) - Relatorio->fYMinAtual > Dados->fAltura)
				Dados->fAltura = (Celula->fY + Celula->fAltura) - Relatorio->fYMinAtual;
		}
		if (CelulaCaixa)
		{
			Celula = CelulaCaixa;
			Dados = Relatorio->DadosAtual;
			Dados->Celulas = g_slist_append (Dados->Celulas, Celula);
			//dimensões atuais
			if ((Celula->fX + Celula->fLargura) - Relatorio->fXMinAtual > Dados->fLargura)
				Dados->fLargura = (Celula->fX + Celula->fLargura) - Relatorio->fXMinAtual;
			if ((Celula->fY + Celula->fAltura) - Relatorio->fYMinAtual > Dados->fAltura)
				Dados->fAltura = (Celula->fY + Celula->fAltura) - Relatorio->fYMinAtual;
		}
	}
	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sFonte);
	BE_free (sCor);

	return FALSE;
}

void _be_relatorio_largura_proporcional (_BERELATORIO *Relatorio, BEPDF_Celula *Celula)
{
	//verificar largura proporcional para lista padrão
	if (Celula->nLocal == BERELATORIO_LOC_ROTULOS || Celula->nLocal == BERELATORIO_LOC_LISTAS)
	{
		Celula->fLargura = Relatorio->fLarguraMaxListas * Relatorio->fLarguraProporcional;
		Celula->fLargura -= (Celula->fX - Relatorio->fXMinListas);//descontar o início relativo
		Celula->fLargura -= 0.0001;//correção de arredondamento de casas decimais
	}
	//verificar largura proporcional para sublista padrão
	else if (Celula->nLocal == BERELATORIO_LOC_SUBROTULOS || Celula->nLocal == BERELATORIO_LOC_SUBLISTAS)
	{
		Celula->fLargura = Relatorio->fLarguraMaxSubListas * Relatorio->fLarguraProporcional;
		Celula->fLargura -= (Celula->fX - Relatorio->fXMinSubListas);//descontar o início relativo
		Celula->fLargura -= 0.0001;//correção de arredondamento de casas decimais
	}
}

gchar *_be_relatorio_dados (_BERELATORIO *Relatorio, const gchar *sValor)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL, *sTemp1 = NULL, *sTemp2 = NULL, *sTemp3 = NULL;
	gchar *sNovo = NULL;

	Sintaxe = Relatorio->Sintaxe;
	if (Sintaxe)
	{
		//numeração da linha (registro)
		if (Relatorio->lLinha == -1)//totais
			sTemp = g_strdup ("Totais");
		else//numeração das linhas
			sTemp = g_strdup_printf ("%d", (gint) Relatorio->lLinha);
		sNovo = be_strreplace (sValor, "$r", sTemp);
		//relatório padrão já indicou o campo
		if (Relatorio->Campo)
		{
			Campo = Relatorio->Campo;
			//localizar e substituir o nome do campo
			BE_free (sTemp);
			BE_free (sTemp1);
			sTemp = sNovo;
			sNovo = be_strreplace (sTemp, "$campos", Campo->sNome);
			//localizar e substituir o valor do campo
			BE_free (sTemp);
			BE_free (sTemp1);
			BE_free (sTemp2);
			sTemp = sNovo;
			if (Campo->sValor && Relatorio->lLinha > 0)//valor do campo
				sTemp1 = be_sql_campo_valor_limitado_formatado (Campo, BERELATORIO_LENSTRING, BESQL_CAMPOS_LIMITAR_FORMATO_XML);
			else if (Campo->sTotal && Relatorio->lLinha == -1)//total do campo
				sTemp1 = g_strdup (Campo->sTotal);
			else//sem valor
				sTemp1 = g_strdup ("");
			//alterar
			sNovo = be_strreplace (sTemp, "$valores", sTemp1);
		}
		//não há indicações, procurar por todos os campos
		else
		{
			for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
			{
				Campo = (BESQL_Campo *) ListaL->data;
				if (Campo->bAtivo)
				{
					//localizar e substituir o valor do campo
					BE_free (sTemp);
					BE_free (sTemp1);
					BE_free (sTemp2);
					BE_free (sTemp3);
					sTemp = g_strdup_printf ("[%s]", Campo->sNome);//nome do campo
					BE_free (sTemp1);
					sTemp1 = sNovo;
					if (Campo->sValor &&  Relatorio->lLinha > 0)//valor do campo
						sTemp2 = be_sql_campo_valor_limitado_formatado (Campo, BERELATORIO_LENSTRING, BESQL_CAMPOS_LIMITAR_FORMATO_XML);
					else if (Campo->sTotal && Relatorio->lLinha == -1)//total do campo
						sTemp2 = g_strdup (Campo->sTotal);
					else//sem valor
						sTemp2 = g_strdup ("");
					//alterar
					sNovo = be_strreplace (sTemp1, sTemp, sTemp2);
				}
			}
		}
	}
	else
		sNovo = g_strdup (sValor);

	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);
	BE_free (sTemp2);
	BE_free (sTemp3);

	return sNovo;
}

GdkPixbuf *_be_relatorio_dados_imagem (_BERELATORIO *Relatorio, const gchar *sValor)
{
	BESQL_Sintaxe *Sintaxe = NULL;
	BESQL_Campo *Campo = NULL;
	GdkPixbuf *Imagem = NULL;
	GSList *ListaL = NULL;
	gchar *sTemp = NULL;

	Sintaxe = Relatorio->Sintaxe;
	if (Sintaxe)
	{
		for (ListaL = Sintaxe->Campos; ListaL; ListaL = g_slist_next (ListaL))
		{
			Campo = (BESQL_Campo *) ListaL->data;
			if (Campo->bAtivo)
			{
				//carregar
				BE_free (sTemp);
				sTemp = g_strdup_printf ("[%s]", Campo->sNome);//nome do campo
				if (g_str_equal (sValor, sTemp))
					Imagem = be_sql_recuperar_arquivo_pixbuf (Campo, 'I');
			}
		}
	}
	BE_free (sTemp);

	return Imagem;
}

gchar *_be_relatorio_variaveis (_BERELATORIO *Relatorio, gchar *sTexto)
{
	BEPDF *PDF = NULL;
	BESQL_Sintaxe *Sintaxe;
	gchar *sTemp = NULL, *sTemp1 = NULL;
	gint nTemp = 0;
	gchar *sValor = NULL;

	Sintaxe = Relatorio->Sintaxe;
	PDF = Relatorio->PDF;
	//verificar valor
	if (sTexto)
	{
		sValor = g_strdup (sTexto);
		//dia
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%d");
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$d", sTemp1);
		//dia da semana
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%w");
		sTemp = sValor;
			//conversão
			nTemp = (gint) g_strtod (sTemp1, NULL);
			BE_free (sTemp1);
			switch (nTemp)
			{
				case 0: sTemp1 = g_strdup ("Domingo"); break;
				case 1: sTemp1 = g_strdup ("Segunda-feira"); break;
				case 2:
					sTemp1 = BE_UTF8_ISO ("Terça-feira");
					break;
				case 3: sTemp1 = g_strdup ("Quarta-feira"); break;
				case 4: sTemp1 = g_strdup ("Quinta-feira"); break;
				case 5: sTemp1 = g_strdup ("Sexta-feira"); break;
				case 6:
					sTemp1 = BE_UTF8_ISO ("Sábado");
					break;
			}
		sValor = be_strreplace (sTemp, "$w", sTemp1);
		//mês
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%m");
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$m", sTemp1);
		//ano completo
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%Y");
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$Y", sTemp1);
		//hora
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%H");
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$H", sTemp1);
		//minuto
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%M");
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$M", sTemp1);
		//segundo
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = BE_tempo ("%S");
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$S", sTemp1);

		//autor
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = g_strdup_printf ("%s %s", PRODUCT_NAME, PRODUCT_VERSION);
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$produto", sTemp1);
		//relatório
		BE_free (sTemp);
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$relatorio", Relatorio->sRelatorio);
		if (Relatorio->sSubRelatorio)
		{
			BE_free (sTemp);
			sTemp = sValor;
			sValor = be_strreplace (sTemp, "$subrelatorio", Relatorio->sSubRelatorio);
		}
		//usuário
		BE_free (sTemp);
		BE_free (sTemp1);
		sTemp1 = be_sql_informacoes (Sintaxe->Tabela->BD->SQL, BESQL_INFO_USUARIO, NULL);
		sTemp = sValor;
		sValor = be_strreplace (sTemp, "$usuario", sTemp1);

		//empresa cliente
		if (Relatorio->EmpresaCliente)
		{
			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sNomeFantasia)
				sValor = be_strreplace (sTemp, "$nomefantasia", Relatorio->EmpresaCliente->sNomeFantasia);
			else
				sValor = be_strreplace (sTemp, "$nomefantasia", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sRazaoSocial)
				sValor = be_strreplace (sTemp, "$razaosocial", Relatorio->EmpresaCliente->sRazaoSocial);
			else
				sValor = be_strreplace (sTemp, "$razaosocial", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sCNPJ)
				sValor = be_strreplace (sTemp, "$cnpj", Relatorio->EmpresaCliente->sCNPJ);
			else
				sValor = be_strreplace (sTemp, "$cnpj", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sIE)
				sValor = be_strreplace (sTemp, "$ie", Relatorio->EmpresaCliente->sIE);
			else
				sValor = be_strreplace (sTemp, "$ie", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sTelefone)
				sValor = be_strreplace (sTemp, "$telefone", Relatorio->EmpresaCliente->sTelefone);
			else
				sValor = be_strreplace (sTemp, "$telefone", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sFax)
				sValor = be_strreplace (sTemp, "$fax", Relatorio->EmpresaCliente->sFax);
			else
				sValor = be_strreplace (sTemp, "$fax", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sPABX)
				sValor = be_strreplace (sTemp, "$pabx", Relatorio->EmpresaCliente->sPABX);
			else
				sValor = be_strreplace (sTemp, "$pabx", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sEmail)
				sValor = be_strreplace (sTemp, "$email", Relatorio->EmpresaCliente->sEmail);
			else
				sValor = be_strreplace (sTemp, "$email", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sSite)
				sValor = be_strreplace (sTemp, "$site", Relatorio->EmpresaCliente->sSite);
			else
				sValor = be_strreplace (sTemp, "$site", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sCep)
				sValor = be_strreplace (sTemp, "$cep", Relatorio->EmpresaCliente->sCep);
			else
				sValor = be_strreplace (sTemp, "$cep", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sTipoLogradouro)
				sValor = be_strreplace (sTemp, "$tipologradouro", Relatorio->EmpresaCliente->sTipoLogradouro);
			else
				sValor = be_strreplace (sTemp, "$tipologradouro", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sLogradouro)
				sValor = be_strreplace (sTemp, "$logradouro", Relatorio->EmpresaCliente->sLogradouro);
			else
				sValor = be_strreplace (sTemp, "$logradouro", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sComplemento)
				sValor = be_strreplace (sTemp, "$complemento", Relatorio->EmpresaCliente->sComplemento);
			else
				sValor = be_strreplace (sTemp, "$complemento", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sBairro)
				sValor = be_strreplace (sTemp, "$bairro", Relatorio->EmpresaCliente->sBairro);
			else
				sValor = be_strreplace (sTemp, "$bairro", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sCidade)
				sValor = be_strreplace (sTemp, "$cidade", Relatorio->EmpresaCliente->sCidade);
			else
				sValor = be_strreplace (sTemp, "$cidade", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sEstado)
				sValor = be_strreplace (sTemp, "$estado", Relatorio->EmpresaCliente->sEstado);
			else
				sValor = be_strreplace (sTemp, "$estado", "");

			BE_free (sTemp);
			sTemp = sValor;
			if (Relatorio->EmpresaCliente->sPais)
				sValor = be_strreplace (sTemp, "$pais", Relatorio->EmpresaCliente->sPais);
			else
				sValor = be_strreplace (sTemp, "$pais", "");
		}
	}
	//limpar
	BE_free (sTemp);
	BE_free (sTemp1);

	return sValor;
}

gfloat _be_relatorio_conversao (const gchar *sValor)
{
	//aceitar somente inteiros originalmente de qualquer unidade
	if (!sValor)
		return 0.0;
	else if (g_strrstr (sValor ,"pt"))
		return g_strtod (sValor, NULL);
	else if (g_strrstr (sValor ,"mm"))
		return g_strtod (sValor, NULL) * BEPDF_PIXEL_POR_POLEGADA;
	else if (g_strrstr (sValor ,"cm"))
		return g_strtod (sValor, NULL) * BEPDF_PIXEL_POR_POLEGADA * 10.0;
	else if (g_strrstr (sValor ,"m"))
		return g_strtod (sValor, NULL) * BEPDF_PIXEL_POR_POLEGADA * 100.0;
	else if (g_strrstr (sValor ,"in"))
		return g_strtod (sValor, NULL) * BERELATORIO_POL_POR_PONTO;
	else//pixels:px
		return g_strtod (sValor, NULL) / BERELATORIO_PIXEL_POR_PONTO;
}

void _be_relatorio_transformacoes (GNode *No, gfloat *fX, gfloat *fY, gfloat *fL, gfloat *fA, gfloat *fFonte)
{
	//pág. 136
	GNode *NoPai = NULL;
	BEXML_No_Elemento *NoElemento = NULL;
	gchar **aValor = NULL;
	const gchar *sValor = NULL;
	gchar *sTemp = NULL;

	//verificar atributo local
	NoElemento = (BEXML_No_Elemento *) No->data;
	NoPai = No;//valor inicial
	//loop
	do
	{
		//localizar atributo transformação
		if (NoElemento)
			sValor = be_xml_no_atributos_valor (NoElemento->Atributos, "transform");
		else
			sValor = NULL;
		//separar valores
		if (sValor)
		{
			BE_strfreev (aValor);
			BE_free (sTemp);
			sTemp = be_strextract (sValor, "(", ")", 0);
			if (sTemp)
				aValor = g_strsplit (sTemp, ",", -1);
		}
		//translate
		if (sValor && g_strrstr (sValor, "translate"))
		{
			//posição x
			if (fX && aValor[0])
				*fX += _be_relatorio_conversao (aValor[0]);//soma e subtração, utiliza conversão de unidades
			//posição y
			if (fY && aValor[0] && aValor[1])
				*fY += _be_relatorio_conversao (aValor[1]);//soma e subtração, utiliza conversão de unidades
		}
		//scale
		else if (sValor && g_strrstr (sValor, "scale"))
		{
			//fonte
			if (fFonte && aValor[0])
				*fFonte /= g_strtod (aValor[0], NULL);//multiplicação e divisão, não utiliza conversão de unidades
			//posição x
			if (fX && aValor[0])
				*fX *= g_strtod (aValor[0], NULL);//multiplicação e divisão, não utiliza conversão de unidades
			//posição y
			if (fY && aValor[0] && aValor[1])
				*fY *= g_strtod (aValor[1], NULL);//multiplicação e divisão, não utiliza conversão de unidades
			else if (fY && aValor[0])
				*fY *= g_strtod (aValor[0], NULL);//multiplicação e divisão, não utiliza conversão de unidades
		}
		//matrix
		else if (sValor && g_strrstr (sValor, "matrix"))
		{
			//preparar largura
			if (fL && fX && aValor[0] && aValor[1] && aValor[2] && aValor[3] && aValor[4])
				*fL += *fX;
			//preparar altura
			if (fA && fY && aValor[0] && aValor[1] && aValor[2] && aValor[3] && aValor[4] && aValor[5])
				*fA += *fY;
			//posição x
			if (fX && aValor[0] && aValor[1] && aValor[2] && aValor[3] && aValor[4])
			{
				*fX *= g_strtod (aValor[0], NULL);//multiplicação e divisão, não utiliza conversão de unidades
				*fX += _be_relatorio_conversao (aValor[4]);//soma e subtração, utiliza conversão de unidades
			}
			//posição y
			if (fY && aValor[0] && aValor[1] && aValor[2] && aValor[3] && aValor[4] && aValor[5])
			{
				*fY += _be_relatorio_conversao (aValor[5]);//soma e subtração, utiliza conversão de unidades
			}
			//largura
			if (fL && fX && aValor[0] && aValor[1] && aValor[2] && aValor[3] && aValor[4])
			{
				*fL *= g_strtod (aValor[0], NULL);//multiplicação e divisão, não utiliza conversão de unidades
				*fL += _be_relatorio_conversao (aValor[4]);//soma e subtração, utiliza conversão de unidades
				*fL -= *fX;
			}
			if (fA && fY && aValor[0] && aValor[1] && aValor[2] && aValor[3] && aValor[4] && aValor[5])
			{
				*fA += _be_relatorio_conversao (aValor[5]);//soma e subtração, utiliza conversão de unidades
				*fA -= *fY;
			}
		}

		//procurar por agrupamento superior
		if (NoPai)
			NoPai = be_xml_no_nome (NoPai->parent, "g", BEXML_LOCALIZAR_PAI);
		if (NoPai)
			NoElemento = (BEXML_No_Elemento *) NoPai->data;
		else
			NoElemento = NULL;
	}
	while (NoElemento);

	//forçar uma grade virtual, arredondamento para inteiros
	if (fX)
		*fX = (float) (int) (*fX + 0.5);
	if (fY)
		*fY = (float) (int) (*fY + 0.5);
	if (fL)
		*fL = (float) (int) (*fL + 0.5);
	if (fA)
		*fA = (float) (int) (*fA + 0.5);
	//forçar uma grade virtual, arredondamento para 3 casa decimais
	/*if (fX)
		*fX = ((float) ((int) ((*fX + 0.005) * 100)) / 100);
	if (fY)
		*fY = ((float) ((int) ((*fY + 0.005) * 100)) / 100);
	if (fL)
		*fL = ((float) ((int) ((*fL + 0.005) * 100)) / 100);
	if (fA)
		*fA = ((float) ((int) ((*fA + 0.005) * 100)) / 100);*/

	//limpar
	BE_strfreev (aValor);
	BE_free (sTemp);
}
