//*****************************************************************************
//
// main.h
//
//*****************************************************************************

#define LED_R_PORT GPIO_PORT_P1
#define LED_R_PIN  GPIO_PIN0
#define LED_G_PORT GPIO_PORT_P4
#define LED_G_PIN  GPIO_PIN7
#define KEY1_PORT  GPIO_PORT_P2
#define KEY1_PIN   GPIO_PIN1
#define KEY2_PORT  GPIO_PORT_P1
#define KEY2_PIN   GPIO_PIN1
#define BOOT0_PORT GPIO_PORT_P1
#define BOOT0_PIN  GPIO_PIN6
#define NRST_PORT  GPIO_PORT_P2
#define NRST_PIN   GPIO_PIN5

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#define __INTERRUPT__(VECTOR) \
        #pragma vector=VECTOR \
        __interrupt
#elif defined(__GNUC__)
#define __INTERRUPT__(VECTOR) __attribute__((interrupt(VECTOR)))
#endif
