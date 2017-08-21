/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2017 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <micro-os-plus/board.h>
#include <micro-os-plus/diag/trace.h>

#include <sysclock.h>
#include <led.h>

#include <stdint.h>
#include <stdio.h>

// ----------------------------------------------------------------------------

/*
 * This program blinks all LEDs on the SiFive HiFive1 board.
 */

// ----- Timing definitions -------------------------------------------------
// Keep the LED on for 3/4 of a second.
#define BLINK_ON_TICKS (OS_INTEGER_SYSCLOCK_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (OS_INTEGER_SYSCLOCK_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- LED definitions ------------------------------------------------------

#define BLINK_PORT_NUMBER         (0)
#define BLINK_ACTIVE_LOW          (true)

// Instantiate a static array of led objects.
led_t blink_leds[3];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

// Automatically called during startup, as for the C++ static
// constructors, before reaching `main()`.
void
__attribute__((constructor))
led_array_construct (void)
{
  led_construct (&blink_leds[0], BLINK_PORT_NUMBER, RED_LED_OFFSET,
  BLINK_ACTIVE_LOW);
  led_construct (&blink_leds[1], BLINK_PORT_NUMBER, GREEN_LED_OFFSET,
  BLINK_ACTIVE_LOW);
  led_construct (&blink_leds[2], BLINK_PORT_NUMBER, BLUE_LED_OFFSET,
  BLINK_ACTIVE_LOW);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int
main (int argc, char* argv[])
{
  // Send a greeting to the trace device (skipped on Release).
  trace_puts ("Hello RISC-V World!");

  // Send a message to the standard output.
  puts ("Standard output message.");

  // Send a message to the standard error.
  fprintf (stderr, "Standard error message.\n");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf ("System clock: %u Hz\n", riscv_core_get_cpu_frequency ());

  // Perform all necessary initialisations for the LEDs.
  for (size_t i = 0; i < (sizeof(blink_leds) / sizeof(blink_leds[0])); ++i)
    {
      led_power_up (&blink_leds[i]);
    }

  uint32_t seconds = 0;

  // Turn all LEDs on.
  for (size_t i = 0; i < (sizeof(blink_leds) / sizeof(blink_leds[0])); ++i)
    {
      led_turn_on (&blink_leds[i]);
    }

  // First interval is longer (one full second).
  os_sysclock_sleep_for (os_sysclock_get_frequency_hz ());

  // Turn all LEDs off.
  for (size_t i = 0; i < (sizeof(blink_leds) / sizeof(blink_leds[0])); ++i)
    {
      led_turn_off (&blink_leds[i]);
    }

  os_sysclock_sleep_for (BLINK_OFF_TICKS);

  ++seconds;
  trace_printf ("Second %u\n", seconds);

  // Loop forever.
  while (true)
    {
      // Blink individual LEDs.
      for (size_t i = 0; i < (sizeof(blink_leds) / sizeof(blink_leds[0])); ++i)
        {
          led_turn_on (&blink_leds[i]);
          os_sysclock_sleep_for (BLINK_ON_TICKS);

          led_turn_off (&blink_leds[i]);
          os_sysclock_sleep_for (BLINK_OFF_TICKS);

          ++seconds;
          trace_printf ("Second %u\n", seconds);
        }
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

