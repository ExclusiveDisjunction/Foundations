#pragma once

#include "Types\BasicObject.h"

namespace Core
{
	class CORE_API AaColor : public BasicObject
	{
	private:
		using Byte = unsigned char;

		unsigned int _Value;
	public:
		AaColor();
		AaColor(const Byte R, const Byte G, const Byte B);
		AaColor(const Byte A, const Byte R, const Byte G, const Byte B);
		AaColor(const AaColor& Other);
		AaColor(const unsigned int& Other);

		Byte A() const;
		Byte R() const;
		Byte G() const;
		Byte B() const;
		
		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		void FillFromString(const String& Obj) override;

		BasicObject* DefaultValue() const override { return new AaColor(); }
		BasicObject* Clone() const override { return new AaColor(*this); }
		bool OverrideFrom(BasicObject* Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;

		void Assign(Byte A, Byte R, Byte G, Byte B);
		void Assign(unsigned int New);

		void Reduce(float By);

		operator unsigned long() const
		{
			return static_cast<unsigned long long>(R()) | static_cast<unsigned long long>(G()) << 8 | static_cast<unsigned long long>(B()) << 16;
		}
	};

#ifdef _GDIPLUS_H
	Gidplus::Color ColorToGDIPColor(const AaColor& Color);
#endif
}