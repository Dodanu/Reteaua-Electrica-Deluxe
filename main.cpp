#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <random>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <time.h>

/**
cum fonctioneaza harta:
    exemplu:
        Nume:
            Paris
        Centrale pe locatie:
            0
        Conexiuni:
            7
        Directii Conexiune Nume, Pret, nr centrale:
            [[Bordeux,-1,-1, 12,0][Lyon,0,-1,11,0][Zurich,1,-1,14,0][London,-1,1,16,0][Stuttgart,0,1,14,0][Rhein-Ruhr,1,1,10,0][Vlaaderen,1,0,7,0]]
conventie numire centrale:
    3(pret)C(combustibil)1(oras)2(folosire combustibil)

ifstream ceva("ceva"); comentarii inseamna -> (termen(i))<-efectiv ce face + ultim cuv <- in ce domeniu se afla
*/

using namespace std;

ifstream cePin("centrale.data");  //pret centrala
ifstream ceCin("centraleComb.data");  //combustibil folosit centrala
ifstream ceOin("centraleOrase.data"); //cate orase alimenteaza centrala
ifstream ceFCin("centraleFolosireComb.data"); //cat combustibil folosit centrala
ifstream ceNCin("centraleNumeCentrale.data"); //nume centrala
ifstream heuNumin("hartaeu.map"); //nume orase
ifstream heuNrConexin("harataeuNumConex.map");  //numar conex orase
ifstream heuNumConexin("harataeuNumConex.map"); //nume orase la conex orase
ifstream heuNrCen("hartaeuNrCen.map");   //cate cen pe orase
ifstream heuCordX("hartaeuCordX.map");  //cord X conex orase
ifstream heuCordY("hartaeuCordY.map");  //cord Y conex orase
ifstream heuPretConex("hartaeuPretConex.map");  //pret conex orase
ifstream heuCJCO("hartaeuCJCO.map"); //ce jucator detine centrala pe oras orase

struct centrala{
    char numeCen[7];
    char numeComb[255];
    int pret;
    int combNecesar;
    int orase;
    float SMC = 0;
};

//struct conex{
//    string numeConex;
//    int cordX;
//    int cordY;
//    int pret;
//    int nrCentrale;
//};

struct oras{
    char numeOras[255];
    //int nrCentrale;
    //int nrConex;
    //conex conexiuni[8];
};

struct hartaEU{
    oras orase[13][7];
    int oX[13][7] = {{0,1,2,6,8,9},{1,3,5,7,10},{3,4,8,9},{3,4,6,7,9},{4,5,6,7,10},{4,5,6,7,9,10},{2,4,6,7},{2,4,5,8},{1,6,10},{2,8},{7,8},{6,8,9}};
    int i = 13;
    int j[13] = {6, 5, 4, 5, 5, 6, 4, 4, 3, 2, 2, 3};
};

int gasirePrimPret(int preturi[][9],int x,int y)
{
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            if(preturi[i][j]==1){
                return i;
            }
        }
    }
    return 0;
}

void bubbleSort(float v[],int n)
{
    bool terminat = false;
    while(!terminat){
        terminat  = true;
        for(int i=0; i<n-1; i++){
            if(v[i]<v[i+1]){
                float aux = v[i];
                v[i] = v[i+1];
                v[i+1] = aux;
            }
        }
    }
}

void afisareHartaEu(hartaEU hE)
{
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            //int aux = hE.oX[a][b];
            cout<<hE.orase[a][hE.oX[a][b]].numeOras<<" ";
        }
        cout<<endl;
    }
}

void citireHartaEU(hartaEU &hE)
{
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            //int aux = hE.oX[a][b];
            heuNumin.getline(hE.orase[a][hE.oX[a][b]].numeOras, 255, '\n');
            cout<<hE.orase[a][hE.oX[a][b]].numeOras<<" ";
        }
        cout<<endl;
    }
}

void afisareCentrale(centrala centrale[])
{
    for(int i=0; i<42; i++){
        cout<<centrale[i].numeCen<<" "<<centrale[i].pret<<" "<<centrale[i].numeComb<<endl;
    }
}

