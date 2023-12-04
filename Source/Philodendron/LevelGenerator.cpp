
#include "LevelGenerator.h"
#include "MainGameStateBase.h"
#include "PaperTileMap.h"
#include "PaperTileMapComponent.h"
#include "ManagerInteractableActors.h"
#include "NavigationSystem.h"
#include "MapHelperToCreate.h"



ALevelGenerator::ALevelGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetLevelGenerator(this);
}


void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma optimize("", off)
void ALevelGenerator::GenerateLevel()
{
	SeedForMapTileGeneration = FDateTime::Now().ToUnixTimestamp();

	GenerateMap(SeedForMapTileGeneration);

	int32 MapEdge = SizeOneTale * MapSizeInCountTale;

	FVector3d MapCenter;
	MapCenter.X = MapEdge / 2;
	MapCenter.Y = MapCenter.X;
	MapCenter.Z = 0;

	FVector TempMapEdge;
	TempMapEdge.Y = MapEdge;
	TempMapEdge.X = MapCenter.X;
	TempMapEdge.Z = 0;

	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateSun(MoveTemp(TempMapEdge));

	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateCore(MapCenter);

	FVector3d MapSize;

	MapSize.X = SizeOneTale * MapSizeInCountTale;
	MapSize.Y = MapSize.X;
	MapSize.Z = 0;

	TArray<TArray<int32>> Grid;
	
	int32 Weight = MapSize.X / SmallRadiusForObject;
	int32 Lenght = MapSize.Y / SmallRadiusForObject;

	for (int32 i = 0; i < Weight; ++i)
	{
		TArray<int32> GridString;
		GridString.Reserve(Lenght);
		for (int32 j = 0; j < Lenght; ++j)
		{
			GridString.Add(0);
		}
		Grid.Add(GridString);
	}


	int32 CoreXPossition = Weight / 2;
	int32 CoreYPossition = Lenght / 2;
	int32 DeathAreaSq = (DeathAreaAroundCore / SmallRadiusForObject) * (DeathAreaAroundCore / SmallRadiusForObject);
	ChangeIncreaseGridElementsInRadius(Grid, CoreXPossition, CoreYPossition, DeathAreaSq, -1, true);

	GenerateInteractableActors(Grid);

	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GenerateActionForInteractableActor();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GenerateStartedAddons(StartedAddons);
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GenerateStartedWorkers(StartedWorkers);

}
#pragma optimize("", on)


void ALevelGenerator::GenerateInteractableActors(TArray<TArray<int32>>& Grid)
{
	int32 Weight = Grid.Num();
	int32 Lenght = Grid[0].Num();
	for (const auto& IActor : ObjectCountForGenerate)
	{
		TArray<int32> DiffObjPriority;
		ChangeObjPriority(DiffObjPriority, Grid, IActor.Key);
		int32 i = 0;

		EInteractableActorCreatingType TypeToCreatObject;
		if (!TypeToGenerateObject.Contains(IActor.Key))
		{
			TypeToCreatObject = EInteractableActorCreatingType::FinalStage;
		}
		else
		{
			TypeToCreatObject = TypeToGenerateObject[IActor.Key];
		}

		while (i < IActor.Value && DiffObjPriority.Num() > 0)
		{
			int32 NumberPriority = FMath::RandRange(0, DiffObjPriority.Num() - 1);

			int32 ObjectNumber = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GenerateRandomObject(IActor.Key);
			FVector3d ObjForCreate = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetBoxSizeFinalFlipbook(IActor.Key, ObjectNumber);

			if (ObjForCreate == FVector3d::ZeroVector)
			{
				break;
			}

			TArray<FVector2D> AvailablePossition;

			int32 Radius = ObjForCreate.X / SmallRadiusForObject;
			if (ObjForCreate.X * SmallRadiusForObject < Radius)
			{
				Radius += 1;
			}
			int32 RadiusSq = Radius * Radius;

			for (int32 k = 0; k < Weight; ++k)
			{
				for (int32 j = 0; j < Lenght; ++j)
				{
					if (Grid[k][j] != DiffObjPriority[NumberPriority] || !CheckGridPlace(Grid, k, j, Radius))
					{
						continue;
					}

					FVector2D TempPos;
					TempPos.X = k;
					TempPos.Y = j;

					AvailablePossition.Add(TempPos);
				}
			}

			if (AvailablePossition.Num() == 0)
			{
				int32 PriorityToRemove = DiffObjPriority[NumberPriority];
				DiffObjPriority.Remove(PriorityToRemove);
				continue;
			}

			int32 NumberPossition = FMath::RandRange(0, AvailablePossition.Num() - 1);

			if (TryCreateObj(Grid, AvailablePossition[NumberPossition].X, AvailablePossition[NumberPossition].Y, RadiusSq, IActor.Key, ObjectNumber, TypeToCreatObject))
			{
				DiffObjPriority.Empty();
				ChangeObjPriority(DiffObjPriority, Grid, IActor.Key);
				++i;
			}
		}

		ClearGridAfterCreateIActor(Grid);

	}
}

void ALevelGenerator::ClearGridAfterCreateIActor(TArray<TArray<int32>>& Grid)
{
	int32 Weight = Grid.Num();
	int32 Lenght = Grid[0].Num();

	for (int32 i = 0; i < Weight; ++i)
	{
		for (int32 j = 0; j < Lenght; ++j)
		{
			if (Grid[i][j] != -1)
			{
				Grid[i][j] = 0;
			}
		}
	}
}

