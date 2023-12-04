// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumActionTreeElementName.h"
#include "WorkerTreeElementWidget.generated.h"

class UButton;
class UTextBlock;
class IWorkerTreeElementListener;

UCLASS()
class PHILODENDRON_API UWorkerTreeElementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorkerTreeElementWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


	void InitTreeElementWidget(EActionTreeElementName Name, const FString& Text, IWorkerTreeElementListener* Listener);

	void HideSelectButton();
	void ShowSelectButton();
	void HideDiselectButton();
	void ShowDiselectButton();

	void ClearMemorySize();
	void SetMemorySize(int32 Size);

	EActionTreeElementName GetTreeElementName() const;

	void MakeActive();
	void MakeUnActive();

private:
	UFUNCTION()
	void PressOnSelect();

	UFUNCTION()
	void PressOnDiselect();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> Select;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> Diselect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> VisibleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> MemorySize;

private:
	UPROPERTY()
	EActionTreeElementName TreeElementName;
	
	IWorkerTreeElementListener* ButtonListener;

};
