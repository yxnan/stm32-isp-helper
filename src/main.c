//***************************************************************************************
//
// main.c
//
//***************************************************************************************

#include "driverlib.h"
#include "main.h"

static void __attribute__((naked, section(".crt_0042"), used))
disable_watchdog (void)
{
  WDTCTL = WDTPW | WDTHOLD;
}

static void    WDT_startDebounce();
static void    Key_setupForPress();
static void    Key_setupForRelease();
static uint8_t Key_performCheck();

/*
 * P1.0 = LED RED
 * P4.7 = LED GREEN
 * P2.1 = KEY1 -> BOOT0 & NRST
 * P1.1 = KEY2 -> NRST
 * P1.6 = BOOT0
 * P2.5 = NRST
*/
int main(void)
{
    GPIO_setAsOutputPin(LED_R_PORT, LED_R_PIN);
    GPIO_setAsOutputPin(LED_G_PORT, LED_G_PIN);
    GPIO_setAsOutputPin(BOOT0_PORT, BOOT0_PIN);
    GPIO_setAsOutputPin(NRST_PORT, NRST_PIN);

    GPIO_setOutputLowOnPin(LED_R_PORT, LED_R_PIN);
    GPIO_setOutputHighOnPin(LED_G_PORT, LED_G_PIN);
    GPIO_setOutputLowOnPin(BOOT0_PORT, BOOT0_PIN);
    GPIO_setOutputHighOnPin(NRST_PORT, NRST_PIN);

    // __bis_SR_register(LPM4_bits + GIE);
    __enable_interrupt();
    __no_operation();

    while (1)
    {
        Key_setupForPress();
        uint8_t key = Key_performCheck();
        if (key > 0)
        {
            if (key == 1)
            {
                GPIO_toggleOutputOnPin(LED_R_PORT, LED_R_PIN);
                GPIO_toggleOutputOnPin(LED_G_PORT, LED_G_PIN);
                GPIO_toggleOutputOnPin(BOOT0_PORT, BOOT0_PIN);
            }

            GPIO_setOutputLowOnPin(NRST_PORT, NRST_PIN);

            while (Key_performCheck())
            {
                Key_setupForRelease();
            }

            GPIO_setOutputHighOnPin(NRST_PORT, NRST_PIN);
        }
    }
}

static void WDT_startDebounce()
{
    // WDT as 15.625ms interval counter
    SFRIFG1 &= ~WDTIFG;
    WDTCTL = WDTPW + WDTSSEL_1 + WDTTMSEL + WDTCNTCL + WDTIS_6;
    SFRIE1 |= WDTIE;
    __bis_SR_register(LPM3_bits);     // Enter LPM3
}


static void Key_setupForPress()
{
    GPIO_setAsInputPinWithPullUpResistor(KEY1_PORT, KEY1_PIN);
    GPIO_selectInterruptEdge(KEY1_PORT, KEY1_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(KEY1_PORT, KEY1_PIN);
    GPIO_enableInterrupt(KEY1_PORT, KEY1_PIN);

    GPIO_setAsInputPinWithPullUpResistor(KEY2_PORT, KEY2_PIN);
    GPIO_selectInterruptEdge(KEY2_PORT, KEY2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(KEY2_PORT, KEY2_PIN);
    GPIO_enableInterrupt(KEY2_PORT, KEY2_PIN);

    __bis_SR_register(LPM4_bits);          // Enter LPM4, wake on button push
    WDT_startDebounce();                   // Debounce button
}

static void Key_setupForRelease()
{
    GPIO_selectInterruptEdge(KEY1_PORT, KEY1_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(KEY1_PORT, KEY1_PIN);
    GPIO_enableInterrupt(KEY1_PORT, KEY1_PIN);

    GPIO_selectInterruptEdge(KEY2_PORT, KEY2_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(KEY2_PORT, KEY2_PIN);
    GPIO_enableInterrupt(KEY2_PORT, KEY2_PIN);

    __bis_SR_register(LPM4_bits);          // Enter LPM4, wake on button release
    WDT_startDebounce();                   // Debounce button
}

static uint8_t Key_performCheck()
{
    if (GPIO_getInputPinValue(KEY1_PORT, KEY1_PIN) == GPIO_INPUT_PIN_LOW)
        return 1;
    else if (GPIO_getInputPinValue(KEY2_PORT, KEY2_PIN) == GPIO_INPUT_PIN_LOW)
        return 2;
    else
        return 0;
}

__INTERRUPT__(WDT_VECTOR)
void WDT_ISR(void)
{
    WDT_A_hold(WDT_A_BASE);                 // Stop WDT
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}

__INTERRUPT__(PORT1_VECTOR)
void Port_1 (void)
{
    GPIO_clearInterrupt(KEY2_PORT, KEY2_PIN);
    GPIO_disableInterrupt(KEY2_PORT, KEY2_PIN);
    __bic_SR_register_on_exit(LPM4_bits);   // Exit LPM4
}

__INTERRUPT__(PORT2_VECTOR)
void Port_2 (void)
{
    GPIO_clearInterrupt(KEY1_PORT, KEY1_PIN);
    GPIO_disableInterrupt(KEY1_PORT, KEY1_PIN);
    __bic_SR_register_on_exit(LPM4_bits);   // Exit LPM4
}
