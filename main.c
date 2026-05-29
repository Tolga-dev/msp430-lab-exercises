#include <msp430.h> 
#include <stdio.h>
#include <stdint.h>

#define DELAY 2000
#define LONG_DELAY 20000
#define SW BIT3
#define LED BIT0

#define GREEN     BIT0
#define RED   BIT6

#define LED1  BIT0
#define LED2 BIT6
#define B1    BIT3


void main (void)
{

}

void TogglesLed()
{
    WDTCTL = WDTPW|WDTHOLD;                   // Stop watchdog timer
       P1OUT = LED1;                                   // Preload LED1 on, LED2 off
       P1DIR = LED1|LED2;                          // Set pins for LED1,2 to output
       TACCR0 = 4999;                     // Upper limit of count for TAR
       TACTL = MC_1|ID_3|TASSEL_2|TACLR;     // Set up and start Timer A
       // Up to "CCR0" mode, divide clock by 8, clock from SMCLK, clear timer

       for (;;) {                                      // Loop forever
           while ((TACTL & TAIFG) == 0) {    // Wait for overflow
           }                                                 // doing nothing
           TACTL &= ~TAIFG;                        // Clear overflow flag
           P1OUT ^= LED1|LED2;                     // Toggle LEDs
       }
       // Back around infinite loop

}

void SwitchingLed()
{
    volatile uint16_t counter;

       WDTCTL = WDTPW | WDTHOLD;
       P1OUT = ~LED1;
       P1DIR = LED1 | LED2;

       while(1)
       {
           for(counter=0; counter < 50000; counter++);

           P1OUT ^= LED1 | LED2;
       }
}
void PressedLedTurnOff()
{
    WDTCTL = WDTPW | WDTHOLD;
     P1OUT &= ~LED1;
     P1DIR |= LED1;
     P1REN |= B1;
     P1OUT |= B1;


     while(1)
     {
         while((P1IN & B1) != 0) {}
         P1OUT |= LED1;

         while((P1IN & B1) == 0) {}
         P1OUT &= ~LED1;

     }
}

/*
void ISRLedSwitcher()
{
    WDTCTL = WDTPW | WDTHOLD;
      P1OUT = LED1;
      P1DIR = LED1 | LED2;
      TACCR0 = 30000;
      TACCTL0 = CCIE;

      TACTL = MC_1 | TASSEL_1 | TACLR;

      while(1)
      {
          __bis_SR_register(GIE | LPM3_bits);
      }
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR()
{
    P1OUT ^= LED1 | LED2;
}
*/

void Timer()
{

    P1OUT = LED1;
    P1DIR = LED1 | LED2;

    TACCR0 = 49999;
    TACTL = MC_1 | TASSEL_1 | TACLR;

    while(1)
    {
        while((TACTL & TAIFG) == 0);

        TACTL &= ~TAIFG;
        P1OUT ^= LED1 | LED2;

    }

}

/*

void PowerOptimizationWithRegisterInterrupt()
{
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= GREEN;
    P1DIR &= ~SW;
    P1REN |= SW;
    P1OUT |= SW;

    P1IES &= ~SW;
    P1IE |= SW;

    __bis_SR_register(LPM4_bits + GIE);
    // this function, sets the selected bits of the status register(input parameters of the function)
    // GIE, for general interrupt enable, without it the interrupts are disabled, and the ISR never will be attended and for that, code does not work.


}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1()
{
    P1OUT ^= GREEN;
    P1IFG &= ~SW;
}
*/


/*
void TogglingLedWithTimerMode()
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

       P1DIR |= BIT0;

       TACTL |= TASSEL_2+MC_2+TAIE;

      __low_power_mode_0();

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TMR0()
{
    if(TAIV==TA0IV_TAIFG)   //Check if Timer overflow caused the interrupt
                            //This would be required in projects where multiple interrupts have
                            //the same interrupt vector. Here it is only optional.
        {
            P1OUT^=BIT0; //Toggle the LED
            TACTL&=~(TAIFG); //Reset the interrupt flag
        }
}
*/

