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
ifstream heuNrConexin("harataeuNrConex.map");  //numar conex orase
ifstream heuNumConexin("harataeuNumConex.map"); //nume orase la conex orase
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

struct combustibil{
    char numeComb[255];
    int culoareComb;
    int preturi[9][9];
    int combTotal;
    int x;
    int y;
    int meta[3];
};

struct jucator_{
    char nume[255];
    char culoare[255];
    char combDetinut[4][1] = {0, 0, 0, 0};
    centrala centraleDetinute[3];
    int orasePosbilAlimentare = 0;
    int contorCenDet=0;
    int bani = 50;
    int ordine;
    int nrOrase = 0;
};

struct conex{
    char numeConex[255];
    int cordX;
    int cordY;
    int pret;
    float SMCx;
    int nrCentrale = 0;
};

struct oras{
    char numeOras[255];
    char regiune[255];
    int nrCentrale = 0;
    int nrConex;
    float SMO;
    float SMCxTotal = 0;
    int pretConexTotal = 0;
    conex conexiuni[8];
};

struct hartaEU{
    oras orase[13][11];
    int oX[13][7] = {{0,1,2,6,8,9},{1,3,5,7,10},{3,4,8,9},{3,4,6,7,9},{4,5,6,7,10},{4,5,6,7,9,10},{2,4,6,7},{2,4,5,8},{1,6,10},{2,8},{7,8},{6,8,9}};
    int i = 13;
    int j[13] = {6, 5, 4, 5, 5, 6, 4, 4, 3, 2, 2, 3};
};

void toLower(char txt[])
{
    for(int i=0; i<strlen(txt); i++){
        txt[i] = tolower(txt[i]);
    }
}

/*
void asignareComb(combustibil comb[])
{
    strcpy(comb[0].numeComb,"carbune");
    comb[0].culoareComb = 6;
    comb[0].x = 9;
    comb[0].y = 4;
    comb[0].combTotal = 27;
    comb[0].preturi = {{0},{0},{0, 0, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1}};
    comb[0].meta = {5, 10, 5};
    strcpy(comb[1].numeComb,"apa");
    comb[1].culoareComb = 3;
    comb[1].x = 8;
    comb[1].y = 3;
    comb[1].combTotal = 24;
    comb[1].preturi = {{0},{0, 0, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1}};
    comb[1].meta = {6, 8, 10};
    strcpy(comb[2].numeComb,"petrol");
    comb[2].culoareComb = 8;
    comb[2].x = 8;
    comb[2].y = 1;
    comb[2].combTotal = 21;
    comb[2].preturi = {{0},{0, 0, 0, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{1, 1, 1, 1}};
    comb[2].meta = {6, 7, 9};
    strcpy(comb[3].numeComb,"apapetrol");
    comb[3].culoareComb = 1;
    comb[3].preturi = {{0},{0, 0, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1}}
    comb[3].x = 8;
    comb[3].y = 5;
    comb[3].combTotal = 45;
    comb[3].meta = {7, 9, 11};
    strcpy(comb[4].numeComb,"nuclear");
    comb[4].culoareComb = 12;
    comb[4].preturi = {{0},{0},{0},{0},{0},{0},{0, 1},{1, 1},{1, 1}};
    comb[4].x = 8;
    comb[4].y = 2;
    comb[4].combTotal = 12;
    comb[4].meta = {3, 3, 5};
    strcpy(comb[5].numeComb,"eco");
    comb[5].culoareComb = 10;
    comb[5].preturi = {{1}}
    comb[5].x = 1
    comb[5].y = 1;
    comb[5].combTotal = 0;
    comb[5].meta = {10,10,10};
}
*/

void afisareHartaEu(hartaEU hE)
{
    int nrOras=1, nrConex = 1;
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            //cout<<hE.oX[a][b]<<" ";
            cout<<nrOras<<" ";
            cout<<hE.orase[a][hE.oX[a][b]].numeOras<<endl;
            for(int c=0; c<hE.orase[a][hE.oX[a][b]].nrConex; c++){
                cout<<nrConex<<"     "<<hE.orase[a][hE.oX[a][b]].conexiuni[c].numeConex<<" "<<hE.orase[a][hE.oX[a][b]].conexiuni[c].pret<<" "<<hE.orase[a][hE.oX[a][b]].conexiuni[c].cordX<<" "<<hE.orase[a][hE.oX[a][b]].conexiuni[c].cordY<<endl;
                nrConex++;
            }
            nrOras++;
            //cout<<hE.orase[a][hE.oX[a][b]].nrConex<<" ";
        }
        cout<<endl;
    }
}

void citireNumeHartaEU(hartaEU &hE)
{
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            heuNumin.getline(hE.orase[a][hE.oX[a][b]].numeOras, 255, '\n');
        }
    }
}

void citireNrConexHartaEu(hartaEU &hE)
{
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            heuNrConexin>>hE.orase[a][hE.oX[a][b]].nrConex;
        }
    }
}

void citireNumeConexHarta(hartaEU &hE)
{
    int numarConexTotal = 0;
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            for(int c=0; c<hE.orase[a][hE.oX[a][b]].nrConex; c++){
                heuNumConexin.getline(hE.orase[a][hE.oX[a][b]].conexiuni[c].numeConex, 255, '\n');
                numarConexTotal++;
            }
        }
    }
}

void citirePretConexHarta(hartaEU &hE)
{
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            for(int c=0; c<hE.orase[a][hE.oX[a][b]].nrConex; c++){
                heuPretConex>>hE.orase[a][hE.oX[a][b]].conexiuni[c].pret;
            }
        }
    }
}

