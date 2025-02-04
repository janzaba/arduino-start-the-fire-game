# 🔌 **Warsztaty Elektroniki: Pierwsze kroki w elektronice**

## 🎯 Cel zajęć
Nauczyć się podstaw elektroniki poprzez budowę prostych obwodów na płytce stykowej oraz zrozumieć działanie kluczowych elementów, takich jak dioda LED, rezystor, przycisk, kondensator i układ NE555.

---

## 📋 Lista elementów potrzebnych do wykonania ćwiczeń

### 🔩 Podstawowe komponenty:
- **Płytka stykowa** (breadboard) – 1 szt.
- **Przewody połączeniowe** (męskie-męskie lub męskie-żeńskie) – kilka szt.
- **Źródło zasilania**: bateria 9V z uchwytem lub moduł zasilający 5V.

### ⚡ Komponenty elektroniczne:
- **Dioda LED** (czerwona lub inna) – minimum 1 szt.
- **Rezystory**: 220Ω i 1kΩ – po 1 szt. każdego rodzaju.
- **Przycisk (tact switch)** – 1 szt.
- **Układ scalony NE555** (opcjonalnie moduł gotowy do użycia) – 1 szt.
- **Kondensator 10µF** (opcjonalnie do NE555) – 1 szt.

---

## 💡 Ćwiczenie 1: Zapal diodę LED

> 📚 **Teoria: Dioda LED i Rezystor**  
> **Dioda LED** to element emitujący światło, posiadający dwie nóżki:
> - **Anoda (+)** – podłączana do dodatniego bieguna zasilania.
> - **Katoda (-)** – podłączana do masy (GND).
>
> **Rezystor** ogranicza przepływ prądu, chroniąc diodę LED przed przepaleniem. Wartość (np. 220Ω) jest dobrana tak, aby prąd mieścił się w bezpiecznym zakresie. ⚡

![LED](./led.png)

### Schemat połączenia
Bateria (lub moduł zasilający) → rezystor (220Ω) → dioda LED → masa

### Instrukcja:
➡️ Wybierz rząd otworów na płytce stykowej.  
➡️ Podłącz anodę diody LED do jednego końca rezystora (220Ω).  
➡️ Połącz drugi koniec rezystora z dodatnim biegunem źródła zasilania.  
➡️ Podłącz katodę diody LED do masy.  
➡️ Podłącz źródło zasilania.

⚠️ *Jeśli LED nie świeci:*
- Sprawdź poprawność polaryzacji diody.
- Upewnij się, że wszystkie połączenia są solidne.

---

## 🖲️ Ćwiczenie 2: Włącz diodę przyciskiem

> 📚 **Teoria: Przycisk (Tact Switch)**  
> Przycisk działa jak przełącznik – po wciśnięciu zamyka obwód, umożliwiając przepływ prądu. Dzięki temu można sterować stanem diody LED, włączając ją tylko przy naciśnięciu. 👍

### Schemat połączenia
Bateria → rezystor (220Ω) → dioda LED → masa  
*(Przycisk umieszczony między dodatnim biegunem źródła zasilania a anodą diody LED)*

### Instrukcja:
➡️ Zmodyfikuj układ z Ćwiczenia 1, usuwając bezpośrednie połączenie między dodatnim biegunem zasilania a anodą diody.  
➡️ Wstaw przycisk pomiędzy źródło zasilania a anodę LED.  
➡️ Po naciśnięciu przycisku dioda LED powinna się zaświecić.

⚠️ *Jeśli LED nie świeci:*
- Sprawdź, czy przycisk działa poprawnie (przetestuj różne połączenia).
- Upewnij się, że przycisk i rezystor są umieszczone w odpowiednich miejscach.

---

## ⏱️ Ćwiczenie 3: Migająca dioda LED z NE555

