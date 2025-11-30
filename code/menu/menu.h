/*
 * menu.h
 *
 *  Created on: 2025年3月19日
 *      Author: sun
 */

#ifndef CODE_MENU_MENU_H_
#define CODE_MENU_MENU_H_

//#include "zf_device_ips200.h"
//#include "MYKEY.h"
//#include "Find_Border.h"
//#include "Image_Element.h"
//#include "Camera.h"
//#include "show_image_process.h"
//#include "Beep.h"
#include "MyHeadFile.h"

#define DEFAULT_BACKGROUD   RGB565_BLACK
#define DEFAULT_BRUSH       RGB565_WHITE
#define SELECTED_BRUSH      RGB565_RED
#define MENU_KEY            MY_KEY_1
// #define MENU_FAST_KEY       MY_KEY_2                //快速触发按键，可以用于快速跳转到指定页，不过写的是💩

// #define MY_X_MAX            240
// #define MY_Y_MAX            320

// #define my_assert(x)        zf_assert(x)


// void Motor_PID_Page_Init(void);
// void Steer_PID_Page_Init(void);
// void Turn_PID_Page_Init(void);

// /***********************************************
//  * @brief : 创建进入功能运行项，调用指定的函数
//  * @param : name       菜单项名称
//  * @param : func_ptr   指向要执行的函数
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_ENTER_FUNC(name, func_ptr, line_count) \
//     { name, ENTER_FUNC_RUN_TYPE, .action.void_func = func_ptr, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建静态功能运行项，常态下直接执行关联函数
//  * @param : name       菜单项名称
//  * @param : func_ptr   指向要执行的函数
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_STATIC_FUNC(name, func_ptr, line_count) \
//     { name, STATIC_FUNC_RUN_TYPE, .action.void_func = func_ptr, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建浮点数显示项，用于实时显示浮点值
//  * @param : name       菜单项名称
//  * @param : value_ptr  指向要显示浮点变量的指针
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_FLOAT_SHOW(name, value_ptr, line_count) \
//     { name, FLOAT_VALUE_SHOW_TYPE, .line_extends.float_value_show_line.show_value = value_ptr, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建整数显示项，用于实时显示整数值
//  * @param : name       菜单项名称
//  * @param : value_ptr  指向要显示整数变量的指针
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_INT_SHOW(name, value_ptr, line_count) \
//     { name, INT_VALUE_SHOW_TYPE, .line_extends.int_value_show_line.show_value = value_ptr, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建浮点数编辑项，允许用户调整浮点数值
//  * @param : name       菜单项名称
//  * @param : edit_ptr   指向可编辑浮点变量的指针
//  * @param : base_val   浮点基准值
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_FLOAT_EDIT(name, edit_ptr, base_val, line_count) \
//     { name, FLOAT_VALUE_EDIT_TYPE, .action.float_float_func = menu_Val_CFG, \
//       .line_extends.float_value_edit_line.edit_value = edit_ptr, \
//       .line_extends.float_value_edit_line.basic_val = base_val, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建配置数值显示项，用于显示特定的配置值
//  * @param : name       菜单项名称
//  * @param : config_ptr 指向要显示配置变量的指针
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_CONFIG_SHOW(name, config_ptr, line_count) \
//     { name, CONFIG_VALUE_SHOW_TYPE, .line_extends.config_value_show_line.show_value = config_ptr, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建菜单页面跳转项，执行页面跳转函数
//  * @param : name       菜单项名称
//  * @param : func_ptr   指向跳转函数的指针
//  * @param : line_count 显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_PAGE_JUMP(name, func_ptr, line_count) \
//     { name, PAGE_JUMP_TYPE, .action.void_func = func_ptr, .display_line_count = line_count }

// /***********************************************
//  * @brief : 创建整数编辑项，允许用户调整整数值
//  * @param : name           菜单项名称
//  * @param : edit_ptr       指向可编辑整数变量的指针
//  * @param : base_val       整数基准值
//  * @param : son_start_line 子菜单起始显示行
//  * @param : line_count     显示行数
//  * @return: 无返回值
//  * @date  : 2025年2月25日10:41:45
//  * @author: SJX
//  ************************************************/
// #define MENU_ITEM_INT_EDIT(name, edit_ptr, base_val, son_start_line, line_count) \
//     { (name), INT_VALUE_EDIT_TYPE, .action.int16_int16_int16_func = menu_Val_CFG_int, \
//       .line_extends = { .int_value_edit_line = { (edit_ptr), (base_val), (son_start_line) } }, \
//       .display_line_count = (line_count), .display_x_start = 0 }


