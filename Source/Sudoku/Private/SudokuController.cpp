// Fill out your copyright notice in the Description page of Project Settings.


#include "SudokuController.h"

#include <chrono>
#include <random>

// Sets default values
ASudokuController::ASudokuController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASudokuController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASudokuController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGamePlaying)
	{
		ElapsedTimeSeconds += DeltaTime;
	}

}

int32 ASudokuController::Patter(int32 row, int32 collum)
{
	return ((Base*(row%Base))+row/Base+collum)%CellSize;
}

TArray<int32> ASudokuController::Shuffle(int32 listSize, int32 startIndex)
{
	TArray<int32> list;
	for (int32 i = 0; i < listSize; i++)
	{
		list.Add(i + startIndex);
	}
	int32 lastIndex = list.Num() - 1;
	for(int32 i = 0; i <= lastIndex; i++)
	{
		int32 Index = FMath::RandRange(i, lastIndex);
		if (i != Index)
		{
			list.Swap(i, Index);
		}
	}
	return list;
}

void ASudokuController::SetupSudoku(int32 SudokuBase, EGameDifficulty difficulty)
{
	ElapsedTimeSeconds = 0.f;
	Base = SudokuBase;
	CellSize = Base * Base;
	TArray<int32> rows;
	TArray<int32> colluns;

	Board = TArray<TArray<ACell*>>();

	for (int32 o : Shuffle(Base))
	{
		for (int32 p : Shuffle(Base))
		{
			rows.Add(p*Base + o);
		}
	}

	for (int32 c : Shuffle(Base))
	{
		for (int32 g : Shuffle(Base))
		{
			colluns.Add(g*Base + c);
		}
	}

	TArray<int32> numbs = Shuffle((Base*Base), 1);

	int32 rowIndex = 0;
	for (int32 r : rows)
	{
		TArray<ACell*> row;
		int32 collunIndex = 0;
		for (int c : colluns)
		{
			int32 cel = numbs[Patter(r, c)];
			ACell* cell = GetWorld()->SpawnActor<ACell>(ACell::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
			cell->CorrectValue = cel;
			cell->RowIndex = rowIndex;
			cell->CollumIndex = collunIndex;
			cell->FOnCellUpdated.AddDynamic(this, &ASudokuController::CheckBoard);
			row.Add(cell);
			collunIndex += 1;
		}
		rowIndex += 1;
		Board.Add(row);
	}
	SetupCellByDifficulty(difficulty);

    
}

TArray<ACell*> ASudokuController::ReciveRowByIndex(int32 RowIndex)
{
	TArray<ACell*> row = TArray<ACell*>();
	for(TArray<ACell*> r : Board)
	{
		for (ACell* cell : r)
		{
			if(cell->RowIndex == RowIndex)
			{
				row.Add(cell);
			}
		}
	}
	return row;
}

void ASudokuController::CheckBoard(ACell* Cell)
{
	CheckGroupByCell(Cell);
	bool isComplete = true;
	for (TArray<ACell*> row : Board)
	{
		for (ACell* cell : row)
		{
			if (!cell->IsCorrect())
			{
				isComplete = false;
				break;
			}
		}
	}

	if (isComplete) {
		for (TArray<ACell*> row : Board)
		{
			for (ACell* cell : row)
			{
				cell->FOnCellGroupCompleted.Broadcast();
			}
		}
		FOnBoardCompleted.Broadcast();
		PauseGame();
	}
}

TArray<ACell*> ASudokuController::ReciveCollumByIndex(int32 CollumIndex)
{
	TArray<ACell*> collum = TArray<ACell*>();
	for(TArray<ACell*> row : Board)
	{
		for (ACell* cell : row)
		{
			if(cell->CollumIndex == CollumIndex)
			{
				collum.Add(cell);
			}
		}
	}
	return collum;
}

void ASudokuController::PlayGame()
{
	bIsGamePlaying = true;
}

void ASudokuController::PauseGame()
{
	bIsGamePlaying = false;
}

int32 ASudokuController::GetElapsedTimeSeconds()
{
	if (ElapsedTimeSeconds > 60)
	{
		return int32(ElapsedTimeSeconds) % 60;
	}
	return int32(ElapsedTimeSeconds);
}

int32 ASudokuController::GetElapsedTimeMinutes()
{
	if (ElapsedTimeSeconds > 60)
	{
		return int32(ElapsedTimeSeconds) / 60;
	}
	return 0;
}

FEnlapsedTime ASudokuController::GetElapsedTime()
{
	FEnlapsedTime enlapsedTime;
	enlapsedTime.Minutes = GetElapsedTimeMinutes();
	enlapsedTime.Seconds = GetElapsedTimeSeconds();
	return enlapsedTime;
}


TArray<ACell*> ASudokuController::ReciveSquareByCell(ACell* Cell)
{
	TArray<ACell*> square = TArray<ACell*>();
	int32 firstSquareRowIndex = Cell->RowIndex - (Cell->RowIndex % Base);
	int32 firstSquareCollumIndex = Cell->CollumIndex - (Cell->CollumIndex % Base);
	for (int32 i = 0; i < Base; i++)
	{
		for (int32 j = 0; j < Base; j++)
		{
			square.Add(Board[firstSquareRowIndex + i][firstSquareCollumIndex + j]);
		}
	}
	return square;
}

bool ASudokuController::IsListCompleted(TArray<ACell*> Cells)
{
	for (ACell* cell : Cells)
	{
		if (!cell->IsCorrect())
		{
			return false;
		}
	}
	return true;
}

void ASudokuController::CheckGroupByCell(ACell* Cells)
{
	TArray<ACell*> row = ReciveRowByIndex(Cells->RowIndex);
	TArray<ACell*> collum = ReciveCollumByIndex(Cells->CollumIndex);
	TArray<ACell*> square = ReciveSquareByCell(Cells);
	if (IsListCompleted(row))
	{
		for (ACell* cell : row)
		{
			cell->FOnCellGroupCompleted.Broadcast();
		}
	}
	if (IsListCompleted(collum))
	{
		for (ACell* cell : collum)
		{
			cell->FOnCellGroupCompleted.Broadcast();
		}
	}
	if (IsListCompleted(square))
	{
		for (ACell* cell : square)
		{
			cell->FOnCellGroupCompleted.Broadcast();
		}
	}	
}

void ASudokuController::SetupCellByDifficulty(EGameDifficulty Difficulty)
{
	int32 totalCells = CellSize;
	int32 correctCells = 0;
	switch (Difficulty)
	{	
	case EGameDifficulty::Easy:
		for(TArray<ACell*> row : Board)
		{
			for (ACell* cell : row)
			{
				if (FMath::RandRange(0.0, 1.0) > 0.2) {
					cell->CurrentValue = cell->CorrectValue;
					cell->bCanChangeValue = false;
				}
			}
		}
		break;
	case EGameDifficulty::Medium:
		for (TArray<ACell*> row : Board)
		{
			for (ACell* cell : row)
			{
				if (FMath::RandRange(0.0, 1.0) > 0.4) {
					cell->CurrentValue = cell->CorrectValue;
					cell->bCanChangeValue = false;
				}
			}
		}
		break;
	case EGameDifficulty::Hard:
		for (TArray<ACell*> row : Board)
		{
			for (ACell* cell : row)
			{
				if (FMath::RandRange(0.0, 1.0) > 0.6) {
					cell->CurrentValue = cell->CorrectValue;
					cell->bCanChangeValue = false;
				}
			}
		}
		break;
	default:
		for (TArray<ACell*> row : Board)
		{
			for (ACell* cell : row)
			{
				if (FMath::RandRange(0.0, 1.0) > 0.2) {
					cell->CurrentValue = cell->CorrectValue;
					cell->bCanChangeValue = false;
				}
			}
		}
		break;
	}

}
