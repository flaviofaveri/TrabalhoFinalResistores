#include <stdio.h>
#include <stdlib.h>

struct header
{
    int tam;
    struct serie* pSeriePrim;
    struct serie* pSerieUltm;
};
typedef struct header Header;

struct serie //cria a lista de series
{
    struct serie* pSerieProx;
    struct erro* pErroPrim; //aponta para a o primeiro n� da lista de erro
    struct erro* pErroUltm; //aponta para a o ultimo n� da lista de erro
    int numSerie;
};
typedef struct serie Serie;

struct erro //cria a lista de erros
{
    struct erro* pErroProx;
    struct potencia* pPotenciaPrim; //aponta para a o primeiro n� da lista de potencia
    struct potencia* pPotenciaUltm; //aponta para a o ultimo n� da lista de potencia
    int numErro;
};
typedef struct erro Erro;

struct potencia //cria a lista de potencias
{
    struct potencia* pPotenciaProx;
    struct arvResistor* pArvRaiz; //aponta para a arvore de resistores da potencia espec�fica
    int numPotencia;
};
typedef struct potencia Potencia;

struct arvResistor //cria uma arvore de resistores de potencia especifica
{
    struct arvResistor* pResEsq;
    struct arvResistor* pResDir;
    int valor;
    int qtd; //quantidade de resistores
};
typedef struct arvResistor ArvResistor;

Header* criaBanco();
Header* carregaBanco(Header* pHeader);
//insere uma lista de s�rie, uma lista de erro, uma lista de pot�ncia
void insereListas(Header* pHeader, int sre, int err, int pta, int vlr, int qdd);
//insere uma arvore de resistores abaixo do n� de cada potencia
void insereArvore(ArvResistor** pArvRes, int vlr, int qdd);

int main()
{
    Header* pHeader = criaBanco();
    carregaBanco(pHeader);
    printf("\n");
    return 0;
}

Header* criaBanco() //cria o cabe�alho da estrutura
{
    Header* pHeader = (Header*)malloc(sizeof(Header));
    pHeader->pSeriePrim = NULL;
    pHeader->pSerieUltm = NULL;
    pHeader->tam = 0;
    return pHeader;
}

