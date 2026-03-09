#include "Eeprom.h"
#include "gd32f4xx.h"
#include <stdbool.h>
#include <stdint.h>

/* =========================
   硬件接口配置
   ========================= */

// SDA/SCL 引脚操作宏
#define SDA_HIGH()     gpio_bit_set(GPIOB, GPIO_PIN_7)
#define SDA_LOW()      gpio_bit_reset(GPIOB, GPIO_PIN_7)
#define SCL_HIGH()     gpio_bit_set(GPIOB, GPIO_PIN_6)
#define SCL_LOW()      gpio_bit_reset(GPIOB, GPIO_PIN_6)

// 读取 SDA 引脚电平
#define SDA_READ()     gpio_input_bit_get(GPIOB, GPIO_PIN_7)

/**
 * @brief 设置 SDA 为输出模式
 */
static void SDA_Output(void)
{
    // TODO: 配置 GPIOB PIN7 为推挽输出
}

/**
 * @brief 设置 SDA 为输入模式
 */
static void SDA_Input(void)
{
    // TODO: 配置 GPIOB PIN7 为浮空输入或上拉输入
}

/**
 * @brief I2C 延时函数，用于控制时序
 */
static void I2C_Delay(void)
{
    for (volatile int i = 0; i < 50; i++);
}

/* =========================
   I2C 基础操作函数
   ========================= */

/**
 * @brief 产生 I2C 起始信号
 */
static void I2C_Start(void)
{
    SDA_Output();
    SDA_HIGH();
    SCL_HIGH();
    I2C_Delay();
    SDA_LOW();  // SDA 下降沿触发起始信号
    I2C_Delay();
    SCL_LOW();
}

/**
 * @brief 产生 I2C 停止信号
 */
static void I2C_Stop(void)
{
    SDA_Output();
    SCL_LOW();
    SDA_LOW();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SDA_HIGH();  // SDA 上升沿触发停止信号
    I2C_Delay();
}

/**
 * @brief 等待从设备 ACK 响应
 * @return true: 收到 ACK, false: 超时或未 ACK
 */
static bool I2C_WaitAck(void)
{
    uint8_t timeout = 0;

    SDA_Input();
    SDA_HIGH();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();

    // 等待 SDA 拉低表示 ACK
    while (SDA_READ())
    {
        timeout++;
        if (timeout > 250)
        {
            I2C_Stop();  // 超时，发停止信号
            return false;
        }
    }

    SCL_LOW();
    return true;
}

/**
 * @brief 发送 ACK
 */
static void I2C_SendAck(void)
{
    SCL_LOW();
    SDA_Output();
    SDA_LOW();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
}

/**
 * @brief 发送 NACK
 */
static void I2C_SendNack(void)
{
    SCL_LOW();
    SDA_Output();
    SDA_HIGH();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
}

/**
 * @brief 发送一个字节到 I2C 总线
 * @param data 要发送的字节
 */
static void I2C_WriteByte(uint8_t data)
{
    SDA_Output();
    SCL_LOW();

    for (int i = 0; i < 8; i++)
    {
        if (data & 0x80)
            SDA_HIGH();
        else
            SDA_LOW();

        data <<= 1;

        I2C_Delay();
        SCL_HIGH();
        I2C_Delay();
        SCL_LOW();
    }
}

/**
 * @brief 从 I2C 总线读取一个字节
 * @return 读取到的字节
 */
static uint8_t I2C_ReadByte(void)
{
    uint8_t data = 0;
    SDA_Input();

    for (int i = 0; i < 8; i++)
    {
        SCL_LOW();
        I2C_Delay();
        SCL_HIGH();
        data <<= 1;
        if (SDA_READ())
            data |= 0x01;
        I2C_Delay();
    }

    SCL_LOW();
    return data;
}

/* =========================
   EEPROM 驱动函数
   ========================= */

#define EEPROM_ADDR  0xA0  // M24C02 芯片地址（写操作）

/**
 * @brief 初始化 EEPROM（硬件层），保持 SDA/SCL 高电平
 */
void EEPROM_Init(void)
{
    SDA_HIGH();
    SCL_HIGH();
}

/**
 * @brief 向 EEPROM 写入 1 个字节
 * @param addr EEPROM 内部地址
 * @param data 要写入的数据
 * @return true 写入成功, false 写入失败
 */
bool EEPROM_WriteByte(uint8_t addr, uint8_t data)
{
    I2C_Start();

    I2C_WriteByte(EEPROM_ADDR);        // 发送写命令
    if (!I2C_WaitAck()) return false;

    I2C_WriteByte(addr);               // 发送内部地址
    if (!I2C_WaitAck()) return false;

    I2C_WriteByte(data);               // 发送数据
    if (!I2C_WaitAck()) return false;

    I2C_Stop();

    // 等待 EEPROM 内部写周期（约 5ms）
    for (volatile int i = 0; i < 50000; i++);

    return true;
}

/**
 * @brief 从 EEPROM 读取 1 个字节
 * @param addr EEPROM 内部地址
 * @param data 读取结果指针
 * @return true 读取成功, false 读取失败
 */
bool EEPROM_ReadByte(uint8_t addr, uint8_t *data)
{
    I2C_Start();

    I2C_WriteByte(EEPROM_ADDR);        // 发送写命令，准备读地址
    if (!I2C_WaitAck()) return false;

    I2C_WriteByte(addr);               // 发送内部地址
    if (!I2C_WaitAck()) return false;

    I2C_Start();                       // 重复起始信号
    I2C_WriteByte(EEPROM_ADDR | 0x01); // 发送读命令
    if (!I2C_WaitAck()) return false;

    *data = I2C_ReadByte();            // 读取数据
    I2C_SendNack();                    // 最后一个字节 NACK
    I2C_Stop();

    return true;
}

/**
 * @brief 向 EEPROM 写入多字节数据
 * @param addr 起始地址
 * @param buf 数据缓冲区
 * @param len 数据长度
 * @return true 写入成功, false 写入失败
 */
bool EEPROM_WriteBuffer(uint8_t addr, uint8_t *buf, uint16_t len)
{
    while (len--)
    {
        if (!EEPROM_WriteByte(addr++, *buf++))
            return false;
    }
    return true;
}

/**
 * @brief 从 EEPROM 读取多字节数据
 * @param addr 起始地址
 * @param buf 数据缓冲区
 * @param len 数据长度
 * @return true 读取成功, false 读取失败
 */
bool EEPROM_ReadBuffer(uint8_t addr, uint8_t *buf, uint16_t len)
{
    while (len--)
    {
        if (!EEPROM_ReadByte(addr++, buf++))
            return false;
    }
    return true;
}