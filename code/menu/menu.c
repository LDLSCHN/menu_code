#include "menu.h"

uint8 menu_key_event = menu_release;
// uint8 menu_update_flag = 0;
// uint8 menu_wait_flag = 0;
// static uint8 menu_cfg_flag = 0;
// static uint8 menu_global_line = 0;
// static uint8 last_menu_global_line = 0;
// static uint8 menu_global_line_buff_flag = 0;
// MenuPage_Linked_List *menu_head_page_node = NULL; // 菜单链表头指针

// /*
// {"Test1", FLOAT_VALUE_SHOW_TYPE, .line_extends.float_value_show_line.show_value = &Test,.display_line_count = 0},
// {"Test2", INT_VALUE_SHOW_TYPE, .line_extends.int_value_show_line.show_value = &int_val,.display_line_count = 0},
// {"Beep", ENTER_FUNC_RUN_TYPE, .action.void_func=MENUT, .display_line_count = 0},
// {"TEST", STATIC_FUNC_RUN_TYPE, .action.void_func=MENUTE,.display_line_count = 0},
// {"EDIT", FLOAT_VALUE_EDIT_TYPE, .action.float_float_func = menu_Val_CFG,.line_extends.float_value_edit_line.edit_value = &Test,.line_extends.float_value_edit_line.basic_val = 1,.display_line_count = 0},
// {"EDIT", CONFIG_VALUE_SHOW_TYPE, .line_extends.config_value_show_line.show_value = &Test,.display_line_count = 0},
// {"TEST", PAGE_JUMP_TYPE, .action.void_func=Test_Page_Init,.display_line_count = 0},
//  */


// void Bin_Image_Page_Init(void)
// {
//     static MenuLine  LineList[] = {
//             MENU_ITEM_STATIC_FUNC(" ", Bin_Image_Show, -1),
//             MENU_ITEM_FLOAT_SHOW("Steer_err", &ImageStatus.Steer_Center_error, 7),
//             MENU_ITEM_FLOAT_SHOW("Motor_err", &ImageStatus.Motor_Center_error, 0),
//             MENU_ITEM_INT_SHOW("L_roundabout_state",(int16*)&L_roundabout_state, 0),
//             MENU_ITEM_INT_SHOW("R_roundabout_state",(int16*)&R_roundabout_state, 0),
// //            MENU_ITEM_INT_EDIT("threshold1", &threshold1, 1, 0, 0),
// //            MENU_ITEM_INT_EDIT("Vy", (int16*)&Vy, 1, 0, 0),
// //            MENU_ITEM_INT_EDIT("P_line", (int16*)&P_line, 1, 0, 0),
// //            MENU_ITEM_INT_SHOW("round_vy",(int16*)&roundabout_v_point_y, 0),
// //            MENU_ITEM_FLOAT_SHOW("Lost",&lost_ratio, 0),
//             MENU_ITEM_FLOAT_SHOW("R2", &r_squared, 0),
//             MENU_ITEM_INT_SHOW("cross_state",(int16*)&cross_state, 0),
// //            MENU_ITEM_INT_SHOW("round_vp",(int16*)&roundabout_p_point_y, 0),
// //            MENU_ITEM_INT_SHOW("round_va",(int16*)&roundabout_a_point_y, 0),
//             MENU_ITEM_INT_SHOW("is_left_continuous",&is_left_continuous, 0),
//             MENU_ITEM_INT_SHOW("is_right_continuous",&is_right_continuous, 0),
//             MENU_ITEM_FLOAT_SHOW("variance", &variance, 0),

// //            MENU_ITEM_PAGE_JUMP("Camera",Camera_Page_Init, 0),
//             {".",  }

//     };
//     static MenuPage Page = {"Bin_Image", .line = LineList, .open_status = 0} ;

//     Menu_Push_Node(&Page);
// }

// void PID_Page_Init(void)
// {
//     static MenuLine LineList[] = {
//         MENU_ITEM_FLOAT_EDIT("Steer_Kp", &Steer_PID.Kp, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Steer_Ki", &Steer_PID.Ki, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Steer_Kd", &Steer_PID.Kd, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Motor_Kp_l", &Motor_Speed_PID_l.Kp, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Motor_Ki_l", &Motor_Speed_PID_l.Ki, 1.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Motor_Kd_l", &Motor_Speed_PID_l.Kd, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Motor_Kp_r", &Motor_Speed_PID_r.Kp, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Motor_Ki_r", &Motor_Speed_PID_r.Ki, 1.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Motor_Kd_r", &Motor_Speed_PID_r.Kd, 10.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Turn_Kp", &Turn_PID.Kp, 1.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Turn_Ki", &Turn_PID.Ki, 1.0f, 0),
//         MENU_ITEM_FLOAT_EDIT("Turn_Kd", &Turn_PID.Kd, 1.0f, 0),
//         {".", }
//     };
//     static MenuPage Page = {"PID", .line = LineList, .open_status = 0};
//     Menu_Push_Node(&Page);
// }

// //void Turn_PID_Page_Init(void)
// //{
// //    static MenuLine LineList[] = {
// //        MENU_ITEM_FLOAT_EDIT("Kp", &Turn_PID.Kp, 1.0f, 0),
// //        MENU_ITEM_FLOAT_EDIT("Ki", &Turn_PID.Ki, 0.1f, 0),
// //        MENU_ITEM_FLOAT_EDIT("Kd", &Turn_PID.Kd, 0.1f, 0),
// //        {".", }
// //    };
// //    static MenuPage Page = {"Turn PID", .line = LineList, .open_status = 0};
// //    Menu_Push_Node(&Page);
// //}

// //参数调节主页面
// void Param_Tuning_Page_Init(void)
// {
//     static MenuLine LineList[] = {
//         // --- PID调节 ---
// //        MENU_ITEM_PAGE_JUMP("Steer PID", Steer_PID_Page_Init, 0),
// //        MENU_ITEM_PAGE_JUMP("Turn PID", Turn_PID_Page_Init, 0),

//         // --- 图像参数调节 ---
//         // base_val=1, son_start_line=1 表示最小步长为1，默认选中x1步长
//         MENU_ITEM_INT_EDIT("S_Fsight", &ImageStatus.Steer_TowPoint, 1, 1, 1),
//         MENU_ITEM_INT_EDIT("M_Fsight", &ImageStatus.Motor_TowPoint, 1, 1, 1),
//         MENU_ITEM_INT_EDIT("set_speed", &set_speed, 1, 1, 1),

//         {".", }
//     };
//     static MenuPage Page = {"Parameter Tuning", .line = LineList, .open_status = 0};
//     Menu_Push_Node(&Page);
// }
// void Motor_Page_Init(void)
// {
//     static MenuLine LineList[] = {
//             MENU_ITEM_INT_SHOW("Speed_Goal_l", &Speed_Goal_l, 0),
//             MENU_ITEM_INT_SHOW("Speed_Goal_r", &Speed_Goal_r, 0),
//             MENU_ITEM_INT_SHOW("Encoder L", &Speed_Encoder_l, 0),
//             MENU_ITEM_INT_SHOW("Encoder R", &Speed_Encoder_r, 0),
//             MENU_ITEM_INT_SHOW("PWM_L", &Speed_PID_OUT_l, 0),
//             MENU_ITEM_INT_SHOW("PWM_R", &Speed_PID_OUT_r, 0),


//         {".", }
//     };
//     static MenuPage Page = {"Motor", .line = LineList, .open_status = 0};
//     Menu_Push_Node(&Page);
// }

// void element_Page_Init(void)
// {
//     static MenuLine LineList[] = {
// //            MENU_ITEM_INT_EDIT("R_round", (int16*)&R_round_status, 1, 2, 0),
// //            MENU_ITEM_INT_EDIT("L_round", (int16*)&L_round_status, 1, 2, 0),
//             MENU_ITEM_CONFIG_SHOW("R_round",&R_round_status, 0),
//             MENU_ITEM_CONFIG_SHOW("L_round",&L_round_status, 0),
//             MENU_ITEM_INT_SHOW("long_st", &is_long_straight, 0),
//         {".", }
//     };
//     static MenuPage Page = {"element", .line = LineList, .open_status = 0};
//     Menu_Push_Node(&Page);
// }

