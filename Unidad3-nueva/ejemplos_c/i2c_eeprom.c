/**
 * @file i2c_eeprom.c
 * @brief Ejemplo de lectura/escritura en EEPROM I2C (AT24C64)
 */

#include "stm32f4xx.h"
#include <stdint.h>

#define EEPROM_ADDR 0x50

void I2C1_Init(void);
int I2C1_Start(void);
void I2C1_Stop(void);
int I2C1_SendAddress(uint8_t addr, uint8_t read);
int I2C1_WriteByte(uint8_t data);
int I2C1_ReadByte(uint8_t last);

int EEPROM_WriteByte(uint16_t mem_addr, uint8_t data);
int EEPROM_ReadByte(uint16_t mem_addr);

int main(void) {
    I2C1_Init();
    
    // Escribir dato en dirección 0x0000
    EEPROM_WriteByte(0x0000, 0xAB);
    
    // Leer dato de dirección 0x0000
    int data = EEPROM_ReadByte(0x0000);
    
    // Verificar
    if (data == 0xAB) {
        // Éxito: LED verde
    } else {
        // Error: LED rojo
    }
    
    while (1);
}

int EEPROM_WriteByte(uint16_t mem_addr, uint8_t data) {
    I2C1_Start();
    I2C1_SendAddress(EEPROM_ADDR, 0);
    I2C1_WriteByte((mem_addr >> 8) & 0xFF);
    I2C1_WriteByte(mem_addr & 0xFF);
    I2C1_WriteByte(data);
    I2C1_Stop();
    
    // Esperar ciclo de escritura (~5ms)
    for (volatile int i = 0; i < 50000; i++);
    
    return 0;
}

int EEPROM_ReadByte(uint16_t mem_addr) {
    int data;
    
    I2C1_Start();
    I2C1_SendAddress(EEPROM_ADDR, 0);
    I2C1_WriteByte((mem_addr >> 8) & 0xFF);
    I2C1_WriteByte(mem_addr & 0xFF);
    
    I2C1_Start();  // Repeated START
    I2C1_SendAddress(EEPROM_ADDR, 1);
    data = I2C1_ReadByte(1);  // Último byte
    I2C1_Stop();
    
    return data;
}

void I2C1_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    
    // PB6=SCL, PB7=SDA
    GPIOB->MODER &= ~(0xF << (6*2));
    GPIOB->MODER |= (0xA << (6*2));
    GPIOB->OTYPER |= (0x3 << 6);  // Open-drain
    GPIOB->PUPDR |= (0x5 << (6*2));  // Pull-up
    GPIOB->AFR[0] &= ~(0xFF << (6*4));
    GPIOB->AFR[0] |= (0x44 << (6*4));  // AF4
    
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 42;
    I2C1->CCR = 210;
    I2C1->TRISE = 43;
    I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
}

int I2C1_Start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
    return 0;
}

void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

int I2C1_SendAddress(uint8_t addr, uint8_t read) {
    I2C1->DR = (addr << 1) | read;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR1;
    (void)I2C1->SR2;
    return 0;
}

int I2C1_WriteByte(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
    return 0;
}

int I2C1_ReadByte(uint8_t last) {
    if (last) {
        I2C1->CR1 &= ~I2C_CR1_ACK;
    } else {
        I2C1->CR1 |= I2C_CR1_ACK;
    }
    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    return I2C1->DR;
}
