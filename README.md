[데모 영상](https://youtu.be/EzXRV7_FCt8)

<h1 align="center">라즈베리파이를 사용한 블루투스 디지털 알람 시계</h1>

<p align="center">
  스마트폰으로 알람을 설정하여 디지털 알람 시계처럼 사용할 수 있습니다.
</p>

## Features
- 스마트폰으로 라즈베리파이에 연결하여 알람 설정(추가/삭제/변경) 가능
- 설정한 알람에 따라 알람음 출력
- 밝기 값에 따라 7segment display의 밝기를 2단계(주간, 야간)로 조절
![KakaoTalk_20221214_034814757](https://user-images.githubusercontent.com/45515388/207721891-c9db27e3-5d06-4981-8fd0-93ae5fe4c23e.gif)

## Used
- Raspberry Pi 4 model B
- 7segment display -> wiringPi의 softPwm 사용
- HC-06(bluetooth module) -> UART 통신 사용
- YL-40(조도 센서) -> I2C 통신, ADC 사용
- tact switch
- 수동 부저(Passive Buzzer) -> PWM 사용
- pthread를 통해 쓰레드와 mutex사용

## How to setup?
<p align="center">
  <img width="500" src="https://user-images.githubusercontent.com/45515388/207717568-91202446-e071-4760-9842-a6a4bc7c23af.png">
</p>
전체 시스템 구조는 위과 같습니다. 따라서 라즈베리파이와 모듈(센서)의 연결은 아래의 회로도와 같이 연결합니다.
<br/>
<p align="center">
  <img width="500" src="https://user-images.githubusercontent.com/45515388/207717779-aeecf506-eabb-4139-89f5-1f7933732023.png">
</p>
<br/>
스마트폰과 블루투스 모듈 연결은 Serial Bluetooth Terminal 앱 (https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) 을 통해 연결합니다.

## How to run?
```sh
cd {프로젝트 폴더}
make
sudo ./test
```

## How to use?
- 알람 추가: insert MMDDHHmm (insert 12121015)
- 알람 삭제: delete MMDDHHmm (delete 12121015)
- 알람 변경: update MMDDHHmm MMDDHHmm (update 12121015 12131020)
- 알람 확인: select
- 알람 끄기: tact switch 사용
<p align="center">
  <img width="200" src="https://user-images.githubusercontent.com/45515388/207724034-bb40f0c8-73fa-4690-9b39-d321ef4ac314.png">
</p>
