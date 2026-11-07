#include "lladc_declarations.h"

void register_set(volatile uint32_t *addr, uint32_t val, uint32_t mask);

void adc_init(ADC_TypeDef *adc) {
  register_set(&(ADC->CCR), ADC_CCR_TSVREFE | ADC_CCR_VBATE, 0xC30000U);
  register_set(&(adc->CR2), ADC_CR2_ADON, 0xFF7F0F03U);
}

static uint16_t adc_get_raw(const adc_signal_t *signal) {
  // sample time
  if (signal->channel < 10U) {
    signal->adc->SMPR2 = ((uint32_t) signal->sample_time << (signal->channel * 3U));
  } else {
    signal->adc->SMPR1 = ((uint32_t) signal->sample_time << ((signal->channel - 10U) * 3U));
  }

  // select channel
  signal->adc->JSQR = ((uint32_t) signal->channel << 15U);

  // start conversion
  signal->adc->SR &= ~(ADC_SR_JEOC);
  signal->adc->CR2 |= ADC_CR2_JSWSTART;
  while (!(signal->adc->SR & ADC_SR_JEOC));

  return signal->adc->JDR1;
}

uint16_t adc_get_mV(const adc_signal_t *signal) {
  return (adc_get_raw(signal) * current_board->avdd_mV) / 4095U;
}

// F4: ADC2 shares the GPIO input channels with ADC1 and there is no DMA
// occupancy, so the H7 "adc2 busy" gate (see board/stm32h7/lladc.h) is always
// false here.
static volatile bool adc2_busy = false;

// AVDD drift detection is H7-only (see board/stm32h7/lladc.h: it monitors the
// H7 VREFINT channel). The F4 (dos) target keeps the shared main.c call
// surface as a no-op.
void adc_check(uint32_t expected_avdd_mV) {
  UNUSED(expected_avdd_mV);
}
