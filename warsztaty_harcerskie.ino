#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Preferences.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensorPin = 34; // Pin czujnika odległości
const int buttonPin = 15; // Pin przycisku
const int samplingTime = 100;

unsigned long previousMillis = 0;
float ogniskoLevel = 0.0;
int lastDistance = 0;
float tempo = 0.0;
unsigned long startTime = 0; // Czas rozpoczęcia gry
unsigned long finishTime = 0; // Czas zakończenia gry
unsigned long bestTime = 0; // Rekord gry (0 oznacza brak rekordu)

bool gameStarted = false;
bool ogniskoZapalone = false;

const int ledPins[] = {25, 26, 27, 14, 12, 13, 33, 32, 4, 2};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

Preferences preferences;

void setup() {
  Serial.begin(115200);

  // Inicjalizacja pamięci NVS
    preferences.begin("game-data", false); // Nazwa przestrzeni, false = odczyt/zapis
    bestTime = preferences.getUInt("bestTime", 0); // Domyślnie 0, jeśli brak rekordu
    if (bestTime == 0) {
      bestTime = 9999;
    }

    Serial.print("Odczytany rekord: ");
    Serial.println(bestTime);

  // Inicjalizacja wyświetlacza OLED
  if (!display.begin(OLED_ADDRESS, true)) {
    Serial.println("Błąd inicjalizacji OLED!");
    while (true);
  }

  display.clearDisplay();
  display.display();

  // Inicjalizacja LED-ów
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Inicjalizacja przycisku
  pinMode(buttonPin, INPUT_PULLUP);

  // Wyświetl ekran powitalny na start
  displayWelcomeScreen();
}

void loop() {
  handleButton(); // Obsługa przycisku

  if (!gameStarted) {
    return;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= samplingTime) {
    previousMillis = currentMillis;
    updateGameLogic();
  }
}

void handleButton() {
  static unsigned long buttonPressStart = 0; // Czas rozpoczęcia wciśnięcia
  static bool isButtonHeld = false;          // Flaga przytrzymania

  if (digitalRead(buttonPin) == LOW) {
    if (!isButtonHeld) {
      buttonPressStart = millis(); // Rejestracja czasu wciśnięcia
      isButtonHeld = true;
    }

    unsigned long pressDuration = millis() - buttonPressStart;

    if (pressDuration > 30000) { // Przytrzymanie > 30 sekund
      preferences.begin("game-data", false);
      preferences.remove("bestTime"); // Usunięcie rekordu
      preferences.end();
      Serial.println("Rekord został usunięty!");
      displayWelcomeScreen();
      isButtonHeld = false; // Reset flagi
    }
  } else {
    if (isButtonHeld) { // Przyciski puszczono
      unsigned long pressDuration = millis() - buttonPressStart;
      isButtonHeld = false;

      if (pressDuration < 30000) { // Krótkie wciśnięcie
        if (ogniskoZapalone) {
          // Reset gry
          ogniskoZapalone = false;
          gameStarted = false;
          ogniskoLevel = 0.0;
          startTime = 0;
          finishTime = 0;
          for (int i = 0; i < numLeds; i++) {
            digitalWrite(ledPins[i], LOW); // Wyłącz wszystkie LED
          }
          displayWelcomeScreen();
          Serial.println("Gra zresetowana!");
        } else {
          // Rozpocznij grę
          gameStarted = true;
          displayGameStatus();
          Serial.println("Gra rozpoczęta!");
        }
      }
    }
  }
}

void displayWelcomeScreen() {
  display.clearDisplay();
  // display.setFont(&Oswald_7pt7b);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5, 4);
  display.println("Wcisnij przycisk");
  display.setCursor(5, 14);
  display.println("aby rozpoczac gre.");

  display.setCursor(5, 45);
  display.print("Aktualny rekord: ");
  display.print(bestTime);
  display.print("s");
  display.display();
}

