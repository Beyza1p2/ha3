#include <msp430.h> // Bibliothek für MSP430-Register und Funktionen
void initGPIO() {
 P2DIR &= ~BIT1; // P2.1 als Eingang konfigurieren (Taster S1)
 P2REN |= BIT1; // Pull-up-/Pull-down-Widerstand aktivieren
 P2OUT |= BIT1; // Pull-up-Widerstand aktivieren (logisch HIGH)
 P4DIR |= BIT7; // P4.2 als Ausgang konfigurieren (LED2)
 P4OUT &= ~BIT7; // LED2 initial ausschalten
}
void initTimer() {
 TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR; // Timer_B mit SMCLK, Up-Mode, Reset
 TB0CCR0 = 60000; // Vergleichswert (z.B. ca. 15ms bei 1MHz)
 TB0EX0 = TBIDEX_3; // Taktteiler: Eingangstakt / 4
 TB0CCTL0=0;
}
void delayWithTimer() {
 TB0CTL |= MC__UP; // Timer starten (Up-Mode)
 while (!(TB0CCTL0 & CCIFG)); // Warten bis Vergleich erreicht
 TB0CCTL0 &= ~CCIFG; // Flag löschen
 TB0CTL &= ~MC__UP; // Timer stoppen
}
int main(void) {
 WDTCTL = WDTPW | WDTHOLD; // Watchdog-Timer deaktivieren
 initGPIO(); // GPIO initialisieren
 initTimer(); // Timer initialisieren
 while (1) {
 if (!(P2IN & BIT1)) { // Wenn Taster S1 gedrückt ist
 P4OUT ^= BIT7; // LED2 umschalten (toggle)
 delayWithTimer(); // Verzögerung mit Timer_B
 }
 }
}