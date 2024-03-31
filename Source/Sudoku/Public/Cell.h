// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCellUpdatedDelagate, ACell*, Cell);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCellGroupCompletedDelagate);

UCLASS()
class SUDOKU_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CorrectValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanChangeValue = true;

	UFUNCTION(BlueprintCallable, BlueprintPure=true)
	bool IsCorrect();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCellUpdatedDelagate FOnCellUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCellGroupCompletedDelagate FOnCellGroupCompleted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 RowIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CollumIndex;

};
