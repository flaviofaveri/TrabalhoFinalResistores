#include <stdio.h>
#include <stdlib.h>

struct header
{
    int tam;
    struct serie* pSeriePrim;
    struct serie* pSerieUltm;
};
typedef struct header Header;

struct serie
{
    struct serie* pSerieProx;
    struct erro* pErroPrim;
    struct erro* pErroUltm;
    int numSerie;
};
typedef struct serie Serie;

struct erro
{
    struct erro* pErroProx;
    struct potencia* pPotenciaPrim;
    struct potencia* pPotenciaUltm;
    int numErro;
};
typedef struct erro Erro;

struct potencia
{
    struct potencia* pPotenciaProx;
    struct arvResistor* pArvRaiz;
    int numPotencia;
};
typedef struct potencia Potencia;

struct arvResistor
{
    struct arvResistor* pResEsq;
    struct arvResistor* pResDir;
    int valor;
    int qtd;
};
typedef struct arvResistor ArvResistor;

Header* criaBanco();
Header* carregaBanco(Header* pHeader);
void insereListas(Header* pHeader, int sre, int err, int pta);

int main()
{
    Header* pHeader = criaBanco();
    carregaBanco(pHeader);
    printf("Hello world!\n");
    return 0;
}

Header* criaBanco()
{
    Header* pHeader = (Header*)malloc(sizeof(Header));
    pHeader->pSeriePrim = NULL;
    pHeader->pSerieUltm = NULL;
    pHeader->tam = 0;
    return pHeader;
}

Header* carregaBanco(Header* pHeader)
{
    int sre=0, err=0, pta=0, vlr=0, qdd=0;
    FILE *pFILE = fopen ("ListaResistores.txt", "r");
    if (pFILE == NULL)
    {
        printf ("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    do
    {
        fscanf(pFILE, "%d %d %d\n", &sre, &err, &pta);
        insereListas(pHeader, sre, err, pta);
    }
    while(!feof(pFILE));
    fclose (pFILE);
    return pHeader;
}

void insereListas(Header* pHeader, int sre, int err, int pta)
{
    Serie* pSerieAux;
    for (pSerieAux=pHeader->pSeriePrim; pSerieAux!=NULL; pSerieAux=pSerieAux->pSerieProx)
    {
        if (pSerieAux->numSerie != sre)
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
        }
    }
    Erro* pErroAux;
    for (pErroAux=pHeader->pSeriePrim->pErroPrim; pErroAux!=NULL; pErroAux=pErroAux->pErroProx)
    {
        if (pErroAux->numErro != err)
        {
            Erro* pErroNovo = (Erro*)malloc(sizeof(Erro));
            pErroNovo->numErro = err;
            pErroNovo->pErroProx = NULL;
            pErroNovo->pPotenciaPrim = NULL;
            pErroNovo->pPotenciaUltm = NULL;
            if (pHeader->pSeriePrim->pErroPrim == NULL) pHeader->pSeriePrim->pErroPrim = pErroNovo;
            if (pHeader->pSeriePrim->pErroUltm == NULL) pHeader->pSeriePrim->pErroUltm = pErroNovo;
            else
            {
                pHeader->pSeriePrim->pErroUltm->pErroProx = pErroNovo;
                pHeader->pSeriePrim->pErroUltm = pErroNovo;
            }
            printf("%d\n", pErroNovo->numErro);
        }
    }
    Potencia* pPotAux;
    for (pPotAux=pHeader->pSeriePrim->pErroPrim->pPotenciaPrim; pPotAux!=NULL; pPotAux=pPotAux->pPotenciaProx)
    {
        if (pPotAux->numPotencia != pta)
        {
            Potencia* pPotNovo = (Potencia*)malloc(sizeof(Potencia));
            pPotNovo->numPotencia = pta;
            pPotNovo->pArvRaiz = NULL;
            pPotNovo->pPotenciaProx = NULL;
            if (pHeader->pSeriePrim->pErroPrim->pPotenciaPrim == NULL)
                pHeader->pSeriePrim->pErroPrim->pPotenciaPrim = pPotNovo;
            if (pHeader->pSeriePrim->pErroPrim->pPotenciaUltm == NULL)
                pHeader->pSeriePrim->pErroPrim->pPotenciaUltm = pPotNovo;
            else
            {
                pHeader->pSeriePrim->pErroPrim->pPotenciaUltm->pPotenciaProx = pPotNovo;
                pHeader->pSeriePrim->pErroPrim->pPotenciaUltm = pPotNovo;
            }
            printf("%d\n", pPotNovo->numPotencia);
        }
    }
}
















