#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h" // 視覚イベントのデータ構造に必要
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class FL1_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

protected:
    virtual void BeginPlay() override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

    // 視覚センサーコンポーネント
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComp;

    // 視覚の設定（視野角・距離など）
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    // 何かを視認・見失った瞬間に自動で呼ばれるイベント関数（UFUNCTION必須）
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
    void UpdateAIState();
    void PatrolRandomLocation();

private:
    FTimerHandle AIUpdateTimerHandle;

    // 巡回範囲の半径
    UPROPERTY(EditAnywhere, Category = "AI")
    float PatrolRadius = 1000.0f;

    // 攻撃を開始する距離（cm単位: 180 = 1.8m）
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 180.0f;

    // 現在視認・追跡中のプレイヤー（見失ったら nullptr になる）
    UPROPERTY()
    APawn* TargetPlayer = nullptr;
};