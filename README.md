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
    

![image.png](attachment:ce39778b-0582-46cc-b924-5e99e00fdbbe:image.png)

- 실행 중 물결표 누르면 cmd 활성화
- Show Collision 하면 심플 콜리전이 뜸

→ 라인 트레이스할 때 더 결과가 좋음

![image.png](attachment:bb9ca7c4-cb90-4330-87bc-b7d74913b2ff:image.png)

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

![image.png](attachment:4b198b7a-fe8d-49ef-af05-c7f8ca9ca93e:image.png)

- 보이지 않지만 Hit판정이 될 수 있게 함.
- Collision Presets을 아래처럼 설정
    
    ![image.png](attachment:7f31be20-34a9-4fcc-bfc8-c3e9f944bfbf:image.png)
    
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
	)
```

![image.png](attachment:c1295e6f-5ff4-40b4-9267-6f63a0a277d0:image.png)

- `FMath::RInterpTo(시작 벡터, 목표 벡터, DeltaTime, 회전 속도)`
