#STATE MACHINE BUTTON STM32 LOW LEVEL

ideas 

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define DEBOUND_TIME    50
#define WAIT_3S         800

typedef enum
{
    NO_PRESS,
    PRESS,
    LONG_PRESS,
}button_state;

typedef struct 
{
    bool isPress;
    uint16_t timePress;
    button_state state;    
}button_t;

button_state readStateButton(button_t *button)
{
    button->state = NO_PRESS;
    while(!readStatePin())
    {
        button->timePress++;
        button->isPress = true;
        delay_ms(1);
    }
    if(button->isPress)
    {
        while(readStatePin())
        {
            if(button->timePress >= DEBOUND_TIME && button->timePress < WAIT_3S)
            {
                button->isPress = false;
                button->timePress = 0;
                button->state = PRESS;
                return button->state;
            }
            else if(button->timePress > WAIT_3S)
            {
                button->isPress = false;
                button->timePress = 0;
                button->state = LONG_PRESS;
                return button->state;
            }
        }
    }
    return button->state;
}


int main(void)
{
    button_t obj ={
        .isPress = false;
        .timePress = 0;
        .state = NO_PRESS;
    };
    
    button_state currentStateButton;

    while(1)
    {
        currentStateButton = readStateButton(&obj);
        switch(currentStateButton)
        {
            case PRESS:
            {
                printf("PRESS\n");
                // do anything
                break;
            }
            case LONG_PRESS:
            {
                printf("LONG PRESS\n");
                // do anything
                break;
            }
        }
    }
}
```
