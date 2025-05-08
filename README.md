### 113강. 프로젝트 개요

- Input Mapping 설정
  
![image](https://github.com/user-attachments/assets/091f29e9-9eb5-467a-97d9-abf95a730f9a)

### 114강. Pawn 클래스 생성

### tool → C++ 생성

- `None` :
    - 단순하게 생성할 수 있지만 몇가지 추가적인 기능 필요
    
    → 객체 인스턴스를 월드에 배치 못함.
    
- `Actor` :
    - 월드에 배치 가능
    - 시각적인 표현도 가능
- `Pawn` :
    - Actor의 모든 것을 상속받음
    - 플레이어에 의해 컨트롤 됨
    - 움직임 입력을 처리가능
- `Character` :
    - Pawn의 모든 속성을 상속받음
    - Character Movement Component라는 특수한 것이 있음
        
        → 캐릭터의 다양한 움직임 모드와 추가 능력을 부여(비행, 수영), 2족보행 캐릭터에 적합

      ### 115강. Components

## `USceneComponent`란?

> **위치, 회전, 스케일 등의 변형 정보를 가지는 컴포넌트의 기본 베이스 클래스.**
> 
- 위치나 회전 의미
- **Attach(부착)** 시스템을 지원 → 부모-자식 구조로 연결 가능
- 모든 컴포넌트들은 보통 이 `USceneComponent`에서 파생됨

### 📦 파생 클래스 예시

`UCapsuleComponent`

- 충돌 처리하기 위한 클래스(그게 전부)

`StaticMeshComponent`

- 시각적인 표현할 수 있게 해줌(랜더링)

### Pawn 클래스와 컴포넌트 구조

- `APawn` 또는 `ACharacter` 클래스는 이미 Root Component를 가짐
    - 타입은 USceneComponent
    - 이 루트에 모든 다른 컴포넌트를 **Attach**해서 계층 구조를 만든다.
    
![image](https://github.com/user-attachments/assets/4fbe3e46-0729-482b-a2a4-31b3b9683f4b)

    

다른 컴포넌트를 생성해서 씬 컴포넌트에 연결

→ BashMesh(StaticMeshComponent)를 생성하면, Root Component에 연결가능

![image1](https://github.com/user-attachments/assets/4d6acd45-9927-46ab-9f37-d0adb3c4cbcf)


## 🎯 예시: 내 캐릭터 구조

```
MyPawn (APawn)
├── CapsuleComponent (Root)
├── StaticMeshComponent (BodyMesh)
├── CameraComponent (FollowCamera)
└── GrabberComponent (커스텀 기능 모듈)
```

- **시작점은 Capsule**이지만
- 실제 **보이는 것은 StaticMesh**
- **기능 추가는 SceneComponent 기반 커스텀 컴포넌트**로 함

### 116강. 전방 선언

## 포인터 사용에 대한 원칙

> Unreal C++에서는 정수, 실수처럼 단순한 값이 아닌 복잡한 객체는 대부분 포인터로 처리한다.
> 

```cpp
UStaticMeshComponent* Mesh;      // GOOD
FVector Position;                // 값 타입이므로 포인터 아님 (GOOD)
```

### UPROPERTY

- 언리얼 엔진 리플렉션 시스템에 참여할 수 있게 함
- 언리얼 엔진 리플렉션 시스템
    - 변수를 BP에 노출.
    - 가비지 컬렉션에 참여

### UCapsuleComponent를 사용하려면?

> 기존 UCapsuleComponent를 사용하려면 헤더를 추가해야됨.
> 

→ 사용하지 않는 모든 것을 불러온다.(비효율)

- 전방선언으로 앞에 class를 붙임.

```cpp
private:
	UPROPERTY()
	class UCapsuleComponent* CapsuleComp;
```

- 이렇게 하면 .cpp파일에서는 해더를 불러야됨.

### .h에 헤더 파일을 포함한다면?

- 다른 파일이 이 헤더를 포함할 때마다 **불필요한 클래스들까지 전부 메모리에 로딩**
- 여러 헤더 간 중첩되면 **컴파일 시간이 기하급수적으로 늘어남**
- 유지보수 시 **의존성 파악 어려움**

### 117강. 캡슐 생성

- `CreateDefaultSubobject<>()`
    - <type> (name)
    
    ```
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
    RootComponent = CapsuleComp;
    ```
    
    → UStaticMeshComponent의 포인터가 저장
    
- CapsuleComponent 생성
- RootComponent에 저장
    
    <img width="583" alt="Image" src="https://github.com/user-attachments/assets/8106d26a-3c3c-44c2-ac9c-0de758d3b30e" />


### 118강. Static Mesh Components

- 구조

CapsuleComp

|

BaseMesh

|

TurretMesh

|

ProjectileSpawnPoint

```
CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
RootComponent = CapsuleComp;
	
BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
BaseMesh->SetupAttachment(CapsuleComp);

TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
TurretMesh->SetupAttachment(BaseMesh);

ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
ProjectileSpawnPoint->SetupAttachment(TurretMesh);
```

- SetupAttachment : 해당 컴포넌트에 붙인다.

### 119강. Deriving BP Classes

- C++를 BP로 만들면 아래와 같이 나옴
    
   <img width="382" alt="Image" src="https://github.com/user-attachments/assets/b3533523-cb75-4464-9924-24f1d44d47c7" />
    
- BUT Details를 보면 아무것도 안보임
    
   <img width="423" alt="Image" src="https://github.com/user-attachments/assets/74ed07ba-d174-489b-94b0-4a9545ef7fb8" />
    
    아무것도 안보이는게 정상
    

### 120강. 인스턴스 vs 디폴트

- BasePawn.h에 다음과 같이 선언

```
UPROPERTY()
int32 VisibleAnywhereInt = 12;
```

- BP에서 Self에서 확인 가능
- UPROPERTY 옵션
    - VisibleAnywhere : 보기 가능(편집 x)
    
    ```
    UPROPERTY(VisibleAnywhere)
    int32 VisibleAnywhereInt = 12;
    ```
    
    - EditAnywhere : 편집 가능
    
    ```cpp
    UPROPERTY(EditAnywhere)
    int32 EditAnywhereInt = 12;
    ```
    
    - VisibleInstanceOnly :
    
    ```
    	UPROPERTY(VisibleInstanceOnly)
    	int32 VisibleInstanceOnlyInt = 32;
    ```
    
    <img width="383" alt="Image" src="https://github.com/user-attachments/assets/dd6bacdf-efa7-4e29-a386-a73ed2348c37" />
    
    - BP에서는 안보이지만 레벨에서 BP를 드래그 한 인스턴스에는 보임
        
     <img width="322" alt="Image" src="https://github.com/user-attachments/assets/86388817-1be1-4881-9b91-2ab7a1467ba0" />
        
- BP를 레벨에 드래그하여 생성되는 것이 인스턴스

### 121강. 노출된 변수 편집

- UPROPERTY 속성
    - VisibleDefaultsOnly : BP에서만 볼 수 있음
    
    ```
    UPROPERTY(VisibleDefaultsOnly)
    int32 VisibleDefaultOnlyInt = 42;
    ```
    
  <img width="330" alt="Image" src="https://github.com/user-attachments/assets/41e8b0ad-7fcf-4692-9d1f-aef40b672f0e" />
    
    - EditDefaultsOnly : BP에서만 편집
    - EditDefaultsOnly : 인스턴스에서만 편집
    
    ```cpp
    UPROPERTY(EditDefaultsOnly)
    int32 EditDefaultOnlyInt = 52;
    	
    UPROPERTY(EditDefaultsOnly)
    int32 EditInstanceOnlyInt = 62;
    ```
    
    - BlueprintReadWrite : BP EventGraph에서 사용 가능
    
    ```cpp
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 VisibleAnywhereInt = 12;
    ```
    
    <img width="214" alt="Image" src="https://github.com/user-attachments/assets/4e062ba6-34c5-426e-9a99-e5ad04e02ee7" />
    
    - BlueprintReadOnly : BP에서 get만 가능
    
    ```
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 EditAnywhereInt = 22;
    ```
    
   <img width="265" alt="Image" src="https://github.com/user-attachments/assets/155fa045-0c55-4490-b2be-14f02873e914" />
    
- 최종 테이블
    
    ![Image](https://github.com/user-attachments/assets/0d615bfd-35d7-4780-ad87-31e6453dba5c)
    

### 122강. 컴포넌트 노출

- `BlueprintReadWrite`과 `BlueprintReadOnly`는 private에서 사용 못함
- meta = (AllowPrivateAccess = "true")를 하면 BP에서 노출

```
UPROPERTY(BlueprintReadWrite,
meta = (AllowPrivateAccess = "true"))
int32 VisibleAnywhereInt = 12;
```

- Category = "super duper variables”
    - 카테고리 추가
    
    ```cpp
    UPROPERTY(VisibleAnywhere,Category = "super duper variables", BlueprintReadWrite)
    int32 VisibleAnywhereInt = 12;
    
    UPROPERTY(EditAnywhere,Category = "super duper variables", BlueprintReadOnly)
    int32 EditAnywhereInt = 22;
    ```
    
   ![Image](https://github.com/user-attachments/assets/f2efcac4-2b03-4d4d-828d-45caa942d479)
    

### 123강. 하위 C++ 생성

- 동일한 BasePawn에 2개를 생성함.
- 각각 다른 기능을 가지게 할것임
- 하위 C++ 생성
    
  ![Image](https://github.com/user-attachments/assets/3e5c90b1-bf37-46e5-bae8-c47547ad9e21)
    
- Tank 추가
    
    ```cpp
    
    #pragma once
    
    #include "CoreMinimal.h"
    #include "BasePawn.h"
    #include "Tank.generated.h"
    
    /**
     * 
     */
    UCLASS()
    class TOONTANKS_API ATank : public ABasePawn
    {
    	GENERATED_BODY()
    
    public:
    	ATank();
    private:
    	UPROPERTY(VisibleAnywhere, Category = "Components")
    	class USpringArmComponent* SpringArm;
    
    	UPROPERTY(VisibleAnywhere, Category = "Components")
    	class UCameraComponent* Camera;
    	
    };
    
    #include "Tank.h"
    #include "GameFramework/SpringArmComponent.h"
    #include "Camera/CameraComponent.h"
    
    ATank::ATank()
    {
        SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
        SpringArm->SetupAttachment(RootComponent);
    
        Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
        Camera->SetupAttachment(SpringArm);
    }
    
    ```
    
- 그 후 BP_PawnTank에서 Class Settings → Class Options : Tank로 설정
    - 이렇게 한 이유
        
        ## ✅ 이게 가능한 이유
        
        BP의 *Parent Class*는 언제든 **상속 구조상 자식 클래스(C++)로 교체 가능**해.
        
        `Tank`는 `BasePawn`을 상속한 자식 클래스니까, `BasePawn` 기반으로 만든 `BP_PawnTank`의 부모를 `Tank`로 바꾸는 건 논리적으로 **올바른 흐름**이야.
        
        ---
        
        ## ✅ 작동 방식 (중요)
        
        BP에서 Parent Class를 바꾸면:
        
        - 기존 BP는 여전히 유지됨 (`BP_PawnTank`)
        - 하지만 그 BP는 이제 **`Tank`의 모든 속성과 함수**에 접근 가능
        - `Tank`가 `BasePawn`을 상속하므로 기존 `BasePawn`의 로직도 유지됨
        - 단, C++에서 `Tank`에 새로 추가한 변수/함수는 BP에서 **재컴파일 이후** 접근 가능
        
        ---
        
        ## 🧠 언제 유용하냐면?
        
        - `BasePawn`을 여러 타입으로 확장하고 싶을 때 (예: `Tank`, `Plane`, `Turret` 등)
        - 공통 기능은 `BasePawn`에, 개별 로직은 `Tank` 등에서 처리
        - 그 후, 각 C++ 클래스에 대응하는 BP를 만들어서 **직접 수정 없이** 다양하게 확장 가능
          
- SpringArm : 시야가 가려지면 카메라 조정됨.

### 124강. Pawn에 빙의하기(Possess)

- 혼자 게임할 땐 0이 국룰
  
![Image](https://github.com/user-attachments/assets/5119c85c-2a27-4fe8-88bd-ec92dbe71bd7)

### 125강. 인풋 처리하기

- wasd를 누르면 move함수에 적용된다
    - w:1,0 , s:-1,0 등
- 매개 변수 안에서 전방 선언:
    - `virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;`
- BasePawn에서는 Input처리가 필요 없음.
    
    → Tanck에 virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 설정
    
- 축 바인딩을 할 때 도움됨.
    - `PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);`
    - `TEXT("MoveForward")` : Axis Mapping한 이름
    - `this` : 바인딩할 객체
    - `&ATank::Move` : 할당할 함수 주소
 
  - Local Space vs World Space
  - 기즈모의 차이를 보면 됨
    
    ![Image](https://github.com/user-attachments/assets/1e4c3cf7-1789-40e2-a843-5d0c363096a4)
    
    ![Image](https://github.com/user-attachments/assets/fceb4610-5a44-4806-acbe-d72c771c548d)
    
- 둘이 같거나 다를 수 있음
- 이상태에서 x방향으로 움직인다면, world에서는 월드 기준 전방, 로컬에서는 탱크의 전방으로 이동
- `AActor::AddActorLocalOffset`
    
    ```c++
	void AddActorLocalOffset  
    (  
        FVector DeltaLocation,  
        bool bSweep,  
        FHitResult * OutSweepHitResult,  
        ETeleportType Teleport  
    ) 
    ```
    
- BasePawn.cpp에서 추가
    
    ```c++
    FVector DeltaLocation(0.f,0.f,0.f);
    	DeltaLocation.X = 2.f;
    	AddActorLocalOffset(DeltaLocation);
    }
    ```
    
    - 이러면 tower랑 turret이 모두 자기의 X 방향으로 움직임
    
    → 둘다 BasePawn을 상속받으니까
    
- Simulate 실행하기(Alt + s) 멀리서 보기 가능
- Tank.cpp
```c++
 void ATank::Move(float Value)
{
    UE_LOG(LogTemp, Display, TEXT("%f"),Value);
    FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value;
	AddActorLocalOffset(DeltaLocation);
}
```
### 127강. Movement Speed

- 프레임의 속도에 따라 달라짐 → DeltaTime
- 처음 시작하면 fps이 낮아짐
    - 모든 BeginPlay과 월드를 호출하기에
- DeltaTime
    - Tick은 일정하게 호출되지 않음
        
        → 프레임마다 계산하기 때문
        
        → 속도 저하됨
        
    - CPU 사용량에 따라 달라짐
- 균일하게 수행됨.
- 일반 함수에서 DeltaTime 구하기
- `UGameplayStatics::GetWorldDeltaSeconds(World Context Objcet)`
    - 게임 개발에 도움되는 통계 기능
    - return DeltaTime
    - 우리가 어떤 월드에 있는지를 말해줘야됨.
        
        → this 주로 사용.
        

```jsx
 FVector DeltaLocation = FVector::ZeroVector;
 DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
 AddActorLocalOffset(DeltaLocation);
```

### 128강. 로컬 회전

- Sweeping : 특정한 한 프레임에서 움직일 때 두 오브젝트가 겹치는지를 확인
    
    → 겹치면 겹치기 전 프레임으로 움직여 겹치는것을 방지
    
    - `AddActorLocalOffset(DeltaLocation,true);`
    - 콜리전 기능 있어야됨.
        
        → BP_TankPawn Collision Presets 활성화
        
    - BlockAllDynamic 은 모든 오브젝트를 못지나감
- Pitch / Yaw / Roll
    - 좌우 회전: Yaw
    - 위아래 고개 숙이기 : Pitch
    - 몸을 기울이기 : Roll
- `AddActorLocalRotation`

```jsx
void ATank::Turn(float Value)
{
    FRotator DeltaRotator = FRotator::ZeroRotator;
    DeltaRotator.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotator);
}
```

### 129강. 캐스팅

- Pawn에는 보이지 않는 Controller가 있음

AController → AAiController → APlayerController

- PlayerControllerRef = GetController();
    
    → 컴파일 오류! 타입 안맞음
    
    → Cast를 한다
    
    - Cast<Type>(Pointer)
    
    `PlayerControllerRef = Cast<APlayerController>(GetController());`
    
- 이제 APlayerController에 있는 GetHitResultUnderCursor 사용가능

### 130강. 마우스 커서 적용

- 마우스 커서를 따라서 디버그 구체 적용
- 시작부터 끝점까지 선을 그어 중간에 물체가 있다면 Hit 판정
    
    → Line Trace, Trace Hit
    

## **FHitResult**

- **FHitResult** : 충돌(히트) 정보를 저장하는 **구조체**.
- 주로 **트레이스(선, 구, 박스 등)** 또는 **충돌** 관련 함수에서 사용.
- 내부에 담기는 데이터 예:
    - `ImpactPoint` : 충돌한 지점의 위치.
    - `Normal` : 충돌면의 노멀(방향 벡터).
    - `Distance` : 충돌까지의 거리.
    - `Actor` : 충돌한 액터.
    - `Component` : 충돌한 컴포넌트.
    - `bBlockingHit` : 실제로 막히는 히트가 발생했는지.

### PlayerControllerRef→GetHitResultUnderCursor(...)

```cpp
PlayerControllerRef->GetHitResultUnderCursor(
ECollisionChannel::ECC_Visibility,
false,
HitResult);
```

- **현재 플레이어의 마우스 커서 아래에 있는 물체와의 충돌(히트) 정보**를 얻는다.

### 매개변수:

- **ECollisionChannel::ECC_Visibility**
    
    → 어떤 충돌 채널로 검사할지 지정.
    
    → 기본적으로 **카메라에서 볼 수 있는 물체**만 검사.
    → 만약 무기용 충돌, AI용 충돌 등 검사하려면 다른 채널을 써야 함.
    
- **bool bTraceComplex**

→**복잡한 충돌(Collision Complex)**

검사 여부.

→true: 복잡한 콜리전(메시 폴리곤 그대로 검사).

→false: 간단한 콜리전(프리미티브, 예: 캡슐, 박스, 구

- **&HitResult**

→**out parameter**

→ 커서 아래에서 충돌된 정보를 이 구조체에 저장.

### DrawDebugSphere(...)

```cpp
DrawDebugSphere(
            GetWorld(),
            HitResult.ImpactPoint,
            25.f,
            12,
            FColor::Blue,
            false,
            -1.f);
```

### 함수 역할:

- **디버깅용으로 월드에 구(스피어)를 그림**.
- 주로 **충돌 위치나 검사 결과를 시각적으로 확인**할 때 사용.

### 매개변수:

- **GetWorld()**
    
    → 현재 오브젝트(액터, 컴포넌트 등)가 속한 월드.
    
- **HitResult.ImpactPoint**
    
    → 구를 그릴 위치.
    
    → 충돌 지점.
    
- **25.f**
    
    → 구의 반지름.
    
- **12**
    
    → 구의 세그먼트 개수. 많을수록 부드러움.
    
- **FColor::Blue**
    
    → 구의 색상.
    
- **false**
    
    → **영구적인가?** → `false`면 일정 시간 후 사라짐.
    
- **-1.f**
    
    → 유지 시간. `-1.f`이면 다음 씬 업데이트까지 유지됨.
    

![Image](https://github.com/user-attachments/assets/e1434030-184d-47ff-8bf3-d509504f2e13)

- 실행 중 물결표 누르면 cmd 활성화
- Show Collision 하면 심플 콜리전이 뜸

→ 라인 트레이스할 때 더 결과가 좋음

![Image](https://github.com/user-attachments/assets/d887686b-3085-4a3f-9d42-5161722148fc)

- 이거는 Mesh에서 직접 확인한 Complex 콜리젼 → 계산 더 많음

### 131강. Rotating The Turret

- 시작과 끝사이의 벡터 구하기
    - ToTarget = end - start;
- 이렇게 회전을 하면 탱크 앞쪽이 바닥을 본다.
    
    → Yaw값만 바꾸고 싶음.
    
- BasePawn.cpp

```cpp
void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LooAtRotation = FRotator(0.f,ToTarget.Rotation().Yaw,0.f);
	TurretMesh->SetWorldRotation(LooAtRotation);
}
```

- Tank.cpp

```cpp
 RotateTurret(HitResult.ImpactPoint);
```

- 문제점: 마우스가 임팩트 지점을 못찾으면(먼 곳) 엉뚱한 곳으로 감
- Blocking Volume이 있음

	![Image](https://github.com/user-attachments/assets/569bead2-0f8e-4454-a439-230b19138393)

- 보이지 않지만 Hit판정이 될 수 있게 함.
- Collision Presets을 아래처럼 설정
    
	![Image](https://github.com/user-attachments/assets/3eff2e90-c1b6-4050-b458-04366aacb969)
    
- HitResult에서 채널을 ECC_Visibility로 했기 때문

BUT 탱크 중간에 마우스를 하면 이상해짐.

→ 보간 적용

```cpp
FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
FRotator LooAtRotation = FRotator(0.f,ToTarget.Rotation().Yaw,0.f);
TurretMesh->SetWorldRotation(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(),
		LooAtRotation,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		5.f)
	);
```

![Image](https://github.com/user-attachments/assets/0acc7ec4-d569-4d46-b29c-65b3d53fb000)

- `FMath::RInterpTo(시작 벡터, 목표 벡터, DeltaTime, 회전 속도)`

  ### 132강. Tower 클래스

- Tower에서 Tank접근할 때
- 헤더에 class ATank* Tank 선언.
- cpp파일 BeginPlay
- `APawn* GetPlayerPawn(const UObject* WorldContextObject, int32 PlayerIndex);`
    1. Actor에 대한 정보를 넣어야됨.(ATower)
    2. 설정한 Player 인덱스를 넣어야됨
        
        ![image.png](attachment:d0a1ae1d-35ee-4704-9bdb-990f915ed272:image.png)
        
    
    - This랑 GetOwner()랑 Actor 클래스에서는 상관없지만 Component 클래스에서는 this는 컴포넌트 자기자신, GetOwner()는 컴포넌트가 붙어있는 Actor임
- Tower.cpp
    
    ```cpp
     if(Tank)
        {
            // tank까지의 거리를 구함
            float Distance = FVector::Dist(GetActorLocation(),Tank->GetActorLocation());
            // 사정거리 안에 있는지 확인
            if(Distance <= FireRange)
            {
                // 있다면 Tank를 향해 회전ㅁㅁ
                RotateTurret(Tank->GetActorLocation());
            }
    ```
    

### 133강. Fire

- 이미 Fire는 Input Mapping에 할당함
- Turret과 Tower 모두 발사기능 필요 → BasePawn에 구현.
- 이동에서는 BindAxis를 했지만 공격은 BindAction사용
    
    ```cpp
     PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
    ```
    
- BasePawn.h에 Fire 선언
- 클릭시 확인을 위해 디버그사용
    
    ```cpp
    void ABasePawn::Fire()
    {
    	FVector ppL = ProjectileSpawnPoint->GetComponentLocation();
    	DrawDebugSphere(
    		GetWorld(),
    		ppL,
    		25.f,
    		12,
    		FColor::Red,
    		false,
    		3.f);
    }
    ```
    
    - ProjectileSpawnPoint는 Actor가 아니라 Component라서 GetComponentLocation을 사용

### 134강. Timer

- FTimerManager로 타이머 설정
    - SetTimer()

```cpp
bool SetTimer(
		FTimerHandle& InOutHandle,
		UObject* InObj,
		FTimerDelegate InDelegate,
		float InRate,
		bool InbLooping);
```

1. F
2. 할당할 오브젝트
3. 수행할 함수
4. 시간
5. 반복할거면 true
- FTimerHandle의 TimerHandle을 사용하여 SetTimer를 설정
- Tower.h에 변수 추가
    
    ```cpp
    FTimerHandle FireRateTimerHandle;
    float FireRate = 2.f;
    void CheckFireCondition();
    ```
    
- Tower.cpp BeginPlay에 추가
    
    ```
    GetWorldTimerManager().SetTimer(
            FireRateTimerHandle,
            this,
            & ATower::CheckFireCondition,
            FireRate,
            true);
    ```
    
    ```cpp
    void ATower::CheckFireCondition()
    {
        if(InFireRange())
        {
            Fire();
        }
    }
    ```
    
    - `InFireRange()`는 범위 안에 있는지를 확인하는 함수

### 135강. ProjectileClass

- Actor Class는 월드에 배치하거나 월드에 스폰 가능

```cpp
UPROPERTY(EditDefaultsOnly, Category = "Combat")
UStaticMeshComponent* ProjectileMesh;
```

```cpp
AProjectile::AProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

}
```

- false로 하는 이유 : Tick마다의 호출이 필요 없어서
- Class 기반 BP 생성 후 mesh 입히기

### 136강. 발사체 생성

- 기본적인 Projectile에서 메쉬를 추가 위해 BP로 만듦.
- TSubclassOf<Type>
    - 특정 클래스 또는 그 클래스를 상속받은 Blueprint Class들을 참조하는 변수.
    - `AProjectile`이라는 클래스 또는 그 자식 클래스를 참조하겠다는 의미.
- UWorld::SpawnActor<Type>(매개변수)
    
    <> : AProjectile
    
    () : UClass(BP Class), Location, Rotation
    
- 결론적으로는 다음과 같이 사용됨
- `SpawnActor<AProjectile>(ProjectileClass, Loc, Rot);`

![image.png](attachment:ee20404d-4206-42fc-b6e1-2a87d2baf8bd:image.png)

### ✅ 왜 TSubclassOf를 쓰지?

만약 이렇게 하면:

```cpp
UClass* ProjectileClass;
```

**문제점**:

- ProjectileClass에 *모든 종류의 클래스*가 할당될 수 있음.
- **컴파일러가 타입 검사**를 못 해서 런타임 오류 발생 가능.

`TSubclassOf<AProjectile>`

- 반드시 **AProjectile** 또는 **그 자식 클래스만** 넣을 수 있게 **타입 안전성** 보장.
- 특히 **Blueprint에서** 어떤 **Projectile BP**를 선택할 때 실수를 방지.
- UClass를 저장하고 BP_Tank에서 BP_Projectile과 동일하게 사용하려고

## ✅ UClass란?

- **언리얼 엔진 런타임에서 클래스를 나타내는 객체**.
- 실제 C++ 클래스가 메모리에 어떤 형태로 배치되어 있는지, 어떤 변수와 함수가 있는지 등을 설명하는 **메타 데이터** 역할.
- **클래스에 대한 정보**가 들어 있는 객체


### 137강. 발사체 이동 컴포넌트

- 발사체 이동 구현 방식
    1. Location과 Rotation을 설정(Tick)
    2. Impuse적용
    3. Movement Component사용
        1. Projectile로 만들기
- UProjectileMovementComponent(Projectile.cpp)
    
    ```cpp
    ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
    	
    ProjectileComponent->MaxSpeed = 1300.f;
    ProjectileComponent->InitialSpeed = 1300.f; // 최대 속도 넘으면 안됨.
    ```
    
- Tower도 쏴야하니까 이미지처럼 BP_Projectile 넣는다.
    
    ![image.png](attachment:37a5ee3f-a215-4812-af9e-f2c5e97f456a:image.png)
    
    - Projectile Class는 BasePawn에 있기에 상속받아 사용하는 Tower에서도 사용가능

### 138강. Hit 이벤트

- UFUNCTION을 쓰면 라이브코딩 하지 마셈
- Projectile은 StaticMehs로 구성
    
    USceneComponent
    
    |
    
    UPrimitiveComponent
    
    |
    
    StaticMeshComponent
    
- UPrimitiveComponent는 Hit Events 발생 가능
    
    → 두 오브젝트가 충돌하면 Hit Component에 접근
    

### **OnHitComponent**

- Unity에서의 OnCollisionEnter
- Solid한거랑 충돌 시 발생
- Type은 FComponentHitSignature
- OnHit함수를 만들어서 바인딩함

```cpp
ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("%s"),*HitComp->GetName());
	UE_LOG(LogTemp, Display, TEXT("%s"),*OtherActor->GetName());
	UE_LOG(LogTemp, Display, TEXT("%s"),*OtherComp->GetName());
}
```

- `OnComponentHit` → **FComponentHitSignature** 타입의 **멀티캐스트 델리게이트**.
- `AddDynamic` → **이벤트(델리게이트)에 함수(메서드)를 바인딩**.

## ✅ **멀티캐스트 델리게이트란?**

`OnComponentHit`은 **Multicast Delegate**.

| 특징 | 설명 |
| --- | --- |
| **Multicast** | 여러 함수를 동시에 등록 가능 |
| **Delegate** | 특정 이벤트 발생 시 미리 등록한 함수들을 호출 |
- ProjectileMesh->OnComponentHit
- 여기에는 여러 개의 함수를 .AddDynamic()으로 계속 추가 가능.
→ 충돌 발생 시 등록된 모든 함수 호출.

## ✅ **AddDynamic** 함수 설명

- AddDynamic은 **런타임에 동적으로** 함수를 바인딩.
- 즉, 게임 실행 중 바인딩되고, **블루프린트에도 노출 가능**.
- 참고
    
    **반면**:
    
    - `AddStatic` → 정적 함수 바인딩 (static 메서드만 가능)
    - `AddLambda` → 람다 함수 바인딩

```cpp
ProjectileMesh->OnComponentHit.AddDynamic(
    this,                      // 바인딩 대상 (AProjectile 인스턴스)
    &AProjectile::OnHit        // 호출할 함수
);
```

| 파라미터 | 설명 |
| --- | --- |
| `this` | 이 객체(AProjectile 클래스 인스턴스) |
| `&AProjectile::OnHit` | 이 객체의 `OnHit` 함수를 바인딩 |

**조건**:

- 함수 시그니처(함수의 모양이나 형태)가 델리게이트(호출할 함수)와 정확히 일치해야 함.
- 함수는 UFUNCTION이여야됨.
- 함수의 시그니처는 정해짐

```cpp
void OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
);
```

### 139강. Health Component

UActorComponent

|

USceneComponent

- UActorComponent
    - No transform
    - No attachment
- USceneComponent
    - has transform
    - can attach
- UActorComponent
- `UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )`
    - BP에서 직접 이 컴포넌트를 추가할 수 있게 함

## ✅ 1️⃣ 델리게이트/이벤트에 바인딩할 함수 → **시그니처가 정해짐**

```cpp
Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
```

위 코드에서처럼 **언리얼의 내장 델리게이트(예: OnTakeAnyDamage)** 에 바인딩하는 경우:

- **반드시 해당 델리게이트가 요구하는 시그니처**를 가져야 해.
- OnTakeAnyDamage 델리게이트의 시그니처:

```cpp
void DamageTaken(
    AActor* DamagedActor,
    float Damage,
    const UDamageType* DamageType,
    AController* Instigator,
    AActor* DamageCauser
);
```

그래서 이 형태를 **반드시** 따라야 함.

**커스텀 불가**. (인자 바꾸면 AddDynamic에서 에러 발생)

### 140강. 데미지 적용

## 함수 시그니처

```cpp
static float ApplyDamage(
    AActor* DamagedActor,
    float BaseDamage,
    AController* EventInstigator,
    AActor* DamageCauser,
    TSubclassOf<class UDamageType> DamageTypeClass
);
```

---

## ✅ 매개변수 설명

| 매개변수 | 설명 | 타입 |
| --- | --- | --- |
| DamagedActor | **데미지를 받을 액터** (피해자) | AActor* |
| BaseDamage | **기본 데미지** 값 | float |
| EventInstigator | 공격을 지시한 **컨트롤러** (플레이어, AI 등) | AController* |
| DamageCauser | **실제로 데미지를 준 객체** (예: 총알, 폭탄 등) | AActor* |
| DamageTypeClass | **데미지 종류** 클래스. (폭발, 불, 독 등 다양한 데미지 타입 가능) | TSubclassOf<UDamageType> |
- BasePawn::Fire()

```cpp
auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,ppL,Rotator);
	Projectile->SetOwner(this);
```

- Projectile::OnHit()

```cpp
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr) return;
	
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		Destroy();
	}
}
```

### 141강. Game Mode Class

- 시작 Pawn 정하기
- 죽음 관리
- 게임 시작 끝 관리

AGameModeBase

|

AGameMode

- BP_ToonTanksGameMode 만들기
- 아래 BP_PawnTank 설정

![image.png](attachment:7a533d86-ee82-4b7a-828b-beddb8c8ab26:image.png)

- project settings에서 gamemode 설정
- player start가 없다면→

![image.png](attachment:362c82ba-919a-4b8a-9494-05a09ab8056f:image.png)

### 142강. Pawn 죽음 다루기

- VSCode에서 f2누르고 바꾸면 모두 수정됨

✅ **3. GetGameMode (게임 모드 캐스팅)**

- `ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));`
- `UGameplayStatics::GetGameMode(this)` → **현재 맵에 설정된 GameMode 객체 반환**.

✅ **4. HealthComponent.cpp → 데미지 처리**

- 사망

```cpp
void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)
{
	...
	Health -= Damage;
	if(Health <= 0.f && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}
}
```

- 어떤 액터가 죽었는지 GameMode에게 알릴 수 있음.
- GameMode가 적이 죽었는지, 플레이어가 죽었는지 구분해서 다른 행동 가능.

✅ **5. ToonTanksGameMode.cpp → 사망 처리 로직**

```cpp
void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if(Tank->GetTankPlayerController())
        {
            Tank->DisableInput(Tank->GetTankPlayerController());
            Tank->GetTankPlayerController()->bShowMouseCursor = false;
        }
    }
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
    }
}
```

- **플레이어(Tank)가 죽으면**:
    - `HandleDestruction()` 호출 → 숨기고 Tick 중지.
    - 조종 비활성화.
- **타워가 죽으면**:
    - `Destroy()` 호출 → **액터 삭제**.

**핵심**: **다형성(Polymorphism)** 이용해서 Actor 하나로 받고, 타입에 따라 다르게 처리

✅ **6. Tank.cpp와 Tower.cpp → HandleDestruction**

```cpp
void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}
```

- 부모(BasePawn)의 파괴 이펙트 호출 (`Super::HandleDestruction()`).
- `SetActorHiddenInGame(true)` → **액터를 렌더링에서 제외** (카메라는 여전히 플레이어에게 따라붙음).
- `SetActorTickEnabled(false)` → **Tick (Update 함수 호출) 중지**. 성능 최적화됨.

```cpp
void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}
```

- 부모 파괴 이펙트 호출.
- Destroy() → **씬에서 완전히 삭제**.

## ✅ **추가 설명: 왜 이런 구조냐?**

언리얼 C++의 특징을 활용:

- **GameMode** → 전체 게임 상태 관리 (누가 죽었는지 판정).
- **Component (HealthComponent)** → 데미지, 죽음 조건 처리.
- **Pawn (Tank, Tower)** → 파괴 시 구체적인 행동 수행.

**이렇게 분리하면**:

- HealthComponent 는 어떤 액터에 붙여도 재사용 가능.
- GameMode는 모든 액터의 죽음을 통제.
- Pawn은 자신만의 사망 연출 처리.

### 143강. 사용자 지정 플레이어 컨트롤러

- ToonTanksPlayerController 만들기
    - 사용자 조작 담당 클래스 제작
- PlayerController를 상속

```cpp
void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabeld)
{
    if(bPlayerEnabeld)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnabeld;
    
}

```

- ToonTanksGameMode.cpp 수정

```cpp
void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0)); 
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));
}
void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
            
            // Tank->DisableInput(Tank->GetTankPlayerController());
            // Tank->GetTankPlayerController()->bShowMouseCursor = false;
        }
    }
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
    }
}

```

## **블루프린트 BP_ToonTanksPlayerController 설정**

언리얼 에디터에서:

1. **BP_ToonTanksPlayerController** 블루프린트 생성.
2. **클래스 설정** → 부모 클래스로 **ToonTanksPlayerController** 지정.
    
    ![image.png](attachment:2afbc703-940c-4d11-be17-ea4d08a347b5:image.png)
    
- 그 후 BP_ToonTanksPlayerController에서 아래 설정.
    
    ![image.png](attachment:67453052-9e41-4b0d-84c5-ce8fe73c3d1d:image.png)
    

### 144강. 게임 시작

- 시간 제한
- FTimerMananger → SetTimer()에도 여러개가 있다.

```cpp
void AToonTanksGameMode::HandleGameStart()
{
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0)); 
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));

    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false);
    }
}
```

## ✅ **HandleGameStart 함수 해설**

```cpp
Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
```

- 플레이어의 **Pawn** 가져와서 **ATank** 타입으로 캐스팅 → Tank 변수에 저장.

```cpp
ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));
```

- 플레이어의 **Controller** 가져와서 **AToonTanksPlayerController** 로 캐스팅 → ToonTanksPlayerController 변수에 저장.

---

## ✅ **플레이어 조작 비활성화**

```cpp
ToonTanksPlayerController->SetPlayerEnabledState(false);
```

- 게임 시작할 때 **플레이어 조작을 비활성화**.
- 예: 게임 시작 연출, 준비 시간 등.

---

## ✅ **Timer 설정: 나중에 조작 활성화**

```cpp
FTimerHandle PlayerEnableTimerHandle;
FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
    ToonTanksPlayerController,
    &AToonTanksPlayerController::SetPlayerEnabledState,
    true);
```

- 타이머를 제어하는 **핸들** (나중에 타이머 취소하거나 상태 확인 가능).

**핵심**:

- **TimerDelegate** 에 "나중에 실행할 함수" 와 "매개변수" 를 지정.
- `ToonTanksPlayerController` → SetPlayerEnabledState(true) 실행하도록 예약.

```cpp
GetWorldTimerManager().SetTimer(
    PlayerEnableTimerHandle,
    PlayerEnableTimerDelegate,
    StartDelay,
    false);
```

**설명**:

- **StartDelay** 초 뒤에 PlayerEnableTimerDelegate 실행.
- 즉, **StartDelay** 초 뒤에 플레이어 조작이 다시 활성화됨.

---

## ✅ **왜 Delegate 를 썼나?**

`SetTimer` 은 일반 함수 포인터로는 **매개변수가 없는 함수**만 호출 가능

하지만 `SetPlayerEnabledState(true)` 처럼 **매개변수(true)** 를 넘겨야 하는 경우에는

**FTimerDelegate** 을 사용해서 매개변수까지 같이 넘김

```cpp
FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
    ToonTanksPlayerController,
    &AToonTanksPlayerController::SetPlayerEnabledState,
    true);  // ← 매개변수
```

**결론**:

**Timer + Delegate** 조합 → 일정 시간 후 **매개변수를 포함한 함수 호출** 가능.

---

## ✅ **전체 흐름**

1. 게임 시작하면 Tank, PlayerController 가져오기.
2. 플레이어 입력 끄기.
3. 일정 시간 후 Delegate 로 입력 켜기.

### 145강. 게임 시작 위젯

## ✅ **BP Implementable Event란?**

- ToonTanksGameMode.cpp

```cpp
UFUNCTION(BlueprintImplementableEvent)
void StartGame();
```

- **C++에서는 함수 선언만** 하고, 정의(구현)는 **블루프린트에서 함**.
- **가상 함수처럼 동작**하지만 BP에서만 구현 가능.

즉:

- **C++ 코드 → Blueprint 이벤트 발생 → BP 안에서 원하는 동작 실행**

예시:

C++에서 StartGame() 호출 → BP_ToonTanksGameMode 안에 StartGame 이벤트 그래프가 반응해서 UI 표시, 카메라 연출 등 실행.

- Widget_BP 생성
    
    ![image.png](attachment:5c7b66f3-134c-4e9c-b7c9-8e42b6a8aa39:image.png)
    
- BP_ToonTanksGameMode에 추가.
    
    ![image.png](attachment:ee66d2a9-d456-467a-846d-4448b01e593d:image.png)
    
    class 추가 잘 하기
    

### 146강. 카운트다운 타이머

- TimerHandle에서 3초를 했지만,WBP에서 한 것은 전혀 다름
    
    ![image.png](attachment:05e16eb2-8e44-472d-b737-c4c9a266d275:image.png)
    

![image.png](attachment:7ae54246-7985-454b-a50b-08aad40eeac3:image.png)

- ceil은 소수점에 근사한 값으로 반올림함.

### 147강. 카운트다운 표시하기

- 위젯에 카운트다운 표시
- Text에서 IsVariable 체크
- print 대신 Display로 설정
    
    ![image.png](attachment:d9501ce2-797d-4cc7-af77-9f3a6c9c1bfe:image.png)
    
- Remove from parent로 bp 삭제

### 148강. 승패 조건

- 월드 Tower개수 확인
    
    →GetAllActorsOfClass
    

### ToonTanksGameMode.h

- 코드 추가

```cpp
UFUNCTION(BlueprintImplementableEvent)
void GameOver(bool bWongame);
```

- BP로 확인

![image.png](attachment:4a1dc306-539b-48c0-b507-da5e0d645aff:image.png)

```cpp
int32 TargetTowers = 0;
int32 GetTargetTowerCount();
```

### ToonTanksGameMode.cpp

```cpp
int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;  // AActor 포인터 배열을 만듦
    UGameplayStatics::GetAllActorsOfClass(
        this,                   // WorldContextObject → 지금 GameMode 객체 사용
        ATower::StaticClass(),  // 검색할 클래스 → ATower
        Towers                  // 결과를 이 배열에 저장
    );
    return Towers.Num();  // 찾은 타워 개수를 반환
}

```

- **UGameplayStatics::GetAllActorsOfClass**
    - **특정 클래스**(여기선 `ATower`)를 상속한 액터들을 **씬(월드)에 존재하는 전부** 검색해서 배열에 넣음.
    - **this**: WorldContextObject. 여기서 `this`는 **GameMode**이니까 현재 월드 정보를 알아낼 수 있음.
- **Towers.Num()**: 배열에 들어간 타워 개수 반환.

## ✅ WorldContextObject 왜 this 쓸까?

`this` → **AToonTanksGameMode** 객체인데

**GameMode는 항상 월드에 존재**하고, GetWorld()를 내부적으로 사용할 수 있어서 **월드 컨텍스트**로 적합.

## 추가 Tip

만약 **ATower** 클래스 말고 **BP_Tower**만 찾고 싶으면 `ATower::StaticClass()` 대신 **BP_Tower_C::StaticClass()** 써야 함.

(다만, 보통 BP도 C++ 클래스의 인스턴스로 인식돼서 StaticClass()로 충분한 경우가 많음.)

### 149강. GameOver HUD

- WBP_EndGame 만들기
- BP_ToonTanksGameMode 수정
    - Select Node로 정할 수 있음.
    
    ![image.png](attachment:6b53e7fb-aab6-48f5-8ec6-b6b53341f21c:image.png)
    

## 🎯 핵심 포인트: **Select 노드**

- 조건문을 만들지 않고 **값에 따라 자동 선택**하게 만드는 노드.
- **if문을 블루프린트에서 간단하게 대체**하는 역할.
- Index가 true/false일 때 각기 다른 값을 반환.

### 150강. 타격 아티클

## 🔥 **SpawnEmitterAtLocation**

**파티클 이펙트를 특정 위치에 생성**하는 함수.

```cpp
UGameplayStatics::SpawnEmitterAtLocation(
    UObject* WorldContextObject,
    UParticleSystem* EmitterTemplate,
    FVector Location,
    FRotator Rotation
);
```

### 매개변수 설명:

- **WorldContextObject** → 보통 `this` 사용 (**현재 액터**가 속한 월드를 알아내는 역할).
- **EmitterTemplate** → 사용할 파티클 (예: 폭발, 불꽃).
- **Location** → 이펙트를 생성할 위치.
- **Rotation** → 이펙트의 방향.
- Projectile.h 수정
    
    ```cpp
    UPROPERTY(EditAnywhere, Category = "Combat")
    class UParticleSystem* HitParticles;
    ```
    
- BP_Projectile 추가
    
    ![image.png](attachment:ae463937-7b10-4c86-a579-6da2a81e6dc6:image.png)
    
- Projectile::OnHit()

```cpp
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if(HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}
		
	}
	Destroy();
}
```

### 151강. 연기 궤적

## 🔎 **UParticleSystem vs UParticleSystemComponent**

| 클래스 | 타입 | 역할 |
| --- | --- | --- |
| **UParticleSystem** | *UObject(데이터)* | 파티클의 *설계 데이터*. 어떤 모양으로 어떻게 움직일지 정의된 **에셋** |
| **UParticleSystemComponent** | *UActorComponent(씬 컴포넌트)* | 실제 씬 안에서 **파티클을 재생**하고 제어하는 **컴포넌트** |
- UParticleSystemComponent
    - 컴포넌트
    - CreateDefaultSubobject로 만듦.
    - Root에 연결
    
    ```cpp
    	UPROPERTY(VisibleAnywhere, Category = "Combat")
    	class UParticleSystemComponent* ParticleSystemComponent;
    ```
    
    ```cpp
    	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    	ParticleSystemComponent->SetupAttachment(RootComponent);
    ```
    
- 이 컴포넌트는 항상 액터에 따라붙고, 따로 스폰할 필요 없이 자동으로 씬에서 작동.

## 📝 정리

| 방식 | 특징 | 사용 예 |
| --- | --- | --- |
| **UParticleSystem** | 데이터만 존재 | 충돌할 때마다 스폰 |
| **UParticleSystemComponent** | 액터에 항상 붙음 | 미리 부착해서 지속적인 효과 (예: 엔진 불꽃) |

---

## ✅ 결론

**UParticleSystem → 단발 효과 (Spawn으로 뿌리는 이펙트)**

**UParticleSystemComponent → 지속 효과 (항상 붙는 이펙트)**

### 152강. 죽음 파티클

- 죽음은 Turret과 Tower에 다 있음
    
    → BasePawn에 생성
    
- 기본적으로는 UParticleSystem과 동일

```cpp
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathEffect;
```

```cpp
void ABasePawn::HandleDestruction()
{
	//시각 음악 효과
	if(DeathEffect)
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathEffect, GetActorLocation(),GetActorRotation());
}
```

### 153강. 사운드

- 어느 한 줄에 전방선언을 하면 또 class를 안해줘도 됨.
- PlaySoundAtLocation로 구현
- Projectile.h
    
    ```cpp
    UPROPERTY(EditAnywhere, Category = "Combat")
    	class USoundBase* LaunchSound;
    
    	UPROPERTY(EditAnywhere, Category = "Combat")
    	USoundBase* HitSound;
    ```
    
- Projectile.cpp
    
    ```cpp
    if(LaunchSound)
    		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
    ```
    
- BasePawn.h
    
    ```cpp
    	UPROPERTY(EditAnywhere, Category = "Combat")
    	class USoundBase* DeathSound;
    ```
    
- BasePawn.cpp
    
    ```cpp
    UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
    ```
    

### 154강. 카메라 쉐이크(흔들림)

- Hit || 폰이 죽을 때.
- BP_CameraShake(Legacy)생성

![image.png](attachment:8e8310be-4bc1-4a8e-98c2-fc88a8ead6e3:image.png)

- 0.25초동안 유지되며, Shake의 시작과 끝의 블렌드 지속시간 의미

![image.png](attachment:a0790556-4bcd-46a5-af81-0f15f40d4374:image.png)

- Rot : **Yaw, Pitch, Roll** 기준으로 회전하는 흔들림
- Loc : **X,Y,Z 축**으로 위치 흔들림
- FOV : **카메라 시야각** 흔들림

### 1️⃣ Amplitude

- **진폭**. 흔들림 크기.
- 높을수록 큰 흔들림.

### 2️⃣ Frequency

- **주파수**. 1초당 몇 번 흔들릴지.
- 높을수록 빨리 흔들림.

### 3️⃣ Initial Offset

- **Random**이면 파형의 시작점이 매번 달라서 같은 카메라 흔들림이라도 연속 재생 시 반복 느낌이 덜함.

### 4️⃣ Waveform

- **Sine Wave** → 부드럽게 왕복.
- 다른 Waveform도 있음 (예: Perlin Noise → 랜덤성).
- BP_DeathCameraShake(Legacy)생성

![image.png](attachment:7ffa9b4e-1ebd-4cb3-9182-4a3646b71c76:image.png)

- UCameraShakeBase

```cpp
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
```

**TSubclassOf<UCameraShakeBase>** → *카메라 쉐이크*의 **클래스**를 담는 변수.

- **클래스**지 **인스턴스** 아님.
- 블루프린트에서 만든 BP_HitCameraShake나 BP_DeathCameraShake를 지정해줄 수 있음.

```cpp
GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
```

- **GetFirstPlayerController()**

→ **첫 번째 플레이어의 컨트롤러** (일반적으로 싱글플레이어에서는 본인의 컨트롤러).

- **ClientStartCameraShake(클래스)**

→ 지정한 **Camera Shake 클래스를 인스턴스화** 해서 카메라 흔들림 효과 시작.

## ✅ 추가: 왜 *클래스*를 넘길까?

- **UCameraShakeBase**는 **UObject**라서 `SpawnActor`로 인스턴스를 만들 수 없음.
- 그래서 **TSubclassOf**를 사용해 **클래스**만 넘기고, 엔진 내부에서 자동으로 인스턴스 생성 & 실행함.

### 155강. 마무리

### 카메라 움직임을 더 자연스럽게

- BP_PawnTank에서 SpringArm 설정

![image.png](attachment:e478007c-8478-4e30-881a-7d8886af884e:image.png)

- **Camera Lag** 활성화 → 플레이어 움직임에 *관성* 느낌 추가.
- **Lag Speed** → 카메라가 목표 위치에 얼마나 빠르게 따라갈지 결정 (**감도** 같은 역할).

### Turret이 Player 죽어도 발사되는 문제

- Tank.h
    - bool bAlive 추가
    - HandleDestruction()에서 false로 설정.
    - Tower::CheckFireCondition()
        
        ```cpp
        void ATower::CheckFireCondition()
        {
            if(Tank == nullptr)
                return;
        
            if(InFireRange() && Tank->bAlive)
            {
                Fire();
            }
        }
        ```
        

### 언리얼에서는 auto를 권장하지 않음

- 처음만 auto로 하고 나중에 수정하는 방법

### Package하기

![image.png](attachment:473a537a-4e5b-4eb4-a317-97725605d4e0:image.png)