void displayGameStatus() {
  display.clearDisplay();

  // Wyświetlanie poziomu rozpalenia w prawym górnym rogu
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(100, 1);
  display.print((int)(ogniskoLevel * 100));
  display.println("%");

  // Rysowanie podstawy ogniska
  int baseWidth = 50; // Szerokość podstawy
  int baseHeight = 5; // Wysokość podstawy
  int baseX = (SCREEN_WIDTH - baseWidth) / 2; // Wyśrodkowanie podstawy
  int baseY = SCREEN_HEIGHT - baseHeight; // Pozycja podstawy na dole
  display.fillRect(baseX, baseY, baseWidth, baseHeight, SH110X_WHITE);

  // Rysowanie płomieni
  int numFlames = 5; // Liczba warstw płomieni
  int flameWidth = 30; // Szerokość płomienia
  int flameHeight = 5; // Wysokość każdego płomienia
  int flameX = (SCREEN_WIDTH - flameWidth) / 2; // Wyśrodkowanie płomienia
  int maxFlames = (int)(ogniskoLevel * numFlames); // Liczba zapalonych płomieni

  for (int i = 0; i < maxFlames; i++) {
    int flameY = baseY - (i + 1) * flameHeight; // Pozycja każdego płomienia
    display.fillRect(flameX, flameY, flameWidth, flameHeight, SH110X_WHITE);
  }

  display.display();
}

void displaySuccessScreen(bool isNewRecord) {
    display.clearDisplay();

    // Komunikat główny
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    if (isNewRecord) {
        display.setCursor(10, 10);
        display.println("Brawo!");
        display.setTextSize(1);
        display.setCursor(10, 40);
        display.print("Nowy rekord: ");
        display.print(bestTime);
        display.println(" s");
    } else {
        display.setCursor(10, 10);
        display.println("Brawo!");
        display.setTextSize(1);
        display.setCursor(10, 40);
        display.print("Czas: ");
        display.print((finishTime - startTime) / 1000); // Aktualny czas
        display.println(" s");

        display.setCursor(10, 50);
        display.print("Rekord: ");
        display.print(bestTime); // Aktualny rekord
        display.println(" s");
    }

    display.display();
}

void startGame() {
  gameStarted = true;
  Serial.println("Gra rozpoczęta!");
  display.clearDisplay();
  display.display();
}

void resetGame() {
  ogniskoZapalone = false;
  gameStarted = false;
  ogniskoLevel = 0.0;
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  Serial.println("Gra zresetowana!");
  displayWelcomeScreen();
}

void updateGameLogic() {
  int currentDistance = analogRead(sensorPin);
  int distanceChange = abs(currentDistance - lastDistance);

  tempo = constrain((float)distanceChange / 500.0, 0.0, 1.0);

  float easeFactor = 3.0 - (ogniskoLevel * 2.0);
  easeFactor = constrain(easeFactor, 1.0, 3.0);

  if (tempo > 0.1) {
    ogniskoLevel += tempo / (150.0 / easeFactor);
  } else {
    ogniskoLevel -= (1.0 - tempo) / (50.0 / easeFactor);
  }

  ogniskoLevel = constrain(ogniskoLevel, 0.0, 1.0);

  if (ogniskoLevel >= 1.0 && !ogniskoZapalone) {
    ogniskoZapalone = true;
    gameStarted = false;
    finishTime = millis(); // Zarejestruj czas zakończenia

    unsigned long elapsedTime = (finishTime - startTime) / 1000; // Czas w sekundach

    // Sprawdź, czy to nowy rekord
    if (bestTime == 0 || elapsedTime < bestTime) {
        bestTime = elapsedTime; // Aktualizuj rekord
        preferences.putUInt("bestTime", bestTime); // Zapisz rekord do pamięci
        Serial.println("Nowy rekord zapisany!");
        displaySuccessScreen(true); // Nowy rekord
    } else {
        displaySuccessScreen(false); // Brak nowego rekordu
    }

    Serial.println("Ognisko w pełni rozpalone!");
    successSignal();
    return;
}

  if (gameStarted && startTime == 0) {
    startTime = millis(); // Zarejestruj czas rozpoczęcia
  }

  int litLeds = ogniskoZapalone ? numLeds : round(ogniskoLevel * numLeds);
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], i < litLeds ? HIGH : LOW);
  }

  displayGameStatus();

  lastDistance = currentDistance;
}

void successSignal() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(300);
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(300);
  }
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}