/*
+------------------------------------------------------------------------------+
| Luciano Trindade Pereira 2006                                                |
| Bibliotecas Estendidas em C                                                  |
+------------------------------------------------------------------------------+
| BERelatorio                                                                  |
+------------------------------------------------------------------------------+
| BERelatorio é uma biblioteca livre, você pode redistribuí-la e/ou            |
| modificá-la dentro dos termos da Licença Pública Geral GNU como              |
| publicada pela Fundação do Software Livre (FSF); na versão 2 da              |
| Licença, ou (na sua opnião) qualquer versão.                                 |
| Este programa é distribuido na esperança que possa ser útil,                 |
| mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÃO a qualquer |
| MERCADO ou APLICAÇÃO EM PARTICULAR.                                          |
| Veja a Licença Pública Geral GNU para maiores detalhes.                      |
| Você deve ter recebido uma cópia da Licença Pública Geral GNU                |
| junto com este programa, se não, escreva para a Fundação do Software         |
| Livre(FSF) Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     |
+------------------------------------------------------------------------------+
*/
#ifndef _BERELATORIO_H
	#define _BERELATORIO_H
	#include "../global/beglobal.h"
	#include "../pdf/bepdf.h"
	#include "../sql/besql.h"
	#include "../texto/betexto.h"

	#define BERELATORIO_VERSAO "BERelatório 1.1"
	#define BERELATORIO_PIXEL_POR_MM 3.41
	#define BERELATORIO_PIXEL_POR_PONTO 1.25
	#define BERELATORIO_POL_POR_PONTO 72
	#define BERELATORIO_LENSTRING 500
	#define BERELATORIO_LENSTRING_BIN 16
	#define BERELATORIO_LENSTRING_FILTRO 25
	#define BERELATORIO_MAX_LINHAS_POR_PAGINA 1000
	#define BERELATORIO_LISTA_MAX_COLUNAS 15
	#define BERELATORIO_LOC_PAGINAS 16
	#define BERELATORIO_LOC_FORMULARIOS 17
	#define BERELATORIO_LOC_LISTAS 18
	#define BERELATORIO_LOC_ROTULOS 19
	#define BERELATORIO_LOC_INDICES 20
	#define BERELATORIO_LOC_SUBLISTAS 21
	#define BERELATORIO_LOC_SUBROTULOS 22
	#define BERELATORIO_LOC_SUBINDICES 23
	#define BERELATORIO_ESPACAMENTO_SUBLISTAS 20

	//sublistas-campos
	typedef struct
	{
		GSList *SubCabecalhos; //aponta para (_BERELATORIO_Dados *)
		GSList *SubListas; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_SUBLISTAS
		GSList *SubRotulos; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_SUBROTULOS
		GSList *SubIndices; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_SUBINDICES
		GSList *Linhas;//aponta para GSList *SubLinhas; a primeira linha é o cabeçalho e rótulos, as do meio são as linhas e seus índices, e a últim a é o subtotal.
		gfloat fYMinSubListas;
		glong lLinha; //linha, atual
		BESQL_Campo *Campo; //número/sequencia do campo relacionado
		gchar *sSubRelatorio;//nome do relatório
	} _BERELATORIO_SubLista;

	//dados
	typedef struct
	{
		GSList *Celulas; //aponta para (BEPDF_Celula *)
		gfloat fLargura, fAltura; //dimensões máximas
		glong lLinha; //linha, atual
		BESQL_Campo *Campo; //número/sequencia do campo relacionado
	} _BERELATORIO_Dados;
	//estrutura
	typedef struct
	{
		BEPDF *PDF;
		BESQL_Sintaxe *Sintaxe;
		BESQL_Sintaxe *SubSintaxe;
		PyObject *pyModulo, *pyScript;
		glong lLinha; //linha, atual
		BESQL_Campo *Campo;//campo dentro de um relatório padrão, atual
		gint nPagina, nPaginas; //contagem das páginas
		gfloat fLarguraProporcional; //largura proporcional, para lista padrão, atual
		_BERELATORIO_Dados *DadosAtual;//dados, atual
		gfloat fXMinAtual, fYMinAtual;//para dados, atual
		gfloat fYRelativo;//para dados, padrão é 0
		GSList *Dados; //aponta para (_BERELATORIO_Dados *) //BEPDF_LOC_CORPO
		gfloat fXMinDados, fYMinDados, fLarguraMaxDados, fAlturaMaxDados;//para dados
		GSList *Paginas; //aponta para (_BERELATORIO_Dados *) //BEPDF_LOC_PAGINA
		GSList *Formularios; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_FORMULARIOS
		gfloat fXMinForms, fYMinForms, fLarguraMaxForms, fAlturaMaxForms;//para formulários
		GSList *Listas; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_LISTAS
		GSList *Rotulos; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_ROTULOS
		GSList *Indices; //aponta para (_BERELATORIO_Dados *) //BERELATORIO_LOC_INDICES
		gfloat fXMinListas, fYMinListas, fLarguraMaxListas, fAlturaMaxListas;//para listas e rótulos
		GSList *SubListas; //aponta para (_BERELATORIO_SubLista *)
		gfloat fXMinSubListas, fYMinSubListas, fLarguraMaxSubListas, fAlturaMaxSubListas;//para sublistas e rótulos; temporário
		const BEGLOBAL_Empresa_Cliente *EmpresaCliente;
		const gchar *sRelatorio, *sSubRelatorio, *sAutor, *sArquivoSVG;
		GSList *Imagens;//imagens carregadas apartir de arquivos, aponta para (GdkPixbuf *)
		BEPDF_LOC_ENUM nLocal;
	} _BERELATORIO;

	//Protótipos
	gboolean be_relatorio_gerar (
		const gchar *sArquivoSVG,
		const gchar *sArquivoSaida,
		const gchar *sArquivoFonte,
		BESQL_Campo *Campo, //campo para complementar o nome de vários arquivos
		const gchar *sRelatorio, //nome do relatório
		const BEGLOBAL_Empresa_Cliente *EmpresaCliente, //informações da empresa cliente
		BESQL_Sintaxe *Sintaxe, //dados
		gchar **sErros //erros
	);
	//Protótipos reservados
	gboolean _be_relatorio_gerar (
		const gchar *sArquivoSVG,
		const gchar *sArquivoSaida,
		const gchar *sArquivoFonte,
		const gchar *sRelatorio,
		const BEGLOBAL_Empresa_Cliente *EmpresaCliente,
		BESQL_Sintaxe *Sintaxe,
		gchar **sErros
	);
	_BERELATORIO_SubLista *_be_subrelatorio_gerar (_BERELATORIO *Relatorio, BESQL_Sintaxe *SubSintaxe, const gchar *sSubRelatorio, GNode *No, glong lLinha, BESQL_Campo *CampoRef, gchar **sErros);
	void _be_subrelatorio_reposicionar (_BERELATORIO *Relatorio, _BERELATORIO_SubLista *SubLista, GNode *No, gfloat *fY, gint *nPagina);
	void _be_subrelatorio_limpar (_BERELATORIO_SubLista *SubLista);
	gboolean _be_relatorio_loop (GNode *node, gpointer data);
	gchar *_be_relatorio_variaveis (_BERELATORIO *Relatorio, gchar *sTexto);
	gchar *_be_relatorio_dados (_BERELATORIO *Relatorio, const gchar *sValor);
	GdkPixbuf *_be_relatorio_dados_imagem (_BERELATORIO *Relatorio, const gchar *sValor);
	gfloat _be_relatorio_conversao (const gchar *sValor);//conversão de unidade de medidas
	BEPDF_Celula *_be_relatorio_retangulo (GNode *No, _BERELATORIO *Relatorio);
	void _be_relatorio_largura_proporcional (_BERELATORIO *Relatorio, BEPDF_Celula *Celula);
	void _be_relatorio_transformacoes (GNode *No, gfloat *fX, gfloat *fY, gfloat *fL, gfloat *fA, gfloat *fFonte);

	//#define BERELATORIO_DEPURAR

	/*
	SEÇÕES:
    =======
    -Fundo:
    -Dados:
    -Formulários;
    -Lista:
    -Rótulos:
    -Índices:
    -Cabeçalho:
    -Rodapé:
    -Página:

    Limites: Os limites do loop dos dados para listas são definidos por qualquer retângulo com o nome de LISTAS. Este retângulo será ignorando na visualização, alterando tipo para rect1.
    Limites: Os limites do loop dos dados para formulários são definidos por qualquer retângulo com o nome de FORMULARIOS. Este retângulo será ignorando na visualização, alterando tipo para rect1.
	*/
#endif