Header* carregaBanco(Header* pHeader)
{
    int sre=0, err=0, pta=0, vlr=0, qdd=0; //variav�is para pegar serie, erro, potencia, valor, quantidade do arquivo
    FILE *pFILE = fopen ("ListaResistores.txt", "r");
    if (pFILE == NULL)
    {
        printf ("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    do
    {
        fscanf(pFILE, "%d %d %d %d %d\n", &sre, &err, &pta, &vlr, &qdd);
        insereListas(pHeader, sre, err, pta, vlr, qdd);
    }
    while(!feof(pFILE));
    fclose (pFILE);
    return pHeader;
}

void insereListas(Header* pHeader, int sre, int err, int pta, int vlr, int qdd)
{
    Serie* pSerieAux = NULL;
    Erro* pErroAux = NULL;
    Potencia* pPotAux = NULL;
    int serieIgual=0, erroIgual=0, potIgual=0; //flags para verificar se uma serie, erro ou potencia j� existem na lista
    for (pSerieAux=pHeader->pSeriePrim; pSerieAux!=NULL; pSerieAux=pSerieAux->pSerieProx)
    {
        if (pSerieAux->numSerie == sre) //verifica se a serie j� existe na lista
        {
            serieIgual = 1; //flag de que a serie j� existe
            for (pErroAux=pSerieAux->pErroPrim; pErroAux!=NULL; pErroAux=pErroAux->pErroProx)
            {
                if (pErroAux->numErro == err) //verifica se o erro j� existe na lista
                {
                    erroIgual = 1; //flag de que o erro j� existe
                    for (pPotAux=pErroAux->pPotenciaPrim; pPotAux!=NULL; pPotAux=pPotAux->pPotenciaProx)
                    {
                        if (pPotAux->numPotencia == pta) //verifica se a potencia j� existe na lista
                        {
                            potIgual = 1; //flag de que a potencia j� existe
                            insereArvore(&(pPotAux->pArvRaiz),vlr,qdd); //chama a fun��o de inserir n� de arvore
                            break;
                        }
                    }
                    if (potIgual == 0) //insere novo n� de potencia, caso seja um novo valor de potencia
                    {
                        Potencia* pPotNovo = (Potencia*)malloc(sizeof(Potencia));
                        pPotNovo->numPotencia = pta;
                        pPotNovo->pArvRaiz = NULL;
                        pPotNovo->pPotenciaProx = NULL;
                        pHeader->pSerieUltm->pErroUltm->pPotenciaUltm->pPotenciaProx = pPotNovo;
                        pHeader->pSerieUltm->pErroUltm->pPotenciaUltm = pPotNovo;
                        printf("%d\n", pPotNovo->numPotencia);
                        insereArvore(&(pPotNovo->pArvRaiz),vlr,qdd); //com nova potencia, novo n� de arvore de resistor
                    }
                }
            }
            if (erroIgual == 0) //insere novo n� de erro, caso seja um novo valor de erro
            {
                Erro* pErroNovo = (Erro*)malloc(sizeof(Erro));
                pErroNovo->numErro = err;
                pErroNovo->pErroProx = NULL;
                pErroNovo->pPotenciaPrim = NULL;
                pErroNovo->pPotenciaUltm = NULL;
                pSerieAux->pErroUltm->pErroProx = pErroNovo;
                pSerieAux->pErroUltm = pErroNovo;
                printf("%d\n", pErroNovo->numErro);
                if (pErroNovo->pPotenciaUltm == NULL) //se for novo erro, haver� nova potencia
                {
                    Potencia* pPotNovo = (Potencia*)malloc(sizeof(Potencia));
                    pPotNovo->numPotencia = pta;
                    pPotNovo->pArvRaiz = NULL;
                    pPotNovo->pPotenciaProx = NULL;
                    pErroNovo->pPotenciaPrim = pPotNovo;
                    pErroNovo->pPotenciaUltm = pPotNovo;
                    printf("%d\n", pPotNovo->numPotencia);
                    insereArvore(&(pPotNovo->pArvRaiz),vlr,qdd); //com nova potencia, novo n� de arvore de resistor
                }
            }

        }
    }
    if (serieIgual == 0) //insere novo n� de serie, caso seja um novo valor de serie
    {
        Serie* pSerieNovo = (Serie*)malloc(sizeof(Serie));
        pSerieNovo->numSerie = sre;
        pSerieNovo->pSerieProx = NULL;
        pSerieNovo->pErroPrim = NULL;
        pSerieNovo->pErroUltm = NULL;
        if (pHeader->pSeriePrim == NULL) pHeader->pSeriePrim = pSerieNovo;
        if (pHeader->pSerieUltm == NULL) pHeader->pSerieUltm = pSerieNovo;
        else
        {
            pHeader->pSerieUltm->pSerieProx = pSerieNovo;
            pHeader->pSerieUltm = pSerieNovo;
        }
        pHeader->tam++;
        printf("%d\n", pSerieNovo->numSerie);
        if (pSerieNovo->pErroUltm == NULL) //se for nova serie, haver� novo erro
        {
            Erro* pErroNovo = (Erro*)malloc(sizeof(Erro));
            pErroNovo->numErro = err;
            pErroNovo->pErroProx = NULL;
            pErroNovo->pPotenciaPrim = NULL;
            pErroNovo->pPotenciaUltm = NULL;
            pSerieNovo->pErroPrim = pErroNovo;
            pSerieNovo->pErroUltm = pErroNovo;
            printf("%d\n", pErroNovo->numErro);
            if (pErroNovo->pPotenciaUltm == NULL) //se for novo erro, haver� nova potencia
            {
                Potencia* pPotNovo = (Potencia*)malloc(sizeof(Potencia));
                pPotNovo->numPotencia = pta;
                pPotNovo->pArvRaiz = NULL;
                pPotNovo->pPotenciaProx = NULL;
                pErroNovo->pPotenciaPrim = pPotNovo;
                pErroNovo->pPotenciaUltm = pPotNovo;
                printf("%d\n", pPotNovo->numPotencia);
                insereArvore(&(pPotNovo->pArvRaiz),vlr,qdd); //com nova potencia, novo n� de arvore de resistor
            }
        }
    }
}

void insereArvore(ArvResistor** pArvRes, int vlr, int qdd)
{
    //a fun��o insere novo n� recursivamente, com exce��o do primeiro n�
    ArvResistor* pArvResAux = NULL;
    if(!(*pArvRes)) //verifica se j� existe n� dentro da �rvore de resistores
    {
        pArvResAux = (ArvResistor*)malloc(sizeof(ArvResistor));
        pArvResAux->pResEsq = NULL;
        pArvResAux->pResDir = NULL;
        pArvResAux->valor = vlr;
        pArvResAux->qtd = qdd;
        *pArvRes = pArvResAux;
        return;
    }
    if(vlr < (*pArvRes)->valor)
    {
        insereArvore(&(*pArvRes)->pResEsq, vlr, qdd);
    }
    else if(vlr > (*pArvRes)->valor)
    {
        insereArvore(&(*pArvRes)->pResDir, vlr, qdd);
    }
}
