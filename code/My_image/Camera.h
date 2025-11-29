#ifndef CODE_MY_IMAGE_CAMERA_H_
#define CODE_MY_IMAGE_CAMERA_H_

#include "MYHEADFILE.h"
//#include "Image_Inverse_array.h"

#pragma section all "cpu1_dsram"
#define WHITE 255     //图像黑
#define BLACK 0       //图像白

typedef struct
{
     int16 Steer_TowPoint;             //初始比较点           给定前瞻
     int16 Motor_TowPoint;
     int Det_True;             //由GetDet()函数解算出来的平均图像偏差
     int Det_all;              //图像的近端到远端的总偏差
     float Det_all_k;          //斜率

     uint8 Threshold;          //二值化阈值
     uint32 Threshold_static;  //二值化静态下限
     uint8 Threshold_detach;   //阳光算法分割阈值
     uint8 MiddleLine;         //屏幕中心
     int Foresight;            //偏差速度因子   远端的偏差  用于控速
     uint8 Left_Line;          //左边丢边数
     uint8 Right_Line;         //右边丢边数
     uint8 OFFLine;            /////图像顶边
     uint8 WhiteLine;          ////双边丢边数

     int16 WhiteLine_L;        //左边丢线数
     int16 WhiteLine_R;        //右边丢线数
     int16 OFFLineBoundary;    //八领域截止行
     float Steer_Center_error;       //舵机中线误差
     float Motor_Center_error;       //电机误差
} ImageStatustypedef;

typedef struct
{
        uint8 IsRightFind;      //右边有边标志
        uint8 IsLeftFind;       //左边有边标志
        //uint8 isBlackFind;      //三叉边
        int Wide;               //边界宽度
        int LeftBorder;         //左边界
        int RightBorder;        //右边界
        int close_LeftBorder;   //靠边边界
        int close_RightBorder;  //靠边边界
        int opp_LeftBorder;     //反向边界
        int opp_RightBorder;    //反向边界
        int Center;             //中线
        int RightTemp;          //右边临时值
        int LeftTemp;           //左边临时值
        int CenterTemp;         //中线临时值
        int Black_Point;        //单行黑点数量

        //左右手法则扫线数据
        int LeftBoundary_First;  //左边界 保存第一次数据
        int RightBoundary_First; //右边界 保存第一次数据
        int LeftBoundary;        //左边界 保存最后一次数据
        int RightBoundary;       //右边界 保存最后一次数据

}ImageDealDatatypedef;

extern int16 threshold1;

extern ImageStatustypedef ImageStatus;
extern ImageDealDatatypedef ImageDeal[MT9V03X_H];

//#define USE_IMAGE_H     MT9V03X_H     //使用图像高度
//#define USE_IMAGE_W     144     //使用图像宽度
extern uint8 USE_IMAGE_W;
extern uint8 USE_IMAGE_H;


//#include "Find_Border.h"
//#include "Image_Feature.h"
extern uint8 Camera_Wifi_Image_Send_Flag;

extern uint8    image_original[MT9V03X_H][MT9V03X_W];               //原始图像储存数据储存数组
//extern uint8    reverse_image[MT9V03X_H][MT9V03X_W];                //逆透视后图像使用数据储存数组
extern uint8    use_gray_image[MT9V03X_H][MT9V03X_W];               //图像原始数据储存数组
extern uint8    binary_image[MT9V03X_H][MT9V03X_W];                 //二值化后图像储存数组
//extern uint8    Black_Box_value;        //黑框灰度值
//extern uint32   image_num;              //处理图像数量记录
//extern uint8    x,y;

//图像帧数相关函数
//extern uint32 camera_process_FPS;
//extern uint32 camera_process_cnt_show;

//extern uint8 triangle_w;//三角形水平边长
//extern uint8 triangle_h;//三角形竖直边长

extern uint8 def_black_box_start_y;
extern uint8 circle_black_box_start_y;