void citireCentrale(centrala centrale[])
{
    int pr, ors, comb;
    char aux[10], aux2[10];
    int i = 0;
    while(i<42){
        cePin>>pr;

        ceCin.getline(aux, 3, '\n');
        ceOin>>ors;
        ceFCin>>comb;
        ceNCin.getline(aux2, 7, '\n');
        strcpy(centrale[i].numeCen, aux2);
        centrale[i].pret = pr;
        centrale[i].orase = ors;
        centrale[i].combNecesar = comb;
        strcpy(centrale[i].numeComb,aux);
        i++;
    }
}

void citireCentraleInUz(char centInUz[9],int stage,centrala centrale[],HANDLE h,int cPreturi[][9],int cX,int cY,int cMeta[],int aPreturi[][9],int aX,int aY,int aMeta[],int pPreturi[][9],int pX,int pY,int pMeta[],int aPPreturi[][9],int aPX,int aPY,int aPMeta[],int nPreturi[][9],int nX,int nY,int nMeta[],int ePreturi[][9],int eX,int eY,int eMeta[])
{
    int centPePiata, nAux = 0;
    float auxSMC[45];
    char auxNum[45][45];
    cin>>centPePiata;
    cin.ignore();
    for(int i=0; i<centPePiata; i++){
        cin.getline(centInUz, 9);
        for(int j=0; j<42; j++){
            if(strcmp(centrale[j].numeCen,centInUz)==0){
                int ors = centrale[j].orase;
                if(centrale[j].orase<3)
                    ors = ors * -1 - (3-ors);
                if(strcmp(centrale[j].numeComb,"C")==0){
                    //cout<<"da carb"<<endl;
                    centrale[j].SMC = cMeta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(cPreturi, cX, cY)) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"A")==0){
                    //cout<<"da apa"<<endl;
                    centrale[j].SMC = aMeta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(aPreturi, aX, aY)) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"P")==0){
                    //cout<<"da pet"<<endl;
                    centrale[j].SMC = pMeta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(pPreturi, pX, pY)) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"AP")==0){
                    //cout<<"da appet"<<endl;
                    centrale[j].SMC = aPMeta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(aPPreturi, aPX, aPY)) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"N")==0){
                    //cout<<"da nuc"<<endl;
                    centrale[j].SMC = nMeta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(nPreturi, nX, nY)) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"E")==0){
                    //cout<<"da eco"<<endl;
                    centrale[j].SMC = eMeta[stage] + ors - centrale[j].combNecesar - (centrale[j].pret/10);
                }
                auxSMC[nAux] = centrale[j].SMC;
                strcpy(auxNum[nAux], centrale[j].numeCen);
                nAux++;
                /*
                if(centrale[j].SMC>7){
                    SetConsoleTextAttribute(h,2);
                    cout<<"Scor meta al centralei "<<centrale[j].numeCen<<" este: "<<centrale[j].SMC<<endl;
                }
                if(5<=centrale[j].SMC || centrale[j].SMC<=7){
                    SetConsoleTextAttribute(h,14);
                    cout<<"Scor meta al centralei "<<centrale[j].numeCen<<" este: "<<centrale[j].SMC<<endl;
                }
                if(centrale[j].SMC<5){
                    SetConsoleTextAttribute(h,4);
                    cout<<"Scor meta al centralei "<<centrale[j].numeCen<<" este: "<<centrale[j].SMC<<endl;
                }
                SetConsoleTextAttribute(h,15);
                */
            }
        }
    }
    for(int i=0; i<nAux; i++){
        if(auxSMC[i]>7){
            SetConsoleTextAttribute(h,2);
            cout<<"Scor meta al centralei "<<auxNum[i]<<" este: "<<auxSMC[i]<<endl;
        }
        if(5<=auxSMC[i] || auxSMC[i]<=7){
            SetConsoleTextAttribute(h,14);
            cout<<"Scor meta al centralei "<<auxNum[i]<<" este: "<<auxSMC[i]<<endl;
        }
        if(auxSMC[i]<5){
            SetConsoleTextAttribute(h,4);
            cout<<"Scor meta al centralei "<<auxNum[i]<<" este: "<<auxSMC[i]<<endl;
        }
        SetConsoleTextAttribute(h,15);
    }
}