void calculareSMCx(hartaEU &hE)
{
    int aux;
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            for(int c=0; c<hE.orase[a][hE.oX[a][b]].nrConex; c++){
                switch(hE.orase[a][hE.oX[a][b]].conexiuni[c].nrCentrale){
                    case 0:
                        aux = 10;
                        break;
                    case 1:
                        aux = 15;
                        break;
                    case 2:
                        aux = 20;
                        break;
                }
                hE.orase[a][hE.oX[a][b]].conexiuni[c].SMCx = 0 + hE.orase[a][hE.oX[a][b]].conexiuni[c].pret + aux;
                hE.orase[a][hE.oX[a][b]].SMCxTotal = 0 + hE.orase[a][hE.oX[a][b]].conexiuni[c].SMCx;
            }
        }
    }
}

void calculareSMO(hartaEU &hE)
{
    int aux;
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            for(int c=0; c<hE.orase[a][hE.oX[a][b]].nrConex; c++){
                hE.orase[a][hE.oX[a][b]].pretConexTotal = hE.orase[a][hE.oX[a][b]].pretConexTotal + hE.orase[a][hE.oX[a][b]].conexiuni[c].pret;
            }
            if(1<=hE.orase[a][hE.oX[a][b]].nrConex && hE.orase[a][hE.oX[a][b]].nrConex<3)
                aux = -5;
            if(3<=hE.orase[a][hE.oX[a][b]].nrConex && hE.orase[a][hE.oX[a][b]].nrConex<4)
                aux = 0;
            if(4<=hE.orase[a][hE.oX[a][b]].nrConex && hE.orase[a][hE.oX[a][b]].nrConex<=5)
                aux = 5;
            if(6<=hE.orase[a][hE.oX[a][b]].nrConex && hE.orase[a][hE.oX[a][b]].nrConex<=7)
                aux = 10;
            hE.orase[a][hE.oX[a][b]].SMO = 0 + aux + hE.orase[a][hE.oX[a][b]].SMCxTotal/10 - hE.orase[a][hE.oX[a][b]].pretConexTotal/10;
            //hE.orase[a][hE.oX[a][b]].SMO = hE.orase[a][hE.oX[a][b]].SMO * -1;
        }
    }
}

/*
void calcaulareConexiuniOrase(hartaEU &hE)
{
    int auxI, auxJ;
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            for(int c=0; c<hE.orase[a][hE.oX[a][b]].nrConex; c++){
                for(int d=0; d<hE.i; d++){
                    for(int f=0; f<hE.j[a]; f++){
                        if(strcmp(hE.orase[a][hE.oX[a][b]].conexiuni[c].numeConex,hE.orase[d][hE.oX[d][f]].numeOras)==0){
                            auxI = d;
                            auxJ = f;
                            d = hE.i;
                            f=hE.j[a];
                        }
                    }
                }
                if(a>auxI)
                    hE.orase[a][hE.oX[a][b]].conexiuni[c].cordX = a - auxI;
                else
                    hE.orase[a][hE.oX[a][b]].conexiuni[c].cordX = auxI - a;
                if(hE.oX[a][b]>auxJ)
                    hE.orase[a][hE.oX[a][b]].conexiuni[c].cordY = hE.oX[a][b] - auxJ;
                else
                    hE.orase[a][hE.oX[a][b]].conexiuni[c].cordY = auxJ - hE.oX[a][b];
            }
        }
    }
}
*/

void citireJucatori(jucator_ jucatori[],int &nrJucatori,HANDLE h)
{
    char aux2[255];
    cout<<"Cati jucatori sunt?"<<endl;
    cin>>nrJucatori;
    for(int i=0; i<nrJucatori; i++){
        if(i!=0)
            cout<<"Dati numele jucatorului "<<i<<": ";
        else
            cout<<"Dati numele user-ului: ";
        cin.ignore();
        cin.get(jucatori[i].nume, 255, '\n');
        toLower(jucatori[i].nume);
        if(i!=0)
            cout<<"Jucatorul "<<i<<" are numele: "<<jucatori[i].nume<<", corect?"<<endl;
        else{
            cout<<"User-ul are numele: "<<jucatori[i].nume<<", corect?"<<endl;
        }
        cin.ignore();
        cin.get(aux2, 255, '\n');
        if(strcmp(aux2, "nu")==0||strcmp(aux2, "negativ")==0||strcmp(aux2, "nicidecum")==0||strcmp(aux2, "deloc")==0||strcmp(aux2, "exclus")==0||strcmp(aux2, "nup")==0||strcmp(aux2, "de niciun fel")==0){
            SetConsoleTextAttribute(h, 4);
            cout<<"⚠ Eroare!(0001) ";
            SetConsoleTextAttribute(h, 14);
            cout<<"Numele jucatorului a fost dat gresit, for-ul se va repeta la final cu acelasi indice."<<endl;
            i--;
        }
        cout<<"Dati culoarea lui "<<jucatori[i].nume<<": ";
        cin.ignore();
        cin.get(jucatori[i].culoare, 255, '\n');
        toLower(jucatori[i].culoare);
        cout<<"Jucatorul "<<jucatori[i].nume<<" are culoarea: "<<jucatori[i].culoare<<", corect?"<<endl;
        cin.ignore();
        cin.get(aux2, 255, '\n');
        if(strcmp(aux2, "nu")==0||strcmp(aux2, "negativ")==0||strcmp(aux2, "nicidecum")==0||strcmp(aux2, "deloc")==0||strcmp(aux2, "exclus")==0||strcmp(aux2, "nup")==0||strcmp(aux2, "de niciun fel")==0){
            SetConsoleTextAttribute(h, 4);
            cout<<"⚠ Eroare!(0002) ";
            SetConsoleTextAttribute(h, 14);
            cout<<"Culoarea jucatorului a fost data gresit, for-ul se va repeta la final cu acelasi indice."<<endl;
            i--;
        }
        cout<<"Dati ordinea lui "<<jucatori[i].nume<<": ";
        cin.ignore();
        cin>>jucatori[i].ordine;
        cout<<"Jucatorul "<<jucatori[i].nume<<" este la ordinea: "<<jucatori[i].ordine<<", corect?"<<endl;
        cin.ignore();
        cin.get(aux2, 255, '\n');
        if(strcmp(aux2, "nu")==0||strcmp(aux2, "negativ")==0||strcmp(aux2, "nicidecum")==0||strcmp(aux2, "deloc")==0||strcmp(aux2, "exclus")==0||strcmp(aux2, "nup")==0||strcmp(aux2, "de niciun fel")==0){
            SetConsoleTextAttribute(h, 4);
            cout<<"⚠ Eroare!(0003) ";
            SetConsoleTextAttribute(h, 14);
            cout<<"Ordinea jucatorului a fost data gresit, for-ul se va repeta la final cu acelasi indice."<<endl;
            SetConsoleTextAttribute(h, 15);
            i--;
        }
        SetConsoleTextAttribute(h, 15);
    }
    SetConsoleTextAttribute(h, 15);
}

