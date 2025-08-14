// CashRegister.cpp (Œ¡ÕŒ¬À≈ÕÕ€…)

#include "CashRegister.h"
#include "Components/StaticMeshComponent.h"
#include "EconomySubsystem.h" // <-- 1. ƒŒ¡¿¬Àﬂ≈Ã ÕŒ¬€… »Õ Àﬁƒ

ACashRegister::ACashRegister() {
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  RootComponent = MeshComponent;

  InteractionPromptText =
      FString(TEXT("Get Daily Profit")); // ÃÓÊÌÓ ÔÓÏÂÌˇÚ¸ ÚÂÍÒÚ
}

void ACashRegister::OnInteract(AController *Interactor) {
  Super::OnInteract(Interactor);

  // 2. œŒÀ”◊¿≈Ã ——€À ” Õ¿ œŒƒ—»—“≈Ã” › ŒÕŒÃ» »
  if (UEconomySubsystem *EconomySubsystem =
          GetWorld()->GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
    // 3. ¬€«€¬¿≈Ã ≈≈ Ã≈“Œƒ ƒÀﬂ ƒŒ¡¿¬À≈Õ»ﬂ ƒ≈Õ≈√
    const int32 ProfitAmount = 100; // —ÛÏÏ‡ ‰Îˇ ÔËÏÂ‡
    EconomySubsystem->AddMoney(ProfitAmount);

    UE_LOG(LogTemp, Log, TEXT("Added %d to balance via Cash Register."),
           ProfitAmount);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("CashRegister could not find EconomySubsystem!"));
  }
}