typedef enum
{
    menu_release                    ,
    menu_yes                        ,
    menu_back                       ,
    menu_goto_camera                ,
} Menu_Event;

extern uint8 menu_key_event;
// extern uint8 menu_update_flag;
// extern uint8 menu_wait_flag;

// enum LineExtendsType{
//     STATIC_FUNC_RUN_TYPE,       //常态函数,默认直接执行
//     ENTER_FUNC_RUN_TYPE,        //需要按下执行的函数
//     PAGE_JUMP_TYPE,
//     FLOAT_VALUE_SHOW_TYPE,
//     INT_VALUE_SHOW_TYPE,
//     CONFIG_VALUE_SHOW_TYPE,
//     FLOAT_VALUE_EDIT_TYPE,
//     INT_VALUE_EDIT_TYPE,
// };

// typedef union {
//     void (*void_func)(void);
//     void (*int_func)(int*);
//     void (*int16_func)(int16*);
//     void (*int16_int16_int16_func)(int16*, int16, int16);
//     void (*float_func)(float*);
//     void (*float_float_func)(float*, float);
// } ActionFunction;

// typedef union {
//     struct { float *show_value; } float_value_show_line;
//     struct { int16 *show_value; } int_value_show_line;
//     struct { uint8 *show_value; } config_value_show_line;
//     struct { float *edit_value; float basic_val; } float_value_edit_line;
//     struct { int16 *edit_value; int16 int_basic_val; int16 son_start_line; } int_value_edit_line;
// } LineExtends;

// typedef struct {
//     const char *line_name;                          // 行名称
//     enum LineExtendsType line_type;                 // 行类型
//     ActionFunction action;                          // 统一的函数指针
//     LineExtends line_extends;   // 具名 union
//     int display_line_count;      // 显示的行数（可选）
//     int display_x_start;         // 显示的 x 坐标起始点（可选）
// } MenuLine;

// typedef struct{
//     const char *page_name;       // 行名称
//     uint8       line_num;
//     uint8       line_num_max;
//     uint8       open_status ;
//     MenuLine    *line   ;
// }MenuPage;

// // 菜单页面结构体
// typedef struct MenuPage_Linked_List{
//     MenuPage *page;
// //    struct MenuPage_Linked_List *prev;
//     struct MenuPage_Linked_List *next;
// } MenuPage_Linked_List;

// void Menu_Push_Node(MenuPage *new_page);
// void Menu_Pop_Node(void);
// void Menu_Event_Flush(void);
// uint8 Menu_Get_Page_LineNumMAX(MenuPage* Page);
// void MENU_RUN(void);
// void Menu_Init(void);
// void Menu_Val_CFG_Limit(uint8 *line, uint8 line_max);
// void Menu_Key_Process(void);
// void menu_Val_CFG(float *CFG_val, float basic_val );
// void menu_Val_CFG_int(int16 *CFG_val, int16 basic_val, int16 son_start_line);
// void Menu_Page_Update(void);
// void Camera_Page_Init(void);
// //void GIF_Show(uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode, uint16 fps);

// extern MenuPage_Linked_List *menu_head_page_node;

// static inline void My_Clear(void) {
//     ips200_clear();
// }

// static inline void My_Full(uint16 color) {
//     ips200_full(color);
// }

// static inline void My_Set_Dir(ips200_dir_enum dir) {
//     ips200_set_dir(dir);
// }

// static inline void My_Set_Font(ips200_font_size_enum font) {
//     ips200_set_font(font);
// }

// static inline void My_Set_Color(uint16 pen, uint16 bgcolor) {
//     ips200_set_color(pen, bgcolor);
// }

// static inline void My_Draw_Point(uint16 x, uint16 y, uint16 color) {
//     ips200_draw_point(x, y, color);
// }

// static inline void My_Draw_Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color) {
//     ips200_draw_line(x1, y1, x2, y2, color);
// }

// static inline void My_Show_Char(uint16 x, uint16 y, char dat) {
//     ips200_show_char(x, y, dat);
// }

// static inline void My_Show_String(uint16 x, uint16 y, const char dat[]) {
//     ips200_show_string(x, y, dat);
// }

// static inline void My_Show_Int(uint16 x, uint16 y, int32 dat, uint8 num) {
//     ips200_show_int(x, y, dat, num);
// }

