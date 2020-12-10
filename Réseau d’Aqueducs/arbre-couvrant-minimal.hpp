#pragma once

#include <vector>

#include "tas.hpp"

namespace ArbreCouvrantMinimal
{
	template <typename type, typename couts>
	auto trouver_arretes(const std::vector<type>& Sommets, couts&& Couts)
	{
		struct arrete
		{
			unsigned Origine;
			unsigned Destination;
			decltype(Couts(std::declval<type>(), std::declval<type>())) Cout;

			bool operator <(const arrete& Arrete) noexcept
			{
				return this->Cout < Arrete.Cout;
			}

			bool operator >(const arrete& Arrete) noexcept
			{
				return this->Cout > Arrete.Cout;
			}

			static int comparer(const void* a, const void* b)
			{
				return ((arrete*)b)->Cout - ((arrete*)a)->Cout;
			}
		};

		auto Source{ 0u };
		std::vector<arrete> Retour;
		std::vector<bool> Visites(Sommets.size(), false);
		tas<arrete> Tas{ 2000 * 2000 };

		while (Retour.size() + 1 < Sommets.size())
		{
			Visites[Source] = true;

			for (auto i{ 0u }; i < Sommets.size(); ++i)
			{
				if (!Visites[i]) Tas.inserer({ Source, i, Couts(Sommets[Source], Sommets[i]) });
			}

			//std::qsort(Tas.data(), Tas.size(), sizeof(arrete), arrete::comparer);

			arrete x;

			do
			{
				x = Tas.extraire();
			} while (Visites[x.Destination]);

			Retour.push_back(x);
			Source = x.Destination;
		}



		return Retour;
	}
}