#include "UI/HUD/MoneyHUDWidget.h"
#include "Components/TextBlock.h"

void UMoneyHUDWidget::SetMoney(int32 NewValue) {
  UE_LOG(LogTemp, Warning, TEXT("[HUD] SetMoney called: %d"), NewValue);

  if (!MoneyText) {
    UE_LOG(LogTemp, Error,
           TEXT("[HUD] MoneyText is nullptr � ������� BindWidget � ��� � BP!"));
    return;
  }

  // ����������� ����� � � 
  const FString Formatted = FString::Printf(TEXT("�%d"), NewValue);
  MoneyText->SetText(FText::FromString(Formatted));

  UE_LOG(LogTemp, Warning, TEXT("[HUD] MoneyText updated to: %s"), *Formatted);
}
