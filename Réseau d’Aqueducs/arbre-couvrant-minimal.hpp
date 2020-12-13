#pragma once

#include <vector>

#include "tas.hpp"
#include "arrete.hpp"

namespace Graphe
{
	template <typename type, typename couts>
	auto trouver_arretes(const std::vector<type>& Sommets, couts&& Couts)
	{
		using position = typename std::vector<type>::size_type;
		using cout = decltype(Couts(std::declval<type>(), std::declval<type>()));

		std::vector<arrete<position, cout>> Retour;

		if (Sommets.size() > 0)
		{
			Retour.reserve(Sommets.size() - 1);
			std::vector<bool> Visites(Sommets.size(), false);
			tas<arrete<position, cout>> Tas{ (Sommets.size() * (Sommets.size() - 1)) >> 1 };
			position Origine = 0;

			while (Retour.size() + 1 < Sommets.size())
			{
				Visites[Origine] = true;

				for (size_t Destination = 0; Destination < Sommets.size(); ++Destination)
				{
					if (!Visites[Destination]) Tas.inserer(Origine, Destination, Couts(Sommets[Origine], Sommets[Destination]));
				}

				arrete<position, cout> Arrete;

				do
				{
					Arrete = Tas.extraire();
				} while (Visites[Arrete.Destination]);

				Retour.push_back(Arrete);
				Origine = Arrete.Destination;
			}
		}
		return Retour;
	}

	template <typename position, typename cout>
	auto trouver_arretes(const std::vector<arrete<position, cout>>& Arretes, position Taille)
	{
		std::vector<position> Composantes;

		Composantes.reserve(Taille);

		for (position i = 0; i < Taille; ++i)
		{
			Composantes.push_back(i);
		}

		std::vector <arrete<position, cout>> Resultat;

		Resultat.reserve(Taille - 1);

		for (auto& Arrete : Arretes)
		{
			if (Composantes[Arrete.Origine] != Composantes[Arrete.Destination])
			{
				Resultat.push_back(Arrete);

				auto Origine = Composantes[Arrete.Origine];
				auto Destination = Composantes[Arrete.Destination];

				for (auto& Composante : Composantes)
				{
					if (Composante == Destination) Composante = Origine;
				}
			}
		}

		return Resultat;
	}
}