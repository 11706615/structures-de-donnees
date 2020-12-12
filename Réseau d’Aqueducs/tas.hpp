#pragma once

#include <new>

template <typename type>
class tas
{
protected:

	type* Table;
	size_t Nombre;
	size_t Taille;

public:

	static_assert(noexcept(Table->~type()), "La destruction d'un élément ne doit pas lever d'exception.");
	static_assert(noexcept(*Table = static_cast<type&&>(*Table)), "Un élément doit pouvoir être déplacé sans lever d'exception.");
	static_assert(noexcept(type{ static_cast<type&&>(*Table) }), "Un élément doit pouvoir être construit par déplacement sans lever d'exception.");

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
	tas(size_t Taille) try : Table{ static_cast<type*>(::operator new(Taille * sizeof(type), static_cast <std::align_val_t> (alignof(type)))) }, Nombre{ }, Taille{ Taille } {}
	catch (const std::bad_alloc&)
	{
		throw exception::Allocation;
	}

	// Copie les éléments du tas existant vers le nouveau.
	tas(const tas<type>& Tas) try : Table{ static_cast<type*>(::operator new(Tas.Taille * sizeof(type)), static_cast <std::align_val_t> (alignof(type))) }, Nombre{ }, Taille{ Tas.Taille }
	{
		for (size_t i = 0; i < Tas.Nombre; ++i)
		{
			try
			{
				new(this->Table + i) type(Tas.Table[i]);
			}
			catch (...)
			{
				for (size_t j = 0; j < i; ++j)
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
		for (size_t i = 0; i < this->Taille; ++i)
		{
			this->Table[i].~type();
		}

		::operator delete(this->Table, static_cast <std::align_val_t> (alignof(type)));
	}

	// Assigne une copie d'un tas.
	tas <type>& operator =(const tas <type>& Tas)
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

		for (size_t i = 0; i < Tas.Nombre; ++i)
		{
			try
			{
				new(Table + i) type(Tas.Table[i]);
			}
			catch (...)
			{
				for (size_t j = 0; j < i; ++j)
				{
					Table[j].~type();
				}

				::operator delete(Table, static_cast <std::align_val_t> (alignof(type)));

				throw;
			}
		}

		for (size_t i = 0; i < this->Nombre; ++i)
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

		size_t Nombre = this->Nombre;
		this->Nombre = Tas.Nombre;
		Tas.Nombre = Nombre;

		size_t Taille = this->Taille;
		this->Taille = Tas.Taille;
		Tas.Taille = Taille;

		return *this;
	}

	size_t nombre() const noexcept
	{
		return this->Nombre;
	}

	size_t taille() const noexcept
	{
		return this->Taille;
	}

private:

	auto a_un_pere(size_t Fils)
	{
		return Fils > 0;
	}
	auto pere(size_t Fils)
	{
		return (Fils - 1) >> 1;
	}
	auto a_un_fils_gauche(size_t Pere)
	{
		return (Pere << 1) + 1 < this->Nombre;
	}
	auto fils_gauche(size_t Pere)
	{
		return (Pere << 1) + 1;
	}
	auto a_un_fils_droit(size_t Pere)
	{
		return (Pere << 1) + 2 < this->Nombre;
	}
	auto fils_droit(size_t Pere)
	{
		return (Pere << 1) + 2;
	}

public:

	template <typename... arguments>
	tas<type>& inserer(arguments&& ...Arguments)
	{
		if (this->Nombre < this->Taille)
		{
			size_t Fils = this->Nombre, Pere;

			new(this->Table + Fils) type{ static_cast <arguments && ...> (Arguments)... };

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
			size_t Fils, FilsGauche, FilsDroit, Pere = 0;

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