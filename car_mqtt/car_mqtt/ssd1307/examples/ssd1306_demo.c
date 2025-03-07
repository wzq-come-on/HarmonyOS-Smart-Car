/*
 * Copyright (c) 2020, HiHope Community.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_pwm.h"
#include "iot_i2c.h"
#include "iot_errno.h"

#include "ssd1306.h"
#include "ssd1306_tests.h"

#include "hi_io.h"
#include "../../car_test.h"
#define OLED_I2C_BAUDRATE 400*1000

void TestGetTick(void)
{
    for (int i = 0; i < 20; i++) {
        usleep(10*1000);
        //printf("HAL_GetTick(): %d\r\n", HAL_GetTick());
    }

    for (int i = 0; i < 20; i++) {
        HAL_Delay(25);
        //printf(" HAL_GetTick(): %d\r\n", HAL_GetTick());
    }
}

void Show_UpperHalf(char str[])
{
    ssd1306_FillUpperHalf(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_DrawString(str, Font_7x10, White);
    uint32_t start = HAL_GetTick();
    ssd1306_UpdateScreen();
    uint32_t end = HAL_GetTick();
}

void Show_LowerHalf(char str[])
{
    ssd1306_FillLowerHalf(Black);
    uint8_t screenHeight = SSD1306_HEIGHT;
    uint8_t startY = screenHeight / 2; 
    ssd1306_SetCursor(0, startY);
    ssd1306_DrawString(str, Font_7x10, White);
    uint32_t start = HAL_GetTick();
    ssd1306_UpdateScreen();
    uint32_t end = HAL_GetTick();
}

int width=0;

void DrawDashedLine(int x0, int y0, int x1, int y1, int dashLength) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int count = 0; // 计数器，用于控制虚线

    while (1) {
        // 绘制像素（如果计数器在绘制范围内）
        if (count < dashLength) {
            ssd1306_DrawPixel(x0, y0, White);
        }

        // 更新计数器
        count++;
        if (count >= dashLength * 2) {
            count = 0; // 重置计数器
        }

        // 判断是否到达终点
        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void FillGridCell(int row, int col, char text) {
    // 计算网格的起始位置
    int startX = col * width / 6;
    int startY = row * width / 6;
    int cellWidth = width / 6;
    int cellHeight = width / 6;

    // 稍微偏移以避免覆盖网格线
    startX += 2;
    startY += 2;
    cellWidth -= 4;
    cellHeight -= 4;

    if (text >= '1' && text <= '4') {
        switch (text) {
            case '1':
                // 绘制字符 1
                ssd1306_DrawLine(startX + cellWidth / 2, startY, startX + cellWidth / 2, startY + cellHeight, 1);
                break;
            case '2':
                // 绘制字符 2
                ssd1306_DrawLine(startX, startY, startX + cellWidth, startY, 1);
                ssd1306_DrawLine(startX + cellWidth, startY, startX + cellWidth, startY + cellHeight / 2, 1);
                ssd1306_DrawLine(startX + cellWidth, startY + cellHeight / 2, startX, startY + cellHeight / 2, 1);
                ssd1306_DrawLine(startX, startY + cellHeight / 2, startX, startY + cellHeight, 1);
                ssd1306_DrawLine(startX, startY + cellHeight, startX + cellWidth, startY + cellHeight, 1);
                break;
            case '3':
                // 绘制字符 3
                ssd1306_DrawLine(startX, startY, startX + cellWidth, startY, 1);
                ssd1306_DrawLine(startX + cellWidth, startY, startX + cellWidth, startY + cellHeight / 2, 1);
                ssd1306_DrawLine(startX + cellWidth, startY + cellHeight / 2, startX, startY + cellHeight / 2, 1);
                ssd1306_DrawLine(startX + cellWidth, startY + cellHeight / 2, startX + cellWidth, startY + cellHeight, 1);
                ssd1306_DrawLine(startX, startY + cellHeight, startX + cellWidth, startY + cellHeight, 1);
                break;
            case '4':
                // 绘制字符 4
                ssd1306_DrawLine(startX + cellWidth / 2, startY, startX + cellWidth / 2, startY + cellHeight, 1);
                ssd1306_DrawLine(startX + cellWidth / 2, startY, startX, startY + cellHeight / 2, 1);
                ssd1306_DrawLine(startX , startY + cellHeight/2+1, startX + cellWidth-1, startY + cellHeight/2+1, 1);
                break;
            default:
                break;
        }
    }
}

void DrawArrow(int row, int col, char direction) {
    // 计算网格的起始位置
    int startX = col * width / 6;
    int startY = row * width / 6;
    int cellWidth = width / 6;
    int cellHeight = width / 6;

    // 计算箭头的中心点
    int centerX = startX + cellWidth / 2;
    int centerY = startY + cellHeight / 2;

    // 根据方向绘制箭头
    switch (direction) {
        case 'U': // 上箭头
            ssd1306_DrawLine(centerX, startY + 2, centerX, startY + cellHeight - 2, White); // 主线
            ssd1306_DrawLine(centerX, startY + 2, centerX - 2, startY + 4, White); // 左斜线
            ssd1306_DrawLine(centerX, startY + 2, centerX + 2, startY + 4, White); // 右斜线
            break;

        case 'D': // 下箭头
            ssd1306_DrawLine(centerX, startY + 2, centerX, startY + cellHeight - 2, White); // 主线
            ssd1306_DrawLine(centerX, startY + cellHeight - 2, centerX - 2, startY + cellHeight - 4, White); // 左斜线
            ssd1306_DrawLine(centerX, startY + cellHeight - 2, centerX + 2, startY + cellHeight - 4, White); // 右斜线
            break;

        case 'L': // 左箭头
            ssd1306_DrawLine(startX + 2, centerY, startX + cellWidth - 2, centerY, White); // 主线
            ssd1306_DrawLine(startX + 2, centerY, startX + 4, centerY - 2, White); // 上斜线
            ssd1306_DrawLine(startX + 2, centerY, startX + 4, centerY + 2, White); // 下斜线
            break;

        case 'R': // 右箭头
            ssd1306_DrawLine(startX + 2, centerY, startX + cellWidth - 2, centerY, White); // 主线
            ssd1306_DrawLine(startX + cellWidth - 2, centerY, startX + cellWidth - 4, centerY - 2, White); // 上斜线
            ssd1306_DrawLine(startX + cellWidth - 2, centerY, startX + cellWidth - 4, centerY + 2, White); // 下斜线
            break;

        default:
            break;
    }
    uint32_t start = HAL_GetTick();
    ssd1306_UpdateScreen();
    uint32_t end = HAL_GetTick();
}

void DrawGrid() {
    // 清屏
    ssd1306_Fill(Black);
    
    FillGridCell(2, 2, '1');
    FillGridCell(2, 5, '2');
    FillGridCell(5, 2, '3');
    FillGridCell(5, 5, '4');
    // 绘制垂直虚线
    for (int x = 0; x <= width; x += width / 6) {
        DrawDashedLine(x, 0, x, width, 2); // 4个像素的虚线
    }

    // 绘制水平虚线
    for (int y = 0; y <= width; y += width / 6) {
        DrawDashedLine(0, y, width, y, 2); // 4个像素的虚线
    }

    // 更新屏幕
    uint32_t start = HAL_GetTick();
    ssd1306_UpdateScreen();
    uint32_t end = HAL_GetTick();
}

void Ssd1306TestTask(void* arg)
{
    (void) arg;
    IoTGpioInit(HI_IO_NAME_GPIO_13);
    IoTGpioInit(HI_IO_NAME_GPIO_14);

    hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
    hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);
    
    IoTI2cInit(0, OLED_I2C_BAUDRATE);

    //WatchDogDisable();

    usleep(20*1000);
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_DrawString("Hello HarmonyOS!", Font_7x10, White);

    uint32_t start = HAL_GetTick();
    ssd1306_UpdateScreen();
    uint32_t end = HAL_GetTick();
    printf("ssd1306_UpdateScreen time cost: %d ms.\r\n", end - start);

    // TestDrawChinese1();
    // TestDrawChinese2();
    // TestDrawChineseForward();

    width = ((int)(SSD1306_HEIGHT/6))*6;
    TestGetTick();
    int last_show_state=show_state;
    while (1) {
        // ssd1306_TestAll();
        // printf("ssd1307 state = %d\n",state);
        if(last_show_state!=show_state)
        {
            ssd1306_Fill(Black);
            last_show_state=show_state;
        }
        if(show_state==0)//状态显示
        {
            switch(state)
                {
                    case 0:
                        Show_UpperHalf("Hello HarmonyOS!");
                        break;
                    case 1:
                        Show_UpperHalf("Stop!");
                        break;
                    
                    case 2:
                        Show_UpperHalf("Forward!");
                        break;

                    case 3:
                        Show_UpperHalf("Backward!");
                        break;

                    case 4:
                        Show_UpperHalf("Left!");
                        break;

                    case 5:
                        Show_UpperHalf("Right!");
                        break;

                    default:
                        break;
                }

            switch(table_state)
                {
                    case -1:
                        Show_LowerHalf("No task!");
                        break;
                    
                    case 0:
                        Show_LowerHalf("Going to out food!");
                        break;
                    
                    case 1:
                        Show_LowerHalf("Going to tabel 1");
                        break;

                    case 2:
                        Show_LowerHalf("Going to tabel 2");
                        break;

                    case 3:
                        Show_LowerHalf("Going to tabel 3");
                        break;

                    case 4:
                        Show_LowerHalf("Going to tabel 4");
                        break;

                    default:
                        break;
                }
        }
        else{//地图显示
            DrawGrid();
            char dir='U';
            if(pos_state<=36)
            {
                dir='U';
            }else if(pos_state<=72)
            {
                dir='D';
            }else if(pos_state<=108)
            {
                dir='L';
            }else{
                dir='R';
            }
            //printf("Now poi:%d\n",pos_state);
            int pp=pos_state%36;
            if(pp==0)pp=36;
            int x=(pp-1)/6;
            int y=pp%6;
            if(y==0)y=6;
            y--;
            //printf("(%d,%d) %c\n",x,y,dir);
            DrawArrow(x, y, dir);
        }   
        usleep(10000);
    }
}

void Ssd1306TestDemo(void)
{
    printf("Ssd1306TestDemo start\n");

    osThreadAttr_t attr;

    attr.name = "Ssd1306Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;

    if (osThreadNew(Ssd1306TestTask, NULL, &attr) == NULL) printf("[Ssd1306TestDemo] Falied to create Ssd1306TestTask!\n");{
        
    }
}
APP_FEATURE_INIT(Ssd1306TestDemo);