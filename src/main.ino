#include <Arduino.h>
#include <BleKeyboard.h>
#include <NimBLEDevice.h> // 보안 설정을 위해 필수

// ===================== 핀맵 =====================
static const int PIN_PREV  = 6;
static const int PIN_NEXT  = 5;

// ===================== BLE 설정 =====================
// iPad가 새 기기로 인식하도록 이름을 살짝 변경하는 것을 추천합니다.
static const char* DEVICE_NAME = "PageTurner-v1.1"; 
BleKeyboard bleKeyboard(DEVICE_NAME, "LimIkJu", 100);

// ===================== 디바운스 및 반복 설정 =====================
static const uint32_t DEBOUNCE_MS     = 30;
static const uint32_t REPEAT_DELAY_MS = 150;
static const uint32_t REPEAT_RATE_MS  = 50;

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

// ===================== 보안 콜백 클래스 =====================
class MySecurityCallbacks : public NimBLESecurityCallbacks {
    // 1. 패스키 직접 입력 요청 (사용되지 않더라도 필수 구현)
    uint32_t onPassKeyRequest() {
        Serial.println("Passkey Request: 123456");
        return 123456; 
    }

    // 2. 패스키 알림 (iPad에 뜨는 숫자를 시리얼에 표시) - 필수 구현
    void onPassKeyNotify(uint32_t pass_key) {
        Serial.printf("The Passkey is: %06u\n", pass_key);
    }

    // 3. 보안 요청 허용 여부 - 필수 구현
    bool onSecurityRequest() {
        return true;
    }

    // 4. 숫자 비교 확인 (iPad 화면과 보드의 숫자가 맞는지 확인)
    bool onConfirmPIN(uint32_t pin) {
        Serial.printf("Confirm PIN: %06u\n", pin);
        return true; 
    }

    // 5. 인증 완료 시 호출 (매개변수 형식을 ble_gap_conn_desc*로 변경) - 필수 구현
    void onAuthenticationComplete(ble_gap_conn_desc* desc) {
        if (desc->sec_state.encrypted) {
            Serial.println("Authentication Success & Encrypted!");
        } else {
            Serial.println("Authentication Failed.");
        }
    }
};

// ===================== 키 입력 송신 =====================
void sendKey(uint8_t key) {
  if (bleKeyboard.isConnected()) {
    // CTRL 키 조합합니다.
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.write(key);

    delay(10); // 키가 제대로 인식되도록 약간의 딜레이를 추가합니다.
    bleKeyboard.releaseAll();
    Serial.printf("Key Sent: %d (with GUI)\n", key);
  }
}

// ===================== 버튼 처리 로직 =====================
void handleButton(ButtonState& b, void (*onPress)(), bool repeat) {
  uint32_t now = millis();
  bool raw = digitalRead(b.pin);

  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.lastChangeMs = now;
  }

  if ((now - b.lastChangeMs) >= DEBOUNCE_MS && b.stable != raw) {
    b.stable = raw;
    if (b.stable == LOW) {
      b.pressedAtMs = now;
      b.lastRepeatMs = now;
      onPress();
    } else {
      b.pressedAtMs = 0;
    }
  }

  if (repeat && b.pressedAtMs && b.stable == LOW) {
    if ((now - b.pressedAtMs) >= REPEAT_DELAY_MS && 
        (now - b.lastRepeatMs) >= REPEAT_RATE_MS) {
      b.lastRepeatMs = now;
      onPress();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); 

  pinMode(PIN_PREV, INPUT_PULLUP);
  pinMode(PIN_NEXT, INPUT_PULLUP);

  // --- 보안 설정 수정 (상수 이름 확인) ---
  NimBLEDevice::init(DEVICE_NAME); 
  
  // AUTH_REQ -> AUTHREQ 로 언더바 하나를 제거했습니다.
  NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_BOND | 
                                BLE_SM_PAIR_AUTHREQ_MITM | 
                                BLE_SM_PAIR_AUTHREQ_SC); 
                                
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_YESNO); 
  NimBLEDevice::setSecurityCallbacks(new MySecurityCallbacks());
  // ------------------------------------------

  bleKeyboard.begin();
  Serial.println("Waiting for Secure Pairing with Passkey...");
}

// ===================== 메인 루프 =====================
void loop() {
  handleButton(btnPrev, [](){ sendKey(KEY_LEFT_ARROW); }, false); // ←
  handleButton(btnNext, [](){ sendKey(KEY_RIGHT_ARROW); }, false); // →

  delay(50); 
}