void Draw_Corner_Triangles(uint8 a, uint8 b, uint8(*image)[MT9V03X_W]);
void Copy_Cut_Image(uint8 crop_top, uint8 crop_bottom, uint8 crop_left, uint8 crop_right);            //图像裁剪
void MyCamera_Init(void);               //图像初始化
void Copy_Zip_Image(void);              //图像复制并压缩
void Draw_Black_Box(uint8 black_box_value, uint8(*image)[MT9V03X_W]); //图像补框
void Get_Inverse_Perspective_Image(uint8(*source_image)[USE_IMAGE_W], uint8 (*target_image)[MT9V03X_W]);
void Camera_Param_Load(void);
void Image_Show(void);
void Bin_Image_Show(void);
void Draw_Fitting_Lines(void);

void ImageProcess(void);
void DrawLine(void);
uint8 Threshold_deal(uint8* image,uint16 col,uint16 row,uint32 pixel_threshold);
void Binaryzation(void);
void Pixel_Filter(void);
void Search_Bottom_Line_OTSU(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline);
void Search_Left_and_Right_Lines(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline);
void Search_Border_OTSU(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline);
void RouteFilter(void);
void Calculate_Centerline(void);
//void Image_Show_Processed(void);
float Get_Steer_Error(void);
float Get_Motor_Error(void);
short GetOTSU (unsigned char tmImage[MT9V03X_H][MT9V03X_W]);

#define image_h 120//图像高度
#define image_w 188//图像宽度

#define white_pixel 255
#define black_pixel 0

#define bin_jump_num    1//跳过的点数
#define border_max  image_w-2 //边界最大值
#define border_min  1   //边界最小值
//extern uint8 original_image[image_h][image_w];
//extern uint8 bin_image[image_h][image_w];//图像数组

extern uint8 l_border[image_h];
extern uint8 r_border[image_h];

//用于存储 border 到 edge 索引映射的数组
extern uint16 l_border_to_edge_index[image_h];
extern uint16 r_border_to_edge_index[image_h];

extern uint8 A_POINT_TEST_MODE; // 0 = 关闭, 1 = 测试左边界A点, 2 = 测试右边界A点
#define A_POINT_FINE_SEARCH_RANGE 25

extern void image_process(void); //直接在中断或循环里调用此程序就可以循环执行了

//#define R_STATE_NONE        0  // 状态0: 无环岛，正常巡线
//#define R_STATE_INIT        1  // 状态1: 初见环岛，已识别入口特征 (A点和P点)
//#define R_STATE_V_POINT     3  // 状态3: 发现V点，准备入环
//#define R_STATE_ENTERING    4  // 状态4: 即将入环，V点消失
//#define R_STATE_INSIDE      5  // 状态5: 完全在环内行驶
//#define R_STATE_EXITING     6  // 状态6: 发现出口，出现出环特征
//#define R_STATE_LEAVING     7  // 状态7: 即将出环，正在离开环岛

//存储P点和V点实时坐标的全局变量
//extern uint16 roundabout_a_point_x, roundabout_a_point_y;
//extern uint16 roundabout_p_point_x, roundabout_p_point_y;
//extern uint16 roundabout_v_point_x, roundabout_v_point_y;
//extern uint16 roundabout_ld_point_x,roundabout_ld_point_y;
//extern uint16 roundabout_rd_point_x,roundabout_rd_point_y;
//extern uint16 roundabout_out_point_x,roundabout_out_point_y;

// 右圆环拐点
extern uint16 R_roundabout_a_point_x, R_roundabout_a_point_y;  // 右圆环下拐点
extern uint16 R_roundabout_p_point_x, R_roundabout_p_point_y;  // 右圆环环岛点
extern uint16 R_roundabout_v_point_x, R_roundabout_v_point_y;  // 右圆环上拐点
extern uint16 R_roundabout_out_point_x , R_roundabout_out_point_y;  // 右圆环出环点

