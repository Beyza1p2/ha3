#include <msp430.h>
void initGPIO() {
 P2DIR &= ~BIT1; // S1 (P2.1) als Eingang
 P2REN |= BIT1;
 P2OUT |= BIT1;
 P1DIR &= ~BIT1; // S2 (P1.1) als Eingang
 P1REN |= BIT1;
 P1OUT |= BIT1;
 P4DIR |= BIT7; // LED2 (P4.7) als Ausgang
 P4OUT &= ~BIT7;
 P1DIR |= BIT0; // LED1 (P1.0) als Ausgang
 P1OUT &= ~BIT0;
}
void initTimer() {
 TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR; // Timer mit SMCLK, Up-Mode
 TB0CCR0 = 60000; // Vergleichswert (z.B. 15ms)
 TB0EX0 = TBIDEX_7; // Taktteiler: SMCLK / 8
}
void delayWithTimer() {
 TB0CTL |= MC__UP; // Timer starten
 while (!(TB0CCTL0 & CCIFG)); // Auf Vergleichswert warten
 TB0CCTL0 &= ~CCIFG; // Flag löschen
 TB0CTL &= ~MC__UP; // Timer stoppen
}
int main(void) {
 WDTCTL = WDTPW | WDTHOLD; // Watchdog deaktivieren
 initGPIO(); // Ein-/Ausgänge konfigurieren
 initTimer(); // Timer initialisieren
 while (1) {
 if (!(P2IN & BIT1)) { // Wenn S1 gedrückt
 P4OUT ^= BIT7; // LED2 toggeln
 delayWithTimer(); // Timer-Verzögerung
 }
 if (!(P1IN & BIT1)) { // Wenn S2 gedrückt
 P1OUT |= BIT0; // LED1 einschalten (sofort)
 } else {
 P1OUT &= ~BIT0; // LED1 ausschalten
 }
 }
}
