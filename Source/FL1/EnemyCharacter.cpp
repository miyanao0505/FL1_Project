// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    // 移動方向にキャラクターの向きを自動で合わせる設定
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // 歩行速度の調整（必要に応じて変更）
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void AEnemyCharacter::PlayAttackMontage()
{
    if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        // すでに攻撃モーション再生中でなければ再生する
        if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
        {
            PlayAnimMontage(AttackMontage);
        }
    }
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}