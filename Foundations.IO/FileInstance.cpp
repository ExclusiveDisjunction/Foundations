#include "FileInstance.h"
#include <filesystem>

using namespace std;

namespace Core::IO
{
	FileInstance::FileInstance()
	{
		ConstructRoot();
	}
	FileInstance::FileInstance(const std::string& Path) : FileInstance()
	{
		_ThisPath = Path;
		OpenStream();
	}
	FileInstance::~FileInstance()
	{
		if (fileInst)
		{
			fileInst->close();
			delete fileInst;
		}

		if (Root)
			delete Root;

		_ThisPath = std::string();
	}

	void FileInstance::ConstructRoot(bool Delete)
	{
		if (Delete && Root)
			delete Root;

		Root = new Element(this, nullptr, true);
		Root->Type = "ROOT";
	}

	Reference FileInstance::getRoot() const
	{
		return !Root ? Reference(nullptr) : Root->getReference();
	}

	void FileInstance::OpenStream()
	{
		fileInst = new std::fstream(_ThisPath, std::ios::app | std::ios::beg | std::ios::in | std::ios::out);

		if (fileInst->bad())
		{
			fileInst->close();
			delete fileInst;
			_ThisPath = std::string();
		}
	}

	bool FileInstance::WriteToFile()
	{
		std::ofstream outFile(_ThisPath + ".temp", std::ios::trunc);
		if (!outFile)
			return false;

		outFile << "DECLARE ElementIO V:1 !!DECLARE" << endl;

		outFile << "SECTION TYPELIST" << endl
			<< "!!SECTION TYPELIST" << endl;

		outFile << "SECTION CONFIGURATION" << endl
			<< "!!SECTION CONFIGURATION" << endl;

		outFile << "SECTION BLOBS" << endl
			<< "!!SECTION BLOBS" << endl;

		outFile << "SECTION ELEMENTS" << endl;
		bool Return = WriteNode(outFile, *Root, 1);
		outFile << "!!SECTION ELEMENTS" << endl;

		//Release handles
		outFile.close();

		if (std::filesystem::exists(_ThisPath))
		{
			if (fileInst)
			{
				fileInst->close();
				delete fileInst;
				fileInst = nullptr;
			}

			std::filesystem::remove(_ThisPath);
			std::filesystem::copy_file(_ThisPath + ".temp", _ThisPath); //Copy file over
			OpenStream(); //Opens the file again.

			std::filesystem::remove(_ThisPath + ".temp");
		}
		else
			std::filesystem::rename(_ThisPath + ".temp", _ThisPath);
		

		return Return;
	}
	bool FileInstance::WriteNode(std::ostream& out, Element& Target, int TabIndex)
	{
		std::string TabIndexStr;
		for (int i = 0; i < TabIndex; i++)
			TabIndexStr += '\t';

		out << TabIndexStr;
		Target.Begin = out.tellp();
		out << "ELEMENT " << Target.ID << ' ' << (Target.Type == std::string() ? "NONE" : Target.Type);

		//Atributes
		{
			/*
			* The node must be 'scratch-written' IF
			* 1. The node is modified
			* 2. The node's header locations are not known
			* 3. The fileinstance of the previous load is not valid.
			* 
			* The node can be copied IF
			* 1. The last three critera are all false
			* 2. The node is not loaded AND the header locations are known AND the fileinstance of the previous load is valid.
			*/

			bool IsModified = Target.State & ElementState::ES_Modified,
				HeaderLocIsKnown = Target.HBegin != 0 && Target.HEnd != 0,
				PrevFileExists = fileInst != nullptr;

			bool scratchWrite = IsModified || !HeaderLocIsKnown || !PrevFileExists;
			bool copy = (!(Target.State & ES_Loaded) && HeaderLocIsKnown && PrevFileExists) || !scratchWrite;

			if (scratchWrite)
			{
				Target.AttrBegin = out.tellp();
				AttributesWriter wr(out); //Export attributes.
				Target.WriteAttributes(wr);

				Target.HEnd = out.tellp();
				out << ' ';
			}
			else if (copy)
			{
				std::streampos BegTemp = out.tellp();
				
				//In this, we will copy the attributes from the previously written Element.
				std::fstream& prev = *(this->fileInst);

				//Move to the attributes begin location, copy the data.
				prev.seekg(Target.AttrBegin);
				size_t ArrSize = Target.HEnd - Target.AttrBegin + 1;
				char* Data = new char[ArrSize];
				memset(Data, 0, ArrSize);

				prev.read(Data, ArrSize - 1);

				//Take the copied data and write it to the new stream.
				out.write(Data, ArrSize-1);
				delete[] Data;

				Target.AttrBegin = BegTemp;
				Target.HEnd = out.tellp();
				out << ' ';
			}		
			else
				return false; //Dont know what to do :(. Shouldnt ever happen. 
		}		

		if (Target.SupportsChildren() && Target.getChildren().count())
		{
			out << endl;
			//Output each child with a tab index one greater.

			try
			{
				ElementList& Children = Target.getChildren();
				for (Element& Child : Children)
				{
					if (!WriteNode(out, Child, TabIndex + 1))
						return false;
				}
			}
			catch (...)
			{
				return false;
			}

			out << TabIndexStr << "!!ELEMENT " << Target.ID;
			Target.End = out.tellp();
			out << endl;
		}
		else
		{
			out << "!!ELEMENT " << Target.ID;
			Target.End = out.tellp();

			out << endl;
		}

		return true;
	}
	bool FileInstance::LoadFromFile()
	{
		return false;
	}
}