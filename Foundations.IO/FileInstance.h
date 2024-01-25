#pragma once

#include "IOCommon.h"
#include <fstream>

#include "Element.h"
#include "ElementList.h"
#include "Reference.h"

namespace Core::IO
{
	class IO_API FileInstance
	{
	private:
		std::fstream* fileInst = nullptr;
		std::string _ThisPath;

		Element* Root = nullptr;
		void ConstructRoot(bool DeleteIfValid = true);

		unsigned int CurrID = 0;

		bool WriteNode(std::ostream& out, Element& Root, int TabIndex);

		void OpenStream();
	public:
		FileInstance();
		FileInstance(const std::string& Path);
		FileInstance(const FileInstance& Obj) = delete;
		FileInstance(FileInstance&& Obj) = delete;
		~FileInstance();

		FileInstance& operator=(const FileInstance& Obj) = delete;
		FileInstance& operator=(FileInstance&& Obj) = delete;

		bool WriteToFile();
		bool LoadFromFile();

		Reference getRoot() const;
		unsigned int GetNextID() { return ++CurrID; }
	};
}