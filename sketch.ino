#include <LiquidCrystal.h>
#include <DHT.h>

// ─── Pinos LCD ───────────────────────────────────────────────
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ─── DHT22 ───────────────────────────────────────────────────
#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ─── LEDs por categoria ──────────────────────────────────────
#define LED_VOLCANO   A0   // Vermelho
#define LED_STORM     A1   // Azul
#define LED_FIRE      A2   // Amarelo
#define LED_QUAKE     A3   // Verde
#define LED_FLOOD     7    // Laranja (simulado com amarelo+vermelho)
#define LED_RGB_R     8
#define LED_RGB_G     9
#define LED_RGB_B     10

// ─── Buzzer e Botões ─────────────────────────────────────────
#define BUZZER        13
#define BTN_NEXT      A4
#define BTN_PREV      A5

// ─── Categorias de desastre ──────────────────────────────────
#define CAT_VOLCANO 0
#define CAT_STORM   1
#define CAT_FIRE    2
#define CAT_QUAKE   3
#define CAT_FLOOD   4

// ─── Estrutura de evento ─────────────────────────────────────
struct Event {
  const char* name;
  const char* region;
  uint8_t category;
  uint8_t risk; // 1=Baixo 2=Medio 3=Alto
};

// ─── Dados simulados (NASA EONET + Starlink temp) ────────────
const Event events[] = {
  {"Tufao Mawar",    "Asia-Pacifico",  CAT_STORM,   3},
  {"Incendio CA",    "America Norte",  CAT_FIRE,    2},
  {"Vulcao Etna",    "Europa-Italia",  CAT_VOLCANO, 2},
  {"Sismo M6.4",     "America Sul",    CAT_QUAKE,   2},
  {"Enchente Nilo",  "Africa-Egito",   CAT_FLOOD,   3},
  {"Tufao Haikui",   "China-Fujian",   CAT_STORM,   3},
  {"Incendio Pantanal","Brasil-MT",    CAT_FIRE,    3},
  {"Vulcao Fuego",   "Guatemala",      CAT_VOLCANO, 3},
  {"Sismo M5.8",     "Japao-Honshu",   CAT_QUAKE,   1},
  {"Enchente Kerala","India-Sul",      CAT_FLOOD,   2},
};

const uint8_t NUM_EVENTS = sizeof(events) / sizeof(events[0]);

// ─── Estado ──────────────────────────────────────────────────
int8_t currentEvent = 0;
bool showTemp = false;
unsigned long lastBuzz = 0;
unsigned long lastScroll = 0;
unsigned long lastTempRead = 0;
bool btnNextLast = HIGH;
bool btnPrevLast = HIGH;
uint8_t scrollPos = 0;

// ─── Setup ───────────────────────────────────────────────────
void setup() {
  lcd.begin(16, 2);
  dht.begin();

  pinMode(LED_VOLCANO, OUTPUT);
  pinMode(LED_STORM,   OUTPUT);
  pinMode(LED_FIRE,    OUTPUT);
  pinMode(LED_QUAKE,   OUTPUT);
  pinMode(LED_FLOOD,   OUTPUT);
  pinMode(LED_RGB_R,   OUTPUT);
  pinMode(LED_RGB_G,   OUTPUT);
  pinMode(LED_RGB_B,   OUTPUT);
  pinMode(BUZZER,      OUTPUT);
  pinMode(BTN_NEXT,    INPUT_PULLUP);
  pinMode(BTN_PREV,    INPUT_PULLUP);

  // Animação de boot
  bootAnimation();
  displayEvent(currentEvent);
}

// ─── Loop ────────────────────────────────────────────────────
void loop() {
  handleButtons();
  handleBuzzer();
  handleScroll();

  // Alterna leitura de temperatura a cada 10s
  if (millis() - lastTempRead > 10000) {
    lastTempRead = millis();
    showTemp = !showTemp;
    if (showTemp) displayTemperature();
    else displayEvent(currentEvent);
  }
}

// ─── Animação de boot ────────────────────────────────────────
void bootAnimation() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("** NASA MONITOR **");
  lcd.setCursor(0, 1);
  lcd.print(" Iniciando...   ");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starlink Online ");
  lcd.setCursor(0, 1);
  lcd.print("EONET API: OK   ");
  delay(1500);

  // Pisca todos os LEDs
  setAllLeds(HIGH);
  tone(BUZZER, 1000, 100);
  delay(300);
  setAllLeds(LOW);
  delay(200);
  setAllLeds(HIGH);
  tone(BUZZER, 1200, 100);
  delay(300);
  setAllLeds(LOW);
  delay(500);
}