void ALevelGenerator::ChangeIncreaseGridElementsInRadius(TArray<TArray<int32>>& Grid, int32 X, int32 Y, int32 RadiusSq, int32 Value, bool IsChange)
{
	int32 Weight = Grid.Num();
	int32 Lenght = Grid[0].Num();

	for (int32 i = 0; i < Weight; ++i)
	{
		for (int32 j = 0; j < Lenght; ++j)
		{
			if ((X - i) * (X - i) + (Y - j) * (Y - j) <= RadiusSq)
			{
				if (Grid[i][j] == -1)
				{
					continue;
				}

				if (IsChange)
				{
					Grid[i][j] = Value;
				}
				else
				{
					Grid[i][j] += Value;
				}
			}
		}
	}
}

bool ALevelGenerator::CheckGridPlace(TArray<TArray<int32>>& Grid, int32 X, int32 Y, int32 Radius)
{
	int32 Weight = Grid.Num();
	int32 Lenght = Grid[0].Num();

	int32 StartX = X - Radius < 0 ? 0 : X - Radius;
	int32 StartY = Y - Radius < 0 ? 0 : Y - Radius;

	int32 FinishX = X + Radius > Weight ? Weight : X + Radius;
	int32 FinishY = Y + Radius > Lenght ? Lenght : Y + Radius;

	for (int32 i = StartX; i < FinishX; ++i)
	{
		for (int32 j = StartY; j < FinishY; ++j)
		{
			if (((X - i) * (X - i) + (Y - j) * (Y - j) <= Radius) && (Grid[i][j] < 0))
			{
				return false;
			}
		}
	}
	return true;
}


void ALevelGenerator::ChangeObjPriority(TArray<int32>& ObjPriority, TArray<TArray<int32>>& Grid, EInteractableActorType AType)
{
	int32 Weight = Grid.Num();
	int32 Lenght = Grid[0].Num();
	TArray<int32> DiffNeighbourCounts;
	for (int32 i = 0; i < Weight; ++i)
	{
		for (int32 j = 0; j < Lenght; ++j)
		{
			if (Grid[i][j] >= 0 && DiffNeighbourCounts.Find(Grid[i][j]) == INDEX_NONE)
			{
				DiffNeighbourCounts.Add(Grid[i][j]);
			}
		}
	}

	for (const auto& Count : DiffNeighbourCounts)
	{
		if (!Neighbours.Contains(AType))
		{
			ObjPriority.Add(Count);
			continue;
		}

		const TArray<FPriorityForNeighbours>& TempPriority = Neighbours[AType].PriorityForNeighbours;
		int32 TempPriorityNum = TempPriority.Num();

		for (int32 i = 0; i < TempPriorityNum; ++i)
		{
			if (Count <= TempPriority[i].UpperBoundOfTheGroupCountNeighbours || i == TempPriorityNum - 1)
			{
				for (int32 j = 0; j < TempPriority[i].Priority; ++j)
				{
					ObjPriority.Add(Count);
				}
				break;
			}

			
		}
	}

}
#pragma optimize("", off)
bool ALevelGenerator::TryCreateObj(TArray<TArray<int32>>& Grid, int32 X, int32 Y, int32 RadiusSq, EInteractableActorType IAType, int32 ObjectNumber, EInteractableActorCreatingType CreatingType)
{
	FVector Location;
	//Location.X = TileMapComponent[0]->Bounds.GetBox().Min.X + X * SmallRadiusForObject;
	//Location.Y = TileMapComponent[0]->Bounds.GetBox().Min.Y + Y * SmallRadiusForObject;
	Location.X = X * SmallRadiusForObject;
	Location.Y = Y * SmallRadiusForObject;
	
	Location.Z = 0;

	if (!Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateNewInteractableActor(IAType, Location, ObjectNumber, CreatingType))
	{
		return false;
	}

	ChangeIncreaseGridElementsInRadius(Grid, X, Y, RadiusSq, -1, true);
	if (Neighbours.Contains(IAType))
	{
		int32 GroupRadius = Neighbours[IAType].GroupingRadius / SmallRadiusForObject;
		if (Neighbours[IAType].GroupingRadius * SmallRadiusForObject < GroupRadius)
		{
			GroupRadius += 1;
		}

		int32 GroupRadiusSq = GroupRadius * GroupRadius;
		ChangeIncreaseGridElementsInRadius(Grid, X, Y, GroupRadiusSq, 1, false);
	}

	return true;
}
#pragma optimize("", on)

const TArray<TObjectPtr<AMapHelperToCreate>>& ALevelGenerator::GetMapHelperList() const
{
	return MapHelpers;
}

int32 ALevelGenerator::GetSeedForMapTileGeneration() const
{
	return SeedForMapTileGeneration;
}

void ALevelGenerator::GenerateMap(int32 RandomSeed)
{
	int32 CountTileMap = 0;

	int32 DeltaX = SizeOneTale / 2;
	int32 DeltaY = SizeOneTale / 2;

	FRandomStream Randomize = FRandomStream();

	Randomize.Initialize(RandomSeed);

	for (int32 i = 0; i < MapSizeInCountTale; ++i)
	{
		for (int32 j = 0; j < MapSizeInCountTale; ++j)
		{
			FVector NewPossition;
			NewPossition.X = SizeOneTale * i + DeltaX;
			NewPossition.Y = SizeOneTale * j + DeltaY;
			NewPossition.Z = 0;

			AMapHelperToCreate* NewMapTile = this->GetWorld()->SpawnActor<AMapHelperToCreate>(AMapHelperToCreate::StaticClass(), { 0, 0, 0 }, { 0, 0, 0 });

			int32 TileNumber = Randomize.FRandRange(0, TileMap.Num() - 1);
			NewMapTile->InitTile(NewPossition, TileMap[TileNumber], TileNumber);

			MapHelpers.Add(NewMapTile);

			CountTileMap += 1;
		}
	}
}