// /***********************************************
// * @brief : 主页面初始化
// * @param : void
// * @return: void
// * @date  : 2025年3月20日14:26:28
// * @author: SJX
// ************************************************/
// void Main_Page_Init(void)
// {
//     static MenuLine  LineList[] = {
//             MENU_ITEM_STATIC_FUNC(" ", Image_Show, -1),
//             MENU_ITEM_PAGE_JUMP("Tune Params", Param_Tuning_Page_Init, 7),
//             MENU_ITEM_PAGE_JUMP("Bin_Image",Bin_Image_Page_Init, 0),
//             MENU_ITEM_PAGE_JUMP("PID", PID_Page_Init, 0),
//             MENU_ITEM_PAGE_JUMP("Motor",Motor_Page_Init, 0),
//             MENU_ITEM_PAGE_JUMP("element",element_Page_Init, 0),
//             MENU_ITEM_INT_EDIT("Beep", (int16*)&P, 1, 2, 0),
// //            MENU_ITEM_INT_EDIT("steer_debug_pwm", (int16*)&steer_debug_pwm, 10, 2, 0),
// //            MENU_ITEM_FLOAT_SHOW("Center_err", &ImageStatus.Center_error, 0),
// //            MENU_ITEM_INT_SHOW("Steer_stat",(int16*)&Steer_status,0),
//             MENU_ITEM_INT_SHOW("Car_stat",(int16*)&g_Car_Status,0),
//             MENU_ITEM_INT_EDIT("cross_status", (int16*)&cross_status, 1, 2, 0),
// //            MENU_ITEM_INT_SHOW("left_continuous", (int16*)&is_left_continuous, 0),
// //            MENU_ITEM_INT_SHOW("right_continuous", (int16*)&is_right_continuous, 0),
// //            MENU_ITEM_INT_SHOW("roundabout_state",(int16*)&roundabout_state,0),
// //            MENU_ITEM_FLOAT_SHOW("R2", &r_squared, 0),
// //            MENU_ITEM_INT_EDIT("X",(int16*)&x,1,2,0),
// //            MENU_ITEM_INT_EDIT("Y",(int16*)&y,1,2,0),
// //            MENU_ITEM_INT_SHOW("Speed_Goal_l", &Speed_Goal_l, 0),
// //            MENU_ITEM_INT_SHOW("Speed_Goal_r", &Speed_Goal_r, 0),
// //            MENU_ITEM_INT_SHOW("Encoder L", &Speed_Encoder_l, 0),
// //            MENU_ITEM_INT_SHOW("Encoder R", &Speed_Encoder_r, 0),
// //            MENU_ITEM_INT_SHOW("PWM_L", &Speed_PID_OUT_l, 0),
// //            MENU_ITEM_INT_SHOW("PWM_R", &Speed_PID_OUT_r, 0),
// //            MENU_ITEM_PAGE_JUMP("Camera",Camera_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Camera",Camera_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Camera",Camera_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Camera",Camera_Page_Init, 0),

// //            MENU_ITEM_PAGE_JUMP("Start",Start_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Config",Config_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Value",Value_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Data",Data_Page_Init, 1),
// //            MENU_ITEM_PAGE_JUMP("Speed",Speed_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Element",element_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Element_CFG",element_Config_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Leg",leg_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Motor",Motor_Page_Init, 0),
// //
// ////            MENU_ITEM_PAGE_JUMP("UB",UB_Page_Init, 1),
// //            MENU_ITEM_PAGE_JUMP("Preset",Preset_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Race_Preset",Race_Preset_Page_Init, 0),
// //            MENU_ITEM_PAGE_JUMP("Race_Time",Race_Time_Page_Init, 0),
//             {".",  }
//     };
//     static MenuPage Page = {"Main", .line = LineList, .open_status = 0} ;

//     Menu_Push_Node(&Page);
// }

// /***********************************************
// * @brief : 菜单初始化
// * @param : void
// * @return: page
// * @date  : 2025年3月20日16:32:05
// * @author: SJX
// ************************************************/
// void Menu_Init(void)
// {
//     ips200_init(IPS200_TYPE_SPI);           //屏幕初始化
//     Main_Page_Init();
// //    Camera_Page_Init();
// }

// /***********************************************
// * @brief : 菜单主函数运行
// * @param : void
// * @return: void
// * @date  : 2025年3月20日18:58:35
// * @author: SJX
// ************************************************/
// void MENU_RUN(void)
// {
//     static uint8 i = 0;

//     my_assert(menu_head_page_node != NULL);
//     Menu_Key_Process();
//     if(menu_head_page_node->page->open_status != 1)
//     {
//         Menu_Get_Page_LineNumMAX(menu_head_page_node->page);
//         menu_global_line = menu_head_page_node->page->line_num;
// //        menu_global_line = menu_head_page_node->page->line_num;
//         My_Show_String_Centered(0, menu_head_page_node->page->page_name);
// //        Menu_Val_CFG_Limit(&menu_global_line, menu_head_page_node->page->line_num_max);
//     }

//     static uint16 use_start_line = 0;
//     if(menu_global_line_buff_flag == 1 || menu_key_event != menu_release || menu_head_page_node->page->open_status == 0 || menu_update_flag == 1)
//     {
//         if(menu_key_event != menu_release )   menu_key_event = menu_release ;
//         if(menu_global_line_buff_flag == 1)  menu_global_line_buff_flag = 0 ;
//         if(menu_head_page_node->page->open_status == 0)  menu_head_page_node->page->open_status = 1;
//         if(menu_update_flag == 1)  menu_update_flag = 0;

//         for(i = 0; i < (menu_head_page_node->page->line_num_max); i++)
//         {
//             if(menu_head_page_node->page->line[i].display_line_count != 0)
//             {
//                 use_start_line += menu_head_page_node->page->line[i].display_line_count;
//             }

