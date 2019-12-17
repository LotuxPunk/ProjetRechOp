#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>

#define COUT_CLIENT_PRIORITAIRE 40/60
#define COUT_CLIENT_ORDINAIRE 25/60
#define COUT_STATION_OCCUPE 35/60
#define COUT_STATION_INOCCUPE 20/60
#define COUT_CHANGEMENT_FILE 50
#define TEMPS_SIM 600

#define M 63
#define A 22 //a-1 62
#define C 4 //c-1 56
#define X0 19

#define sMin 4
#define sMax 30

#define INITIAL_CAPACITY 2
#define POISSON_CAPACITY 5

typedef struct Stations {
	int dureeService;
	bool estPrioritaire;
} Stations;

bool isHullDobell(int, int, int);
int pgcd(int, int);
bool initFormule();
void init(double []);
int fact(int);
void traitement(int, double[], double[]);
void genererArrivees(int*, int* , int* , double* , double[]);
int genererDureeService();
void genererNombreAleatoire();
void rechercheStationIdeale(double []);
void sortieInfosFilesStations(Stations [], int , int , int , int , int);

int x;

void main(void){
	double tabPoissonCumul[POISSON_CAPACITY];
	init(tabPoissonCumul);

	if(isHullDobell(A, C, M)){
		int nbStations = sMin;
		double couts[sMax - sMin + 1];
		while(nbStations <= sMax){
			x = X0;
			traitement(nbStations, couts, tabPoissonCumul);
			nbStations++;
		}
		rechercheStationIdeale(couts);
	}
	else{
		printf("La suite n'est pas valide");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

bool isHullDobell(int a, int c, int m)
{
	// return (pgcd(c, m) == 1) && ((a - 1) % (c - 1) == 0) && (m % 4 == 0 ? (a - 1) % 4 == 0 : true);  
	bool cond1 = pgcd(c, m) == 1;
	bool cond2 = (a - 1) % (c - 1) == 0;
	bool cond3;

	if (m % 4 == 0)
	{
		cond3 = ((a - 1) % 4 == 0);
	}
	else
	{
		cond3 = true;
	}
	printf("%d\n", (c - 1) % (a - 1));
	printf("%d\n", cond1);
	printf("%d\n", cond2);
	printf("%d\n", cond3);
	return cond1 && cond2 && cond3;
}

void init(double tabPoissonCumul[])
{
	int i = 0;
	double valPrec = 0;

	while(i < 5)
	{
		valPrec += pow(1.8, (double)i) * exp(-1.8) / fact(i);
		tabPoissonCumul[i] = valPrec;
		i += 1;
	}
	//todo vérifier test des courses ici
}

int fact(int n)
{
	if (n == 0)
	{
		return 1;
	}
	return n * fact(n - 1);
}

int pgcd(int a, int b)
{
	int r;
	while (b != 0)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

void traitement(int nbStations, double couts[], double tabPoisson[])
{
	int nbClientsOrdinairesDansFile = 0;
	int nbClientsPrioritairesDansFile = 0;
	int temps = 0;
	double coutClientOrdinaire = 0;
	double coutClientPrioritaire = 0;
	double coutStationOccupee = 0;
	double coutStationInnoccupee = 0;
	double coutChangementTypeClient = 0;

	Stations stations[sMax];

	while (temps < TEMPS_SIM)
	{
		
		int nbNouveauxClientsOrdinaires;
		int nbNouveauxClientsPrioritaires;

		genererArrivees(&nbNouveauxClientsOrdinaires, &nbNouveauxClientsPrioritaires, &nbClientsPrioritairesDansFile, &coutChangementTypeClient, tabPoisson);

		nbClientsOrdinairesDansFile += nbNouveauxClientsOrdinaires;

		if ((nbStations == sMin || nbStations == 5 + sMin) && temps <= 20)
		{
			printf("Nombre de nouveaux clients ordinaire : %d\nNombre de nouveaux clients prioritaire : %d\n\n", nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires);
			printf("Debut de minute : %d\n", temps);
			sortieInfosFilesStations(stations, nbNouveauxClientsOrdinaires, nbNouveauxClientsPrioritaires, nbClientsOrdinairesDansFile, nbClientsPrioritairesDansFile, nbStations);
			printf("\n-----------\n");
		}

		int iStation = 0;

		while (iStation < nbStations)
		{
			if (stations[iStation].dureeService == 0)
			{
				if (iStation == 0)
				{
					if (nbClientsPrioritairesDansFile > 0)
					{
						nbClientsPrioritairesDansFile--;
						int dureeService = genererDureeService();
						stations[iStation].dureeService = dureeService;
						stations[iStation].estPrioritaire = true;
						coutClientPrioritaire += (double) COUT_CLIENT_PRIORITAIRE;
						coutStationOccupee += (double) COUT_STATION_OCCUPE;
					}
					else {
						coutStationInnoccupee += (double)COUT_STATION_INOCCUPE;
					}
				}
				else
				{
					if (nbClientsOrdinairesDansFile > 0) {
						nbClientsOrdinairesDansFile--;
						int dureeService = genererDureeService();
						stations[iStation].dureeService = dureeService;
						stations[iStation].estPrioritaire = false;
						coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
						coutStationOccupee += (double)COUT_STATION_OCCUPE;
					}
					else {
						if (nbClientsPrioritairesDansFile > 0) {
							nbClientsPrioritairesDansFile--;
							int dureeService = genererDureeService();
							stations[iStation].dureeService = dureeService;
							stations[iStation].estPrioritaire = true;
							coutClientPrioritaire += (double)COUT_CLIENT_PRIORITAIRE;
							coutStationOccupee += (double)COUT_STATION_OCCUPE;
						}
						else {
							coutStationInnoccupee += (double)COUT_STATION_INOCCUPE;
						}
					}
				}
			}
			else {
				coutStationOccupee += (double)COUT_STATION_OCCUPE;
				if (stations[iStation].estPrioritaire) {
					coutClientPrioritaire += (double)COUT_CLIENT_PRIORITAIRE;
				}
				else {
					coutClientOrdinaire += (double)COUT_CLIENT_ORDINAIRE;
				}
				stations[iStation].dureeService--;
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
	couts[nbStations - sMin] = coutClientOrdinaire + coutClientPrioritaire + coutStationInnoccupee + coutStationOccupee + coutChangementTypeClient;
	printf("coutClientOrdinaire : %lf\ncoutClientPrioritaire : %lf\ncoutStationInnoccupee : %lf\ncoutStationOccupee : %lf\ncoutChangementFile : %lf\nCout Total : \t%lf\n\n\n", coutClientOrdinaire, coutClientPrioritaire, coutStationInnoccupee, coutStationOccupee, coutChangementTypeClient, couts[nbStations - sMin]);	
}


void genererArrivees(int* nbNouveauxClientsOrdinaires, int* nbNouveauxClientsPrioritaires, int* nbClientsPrioritairesDansFile, double* coutChangementTypeClient, double tabPoissonCumul[]) {
	int nbArrivees;
	*nbNouveauxClientsOrdinaires = 0;
	*nbNouveauxClientsPrioritaires = 0;
	
	genererNombreAleatoire();
	double uN = (double) x / M;
	int i;
	for(i = 0; i < 5 && tabPoissonCumul[i] < uN; i++);
	nbArrivees = i;
	i = 0;
	while (i < nbArrivees){
		genererNombreAleatoire();
		double uN = (double)x / M;
		if (uN < 0.3){
			if(*nbClientsPrioritairesDansFile == 5){
				*nbNouveauxClientsOrdinaires +=1;
				*coutChangementTypeClient += COUT_CHANGEMENT_FILE;
			}
			else{
				*nbNouveauxClientsPrioritaires +=1;
				*nbClientsPrioritairesDansFile +=1;
			}	
		}
		else{
			*nbNouveauxClientsOrdinaires +=1;
		}
		i++;
	}
}

int genererDureeService(){
	int nbMinutes = 0;
	genererNombreAleatoire();
	double uN = (double) x / M;
	if (uN < 0.3051) {
		nbMinutes = 1;
	}
	else{
		if (uN < 0.6610) {
			nbMinutes = 2;
		}
		else
		{
			if (uN < 0.9153){
				nbMinutes = 3;
			}
			else{
				if (uN < 0.9661){
					nbMinutes = 4;
				}
				else{
					if (uN < 0.9831){
						nbMinutes = 5;
					}
					else{
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
	printf("La station ideale N° : %d et son cout minimum : %lf", stationIdeale, coutMin);
}

void sortieInfosFilesStations(Stations stations[], int nbNouveauxClientsOrdinaires, int nbNouveauxClientsPrioritaires, int nbClientsOrdinairesDansFile, int nbClientPrioritairesDansFile, int nbStations) {
	printf("Nombre de clients ordinaire dans file: %d\nNombre de clients prioritaire dans file: %d\n\n", nbClientsOrdinairesDansFile, nbClientPrioritairesDansFile);

	//info en debut de minute pour comprendre le fonctionnement de la file
	int iStation = 0;
	printf("Info Stations :\n\n");
	while (iStation < nbStations)
	{
		if (stations[iStation].dureeService > 0) // sortie des infos par station
		{
			printf("Station numero %d:\n", iStation + 1);
			if (stations[iStation].estPrioritaire)
				printf("Client Prioritaire\n");
			else
				printf("Client Ordinaire\n");
			printf("Duree de service : %d minutes\n\n",stations[iStation].dureeService);
		}
		iStation++;
	}
}

void genererNombreAleatoire() {
	x = (A * x + C) % M;
}