#pragma once

#include <vector>


//auto inserer(std::vector)


// On utilise l'algorithme de Prim car d'après le problème posé le graphe est complet ce qui facilite l'utilisation de cet algorithme (tous les sommets sont reliés).


template </*typename cout, */typename type, typename couts>
auto algo(const std::vector<type>& Sommets, couts&& Couts)
{
	//template <typename type2>
	//struct arrete
	//{
	//	type Origine;
	//	type Destination;
	//};

	struct arrete
	{
		unsigned Origine;
		unsigned Destination;
		decltype(Couts(std::declval<type>(), std::declval<type>())) Cout;
		
		bool operator <(const arrete& Arrete) noexcept
		{
			return this->Cout < Arrete.Cout;
		}

		static int comparer(const void* a, const void* b)
		{
			return ((arrete*)b)->Cout - ((arrete*)a)->Cout;
		}
	};

	arrete a{ 1,2,3.3 }, b{ 2,3,4.5 };
	auto x = a < b;

	auto Source{ 0u };
	std::vector<arrete> Retour;
	std::vector<bool> Visites(Sommets.size(), false);
	std::vector<arrete> Tas;

	while (Retour.size() + 1 < Sommets.size())
	{
		Visites[Source] = true;

		for (auto i{ 0u }; i < Sommets.size(); ++i)
		{
			if (!Visites[i]) Tas.push_back({ Source, i, Couts(Sommets[Source], Sommets[i]) });
		}

		std::qsort(Tas.data(), Tas.size(), sizeof(arrete), arrete::comparer);

		arrete x;

		do
		{
			x = Tas.back();
			Tas.pop_back();
		} while (Visites[x.Destination]);
		
		Retour.push_back(x);
		Source = x.Destination;
	}

	

	return Retour;
}