//             if(menu_head_page_node->page->line[i].line_name != 0)
//                 My_Show_String_Color(0, (use_start_line + (i+1)) * 18, menu_head_page_node->page->line[i].line_name,(i+1)==menu_global_line?SELECTED_BRUSH:DEFAULT_BRUSH);
//             switch(menu_head_page_node->page->line[i].line_type)
//             {
//                 case CONFIG_VALUE_SHOW_TYPE:
//                 {
//                     if(menu_head_page_node->page->line[i].line_extends.config_value_show_line.show_value != NULL)
//                     {
//                         if(*(menu_head_page_node->page->line[i].line_extends.config_value_show_line.show_value) == 0)
//                             My_Show_String_Color(180, (use_start_line + (i+1)) * 18, "Close", (i+1)==menu_global_line?SELECTED_BRUSH:DEFAULT_BRUSH);
//                         else if(*(menu_head_page_node->page->line[i].line_extends.config_value_show_line.show_value) == 1)
//                             My_Show_String_Color(180, (use_start_line + (i+1)) * 18, "Open ", (i+1)==menu_global_line?SELECTED_BRUSH:DEFAULT_BRUSH);
//                         else
//                             my_assert(0);                   //配置变量传入open || close以外的值
//                     }
//                        break;
//                 }
//                 case FLOAT_VALUE_EDIT_TYPE:
//                 {
//                     if(menu_head_page_node->page->line[i].line_extends.float_value_edit_line.edit_value != NULL)
//                     {
//                         My_Show_Float_Color(120, (use_start_line + (i+1)) * 18, (*(menu_head_page_node->page->line[i].line_extends.float_value_edit_line.edit_value))+((*(menu_head_page_node->page->line[i].line_extends.float_value_edit_line.edit_value))>0?0.000001:-0.000001), 8, 5, (i+1)==menu_global_line?SELECTED_BRUSH:DEFAULT_BRUSH);
//                     }
//                     else
//                         My_Show_Float_Color(120, (use_start_line + (i+1)) * 18, +0.000001, 8, 5, DEFAULT_BRUSH);
//                     break;
//                 }
//                 case INT_VALUE_EDIT_TYPE:
//                 {
//                     if(menu_head_page_node->page->line[i].line_extends.int_value_edit_line.edit_value != NULL)
//                     {
//                         My_Show_Int_Color(120, (use_start_line + (i+1)) * 18, (*(menu_head_page_node->page->line[i].line_extends.int_value_edit_line.edit_value)), 10,  (i+1)==menu_global_line?SELECTED_BRUSH:DEFAULT_BRUSH);
//                     }
//                     else
//                         My_Show_Int_Color(120, (use_start_line + (i+1)) * 18, 0, 0, DEFAULT_BRUSH);
//                 }
//                    break;
//                 default:break;
//             }
//         }
//     }
//     use_start_line = 0;
//     for(i = 0; i < (menu_head_page_node->page->line_num_max); i++)
//     {
//         if(menu_head_page_node->page->line[i].display_line_count != 0)
//         {
//             use_start_line += menu_head_page_node->page->line[i].display_line_count;
//         }
//         switch(menu_head_page_node->page->line[i].line_type)
//         {
//             case FLOAT_VALUE_SHOW_TYPE:
//                 if(menu_head_page_node->page->line[i].line_extends.float_value_show_line.show_value != NULL)
//                 {
//                     My_Show_Float_Color(120, (use_start_line + (i+1)) * 18, (*(menu_head_page_node->page->line[i].line_extends.float_value_show_line.show_value))+((*(menu_head_page_node->page->line[i].line_extends.float_value_show_line.show_value))>0?0.000001:-0.000001), 8, 5, DEFAULT_BRUSH);
//                 }
//                 else
//                     My_Show_Float_Color(120, (use_start_line + (i+1)) * 18, +0.00001, 8, 5, DEFAULT_BRUSH);
//                    break;
//             case INT_VALUE_SHOW_TYPE:
//                 if(menu_head_page_node->page->line[i].line_extends.int_value_show_line.show_value != NULL)
//                 {
//                     My_Show_Int_Color(120, (use_start_line + (i+1)) * 18, (*(menu_head_page_node->page->line[i].line_extends.int_value_show_line.show_value)), 10,  DEFAULT_BRUSH);
//                 }
//                 else
//                     My_Show_Int_Color(120, (use_start_line + (i+1)) * 18, 0, 0, DEFAULT_BRUSH);
//                    break;
//             case STATIC_FUNC_RUN_TYPE:
//             {
//                 if(menu_head_page_node->page->line[i].action.void_func)
//                 {
//                     menu_head_page_node->page->line[i].action.void_func();
//                 }
//                 else
//                     my_assert(0);       //不传函数执行牛魔
//                    break;
//             }
//             default:break;
//         }
//     }
//     use_start_line = 0;
// //    printf("%d,%d\r\n", menu_global_line, i);
// }



// /***********************************************
// * @brief : 按键刷新菜单动作
// * @param : void
// * @return: void
// * @date  : 2025年3月20日14:26:28
// * @author: SJX
// ************************************************/
// void Menu_Event_Flush(void)
// {
// //    if(my_key_get_state(MENU_KEY) == MY_KEY_RELEASE)
// //    {
// //        menu_key_event = menu_release;
// //    }
//     if(If_Switch_Encoder_Change() == 1)
//     {
//         if(menu_cfg_flag == 0)
//         {
//             menu_global_line += switch_encoder_change_num;
//             Menu_Val_CFG_Limit(&menu_global_line, menu_head_page_node->page->line_num_max);
//             if(menu_global_line != last_menu_global_line)
//                 menu_global_line_buff_flag = 1;
// //            else;
// //                menu_global_line_buff_flag = 0;
//         }
//     }

// //    if(menu_wait_flag == 1)
// //        return;
//     if(my_key_get_state(MENU_KEY) == MY_KEY_SHORT_PRESS)
//     {
//         menu_key_event = menu_yes;
//     }
//     else if(my_key_get_state(MENU_KEY) == MY_KEY_LONG_PRESS)
//     {
//         menu_key_event = menu_back;
//     }
// //    else if(my_key_get_state(MENU_FAST_KEY) == MY_KEY_LONG_PRESS)
// //    {
// //        menu_key_event = menu_goto_camera;
// //    }

// }
// /***********************************************
// * @brief : 菜单按键处理
// * @param : void
// * @return: void
// * @date  : 2025年3月20日14:29:25
// * @author: SJX
// ************************************************/
// void Menu_Key_Process(void)
// {
//     my_assert(menu_head_page_node != NULL);

//     // 处理按键事件
//     if(menu_key_event != menu_release)
//     {
//         switch (menu_key_event)
//         {
//             case menu_back:
//                 Menu_Pop_Node();
//                 break;
//             case menu_yes:      // 执行当前选中的行的操作
//             {
//                 switch (menu_head_page_node->page->line[menu_global_line-1].line_type)
//                 {
//                     case ENTER_FUNC_RUN_TYPE:
//                     {
//                         if(menu_head_page_node->page->line[menu_global_line-1].action.void_func)
//                         {
//                             menu_head_page_node->page->line[menu_global_line-1].action.void_func();
//                         }
//                         else
//                             my_assert(0);       //不传函数执行牛魔
//                            break;
//                     }
//                     case PAGE_JUMP_TYPE:
//                     {
//                         if(menu_head_page_node->page->line[menu_global_line-1].action.void_func)
//                         {
//                             menu_head_page_node->page->line[menu_global_line-1].action.void_func();
//                         }
//                         else
//                             my_assert(0);       //不传函数执行牛魔
//                            break;
//                     }
//                     case CONFIG_VALUE_SHOW_TYPE:
//                     {
//                         if(menu_head_page_node->page->line[menu_global_line-1].line_extends.config_value_show_line.show_value != NULL)
//                         {
//                             *(menu_head_page_node->page->line[menu_global_line-1].line_extends.config_value_show_line.show_value) =!*(menu_head_page_node->page->line[menu_global_line-1].line_extends.config_value_show_line.show_value);
// //                            Flash_WriteAllVal();
//                         }
//                         else
//                             my_assert(0);
//                            break;
//                     }
//                     case FLOAT_VALUE_EDIT_TYPE:
//                     {
//                         if(menu_head_page_node->page->line[menu_global_line-1].action.float_float_func)
//                         {
//                             menu_head_page_node->page->line[menu_global_line-1].action.float_float_func(
//                                 menu_head_page_node->page->line[menu_global_line-1].line_extends.float_value_edit_line.edit_value,
//                                 menu_head_page_node->page->line[menu_global_line-1].line_extends.float_value_edit_line.basic_val
//                             );
//                         }
//                         else
//                             my_assert(0);       //不传函数执行牛魔
//                            break;
//                     }
//                     case INT_VALUE_EDIT_TYPE:
//                     {
//                         if(menu_head_page_node->page->line[menu_global_line-1].action.int16_int16_int16_func)
//                         {
//                             menu_head_page_node->page->line[menu_global_line-1].action.int16_int16_int16_func(
//                                 menu_head_page_node->page->line[menu_global_line-1].line_extends.int_value_edit_line.edit_value,
//                                 menu_head_page_node->page->line[menu_global_line-1].line_extends.int_value_edit_line.int_basic_val,
//                                 menu_head_page_node->page->line[menu_global_line-1].line_extends.int_value_edit_line.son_start_line
//                             );
//                         }
//                         else
//                             my_assert(0);       //不传函数执行牛魔
//                            break;
//                     }
//                     default:break;
//                 }

//             }break;

//             case menu_goto_camera:
//             {
// //                Camera_Page_Init();
//             }break;

//             default:break;

//         }
//     }
// //    menu_key_event = menu_release;

// }



// /***********************************************
// * @brief : 菜单链表新加页
// * @param : void
// * @return: add_page
// * @date  : 2025年3月20日18:43:16
// * @author: SJX
// ************************************************/
// void Menu_Push_Node(MenuPage *new_page)
// {
//     My_Clear();
//     my_assert(new_page != NULL); // 先判断 new_page 是否为 NULL

//     MenuPage_Linked_List *q = (MenuPage_Linked_List *)malloc(sizeof(MenuPage_Linked_List));

