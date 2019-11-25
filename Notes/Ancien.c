#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>



#define A 47
#define C 23
#define X0 44
#define M 32765
#define N 10000
const double KHI[] = { 3.8415,5.9915,7.8147,9.4877,11.0705,12.5916, 14.0671, 15.5073, 16.9190, 18.3070,19.6752, 21.0261, 22.3620, 23.6848, 24.9958, 26.2962, 27.5871, 28.8693, 30.1435, 31,4104 };
#define TEMPS_SIM 960
#define sMin 4
#define sMax 30
#define TAILLETAB 6
const int ri[TAILLETAB] = { 16, 20, 12, 5, 3, 2 };
#define COUT_CLIENT_PRIORITAIRE 40/60
#define COUT_CLIENT_ORDINAIRE 25/60
#define COUT_STATION_OCCUPE 35/60
#define COUT_STATION_INOCCUPE 20/60

typedef struct TabCarr�Unit� {
	long double classe;
	long double pi;
} TabCarr�Unit�;

typedef struct TabCarr�Unit�Classe {
	long double classe;
	long double pi;
	int ri;
} TabCarr�Unit�Classe;

typedef struct Stations {
	int dur�eService;
	bool estPrioritaire;
}Stations;

bool initFormule();
void simulation(int nbStations, double couts[]);
void initTabPiCarr�Unit�(TabCarr�Unit� tabCarr�Unit� []);
int initTabClasseCarr�Unit�(TabCarr�Unit� tabCarr�Unit�[], TabCarr�Unit�Classe tabCarr�Unit�Classe[]);
int g�n�rerNombreAl�atoire();
void g�n�rerArriv�es(int* nbNouveauxClientsOrdinaires, int* nbNouveauxClientsPrioritaires);
int g�n�rerDur�eService();
int formuleDePoisson(int x);
int fact(int n);
void rechercheStationIdeale(double couts[]);
void sortieInfosFilesStations(Stations stations[], int nbNouveauxClientsOrdinaires, int nbNouveauxClientsPrioritaires,int nbClientsOrdinairesDansFile, int nbClientPrioritairesDansFile, int nbStations);

int x;

void main(void)
{
	if (initFormule()) {
		int nbStations = sMin;
		double couts[sMax - sMin + 1];
		while (nbStations <= sMax) {
			simulation(nbStations, couts);
			nbStations++;
		}
		rechercheStationIdeale(couts);
	}
	else {
		printf("KO Formule");
	}
	getchar();
}

bool initFormule(){
	int i = 0;
	TabCarr�Unit� tabCarr�Unit�[20];
	TabCarr�Unit�Classe tabCarr�Unit�Classe[20];
	initTabPiCarr�Unit�(tabCarr�Unit�);
	int nbClasses = initTabClasseCarr�Unit�(tabCarr�Unit�, tabCarr�Unit�Classe);
	x = X0;
	while (i < N) {
		int j = 0;
		double nbr[4];
		while (j < 4) {
			x = g�n�rerNombreAl�atoire();
			nbr[j] = (double) x / M;
			j++;
			i++;
		}
		double d = (nbr[3] - nbr[1])*(nbr[3] - nbr[1]) + (nbr[2] - nbr[0])*(nbr[2] - nbr[0]);
		int iCarr�Unit� = nbClasses - 1;
		bool estPlace = false;
		while (!estPlace) {
			if (d < tabCarr�Unit�Classe[iCarr�Unit�].classe) {
				tabCarr�Unit�Classe[iCarr�Unit�].ri++;
				estPlace = true;
			}
			iCarr�Unit�--;
		}
	}

	double sommeEcartsCarr� = 0;
	int iCarr�Unit� = 0;
	while (iCarr�Unit� < nbClasses) {
		sommeEcartsCarr� += (tabCarr�Unit�Classe[iCarr�Unit�].ri - tabCarr�Unit�Classe[iCarr�Unit�].pi * (N/4))*(tabCarr�Unit�Classe[iCarr�Unit�].ri - tabCarr�Unit�Classe[iCarr�Unit�].pi * (N / 4)) / (tabCarr�Unit�Classe[iCarr�Unit�].pi * N);
		iCarr�Unit�++;
	}
	return sommeEcartsCarr� <= KHI[nbClasses - 1];
}

