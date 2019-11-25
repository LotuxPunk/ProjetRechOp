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

typedef struct TabCarréUnité {
	long double classe;
	long double pi;
} TabCarréUnité;

typedef struct TabCarréUnitéClasse {
	long double classe;
	long double pi;
	int ri;
} TabCarréUnitéClasse;

typedef struct Stations {
	int duréeService;
	bool estPrioritaire;
}Stations;

bool initFormule();
void simulation(int nbStations, double couts[]);
void initTabPiCarréUnité(TabCarréUnité tabCarréUnité []);
int initTabClasseCarréUnité(TabCarréUnité tabCarréUnité[], TabCarréUnitéClasse tabCarréUnitéClasse[]);
int générerNombreAléatoire();
void générerArrivées(int* nbNouveauxClientsOrdinaires, int* nbNouveauxClientsPrioritaires);
int générerDuréeService();
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
	TabCarréUnité tabCarréUnité[20];
	TabCarréUnitéClasse tabCarréUnitéClasse[20];
	initTabPiCarréUnité(tabCarréUnité);
	int nbClasses = initTabClasseCarréUnité(tabCarréUnité, tabCarréUnitéClasse);
	x = X0;
	while (i < N) {
		int j = 0;
		double nbr[4];
		while (j < 4) {
			x = générerNombreAléatoire();
			nbr[j] = (double) x / M;
			j++;
			i++;
		}
		double d = (nbr[3] - nbr[1])*(nbr[3] - nbr[1]) + (nbr[2] - nbr[0])*(nbr[2] - nbr[0]);
		int iCarréUnité = nbClasses - 1;
		bool estPlace = false;
		while (!estPlace) {
			if (d < tabCarréUnitéClasse[iCarréUnité].classe) {
				tabCarréUnitéClasse[iCarréUnité].ri++;
				estPlace = true;
			}
			iCarréUnité--;
		}
	}

	double sommeEcartsCarré = 0;
	int iCarréUnité = 0;
	while (iCarréUnité < nbClasses) {
		sommeEcartsCarré += (tabCarréUnitéClasse[iCarréUnité].ri - tabCarréUnitéClasse[iCarréUnité].pi * (N/4))*(tabCarréUnitéClasse[iCarréUnité].ri - tabCarréUnitéClasse[iCarréUnité].pi * (N / 4)) / (tabCarréUnitéClasse[iCarréUnité].pi * N);
		iCarréUnité++;
	}
	return sommeEcartsCarré <= KHI[nbClasses - 1];
}

void initTabPiCarréUnité(TabCarréUnité tabCarréUnité[]) {
	tabCarréUnité[0].classe = 0.1;
	tabCarréUnité[0].pi = 0.23483;
	tabCarréUnité[1].classe = 0.2;
	tabCarréUnité[1].pi = 0.17497;
	tabCarréUnité[2].classe = 0.3;
	tabCarréUnité[2].pi = 0.13949;
	tabCarréUnité[3].classe = 0.4;
	tabCarréUnité[3].pi = 0.11271;
	tabCarréUnité[4].classe = 0.5;
	tabCarréUnité[4].pi = 0.09097;
	tabCarréUnité[5].classe = 0.6;
	tabCarréUnité[5].pi = 0.07261;
	tabCarréUnité[6].classe = 0.7;
	tabCarréUnité[6].pi = 0.05675;
	tabCarréUnité[7].classe = 0.8;
	tabCarréUnité[7].pi = 0.04281;
	tabCarréUnité[8].classe = 0.9;
	tabCarréUnité[8].pi = 0.03043;
	tabCarréUnité[9].classe = 1;
	tabCarréUnité[9].pi = 0.01933;
	tabCarréUnité[10].classe = 1.1;
	tabCarréUnité[10].pi = 0.01078;
	tabCarréUnité[11].classe = 1.2;
	tabCarréUnité[11].pi = 0.00634;
	tabCarréUnité[12].classe = 1.3;
	tabCarréUnité[12].pi = 0.00374;
	tabCarréUnité[13].classe = 1.4;
	tabCarréUnité[13].pi = 0.00214;
	tabCarréUnité[14].classe = 1.5;
	tabCarréUnité[14].pi = 0.00116;
	tabCarréUnité[15].classe = 1.6;
	tabCarréUnité[15].pi = 0.00057;
	tabCarréUnité[16].classe = 1.7;
	tabCarréUnité[16].pi = 0.00025;
	tabCarréUnité[17].classe = 1.8;
	tabCarréUnité[17].pi = 0.000084;
	tabCarréUnité[18].classe = 1.9;
	tabCarréUnité[18].pi = 0.000018;
	tabCarréUnité[19].classe = 2;
	tabCarréUnité[19].pi = 0.000001;

}

int initTabClasseCarréUnité(TabCarréUnité tabCarréUnité[], TabCarréUnitéClasse tabCarréUnitéClasse[]) {
	int nbClasses = 0;
	int iTab = 19;
	int indiceClasse = iTab;
	double sommeNpi = 0;

	while (iTab >= 0) {
		sommeNpi += tabCarréUnité[iTab].pi * (N/4);
		if (sommeNpi >= 5) {
			tabCarréUnitéClasse[nbClasses].classe = tabCarréUnité[indiceClasse].classe;
			tabCarréUnitéClasse[nbClasses].ri = 0;
			tabCarréUnitéClasse[nbClasses].pi = sommeNpi /(N/4);
			indiceClasse = iTab - 1;
			sommeNpi = 0;
			nbClasses++;
		}
		iTab--;
	}
	return nbClasses;
}

int générerNombreAléatoire() {
	x = (A * x + C) % M;
	return x;
}