//     my_assert(q != NULL);

//     q->page = new_page;
//     q->next = NULL;

//     if(menu_head_page_node == NULL)
//     {
//         menu_head_page_node = q;
//     }
//     else
//     {
//         menu_head_page_node->page->line_num = menu_global_line;
//         q->next = menu_head_page_node;
//         menu_head_page_node = q;
//     }
// }

// /***********************************************
// * @brief : 菜单链表删除结点
// * @param : void
// * @return: add_page
// * @date  : 2025年3月20日18:43:16
// * @author: SJX
// ************************************************/
// void Menu_Pop_Node(void)
// {
//     MenuPage_Linked_List *p;
//     my_assert(menu_head_page_node != NULL);

//     if(menu_head_page_node->next == NULL)
//         return;

//     My_Clear();

//     menu_head_page_node->page->open_status = 0;
//     menu_head_page_node->page->line_num = menu_global_line;

//     p = menu_head_page_node->next;
//     free(menu_head_page_node);

//     menu_head_page_node = p;

//     menu_head_page_node->page->open_status = 0;
// }


// /***********************************************
// * @brief : 获取页的行数
// * @param : void
// * @return: page
// * @date  : 2025年3月20日16:31:35
// * @author: SJX
// ************************************************/
// uint8 Menu_Get_Page_LineNumMAX(MenuPage* Page)
// {
//     uint8 i = 0;
//     if(Page->line_num_max != 0)
//     {
//         return Page->line_num_max;
//     }
//     else
//     {
//         Page->line_num = 1;
//         for(; Page->line[i].line_name[0] != '.'; i++);
//         Page->line_num_max = i;
//         return Page->line_num_max;
//     }
// }

// /***********************************************
// * @brief : 菜单参数配置子页面行数限幅函数
// * @param : *line              需要配置的行的地址，记得&
// *           line_max          最大行
// * @return: void
// * @date  : 2024.10.18
// * @author: SJX
// ************************************************/
// void Menu_Val_CFG_Limit(uint8 *line, uint8 line_max)
// {
//     if(*line > line_max)
//     {
//         *line = 1;
//     }
//     else if(*line < 1)
//     {
//         *line = line_max;
//     }
// }

// /***********************************************
// * @brief : 菜单刷新
// * @param : void
// * @return: void
// * @date  : 2024.10.18
// * @author: SJX
// ************************************************/
// void Menu_Page_Update(void)
// {
//     menu_update_flag = 1;
// }

// /*****************内部调用,用户无需在意,后续自行优化*******************/
// void menu_Val_CFG_clear(uint8 *page_start_row);
// void menu_Val_CFG_Flush(float *CFG_val, uint16 page_start_row,  bool temp_based_flag);
// void menu_Val_CFG_Arrow_Show(uint16 page_start_row,uint16 line_num);
// /*****************内部调用,用户无需在意,后续自行优化*******************/

// #define VAL_VFG_SHOW_NUM_BIT          6
// #define VAL_VFG_SHOW_POINT_BIT        5
// #define VAL_VFG_SHOW_TOTAL_LINE       6                     //总行数，包含变量固定行和几个数字行和exit行
// #define LINE_MAX                      VAL_VFG_SHOW_TOTAL_LINE-1
// uint8 up_down_flag = 0;         //调参菜单在上面还是在下面，默认为0在下面，为1在上面
// float basic_1, basic_10, basic_100 ;
// uint8 menu_Val_CFG_line;
// void menu_Val_CFG(float *CFG_val, float basic_val )
// {
//     menu_head_page_node->page->line_num = menu_global_line;
//     menu_cfg_flag = 1;
//     uint8 flush_flag;
//     uint8 CFG_stop_flag = 0;
//     menu_Val_CFG_line = 1;
//     bool based_flag = 0;                            //选中标志位，选中为1，不选中为0
//     basic_1 = basic_val;
//     basic_10 = basic_val / 10 ;
//     basic_100 = basic_val / 100;

//     uint8 page_start_row;

//     page_start_row = menu_global_line*18;
// //    printf("%d\r\n", page_start_row);
//     if(page_start_row > (319 - (10+18+ 18 * VAL_VFG_SHOW_TOTAL_LINE)))
//     {
//         up_down_flag = 1;
//     }
//     else up_down_flag = 0;
//     menu_Val_CFG_clear(&page_start_row);
//     ips200_set_color(RGB565_WHITE, RGB565_BLACK);
//     menu_Val_CFG_Flush(CFG_val, page_start_row, based_flag);
//     menu_key_event = menu_release;
//     while(CFG_stop_flag == 0)
//     {
// //        menu_key_event == menu_release?flush_flag = 0:flush_flag = 1;
//         if(menu_key_event == menu_release)
//         {
//             flush_flag = 0;
//         }
//         else
//             flush_flag = 1;
//         if(menu_key_event == menu_back)
//         {
//             menu_key_event = menu_release;
//             CFG_stop_flag = 1;
//             ips200_clear();
//             menu_cfg_flag = 0;
//             menu_head_page_node->page->open_status = 0;
// //            Flash_WriteAllVal();
//         }
//         if((flush_flag == 1 || switch_encoder_change_num != 0) && CFG_stop_flag == 0)
//         {
//             if(menu_key_event == menu_yes)
//             {
// //                Flash_WriteAllVal();
//                 menu_key_event = menu_release;
//                 switch(menu_Val_CFG_line)
//                 {
//                     case 1:
//                         based_flag = !based_flag;
//                         break;

//                     case 2:
//                         based_flag = !based_flag;
//                         break;

//                     case 3:
//                         based_flag = !based_flag;
//                         break;

//                     case 4:
//                         *CFG_val = 0;
//                         Beep_ShortRing();
//                         break;

//                     case 5:
//                         CFG_stop_flag = 1;
//                         menu_cfg_flag = 0;
//                         menu_head_page_node->page->open_status = 0;
//                         break;
//                 }
//             }
//             if(If_Switch_Encoder_Change() == 1)
//             {
//                 if(based_flag == 0)
//                 {
//                     menu_Val_CFG_line += switch_encoder_change_num;
//                     Menu_Val_CFG_Limit(&menu_Val_CFG_line, LINE_MAX);
//                 }
//                 else
//                 {
//                     switch (menu_Val_CFG_line)
//                    {
//                        case 1:
//                            *CFG_val += (basic_1 * switch_encoder_change_num);
//                            break;
//                        case 2:
//                            *CFG_val += basic_10 * switch_encoder_change_num;
//                            break;
//                        case 3:
//                            *CFG_val += basic_100 * switch_encoder_change_num;
//                            break;
//                    }
//                 }
//             }
//             menu_Val_CFG_Flush(CFG_val, page_start_row, based_flag);
// //            Beep_ShortRing();
//         }
//     }
//     ips200_clear();
// }

// /***********************************************
// * @brief : 菜单参数配置子页面清屏函数(置黑)并确定起始行
// * @param : page_start_row       起始行
// * @return: void
// * @date  : 2024.10.18
// * @author: SJX
// ************************************************/
// void menu_Val_CFG_clear(uint8 *page_start_row)
// {
//     uint16 i = 0;
//     if(up_down_flag == 0)
//     {

//         for(i = *page_start_row + 18; i < *page_start_row + 10+18 + VAL_VFG_SHOW_TOTAL_LINE * 18; i++)
//         {
//             ips200_draw_line(0, i, 239, i, RGB565_BLACK);
//         }
// //        IPS200_Full_Parts(0, 240, *page_start_row + 18, *page_start_row + 10+18 + VAL_VFG_SHOW_TOTAL_LINE * 18);
//         *page_start_row = *page_start_row + 5 + 18 * 1;

//     }
//     else if(up_down_flag == 1)
//     {
//         for(i = *page_start_row  ; i > *page_start_row -10 -18 * VAL_VFG_SHOW_TOTAL_LINE  ; i--)
//         {
//             ips200_draw_line(0, i, 239, i, RGB565_BLACK);
//         }
//         *page_start_row = *page_start_row -5 - 18 * VAL_VFG_SHOW_TOTAL_LINE;
//     }
// }