// static inline void My_Show_Uint(uint16 x, uint16 y, uint32 dat, uint8 num) {
//     ips200_show_uint(x, y, dat, num);
// }

// static inline void My_Show_Float(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum) {
//     ips200_show_float(x, y, dat, num, pointnum);
// }

// static inline void My_Show_Binary_Image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height) {
//     ips200_show_binary_image(x, y, image, width, height, dis_width, dis_height);
// }

// static inline void My_Show_Gray_Image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold) {
//     ips200_show_gray_image(x, y, image, width, height, dis_width, dis_height, threshold);
// }

// static inline void My_Show_RGB565_Image(uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode) {
//     ips200_show_rgb565_image(x, y, image, width, height, dis_width, dis_height, color_mode);
// }

// static inline void My_Show_Wave(uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max) {
//     ips200_show_wave(x, y, wave, width, value_max, dis_width, dis_value_max);
// }

// static inline void My_Show_Chinese(uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, uint16 color) {
//     ips200_show_chinese(x, y, size, chinese_buffer, number, color);
// }

// static inline void My_Init(ips200_type_enum type_select) {
//      ips200_init(type_select);
// }

// static inline void My_Show_Char_Color(uint16 x, uint16 y, char dat, uint16 color) {
//     ips200_show_char_color(x, y, dat, color);
// }

// static inline void My_Show_String_Color(uint16 x, uint16 y, const char dat[], uint16 color) {
//     ips200_show_string_color(x, y, dat, color);
// }

// static inline void My_Show_Int_Color(uint16 x, uint16 y, int32 dat, uint8 num, uint16 color) {
//     ips200_show_int_color(x, y, dat, num, color);
// }

// static inline void My_Show_Uint_Color(uint16 x, uint16 y, uint32 dat, uint8 num, uint16 color) {
//     ips200_show_uint_color(x, y, dat, num, color);
// }

// static inline void My_Show_Float_Color(uint16 x, uint16 y, float dat, uint8 num, uint8 pointnum, uint16 color) {
//     ips200_show_float_color(x, y, dat, num, pointnum, color);
// }

// static inline void My_Show_String_Centered(uint16 y, const char dat[]) {
//     ips200_show_string_centered(y, dat);
// }

#define MENU_MAX_ITEMS_PER_PAGE  16   // 单页最多16项
#define MENU_STACK_DEPTH         8    // 菜单嵌套深度
#define MENU_LINE_HEIGHT         18   // 行高
#define MENU_TITLE_HEIGHT        18   // 标题高度
#define MENU_VALUE_X_OFFSET      130  // 数值显示X偏移
#define MENU_EDIT_INDICATOR_X    190  // 编辑指示器X位置

#define MENU_NAME_X_START        0    // 名称起始X
#define MENU_INT_MAX_WIDTH       7    // 整数最大显示宽度
#define MENU_FLOAT_MAX_WIDTH     6    // 浮点数整数部分宽度
#define MENU_FLOAT_MAX_PRECISION 3    // 浮点数小数位数

#define MENU_INT_MAX_STEP_LEVEL      3 
#define MENU_FLOAT_MAX_STEP_LEVEL    4

/***********************************************
* @brief : 菜单项类型标志
* @note  : 节省内存
* @date  : 2025年11月28日
* @author: LDL
************************************************/
typedef enum 
{
    ITEM_DISPLAY_INT     = 0x01,  // 显示整数
    ITEM_DISPLAY_FLOAT   = 0x02,  // 显示浮点数
    ITEM_DISPLAY_BOOL    = 0x04,  // 显示布尔值（Open/Close）
    ITEM_EDITABLE        = 0x08,  // 可编辑标志
    ITEM_ACTION          = 0x10,  // 可执行动作（按下时调用函数）
    ITEM_SUBMENU         = 0x20,  // 子菜单跳转
    ITEM_STATIC_FUNC     = 0x40,  // 静态函数
} ItemTypeFlags;

/***********************************************
* @brief : 菜单数据结构
* @date  : 2025年11月28日
* @author: LDL
************************************************/
typedef struct 
{
    const char*   name;            // 项名称
    uint8         type;            // 类型标志位（可组合）
    union 
    {
        int16*    i16_ptr;         // 整数指针
        float*    f32_ptr;         // 浮点数指针
        uint8*    u8_ptr;          // 布尔值指针
        void*     any_ptr;         // 通用指针（用于子菜单）
        void      (*func)(void);   // 函数指针
    } data;                        
    union 
    {
        int16     i16_step;        // 整数编辑步长
        float     f32_step;        // 浮点编辑步长
    } step;                        // 步长联合体
    uint8         decimal_places;  // 浮点数小数位数
    int8          line_offset;     // 行偏移（用于多行项显示调整）
} MenuItem;

