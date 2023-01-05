#include "Color.h"

#include "Str.h"
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

namespace Foundations
{
	AaColor::AaColor() : _Value(0)
	{

	}
	AaColor::AaColor(const Byte R, const Byte G, const Byte B)
	{
		Assign(0xFF, R, G, B);
	}
	AaColor::AaColor(const Byte A, const Byte R, const Byte G, const Byte B)
	{
		Assign(A, R, G, B);
	}
	AaColor::AaColor(const AaColor& Other)
	{
		Assign(Other.A(), Other.R(), Other.G(), Other.B());
	}
	AaColor::AaColor(const unsigned int& Other)
	{
		_Value = Other;
	}

	AaColor::Byte AaColor::A() const
	{
		return static_cast<Byte>((_Value >> 24) & 0xFF);
	}
	AaColor::Byte AaColor::R() const
	{
		return static_cast<Byte>((_Value >> 16) & 0xFF);
	}
	AaColor::Byte AaColor::G() const
	{
		return static_cast<Byte>((_Value >> 8) & 0xFF);
	}
	AaColor::Byte AaColor::B() const
	{
		return static_cast<Byte>(_Value & 0xFF);
	}

	String AaColor::ToString() const
	{
		return String(_Value) + L" _AS_COLOR";
	}
	String AaColor::ToUIString() const
	{
		return L"#"; //TODO: Write algor that converts to hex.
	}
	String AaColor::TypeName() const
	{
		return L"COLOR";
	}

	bool AaColor::OverrideFrom(BasicObject* Obj)
	{
		AaColor* FirstConv = dynamic_cast<AaColor*>(Obj);
		if (FirstConv)
		{
			*this = *FirstConv;
			return true;
		}

		String* SecondConv = dynamic_cast<String*>(Obj);
		if (SecondConv)
		{
			this->FillFromString(*SecondConv);
			return true;
		}

		return false;
	}

	void AaColor::FillFromString(const String& Obj)
	{
		*this = AaColor(Obj.ToUInt());
	}

	bool AaColor::HasModifyer() const { return true; }
	BasicObjectModifyer* AaColor::ConstructModifyer() const
	{
		//TODO: Write AaColor Basic Modifyer
		return nullptr;
	}

	void AaColor::Assign(Byte A, Byte R, Byte G, Byte B)
	{
		_Value = static_cast<unsigned int>((A << 24) | (R << 16) | (G << 8) | B);
	}
	void AaColor::Assign(unsigned int New)
	{
		_Value = New;
	}

	void AaColor::Reduce(float By)
	{
		Assign(A(), static_cast<Byte>(R() * By), static_cast<Byte>(G() * By), static_cast<Byte>(B() * By));
	}


	COLORREF ColorToColorRef(const AaColor& Color)
	{
		return RGB(Color.R(), Color.G(), Color.B());
	}
	
	Gdiplus::Color ColorToGDIPColor(const AaColor& Color)
	{
		return Gdiplus::Color(Color.A(), Color.R(), Color.G(), Color.B());
	}
}