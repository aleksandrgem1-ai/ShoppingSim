// Subsystems/FurniturePlaceManagerSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Utils/FurnitureTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FurniturePlaceManagerSubsystem.generated.h"

class UFurnitureDataAsset;
class APlaceableActor;
class AStoreZoneActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlacementModeChanged, bool,
                                            bIsPlacing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFurniturePlaced,
                                             APlaceableActor *, FurnitureActor,
                                             AStoreZoneActor *, ZoneActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlacementPreviewUpdated,
                                            const FVector &, PreviewLocation);

/**
 * Подсистема управления режимами размещения, перемещения и удаления мебели
 */
UCLASS()
class SHOPPINGSIM_API UFurniturePlaceManagerSubsystem
    : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  // Запустить режим размещения мебели (FurnitureData — что размещать)
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  void EnterPlacementMode(UFurnitureDataAsset *FurnitureData);

  // Завершить (отменить) режим размещения
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  void ExitPlacementMode();

  // Подтвердить размещение на позиции (в зоне, либо в позиции)
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  bool PlaceFurnitureAt(AStoreZoneActor *ZoneActor, int32 SlotIndex);

  // Разместить произвольно (например, в Custom-зоне)
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  bool PlaceFurnitureAtPosition(AStoreZoneActor *ZoneActor,
                                const FVector &WorldPos);

  // Начать режим перемещения мебели
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  void StartMoveFurniture(APlaceableActor *FurnitureActor);

  // Удалить мебель из зоны (и вернуть на склад)
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  bool RemoveFurnitureFromZone(APlaceableActor *FurnitureActor);

  // Undo/Redo (по необходимости)
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  void UndoLastAction();
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  void RedoLastAction();

  // Делегаты для UI/логики
  UPROPERTY(BlueprintAssignable, Category = "FurniturePlacement")
  FOnPlacementModeChanged OnPlacementModeChanged;

  UPROPERTY(BlueprintAssignable, Category = "FurniturePlacement")
  FOnFurniturePlaced OnFurniturePlaced;

  UPROPERTY(BlueprintAssignable, Category = "FurniturePlacement")
  FOnPlacementPreviewUpdated OnPlacementPreviewUpdated;

  // Дать FurnitureData для текущего режима
  UFUNCTION(BlueprintCallable, Category = "FurniturePlacement")
  UFurnitureDataAsset *GetFurnitureToPlace() const;

private:
  // Текущий DataAsset для размещаемого объекта
  UPROPERTY()
  TObjectPtr<UFurnitureDataAsset> FurnitureToPlace;

  // Режим — размещение, перемещение, удаление
  bool bInPlacementMode = false;
  bool bMovingFurniture = false;
  UPROPERTY()
  TObjectPtr<APlaceableActor> MovingFurnitureActor;

  // Undo/Redo history (опционально)
  TArray<FString>
      PlacementHistory; // можно тут сделать структуру для хранения операций
  int32 HistoryIndex = -1;

  // Внутренние методы
  void PushHistory(const FString &Operation);
  void NotifyPlacementPreview(const FVector &Location);
};