void sortareOrdineJucatori(int nrJucatori,jucator_ jucatori[])
{
    bool terminat = false;
    while(!terminat){
        terminat = true;
        for(int i=0; i<nrJucatori; i++){
            if(jucatori[i].nrOrase<jucatori[i+1].nrOrase){
                jucator_ aux;
                aux = jucatori[i];
                jucatori[i] = jucatori[i+1];
                jucatori[i+1] = aux;
                terminat = false;
            }
            if(jucatori[i].nrOrase==jucatori[i+1].nrOrase){
                if(jucatori[i].nrOrase==jucatori[i+1].nrOrase){
                    int cenMax1 = 0, cenMax2 = 0;
                    for(int j=0; j<jucatori[i].contorCenDet; j++){
                        if(cenMax1 < jucatori[i].centraleDetinute[j].orase)
                            cenMax1 = jucatori[i].centraleDetinute[j].orase;
                    }
                    for(int j=0; j<jucatori[i+1].contorCenDet; j++){
                        if(cenMax2 < jucatori[i+1].centraleDetinute[j].orase)
                            cenMax2 = jucatori[i+1].centraleDetinute[j].orase;
                    }
                    if(cenMax1 < cenMax2){
                        jucator_ aux;
                        aux = jucatori[i];
                        jucatori[i] = jucatori[i+1];
                        jucatori[i+1] = aux;
                        terminat = false;
                    }
                    if(cenMax1==cenMax2){
                        if(jucatori[i].bani<jucatori[i+1].bani){
                            jucator_ aux;
                            aux = jucatori[i];
                            jucatori[i] = jucatori[i+1];
                            jucatori[i+1] = aux;
                            terminat = false;
                        }
                        if(jucatori[i].bani==jucatori[i+1].bani){
                            if(jucatori[i].contorCenDet<jucatori[i+1].contorCenDet){
                                jucator_ aux;
                                aux = jucatori[i];
                                jucatori[i] = jucatori[i+1];
                                jucatori[i+1] = aux;
                                terminat = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

int gasirePrimPret(combustibil comb)
{
    for(int i=0; i<comb.x; i++){
        for(int j=0; j<comb.y; j++){
            if(comb.preturi[i][j]==1){
                return i+1;
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

int detectareCuloare(jucator_ jucatori)
{
    if(strcmp(jucatori.culoare, "albastru")==0){
        return 1;
    }
    if(strcmp(jucatori.culoare, "galben")==0){
        return 14;
    }
    if(strcmp(jucatori.culoare, "rosu")==0){
        return 4;
    }
    if(strcmp(jucatori.culoare, "verde")==0){
        return 2;
    }
    if(strcmp(jucatori.culoare, "gri")==0){
        return 8;
    }
    if(strcmp(jucatori.culoare, "mov")==0){
        return 5;
    }
    return 15;
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

void recomandareDacaCumparareCentrale(jucator_ jucatori[],int stage,int nrJucatori,HANDLE h,int indiceJucator)
{
    float SMR = 0;
    if(jucatori[indiceJucator].orasePosbilAlimentare < jucatori[indiceJucator].nrOrase)
        SMR = SMR + 1;
    if(jucatori[indiceJucator].orasePosbilAlimentare > jucatori[indiceJucator].nrOrase)
        SMR = SMR - 0.5;
    if(jucatori[indiceJucator].orasePosbilAlimentare==0)
        SMR = SMR + 10;
    if(stage==1){
        if(jucatori[indiceJucator].orasePosbilAlimentare==6 || jucatori[indiceJucator].orasePosbilAlimentare==7)
            SMR = SMR + 1;
        if(jucatori[indiceJucator].orasePosbilAlimentare<=5 && 3<=jucatori[indiceJucator].orasePosbilAlimentare)
            SMR = SMR + 1.5;
        if(jucatori[indiceJucator].orasePosbilAlimentare<3 && 1<=jucatori[indiceJucator].orasePosbilAlimentare)
            SMR = SMR + 5;
    }
    if(stage==2){
        if(12<jucatori[indiceJucator].orasePosbilAlimentare)
            SMR = SMR + 0.5;
        if(10<jucatori[indiceJucator].orasePosbilAlimentare && jucatori[indiceJucator].orasePosbilAlimentare<=12)
            SMR = SMR + 2;
        if(7<=jucatori[indiceJucator].orasePosbilAlimentare && jucatori[indiceJucator].orasePosbilAlimentare<=10)
            SMR = SMR + 5;
        if(jucatori[indiceJucator].orasePosbilAlimentare<=6)
            SMR = SMR + 10;
    }
    if(stage==3){
        if(16<=jucatori[indiceJucator].orasePosbilAlimentare && jucatori[indiceJucator].orasePosbilAlimentare<=18)
            SMR = SMR + 0.5;
        if(14<=jucatori[indiceJucator].orasePosbilAlimentare && jucatori[indiceJucator].orasePosbilAlimentare<16)
            SMR = SMR + 1;
        if(12<=jucatori[indiceJucator].orasePosbilAlimentare && jucatori[indiceJucator].orasePosbilAlimentare <14)
            SMR = SMR + 1.5;
        if(jucatori[indiceJucator].orasePosbilAlimentare<12)
            SMR = SMR + 5;
    }
    int distantaNrOraseAlim = -1;
    for(int i=0; i<nrJucatori; i++){
        if(jucatori[i].orasePosbilAlimentare>jucatori[indiceJucator].orasePosbilAlimentare && i!=indiceJucator)
            distantaNrOraseAlim = jucatori[i].orasePosbilAlimentare;
    }
    if(distantaNrOraseAlim-jucatori[indiceJucator].orasePosbilAlimentare==jucatori[indiceJucator].orasePosbilAlimentare)
        SMR = SMR + 1;
    if(distantaNrOraseAlim-jucatori[indiceJucator].orasePosbilAlimentare<jucatori[indiceJucator].orasePosbilAlimentare)
        SMR = SMR + 0.5;
    if(distantaNrOraseAlim-jucatori[indiceJucator].orasePosbilAlimentare>jucatori[indiceJucator].orasePosbilAlimentare){
        if(distantaNrOraseAlim==18)
            SMR = SMR+10.5;
        else{
                SMR = SMR + (0.5*distantaNrOraseAlim-jucatori[indiceJucator].orasePosbilAlimentare);
        }
    }
    if(0<=SMR && SMR<=6){
        SetConsoleTextAttribute(h,4);
        cout<<"⚠ NU"<<endl;
    }
    if(6<SMR && SMR<=9.5){
        SetConsoleTextAttribute(h, 14);
        cout<<"INDECIS"<<endl;
    }
    if(9.5<=SMR){
        SetConsoleTextAttribute(h, 2);
        cout<<"✓DA"<<endl;
    }
    SetConsoleTextAttribute(h, 15);
}

void recomandareCareCentralaCump(centrala &centInUz,jucator_ jucatori[],int nrJucatori,int stage,HANDLE h,combustibil comb[])
{
    int auxComb;
    if(strcmp(centInUz.numeComb,"C")==0)
        auxComb = 0;
    if(strcmp(centInUz.numeComb,"A")==0)
        auxComb = 1;
    if(strcmp(centInUz.numeComb,"P")==0)
        auxComb = 2;
    if(strcmp(centInUz.numeComb,"AP")==0)
        auxComb = 1;
    if(strcmp(centInUz.numeComb,"N")==0)
        auxComb = 4;
    int jucatoriCuAcelasiTipCombustibil = 0;
    for(int i=0; i<nrJucatori; i++){
        for(int j=0; j<jucatori[i].contorCenDet; j++){
            if(auxComb==0)
                if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"C")==0)
                    jucatoriCuAcelasiTipCombustibil++;
            if(auxComb==1)
                if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"A")==0)
                    jucatoriCuAcelasiTipCombustibil++;
            if(auxComb==2)
                if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"P")==0)
                    jucatoriCuAcelasiTipCombustibil++;
            if(auxComb==4)
                if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"N")==0)
                    jucatoriCuAcelasiTipCombustibil++;
        }
    }
    centInUz.SMC = centInUz.SMC - (jucatoriCuAcelasiTipCombustibil/2);
}

void citireCentraleInUz(centrala centInUz[9],jucator_ jucatori[],int &nrJucatori,int stage,centrala centrale[],HANDLE h,combustibil comb[])
{
    int centPePiata, nAux = 0, centraleCump;
    float auxSMC[45];
    char auxNum[45][45];
    int aux2, aux3;
    centrala centraleInUz[9];
    cout<<"Dati numarul de centrale ce se afla pe piata."<<endl;
    cin>>centPePiata;
    cin.ignore();
    for(int i=0; i<centPePiata; i++){
        if(i+1!=1)
            cout<<"Dati a"<<i+1<<"-a centrala: ";
        else
            cout<<"Dati prima centrala: ";
        cin.getline(centInUz[i].numeCen, 9);
        for(int j=0; j<42; j++){
            if(strcmp(centrale[j].numeCen,centInUz[i].numeCen)==0){
                int ors = centrale[j].orase;
                if(centrale[j].orase<3)
                    ors = ors * -1 - (3-ors);
                if(strcmp(centrale[j].numeComb,"C")==0){
                    centrale[j].SMC = comb[0].meta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(comb[0])) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"A")==0){
                    centrale[j].SMC = comb[1].meta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(comb[1])) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"P")==0){
                    centrale[j].SMC = comb[2].meta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(comb[2])) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"AP")==0){
                    centrale[j].SMC = comb[3].meta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(comb[3])) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"N")==0){
                    centrale[j].SMC = comb[4].meta[stage] + ors - (centrale[j].combNecesar+gasirePrimPret(comb[4])) - (centrale[j].pret/10);
                }
                if(strcmp(centrale[j].numeComb,"E")==0){
                    centrale[j].SMC = comb[5].meta[stage] + ors - (centrale[j].pret/10);
                }
                auxSMC[nAux] = centrale[j].SMC;
                strcpy(auxNum[nAux], centrale[j].numeCen);
                centraleInUz[nAux].pret = centrale[j].pret;
                centraleInUz[nAux].orase = centrale[j].orase;
                centraleInUz[nAux].combNecesar = centrale[j].combNecesar;
                strcpy(centraleInUz[nAux].numeComb, centrale[j].numeComb);
                strcpy(centraleInUz[nAux].numeCen, centrale[j].numeCen);
                nAux++;
            }
        }
    }
    for(int i=0; i<nAux; i++){
        if(strcmp(centraleInUz[i].numeComb,"C")==0){
            SetConsoleTextAttribute(h, comb[0].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"A")==0){
            SetConsoleTextAttribute(h, comb[1].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"P")==0){
            SetConsoleTextAttribute(h, comb[2].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"AP")==0){
            SetConsoleTextAttribute(h, comb[3].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"N")==0){
            SetConsoleTextAttribute(h, comb[4].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"E")==0){
            SetConsoleTextAttribute(h, comb[5].culoareComb);
        }
        cout<<i+1<<") "<<centraleInUz[i].numeCen<<" ";
        SetConsoleTextAttribute(h, 15);
        recomandareCareCentralaCump(centrale[i],jucatori,nrJucatori,stage,h,comb);
        if(8<=centrale[i].SMC){
            SetConsoleTextAttribute(h, 2);
            cout<<"✓ DA ";
            SetConsoleTextAttribute(h, 10);
            cout<<centrale[i].SMC<<endl;
            SetConsoleTextAttribute(h, 15);
        }
        else{
            if(5<=centrale[i].SMC && centrale[i].SMC<8){
                SetConsoleTextAttribute(h, 14);
                cout<<"INDECIS "<<endl;
                SetConsoleTextAttribute(h, 10);
                cout<<centrale[i].SMC<<endl;
            }
            else{
                SetConsoleTextAttribute(h, 4);
                cout<<"⚠ NU "<<endl;
                SetConsoleTextAttribute(h, 10);
                cout<<centrale[i].SMC<<endl;
            }
        }
    }
    SetConsoleTextAttribute(h,15);
    for(int i=0; i<nAux; i++){
        if(strcmp(centraleInUz[i].numeComb,"C")==0){
            SetConsoleTextAttribute(h, comb[0].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"A")==0){
            SetConsoleTextAttribute(h, comb[1].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"P")==0){
            SetConsoleTextAttribute(h, comb[2].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"AP")==0){
            SetConsoleTextAttribute(h, comb[3].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"N")==0){
            SetConsoleTextAttribute(h, comb[4].culoareComb);
        }
        if(strcmp(centraleInUz[i].numeComb,"E")==0){
            SetConsoleTextAttribute(h, comb[5].culoareComb);
        }
        cout<<i+1<<") "<<centraleInUz[i].numeCen<<" ";
        SetConsoleTextAttribute(h, 15);
    }
    cout<<endl;
    cout<<"Cate centrale au fost cumparate?"<<endl;
    cin>>centraleCump;
    for(int i=0; i<centraleCump; i++){
        cout<<"Care centrala a fost cumparata?"<<endl;
        cin>>aux2;
        aux2 = aux2-1;
        cout<<"Care jucator a cumparat centrala?"<<endl;
        for(int j=0; j<nrJucatori; j++){
            SetConsoleTextAttribute(h, detectareCuloare(jucatori[i]));
            cout<<j+1<<") "<<jucatori[j].nume<<" ";
            SetConsoleTextAttribute(h, 15);
        }
        cin>>aux3;
        if(1<=aux3 && aux3<=6){
            jucatori[aux3-1].centraleDetinute[jucatori[aux3-1].contorCenDet] = centraleInUz[aux2];
            jucatori[aux3-1].bani = jucatori[aux3-1].bani - jucatori[aux3-1].centraleDetinute[jucatori[aux3-1].contorCenDet].pret;
            jucatori[aux3-1].contorCenDet++;
            jucatori[aux3-1].orasePosbilAlimentare = jucatori[aux3-1].centraleDetinute[jucatori[aux3-1].contorCenDet].orase;
        }
        else{
            SetConsoleTextAttribute(h, 4);
            cout<<"⚠ Eroare!(0004) ";
            SetConsoleTextAttribute(h, 14);
            cout<<"Jucator necunoscut, for-ul se va repeta la final"<<endl;
            i--;
            SetConsoleTextAttribute(h, 15);
        }
    }
}

