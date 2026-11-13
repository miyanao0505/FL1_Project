// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FL1_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	// AIControllerから呼び出す攻撃関数
	void PlayAttackMontage();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// エディタ（BP_Enemy）から攻撃アニメーションを設定するための変数
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* AttackMontage;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
