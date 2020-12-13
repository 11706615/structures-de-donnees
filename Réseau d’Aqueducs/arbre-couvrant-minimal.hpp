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

		std::vector<Graphe::arrete<position, cout>> Retour;

		if (Sommets.size() > 0)
		{
			Retour.reserve(Sommets.size() - 1);
			std::vector<bool> Visites(Sommets.size(), false);
			tas<Graphe::arrete<position, cout>> Tas{ (Sommets.size() * (Sommets.size() - 1)) >> 1 };
			position Origine = 0;

			while (Retour.size() + 1 < Sommets.size())
			{
				Visites[Origine] = true;

				for (size_t Destination = 0; Destination < Sommets.size(); ++Destination)
				{
					if (!Visites[Destination]) Tas.inserer(Origine, Destination, Couts(Sommets[Origine], Sommets[Destination]));
				}

				Graphe::arrete<position, cout> Arrete;

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

	template <typename type, typename couts>
	auto trouver_arretes(const std::vector<type>& Sommets, couts&& Couts)
	{
		using position = typename std::vector<type>::size_type;
		using cout = decltype(Couts(std::declval<type>(), std::declval<type>()));

		std::vector<Graphe::arrete<position, cout>> Retour;

		if (Sommets.size() > 0)
		{
			Retour.reserve(Sommets.size() - 1);
			std::vector<bool> Visites(Sommets.size(), false);
			tas<Graphe::arrete<position, cout>> Tas{ (Sommets.size() * (Sommets.size() - 1)) >> 1 };
			position Origine = 0;

			while (Retour.size() + 1 < Sommets.size())
			{
				Visites[Origine] = true;

				for (size_t Destination = 0; Destination < Sommets.size(); ++Destination)
				{
					if (!Visites[Destination]) Tas.inserer(Origine, Destination, Couts(Sommets[Origine], Sommets[Destination]));
				}

				Graphe::arrete<position, cout> Arrete;

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
}