void schimbareCombustibil(combustibil comb,int cateAdaug)
{
    for(int i=comb.x-1; i>=0; i--){
        for(int j=comb.y-1; j>=0; j--){
            if(cateAdaug==0)
                break;
            else{
                if(comb.preturi[i][j]==0){
                    comb.preturi[i][j] = 1;
                    cateAdaug--;
                }
            }
        }
    }
}

void eliminareCombustibil(combustibil comb,int cateDeElimin)
{
    for(int i=0; i<comb.x; i++){
        for(int j=0; j<comb.y; j++){
            if(cateDeElimin==0)
                break;
            else{
                if(comb.preturi[i][j]==1){
                    comb.preturi[i][j] = 0;
                    cateDeElimin--;
                }
            }
        }
    }
}

int verificarePretComb(combustibil comb,int cateVerificare)
{
    int pretFin = 1;
    for(int i=0; i<comb.x; i++){
        for(int j=0; j<comb.y; j++){
            if(cateVerificare==0)
                break;
            else{
                if(comb.preturi[i][j]==1){
                    pretFin = pretFin + i;
                    cateVerificare--;
                }
            }
        }
    }
    return pretFin;
}

void cataSchimbareCombustibil(int nrJucatori,int stage,combustibil comb[],HANDLE h)
{
    int resurseDeAdaug, n=4;
    for(int i=0; i<n; i++){
        switch(i){
            case 0:
                cout<<"Cat ";
                SetConsoleTextAttribute(h, comb[0].culoareComb);
                cout<<"carbune";
                SetConsoleTextAttribute(h, 15);
                cout<<":";
                cin>>resurseDeAdaug;
                schimbareCombustibil(comb[0], resurseDeAdaug);
                break;
            case 1:
                cout<<"Cata ";
                SetConsoleTextAttribute(h, comb[1].culoareComb);
                cout<<"apa";
                SetConsoleTextAttribute(h, 15);
                cout<<":";
                cin>>resurseDeAdaug;
                schimbareCombustibil(comb[1], resurseDeAdaug);
                break;
            case 2:
                cout<<"Cat ";
                SetConsoleTextAttribute(h, comb[2].culoareComb);
                cout<<"petrol";
                SetConsoleTextAttribute(h, 15);
                cout<<":";
                cin>>resurseDeAdaug;
                schimbareCombustibil(comb[2], resurseDeAdaug);
                break;
            case 3:
                cout<<"Cat ";
                SetConsoleTextAttribute(h, comb[4].culoareComb);
                cout<<"uraniu";
                SetConsoleTextAttribute(h, 15);
                cout<<":";
                cin>>resurseDeAdaug;
                schimbareCombustibil(comb[4], resurseDeAdaug);
                break;
        }
    }
}

