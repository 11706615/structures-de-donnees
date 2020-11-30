#pragma once

#include <vector>

struct acm
{
	using sommet = unsigned int;

	struct arrete
	{
		sommet a;
		sommet b;
		float poids;
	};

	std::vector<sommet> composantes;
	std::vector <arrete> arretes;

	auto trouver()
	{
		std::vector <arrete> R;

		for (auto& Arrete : this->arretes)
		{
			if (composantes[Arrete.a] != composantes[Arrete.b])
			{
				R.push_back(Arrete);

				auto a = composantes[Arrete.a];
				auto c = composantes[Arrete.b];

				for (auto& Comp : composantes)
				{
					if (Comp == c) Comp = a;
				}
			}
		}

		return R;
	}
};