> 📚 **Teoria: Układ NE555, Kondensator i Tryb Astabilny**  
> **Układ NE555** to uniwersalny timer, działający w trybach monostabilnym, astabilnym i bistabilnym. W tym ćwiczeniu wykorzystujemy tryb **astabilny**, w którym NE555 generuje ciągłe impulsy, powodując miganie diody LED.
>
> **Kluczowe elementy:**
> - **Kondensator (np. 10µF):** Ładuje się i rozładowuje, ustalając częstotliwość impulsów.
> - **Rezystory (np. 1kΩ):** Regulują czas ładowania/rozładowania kondensatora.
>
> **Działanie układu:**  
> 1️⃣ Kondensator ładuje się przez rezystory do osiągnięcia 2/3 napięcia zasilania.  
> 2️⃣ Po osiągnięciu tego progu, wewnętrzny komparator zmienia stan wyjścia (Pin 3), wyłączając diodę LED.  
> 3️⃣ Kondensator rozładowuje się aż do spadku poniżej 1/3 napięcia, co ponownie włącza LED.
>
> W efekcie powstaje cykliczny efekt migania, zależny od wartości użytych rezystorów i kondensatora. 🔄
> 
> ## Opis wyprowadzeń układu NE555
>
> Poniżej przedstawiono opis poszczególnych pinów NE555 wraz z wyjaśnieniem ich funkcji oraz wpływu na zachowanie całego układu:
>
> **Pin 1: GND (Ground)**
> - **Funkcja:** Połączenie z masą układu.
> - **Wpływ:** Ustala odniesienie napięciowe dla wszystkich sygnałów wewnętrznych NE555.
>
> **Pin 2: Trigger (Wyzwalanie)**
> - **Funkcja:** Wejście wyzwalające cykl czasowy.
> - **Wpływ:** Gdy napięcie na tym pinie spadnie poniżej 1/3 Vcc, wewnętrzny przerzutnik zostaje ustawiony, co inicjuje cykl czasowy – w trybie monostabilnym uruchamia impuls, a w trybie astabilnym rozpoczyna ładowanie kondensatora.
>
> **Pin 3: Output (Wyjście)**
> - **Funkcja:** Wyjście sygnału sterującego.
> - **Wpływ:** W zależności od stanu wewnętrznego przerzutnika, pin ten generuje stan wysoki lub niski, sterując zewnętrznym obciążeniem, np. diodą LED.
>
> **Pin 4: Reset**
> - **Funkcja:** Wejście resetujące układ.
> - **Wpływ:** Obniżenie napięcia na tym pinie resetuje przerzutnik, niezależnie od stanu Triggera czy Threshold, co powoduje natychmiastowe wyłączenie wyjścia (Pin 3). Zwykle podłączony bezpośrednio do Vcc, jeśli reset nie jest używany.
>
> **Pin 5: Control Voltage (Napięcie sterujące)**
> - **Funkcja:** Pozwala na modyfikację progów wyzwalania i resetowania.
> - **Wpływ:** Umożliwia zewnętrzną regulację wewnętrznych progów (1/3 i 2/3 Vcc) poprzez podłączenie kondensatora filtrującego do masy, co pomaga zredukować szumy i stabilizować działanie układu.
>
> **Pin 6: Threshold (Próg)**
> - **Funkcja:** Monitoruje napięcie kondensatora w obwodzie RC.
> - **Wpływ:** Gdy napięcie na tym pinie osiągnie 2/3 Vcc, wewnętrzny komparator resetuje przerzutnik, co powoduje przełączenie stanu wyjścia (Pin 3) na niski oraz rozpoczęcie rozładowania kondensatora.
>
> **Pin 7: Discharge (Rozładowanie)**
> - **Funkcja:** Służy do rozładowania kondensatora poprzez wewnętrzny tranzystor.
> - **Wpływ:** W trybie astabilnym pin ten umożliwia szybkie rozładowanie kondensatora, co ustala czas trwania cyklu oscylacji. W trybie monostabilnym pomaga w szybkim zakończeniu impulsu.
>
> **Pin 8: Vcc (Zasilanie)**
> - **Funkcja:** Podłączenie do dodatniego źródła zasilania.
> - **Wpływ:** Dostarcza napięcie (zwykle w zakresie 4,5–16V), od którego zależą wszystkie progi i działanie układu NE555.
>
> ---
>
> ### Jak wyprowadzenia wpływają na działanie układu?
>
> - **Sterowanie cyklem czasowym:**  
>   Piny **Trigger (2)** i **Threshold (6)** ustalają moment rozpoczęcia i zakończenia cyklu ładowania kondensatora. To od nich zależy, kiedy wyjście (Pin 3) zmieni stan, inicjując impuls w trybie monostabilnym lub oscylację w trybie astabilnym.
>
> - **Generowanie sygnału wyjściowego:**  
>   Pin **Output (3)** reaguje na zmiany wewnętrznego przerzutnika, co bezpośrednio steruje elementami zewnętrznymi, np. zapalaniem lub miganiem diody LED.
>
> - **Resetowanie układu:**  
>   Pin **Reset (4)** umożliwia natychmiastowe przerwanie cyklu czasowego, co może być wykorzystane do ręcznego sterowania lub synchronizacji układu z innymi elementami.
>
> - **Stabilizacja i regulacja:**  
>   Pin **Control Voltage (5)** pozwala na dostrojenie poziomów wyzwalania, co ma znaczenie przy precyzyjnej regulacji czasu impulsów, zwłaszcza w aplikacjach wymagających dużej stabilności.
>
> - **Ładowanie i rozładowanie kondensatora:**  
>   Pin **Discharge (7)** umożliwia szybkie rozładowanie kondensatora, co wpływa na częstotliwość oscylacji w trybie astabilnym. W połączeniu z rezystorami, jego działanie decyduje o czasie trwania impulsów.
>
> Dzięki współdziałaniu wszystkich tych wyprowadzeń, układ NE555 może być stosowany w wielu konfiguracjach – jako generator impulsów, timer opóźniający, czy przełącznik – dostosowując swoje działanie do konkretnych potrzeb aplikacji.


