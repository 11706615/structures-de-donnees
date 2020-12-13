#pragma once

namespace Graphe
{
	template <typename position, typename cout>
	struct arrete
	{
		position Origine;
		position Destination;
		cout Cout;

		bool operator <(const arrete& Arrete) noexcept
		{
			return this->Cout < Arrete.Cout;
		}

		bool operator >(const arrete& Arrete) noexcept
		{
			return this->Cout > Arrete.Cout;
		}
	};
}