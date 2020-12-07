#include <cmath>

#include "acm.hpp"

#include "citiesReader.h"

#include "prim-dijkstra.hpp"

void saveGraph(std::vector<acm::arrete> cities) {
	FILE* fileOut = NULL;
	fileOut = fopen("resuGraph.dat", "w");
	for (auto [i, j, poids] : cities)
	{
		fprintf(fileOut, "%i %i\n", i, j);
	}
	fclose(fileOut);
}

template <typename type>
void saveGraph(std::vector<type> cities) {
	FILE* fileOut = NULL;
	fileOut = fopen("resuGraph.dat", "w");
	for (auto [i, j, poids] : cities)
	{
		fprintf(fileOut, "%i %i\n", i, j);
	}
	fclose(fileOut);
}

constexpr auto R{ 6371.f };




auto distance(ListOfCities* List, unsigned a, unsigned b)
{
	auto x = 3.14159265f / 180.f;


	auto lata = List->lat[a]*x, latb = List->lat[b]*x, lona = List->lon[a]*x, lonb = List->lon[b]*x;
	
	//auto x = sin(lata);
	
	return R * std::acosf(std::sinf(lata) * std::sinf(latb) + (std::cosf(lona - lonb) * std::cosf(lata) * std::cosf(latb)));
}

int comparer(const void* a, const void* b)
{
	return ((acm::arrete*)a)->poids - ((acm::arrete*)b)->poids;
}

auto init(unsigned Taille)
{
	std::vector<acm::sommet> Retour;
	Retour.reserve(Taille);

	for (auto i{ 0u }; i < Taille; ++i)
	{
		Retour.push_back(i);
	}

	return Retour;
}



struct ville
{
	char* Nom;
	unsigned Population;
	float Longitude;
	float Latitude;
};

auto cout(const ville& VilleOrigine, const ville& VilleDestination)
{
	auto x = 3.14159265f / 180.f;


	auto lata = VilleOrigine.Latitude * x, latb = VilleDestination.Latitude * x, lona = VilleOrigine.Longitude * x, lonb = VilleDestination.Longitude * x;

	//auto x = sin(lata);

	return R * std::acosf(std::sinf(lata) * std::sinf(latb) + (std::cosf(lona - lonb) * std::cosf(lata) * std::cosf(latb)));
}




struct villeb
{
	char Nom[64];
	unsigned Population;
	float Longitude, Latitude;
};



std::vector<villeb> lire(const char* Emplacement)
{
	std::vector<villeb> Villes;
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "r")) != nullptr)
	{
		villeb Ville;

		while (fscanf_s(Fichier, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%u,%*[^,],%*[^,],%*[^,],%*[^,],%f,%f,%*[^\n]", &Ville.Nom, sizeof(villeb::Nom), &Ville.Population, &Ville.Longitude, &Ville.Latitude) == 4)
		{
			Villes.push_back(Ville);
		}

		fclose(Fichier);
	}

	return Villes;
}

std::vector<villeb> extraire_population_minimale(const std::vector<villeb>& Villes, unsigned Population)
{
	std::vector<villeb> Resultat;

	for (auto& Ville : Villes)
	{
		if (Ville.Population > Population)
		{
			Resultat.push_back(Ville);
		}
	}

	return Resultat;
}

int main(int argc, char* argv[])
{
	auto v = lire("citiesList.csv");
	auto vv = extraire_population_minimale(v, 150000);

	//std::vector<villeb> Vec;


	//auto s = "1,01,ozan,OZAN,ozan,Ozan,O250,OSN,01190,284,01284,2,26,6,618,469,500,93,6.6,4.91667,46.3833,2866,51546,+45456,462330,170,205";

	//char Nom[64];
	//unsigned Population;
	//float Longitude, Latitude;

	//villeb V;

	//FILE* inputFile = NULL;
	//if ((inputFile = fopen("citiesList.csv", "r")) == NULL) {
	//	perror("Could not open file citiesList.csv");
	//	exit(-1);
	//}

	//while (fscanf_s(inputFile, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%u,%*[^,],%*[^,],%*[^,],%*[^,],%f,%f,%*[^\n]", &V.Nom, sizeof(V.Nom), &V.Population, &V.Longitude, &V.Latitude) == 4);

	//fclose(inputFile);

	//while (sscanf_s(s, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%u,%*[^,],%*[^,],%*[^,],%*[^,],%f,%f,%*[^,]", &Nom, sizeof(Nom), &Population, &Longitude, &Latitude) == 4);




	auto* cities = citiesReader(0);


	std::vector<ville> Villes;

	for (unsigned i = 0; i < cities->number; ++i)
	{
		Villes.push_back({ cities->name[i], (unsigned) cities->pop[i], cities->lon[i], cities->lat[i] });
	}



	auto Couts = [](const ville& VilleOrigine, const ville& VilleDestination)
	{
		auto x = 3.14159265f / 180.f;


		auto lata = VilleOrigine.Latitude * x, latb = VilleDestination.Latitude * x, lona = VilleOrigine.Longitude * x, lonb = VilleDestination.Longitude * x;

		//auto x = sin(lata);

		return R * std::acosf(std::sinf(lata) * std::sinf(latb) + (std::cosf(lona - lonb) * std::cosf(lata) * std::cosf(latb)));
	};

	auto y = algo<float>(Villes, cout);

	saveGraph(y);




	auto comp{ init(cities->number) };

	std::vector<acm::arrete> arretes;

	arretes.reserve(0.5 * cities->number * (cities->number - 1));

	for (auto i{ 0u }; i < cities->number; ++i)
	{
		for (auto j{ i + 1 }; j < cities->number; ++j)
		{
			arretes.push_back({ i, j, distance(cities, i, j) });
		}
	}

	std::qsort(arretes.data(), arretes.size(), sizeof(acm::arrete), comparer);

	acm ACM{ comp, arretes };

	auto x = ACM.trouver();

	

	saveGraph(x);


	/*std::vector<acm::sommet> comp{ 0, 1, 2, 3, 4, 5, 6, 7 };
	std::vector<acm::arrete> arretes{ {0,1}, {4, 5}, {1,3}, {0, 7}, {5,6},{0,5},{1,4},{1,2},{2,3},{1,5},{2,4},{6,7}, {3,4} };

	acm ACM{ comp, arretes };

	auto x = ACM.trouver();*/
}