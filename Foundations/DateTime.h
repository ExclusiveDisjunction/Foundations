#pragma once

#include <iostream>
#include <chrono>

#include "Types\BasicObject.h"
#include "Str.h"

//namespace std
//{
//	namespace chrono
//	{
//		template <class _Rep, class _Period>
//		class duration;
//	}
//}

namespace Core
{
	enum class DateStringFormat
	{
		LongDate = 0,
		ShortDate = 1,
		Time = 2,
		Date = 3,
		Duration = 4
	};

	class CORE_API DateTime : public BasicObject
	{
	private:
		std::chrono::duration<double> _Dur;

	public:
		static DateTime Today();
		static DateTime Now();

		DateTime();
		DateTime(int Month, int Day, int Year);
		DateTime(int Hour, int Minute, int Second, int Millisecond);
		DateTime(int Month, int Day, int Year, int Hour, int Minute, int Second, int Millisecond);
		DateTime(std::chrono::duration<double> Tm);

		bool HasValue = true, HasDay = true, HasTime = true;

		int Year() const;
		int Year(std::chrono::duration<double>& Remain) const;
		int Month() const;
		int Month(std::chrono::duration<double>& Remain) const;
		int Day() const;
		int Day(std::chrono::duration<double>& Remain) const;
		int Hour() const;
		int Hour(std::chrono::duration<double>& Remain) const;
		int Minute() const;
		int Minute(std::chrono::duration<double>& Remain) const;
		int Second() const;
		int Second(std::chrono::duration<double>& Remain) const;
		int Millisecond() const;
		int Millisecond(std::chrono::duration<double>& Remain) const;

		DateTime DayParts() const;
		DateTime TimeParts() const;
		
		String ToString() const override { return ToBackString() + L" _AS_DATETIME"; }
		String ToUIString() const override { return ToString(DateStringFormat::LongDate); }
		String TypeName() const override { return L"DATETIME"; }

		BasicObject* DefaultValue() const override { return new DateTime(); }
		BasicObject* Clone() const override { return new DateTime(*this); }
		bool OverrideFrom(BasicObject* Obj) override
		{
			DateTime* New = dynamic_cast<DateTime*>(Obj);
			if (!New)
				return false;

			*this = *New;
			return true;
		}

		void FillFromString(const String& Obj) override
		{
			*this = DateTime::FromBackString(Obj);
		}

		bool HasModifyer() const override { return true; }
		BasicObjectModifyer* ConstructModifyer() const override
		{
			//TODO: Write DateTime modifyer.
			return nullptr;
		}

		/**
		* Converts this DateTime to the following format: MM/DD/YYYY HH:MM:SS [AM/PM]
		*/
		String ToString(DateStringFormat Format) const;
		String ToBackString() const;
		static DateTime FromBackString(const String& Value);

		friend std::wostream& operator<<(std::wostream& out, const DateTime& Obj)
		{
			out << Obj.ToBackString();
			return out;
		}

		friend bool operator==(const DateTime& One, const DateTime& Two);
		friend bool operator!=(const DateTime& One, const DateTime& Two);
		bool operator<(const DateTime& Two) const;
		bool operator>(const DateTime& Two) const;
		std::partial_ordering operator<=>(const DateTime& Two) const;

		friend DateTime operator-(const DateTime& One, const DateTime& Two);
		friend DateTime operator+(const DateTime& One, const DateTime& Two);
		DateTime& operator+=(const DateTime& Other);
		DateTime& operator-=(const DateTime& Other);
	};
}