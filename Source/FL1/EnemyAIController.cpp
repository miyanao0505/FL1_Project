#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
    // 1. 視覚センサーコンポーネントの作成
    AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
    SetPerceptionComponent(*AIPerceptionComp);

    // 2. 視覚設定（SightConfig）の作成とパラメータ設定
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1500.0f;                 // 見える最大距離（15m）
    SightConfig->LoseSightRadius = 1800.0f;             // 一度見つけた後、見失う距離（18m）
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;  // 視野角（左右60度＝合計120度の扇形）
    SightConfig->SetMaxAge(3.0f);                       // 視界から消えても3秒間は記憶する

    // 敵・中立・味方のすべてを検知対象にする（デフォルトでは敵味方判定が未設定だと検知しないため重要）
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // センサーに視覚設定を登録
    AIPerceptionComp->ConfigureSense(*SightConfig);
    AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // 視認イベントに関数をバインド（登録）
    if (AIPerceptionComp)
    {
        AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
    }

    // 0.3秒ごとにAIの行動（追跡 or 攻撃 or 巡回）を更新
    GetWorldTimerManager().SetTimer(
        AIUpdateTimerHandle,
        this,
        &AEnemyAIController::UpdateAIState,
        0.3f,
        true
    );

    PatrolRandomLocation();
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // 検知した対象がプレイヤーかどうか確認
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Actor != PlayerPawn) return;

    if (Stimulus.WasSuccessfullySensed())
    {
        // 視界に入った！（壁越しではない）
        TargetPlayer = PlayerPawn;
    } else
    {
        // 壁に隠れた・距離が離れたなどで完全に見失った
        TargetPlayer = nullptr;
    }
}

void AEnemyAIController::UpdateAIState()
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    if (TargetPlayer)
    {
        // --- 追跡・攻撃モード ---
        float Distance = FVector::Dist(MyPawn->GetActorLocation(), TargetPlayer->GetActorLocation());

        if (Distance <= AttackRange)
        {
            // 攻撃射程内に入ったら移動を止めてプレイヤーの方を向き、攻撃！
            StopMovement();

            // プレイヤーの方向へスムーズに回転させる
            FVector Direction = (TargetPlayer->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal2D();
            if (!Direction.IsNearlyZero())
            {
                MyPawn->SetActorRotation(Direction.Rotation());
            }

            // 攻撃モーション再生
            if (AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(MyPawn))
            {
                EnemyChar->PlayAttackMontage();
            }
        } else
        {
            // 射程外ならプレイヤーを追いかける
            MoveToActor(TargetPlayer, AttackRange * 0.8f);
        }
    } else
    {
        // --- 巡回モード ---
        if (GetMoveStatus() != EPathFollowingStatus::Moving)
        {
            PatrolRandomLocation();
        }
    }
}

void AEnemyAIController::PatrolRandomLocation()
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys)
    {
        FNavLocation RandomPoint;
        if (NavSys->GetRandomReachablePointInRadius(MyPawn->GetActorLocation(), PatrolRadius, RandomPoint))
        {
            MoveToLocation(RandomPoint.Location, 50.0f);
        }
    }
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!TargetPlayer && Result.IsSuccess())
    {
        PatrolRandomLocation();
    }
}