void schimbareCombustibil(int preturi[][9],int x,int y,int cateDeAdaug)
{
    for(int i=x-1; i>=0; i--){
        for(int j=y-1; j>=0; j--){
            if(cateDeAdaug==0)
                break;
            else{
                if(preturi[i][j]==0){
                    preturi[i][j] = 1;
                    cateDeAdaug--;
                }
            }
        }
    }
}

void cataSchimbareCombustibil(int nrJucatori,int stage,int cPreturi[][9],int cX,int cY,int cMeta[],int aPreturi[][9],int aX,int aY,int aMeta[],int pPreturi[][9],int pX,int pY,int pMeta[],int aPPreturi[][9],int aPX,int aPY,int aPMeta[],int nPreturi[][9],int nX,int nY,int nMeta[],int ePreturi[][9],int eX,int eY,int eMeta[])
{
    int resurseDeAdaug, n=4;
    for(int i=0; i<n; i++){
        switch(i){
            cin.ignore();
            case 0:
                cout<<"Cat carbune: ";
                cin>>resurseDeAdaug;
                schimbareCombustibil(cPreturi, cX, cY, resurseDeAdaug);
                break;
            case 1:
                cout<<"Cata apa: ";
                cin>>resurseDeAdaug;
                schimbareCombustibil(aPreturi, aX, aY, resurseDeAdaug);
                break;
            case 2:
                cout<<"Cat petrol: ";
                cin>>resurseDeAdaug;
                schimbareCombustibil(pPreturi, pX, pY, resurseDeAdaug);
                break;
            case 3:
                cout<<"Cat uraniu: ";
                cin>>resurseDeAdaug;
                schimbareCombustibil(nPreturi, nX, nY, resurseDeAdaug);
                break;
        }

    }
}

int main()
{
    int runda = 0, stage = 1, numarJucatori;
    char centInUz[9];
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    hartaEU hartaEuropa;
    centrala centrale[44];
    int cPreturi[9][9] = {{0},{0},{0, 0, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1}};
    int cX=9, cY=4;
    int cMeta[3] = {5, 10, 5};
    int aPreturi[8][9] = {{0},{0, 0, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1}};
    int aX=8, aY=3;
    int aMeta[3] = {6, 8, 10};
    int pPreturi[8][9] = {{0},{0, 0, 0, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{1, 1, 1, 1}};
    int pX=8, pY=4;
    int pMeta[3] = {6, 7, 9};
    int aPpreturi[8][9] = {{0},{0, 0, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1}};
    int aPx=8, aPy=5;
    int aPMeta[3] = {7, 9, 11};
    int nPreturi[9][9] = {{0},{0},{0},{0},{0},{0},{0, 1},{1, 1},{1, 1}};
    int nX=8,nY=2;
    int nMeta[3] = {3, 3, 5};
    int ePreturi[1][9] = {{0}};
    int eX=1, eY=1;
    int eMeta[3] = {10,10,10};
    int centraleDetinute[4], combDetinut[100], centraleDetinuteAltiJucatori[6][4], numarOraseCuCentrale[6][1];
    //cout<<"Dati numarul de jucatori: ";
    //cin>>numarJucatori;
    citireCentrale(centrale);
    citireCentraleInUz(centInUz,stage,centrale,h,cPreturi,cX,cY,cMeta,aPreturi,aX,aY,aMeta,pPreturi,pX,pY,pMeta,aPpreturi,aPx,aPy,aPMeta,nPreturi,nX,nY,nMeta,ePreturi,eX,eY,eMeta);
    //cataSchimbareCombustibil(numarJucatori,stage,cPreturi,cX,cY,cMeta,aPreturi,aX,aY,aMeta,pPreturi,pX,pY,pMeta,aPpreturi,aPx,aPy,aPMeta,nPreturi,nX,nY,nMeta,ePreturi,eX,eY,eMeta);
    //citireHartaEU(hartaEuropa);
    //afisareHartaEu(hartaEuropa);
    return 0;
}
