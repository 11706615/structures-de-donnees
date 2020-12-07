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

struct ville
{
	char Nom[64];
	unsigned Population;
	float Longitude, Latitude;
};

auto cout(const ville& VilleOrigine, const ville& VilleDestination)
{
	auto x = 3.14159265f / 180.f;


	auto lata = VilleOrigine.Latitude * x, latb = VilleDestination.Latitude * x, lona = VilleOrigine.Longitude * x, lonb = VilleDestination.Longitude * x;

	//auto x = sin(lata);

	return R * std::acosf(std::sinf(lata) * std::sinf(latb) + (std::cosf(lona - lonb) * std::cosf(lata) * std::cosf(latb)));
}

std::vector<ville> lire(const char* Emplacement)
{
	std::vector<ville> Villes;
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "r")) != nullptr)
	{
		ville Ville;

		while (fscanf_s(Fichier, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%u,%*[^,],%*[^,],%*[^,],%*[^,],%f,%f,%*[^\n]", &Ville.Nom, sizeof(ville::Nom), &Ville.Population, &Ville.Longitude, &Ville.Latitude) == 4)
		{
			Villes.push_back(Ville);
		}

		fclose(Fichier);
	}

	return Villes;
}

std::vector<ville> extraire_population_minimale(const std::vector<ville>& Villes, unsigned Population)
{
	std::vector<ville> Resultat;

	for (const auto& Ville : Villes)
	{
		if (Ville.Population > Population)
		{
			Resultat.push_back(Ville);
		}
	}

	return Resultat;
}

template <typename type>
std::vector<ville> exporter_arretes(const std::vector<type>& Arretes, const char* Emplacement)
{
	std::vector<ville> Villes;
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "w")) != nullptr)
	{
		for (const auto& [Origine, Destination, Distance] : Arretes)
		{
			fprintf(Fichier, "%i %i\n", Origine, Destination);
		}

		fclose(Fichier);
	}

	return Villes;
}

int main(int argc, char* argv[])
{
	auto Villes{ lire("citiesList.csv") };
	auto PopulationMinimale{ extraire_population_minimale(Villes, 150000) };

	citiesReader(150000);

	auto Arretes = algo(PopulationMinimale, cout);

	exporter_arretes(Arretes, "resuGraph.dat");

	return 0;
}