// /***********************************************
// * @brief : 菜单参数配置子页面变量刷新函数
// * @param : *CFG_val              需要配置的值的地址，记得&
// *           page_start_row       起始行
// * @return: void
// * @
// * @date  : 2024.10.18
// * @author: SJX
// ************************************************/
// void menu_Val_CFG_Flush(float *CFG_val, uint16 page_start_row,  bool temp_based_flag)
// {
//         ips200_show_float(0, page_start_row + 18*0, *CFG_val + ((*CFG_val)>0?0.000001:-0.000001), VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT);
//         menu_Val_CFG_Arrow_Show(page_start_row, menu_Val_CFG_line);
//         if(temp_based_flag == 1)
//         {
//             if(menu_Val_CFG_line == 1)  ips200_show_float_color(18, page_start_row + 18 * menu_Val_CFG_line, basic_1+ 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT,RGB565_RED);
//             else                ips200_show_float_color(18, page_start_row + 18 * 1, basic_1+ 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT,RGB565_WHITE);

//             if(menu_Val_CFG_line == 2)  ips200_show_float_color(18, page_start_row + 18 * menu_Val_CFG_line, basic_10+ 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT,RGB565_RED);
//             else                ips200_show_float_color(18, page_start_row + 18 * 2, basic_10+ 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT,RGB565_WHITE);

//             if(menu_Val_CFG_line == 3)  ips200_show_float_color(18, page_start_row + 18 * menu_Val_CFG_line, basic_100+ 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT,RGB565_RED);
//             else                ips200_show_float_color(18, page_start_row + 18 * 3, basic_100+ 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT,RGB565_WHITE);

//             if(menu_Val_CFG_line == 4)  ips200_show_string_color(18, page_start_row + 18 * menu_Val_CFG_line, "empty", RGB565_RED);
//             else                ips200_show_string_color(18, page_start_row + 18 * 4, "empty", RGB565_WHITE);

//             if(menu_Val_CFG_line == 5)  ips200_show_string_color(18, page_start_row + 18 * menu_Val_CFG_line, "exit", RGB565_RED);
//             else                ips200_show_string_color(18, page_start_row + 18 * 5, "  ", RGB565_WHITE);

//         }
//         else
//         {
//             ips200_show_float(18, page_start_row + 18*1, basic_1   + 0.0000001 , VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT);
//             ips200_show_float(18, page_start_row + 18*2, basic_10  + 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT);
//             ips200_show_float(18, page_start_row + 18*3, basic_100 + 0.0000001, VAL_VFG_SHOW_NUM_BIT, VAL_VFG_SHOW_POINT_BIT);
//             ips200_show_string(18, page_start_row + 18*5, "  ");
//             ips200_show_string(18, page_start_row + 18*4, "empty");
//         }

// //    uint8 i;
// //    i = Key_IfEnter();
// //    val_cfg_key_ifenter_flag += i;
//     key_clear_all_state();
// }

// /***********************************************
// * @brief : 菜单参数配置子页面箭头显示
// * @param : *line              需要配置的行的地址，记得&
// *           line_max          最大行
// * @return: void
// * @
// * @date  : 2024.10.19
// * @author: SJX
// ************************************************/
// void menu_Val_CFG_Arrow_Show(uint16 page_start_row,uint16 line_num)
// {
//     for (uint16 i = 1; i <= LINE_MAX; i++)
//     {
//         if (i == line_num)
//         {
//             // 当前行显示 "->"
//             ips200_show_string(0, page_start_row + i * 18, "->");
//         }
//         else
//         {
//             // 其他行显示空格
//             ips200_show_string(0, page_start_row + i * 18, "  ");
//         }
//     }
// }

// /***********************************************
// * @brief : 菜单参数配置子页面变量刷新函数
// * @param : *CFG_val              需要配置的值的地址，记得&
// *           page_start_row       起始行
// * @return: void
// ************************************************/
// void menu_Val_CFG_int_Flush(int16 *CFG_val, uint16 page_start_row, bool temp_based_flag, int16 basic_1, int16 basic_10, int16 basic_100)
// {
//     // 修改：ips200_show_int 去除 VAL_VFG_SHOW_POINT_BIT 参数
//     ips200_show_int(0, page_start_row + 18 * 0, *CFG_val, VAL_VFG_SHOW_NUM_BIT);
//     menu_Val_CFG_Arrow_Show(page_start_row, menu_Val_CFG_line);
//     if(temp_based_flag == 1)
//     {
//         if(menu_Val_CFG_line == 1)
//             ips200_show_int_color(18, page_start_row + 18 * menu_Val_CFG_line, basic_1, VAL_VFG_SHOW_NUM_BIT, RGB565_RED);
//         else
//             ips200_show_int_color(18, page_start_row + 18 * 1, basic_1, VAL_VFG_SHOW_NUM_BIT, RGB565_WHITE);

//         if(menu_Val_CFG_line == 2)
//             ips200_show_int_color(18, page_start_row + 18 * menu_Val_CFG_line, basic_10, VAL_VFG_SHOW_NUM_BIT, RGB565_RED);
//         else
//             ips200_show_int_color(18, page_start_row + 18 * 2, basic_10, VAL_VFG_SHOW_NUM_BIT, RGB565_WHITE);

//         if(menu_Val_CFG_line == 3)
//             ips200_show_int_color(18, page_start_row + 18 * menu_Val_CFG_line, basic_100, VAL_VFG_SHOW_NUM_BIT, RGB565_RED);
//         else
//             ips200_show_int_color(18, page_start_row + 18 * 3, basic_100, VAL_VFG_SHOW_NUM_BIT, RGB565_WHITE);

//         if(menu_Val_CFG_line == 4)
//             ips200_show_string_color(18, page_start_row + 18 * menu_Val_CFG_line, "empty", RGB565_RED);
//         else
//             ips200_show_string_color(18, page_start_row + 18 * 4, "empty", RGB565_WHITE);

//         if(menu_Val_CFG_line == 5)
//             ips200_show_string_color(18, page_start_row + 18 * menu_Val_CFG_line, "exit", RGB565_RED);
//         else
//             ips200_show_string_color(18, page_start_row + 18 * 5, "  ", RGB565_WHITE);
//     }
//     else
//     {
//         ips200_show_int(18, page_start_row + 18 * 1, basic_1, VAL_VFG_SHOW_NUM_BIT);
//         ips200_show_int(18, page_start_row + 18 * 2, basic_10, VAL_VFG_SHOW_NUM_BIT);
//         ips200_show_int(18, page_start_row + 18 * 3, basic_100, VAL_VFG_SHOW_NUM_BIT);
//         ips200_show_string(18, page_start_row + 18 * 5, "  ");
//         ips200_show_string(18, page_start_row + 18 * 4, "empty");
//     }
//     key_clear_all_state();
// }
// /***********************************************
// * @brief : 整型调参菜单
// * @param : *CFG_val         需要配置的整型值的地址，记得取地址传入
// *          basic_val        基准值
// *          son_start_line   子菜单起始行,例如,basic_val=1,son_start_line=2时,那么默认指向basic_10 = basic_val * 10 = 10
// * @return: void
// * @date  : 2025年4月10日20:51:24
// * @author: SJX
// ************************************************/
// void menu_Val_CFG_int(int16 *CFG_val, int16 basic_val, int16 son_start_line)
// {
//     menu_head_page_node->page->line_num = menu_global_line;
//     menu_cfg_flag = 1;
//     uint8 flush_flag;
//     uint8 CFG_stop_flag = 0;
//     menu_Val_CFG_line = (uint8)son_start_line;
//     bool based_flag = 0;
//     int16 basic_1   = basic_val;
//     int16 basic_10  = basic_val*10;
//     int16 basic_100 = basic_val*100;

