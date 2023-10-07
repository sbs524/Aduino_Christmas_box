#include <Arduino.h>
#include "LedControl.h"     // 도트 매트릭스 라이브러리
#include <SoftwareSerial.h> // 소프트웨어 Serial 통신 라이브러리 사용
#include <Mecha_VoiceRecognition.h> //음성인식 라이브러리
#include <DFPlayer_Mini_Mp3.h>

// 모듈과의 통신을 위한 SoftwareSerial 객체 생성.
// 2번핀에 모듈의 Tx, 3번핀에 Rx가 연결됨.
SoftwareSerial mySerial(2, 3);

// voice 객체 생성
// 이때 모듈의 UART 통신을 위해 앞서 생성한 uart_voice를 함께 전달
Mecha_VoiceRecognition voice(&mySerial);

// Din 핀을 12번, ClK핀을 11번 CS핀을 10번에 연결
// 매트릭스는 1개를 사용 선언
LedControl lc=LedControl(7,6,5,1);

int now=0;
int special=0;
int voice_recogn=0;
byte data[4][8]={
  //별
  {
    B10000100,
    B11101100,
    B01111100,
    B00111111,
    B00111111,
    B01111100,
    B11101100,
    B10000100
  },
  //산타 웃는 얼굴
  {
    B00000100,
    B01100010,
    B10100100,
    B10100000,
    B10100000,
    B10100100,
    B01100010,
    B00000100
  },
  //루돌프(모자)
  {
    B11100000,
    B10110000,
    B10011100,
    B10000110,
    B10000011,
    B10001101,
    B10111010,
    B11100000

  },
  //음표
  {
    B00000000,
    B01100000,
    B01111111,
    B00000101,
    B00000101,
    B01100101,
    B01111111,
    B00000000
  }
};


void setup() {

  // 디버그용 하드웨어 시리얼 통신 초기화
  Serial.begin(9600);
  mp3_set_serial(Serial);
  delay(1);
  mp3_set_volume(30);

  lc.shutdown(0,false); // 0~3번까지 매트릭스 절전모드 해제
  lc.setIntensity(0,10); // 매트릭스의 밝기 선언 0~15의 수
  lc.clearDisplay(0); // 매트릭스 led를 초기화

  // voice 객체 설정 및 초기화
  voice.setDebugOn(&Serial);  // 디버깅 ON, 디버깅을 위한 통신 객체 전달
  voice.setGroup(VOICE_GROUP_3);  // 인식에 사용할 그룹을 1로 설정
  voice.init();                   // voice 객체 초기화

  voice.setMode(VOICE_MODE_RECOGNITION);  // 모듈을 인식 모드로 설정

  randomSeed(analogRead(0));
}

void loop() {
  // 모듈의 응답을 response 변수에 저장.
  // VOICE_InstructionTypeDef 자료형은 열거형으로, 아래 상태를 가짐.
  // VOICE_INSTRUCTION_1: 선택된 그룹의 1번 명령 인식됨
  // VOICE_INSTRUCTION_2: 선택된 그룹의 2번 명령 인식됨
  // VOICE_INSTRUCTION_3: 선택된 그룹의 3번 명령 인식됨
  // VOICE_INSTRUCTION_4: 선택된 그룹의 4번 명령 인식됨
  // VOICE_INSTRUCTION_5: 선택된 그룹의 5번 명령 인식됨
  // VOICE_INSTRUCTION_NOTHING: 아무 내용도 인식되지 않음
  // VOICE_INSTRUCTION_ERROR: 예외 발생

  VOICE_InstructionTypeDef response = voice.getInstruction();
  switch (response) {
    case VOICE_INSTRUCTION_5: // 5번 명령 인식됨. 현재 재생 중인 곡을 2배로 재생
      Serial.println("wow");
      for(int j=0; j<8; j++)
      {
        lc.setRow(0,j,data[3][j]); // 음표
      }
      mp3_play(now+4);
      break;
    case VOICE_INSTRUCTION_1: // 1번 명령 인식됨. 종료
      Serial.println("goodnight");
      for(int j=0; j<8; j++)
      {
        lc.setRow(0,j,B00000000); // 끄기
      }
      mp3_stop(); 
      break;
    case VOICE_INSTRUCTION_2: // 2번 명령 인식됨. Wham! - Last Christmas 재생
      Serial.println("christmas");

      special=random(0, 3);
      if(special==2)
      {
        now=4;
        mp3_play(now);
        for(int j=0;j<8;j++)
        {
          lc.setRow(0, j, data[3][j]);
        }
        break;
      }
      for(int j=0; j<8; j++)
      {
        lc.setRow(0,j,data[0][j]); // 별
      }
      now=1;
      mp3_play(now);
      break;
    case VOICE_INSTRUCTION_3: // 3번 명령 인식됨. Ariana Grande - Santa tell me 재생
      Serial.println("santa");
      
      special=random(0, 3);
      if(special==2)
      {
        now=4;
        mp3_play(now);
        for(int j=0;j<8;j++)
        {
          lc.setRow(0, j, data[3][j]);
        }
        break;
      }
      for(int j=0; j<8; j++)
      {
        lc.setRow(0,j,data[1][j]); // 웃는 표정
      }
      now=2;
      mp3_play(now);
      break;
    case VOICE_INSTRUCTION_4: // 4번 명령 인식됨. 루돌프 사슴코 재생
      Serial.println("carol");

      special=random(0, 3);
      if(special==2)
      {
        now=4;
        mp3_play(now);
        for(int j=0;j<8;j++)
        {
          lc.setRow(0, j, data[3][j]);
        }
        break;
      }
      for(int j=0; j<8; j++)
      {
        lc.setRow(0,j,data[2][j]); // 모자
      }
      now=3;
      mp3_play(now);
      break;
    case VOICE_INSTRUCTION_ERROR: // 에러 발생
      Serial.println("error");
      break;
    case VOICE_INSTRUCTION_NOTHING: // 명령 인식되지 않음
      // no data recieved
      break;
  }
}
