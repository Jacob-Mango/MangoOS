#include <hardware/filesystem/FAT12.h>

#include <memory.h>
#include <string.h>

#define SECTOR_SIZE 512

FILESYSTEM _FSysFat;

MOUNT_INFO _MountInfo;

uint8_t FAT[SECTOR_SIZE * 2];

void ToDOSFileName(const char *filename, char *fname, unsigned int FNameLength)
{
	unsigned int i = 0;

	if (FNameLength > 11)
		return;

	if (!fname || !filename)
		return;

	memset(fname, ' ', FNameLength);
	for (i = 0; i < strlen(filename) - 1 && i < FNameLength; i++)
	{

		if (filename[i] == '.' || i == 8)
			break;
		fname[i] = toupper(filename[i]);
	}
	if (filename[i] == '.')
	{
		for (int k = 0; k < 3; k++)
		{
			++i;
			if (filename[i])
				fname[8 + k] = filename[i];
		}
	}
	for (i = 0; i < 3; i++)
		fname[8 + i] = toupper(fname[8 + i]);
}

void FATGetFilesInRoot(char *ret)
{
	char *_ret = ret;
	char j = 0;
	unsigned char *buf;
	buf = (unsigned char *)vd_getCurrentDataDevice().read_sector(_MountInfo.rootOffset);
	while (*buf != 0)
	{
		DIRECTORY *dir = (DIRECTORY *)(buf);
		memcpy((char *)dir->Filename, ret, 11);
		buf += sizeof(dir);
		j++;
	}
	return;
}

char FATGetNumberOfFilesInRoot()
{
	char *ret;
	char j = 0;
	unsigned char *buf;
	buf = (unsigned char *)vd_getCurrentDataDevice().read_sector(_MountInfo.rootOffset);
	while (*buf != 0)
	{
		j++;
		buf += 0x20;
	}
	return ++j;
}

FILE FATDirectory(const char *DirectoryName)
{
	FILE file;
	unsigned char *buf;
	PDIRECTORY directory;

	char DosFileName[11];
	ToDOSFileName(DirectoryName, DosFileName, 11);
	DosFileName[11] = 0;

	for (int sector = 0; sector < 14; sector++)
	{
		buf = (unsigned char *)vd_getCurrentDataDevice().read_sector(_MountInfo.rootOffset + sector);

		directory = (PDIRECTORY)buf;
		for (int i = 0; i < 16; i++)
		{
			char name[11];
			memcpyTF(name, (char *)(directory->Filename), 11);
			name[11] = 0;

			if (strcmp(DosFileName, name) == 0)
			{
				strcpy(file.name, DirectoryName);
				file.id = 0;
				file.currentCluster = directory->FirstCluster;
				file.fileLength = directory->FileSize;
				file.eof = 0;
				file.fileLength = directory->FileSize;
				if (directory->Attrib == 0x10)
					file.flags = FS_DIRECTORY;
				else
					file.flags = FS_FILE;
				return file;
			}
			directory++;
		}
	}
	file.flags = FS_INVALID;
	return file;
}

void FATWrite(PFILE file, unsigned char *Buffer, unsigned int Length)
{
	if (!file)
		return;
	if (Length == 0)
		return;
	if (Buffer == 0)
		return;

	int StartSectorOfDataOnDisk = _MountInfo.rootOffset + _MountInfo.rootSize;

	unsigned char *sector = (unsigned char *)(vd_getCurrentDataDevice().read_sector(_MountInfo.rootOffset));
	for (int i = 0; i < 14; i++) // do for each sector
	{
		sector = (unsigned char *)(vd_getCurrentDataDevice().read_sector(_MountInfo.rootOffset + i));
		DIRECTORY *entry = (DIRECTORY *)sector;
	}
}