//     uint8 page_start_row;
//     page_start_row = menu_global_line * 18;
//     if(page_start_row > (319 - (10 + 18 + 18 * VAL_VFG_SHOW_TOTAL_LINE)))
//     {
//         up_down_flag = 1;
//     }
//     else
//         up_down_flag = 0;
//     menu_Val_CFG_clear(&page_start_row);
//     ips200_set_color(RGB565_WHITE, RGB565_BLACK);
//     menu_Val_CFG_int_Flush(CFG_val, page_start_row, based_flag, basic_1, basic_10, basic_100);
//     menu_key_event = menu_release;
//     while(CFG_stop_flag == 0)
//     {
// //        if(menu_head_page_node->page->page_name == "Camera_Value"
// //         ||menu_head_page_node->page->page_name == "Camera")
// //        {
// //            Image_Show();
// //        }
//         if(menu_key_event == menu_release)
//         {
//             flush_flag = 0;
//         }
//         else
//             flush_flag = 1;
//         if(menu_key_event == menu_back)
//         {
//             menu_key_event = menu_release;
//             CFG_stop_flag = 1;
//             ips200_clear();
//             menu_cfg_flag = 0;
//             menu_head_page_node->page->open_status = 0;
// //            Flash_WriteAllVal();
//         }
//         if((flush_flag == 1 || switch_encoder_change_num != 0) && CFG_stop_flag == 0)
//         {
//             if(menu_key_event == menu_yes)
//             {
// //                Flash_WriteAllVal();
//                 menu_key_event = menu_release;
//                 switch(menu_Val_CFG_line)
//                 {
//                     case 1:
//                         based_flag = !based_flag;
//                         break;
//                     case 2:
//                         based_flag = !based_flag;
//                         break;
//                     case 3:
//                         based_flag = !based_flag;
//                         break;
//                     case 4:
//                         *CFG_val = 0;
//                         Beep_ShortRing();
//                         break;
//                     case 5:
//                         CFG_stop_flag = 1;
//                         menu_cfg_flag = 0;
//                         menu_head_page_node->page->open_status = 0;
//                         break;
//                 }
//             }
//             if(If_Switch_Encoder_Change() == 1)
//             {
//                 if(based_flag == 0)
//                 {
//                     menu_Val_CFG_line += switch_encoder_change_num;
//                     switch_encoder_change_num = 0;
//                     Menu_Val_CFG_Limit(&menu_Val_CFG_line, LINE_MAX);
//                 }
//                 else
//                 {
//                     switch(menu_Val_CFG_line)
//                     {
//                         case 1:
//                             *CFG_val += basic_1 * switch_encoder_change_num;
//                             break;
//                         case 2:
//                             *CFG_val += basic_10 * switch_encoder_change_num;
//                             break;
//                         case 3:
//                             *CFG_val += basic_100 * switch_encoder_change_num;
//                             break;
//                     }
//                 }
//             }
//             menu_Val_CFG_int_Flush(CFG_val, page_start_row, based_flag, basic_1, basic_10, basic_100);
//         }
//     }
//     ips200_clear();
// }

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示 RGB565 彩色图像
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_height_max-1]
// 参数说明     *image          图像数组指针
// 参数说明     width           图像实际宽度
// 参数说明     height          图像实际高度
// 参数说明     dis_width       图像显示宽度 参数范围 [0, ips200_width_max]
// 参数说明     dis_height      图像显示高度 参数范围 [0, ips200_height_max]
// 参数说明     color_mode      色彩模式 0-低位在前 1-高位在前
// 参数说明     fps             帧数
// 返回参数     void
// 使用示例     GIF_Show(0, 0, scc8660_image[0], SCC8660_W, SCC8660_H, SCC8660_W, SCC8660_H, 1);
//              如果要显示低位在前的其他 RGB565 图像 修改最后一个参数即可
//-------------------------------------------------------------------------------------------------------------------

//void GIF_Show(uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode, uint16 fps)
//{
//    for(uint16 i = 0; i < fps; i++)
//    {
//        ips200_show_rgb565_image(x, y, (image)[i], width, height, dis_width, dis_height, color_mode);
//        system_delay_ms(100);
//    }
//}

static void Menu_Image_Show(void);
static void Menu_Bin_Image_Show(void);
static void handle_browse_mode(void);
static void handle_edit_mode(void);
static void render_title(void);
static void render_items(void);
static void render_item(uint8 index);
static void handle_step_select_mode(void);
// static uint8 page_has_static_func(void);

static MenuContext g_ctx = {{0}}; //全局上下文

/***********************************************
* @brief : 按键事件刷新
* @return: void
* @date  : 2025年11月29日
* @author: LDL
************************************************/
void Menu_Event_Flush(void)
{
    If_Switch_Encoder_Change();
    if (my_key_get_state(MENU_KEY) == MY_KEY_SHORT_PRESS)
    {
        menu_key_event = menu_yes;
        my_key_clear_state(MENU_KEY); 
    }
    else if (my_key_get_state(MENU_KEY) == MY_KEY_LONG_PRESS)
    {
        menu_key_event = menu_back;
        my_key_clear_state(MENU_KEY); 
    }
}

/***********************************************
* @brief : 菜单初始化
* @param : root_page  根页面指针
* @return: void
* @note  : 初始化屏幕、栈、页面状态
* @date  : 2025年11月28日
* @author: LDL
************************************************/
void menu_init(MenuPage* root_page) 
{
    // 清空
    g_ctx.current_page = root_page;
    g_ctx.page_stack[0] = root_page;
    g_ctx.stack_top = 0;
    g_ctx.edit_mode = 0;
    g_ctx.edit_step_level = 0;
    
    // 初始化页面
    root_page->cursor_pos = 0;
    root_page->dirty_mask = 0xFFFF;  //全部刷新
    
    // 初始化屏幕
    ips200_init(IPS200_TYPE_SPI);
    ips200_clear();
}

// /***********************************************
// * @brief : 检查页面是否有静态函数项
// * @return: 1-有，0-无
// * @date  : 2025年11月30日
// * @author: LDL
// ************************************************/
// static uint8 page_has_static_func(void)
// {
//     MenuPage* page = g_ctx.current_page;
//     for (uint8 i = 0; i < page->item_count; i++)
//     {
//         if (page->items[i].type & ITEM_STATIC_FUNC)
//         {
//             return 1;
//         }
//     }
//     return 0;
// }

/***********************************************
* @brief : 菜单运行（MENU_RUN）
* @return: void
* @note  : 输入处理 -> 执行静态函数 -> 渲染
* @date  : 2025年11月28日
* @author: LDL
************************************************/
void menu_task(void) 
{
    MenuPage* page;
    uint8 i;
    Menu_Event_Flush();
    switch (g_ctx.edit_mode)
    {
        case 0:  // 浏览模式
            handle_browse_mode();
            break;
        case 1:  // 选择步长模式
            handle_step_select_mode();
            break;
        case 2:  // 编辑数值模式
            handle_edit_mode();
            break;
        default:
            g_ctx.edit_mode = 0;
            break;
    }
    page = g_ctx.current_page;  //获取指针
    for (i = 0; i < page->item_count; i++)
    {
        if (page->items[i].type & ITEM_STATIC_FUNC) 
        {
            if (page->items[i].data.func) 
            {
                page->items[i].data.func();
            }
        }
    }
    //渲染菜单内容
    if (page->dirty_mask != 0) 
    {
        render_title();
        render_items();
        page->dirty_mask = 0;  // 清除脏标记
    }

    // if (page_has_static_func())  //如果有静态函数项，强制刷新所有项
    // {
    //     render_title();
    //     for (i = 0; i < page->item_count && i < MENU_MAX_ITEMS_PER_PAGE; i++) 
    //     {
    //         render_item(i);
    //     }
    // }
    // else if (page->dirty_mask != 0) 
    // {
    //     render_title();
    //     render_items();
    //     page->dirty_mask = 0;
    // }
}

// /***********************************************
// * @brief : 强制渲染所有菜单项
// * @return: void
// * @date  : 2025年11月30日
// * @author: LDL
// ************************************************/
// static void render_items_force(void) 
// {
//     MenuPage* page = g_ctx.current_page;
//     render_title();
//     for (uint8 i = 0; i < page->item_count && i < MENU_MAX_ITEMS_PER_PAGE; i++) 
//     {
//         render_item(i);
//     }
// }

