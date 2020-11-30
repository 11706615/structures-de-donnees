#include <cmath>

#include "acm.hpp"

#include "citiesReader.h"



void saveGraph(std::vector<acm::arrete> cities) {
	FILE* fileOut = NULL;
	fileOut = fopen("resuGraph.dat", "w");
	for (auto [i, j, poids] : cities)
	{
		fprintf(fileOut, "%i %i\n", i, j);
	}
	fclose(fileOut);
}

constexpr auto R{ 6371.f };

auto distance(ListOfCities* List, unsigned a, unsigned b)
{
	auto x = 3.14159265f / 180.f;


	auto lata = List->lat[a]*x, latb = List->lat[b]*x, lona = List->lon[a]*x, lonb = List->lon[b]*x;
	
	//auto x = sin(lata);
	
	return R * std::acosf(std::sinf(lata) * std::sinf(latb) + (std::cosf(lona - lonb) * std::cosf(lata) * std::cosf(latb)));
}

int comparer(const void* a, const void* b)
{
	return ((acm::arrete*)a)->poids - ((acm::arrete*)b)->poids;
}

auto init(unsigned Taille)
{
	std::vector<acm::sommet> Retour;
	Retour.reserve(Taille);

	for (auto i{ 0u }; i < Taille; ++i)
	{
		Retour.push_back(i);
	}

	return Retour;
}

int main()
{

	auto* cities = citiesReader(150000);

	auto d = distance(cities, 1, 16);


	auto comp{ init(cities->number) };

	std::vector<acm::arrete> arretes;

	arretes.reserve(0.5 * cities->number * (cities->number + 1));

	for (auto i{ 0u }; i < cities->number; ++i)
	{
		for (auto j{ i + 1 }; j < cities->number; ++j)
		{
			arretes.push_back({ i, j, distance(cities, i, j) });
		}
	}

	std::qsort(arretes.data(), arretes.size(), sizeof(acm::arrete), comparer);

	acm ACM{ comp, arretes };

	auto x = ACM.trouver();

	saveGraph(x);


	/*std::vector<acm::sommet> comp{ 0, 1, 2, 3, 4, 5, 6, 7 };
	std::vector<acm::arrete> arretes{ {0,1}, {4, 5}, {1,3}, {0, 7}, {5,6},{0,5},{1,4},{1,2},{2,3},{1,5},{2,4},{6,7}, {3,4} };

	acm ACM{ comp, arretes };

	auto x = ACM.trouver();*/
}