![ne555](./ne555.png)

### Schemat połączenia
```  
✔ **Pin 1 (GND):** masa  
✔ **Pin 2 (Trigger):** podłączony do masy przez kondensator 1µF  
✔ **Pin 3 (Output):** połączony z diodą LED przez rezystor 220Ω i do masy (katoda) 
✔ **Pin 4 (Reset):** podłączony do dodatniego bieguna zasilania  
✔ **Pin 5 (Control):** nie podłączony
✔ **Pin 6 (Threshold):** połączony z Pin 2  
✔ **Pin 7 (Discharge):** połączony z Pin 6 przez rezystor 300kΩ oraz do dodatniego bieguna przez rezustor 300kΩ
✔ **Pin 8 (VCC):** podłączony do dodatniego bieguna zasilania  
```

![NE555-schemat](./ne555-schemat.png)

### Instrukcja:
➡️ Umieść układ NE555 na płytce stykowej.  
➡️ Podłącz piny zgodnie z powyższym schematem.  
➡️ Podłącz źródło zasilania i obserwuj miganie diody LED.

⚠️ *Jeśli LED nie miga:*
- Sprawdź dokładność wszystkich połączeń.
- Upewnij się, że wartości rezystorów i kondensatora są zgodne z założeniami.

---

## Zadania dodatkowe

1. **Regulacja częstotliwości przez zmianę kondensatora lub rezystorów**  
   Wymień kondensator w układzie NE555 i obserwuj, jak zmienia się częstotliwość migania diody LED. Przydatny może okazać się kalkulator: 
[https://www.build-electronic-circuits.com/circuit-calculator-conversion/555-timer-calculator/](https://www.build-electronic-circuits.com/circuit-calculator-conversion/555-timer-calculator/)

<img src="./kalkulator-qr.png" alt="kalkulator-qr" width="200"/>

2. **Układ z dwoma przyciskami**  
   Zaprojektuj układ, w którym jeden przycisk włącza diodę LED, a drugi ją wyłącza.

![solutuion-2](./solution_2.png)

- R1 -> 1kΩ
- R2 -> 1MΩ

3. **Światła policyjne**  
   Zaprojektuj układ z dwiema diodami, tak aby migały naprzemiennie.

![solutuion-3](./solution_3.png)

- R1-R2: 1 kΩ
- R3, R4 - w zależności od diody
- C1: 1000 µF