void FATRead(PFILE file, unsigned char *Buffer, unsigned int Length)
{
	if (file)
	{
		unsigned int physSector = 32 + (file->currentCluster - 1);
		unsigned char *sector = (unsigned char *)vd_getCurrentDataDevice().read_sector(physSector);

		memcpyTF((char *)Buffer, (char *)sector, 512);

		unsigned int FAT_Offset = file->currentCluster + (file->currentCluster / 2); //multiply by 1.5
		unsigned int FAT_Sector = 1 + (FAT_Offset / SECTOR_SIZE);
		unsigned int entryOffset = FAT_Offset % SECTOR_SIZE;

		sector = (unsigned char *)vd_getCurrentDataDevice().read_sector(FAT_Sector);
		memcpyTF((char *)FAT, (char *)sector, 512);

		sector = (unsigned char *)vd_getCurrentDataDevice().read_sector(FAT_Sector + 1);
		memcpyTF((char *)(FAT + SECTOR_SIZE), (char *)sector, 512);

		uint16_t nextCluster = *(uint16_t *)&FAT[entryOffset];

		if (file->currentCluster & 0x0001)
			nextCluster >>= 4;
		else
			nextCluster &= 0x0FFF;

		if (nextCluster >= 0xff8)
		{
			file->eof = 1;
			return;
		}

		if (nextCluster == 0)
		{
			file->eof = 1;
			return;
		}

		file->currentCluster = nextCluster;
	}
}

void FATClose(PFILE file)
{
	if (file)
		file->flags = FS_INVALID;
}

FILE FATOpenSubDir(FILE kFile, const char *filename)
{
	FILE file;

	char DosFileName[11];
	ToDOSFileName(filename, DosFileName, 11);
	DosFileName[11] = 0;

	while (!kFile.eof)
	{
		unsigned char buf[512];
		FATRead(&file, buf, 512);

		PDIRECTORY pkDir = (PDIRECTORY)buf;

		for (unsigned int i = 0; i < 16; i++)
		{
			char name[11];
			memcpyTF(name, (char *)(pkDir->Filename), 11);
			name[11] = 0;

			if (strcmp(name, DosFileName) == 0)
			{
				strcpy(file.name, filename);
				file.id = 0;
				file.currentCluster = pkDir->FirstCluster;
				file.fileLength = pkDir->FileSize;
				file.eof = 0;
				file.fileLength = pkDir->FileSize;

				if (pkDir->Attrib == 0x10)
					file.flags = FS_DIRECTORY;
				else
					file.flags = FS_FILE;
				return file;
			}
			pkDir++;
		}
	}

	file.flags = FS_INVALID;
	return file;
}

FILE FOpenFile(const char *FileName)
{

	FILE curDirectory;
	char *p = 0;
	bool rootDir = true;
	char *path = (char *)FileName;

	p = strchr(path, '\\');
	if (!p)
	{
		curDirectory = FATDirectory(path);

		if (curDirectory.flags == FS_FILE)
			return curDirectory;

		FILE ret;
		ret.flags = FS_INVALID;
		return ret;
	}

	p++;

	while (p)
	{
		char pathname[16];
		int i = 0;
		for (i = 0; i < 16; i++)
		{
			if (p[i] == '\\' || p[i] == '\0')
				break;

			pathname[i] = p[i];
		}
		pathname[i] = 0;

		if (rootDir)
		{
			curDirectory = FATDirectory(pathname);
			rootDir = false;
		}
		else
		{
			curDirectory = FATOpenSubDir(curDirectory, pathname);
		}

		if (curDirectory.flags == FS_INVALID)
			break;

		if (curDirectory.flags == FS_FILE)
			return curDirectory;

		p = strchr(p + 1, '\\');
		if (p)
			p++;
	}

	FILE ret;
	ret.flags = FS_INVALID;
	return ret;
}

void FATMount()
{
	PBOOTSECTOR bootsector;

	bootsector = (PBOOTSECTOR)vd_getCurrentDataDevice().read_sector(0);

	_MountInfo.numSectors = bootsector->Bpb.NumSectors;
	_MountInfo.fatOffset = 1;
	_MountInfo.fatSize = bootsector->Bpb.SectorsPerFat;
	_MountInfo.fatEntrySize = 8;
	_MountInfo.numRootEntries = bootsector->Bpb.NumDirEntries;
	_MountInfo.rootOffset = (bootsector->Bpb.NumberOfFats * bootsector->Bpb.SectorsPerFat) + 1;
	_MountInfo.rootSize = (bootsector->Bpb.NumDirEntries * 32) / bootsector->Bpb.BytesPerSector;
}

void FATInitialize(DATADEVICE datadev)
{
	strcpy(_FSysFat.Name, "FAT12");
	_FSysFat.Directory = FATDirectory;
	_FSysFat.Mount = FATMount;
	_FSysFat.Open = FATOpen;
	_FSysFat.Read = FATRead;
	_FSysFat.Close = FATClose;

	VolRegisterFileSystem(&_FSysFat, datadev.uniqueID);

	FMount();
}