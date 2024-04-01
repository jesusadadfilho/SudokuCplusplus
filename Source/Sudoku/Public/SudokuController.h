// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Cell.h>
#include "SudokuController.generated.h"

UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
	Easy = 0 UMETA(DisplayName = "Easy"),
	Medium = 1 UMETA(DisplayName = "Medium"),
	Hard = 2 UMETA(DisplayName = "Hard")
};

USTRUCT(BlueprintType)
struct FEnlapsedTime
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Minutes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Seconds;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoardCompleted);

UCLASS()
class SUDOKU_API ASudokuController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASudokuController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 Patter(int32 row, int32 collum);

	TArray<int32> Shuffle(int32 listSize, int32 startIndex = 0);

	UFUNCTION(BlueprintCallable, Category="Creation")
	void SetupSudoku(int32 SudokuBase, EGameDifficulty difficulty);

	UFUNCTION(BlueprintCallable)
	TArray<ACell*> ReciveRowByIndex(int32 RowIndex);

	UFUNCTION(BlueprintCallable)
	void CheckBoard(ACell* Cells);

	UFUNCTION(BlueprintCallable)
	TArray<ACell*> ReciveCollumByIndex(int32 CollumIndex);

	UFUNCTION(BlueprintCallable)
	void PlayGame();

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	int32 GetElapsedTimeSeconds();

	UFUNCTION(BlueprintCallable)
	int32 GetElapsedTimeMinutes();

	UFUNCTION(BlueprintCallable)
	FEnlapsedTime GetElapsedTime();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Base;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CellSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ElapsedTimeSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGamePlaying;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnBoardCompleted FOnBoardCompleted;

	FTimerHandle TimerHandle;

	TArray<TArray<ACell*>> Board;

	TArray<ACell*> ReciveSquareByCell(ACell* Cell);

	bool IsListCompleted(TArray<ACell*> Cells);

	void CheckGroupByCell(ACell* Cell);

	void SetupCellByDifficulty(EGameDifficulty Difficulty);
};
