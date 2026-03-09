#include "gd32f4xx.h"
#include "Random.h"

static uint32_t last_random = 0;

/* =========================
   TRNG 初始化
   ========================= */
void TRNG_Init(void)
{
    /* 1. 使能 TRNG 时钟 */
    rcu_periph_clock_enable(RCU_TRNG);

    /* 2. 使能 TRNG */
    TRNG_CTL |= TRNG_CTL_TRNGEN;

    /* 3. 丢弃第一个随机数（FIPS 要求） */
    while (!(TRNG_STAT & TRNG_STAT_DRDY));
    (void)TRNG_DATA;

    last_random = 0;
}

/* =========================
   获取一个真随机数
   ========================= */
uint32_t TRNG_GetRandom(void)
{
    uint32_t value;

    while (1)
    {
        /* 等待数据准备 */
        while (!(TRNG_STAT & TRNG_STAT_DRDY));

        /* 检查错误 */
        if (TRNG_STAT & TRNG_STAT_SEIF)
        {
            TRNG_STAT &= ~TRNG_STAT_SEIF;
            TRNG_CTL &= ~TRNG_CTL_TRNGEN;
            TRNG_CTL |= TRNG_CTL_TRNGEN;
            continue;
        }

        if (TRNG_STAT & TRNG_STAT_CEIF)
        {
            TRNG_STAT &= ~TRNG_STAT_CEIF;
            continue;
        }

        value = TRNG_DATA;

        /* FIPS 要求：不能与上一次相同 */
        if (value != last_random)
        {
            last_random = value;
            return value;
        }
    }
}