void initTabPiCarr�Unit�(TabCarr�Unit� tabCarr�Unit�[]) {
	tabCarr�Unit�[0].classe = 0.1;
	tabCarr�Unit�[0].pi = 0.23483;
	tabCarr�Unit�[1].classe = 0.2;
	tabCarr�Unit�[1].pi = 0.17497;
	tabCarr�Unit�[2].classe = 0.3;
	tabCarr�Unit�[2].pi = 0.13949;
	tabCarr�Unit�[3].classe = 0.4;
	tabCarr�Unit�[3].pi = 0.11271;
	tabCarr�Unit�[4].classe = 0.5;
	tabCarr�Unit�[4].pi = 0.09097;
	tabCarr�Unit�[5].classe = 0.6;
	tabCarr�Unit�[5].pi = 0.07261;
	tabCarr�Unit�[6].classe = 0.7;
	tabCarr�Unit�[6].pi = 0.05675;
	tabCarr�Unit�[7].classe = 0.8;
	tabCarr�Unit�[7].pi = 0.04281;
	tabCarr�Unit�[8].classe = 0.9;
	tabCarr�Unit�[8].pi = 0.03043;
	tabCarr�Unit�[9].classe = 1;
	tabCarr�Unit�[9].pi = 0.01933;
	tabCarr�Unit�[10].classe = 1.1;
	tabCarr�Unit�[10].pi = 0.01078;
	tabCarr�Unit�[11].classe = 1.2;
	tabCarr�Unit�[11].pi = 0.00634;
	tabCarr�Unit�[12].classe = 1.3;
	tabCarr�Unit�[12].pi = 0.00374;
	tabCarr�Unit�[13].classe = 1.4;
	tabCarr�Unit�[13].pi = 0.00214;
	tabCarr�Unit�[14].classe = 1.5;
	tabCarr�Unit�[14].pi = 0.00116;
	tabCarr�Unit�[15].classe = 1.6;
	tabCarr�Unit�[15].pi = 0.00057;
	tabCarr�Unit�[16].classe = 1.7;
	tabCarr�Unit�[16].pi = 0.00025;
	tabCarr�Unit�[17].classe = 1.8;
	tabCarr�Unit�[17].pi = 0.000084;
	tabCarr�Unit�[18].classe = 1.9;
	tabCarr�Unit�[18].pi = 0.000018;
	tabCarr�Unit�[19].classe = 2;
	tabCarr�Unit�[19].pi = 0.000001;

}

int initTabClasseCarr�Unit�(TabCarr�Unit� tabCarr�Unit�[], TabCarr�Unit�Classe tabCarr�Unit�Classe[]) {
	int nbClasses = 0;
	int iTab = 19;
	int indiceClasse = iTab;
	double sommeNpi = 0;

	while (iTab >= 0) {
		sommeNpi += tabCarr�Unit�[iTab].pi * (N/4);
		if (sommeNpi >= 5) {
			tabCarr�Unit�Classe[nbClasses].classe = tabCarr�Unit�[indiceClasse].classe;
			tabCarr�Unit�Classe[nbClasses].ri = 0;
			tabCarr�Unit�Classe[nbClasses].pi = sommeNpi /(N/4);
			indiceClasse = iTab - 1;
			sommeNpi = 0;
			nbClasses++;
		}
		iTab--;
	}
	return nbClasses;
}

int g�n�rerNombreAl�atoire() {
	x = (A * x + C) % M;
	return x;
}

