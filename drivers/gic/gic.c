#include "gic.h"

// GIC Distributor
#define GICD_BASE 0x08000000UL
// GIC CPU interface
#define GICC_BASE 0x08010000UL

#define REG32(addr) (*(volatile uint32_t *)(addr))
#define REG8(addr)  (*(volatile uint8_t *)(addr))

// Distributor Control Register: enable or disable interrupt forwarding
#define GICD_CTLR REG32(GICD_BASE + 0x000)

// Interrupt Set-Enable Register 1: write 1 to enable the corresponding interrupt among IDs 32-63
#define GICD_ISENABLER1 REG32(GICD_BASE + 0x104)

// Interrupt Clear-Enable Register 1: write 1 to disable the corresponding interrupt among IDs 32-63
#define GICD_ICENABLER1 REG32(GICD_BASE + 0x184)

// Interrupt Clear-Pending Register 1: write 1 to clear the pending state of the corresponding interrupt among IDs 32-63
#define GICD_ICPENDR1 REG32(GICD_BASE + 0x284)

// Interrupt Configuration Register 2: configure level-sensitive or edge-triggered operation for IDs 32-47
#define GICD_ICFGR2 REG32(GICD_BASE + 0xc08)

// Interrupt Priority Registers: access the priority byte for the specified interrupt ID
#define GICD_IPRIORITYR(id) REG8(GICD_BASE + 0x400 + (id))

// Interrupt Processor Targets Registers: access the target CPU mask for the specified SPI ID
#define GICD_ITARGETSR(id) REG8(GICD_BASE + 0x800 + (id))

// CPU Interface Control Register: enable or disable interrupt delivery to the CPU
#define GICC_CTLR REG32(GICC_BASE + 0x00)

// Interrupt Priority Mask Register: set the priority threshold for interrupt delivery
#define GICC_PMR REG32(GICC_BASE + 0x04)

// Interrupt Acknowledge Register: read to acknowledge an interrupt and obtain its ID in bits [9:0]
#define GICC_IAR REG32(GICC_BASE + 0x0c)

// End of Interrupt Register: write the saved acknowledge value to signal completion of interrupt handling
#define GICC_EOIR REG32(GICC_BASE + 0x10)

// Distributor Enable: set bit 0 to enable interrupt forwarding in this configuration
#define GICD_CTLR_ENABLE (1U << 0)
// CPU Interface Enable: set bit 0 to enable interrupt delivery in this configuration
#define GICC_CTLR_ENABLE (1U << 0)

// UART Set-Enable: write bit 1 to enable interrupt ID 33
#define GICD_ISENABLER1_UART (1U << 1)
// UART Clear-Enable: write bit 1 to disable interrupt ID 33
#define GICD_ICENABLER1_UART (1U << 1)
// UART Clear-Pending: write bit 1 to clear the pending state of interrupt ID 33
#define GICD_ICPENDR1_UART (1U << 1)

// UART Configuration Mask: clear bits [3:2] to select level-sensitive operation for interrupt ID 33
#define GICD_ICFGR2_UART_MASK (0b11U << 2)
// UART Priority: assign priority value 128 to the UART interrupt
#define GICD_IPRIORITYR_UART_PRIORITY 0x80U
// Target CPU 0: set bit 0 of the target CPU mask
#define GICD_ITARGETSR_CPU0 (1U << 0)
// Priority Mask Threshold: allow priority values numerically lower than 255
#define GICC_PMR_THRESHOLD 0xffU

void gic_init(void)
{
    // Disable the GIC Distributor and CPU interface before configuration
    GICD_CTLR = 0;
    GICC_CTLR = 0;

    // ID 33 occupies bit 1 in the registers for IDs 32-63
    GICD_ICENABLER1 = GICD_ICENABLER1_UART;
    GICD_ICPENDR1 = GICD_ICPENDR1_UART;

    // ID 33 occupies bits [3:2]; 00 selects level-sensitive
    GICD_ICFGR2 &= ~GICD_ICFGR2_UART_MASK;

    GICD_IPRIORITYR(UART_IRQ_ID) = GICD_IPRIORITYR_UART_PRIORITY;
    GICD_ITARGETSR(UART_IRQ_ID) = GICD_ITARGETSR_CPU0;

    GICD_ISENABLER1 = GICD_ISENABLER1_UART;

    GICC_PMR = GICC_PMR_THRESHOLD;
    GICD_CTLR = GICD_CTLR_ENABLE;
    GICC_CTLR = GICC_CTLR_ENABLE;

    __asm__ volatile("dsb sy" ::: "memory");
}

uint32_t gic_acknowledge(void)
{
    return GICC_IAR;
}

void gic_end_interrupt(uint32_t acknowledge)
{
    __asm__ volatile("dsb sy" ::: "memory");

    GICC_EOIR = acknowledge;

    __asm__ volatile("dsb sy" ::: "memory");
}
