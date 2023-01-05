#include "FileInstance.h"

namespace Foundations::IO
{
	FileInstance::FileInstance()
	{
		_Path = String();
		File = nullptr;
		_CurrentID = 0;
	}
	FileInstance::FileInstance(String const& Path) : FileInstance()
	{
		_Path = Path;
	}

	void FileInstance::ConstructFile()
	{

	}

	String FileInstance::Path() const
	{
		return _Path;
	}
	void FileInstance::Path(const String& New)
	{
		if (Close())
		{
			_Path = New;
			Load();
		}
	}

	bool FileInstance::Save()
	{
		return false;
	}
	bool FileInstance::Load()
	{
		return false;
	}
	bool FileInstance::Idle()
	{
		return false;
	}
	bool FileInstance::Close()
	{
		return false;
	}
}