void simulation(int nbStations, double couts[]) {
	int nbClientsOrdinairesDansFile = 0;
	int nbClientsPrioritairesDansFile = 0;
	int temps = 0;
	double coutClientOrdinaire = 0;
	double coutClientPrioritaire = 0;
	double coutStationOccup�e = 0;
	double coutStationInnoccup�e = 0;
	Stations stations[sMax];

	while (temps < TEMPS_SIM) {
		int iStation = 0;
		int nbNouveauxClientsOrdinaires;
		int nbNouveauxClientsPrioritaires;
		g�n�rerArriv�es(&nbNouveauxClientsOrdinaires, &nbNouveauxClientsPrioritaires);
		nbClientsOrdinairesDansFile += nbNouveauxClientsOrdinaires;
		nbClientsPrioritairesDansFile += nbNouveauxClientsPrioritaires;
		if ((nbStations == sMin || nbStations == 5 + sMin) && temps <= 20) {
			printf("Nombre de nouveaux clients ordinaire : %d\nNombre de nouveaux clients prioritaire : %d\n\n", nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires);
			printf("Debut de minute : %d\n", temps);
			sortieInfosFilesStations(stations, nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires, nbClientsOrdinairesDansFile, nbClientsPrioritairesDansFile, nbStations);
			printf("\n-----------\n");
		}
		while (iStation < nbStations) {
			if (stations[iStation].dur�eService <= 0) {
				if (iStation == 0 || iStation == 1) {
					if (nbClientsPrioritairesDansFile > 0) {
						nbClientsPrioritairesDansFile--;
						int dur�eService = g�n�rerDur�eService();
						stations[iStation].dur�eService = dur�eService;
						stations[iStation].estPrioritaire = true;
						coutClientPrioritaire += (double) COUT_CLIENT_PRIORITAIRE;
						coutStationOccup�e += (double) COUT_STATION_OCCUPE;
					}
					else {
						coutStationInnoccup�e += (double)COUT_STATION_INOCCUPE;
					}
				}
				else {
					if (nbClientsOrdinairesDansFile > 0) {
						nbClientsOrdinairesDansFile--;
						int dur�eService = g�n�rerDur�eService();
						stations[iStation].dur�eService = dur�eService;
						stations[iStation].estPrioritaire = false;
						coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
						coutStationOccup�e += (double)COUT_STATION_OCCUPE;
					}
					else {
						if (nbClientsPrioritairesDansFile > 0) {
							nbClientsPrioritairesDansFile--;
							int dur�eService = g�n�rerDur�eService();
							stations[iStation].dur�eService = dur�eService;
							stations[iStation].estPrioritaire = true;
							coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
							coutStationOccup�e += (double)COUT_STATION_OCCUPE;
						}
						else {
							coutStationInnoccup�e += (double)COUT_STATION_INOCCUPE;
						}
					}
				}
			}
			else {
				coutStationOccup�e += (double)COUT_STATION_OCCUPE;
				if (stations[iStation].estPrioritaire) {
					coutClientPrioritaire += (double)COUT_CLIENT_PRIORITAIRE;
				}
				else {
					coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
				}
				stations[iStation].dur�eService--;
			}
			iStation++;
		}
		coutClientPrioritaire += nbClientsPrioritairesDansFile * (double) COUT_CLIENT_PRIORITAIRE;
		coutClientOrdinaire += nbClientsOrdinairesDansFile * (double) COUT_CLIENT_ORDINAIRE;
		if ((nbStations == sMin || nbStations == 5 + sMin) && temps <= 20) {
			printf("Fin de minute : %d\n", temps);
			sortieInfosFilesStations(stations, nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires, nbClientsOrdinairesDansFile, nbClientsPrioritairesDansFile, nbStations);
			printf("\n---------------------------------\n");
		}
		temps++;
	}
	couts[nbStations - sMin] = coutClientOrdinaire + coutClientPrioritaire + coutStationInnoccup�e + coutStationOccup�e;
	printf("coutClientOrdinaire : %lf\ncoutClientPrioritaire : %lf\ncoutStationInnoccup�e : %lf\ncoutStationOccup�e : %lf\nCout Total : \t%lf\n\n\n", coutClientOrdinaire, coutClientPrioritaire, coutStationInnoccup�e, coutStationOccup�e, couts[nbStations - sMin]);	
}