/***********************************************
* @brief : 浏览模式输入处理
* @return: void
* @note  : 光标移动、短按执行、长按返回
* @date  : 2025年11月28日
* @author: LDL
************************************************/
static void handle_browse_mode(void) 
{
    MenuPage* page = g_ctx.current_page;
    
    // 光标移动
    if (switch_encoder_change_num != 0) 
    {
        uint8 old_pos = page->cursor_pos;
        int16 new_pos = (int16)page->cursor_pos + switch_encoder_change_num;
        // 循环限幅
        if (new_pos >= page->item_count) 
        {
            page->cursor_pos = 0;
        } 
        else if (new_pos < 0) 
        {
            page->cursor_pos = page->item_count - 1;
        }
        else
        {
            page->cursor_pos = (uint8)new_pos;
        }
        // 只标记变化的两行为脏
        menu_mark_dirty(old_pos);
        menu_mark_dirty(page->cursor_pos);
        switch_encoder_change_num = 0;
    }
    
    // 短按
    if (menu_key_event == menu_yes) 
    {
        menu_key_event = menu_release;  //先清状态
        const MenuItem* item = &page->items[page->cursor_pos];
        
        if (item->type & ITEM_EDITABLE) 
        {
            g_ctx.edit_mode = 1;
            g_ctx.edit_step_level = 0;  // 默认选择 x1
            menu_mark_dirty(page->cursor_pos);
        } 
        else if (item->type & ITEM_ACTION) 
        {
            if (item->data.func) 
            {
                item->data.func();
            }
        } 
        else if (item->type & ITEM_SUBMENU) 
        {
            // 压栈进入子菜单
            if (g_ctx.stack_top < MENU_STACK_DEPTH - 1) 
            {
                g_ctx.stack_top++;
                MenuPage* sub_page = (MenuPage*)item->data.any_ptr;
                g_ctx.page_stack[g_ctx.stack_top] = sub_page;
                g_ctx.current_page = sub_page;
                sub_page->cursor_pos = 0;
                sub_page->dirty_mask = 0xFFFF;
                ips200_clear();
            }
        }
        else if (item->type & ITEM_DISPLAY_BOOL) 
        {
            //翻转
            *item->data.u8_ptr = !(*item->data.u8_ptr);
            menu_mark_dirty(page->cursor_pos);
        }
        menu_key_event = menu_release;
    }
    
    // 长按返回
    if (menu_key_event == menu_back) 
    {
        if (g_ctx.stack_top > 0) 
        {
            g_ctx.stack_top--;
            g_ctx.current_page = g_ctx.page_stack[g_ctx.stack_top];
            g_ctx.current_page->dirty_mask = 0xFFFF;
            ips200_clear();
        }
        menu_key_event = menu_release;
    }
}

/***********************************************
* @brief : 编辑模式输入处理
* @return: void
* @date  : 2025年11月28日
* @author: LDL
************************************************/
static void handle_edit_mode(void) 
{
    MenuPage* page = g_ctx.current_page;
    const MenuItem* item = &page->items[page->cursor_pos];
    
    if (switch_encoder_change_num != 0) 
    {
        if (item->type & ITEM_DISPLAY_INT) 
        {
            int16 multiplier = 1;
            uint8 i;
            for (i = 0; i < g_ctx.edit_step_level; i++) 
            {
                multiplier *= 10;
            }
            *item->data.i16_ptr += item->step.i16_step * multiplier * switch_encoder_change_num;
        } 
        else if (item->type & ITEM_DISPLAY_FLOAT) 
        {
            float multiplier;
            switch (g_ctx.edit_step_level)
            {
                case 0:  multiplier = 0.01f;  break;
                case 1:  multiplier = 0.1f;   break;
                case 2:  multiplier = 1.0f;   break;
                case 3:  multiplier = 10.0f;  break;
                case 4:  
                default: multiplier = 100.0f; break;
            }
            *item->data.f32_ptr += multiplier * switch_encoder_change_num;
        }
        
        menu_mark_dirty(page->cursor_pos);
        switch_encoder_change_num = 0;
    }
    if (menu_key_event == menu_yes) 
    {
        menu_key_event = menu_release;
    }
    
    if (menu_key_event == menu_back) 
    {
        menu_key_event = menu_release;
        g_ctx.edit_mode = 1;
        menu_mark_dirty(page->cursor_pos);
    }
}

/***********************************************
* @brief : 显示标题栏
* @return: void
* @date  : 2025年11月28日
* @author: LDL
************************************************/
static void render_title(void) 
{
    ips200_show_string_centered(0, g_ctx.current_page->name);
}

/***********************************************
* @brief : 渲染菜单项列表
* @return: void
* @note  : 仅渲染脏标记位为1的项
* @date  : 2025年11月28日
* @author: LDL
************************************************/
static void render_items(void) 
{
    MenuPage* page = g_ctx.current_page;
    
    for (uint8 i = 0; i < page->item_count && i < MENU_MAX_ITEMS_PER_PAGE; i++) 
    {
        if (page->dirty_mask & (1 << i)) 
        {
            render_item(i);
        }
    }
}

/***********************************************
* @brief : 计算菜单项的实际Y坐标
* @param : index - 项索引
* @return: Y坐标
* @date  : 2025年11月30日
* @author: LDL
************************************************/
static uint16 get_item_y_position(uint8 index)
{
    MenuPage* page = g_ctx.current_page;
    uint16 use_start_line = 0;  //偏移量
    // 累加
    for (uint8 i = 0; i < index; i++)
    {
        if (page->items[i].line_offset > 0)
        {
            use_start_line += page->items[i].line_offset;
        }
    }
    if (page->items[index].line_offset > 0)
    {
        use_start_line += page->items[index].line_offset;
    }
    // Y = (累加偏移 + 项索引 + 1) * 行高
    return (use_start_line + index + 1) * MENU_LINE_HEIGHT;
}

/***********************************************
* @brief : 渲染单个菜单项
* @param : index - 项索引
* @return: void
* @date  : 2025年11月28日
* @author: LDL
************************************************/
static void render_item(uint8 index) 
{
    MenuPage* page = g_ctx.current_page;
    const MenuItem* item = &page->items[index];

    if ((item->type & ITEM_STATIC_FUNC) && item->line_offset == -1)
    {
        return;
    }
    uint16 y = get_item_y_position(index);
    uint16 color = (index == page->cursor_pos) ? SELECTED_BRUSH : DEFAULT_BRUSH;
    ips200_show_string_color(MENU_NAME_X_START, y, item->name, color);
    // 显示值
    if (item->type & ITEM_DISPLAY_INT) 
    {
        ips200_show_int_color(MENU_VALUE_X_OFFSET, y, 
                             *item->data.i16_ptr, MENU_INT_MAX_WIDTH, color);
    } 
    else if (item->type & ITEM_DISPLAY_FLOAT) 
    {
        float val = *item->data.f32_ptr;
        val += (val > 0) ? 0.000001f : -0.000001f;
        uint8 precision = (item->decimal_places > MENU_FLOAT_MAX_PRECISION) 
                         ? MENU_FLOAT_MAX_PRECISION 
                         : item->decimal_places;
        
        ips200_show_float_color(MENU_VALUE_X_OFFSET, y, val, 
                               MENU_FLOAT_MAX_WIDTH, precision, color);
    } 
    else if (item->type & ITEM_DISPLAY_BOOL) 
    {
        ips200_show_string_color(MENU_VALUE_X_OFFSET, y, 
                                *item->data.u8_ptr ? "Open " : "Close", color);
    }
    else if (item->type & ITEM_SUBMENU)
    {
        // 子菜单显示 >>>
        ips200_show_string_color(MENU_VALUE_X_OFFSET, y, ">>>", color);
    }
    
    // 编辑模式指示器
    if ((item->type & ITEM_EDITABLE) && index == page->cursor_pos && g_ctx.edit_mode > 0) 
    {
        uint16 step_color;
        uint16 step_x;
        const char* step_str;

        if (g_ctx.edit_mode == 1) 
        {
            step_color = RGB565_WHITE; 
        } 
        else 
        {
            step_color = RGB565_GREEN; 
        }
        
        ips200_show_string_color(MENU_EDIT_INDICATOR_X, y, "     ", DEFAULT_BACKGROUD);
        if (item->type & ITEM_DISPLAY_FLOAT)
        {
            switch (g_ctx.edit_step_level)
            {
                case 0:
                    step_str = "x0.01";
                    step_x = MENU_EDIT_INDICATOR_X + 8; 
                    break;
                case 1:
                    step_str = "x0.1";
                    step_x = MENU_EDIT_INDICATOR_X + 16; 
                    break;
                case 2:
                    step_str = "x1";
                    step_x = MENU_EDIT_INDICATOR_X + 32; 
                    break;
                case 3:
                    step_str = "x10";
                    step_x = MENU_EDIT_INDICATOR_X + 24;
                    break;
                case 4:
                default:
                    step_str = "x100";
                    step_x = MENU_EDIT_INDICATOR_X + 16; 
                    break;
            }
        }
        else
        {
            switch (g_ctx.edit_step_level)
            {
                case 0:
                    step_str = "x1";
                    step_x = MENU_EDIT_INDICATOR_X + 32; 
                    break;
                case 1:
                    step_str = "x10";
                    step_x = MENU_EDIT_INDICATOR_X + 24; 
                    break;
                case 2:
                    step_str = "x100";
                    step_x = MENU_EDIT_INDICATOR_X + 16;
                    break;
                case 3:
                default:
                    step_str = "x1000";
                    step_x = MENU_EDIT_INDICATOR_X + 8; 
                    break;
            }
        }
        
        ips200_show_string_color(step_x, y, step_str, step_color);
    } 
    else 
    {
        ips200_show_string_color(MENU_EDIT_INDICATOR_X, y, "      ", DEFAULT_BACKGROUD);
    }
}

