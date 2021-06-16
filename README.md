# stm32-isp-helper

Using MSP430 Launchpad as a STM32 boot mode helper

This can also serve as an example on how to perform **key debouncing**,
using the watchdog timer.

## GPIO mapping

- P1.6 = Output to BOOT0 on STM32
- P2.5 = Output to NRST on STM32
- P1.0 = LED RED (indicates high level on BOOT0)
- P4.7 = LED GREEN (indicates low level on BOOT0)
- P2.1 = KEY1 -> Toggle BOOT0 & perform a NRST
- P1.1 = KEY2 -> perform a NRST
