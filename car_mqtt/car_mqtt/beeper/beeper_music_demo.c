#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "hi_pwm.h"
#include "../car_test.h"

int song_turn = 0;  // 控制放歌开关
int song = 1;       // 控制曲目

// 音符对应的分频系数（40MHz 时钟）
static const uint16_t g_tuneFreqs[] = {
    0,       // 占位符
    38223,   // 1046.5 Hz (C6)
    34052,   // 1174.7 Hz (D6)
    30338,   // 1318.5 Hz (E6)
    28635,   // 1396.9 Hz (F6)
    25511,   // 1568 Hz   (G6)
    22728,   // 1760 Hz   (A6)
    20249,   // 1975.5 Hz (B6)
    51021    // 783.99 Hz (G5)
};

// 曲谱音符（《两只老虎》）
static const uint8_t g_scoreNotes1[] = {
    1, 2, 3, 1,        1, 2, 3, 1,        3, 4, 5,  3, 4, 5,
    5, 6, 5, 4, 3, 1,  5, 6, 5, 4, 3, 1,  1, 8, 1,  1, 8, 1,
};

// 曲谱时值（单位：125ms）
static const uint8_t g_scoreDurations1[] = {
    4, 4, 4, 4,        4, 4, 4, 4,        4, 4, 8,  4, 4, 8,
    3, 1, 3, 1, 4, 4,  3, 1, 3, 1, 4, 4,  4, 4, 8,  4, 4, 8,
};

// 《七里香》副歌
static const uint8_t g_scoreNotes2[] = {
    6, 5, 3, 5, 6, 6, 6, 5, 3, 5, 6, 6, 6, 5, 3, 5,
    6, 6, 6, 5, 3, 5, 6, 6, 6, 5, 3, 5, 6, 6, 6, 5,
    3, 5, 6, 6, 6, 5, 3, 5, 6, 6, 6, 5, 3, 5, 6, 6,
    6, 5, 3, 5, 6, 6, 6, 5, 3, 5, 6, 6, 6, 5, 3, 5
};

// 假设每个音符的时值都为 125ms 的 2 倍（即 250ms）
static const uint8_t g_scoreDurations2[] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

// 《Ging Gang Goolie》副歌
static const uint8_t g_scoreNotes3[] = {
    5, 5, 6, 6, 5, 5, 3,  5, 5, 6, 6, 5, 5, 3,
    5, 5, 6, 6, 5, 5, 3,  5, 5, 6, 6, 5, 5, 3
};

// 每个音符的时值为 125ms 的 4 倍（即 500ms）
static const uint8_t g_scoreDurations3[] = {
    4, 4, 4, 4, 4, 4, 4,  4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4,  4, 4, 4, 4, 4, 4, 4
};

// 曲目数组
static const uint8_t *g_scoreNotesArray[] = {g_scoreNotes1, g_scoreNotes2, g_scoreNotes3};
static const uint8_t *g_scoreDurationsArray[] = {g_scoreDurations1, g_scoreDurations2, g_scoreDurations3};

// 音乐播放任务
static void *BeeperMusicTask(const char *arg)
{
    (void)arg;

    printf("BeeperMusicTask start!\r\n");

    // 设置 PWM 时钟源为晶体时钟（40MHz）
    hi_pwm_set_clock(PWM_CLK_XTAL);
    int last_song = song;
    while (1) {
        if (song_turn) {  // 检查开关状态
            // 根据 song 变量选择曲目
            const uint8_t *scoreNotes = g_scoreNotesArray[song-1];
            const uint8_t *scoreDurations = g_scoreDurationsArray[song-1];
            size_t scoreLength = sizeof(*scoreNotes) / sizeof(scoreNotes[0]);

            // 遍历曲谱，播放每个音符
            int len=0;
            if(song==1)len=32;
            else if(song==2)len=64;
            else len=28;
            printf("Song begin! Song:%d\r\n",song);
            for (size_t i = 0; i < len; i++) {
                if (!song_turn) {  // 检查开关状态
                    break;
                }
                if(last_song!=song)
                {
                    last_song=song;
                    break;
                }
                uint32_t tune = scoreNotes[i];              // 当前音符
                uint16_t freqDivisor = g_tuneFreqs[tune];    // 分频系数
                uint32_t tuneInterval = scoreDurations[i] * (125 * 1000); // 音符时长（微秒）

                // 打印调试信息
                //printf("Sign:%d ,Note: %d, Frequency: %d Hz, Duration: %d us\r\n",i, tune, (40 * 1000 * 1000) / freqDivisor, tuneInterval);

                // 启动 PWM，播放音符
                hi_pwm_start(PWM_PORT_PWM0, freqDivisor / 2, freqDivisor);
                usleep(tuneInterval); // 保持音符时长
                hi_pwm_stop(PWM_PORT_PWM0); // 停止 PWM
            }
        }
        if(song_turn)("Finish one turn!\r\n");
        sleep(1); // 延迟一段时间，避免CPU占用过高
    }

    printf("BeeperMusicTask finished!\r\n");
    return NULL;
}

// 启动音乐播放任务
static void StartBeepMusicTask(void)
{
    osThreadAttr_t attr;

    // 初始化 PWM
    hi_pwm_init(PWM_PORT_PWM0);

    // 配置任务属性
    attr.name = "BeeperMusicTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024; // 任务栈大小
    attr.priority = osPriorityNormal; // 任务优先级

    // 创建任务
    if (osThreadNew((osThreadFunc_t)BeeperMusicTask, NULL, &attr) == NULL) {
        printf("Failed to create BeeperMusicTask!\n");
    }
}

// 注册任务
SYS_RUN(StartBeepMusicTask);