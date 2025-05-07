// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(InFireRange())
    {
        // 있다면 Tank를 향해 회전ㅁㅁ
        RotateTurret(Tank->GetActorLocation());
    }
   
    
    
}
void ATower::BeginPlay()
{
    Super::BeginPlay();
    //Tank는 Pawn의 하위 클래스임. 따라서 형변환 필요요
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle,
        this,
        &ATower::CheckFireCondition,
        FireRate,
        true
    );
}
void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}
void ATower::CheckFireCondition()
{
    if(Tank == nullptr)
        return;

    if(InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}
bool ATower::InFireRange()
{
    if(Tank)
    {
        // tank까지의 거리를 구함
        float Distance = FVector::Dist(GetActorLocation(),Tank->GetActorLocation());
        // 사정거리 안에 있는지 확인
        if(Distance <= FireRange)
        {
            return true;
       
        }
    }
    return false;
}