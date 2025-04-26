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