// ─── Exibe evento no LCD ─────────────────────────────────────
void displayEvent(int8_t idx) {
  const Event& e = events[idx];
  lcd.clear();
  scrollPos = 0;

  // Linha 1: nome do evento
  lcd.setCursor(0, 0);
  lcd.print(e.name);

  // Linha 2: região + risco
  lcd.setCursor(0, 1);
  lcd.print(e.region);
  lcd.setCursor(13, 1);
  switch (e.risk) {
    case 1: lcd.print("LOW"); break;
    case 2: lcd.print("MED"); break;
    case 3: lcd.print("ALT"); break;
  }

  updateLeds(e);
  updateRGB(e.risk);
}

// ─── Exibe temperatura (DHT22) ───────────────────────────────
void displayTemperature() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  if (!isnan(t)) {
    lcd.print(t, 1);
    lcd.print((char)223); // símbolo °
    lcd.print("C");
  } else {
    lcd.print("--.-C");
  }

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  if (!isnan(h)) {
    lcd.print(h, 1);
    lcd.print("%");
  } else {
    lcd.print("--.-%");
  }

  // Alerta de temperatura alta
  if (!isnan(t) && t > 35.0) {
    tone(BUZZER, 800, 200);
    setRGB(255, 0, 0);
  }
}

// ─── Atualiza LEDs por categoria ─────────────────────────────
void updateLeds(const Event& e) {
  digitalWrite(LED_VOLCANO, e.category == CAT_VOLCANO ? HIGH : LOW);
  digitalWrite(LED_STORM,   e.category == CAT_STORM   ? HIGH : LOW);
  digitalWrite(LED_FIRE,    e.category == CAT_FIRE    ? HIGH : LOW);
  digitalWrite(LED_QUAKE,   e.category == CAT_QUAKE   ? HIGH : LOW);
  digitalWrite(LED_FLOOD,   e.category == CAT_FLOOD   ? HIGH : LOW);
}

// ─── RGB por nível de risco ───────────────────────────────────
void updateRGB(uint8_t risk) {
  switch (risk) {
    case 1: setRGB(0, 255, 0);   break; // Verde
    case 2: setRGB(255, 165, 0); break; // Laranja
    case 3: setRGB(255, 0, 0);   break; // Vermelho
  }
}

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(LED_RGB_R, r);
  analogWrite(LED_RGB_G, g);
  analogWrite(LED_RGB_B, b);
}

// ─── Buzzer por risco ────────────────────────────────────────
void handleBuzzer() {
  if (showTemp) return;
  const Event& e = events[currentEvent];
  unsigned long interval;
  switch (e.risk) {
    case 1: interval = 3000; break;
    case 2: interval = 1500; break;
    case 3: interval = 500;  break;
    default: interval = 3000;
  }
  if (millis() - lastBuzz > interval) {
    lastBuzz = millis();
    tone(BUZZER, 1000 + (e.risk * 200), 80);
  }
}

// ─── Scroll no LCD se nome for longo ─────────────────────────
void handleScroll() {
  if (showTemp) return;
  const Event& e = events[currentEvent];
  String name = String(e.name);
  if (name.length() <= 16) return;

  if (millis() - lastScroll > 400) {
    lastScroll = millis();
    lcd.setCursor(0, 0);
    String view = name.substring(scrollPos, scrollPos + 16);
    lcd.print(view);
    scrollPos++;
    if (scrollPos > name.length() - 16) scrollPos = 0;
  }
}

// ─── Botões de navegação ─────────────────────────────────────
void handleButtons() {
  bool btnNext = digitalRead(BTN_NEXT);
  bool btnPrev = digitalRead(BTN_PREV);

  if (btnNext == LOW && btnNextLast == HIGH) {
    currentEvent = (currentEvent + 1) % NUM_EVENTS;
    showTemp = false;
    displayEvent(currentEvent);
    delay(200);
  }

  if (btnPrev == LOW && btnPrevLast == HIGH) {
    currentEvent = (currentEvent - 1 + NUM_EVENTS) % NUM_EVENTS;
    showTemp = false;
    displayEvent(currentEvent);
    delay(200);
  }

  btnNextLast = btnNext;
  btnPrevLast = btnPrev;
}

// ─── Utilitários ─────────────────────────────────────────────
void setAllLeds(uint8_t state) {
  digitalWrite(LED_VOLCANO, state);
  digitalWrite(LED_STORM,   state);
  digitalWrite(LED_FIRE,    state);
  digitalWrite(LED_QUAKE,   state);
  digitalWrite(LED_FLOOD,   state);
  digitalWrite(LED_RGB_R,   state);
  digitalWrite(LED_RGB_G,   state);
  digitalWrite(LED_RGB_B,   state);
}
