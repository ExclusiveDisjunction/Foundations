#pragma once

#include "IOCommon.h"

#include <fstream>

namespace Foundations::IO
{
	class CoreMember FileInstance
	{
	private:
		std::fstream* File = nullptr;
		String _Path;
		
		unsigned long long _CurrentID = 0;

		void ConstructFile();
	public:
		FileInstance();
		FileInstance(String const& Path);
		FileInstance(const FileInstance& Other) = delete;
		FileInstance(FileInstance&& Other) noexcept = delete;

		FileInstance& operator=(const FileInstance& Other) = delete;
		FileInstance& operator=(FileInstance&& Other) noexcept = delete;

		String Path() const;
		void Path(const String& New);

		bool Save();
		bool Load();
		bool Idle();
		bool Close();
	};
}