#include "Player/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "FL1.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	// ローカルプレイヤーのコントローラーにのみタッチ操作を表示する
	if (IsLocalPlayerController() && ShouldUseTouchControls()) {
		// モバイルコントロールウィジェットを生成する
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget) {
			// コントロールをプレイヤーの画面に追加する
			MobileControlsWidget->AddToPlayerScreen(0);
		}
		else {
			UE_LOG(LogFL1, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}
}

void AMyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// ローカルプレイヤーのコントローラーにのみIMCを追加する
	if (IsLocalPlayerController()) {
		// 入力マッピングコンテキストを追加する
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts) {
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
			
			// モバイルタッチ入力を使用していない場合にのみ、これらのIMCを追加する
			if (!ShouldUseTouchControls()) {
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts) {
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AMyPlayerController::ShouldUseTouchControls() const {
	// モバイルプラットフォームであるか、またはbForceTouchControlsがtrueの場合にタッチ操作を使用する
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;

}