void simulation(int nbStations, double couts[]) {
	int nbClientsOrdinairesDansFile = 0;
	int nbClientsPrioritairesDansFile = 0;
	int temps = 0;
	double coutClientOrdinaire = 0;
	double coutClientPrioritaire = 0;
	double coutStationOccupée = 0;
	double coutStationInnoccupée = 0;
	Stations stations[sMax];

	while (temps < TEMPS_SIM) {
		int iStation = 0;
		int nbNouveauxClientsOrdinaires;
		int nbNouveauxClientsPrioritaires;
		générerArrivées(&nbNouveauxClientsOrdinaires, &nbNouveauxClientsPrioritaires);
		nbClientsOrdinairesDansFile += nbNouveauxClientsOrdinaires;
		nbClientsPrioritairesDansFile += nbNouveauxClientsPrioritaires;
		if ((nbStations == sMin || nbStations == 5 + sMin) && temps <= 20) {
			printf("Nombre de nouveaux clients ordinaire : %d\nNombre de nouveaux clients prioritaire : %d\n\n", nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires);
			printf("Debut de minute : %d\n", temps);
			sortieInfosFilesStations(stations, nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires, nbClientsOrdinairesDansFile, nbClientsPrioritairesDansFile, nbStations);
			printf("\n-----------\n");
		}
		while (iStation < nbStations) {
			if (stations[iStation].duréeService <= 0) {
				if (iStation == 0 || iStation == 1) {
					if (nbClientsPrioritairesDansFile > 0) {
						nbClientsPrioritairesDansFile--;
						int duréeService = générerDuréeService();
						stations[iStation].duréeService = duréeService;
						stations[iStation].estPrioritaire = true;
						coutClientPrioritaire += (double) COUT_CLIENT_PRIORITAIRE;
						coutStationOccupée += (double) COUT_STATION_OCCUPE;
					}
					else {
						coutStationInnoccupée += (double)COUT_STATION_INOCCUPE;
					}
				}
				else {
					if (nbClientsOrdinairesDansFile > 0) {
						nbClientsOrdinairesDansFile--;
						int duréeService = générerDuréeService();
						stations[iStation].duréeService = duréeService;
						stations[iStation].estPrioritaire = false;
						coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
						coutStationOccupée += (double)COUT_STATION_OCCUPE;
					}
					else {
						if (nbClientsPrioritairesDansFile > 0) {
							nbClientsPrioritairesDansFile--;
							int duréeService = générerDuréeService();
							stations[iStation].duréeService = duréeService;
							stations[iStation].estPrioritaire = true;
							coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
							coutStationOccupée += (double)COUT_STATION_OCCUPE;
						}
						else {
							coutStationInnoccupée += (double)COUT_STATION_INOCCUPE;
						}
					}
				}
			}
			else {
				coutStationOccupée += (double)COUT_STATION_OCCUPE;
				if (stations[iStation].estPrioritaire) {
					coutClientPrioritaire += (double)COUT_CLIENT_PRIORITAIRE;
				}
				else {
					coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
				}
				stations[iStation].duréeService--;
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
	couts[nbStations - sMin] = coutClientOrdinaire + coutClientPrioritaire + coutStationInnoccupée + coutStationOccupée;
	printf("coutClientOrdinaire : %lf\ncoutClientPrioritaire : %lf\ncoutStationInnoccupée : %lf\ncoutStationOccupée : %lf\nCout Total : \t%lf\n\n\n", coutClientOrdinaire, coutClientPrioritaire, coutStationInnoccupée, coutStationOccupée, couts[nbStations - sMin]);	
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

void générerArrivées(int* nbNouveauxClientsOrdinaires, int* nbNouveauxClientsPrioritaires) {
	int nbArrivées;
	*nbNouveauxClientsOrdinaires = 0;
	*nbNouveauxClientsPrioritaires = 0;
	int i = 0;
	générerNombreAléatoire();
	double uN = (double) x / M;
	if (uN < 0.3051) {
		nbArrivées = 0;
	}
	else {
		if (uN < 0.6610)
		{
			nbArrivées = 1;
		}
		else
		{
			if (uN < 0.9153)
			{
				nbArrivées = 2;
			}
			else
			{
				if (uN < 0.9661)
				{
					nbArrivées = 3;
				}
				else
				{
					if (uN < 0.9831)
					{
						nbArrivées = 4;

					}
					else
					{
						nbArrivées = 5;
					}
				}
			}
		}
	}
	while (i < nbArrivées)
	{
		générerNombreAléatoire();
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

int générerDuréeService() {
	int nbMinutes = 0;
	générerNombreAléatoire();
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
	printf("La station idéale N° : %d et son cout minimum : %lf", stationIdeale, coutMin);
}

void sortieInfosFilesStations(Stations stations[], int nbNouveauxClientsOrdinaires, int nbNouveauxClientsPrioritaires, int nbClientsOrdinairesDansFile, int nbClientPrioritairesDansFile, int nbStations) {
	printf("Nombre de clients ordinaire dans file: %d\nNombre de clients prioritaire dans file: %d\n\n", nbClientsOrdinairesDansFile, nbClientPrioritairesDansFile);

	//info en debut de minute pour comprendre le fonctionnement de la file
	int iStation = 0;
	printf("Info Stations :\n\n");
	while (iStation < nbStations)
	{
		if (stations[iStation].duréeService > 0) // sortie des infos par station
		{
			printf("Station numéro %d:\n", iStation + 1);
			if (stations[iStation].estPrioritaire)
				printf("Client Prioritaire\n");
			else
				printf("Client Ordinaire\n");
			printf("Duree de service : %d minutes\n\n",stations[iStation].duréeService);
		}
		iStation++;
	}
}

