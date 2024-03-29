#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "architecture.h"

#include <stdint.h>

/*
 * Opcode definitions.
 */
#define PROC_OPCODE_ADD (0x00)
#define PROC_OPCODE_ADDI (0x01)
#define PROC_OPCODE_ADDUI (0x02)
#define PROC_OPCODE_LUH (0x03)
#define PROC_OPCODE_MUL (0x04)
#define PROC_OPCODE_MULI (0x05)
#define PROC_OPCODE_PUSH (0x06)
#define PROC_OPCODE_PUSHI (0x07)
#define PROC_OPCODE_POP (0x08)
#define PROC_OPCODE_JUMP (0x09)
#define PROC_OPCODE_JUMPI (0x0A)
#define PROC_OPCODE_BR (0x0B)
#define PROC_OPCODE_BI (0x0C)
#define PROC_OPCODE_CALL (0x0D)

#define PROC_OPCODE_MOV (0x0E)
#define PROC_OPCODE_HALT (0x0F)
#define PROC_OPCODE_DUMP (0x10)

#define PROC_OPCODE_LOAD (0x11)
#define PROC_OPCODE_STOR (0x12)

#define PROC_OPCODE_RET (0x13)

#define PROC_OPCODE_JZ (0x14)
#define PROC_OPCODE_JZI (0x15)
#define PROC_OPCODE_BZ (0x16)
#define PROC_OPCODE_BZI (0x17)

#define PROC_OPCODE_JLT (0x18)
#define PROC_OPCODE_JLTI (0x19)
#define PROC_OPCODE_BLT (0x1A)
#define PROC_OPCODE_BLTI (0x1B)

#define PROC_OPCODE_MOVZ (0x1C)
#define PROC_OPCODE_MOVLT (0x1D)

#define PROC_OPCODE_AND (0x1E)
#define PROC_OPCODE_ANDI (0x1F)
#define PROC_OPCODE_OR (0x20)
#define PROC_OPCODE_ORI (0x21)
#define PROC_OPCODE_INV (0x22)
#define PROC_OPCODE_XOR (0x23)
#define PROC_OPCODE_XORI (0x24)
#define PROC_OPCODE_LOADH (0x25)
#define PROC_OPCODE_LOADB (0x26)
#define PROC_OPCODE_STORH (0x27)
#define PROC_OPCODE_STORB (0x28)

#define PROC_OPCODE_SAR (0x29)
#define PROC_OPCODE_SLL (0x3A)
#define PROC_OPCODE_SLR (0x3B)
#define PROC_OPCODE_SARI (0x3C)
#define PROC_OPCODE_SLRI (0x3D)
#define PROC_OPCODE_DIV (0x3E)
#define PROC_OPCODE_DIVI (0x3F)
#define PROC_OPCODE_DIVUI (0x40)
#define PROC_OPCODE_MOVW (0x41)

#define PROC_OPCODE_BALI (0x42)
#define PROC_OPCODE_JAL (0x43)

extern register_map_t proc_regs;
extern byte_t* real_memory;

#define get_addr_in_ram(__addr__) \
        ((__addr__) >= ARCH_RAM_OFFSET && \
         (__addr__) < (ARCH_RAM_OFFSET + ARCH_RAM_SIZE))

#define get_addr_in_rom(__addr__) \
        ((__addr__) >= ARCH_ROM_OFFSET && \
         (__addr__) < (ARCH_ROM_OFFSET + ARCH_ROM_SIZE))

#define get_addr_in_real_mem(__addr__) \
        (get_addr_in_ram(__addr__) || get_addr_in_rom(__addr__))

/**
 * @brief Computes the real address within real_memory for the given emulated
 *        address.
 */
#define get_real_addr(__addr__) \
        (get_addr_in_ram(__addr__) ? \
         ((__addr__) - ARCH_RAM_OFFSET + ARCH_ROM_SIZE) : \
         get_addr_in_rom(__addr__) ? \
         ((__addr__) - ARCH_ROM_OFFSET) : \
         0)

#define get_mem_word(__addr__) \
        (*(get_addr_in_real_mem(__addr__) ? \
        ((word_t*)(intptr_t)(real_memory + get_real_addr(__addr__))) : \
        ((word_t*)(intptr_t)device_get_word(__addr__))))

#define get_mem_hword(__addr__) \
        (*(get_addr_in_real_mem(__addr__) ? \
        ((hword_t*)(intptr_t)(real_memory + get_real_addr(__addr__))) : \
        ((hword_t*)(intptr_t)device_get_hword(__addr__))))

#define get_mem_byte(__addr__) \
        (*(get_addr_in_real_mem(__addr__) ? \
        ((byte_t*)(intptr_t)(real_memory + get_real_addr(__addr__))) : \
        ((byte_t*)(intptr_t)device_get_byte(__addr__))))

#define proc_reg(__regidx__) \
        (*(((word_t*)(&proc_regs) + (__regidx__))))

#define proc_clear_alu_flags() \
        proc_regs.SR &= ~SR_ALU_FLAG_MASK

#define proc_sign_extend_imm(__imm__) \
        ((word_t)(((__imm__) & 0x00008000ul) ? \
                  (0xFFFF0000ul | (__imm__)) : (__imm__)))

#define proc_high_ones_mask(__num__) \
        (~proc_low_ones_mask(32 - (__num__)))

#define proc_low_ones_mask(__num__) \
        (0xFFFFFFFF >> (32 - (__num__)))

#endif
