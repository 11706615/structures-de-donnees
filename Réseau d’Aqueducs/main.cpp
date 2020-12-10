#include <cmath>

#include "arbre-couvrant-minimal.hpp"

#include "tas.hpp"

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

using arrete = decltype(ArbreCouvrantMinimal::trouver_arretes(std::declval<std::vector<ville>>(), cout))::value_type;

void exporter_arretes(const std::vector<arrete>& Arretes, const char* Emplacement)
{
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "w")) != nullptr)
	{
		for (const auto& [Origine, Destination, Distance] : Arretes)
		{
			fprintf(Fichier, "%zu %zu\n", Origine, Destination);
		}

		fclose(Fichier);
	}
}

auto calculer_longueur(const std::vector<arrete>& Arretes)
{
	auto Longueur{ 0.f };

	for (const auto& Arrete : Arretes)
	{
		Longueur += Arrete.Cout;
	}

	return Longueur;
}


int main(int Nombre, const char* Arguments[])
{
	tas<int> Tas(3);

	Tas.inserer(4).inserer(1).inserer(2);

	auto x = Tas.extraire();
	auto y = Tas.extraire();


	if (Nombre > 3)
	{
		auto Villes{ lire(Arguments[1]) };

		unsigned Population;

		while (std::printf("Population : "), std::scanf("%u", &Population) == 1)
		{
			auto PopulationMinimale{ extraire_population_minimale(Villes, Population) };

			exporter(PopulationMinimale, Arguments[2]);

			auto Arretes = ArbreCouvrantMinimal::trouver_arretes(PopulationMinimale, cout);

			exporter_arretes(Arretes, Arguments[3]);

			std::printf("%zu villes exportées. Longueur du réseau : %f kms.\n", PopulationMinimale.size(), calculer_longueur(Arretes));
		}

		return 0;
	}
	else
	{
		return 1;
	}
}