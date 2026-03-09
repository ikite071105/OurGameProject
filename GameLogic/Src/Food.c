#include "Food.h"
#include "Snake.h"
#include <stdlib.h>
#include "Random.h"

struct FOOD food;

/* =========================
   初始化
   ========================= */
void food_init(void)
{
    TRNG_Init();
    food_spawn();
}

/* =========================
   生成食物（真随机）
   ========================= */
void food_spawn(void)
{
    int valid = 0;

    while (!valid)
    {
        valid = 1;

        food.x = TRNG_GetRandom() % LENGTH;
        food.y = TRNG_GetRandom() % WIDTH;

        for (int i = 0; i < snake.size; i++)
        {
            if (food.x == snake.body[i].x &&
                food.y == snake.body[i].y)
            {
                valid = 0;
                break;
            }
        }
    }

    uint32_t p = TRNG_GetRandom() % 100;

    if (p < 60)
    {
        food.value = 1;
        draw_food(map_x(food.x), map_y(food.y), 1);
    }
    else if (p < 90)
    {
        food.value = 2;
        draw_food(map_x(food.x), map_y(food.y), 2);
    }
    else
    {
        food.value = 5;
        draw_food(map_x(food.x), map_y(food.y), 3);
    }
}