void cumparareCombustibil(HANDLE h,int stage,int numarJucatori,jucator_ jucatori[],int &baniDentinuti,int baniDetAltiJucatori[][1],combustibil comb[])
{
    bool automat = false, maxim = false, faraBani = false, combGasit = false, greseala = false;
    char raspuns[255], raspuns2[255];
    int deCump,aux,preturi;
    SetConsoleTextAttribute(h, 14);
    cout<<"Doriti ca acesta functie sa ruleze automat?"<<endl;
    SetConsoleTextAttribute(h, 15);
    cin.getline(raspuns, 255);
    toLower(raspuns);
    if(strcmp(raspuns, "da")==0||strcmp(raspuns, "afirmativ")==0||strcmp(raspuns, "desigur")==0||strcmp(raspuns, "fireste")==0||strcmp(raspuns, "dar")==0||strcmp(raspuns, "helbet")==0){
        automat = true;
    }
    SetConsoleTextAttribute(h, 14);
    cout<<"Doriti sa fie cumparat maxim de combustibil sau minim?"<<endl;
    SetConsoleTextAttribute(h, 15);
    if(strcmp(raspuns, "min")==0||strcmp(raspuns, "minim")==0){
        maxim = false;
    }
    if(strcmp(raspuns, "max")==0||strcmp(raspuns, "maxim")==0){
        maxim = false;
    }
    SetConsoleTextAttribute(h, 15);
    for(int i=numarJucatori-1; i>=0; i--){
        preturi = 0;
        faraBani = false;
        for(int j=0; j<jucatori[i].contorCenDet; j++){
            if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"C")==0){
                aux = 0;
            }
            if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"A")==0){
                aux = 1;
            }
            if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"P")==0){
                aux = 2;
            }
            if(strcmp(jucatori[i].centraleDetinute[j].numeComb,"N")==0){
                aux = 3;
            }
            if(automat==true){
                if(maxim == false)
                    deCump = jucatori[i].centraleDetinute[j].combNecesar - jucatori[i].combDetinut[aux][0];
                else
                    deCump = 2*jucatori[i].centraleDetinute[j].combNecesar - jucatori[i].combDetinut[aux][0];
                switch(aux) {
                    case 0:
                        preturi = verificarePretComb(comb[0], deCump);
                        break;
                    case 1:
                        preturi = verificarePretComb(comb[1], deCump);
                        break;
                    case 2:
                        preturi = verificarePretComb(comb[2], deCump);
                        break;
                    case 3:
                        preturi = verificarePretComb(comb[4], deCump);
                        break;
                }
                if(preturi>jucatori[i].bani){
                    SetConsoleTextAttribute(h, 4);
                    cout<<"⚠ Eroare!(0005) ";
                    SetConsoleTextAttribute(h, 14);
                    cout<<"Jucatorul selectat nu are banii necesari pentru cumparea resurselor, aceasta iteratie a for-ului va incepe in mod manual!"<<endl;
                    SetConsoleTextAttribute(h, 15);
                    faraBani = true;
                }
                if(faraBani==false){
                    switch(aux) {
                        case 0:
                            eliminareCombustibil(comb[0], deCump);
                            break;
                        case 1:
                            eliminareCombustibil(comb[1], deCump);
                            break;
                        case 2:
                            eliminareCombustibil(comb[2], deCump);
                            break;
                        case 3:
                            eliminareCombustibil(comb[4], deCump);
                            break;
                    }
                    jucatori[i].bani = jucatori[i].bani - preturi;
                    jucatori[i].combDetinut[aux][0] = jucatori[i].combDetinut[aux][0] + deCump;
                }
            }
            if(automat==false || faraBani==true){
                cout<<"Cat combustibil doreste ";
                SetConsoleTextAttribute(h, detectareCuloare(jucatori[i]));
                cout<<jucatori[i].nume;
                SetConsoleTextAttribute(h, 15);
                cout<<" sa cumpere?"<<endl;
                cin>>deCump;
                cout<<"Ce combustibil doreste ";
                SetConsoleTextAttribute(h, detectareCuloare(jucatori[i]));
                cout<<jucatori[i].nume;
                SetConsoleTextAttribute(h, 15);
                cout<<" sa cumpere?"<<endl;
                cin.ignore();
                cin.getline(raspuns, 255);
                toLower(raspuns);
                if(strcmp(raspuns,"c")==0 || strcmp(raspuns,"carbune")==0 || strcmp(raspuns,"asteorizi")==0){
                    cin.ignore();
                    cout<<"Combustibilul care urmeaza sa fie cumparat este: ";
                    SetConsoleTextAttribute(h, 6);
                    cout<<"carbune";
                    SetConsoleTextAttribute(h, 15);
                    cout<<", corect?"<<endl;
                    cin.getline(raspuns2, 255);
                    toLower(raspuns2);
                    if(strcmp(raspuns2, "nu")==0||strcmp(raspuns2, "negativ")==0||strcmp(raspuns2, "nicidecum")==0||strcmp(raspuns2, "deloc")==0||strcmp(raspuns2, "exclus")==0||strcmp(raspuns2, "nup")==0||strcmp(raspuns2, "de niciun fel")==0){
                        greseala = true;
                        SetConsoleTextAttribute(h, 4);
                        cout<<"⚠ Eroare!(0006) ";
                        SetConsoleTextAttribute(h, 14);
                        cout<<"Combustibilul a fost selectat gresit! For-ul se va repeta la finalul iteratiei cu acelasi indice."<<endl;
                        i--;
                        SetConsoleTextAttribute(h, 15);
                    }
                    else{
                        jucatori[i].bani = jucatori[i].bani - verificarePretComb(comb[0], deCump);
                        jucatori[i].combDetinut[0][0] = jucatori[i].combDetinut[0][0] + deCump;
                        eliminareCombustibil(comb[0], deCump);
                        combGasit = true;
                    }
                    SetConsoleTextAttribute(h, 15);
                }
                if(strcmp(raspuns,"a")==0 || strcmp(raspuns,"apa")==0 || strcmp(raspuns,"gaz")==0 ){
                    cin.ignore();
                    cout<<"Combustibilul care urmeaza sa fie cumparat este: ";
                    SetConsoleTextAttribute(h, 11);
                    cout<<"gaz";
                    SetConsoleTextAttribute(h, 15);
                    cout<<", corect?"<<endl;
                    cin.getline(raspuns2, 255);
                    toLower(raspuns2);
                    if(strcmp(raspuns2, "nu")==0||strcmp(raspuns2, "negativ")==0||strcmp(raspuns2, "nicidecum")==0||strcmp(raspuns2, "deloc")==0||strcmp(raspuns2, "exclus")==0||strcmp(raspuns2, "nup")==0||strcmp(raspuns2, "de niciun fel")==0){
                        greseala = true;
                        SetConsoleTextAttribute(h, 4);
                        cout<<"⚠ Eroare!(0006) ";
                        SetConsoleTextAttribute(h, 14);
                        cout<<"Combustibilul a fost selectat gresit! For-ul se va repeta la finalul iteratiei cu acelasi indice."<<endl;
                        i--;
                        SetConsoleTextAttribute(h, 15);
                    }
                    else{
                        jucatori[i].bani = jucatori[i].bani - verificarePretComb(comb[1], deCump);
                        jucatori[i].combDetinut[1][0] = jucatori[i].combDetinut[1][0] + deCump;
                        eliminareCombustibil(comb[1], deCump);
                        combGasit = true;
                    }
                    SetConsoleTextAttribute(h, 15);
                }
                if(strcmp(raspuns,"p")==0 || strcmp(raspuns,"petrol")==0 || strcmp(raspuns,"petro")==0 ){
                    cin.ignore();
                    cout<<"Combustibilul care urmeaza sa fie cumparat este: ";
                    SetConsoleTextAttribute(h, 8);
                    cout<<"petrol";
                    SetConsoleTextAttribute(h, 15);
                    cout<<", corect?"<<endl;
                    cin.getline(raspuns2, 255);
                    toLower(raspuns2);
                    if(strcmp(raspuns2, "nu")==0||strcmp(raspuns2, "negativ")==0||strcmp(raspuns2, "nicidecum")==0||strcmp(raspuns2, "deloc")==0||strcmp(raspuns2, "exclus")==0||strcmp(raspuns2, "nup")==0||strcmp(raspuns2, "de niciun fel")==0){
                        greseala = true;
                        SetConsoleTextAttribute(h, 4);
                        cout<<"⚠ Eroare!(0006) ";
                        SetConsoleTextAttribute(h, 14);
                        cout<<"Combustibilul a fost selectat gresit! For-ul se va repeta la finalul iteratiei cu acelasi indice."<<endl;
                        i--;
                        SetConsoleTextAttribute(h, 15);
                    }
                    else{
                        jucatori[i].bani = jucatori[i].bani - verificarePretComb(comb[2], deCump);
                        jucatori[i].combDetinut[2][0] = jucatori[i].combDetinut[2][0] + deCump;
                        eliminareCombustibil(comb[2], deCump);
                        combGasit = true;
                    }
                    SetConsoleTextAttribute(h, 15);
                }
                if(strcmp(raspuns,"n")==0 || strcmp(raspuns,"nuclear")==0 || strcmp(raspuns,"nucleara")==0){
                    cin.ignore();
                    cout<<"Combustibilul care urmeaza sa fie cumparat este: ";
                    SetConsoleTextAttribute(h, 12);
                    cout<<"uraniu";
                    SetConsoleTextAttribute(h, 15);
                    cout<<", corect?"<<endl;
                    cin.getline(raspuns2, 255);
                    toLower(raspuns2);
                    if(strcmp(raspuns2, "nu")==0||strcmp(raspuns2, "negativ")==0||strcmp(raspuns2, "nicidecum")==0||strcmp(raspuns2, "deloc")==0||strcmp(raspuns2, "exclus")==0||strcmp(raspuns2, "nup")==0||strcmp(raspuns2, "de niciun fel")==0){
                        greseala = true;
                        SetConsoleTextAttribute(h, 4);
                        cout<<"⚠ Eroare!(0006) ";
                        SetConsoleTextAttribute(h, 14);
                        cout<<"Combustibilul a fost selectat gresit! For-ul se va repeta la finalul iteratiei cu acelasi indice."<<endl;
                        i--;
                        SetConsoleTextAttribute(h, 15);
                    }
                    else{
                        jucatori[i].bani = jucatori[i].bani - verificarePretComb(comb[4], deCump);
                        jucatori[i].combDetinut[3][0] = jucatori[i].combDetinut[3][0] + deCump;
                        eliminareCombustibil(comb[4], deCump);
                        combGasit = true;
                    }
                    SetConsoleTextAttribute(h, 15);
                }
                if(combGasit==false){
                    SetConsoleTextAttribute(h, 4);
                    cout<<"⚠ Eroare!(0007)";
                    SetConsoleTextAttribute(h, 14);
                    cout<<" Combustibilul cautat nu a fost gasit/nu exista, aceasta iteratie a for-ului se va repeta la final cu acelasi indice."<<endl;
                    i--;
                    SetConsoleTextAttribute(h, 15);
                }
            }
        }
    }
}