// 左圆环拐点
extern uint16 L_roundabout_a_point_x, L_roundabout_a_point_y;  // 左圆环下拐点
extern uint16 L_roundabout_p_point_x, L_roundabout_p_point_y;  // 左圆环环岛点
extern uint16 L_roundabout_v_point_x, L_roundabout_v_point_y;  // 左圆环上拐点
extern uint16 L_roundabout_out_point_x, L_roundabout_out_point_y;  // 左圆环出环点

extern uint8 break_row;

extern uint16 l_cross_v_point_x,l_cross_v_point_y;  //左上
extern uint16 l_cross_a_point_x,l_cross_a_point_y;  //左下
extern uint16 r_cross_v_point_x, r_cross_v_point_y;  // 右下
extern uint16 r_cross_a_point_x, r_cross_a_point_y;  // 右上

// 环岛状态机相关全局变量
extern uint16 L_roundabout_state;       // 左环岛状态
extern uint16 R_roundabout_state;       //右环岛状态
extern uint8 roundabout_type;        // 环岛类型 (0:无, 1:左, 2:右)
extern uint8 roundabout_p_point_row; // 用于在状态间传递P点的行号

extern uint8 V_POINT_TEST_MODE;
float Compute_Angle(
           const uint16 (*points)[2],
           int seg1_start,
           int seg1_len,
           int seg2_start,
           int seg2_len,
           float* avg_residual);
float Compute_Angle_Between_Lines(
           uint16 x1, uint16 y1,
           uint16 x2, uint16 y2,
           uint16 x3, uint16 y3,
           uint16 x4, uint16 y4);
void Extend_Border_By_Slope(uint8* border, uint16 a_point_x, uint16 a_point_y,
                            uint8 fit_length, uint8 extend_to_row);

uint8 Find_Border_Break(uint8 *border, uint8 start_row, uint8 end_row, uint8 *break_row);
uint8 Detect_Right_Roundabout_DownCorner(void);
uint8 Detect_Right_Roundabout_UpCorner(void);
uint8 Detect_Right_Roundabout_ringpoint(void);
uint8 Detect_Right_Roundabout_leftdown(void);
uint8 Detect_Right_Roundabout_Outring(void);
uint8 Detect_Left_Roundabout_DownCorner(void);
uint8 Detect_Left_Roundabout_UpCorner(void);
uint8 Detect_Left_Roundabout_ringpoint(void);
uint8 Detect_Left_Roundabout_Outring(void);
void Detect_Right_Roundabout(void);
void Detect_Left_Roundabout(void);
void Test_Right_Roundabout_Entry(void);

uint8 Detect_cross_l_v(void);
uint8 Detect_cross_l_a(void);
uint8 Detect_cross_r_v(void);
uint8 Detect_cross_r_a(void);
void Tilted_Cross_process(void);
uint8 Straight_Judgement(uint8 start_row, uint8 end_row, float variance_threshold);

extern float variance;
extern float lost_ratio;
extern uint16 cross_state;

uint8 Detect_zebraline(void);
void Zebraline_Stop_Control(void);

//void Draw_Cross_Marker(uint16 (*image_buffer)[image_w], uint16 x, uint16 y, uint8 size, uint16 color);
void Draw_Cross_Marker(uint16 x, uint16 y, uint8 size, uint16 color);
void Connect_Border_Points(uint8* border, int x1, int y1, int x2, int y2);
//uint8 Is_Border_Continue(uint8* border, uint8 start_row, uint8 end_row, uint8 border_type);
uint8 Is_Border_Continue(uint8* border, uint8 start_row, uint8 end_row, uint8 border_type, float r2_threshold);
void Convert_Gray8_To_RGB565(const uint8 (*gray_image)[image_w], uint16 (*rgb565_image)[image_w]);
extern uint8 binary_image[MT9V03X_H][MT9V03X_W];
extern int16 is_left_continuous;
extern int16 is_right_continuous;
extern uint16 is_long_straight;
extern uint16 Vy;
extern uint16 P_line;
extern float r_squared;

#pragma section all restore

#pragma section all "cpu0_dsram"

extern uint16 display_image[image_h][image_w];

#pragma section all restore





#endif /* CODE_MY_IMAGE_CAMERA_H_ */