/*
void UsingPISR()
{


    WDTCTL = WDTPW | WDTHOLD;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    P1IE|=BIT3;
    P1IE |= BIT3;

    __enable_interrupt();

    P1DIR|=BIT0; //Set P1.0 as output
    P1OUT&=~BIT0; //Initially turn off the LED


    __low_power_mode_0(); //Go to low power mode 0

}

#pragma vector=PORT1_VECTOR
__interrupt void P1_Function()
{
    int i; //Declare counter variable
    P1OUT^=BIT0; //Toggle the LED
    i=0; //Start a counter variable
    while(i<500) //Wait till the switch is not pressed continusouly for 500 loop cycles
    {
        if((P1IN&BIT3))
            i++;  //If the switch is not pressed, increment the counter variable
        else
            i=0; //If the switch is pressed, reset the counter variable
    }
    P1IFG&=~BIT3; // Reset interrupt flag

}
*/

void ButtonWithStoppingLed()
{
    P1DIR &= ~SW;
    P1REN |= SW;
    P1OUT |= SW;

    P1OUT = ~LED1;
    P1OUT = ~LED2;
    P1DIR = LED1 | LED2;

    volatile uint16_t counter = 0;
    int check = 0;

    while(1)
        {
            if(!(P1IN & SW))
            {


                while(!(P1IN & SW));

                check =  !check;
            }

            if(check == 1)
            {
                for(counter = 0; counter < LONG_DELAY; counter++)
                           {
                           }

               P1OUT ^= LED1 | LED2;
            }
            else
            {
                P1OUT = ~LED1;
                P1OUT = ~LED2;
            }
        }

}
void ButtonWithSwitchingLeds()
{
    P1DIR |= RED + GREEN;
    P1DIR &= ~SW;
    P1REN |= SW;
    P1OUT |= SW;

    volatile unsigned int flag = 0;

    while(1)
    {
      if(!(P1IN & SW))
      {
          __delay_cycles(20000);

          if(!(P1IN & SW))
          {
              while(!(P1IN & SW));
              flag = !flag;
          }
      }
      if(flag)                     // Check flag value
      {
          P1OUT &= ~GREEN;            // Green -> OFF
          P1OUT |= RED;               // Red -> ON
      }
      else
      {
          P1OUT &= ~RED;              // Red -> OFF
          P1OUT |= GREEN;             // Green -> ON
      }
    }

}
void ButtonWithDebouncing2()
{

    P1DIR |= LED;
    P1DIR &= ~SW;
    P1REN |= SW;
    P1OUT |= SW;

    while(1)
    {
        if(!(P1IN & SW))
        {
            __delay_cycles(20000);

            while(!(P1IN & SW));

            P1OUT ^= LED;

        }
    }
}
void ButtonWithDebouncing()
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
       P1DIR&=~BIT3; //Set P1.3 as input
       P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
       P1OUT|=BIT3; //Set P1.3 to have pull up resistors

       P1DIR|=BIT0; //Set P1.0 as output
       P1OUT&=~BIT0; //Initially turn off the LED

       while(1)
       {
           if((P1IN&BIT3)==0) //If the switch is pressed
           {

            P1OUT^=BIT0; //Toggle the LED
             __delay_cycles(LONG_DELAY ); //Wait for switch to be unpressed
           }

       }
}