void afisareSMO(hartaEU hE,HANDLE h)
{
    for(int a=0; a<hE.i; a++){
        for(int b=0; b<hE.j[a]; b++){
            SetConsoleTextAttribute(h, 15);
            cout<<hE.orase[a][hE.oX[a][b]].numeOras<<" ";
            if(-10<=hE.orase[a][hE.oX[a][b]].SMO && hE.orase[a][hE.oX[a][b]].SMO<=1.5){
                SetConsoleTextAttribute(h, 4);
                cout<<hE.orase[a][hE.oX[a][b]].SMO<<endl;
            }
            else{
                if(1.5<hE.orase[a][hE.oX[a][b]].SMO && hE.orase[a][hE.oX[a][b]].SMO<=3){
                    SetConsoleTextAttribute(h, 14);
                    cout<<hE.orase[a][hE.oX[a][b]].SMO<<endl;
                }
                else{
                    SetConsoleTextAttribute(h, 2);
                    cout<<hE.orase[a][hE.oX[a][b]].SMO<<endl;
                }
            }
            SetConsoleTextAttribute(h, 15);
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
    jucator_ jucatori[6];
    combustibil comb[7];
    strcpy(comb[0].numeComb,"carbune");
    comb[0].culoareComb = 6;
    comb[0].x = 9;
    comb[0].y = 4;
    comb[0].combTotal = 27;
    comb[0].preturi = {{0},{0},{0, 0, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 1, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1}};
    comb[0].meta = {5, 10, 5};
    strcpy(comb[1].numeComb,"apa");
    comb[1].culoareComb = 3;
    comb[1].x = 8;
    comb[1].y = 3;
    comb[1].combTotal = 24;
    comb[1].preturi = {{0},{0, 0, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1},{1, 1, 1}};
    comb[1].meta = {6, 8, 10};
    strcpy(comb[2].numeComb,"petrol");
    comb[2].culoareComb = 8;
    comb[2].x = 8;
    comb[2].y = 1;
    comb[2].combTotal = 21;
    comb[2].preturi = {{0},{0, 0, 0, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{0, 0, 1, 1},{1, 1, 1, 1}};
    comb[2].meta = {6, 7, 9};
    strcpy(comb[3].numeComb,"apapetrol");
    comb[3].culoareComb = 1;
    comb[3].preturi = {{0},{0, 0, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1, 1},{1, 1, 1, 1}}
    comb[3].x = 8;
    comb[3].y = 5;
    comb[3].combTotal = 45;
    comb[3].meta = {7, 9, 11};
    strcpy(comb[4].numeComb,"nuclear");
    comb[4].culoareComb = 12;
    comb[4].preturi = {{0},{0},{0},{0},{0},{0},{0, 1},{1, 1},{1, 1}};
    comb[4].x = 8;
    comb[4].y = 2;
    comb[4].combTotal = 12;
    comb[4].meta = {3, 3, 5};
    strcpy(comb[5].numeComb,"eco");
    comb[5].culoareComb = 10;
    comb[5].preturi = {{1}}
    comb[5].x = 1;
    comb[5].y = 1;
    comb[5].combTotal = 0;
    comb[5].meta = {10,10,10};
    //citireJucatori(jucatori, numarJucatori, h);
    //citireCentrale(centrale);
    //citireCentraleInUz(centInUz,jucatori,numarJucatori,stage,centrale,h,comb);
    //cataSchimbareCombustibil(numarJucatori,stage,comb);
    //citireNumeHartaEU(hartaEuropa);
    //citireNrConexHartaEu(hartaEuropa);
    //citireNumeConexHarta(hartaEuropa);
    //citirePretConexHarta(hartaEuropa);
    //calculareSMCx(hartaEuropa);
    //calculareSMO(hartaEuropa);
    //afisareSMO(hartaEuropa, h);
    //afisareHartaEu(hartaEuropa);
    return 0;
}
