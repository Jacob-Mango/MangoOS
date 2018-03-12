#include <hardware/filesystem/filesystem.h>
#include <string.h>

#define DEVICE_MAX 26

PFILESYSTEM _FileSystems[DEVICE_MAX];

FILE VolOpenFile(const char *fname)
{
	if (fname)
	{
		unsigned char device = 'a';
		char *filename = (char *)fname;
		if (fname[2] == ':')
		{
			device = fname[0];
			filename += 3;
		}

		if (_FileSystems[device - 'a'])
		{
			FILE file = _FileSystems[device - 'a']->Open(filename);
			file.deviceID = device;
			return file;
		}
	}

	FILE file;
	file.flags = FS_INVALID;
	return file;
}

void VolReadFile(PFILE file, unsigned char *Buffer, unsigned int Length)
{

	if (file)
		if (_FileSystems[file->deviceID - 'a'])
			_FileSystems[file->deviceID - 'a']->Read(file, Buffer, Length);
}

void VolWriteFile(PFILE file, unsigned char *Buffer, unsigned int Length)
{
	if (file)
		if (_FileSystems[file->deviceID - 'a'])
			_FileSystems[file->deviceID - 'a']->Write(file, Buffer, Length);
}

void VolCloseFile(PFILE file)
{
	if (file)
		if (_FileSystems[file->deviceID - 'a'])
			_FileSystems[file->deviceID - 'a']->Close(file);
}

void VolRegisterFileSystem(PFILESYSTEM fsys, unsigned int deviceID)
{
	static int i = 0;

	if (i < DEVICE_MAX)
		if (fsys)
		{

			_FileSystems[deviceID] = fsys;
			i++;
		}
}

void VolUnregisterFileSystem(PFILESYSTEM fsys)
{

	for (int i = 0; i < DEVICE_MAX; i++)
		if (_FileSystems[i] == fsys)
			_FileSystems[i] = 0;
}

void VolUnregisterFileSystemByID(unsigned int deviceID)
{
	if (deviceID < DEVICE_MAX)
		_FileSystems[deviceID] = 0;
}