int formuleDePoisson(int x) {
	double lambda;
	int i = 0, sommeRi = 0, sommeXiRi = 0;
	for (i = 0; i < TAILLETAB; i++)
	{
		sommeRi += ri[i];
		sommeXiRi += (i*ri[i]);
	}
	lambda = sommeRi / sommeXiRi;

	double e = exp(lambda*-1);

	return e * pow(lambda, (double) x) / fact(x);
}

int fact(int n) {
	if (n == 0) {
		return 1;
	}
	return n * fact(n - 1);
}

void g�n�rerArriv�es(int* nbNouveauxClientsOrdinaires, int* nbNouveauxClientsPrioritaires) {
	int nbArriv�es;
	*nbNouveauxClientsOrdinaires = 0;
	*nbNouveauxClientsPrioritaires = 0;
	int i = 0;
	g�n�rerNombreAl�atoire();
	double uN = (double) x / M;
	if (uN < 0.3051) {
		nbArriv�es = 0;
	}
	else {
		if (uN < 0.6610)
		{
			nbArriv�es = 1;
		}
		else
		{
			if (uN < 0.9153)
			{
				nbArriv�es = 2;
			}
			else
			{
				if (uN < 0.9661)
				{
					nbArriv�es = 3;
				}
				else
				{
					if (uN < 0.9831)
					{
						nbArriv�es = 4;

					}
					else
					{
						nbArriv�es = 5;
					}
				}
			}
		}
	}
	while (i < nbArriv�es)
	{
		g�n�rerNombreAl�atoire();
		double uN = (double)x / M;
		if (uN < 0.3)
		{
			*nbNouveauxClientsPrioritaires +=1;
		}
		else
		{
			*nbNouveauxClientsOrdinaires +=1;
		}
		i++;
	}
}

int g�n�rerDur�eService() {
	int nbMinutes = 0;
	g�n�rerNombreAl�atoire();
	double uN = (double) x / M;
	if (uN < 0.3051) {
		nbMinutes = 1;
	}
	else
	{
		if (uN < 0.6610) {
			nbMinutes = 2;
		}
		else
		{
			if (uN < 0.9153)
			{
				nbMinutes = 3;
			}
			else
			{
				if (uN < 0.9661)
				{
					nbMinutes = 4;
				}
				else
				{
					if (uN < 0.9831)
					{
						nbMinutes = 5;
					}
					else
					{
						nbMinutes = 6;
					}
				}
			}
		}
	}
	return nbMinutes;
}

void rechercheStationIdeale(double couts[])
{
	double coutMin = couts[0];
	int stationIdeale = 1;
	int iRecherche = 0;
	while (iRecherche < (sMax - sMin + 1))
	{
		if (couts[iRecherche] < coutMin)
		{
			coutMin = couts[iRecherche];
			stationIdeale = iRecherche + sMin;
		}
		iRecherche++;
	}
	printf("La station id�ale N� : %d et son cout minimum : %lf", stationIdeale, coutMin);
}

void sortieInfosFilesStations(Stations stations[], int nbNouveauxClientsOrdinaires, int nbNouveauxClientsPrioritaires, int nbClientsOrdinairesDansFile, int nbClientPrioritairesDansFile, int nbStations) {
	printf("Nombre de clients ordinaire dans file: %d\nNombre de clients prioritaire dans file: %d\n\n", nbClientsOrdinairesDansFile, nbClientPrioritairesDansFile);

	//info en debut de minute pour comprendre le fonctionnement de la file
	int iStation = 0;
	printf("Info Stations :\n\n");
	while (iStation < nbStations)
	{
		if (stations[iStation].dur�eService > 0) // sortie des infos par station
		{
			printf("Station num�ro %d:\n", iStation + 1);
			if (stations[iStation].estPrioritaire)
				printf("Client Prioritaire\n");
			else
				printf("Client Ordinaire\n");
			printf("Duree de service : %d minutes\n\n",stations[iStation].dur�eService);
		}
		iStation++;
	}
}

