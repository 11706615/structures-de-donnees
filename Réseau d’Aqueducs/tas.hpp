#pragma once

#include <new>

template <typename type>
class tas
{
protected:

	type* Table;
	unsigned int Nombre;
	unsigned int Taille;

public:

	static_assert(noexcept(Table->~type()));

	friend class tas <type&>;

	// Définit les types d'exception pouvant survenir.
	enum class exception
	{
		Allocation,
		Plein,
		Vide,
	};

	// Construit un tas vide.
	constexpr tas() = default;

	// Construit un tas d'une certaine taille.
	// Ses éléments sont construits à partir des arguments fournis.
	template <typename... arguments>
	tas(unsigned int Taille) try : Table{ static_cast<type*>(::operator new(static_cast <std::size_t> (Taille * sizeof(type)), static_cast <std::align_val_t> (alignof(type)))) }, Nombre{ }, Taille{ Taille } {}
	catch (std::bad_alloc const&)
	{
		throw exception::Allocation;
	}

	// Copie les éléments du tas existant vers le nouveau.
	tas(const tas<type>& Tas) try : Table(static_cast<type*>(::operator new(static_cast<std::size_t> (Tas.Taille * sizeof(type)), static_cast <std::align_val_t> (alignof(type))))), Taille(Tas.Taille)
	{
		for (unsigned int i = 0; i < Tas.Nombre; ++i)
		{
			try
			{
				new(this->Table + i) type(Tas.Table[i]);
			}
			catch (...)
			{
				for (unsigned int j = 0; j < i; ++j)
				{
					this->Table[j].~type();
				}

				::operator delete(this->Table, static_cast <std::align_val_t> (alignof(type)));

				throw;
			}
		}
	}
	catch (const std::bad_alloc&)
	{
		throw exception::Allocation;
	}

	// Déplace les éléments du tas existant vers le nouveau.
	tas(tas <type>&& Tas) noexcept : Table{ Tas.Table }, Nombre{ Tas.Nombre }, Taille{ Tas.Taille }
	{
		Tas.Table = nullptr;
		Tas.Nombre = 0;
		Tas.Taille = 0;
	}

	// Détruit le tas.
	~tas()
	{
		for (auto i{ 0u }; i < this->Taille; ++i)
		{
			this->Table[i].~type();
		}

		::operator delete(this->Table, static_cast <std::align_val_t> (alignof(type)));
	}

	// Assigne une copie d'un tas.
	tas <type>& operator =(tas <type> const& Tas)
	{
		type* Table;

		try
		{
			Table = static_cast <type*> (::operator new(static_cast <std::size_t> (Tas.Taille * sizeof(type)), static_cast <std::align_val_t> (alignof(type))));
		}
		catch (...)
		{
			throw exception::Allocation;
		}

		for (unsigned int i = 0; i < Tas.Nombre; ++i)
		{
			try
			{
				new(Table + i) type(Tas.Table[i]);
			}
			catch (...)
			{
				for (unsigned int j = 0; j < i; ++j)
				{
					Table[j].~type();
				}

				::operator delete(Table, static_cast <std::align_val_t> (alignof(type)));

				throw;
			}
		}

		for (unsigned int i = 0; i < this->Nombre; ++i)
		{
			this->Table[i].~type();
		}

		::operator delete(this->Table, static_cast <std::align_val_t> (alignof(type)));

		this->Table = Table;
		this->Nombre = Nombre;
		this->Taille = Tas.Taille;

		return *this;
	}

	// Échange le contenu des tableaux.
	tas <type>& operator =(tas <type>&& Tas) noexcept
	{
		type* Table = this->Table;
		this->Table = Tas.Table;
		Tas.Table = Table;

		unsigned int Nombre = this->Nombre;
		this->Nombre = Tas.Nombre;
		Tas.Nombre = Nombre;

		unsigned int Taille = this->Taille;
		this->Taille = Tas.Taille;
		Tas.Taille = Taille;

		return *this;
	}

	unsigned nombre() const noexcept
	{
		return this->Nombre;
	}

	unsigned taille() const noexcept
	{
		return this->Taille;
	}

#define a_un_pere(Fils) (Fils > 0)
#define pere(Fils) (0.5 * (Fils - 1))
#define a_un_fils_gauche(Pere) (2 * Pere + 1 < this->Nombre)
#define fils_gauche(Pere) (2 * Pere + 1);
#define a_un_fils_droit(Pere) (2 * Pere + 2 < this->Nombre)
#define fils_droit(Pere) (2 * Pere + 2)

	template <typename... arguments>
	tas<type>& inserer(arguments&& ...Arguments)
	{
		if (this->Nombre < this->Taille)
		{
			unsigned int Fils = this->Nombre, Pere;

			new(this->Table + Fils) type(static_cast <arguments && ...> (Arguments)...);

			while (a_un_pere(Fils) && this->Table[Pere = pere(Fils)] > this->Table[Fils])
			{
				type NouvelElement{ static_cast<type&&>(this->Table[Fils]) };
				this->Table[Fils] = static_cast<type&&>(this->Table[Pere]);
				this->Table[Pere] = static_cast<type&&>(NouvelElement);

				Fils = Pere;
			}

			++this->Nombre;

			return *this;
		}
		else
		{
			throw exception::Plein;
		}
	}

	type extraire()
	{
		if (this->Nombre > 0)
		{
			unsigned int Fils, FilsGauche, FilsDroit, Pere = 0;

			type Premier{ static_cast<type&&>(this->Table[0]) };
			this->Table[0] = static_cast<type&&>(this->Table[--this->Nombre]);

			while (a_un_fils_gauche(Pere))
			{
				FilsGauche = fils_gauche(Pere);

				if (a_un_fils_droit(Pere) && this->Table[FilsGauche] > this->Table[FilsDroit = fils_droit(Pere)])
				{
					Fils = FilsDroit;
				}
				else
				{
					Fils = FilsGauche;
				}

				if (this->Table[Pere] > this->Table[Fils])
				{
					type NouvelElement{ static_cast<type&&>(this->Table[Fils]) };
					this->Table[Fils] = static_cast<type&&>(this->Table[Pere]);
					this->Table[Pere] = static_cast<type&&>(NouvelElement);

					Pere = Fils;
				}
				else
				{
					break;
				}
			}

			return Premier;
		}
		else
		{
			throw exception::Vide;
		}
	}
};