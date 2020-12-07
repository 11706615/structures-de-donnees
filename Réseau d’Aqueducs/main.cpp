#include <cmath>

#include "acm.hpp"

#include "citiesReader.h"

#include "prim-dijkstra.hpp"

struct ville
{
	char Nom[64];
	unsigned Population;
	float Longitude, Latitude;
};

auto cout(const ville& VilleOrigine, const ville& VilleDestination)
{
	constexpr auto Pi{ 3.14159265f };
	constexpr auto Radians{ 3.14159265f / 180.f };
	constexpr auto Rayon{ 6371.f };

	auto lata = VilleOrigine.Latitude * Radians, latb = VilleDestination.Latitude * Radians, lona = VilleOrigine.Longitude * Radians, lonb = VilleDestination.Longitude * Radians;

	//auto Radians = sin(lata);

	return Rayon * std::acosf(std::sinf(lata) * std::sinf(latb) + (std::cosf(lona - lonb) * std::cosf(lata) * std::cosf(latb)));
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

void exporter(const std::vector<ville>& Villes, const char* Emplacement)
{
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "w")) != nullptr)
	{
		for (const auto& [Nom, Population, Longitude, Latitude] : Villes)
		{
			fprintf(Fichier, "%i %f %f\n", Population, Longitude, Latitude);
		}

		fclose(Fichier);
	}
}

template <typename type>
void exporter_arretes(const std::vector<type>& Arretes, const char* Emplacement)
{
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "w")) != nullptr)
	{
		for (const auto& [Origine, Destination, Distance] : Arretes)
		{
			fprintf(Fichier, "%i %i\n", Origine, Destination);
		}

		fclose(Fichier);
	}
}

int main(int argc, char* argv[])
{
	auto Villes{ lire("citiesList.csv") };
	auto PopulationMinimale{ extraire_population_minimale(Villes, 250000) };

	exporter(PopulationMinimale, "resuCities.dat");

	//citiesReader(150000);

	auto Arretes = algo(PopulationMinimale, cout);

	exporter_arretes(Arretes, "resuGraph.dat");

	return 0;
}