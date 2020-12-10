#pragma once

#include <vector>

#include "tas.hpp"

namespace ArbreCouvrantMinimal
{
	template <typename type, typename couts>
	auto trouver_arretes(const std::vector<type>& Sommets, couts&& Couts)
	{
		using position = typename std::vector<type>::size_type;

		struct arrete
		{
			position Origine;
			position Destination;
			decltype(Couts(std::declval<type>(), std::declval<type>())) Cout;

			bool operator <(const arrete& Arrete) noexcept
			{
				return this->Cout < Arrete.Cout;
			}

			bool operator >(const arrete& Arrete) noexcept
			{
				return this->Cout > Arrete.Cout;
			}
		};

		position Source = 0;
		std::vector<arrete> Retour;
		std::vector<bool> Visites(Sommets.size(), false);

		tas<arrete> Tas{ (Sommets.size() * (Sommets.size() - 1)) >> 1 };
		
		while (Retour.size() + 1 < Sommets.size())
		{
			Visites[Source] = true;

			for (size_t i = 0; i < Sommets.size(); ++i)
			{
				if (!Visites[i]) Tas.inserer(Source, i, Couts(Sommets[Source], Sommets[i]));
			}

			arrete Arrete;

			do
			{
				Arrete = Tas.extraire();
			} while (Visites[Arrete.Destination]);

			Retour.push_back(Arrete);
			Source = Arrete.Destination;
		}

		return Retour;
	}
}