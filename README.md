## MinecraftUE

- 마인크래프트 모작


## 커밋 로그

### Commits on Jul 28, 2022

- 필요없는 파일 삭제

### Commits on Jul 17, 2022

- Craft Menu 아이템 드래그 잘 안되는 오류 해결
 
### Commits on Jul 13, 2022

- 불필요 코드 제거

### Commits on Jul 12, 2022

- 멀티플레이어 HUD Craft_Menu 추가
- 멀티 플레이어 인벤토리 HUD 적용 & HUD_Ingame 에서 컨트롤러가 null 이 아닐 때만 진행되도록 처리

### Commits on Jul 5, 2022

- 서버측 플레이어 컨트롤 오류 수정 
 
### Commits on Jul 3, 2022

- 불필요 코드 삭제 
- 클라이언트 쪽 아이템 스폰 안되는 오류 수정 

 
### Commits on Jun 15, 2022

- 맵에 스폰된 블루 프린트 캐릭터 변경 사항을 블루프린트 에셋에도 적용
- 블럭 깨면 Wieldable Item 스폰

 
### Commits on Jun 14, 2022

- Add Break Block RPC


### Commits on June 4, 2022

- ~~블럭을 캐낼때 `destroy` 하는게 아니라, 비활성화 시킴~~
- 나중에 우클릭해서 배치할 때 위치 변경 후, 활성화시키는 방식으로 할 예정

### Commits on Feb 21, 2022

- `TMap` 으로 도구 제작법 저장
- `CraftCreator` 액터 컴포넌트 생성
    - 현재 저장된 크래프트 배열에 맞는 `Wieldable` 아이템이 있으면 반환

<img src="./readme_assets/01.png" width="600">

### Commits on Feb 20, 2022

- Craft Menu 로 Wieldable 생산 가능 (테스트용 코드 제작)

### Commits on Feb 19, 2022

- 캐릭터가 Craft Inventory 가지고 있게끔
    - Craft Menu HUD BP function
        - `SetActiveCraftMenuItem` : Craft 인벤토리 창 시각화
        - `On Drop` : 아이템이 Craft Slot 파트로 드래그 되었는지 확인
        - `PutInNearestCraft` : 아이템을 인벤토리에서 Craft 인벤토리로 이동
- craft 탭에 드래그한 슬롯 아이템 추가 가능
- Craft Menu Craft 슬롯 추가 & CraftMenu 접었다 폈다 가능하도록

### Commits on Feb 18, 2022

- E 키로 Craft Menu 진입
- craft menu 인벤토리 기능 추가
- 블럭 타입 추가 & 블럭 깨면 블록 아이템 드롭
- 인벤토리 및 크래프트 메뉴(크래프트 인벤토리) 모두 블루프린트 단에서 제작

### Commits on Feb 17, 2022

- 블록 깬 후에 블록 아이템 드롭
- 블럭 타입 추가 및 블럭 깨진 후에 블럭 아이템 드롭
- 곡괭이 모든 타입 추가 

### Commits on Feb 16, 2022

- Throw 기능 및 현재 Wield Item 으로 material type & tool type 지정
- 마우스 휠로 인벤토리에서 무기 선택
- 인벤토리 및 픽업 기능
- 픽업 기능 추가 
- 블럭 깨기

### Commits on Feb 15, 2022

- detect block
- 블럭 깨지는 텍스처 추가
- add Block mesh
