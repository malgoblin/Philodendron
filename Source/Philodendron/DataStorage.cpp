#include "DataStorage.h"


void UDataStorage::SetStorageSize(int32 Size)
{
	AvailableMemory = Size;
	MaxSize = Size;
}

void UDataStorage::AddStorageSize(int32 Size)
{
	AvailableMemory += Size;
	MaxSize += Size;
}

// rename -> check space/ has enough
bool UDataStorage::TryToAddNewData(int32 DataSize)
{
	return DataSize <= AvailableMemory;
}

void UDataStorage::AddData(int32 DataSize)
{
	if (AvailableMemory < DataSize)
	{
		return;
	}
	AvailableMemory -= DataSize;
}

void UDataStorage::RemoveData(int32 DataSize)
{
	if (AvailableMemory > MaxSize - DataSize)
	{
		return;
	}
	AvailableMemory += DataSize;
}

void UDataStorage::ResetStorage()
{
	AvailableMemory = MaxSize;
}
