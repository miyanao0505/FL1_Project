#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/// <summary>
/// 三人称視点のゲーム用の基本的なPlayerControllerクラス
/// 入力マッピングを管理する
/// </summary>
UCLASS(abstract)
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/// <summary>
	/// 入力マッピングコンテキスト
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/// <summary>
	/// モバイルで除外される入力マッピングコンテキスト
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/// <summary>
	/// モバイルコントロールウィジェットのクラス
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/// <summary>
	/// モバイルコントロールウィジェットへのポインタ
	/// </summary>
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/// <summary>
	/// trueの場合、プレイヤーはモバイルプラットフォームでなくてもUMGタッチコントロールを使用する
	/// </summary>
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/// <summary>
	/// ゲームプレイの初期化
	/// </summary>
	virtual void BeginPlay() override;

	/// <summary>
	/// 入力マッピングコンテキストの設定
	/// </summary>
	virtual void SetupInputComponent() override;

	/// <summary>
	/// プレイヤーがUMGタッチコントロールを使用すべきかどうかを返す
	/// </summary>
	bool ShouldUseTouchControls() const;

};
