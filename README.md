# Sensor Collector

ESP8266·DS18B20 온도 측정 예제와 Express/MySQL 센서 수집 서버를 담았습니다. LED 예제, 온도 측정 펌웨어, 웹 서버를 분리했습니다.

## 코드 구조

| 경로 | 역할 |
|---|---|
| `firmware/led_example/` | Blink 클래스와 LED 동작 예제 |
| `firmware/temperature_monitor/` | 온도 측정 및 ThingSpeak/IFTTT 전송 예제 |
| `server/src/app.mjs` | 수집·조회·CSV 다운로드 라우트 |
| `server/src/database.mjs` | 주입된 MySQL pool을 사용하는 저장소 |
| `server/src/server.mjs` | 환경변수로 서버·DB 연결 |
| `server/test/app.test.mjs` | DB 대체 객체를 사용하는 HTTP 검증 |

## 실행

```bash
cd server
pnpm install
pnpm test
pnpm start
```

서버는 MYSQL_URL을 사용하고 HOST·PORT로 바인딩을 지정합니다. `sensors`와 `temperature` 테이블은 코드가 읽는 열에 맞춰 사용자가 준비해야 합니다. `/updateIOT`은 device·unit·type·value·seq, `/update`는 value 또는 field1을 받습니다. `/temperature`는 최근 온도, `/download`는 센서 CSV를 반환합니다. 이전 임시 test 테이블용 라우트는 포함하지 않습니다. CSV는 공유 임시 파일 없이 응답합니다. 펌웨어는 빌드 환경의 WIFI_SSID·WIFI_PASSWORD·SENSOR_API_KEY 매크로를 요구합니다. 온도 펌웨어는 ThingSpeak/IFTTT 예제이며 로컬 수집 서버와 바로 연결된 완성 장치라는 뜻은 아닙니다.

## Source and license

See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for project origins and third-party licenses.
