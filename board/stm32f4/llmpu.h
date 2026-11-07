#pragma once

// The MPU safety firewall is an H7-only feature (see board/stm32h7/llmpu.h):
// it depends on the H7 linker script's _ssafety/_stack_guard regions and the
// Cortex-M7 MPU region layout. The F4 (dos) target keeps the shared main.c
// call surface with no-op stubs; the safety posture on F4 is unchanged from
// before the upstream fault-telemetry merge.
void mpu_safety_open(void) {
}

void mpu_safety_close(void) {
}
