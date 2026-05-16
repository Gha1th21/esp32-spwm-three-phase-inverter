#include <Arduino.h>
#include <math.h>
#include "driver/mcpwm.h"


#define AH_PIN  25   
#define AL_PIN  26   


#define BH_PIN  27   
#define BL_PIN  14   


#define CH_PIN  12   
#define CL_PIN  13   

#define POT_PIN 34

#define PWM_FREQ_HZ        10000    
#define ADC_MIN            0
#define ADC_MAX            4095

#define MIN_SINE_FREQ_HZ   1
#define MAX_SINE_FREQ_HZ   50

#define MOD_INDEX          0.85
#define UPDATE_RATE_HZ     1000

#define DEADTIME_TICKS     80

float theta = 0.0;
float sineFreq = 10.0;

unsigned long lastUpdateMicros = 0;
unsigned long lastPrint = 0;

const unsigned long UPDATE_PERIOD_US = 1000000UL / UPDATE_RATE_HZ;

float calculateDuty(float angle)
{
  float sineValue = sin(angle);

  float duty = 0.5 + 0.5 * MOD_INDEX * sineValue;

  if (duty > 0.95) duty = 0.95;
  if (duty < 0.05) duty = 0.05;

  return duty;
}

void setupMCPWM()
{
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, AH_PIN);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, AL_PIN);

  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, BH_PIN);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, BL_PIN);

  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, CH_PIN);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, CL_PIN);

  mcpwm_config_t pwm_config;
  pwm_config.frequency = PWM_FREQ_HZ;
  pwm_config.cmpr_a = 0.0;
  pwm_config.cmpr_b = 0.0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); 
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config); 
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config); 

  mcpwm_deadtime_enable(
    MCPWM_UNIT_0,
    MCPWM_TIMER_0,
    MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE,
    DEADTIME_TICKS,
    DEADTIME_TICKS
  );

  mcpwm_deadtime_enable(
    MCPWM_UNIT_0,
    MCPWM_TIMER_1,
    MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE,
    DEADTIME_TICKS,
    DEADTIME_TICKS
  );

  mcpwm_deadtime_enable(
    MCPWM_UNIT_0,
    MCPWM_TIMER_2,
    MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE,
    DEADTIME_TICKS,
    DEADTIME_TICKS
  );
}

void writePhasePWM(mcpwm_timer_t timer, float duty)
{
  float dutyPercent = duty * 100.0;

  if (dutyPercent > 95.0) dutyPercent = 95.0;
  if (dutyPercent < 5.0) dutyPercent = 5.0;

  // In complementary dead-time mode, output A and B are handled as a safe pair
  mcpwm_set_duty(MCPWM_UNIT_0, timer, MCPWM_OPR_A, dutyPercent);
  mcpwm_set_duty_type(MCPWM_UNIT_0, timer, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

void allOff()
{
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);

  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A);
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B);

  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A);
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(POT_PIN, INPUT);

  setupMCPWM();

  allOff();

  delay(1000);

  Serial.println("ESP32 3-Phase SPWM with MCPWM Dead-Time Started");
}

void loop()
{
  unsigned long now = micros();

  if (now - lastUpdateMicros >= UPDATE_PERIOD_US)
  {
    lastUpdateMicros = now;

    int adcValue = analogRead(POT_PIN);

    sineFreq = map(adcValue, ADC_MIN, ADC_MAX, MIN_SINE_FREQ_HZ, MAX_SINE_FREQ_HZ);

    float dutyA = calculateDuty(theta);
    float dutyB = calculateDuty(theta - 2.0 * PI / 3.0);
    float dutyC = calculateDuty(theta - 4.0 * PI / 3.0);

    writePhasePWM(MCPWM_TIMER_0, dutyA); // Phase A
    writePhasePWM(MCPWM_TIMER_1, dutyB); // Phase B
    writePhasePWM(MCPWM_TIMER_2, dutyC); // Phase C

    theta += 2.0 * PI * sineFreq / UPDATE_RATE_HZ;

    if (theta >= 2.0 * PI)
    {
      theta -= 2.0 * PI;
    }
}
/*
    if (millis() - lastPrint > 500)
    {
      lastPrint = millis();

      Serial.print("ADC: ");
      Serial.print(adcValue);

      Serial.print(" | Sine Freq: ");
      Serial.print(sineFreq);

      Serial.print(" Hz | Duty A: ");
      Serial.print(dutyA * 100.0);

      Serial.print("% | Duty B: ");
      Serial.print(dutyB * 100.0);

      Serial.print("% | Duty C: ");
      Serial.print(dutyC * 100.0);

      Serial.println("%");
    }
  }
*/