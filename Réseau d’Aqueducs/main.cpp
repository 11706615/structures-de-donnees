#include <iostream>

#include "arbre-couvrant-minimal.hpp"
#include "ville.hpp"

using arrete = Graphe::arrete<size_t,float>;

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

#define PRIM

int main(int Nombre, const char* Arguments[])
{
	if (Nombre > 1)
	{
		auto Villes{ lire(Arguments[1]) };

		if (Villes.size() > 0)
		{


			unsigned Population;

			while (std::printf("Population : "), std::scanf("%u", &Population) == 1)
			{
				try
				{
					auto PopulationMinimale{ extraire_population_minimale(Villes, Population) };

					if (Nombre > 2)
					{
						exporter(PopulationMinimale, Arguments[2]);

						std::printf("%zu villes export�es.\n", PopulationMinimale.size());
					}
#ifdef GLOUTON
					struct local
					{
						static auto comparer(const void* Origine, const void* Destination)
						{
							return static_cast<int>(((arrete*)Origine)->Cout - ((arrete*)Destination)->Cout);
						}
					};

					std::vector<arrete> Tri;

					Tri.reserve((PopulationMinimale.size() * (PopulationMinimale.size() - 1)) >> 1);

					for (size_t Origine = 0; Origine < PopulationMinimale.size(); ++Origine)
					{
						for (size_t Destination = Origine + 1; Destination < PopulationMinimale.size(); ++Destination)
						{
							Tri.push_back({ Origine, Destination, distance(PopulationMinimale[Origine], PopulationMinimale[Destination]) });
						}
					}

					std::qsort(Tri.data(), Tri.size(), sizeof(arrete), local::comparer);

					auto Arretes{ Graphe::trouver_arretes(Tri, PopulationMinimale.size()) };
#else
					auto Arretes{ Graphe::trouver_arretes(PopulationMinimale, distance) };
#endif
					if (Nombre > 3)
					{
						exporter_arretes(Arretes, Arguments[3]);

						std::printf("%zu arr�tes export�es.\n", Arretes.size());
					}

					std::printf("Longueur du r�seau : %f kms.\n", calculer_longueur(Arretes));
				}
				catch (tas<arrete>::exception Exception)
				{
					switch (Exception)
					{
					case tas<arrete>::exception::Allocation: std::cerr << "Pas assez de m�moire." << std::endl; break;
					default: std::cerr << "Erreur inattendue." << std::endl; break;
					}
				}
			}
		}
		else
		{
			std::cout << "Aucune ville n'a �t� import�e." << std::endl;
		}

		return 0;
	}
	else
	{
		std::cerr << "Arguments attendus : ImporterVilles [ExporterVilles] [ExporterArretes]" << std::endl;

		return 1;
	}
}