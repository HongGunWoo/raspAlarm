[데모 영상](https://youtu.be/EzXRV7_FCt8)

<h1 align="center">라즈베리파이를 사용한 블루투스 디지털 알람 시계</h1>

<p align="center">
  스마트폰으로 알람을 설정하여 디지털 알람 시계처럼 사용할 수 있습니다.
</p>

## Features
- 스마트폰으로 라즈베리파이에 연결하여 알람 설정(추가/삭제/변경) 가능
- 밝기 값에 따라 7segment display의 밝기를 2단계(주간, 야간)로 조절
![image](https://user-images.githubusercontent.com/45515388/207721139-8b242224-7095-4fdb-ae9e-ac45fb337eba.gif)

## 

## How to setup?
<p align="center">
  <img width="500" src="https://user-images.githubusercontent.com/45515388/207717568-91202446-e071-4760-9842-a6a4bc7c23af.png">
</p>
전체 시스템 구조는 위과 같습니다. 따라서 라즈베리파이와 모듈(센서)와의 연결은 아래의 회로도와 같이 연결합니다.
<br/>
<p align="center">
  <img width="500" src="https://user-images.githubusercontent.com/45515388/207717779-aeecf506-eabb-4139-89f5-1f7933732023.png">
</p>
<br/>
스마트폰과의 연결은 Serial Bluetooth Terminal (https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) 를 통해 연결합니다.

## How to run?
```sh
make
sudo ./test
```

## How to use?