void Button3()
{

    WDTCTL = WDTPW | WDTHOLD;
    P1OUT &= ~LED1;
    P1DIR |= LED1;
    P1REN |= B1;
    P1OUT |= B1;


/*
    for(;;) {
      while((P1IN & B1) != 0){}     // loop while button is up (active low)
      P1OUT ^= LED1;                // button has been pressed, toggle led
      while((P1IN & B1) == 0){}     // loop while button is down
    }
*/

 /*
    while(1)
    {
        if((P1IN & BIT3) == 0)
        {
            P1OUT ^= BIT0;
        }

    }
*/

}
void Button2()
{
    P1DIR |= BIT0;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    while(1)
    {
        if(P1IN & BIT3) // not pressed
        {
            P1OUT &= ~BIT0;

        }
        else
        {
            P1OUT |= BIT0;
        }

    }

}
void Button()
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

   P1DIR &= ~BIT3; //Set P1.3 as input
   P1REN |= BIT3; //Enable pullup/pulldown resistors for P1.3
   P1OUT |= BIT3; //Set P1.3 to have pull up resistors

   P1DIR |=BIT0; //Set P1.0 as output

   while(1) //Run the below code forever
   {
       if((P1IN&BIT3)==(BIT3)) //If P1.3 is logical HIGH
           P1OUT&=~BIT0; //Turn off the LED
       else
           P1OUT|=BIT0; //Else, turn on the LED
   }
}

void ChangingFrequencyOfMultipleBlinking()
{
    volatile int i,j;
        WDTCTL = WDTPW | WDTHOLD;

        P1DIR |= BIT0 | BIT6;

        while(1)
        {
            for(j=0; j < 10000; j+=250)
            {
                P1OUT ^= BIT0 | BIT6;
                for(i=j;i>0;i--); //Wait for j number of cycles
            }
            for(j=10000;j>0;j-=250) //j goes from 10000 to 0 in steps of 250
            {
                P1OUT^=BIT0|BIT6; //Toggle the LEDs
                for(i=j;i>0;i--); //Wait for j number of cycles

            }

        }

}

void ChangingFrequencyOfBlinking()
{
    volatile int i,j; //Force the compiler to not optimize the variables i and j
       WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

       P1DIR|=BIT0; //Set P1.0 as output

       while(1)
       {

           for(j=0;j<10000;j+=250) //j goes from 0 to 10000 in steps of 250
               {
               P1OUT^=BIT0; //Toggle the LED
               for(i=j;i>0;i--); //Wait for j number of cycles

           }

       }
}
void MultiBlinkedLed2()
{
    WDTCTL = WDTPW | WDTHOLD;

       P1DIR |= BIT0 | BIT6;

       P1OUT |= BIT0;
       P1OUT &= ~BIT6;

       volatile unsigned long i;
       while(1)
       {
           P1OUT ^= BIT0 | BIT6;

           for(i = 0; i < 20000; i++);
       }

}
void MultiBlinkLed()
{
    volatile int i; //Force the compliler to not optimize the variable 'i'

        WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
        P1DIR|= BIT0|BIT6; //Set P1.0 and P1.6 as output pin

        while(1) //Keep toggling the LEDs forever
        {
            P1OUT|=BIT0; //Turn on red LED
            P1OUT&=~BIT6; //Turn off green LED

            for(i=DELAY;i>0;i--); //Delay

            P1OUT|=BIT6; //Turn on green LED
            P1OUT&=~BIT0; //Turn off red LED

            for(i=DELAY;i>0;i--); //Delay
        }
}



void SingleBlinkedLed2()
{
    WDTCTL = WDTPW | WDTHOLD;
       P1DIR |= 0x01; // BIT0;

       volatile unsigned int i;

       while(1)
       {
           P1OUT ^= 0x01;

           i = 10000;

           while(i != 0)
           {
               i--;
           }

       }

}

void SingleBlinkLed() // toggling on and off with p1.0
{


    volatile int i; // forcing complier do not optimize the v i
    WDTCTL = WDTPW | WDTHOLD;// stop watch dog timer
    P1DIR |=BIT0; // set p1.0 as output


    while(1)
    {
        P1OUT ^= BIT0; // from high to low or from low to high
        for (i = 0; i < DELAY; ++i){}


    }
}