/***********************************************
* @brief : 菜单页面结构
* @date  : 2025年11月28日
* @author: LDL
************************************************/
typedef struct 
{
    const char*       name;          // 页面名称
    const MenuItem*   items;         // 项数组
    uint8             item_count;    // 项数量
    uint8             cursor_pos;    // 当前光标位置
    uint16            dirty_mask;    // 脏标记位图（16位，每位对应一项）
} MenuPage;

/***********************************************
* @brief : 菜单全局上下文
* @date  : 2025年11月28日
* @author: LDL
************************************************/
typedef struct 
{
    MenuPage*   page_stack[MENU_STACK_DEPTH];  // 页面栈
    uint8       stack_top;                     // 栈顶指针
    MenuPage*   current_page;                  // 当前页面指针
    uint8       edit_mode;                     // 0=浏览模式 1=选择步长模式 2=编辑数值模式
    uint8       edit_step_level;               // 编辑步长
} MenuContext;

/***********************************************
* @brief : 整数显示
* @param : name - 显示名称
* @param : var  - 整数变量指针
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_INT_SHOW(name, ptr, offset) \
    { (name), ITEM_DISPLAY_INT, { (void*)(ptr) }, { 0 }, 0, (offset) }

/***********************************************
* @brief : 浮点数显示
* @param : name     - 显示名称
* @param : var      - 浮点变量指针
* @param : decimals - 小数位数
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_FLOAT_SHOW(name, ptr, precision, offset) \
    { (name), ITEM_DISPLAY_FLOAT, { (void*)(ptr) }, { 0 }, (precision), (offset) }

/***********************************************
* @brief : 布尔值显示项（显示Open/Close）
* @param : name - 显示名称
* @param : var  - uint8变量指针（0=Close 1=Open）
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_BOOL_SHOW(name, ptr, offset) \
    { (name), ITEM_DISPLAY_BOOL, { (void*)(ptr) }, { 0 }, 0, (offset) }

/***********************************************
* @brief : 整数编辑项
* @param : name     - 显示名称
* @param : var      - 整数变量指针
* @param : step_val - 编辑步长
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_INT_EDIT(name, ptr, step_val, offset) \
    { (name), (ITEM_DISPLAY_INT | ITEM_EDITABLE), { (void*)(ptr) }, { (step_val) }, 0, (offset) }

/***********************************************
* @brief : 浮点数编辑项
* @param : name     - 显示名称
* @param : var      - 浮点变量指针
* @param : step_val - 编辑步长
* @param : decimals - 小数位数
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_FLOAT_EDIT(name, ptr, step_val, precision, offset) \
    { (name), (ITEM_DISPLAY_FLOAT | ITEM_EDITABLE), { (void*)(ptr) }, { (int16)((step_val) * 100) }, (precision), (offset) }

/***********************************************
* @brief : 创建动作项（按下时执行函数）
* @param : name - 显示名称
* @param : func - 函数指针
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_ACTION_ITEM(name, func_ptr, offset) \
    { (name), ITEM_ACTION, { (void*)(func_ptr) }, { 0 }, 0, (offset) }

/***********************************************
* @brief : 创建静态函数项（每帧自动执行）
* @param : name - 显示名称
* @param : func - 函数指针
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_STATIC_FUNC_ITEM(name, func_ptr, offset) \
    { (name), ITEM_STATIC_FUNC, { (void*)(func_ptr) }, { 0 }, 0, (offset) }

/***********************************************
* @brief : 创建子菜单跳转项
* @param : name     - 显示名称
* @param : page_ptr - 子页面指针
* @date  : 2025年11月28日
* @author: LDL
************************************************/
#define MENU_SUBMENU_ITEM(name, page_ptr, offset) \
    { (name), ITEM_SUBMENU, { (void*)(page_ptr) }, { 0 }, 0, (offset) }

// ============= 函数声明 =============
void menu_init(MenuPage* root_page);
void menu_task(void);
void menu_mark_dirty(uint8 item_index);
void menu_mark_all_dirty(void);
void Menu_Event_Flush(void);
void Menu_Init(void);

#endif /* CODE_MENU_MENU_H_ */
