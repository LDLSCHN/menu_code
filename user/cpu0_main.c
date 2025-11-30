/*********************************************************************************************************************
* TC264 Opensource Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "SysSe/Math/Ifx_FftF32.h"
#include "MYHEADFILE.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    Menu_Init();
    Beep_Init();                            //蜂鸣器初始化
    MyEncoder_Init();                       //旋转编码器初始化
    // Motor_Init();                           //电机初始化
    // Steer_Init();                           //舵机初始化
    PID_param_init();                       //PID参数初始化
    my_key_init(20);                        //按键扫描输出花
    pit_ms_init(CCU60_CH0, 20);              //中断初始化
     MyCamera_Init();
    pit_ms_init(CCU60_CH1, 5);              //中断初始化
    // pit_ms_init(CCU61_CH0, 5);              //电机控制
    // pit_ms_init(CCU61_CH1, 5);             //舵机控制

    ips200_clear();
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        menu_task();
        system_delay_ms(5);
//        printf("%d\r\n", is_long_straight);
//        if(is_long_straight)
//        {
//            Beep_Start();
//        }
//        else
//        {
//            Beep_Stop();
//        }
//        printf("%d,%d, %f, %d,%d, %f\r\n",Speed_Goal_r,Speed_Encoder_r, Motor_Speed_PID_l.I_Out,Speed_Goal_l,Speed_Encoder_l, Motor_Speed_PID_r.I_Out);
//        Menu_Event_Flush();
//        if(Find_Border_Break(l_border, image_h - 2, hightest, &break_row_dummy, 1))
//        printf("%d\r\n", switch_encoder_num);
//        system_delay_ms(5);
//        if(roundabout_a_point_y > 0)
//            Beep_Start();
//        else
//            Beep_Stop();

        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
