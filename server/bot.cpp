#include "bot.h"

Bot::Bot() : Entity(20)
{
    bot_angle = rand() % 360 ;
    score = 0;
}