/***********************************************
* @brief : 标记单个项为脏
* @param : item_index - 项索引（0-15）
* @return: void
* @date  : 2025年11月28日
* @author: LDL
************************************************/
void menu_mark_dirty(uint8 item_index) 
{
    if (item_index < MENU_MAX_ITEMS_PER_PAGE) 
    {
        g_ctx.current_page->dirty_mask |= (1 << item_index);
    }
}

/***********************************************
* @brief : 标记所有项为脏(用于切换页面或退出编辑模式时)
* @return: void
* @date  : 2025年11月28日
* @author: LDL
************************************************/
void menu_mark_all_dirty(void) 
{
    g_ctx.current_page->dirty_mask = 0xFFFF;
}

/***********************************************
* @brief : PID调参页面菜单项定义
* @return: void
* @date  : 2025年11月29日
* @author: LDL
************************************************/
static const MenuItem pid_items[] = {
    MENU_FLOAT_EDIT("Steer_Kp", &Steer_PID.Kp, 10.0f, 2, 0),
    MENU_FLOAT_EDIT("Steer_Ki", &Steer_PID.Ki, 10.0f, 2, 0),
    MENU_FLOAT_EDIT("Steer_Kd", &Steer_PID.Kd, 10.0f, 2, 0),
    MENU_FLOAT_EDIT("Motor_Kp_l", &Motor_Speed_PID_l.Kp, 10.0f, 2, 0),
    MENU_FLOAT_EDIT("Motor_Ki_l", &Motor_Speed_PID_l.Ki, 1.0f, 2, 0),
    MENU_FLOAT_EDIT("Motor_Kd_l", &Motor_Speed_PID_l.Kd, 10.0f, 2, 0),
    MENU_FLOAT_EDIT("Motor_Kp_r", &Motor_Speed_PID_r.Kp, 10.0f, 2, 0),
    MENU_FLOAT_EDIT("Motor_Ki_r", &Motor_Speed_PID_r.Ki, 1.0f, 2, 0),
    MENU_FLOAT_EDIT("Motor_Kd_r", &Motor_Speed_PID_r.Kd, 10.0f, 2, 0),
};

static MenuPage pid_page = {
    "PID Tuning",
    pid_items,
    sizeof(pid_items) / sizeof(MenuItem),
    0,
    0xFFFF
};

// ============= 参数调节子页面 =============
static const MenuItem param_items[] = {
    MENU_INT_EDIT("S_Fsight", &ImageStatus.Steer_TowPoint, 1, 0),
    MENU_INT_EDIT("M_Fsight", &ImageStatus.Motor_TowPoint, 1, 0),
    MENU_INT_EDIT("set_speed", &set_speed, 1, 0),
};

static MenuPage param_page = {
    "Param Tuning",
    param_items,
    sizeof(param_items) / sizeof(MenuItem),
    0,
    0xFFFF
};

// ============= 电机监控子页面 =============
static const MenuItem motor_items[] = {
    MENU_INT_SHOW("Speed_Goal_l", &Speed_Goal_l, 0),
    MENU_INT_SHOW("Speed_Goal_r", &Speed_Goal_r, 0),
    MENU_INT_SHOW("Encoder_L", &Speed_Encoder_l, 0),
    MENU_INT_SHOW("Encoder_R", &Speed_Encoder_r, 0),
    MENU_INT_SHOW("PWM_L", &Speed_PID_OUT_l, 0),
    MENU_INT_SHOW("PWM_R", &Speed_PID_OUT_r, 0),
};

static MenuPage motor_page = {
    "Motor Status",
    motor_items,
    sizeof(motor_items) / sizeof(MenuItem),
    0,
    0xFFFF
};

// ============= 二值图页面 =============
static const MenuItem bin_image_items[] = {
    MENU_STATIC_FUNC_ITEM(" ", Menu_Bin_Image_Show, -1),
    MENU_FLOAT_SHOW("Steer_err", &ImageStatus.Steer_Center_error, 2, 6),
    MENU_FLOAT_SHOW("Motor_err", &ImageStatus.Motor_Center_error, 2, 0),
    MENU_INT_SHOW("L_round", (int16*)&L_roundabout_state, 0),
    MENU_INT_SHOW("R_round", (int16*)&R_roundabout_state, 0),
    // MENU_FLOAT_SHOW("R2", &r_squared, 3, 0),
    // MENU_INT_SHOW("cross", (int16*)&cross_state, 0),
    // MENU_INT_SHOW("L_conti", (int16*)&is_left_continuous, 0),
    // MENU_INT_SHOW("R_conti", (int16*)&is_right_continuous, 0),
    // MENU_FLOAT_SHOW("variance", &variance, 2, 0),
};

static MenuPage bin_image_page = {
    "Bin_Image",
    bin_image_items,
    sizeof(bin_image_items) / sizeof(MenuItem),
    0,
    0xFFFF
};

// ============= 主页面 =============
static const MenuItem main_items[] = {
    MENU_STATIC_FUNC_ITEM(" ", Menu_Image_Show, -1),
    
    MENU_SUBMENU_ITEM("Tune Params", &param_page, 6),
    MENU_SUBMENU_ITEM("Bin_Image", &bin_image_page, 0),
    MENU_SUBMENU_ITEM("PID", &pid_page, 0),
    MENU_SUBMENU_ITEM("Motor", &motor_page, 0),
    
    MENU_INT_SHOW("Car_stat", (int16*)&g_Car_Status, 0),
    
    MENU_BOOL_SHOW("R_round", (uint8*)&R_round_status, 0),
    MENU_BOOL_SHOW("L_round", (uint8*)&L_round_status, 0),
};

static MenuPage main_page = {
    .name = "Main",
    .items = main_items,
    .item_count = sizeof(main_items) / sizeof(MenuItem),
};

/***********************************************
* @brief : 菜单原始灰度图显示
* @return: void
* @date  : 2025年11月29日
* @author: LDL
************************************************/
static void Menu_Image_Show(void)
{
    Image_Show();
}

/***********************************************
* @brief : 菜单二值图显示
* @return: void
* @date  : 2025年11月29日
* @author: LDL
************************************************/
static void Menu_Bin_Image_Show(void)
{
    Bin_Image_Show();
}

// ============= 初始化函数 =============
void Menu_Init(void) 
{
    menu_init(&main_page);
}
