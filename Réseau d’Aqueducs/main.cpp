#include <cmath>
#include <iostream>

#include "arbre-couvrant-minimal.hpp"

#include "ville.hpp"

using arrete = decltype(ArbreCouvrantMinimal::trouver_arretes(std::declval<std::vector<ville>>(), distance))::value_type;

void exporter_arretes(const std::vector<arrete>& Arretes, const char* Emplacement)
{
	std::FILE* Fichier;

	if ((Fichier = std::fopen(Emplacement, "w")) != nullptr)
	{
		for (const auto [Origine, Destination, Distance] : Arretes)
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
	if (Nombre > 1)
	{
		auto Villes{ lire(Arguments[1]) };

		unsigned Population;

		while (std::printf("Population : "), std::scanf("%u", &Population) == 1)
		{
			try
			{
				auto PopulationMinimale{ extraire_population_minimale(Villes, Population) };

				if (Nombre > 2) exporter(PopulationMinimale, Arguments[2]);

				auto Arretes{ ArbreCouvrantMinimal::trouver_arretes(PopulationMinimale, distance) };

				if (Nombre > 3) exporter_arretes(Arretes, Arguments[3]);

				std::printf("%zu villes exportées. Longueur du réseau : %f kms.\n", PopulationMinimale.size(), calculer_longueur(Arretes));
			}
			catch (tas<arrete>::exception Exception)
			{
				switch (Exception)
				{
				case tas<arrete>::exception::Allocation: std::cerr << "Pas assez de mémoire." << std::endl; break;
				default: std::cerr << "Erreur inattendue." << std::endl; break;
				}
			}
		}

		return 0;
	}
	else
	{
		std::cerr << "Arguments attendus : ImporterVilles [ExporterVilles] [ExporterArretes]";

		return 1;
	}
}