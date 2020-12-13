#pragma once

#include <cmath>
#include <vector>

struct ville
{
	char Nom[64];
	unsigned Population;
	float Longitude, Latitude;
};

auto distance(const ville& VilleOrigine, const ville& VilleDestination)
{
	constexpr auto Pi{ 3.14159265f };
	constexpr auto Radians{ Pi / 180.f };
	constexpr auto Rayon{ 6371.f };

	auto LatitudeOrigine = VilleOrigine.Latitude * Radians, LatitudeDestination = VilleDestination.Latitude * Radians, LongitudeOrigine = VilleOrigine.Longitude * Radians, LongitudeDestination = VilleDestination.Longitude * Radians;

	return Rayon * std::acos(std::sin(LatitudeOrigine) * std::sin(LatitudeDestination) + (std::cos(LongitudeOrigine - LongitudeDestination) * std::cos(LatitudeOrigine) * std::cos(LatitudeDestination)));
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

std::vector<ville> lire(const char* Emplacement)
{
	std::vector<ville> Villes;
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "r")) != nullptr)
	{
		ville Ville;

		while (fscanf(Fichier, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%u,%*[^,],%*[^,],%*[^,],%*[^,],%f,%f,%*[^\n]", &Ville.Nom, &Ville.Population, &Ville.Longitude, &Ville.Latitude) == 4)
		{
			Villes.push_back(Ville);
		}

		fclose(Fichier);
	}

	return Villes;
}

void exporter(const std::vector<ville>& Villes, const char* Emplacement)
{
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "w")) != nullptr)
	{
		for (const auto [Nom, Population, Longitude, Latitude] : Villes)
		{
			fprintf(Fichier, "%i %f %f\n", Population, Longitude, Latitude);
		}

		fclose(Fichier);
	}
}