#include <Arduino.h>
#include <BleKeyboard.h>

// ===================== 핀맵 =====================
static const int PIN_PREV  = 6;
static const int PIN_NEXT  = 5;

// ===================== BLE 설정 =====================
static const char* DEVICE_NAME = "PageTurner";
BleKeyboard bleKeyboard(DEVICE_NAME, "LimIkJu", 100);

// ===================== 디바운스 및 반복 설정 =====================
static const uint32_t DEBOUNCE_MS     = 30;
static const uint32_t REPEAT_DELAY_MS = 450;
static const uint32_t REPEAT_RATE_MS  = 120;

// ===================== 버튼 상태 관리 구조체 =====================
struct ButtonState {
  int pin;
  bool lastRaw;
  bool stable;
  uint32_t lastChangeMs;
  uint32_t pressedAtMs;
  uint32_t lastRepeatMs;
};

ButtonState btnPrev{PIN_PREV, true, true, 0, 0, 0};
ButtonState btnNext{PIN_NEXT, true, true, 0, 0, 0};

// ===================== 키 입력 송신 =====================
void sendKey(uint8_t key) {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.press(KEY_LEFT_GUI); 
    bleKeyboard.write(key);
    bleKeyboard.releaseAll();
    Serial.printf("Key Sent: %d\n", key);
  }
}

// ===================== 버튼 처리 로직 =====================
void handleButton(ButtonState& b, void (*onPress)(), bool repeat) {
  uint32_t now = millis();
  bool raw = digitalRead(b.pin);

  // 상태 변화 감지 (디바운싱 시작)
  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.lastChangeMs = now;
  }

  // 안정화된 상태 확인
  if ((now - b.lastChangeMs) >= DEBOUNCE_MS && b.stable != raw) {
    b.stable = raw;
    if (b.stable == LOW) { // 버튼 눌림 (PULLUP 기준)
      b.pressedAtMs = now;
      b.lastRepeatMs = now;
      onPress();
    } else {
      b.pressedAtMs = 0;
    }
  }

  // 연속 입력 처리 (페이지를 계속 넘길 때)
  if (repeat && b.pressedAtMs && b.stable == LOW) {
    if ((now - b.pressedAtMs) >= REPEAT_DELAY_MS && 
        (now - b.lastRepeatMs) >= REPEAT_RATE_MS) {
      b.lastRepeatMs = now;
      onPress();
    }
  }
}

// ===================== 초기 설정 =====================
void setup() {
  Serial.begin(115200);
  
  // 핀 모드 설정 (내부 풀업 저항 사용)
  pinMode(PIN_PREV, INPUT_PULLUP);
  pinMode(PIN_NEXT, INPUT_PULLUP);

  // 블루투스 키보드 시작
  bleKeyboard.begin();
  Serial.println("BLE Keyboard Ready!");
}

// ===================== 메인 루프 =====================
void loop() {
  // 1. 페이지 넘김 버튼 체크
  // handleButton(btnPrev, [](){ sendKey(KEY_LEFT_ARROW); }, true);
  // handleButton(btnNext, [](){ sendKey(KEY_RIGHT_ARROW); }, true);

  if(btnPrev.stable == LOW) {
    sendKey(KEY_LEFT_ARROW);
  }
  if(btnNext.stable == LOW) {
    sendKey(KEY_RIGHT_ARROW);
  }
  delay(5); // 시스템 안정성을 위한 짧은 휴식
}