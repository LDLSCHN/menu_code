/*
 * My_image.c
 *
 *  Created on: 2025年3月13日
 *      Author: 15955
 */
#include "Camera.h"
#include "math.h"
#pragma section all "cpu1_dsram"
uint8 USE_IMAGE_W = MT9V03X_W;
uint8 USE_IMAGE_H = MT9V03X_H;

#define PIXEL_FILTER_START_ROW 10
#define PIXEL_FILTER_END_ROW   110  // 需调整
#define PIXEL_FILTER_START_COL 10
#define PIXEL_FILTER_END_COL   178 // 需调整

uint8    image_original[MT9V03X_H][MT9V03X_W];               //原始图像储存数据储存数组
//uint8    reverse_image[MT9V03X_H][MT9V03X_W];               //逆透视后图像使用数据储存数组
uint8    use_gray_image[MT9V03X_H][MT9V03X_W];              //灰度图像使用数据储存数组
uint8    binary_image[MT9V03X_H][MT9V03X_W];                //二值化后图像储存数组
uint8    image_buffer[MT9V03X_H][MT9V03X_W];            //图像缓冲数组
//uint16_t display_image[image_h][image_w];            //转换后得到的RGB图像储存数组
//uint32   image_num = 0;         //处理图像数量记录
//uint8    Black_Box_value = 50;       //黑框灰度值
//uint8 Camera_Wifi_Image_Send_Flag = 0;
//uint8 x,y;
//static int ytemp = 0;                              //存放行
//static int Ysite = 0, Xsite = 0;                   //Y坐标=列
//static float DetR = 0, DetL = 0;                   //存放斜率

ImageStatustypedef ImageStatus = {.Steer_TowPoint = 60,.Motor_TowPoint = 90};
ImageDealDatatypedef ImageDeal[MT9V03X_H];

//图像处理帧数相关变量
uint32 camera_process_FPS = 0;
uint32 camera_process_cnt_show = 0;
#pragma section all restore

#pragma section all "cpu0_dsram"
uint16_t display_image[image_h][image_w];       //转换后得到的RGB图像储存数组
#pragma section all restore

#pragma section all "cpu1_psram"
/*
*
    =========================================       ---
    |                                        |        *
    |                                        |        |
    |--------+---------------------+----------        |
    |        |                     |         |        |
    | USE_IMAGE_H   所用图像      |            |  MT9V03X_H
    |        |                     |         |        |
    |--------+---------------------+----------        |
    |        |<----USE_IMAGE_W---->|         |        |
    |                                        |        *
    ==========================================       ---
    |<------------MT9V03X_W------------->|
*/
/*
函数名称：int my_abs(int value)
功能说明：求绝对值
参数说明：
函数返回：绝对值
修改时间：2022年9月8日
备    注：
example：  my_abs( x)；
 */
int my_abs(int value)
{
if(value>=0) return value;
else return -value;
}

int16 limit_a_b(int16 x, int a, int b)
{
    if(x<a) x = (int16) a;
    if(x>b) x = (int16) b;
    return x;
}

/*
函数名称：int16 limit(int16 x, int16 y)
功能说明：求x,y中的最小值
参数说明：
函数返回：返回两值中的最小值
修改时间：2022年9月8日
备    注：
example：  limit( x,  y)
 */
int16 limit1(int16 x, int16 y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                return x;
}
/***********************************************
* @brief : 镜头初始化
* @param : void
* @return:
* @date  : 2025年3月14日16:23
* @author: yuu
************************************************/
void MyCamera_Init(void)
{
    ips200_show_string(0, 0, "mt9v03x init.");
    if(mt9v03x_init())
    {
        ips200_show_string(0, 80, "mt9v03x reinit.");
        while(My_Key_IfEnter() == 0);
    }
//        if ( key_get_state(KEY_4) == KEY_SHORT_PRESS )
//            break;
    else
    {
        ips200_show_string(0, 16, "init success.");
        system_delay_ms(500);
    }
}

//void Draw_Cross(uint16 x, uint16 y)  //画大十字
//{
//    uint16 i,j;
//    for (i = 0;i < MT9V03X_W;i ++)
//    {
//        ips200_draw_point(i,y,RGB565_BLUE);
//    }
//    for (j = 0;j < MT9V03X_H;j ++)
//    {
//        ips200_draw_point(x,j,RGB565_BLUE);
//    }
//}

//void Draw_Fitting_Lines(void)  //画拟合的左右边线
//{
//    int i;
//    float y1,y2;
//
//    for (i = 0;i < MT9V03X_W;i ++)
//    {
//        y1 = -1.167*i + 107.5;
//        y2 = 1.228*i - 115.1;
//        if (y1 < 0){y1 = 0;}
//        if (y1 > 120){y1 = 120;}
//        if (y2 < 0){y2 = 0;}
//        if (y2 > 120){y2 = 120;}
//        ips200_draw_point(i,(uint16)y1,RGB565_BLUE);  //左边线
//        ips200_draw_point(i,(uint16)y2,RGB565_RED);   //右边线
//    }
//}
/***********************************************
* @brief : 从摄像头DMA缓冲区获取最新一帧图像
* @param : void
* @return: void
* @date  : 2025年10月25日
* @author: LDL
************************************************/
void Get_Camera_Image(void)
{
    memcpy(image_original, mt9v03x_image, sizeof(image_original));
}

//将灰度图改为RGB图
void Convert_Gray8_To_RGB565(const uint8 (*gray_image)[image_w], uint16 (*rgb565_image)[image_w])
{
    for (int y = 0; y < image_h; y++)
    {
        for (int x = 0; x < image_w; x++)
        {
            uint8 gray_value = gray_image[y][x];
            uint8 r5 = gray_value >> 3;
            uint8 g6 = gray_value >> 2;
            uint8 b5 = gray_value >> 3;
            rgb565_image[y][x] = (uint16)(r5 << 11) | (uint16)(g6 << 5) | (uint16)b5;
        }
    }
}

//uint8 triangle_w = 50;//三角形水平边长
//uint8 triangle_h = 60;//三角形竖直边长
/***********************************************
 * @brief : 在图像左上角和右上角绘制黑色直角三角形
 * @param : w            水平直角边长度
 * @param : h            垂直直角边长度
 * @param : image        要绘制的图像数组
 * @return: void
 * @date  : 2025年4月16日
 * @author: yuu
 ***********************************************/
//void Draw_Corner_Triangles(uint8 w, uint8 h, uint8(*image)[MT9V03X_W])
//{
//    // 安全检查
//    if (w >= USE_IMAGE_W || h >= USE_IMAGE_H)
//    {
//        return;
//    }
//    // 绘制左上角三角形
//    for (uint8 y = 0; y < h; y++)
//    {
//        // 计算当前行需要填充的像素数
//        uint8 width = (uint8)((float)w * (h - y) / h);
//        for (uint8 x = 0; x < width; x++)
//        {
//            image[y][x] = Black_Box_value;  // 设置为黑色
//        }
//    }
//    // 绘制右上角三角形
//    for (uint8 y = 0; y < h; y++) {
//        // 计算当前行需要填充的起始位置
//        uint8 start_x = USE_IMAGE_W - (uint8)((float)w * (h - y) / h);
//        for (uint8 x = start_x; x < USE_IMAGE_W; x++)
//        {
//            image[y][x] = Black_Box_value;  // 设置为黑色
//        }
//    }
//}
///***********************************************
// * @brief : 图像裁剪
// * @param : 上、下、左、右分别裁剪的像素数量
// * @return: void
// * @date  : 2025年4月16日
// * @author: yuu
// ***********************************************/
//void Copy_Cut_Image(uint8 crop_top, uint8 crop_bottom, uint8 crop_left, uint8 crop_right)
//{
//    // 安全检查：确保裁剪后图像不为负数或超界
//    if ((crop_top + crop_bottom >= MT9V03X_H) ||
//        (crop_left + crop_right >= MT9V03X_W))
//    {
//        USE_IMAGE_W = 0;
//        USE_IMAGE_H = 0;
//        return;
//    }
//
//    USE_IMAGE_W = MT9V03X_W - crop_left - crop_right;
//    USE_IMAGE_H = MT9V03X_H - crop_top - crop_bottom;
//
//    x_border_max = USE_IMAGE_W - 3;
//    y_border_max = USE_IMAGE_H - 3;
//
//    for (uint8 y = 0; y < USE_IMAGE_H; y++)
//    {
//        for (uint8 x = 0; x < USE_IMAGE_W; x++)
//        {
//            use_gray_image[y][x] = image_original[y + crop_top][x + crop_left];
//        }
//    }
//
//}
/***********************************************
* @brief : 图像复制并压缩
* @param : void
* @return:
* @date  : 2025年3月14日16:23
* @author: yuu
************************************************/
//void Copy_Zip_Image(void)
//{
//    uint8 i,j;
//
//    for(i = 0; i < USE_IMAGE_H; i++)
//    {
//        for(j = 0; j < USE_IMAGE_W; j++)
//        {
//            use_gray_image[i][j] = image_original[(uint8)(i * (MT9V03X_H/USE_IMAGE_H))][(uint8)(j * (MT9V03X_W/USE_IMAGE_W))];
//            //use_gray_image[i][j] = image_original[i][j];
//        }
//    }
//}
///***********************************************
//* @brief : 图像补框
//* @param : void
//* @return: uint8 black_box_value            黑框的灰度值
//*          uint8(*image)[Image_X]           要补黑框的图像
//* @date  : 2025年3月18日13:23
//* @author: yuu
//* @exp   :Draw_Black_Box(Black_Box_value, Find_Line_Image);
//************************************************/
//uint8 Black_Box_value_FFF = 50;
//uint8 Black_Box_value_FF = 50;
//uint8 Black_Box_value_F = 50;
//uint8 def_black_box_start_y = 2;
//uint8 circle_black_box_start_y = 5;
//void Draw_Black_Box(uint8 black_box_value, uint8(*image)[MT9V03X_W])
//{
//    uint8 i,j;
//
//    Black_Box_value_FFF = Black_Box_value_FF;
//    Black_Box_value_FF = Black_Box_value_F;
//    Black_Box_value_F = black_box_value;
//    black_box_value = 0.5 * Black_Box_value_F + 0.3 * Black_Box_value_FF + 0.2 * Black_Box_value_FFF;  //滤波
//    Black_Box_value = black_box_value;
//    static uint8 black_box_start_y = 0;
//    if(element_state == right_circle_3
//     ||element_state == left_circle_3)
//        black_box_start_y = circle_black_box_start_y;
//    else
//        black_box_start_y = def_black_box_start_y;
//
//    for(i = 1; i < USE_IMAGE_H; i++)
//    {
//        image[i][USE_IMAGE_W - 3] = black_box_value;
//        image[i][2] = black_box_value;
//    }
//    for(j = 2; j <= USE_IMAGE_W -3; j++)
//    {
//        image[black_box_start_y][j] = black_box_value;
//    }
//}
///***********************************************
//* @brief : 求透视后图像
//* @param : void
//* @return:uint8(*source_image)[Image_X]        要进行逆透视变换的矩阵
//*         uint8 (*target_image)[Image_X]        逆透视变换后的图像
//* @date  : 2025年3月18日13:23
//* @author: yuu
//* @exp   : Get_Inverse_Perspective_Image(Find_Line_Image, I_Perspective_Image);
//************************************************/
//void Get_Inverse_Perspective_Image(uint8(*source_image)[MT9V03X_W], uint8 (*target_image)[MT9V03X_W])          //835us
//{
//    uint8 i = 0, j = 0;
//    for(j = 0; j < USE_IMAGE_H; j ++)
//    {
//        for(i = 0; i < USE_IMAGE_W; i ++)
//        {
//              target_image[j][i] = source_image[invers_matrix_row[j * USE_IMAGE_W + i]][invers_matrix_col[j * USE_IMAGE_W + i]];
//        }
//    }
//}

///***********************************************
//* @brief : 图像参数重载
//* @param : void
//* @return: void
//* @date  : 2025年7月14日18:54:46
//* @author: SJX
//************************************************/
//void Camera_Param_Load(void)
//{
//    if(image_light  != last_image_light)
//    {
////        printf("%f\r\n", filter_speed);
////        Beep_MediumRing();
//        mt9v03x_set_exposure_time(image_light);
//        last_image_light = image_light;
//    }
//    if(imgae_enable != last_image_enable)
//    {
//        mt9v03x_init();
////        Beep_MediumRing();
////        printf("%d\r\n",mt9v03x_set_auto_exposure(imgae_enable));
//        last_image_enable = imgae_enable;
//    }
//    if(last_image_gain != image_gain)
//    {
//        mt9v03x_init();
////        Beep_MediumRing();
////        printf("%d\r\n",mt9v03x_set_auto_exposure(imgae_enable));
//        last_image_gain = image_gain;
//    }
//}

/***********************************************
* @brief : 图像显示
* @param : void
* @return: void
* @date  : 2025年9月30日15:47:05
* @author: SJX
************************************************/
void Image_Show(void)
{
    ips200_show_gray_image(0, 18, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
}

/***********************************************
* @brief : 显示处理后的二值化图像、边界和中线
* @param : void
* @return: void
* @date  : 2025年10月28日
* @author: LDL
************************************************/
//void Image_Show_Processed(void)
//{
//    //屏幕上的起始Y坐标
//    const uint16 start_y = 18;
//
//    ips200_show_gray_image(0, start_y, (const uint8 *)binary_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 1);
//
//    uint8 bottom_row = MT9V03X_H - 2;
//
//    for (int i = bottom_row; i > ImageStatus.OFFLineBoundary; i--)
//    {
//        // 绘制左边界（红色）
//        if (ImageDeal[i].LeftBorder > 0 && ImageDeal[i].LeftBorder < MT9V03X_W - 1)
//        {
//            ips200_draw_point((uint16) ImageDeal[i].LeftBorder,(uint16) (i + start_y), RGB565_RED);
//        }
//        // 绘制右边界（蓝色）
//        if (ImageDeal[i].RightBorder > 0 && ImageDeal[i].RightBorder < MT9V03X_W - 1)
//        {
//            ips200_draw_point((uint16) ImageDeal[i].RightBorder, (uint16) (i + start_y), RGB565_BLUE);
//        }
//        // 绘制中线（绿色）
//        if (ImageDeal[i].Center > 0 && ImageDeal[i].Center < MT9V03X_W - 1)
//        {
//            ips200_draw_point((uint16) ImageDeal[i].Center, (uint16) (i + start_y), RGB565_GREEN);
//        }
//    }
//     float avg_error = Get_Error();
//
//     uint16 text_y = start_y + MT9V03X_H + 5;
//     ips200_show_string(10, text_y, "Avg Error: ");
//
//     ips200_show_float(120, text_y, avg_error, 3, 2); // 整数3位，小数2位
//     ips200_show_string(190, text_y, "      "); // 清理尾部
//
//     int foresight_start_row = ImageStatus.TowPoint;
//     int foresight_end_row = foresight_start_row + 5;
//
//     // 绘制前瞻起始线
//     if (foresight_start_row >= 0 && foresight_start_row < MT9V03X_H)
//     {
//         uint16 screen_y1 = (uint16)(foresight_start_row + start_y);
//         ips200_draw_line(0, screen_y1, MT9V03X_W - 1, screen_y1, RGB565_YELLOW);
//     }
//     // 绘制前瞻结束线
//     if (foresight_end_row >= 0 && foresight_end_row < MT9V03X_H)
//     {
//         uint16 screen_y2 = (uint16)(foresight_end_row + start_y);
//         ips200_draw_line(0, screen_y2, MT9V03X_W - 1, screen_y2, RGB565_YELLOW);
//     }
//}

/***********************************************
* @brief : 画边界
* @param : void
* @return: void
* @date  : 2025/10/20
* @author: LDL
************************************************/
//void DrawLine(void)  //画边界调试
//{
//  uint8 i;
//  for (i = MT9V03X_H - 1; i > ImageStatus.OFFLine; i--)
//  {
//      if (i < MT9V03X_H) // 安全检查
//      {
//          binary_image[i][ImageDeal[i].LeftBorder + 2] = 0;
//          binary_image[i][ImageDeal[i].RightBorder - 2] = 0;
//          binary_image[i][ImageDeal[i].Center] = 0;
//      }
//  }
//}

/***********************************************
* @brief : 大津法求阈值
* @param : uint8* image,uint16 col,uint16 row,uint32 pixel_threshold
* @return: threshold 阈值
* @date  : 2025/10/19
* @author: LDL
************************************************/
//uint8 Threshold_deal(uint8* image,uint16 col,uint16 row,uint32 pixel_threshold)
//{
//  #define GrayScale 256
//  uint16 width = col;
//  uint16 height = row;
//  int pixelCount[GrayScale];
//  float pixelPro[GrayScale];
//  int i, j;
//  long pixelSum = (long)width * height; //long防溢出
//  uint8 threshold = 0;
//  uint8* data = image;  //指向像素数据的指针
//  for (i = 0; i < GrayScale; i++)
//  {
//    pixelCount[i] = 0;
//    pixelPro[i] = 0;
//  }
//
//  unsigned long gray_sum = 0;
//  //统计灰度级中每个像素在整幅图像中的个数
//  for (i = 0; i < height; i += 1)
//  {
//    for (j = 0; j < width; j += 1)
//    {
//      pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
//      gray_sum += (int)data[i * width + j];  //灰度值总和
//    }
//  }
//
//  //计算每个像素值的点在整幅图像中的比例
//  for (i = 0; i < GrayScale; i++)
//  {
//    pixelPro[i] = (float)pixelCount[i] / pixelSum;
//  }
//
//  //遍历灰度级[0,255]
//  float w0 = 0, w1 = 0, u0tmp = 0, u1tmp = 0, u0 = 0, u1 = 0, deltaTmp = 0, deltaMax = 0;
//  double total_mean_gray = (double)gray_sum / pixelSum;
//
//  for (j = 0; j < pixel_threshold; j++)
//  {
//     w0 += pixelPro[j];
//     u0tmp += j * pixelPro[j];
//
//     //if (w0 == 0) continue;
//     //w1 = 1.0f - w0;
//     //if (w1 == 0) break;
//
//     u0 = u0tmp / w0;
//     u1tmp = total_mean_gray - u0tmp;
//     u1 = u1tmp / w1;
//
//     deltaTmp = w0 * w1 * pow((u0 - u1), 2); // 类间方差
//     if (deltaTmp > deltaMax)
//     {
//       deltaMax = deltaTmp;
//       threshold = (uint8) j;
//     }
//  }
//  return threshold;
//}

/***********************************************
* @brief : 图像二值化
* @param : void
* @return: void
* @date  : 2025/10/19
* @author: LDL
******************************************** ****/
int16 threshold1 = 0;
void Binaryzation(void)
{

    ImageStatus.Threshold = (uint8) GetOTSU(image_original);
    uint8 i, j = 0;
    for (i = 0; i < MT9V03X_H; i++)
    {
      for (j = 0; j < MT9V03X_W; j++)
      {
        if (image_original[i][j] > ImageStatus.Threshold + threshold1)
          binary_image[i][j] = 255;  //白
        else
          binary_image[i][j] = 0;  //黑
      }
    }
}

/***********************************************
* @brief : 像素滤波
* @param : void
* @return: void
* @date  : 2025/10/19
* @author: LDL
************************************************/
//void Pixel_Filter(void)
//{
//    int col,row;
//    for (row = PIXEL_FILTER_START_ROW; row < PIXEL_FILTER_END_ROW; row++)
//    {
//        for (col = PIXEL_FILTER_START_COL; col < PIXEL_FILTER_END_COL; col++)
//        {
//            //边界检查
//            if (row > 0 && row < MT9V03X_H - 1 && col > 0 && col < MT9V03X_W - 1)
//            {
//                if (binary_image[row][col] == 0 && (binary_image[row - 1][col] + binary_image[row + 1][col] +
//                        binary_image[row][col + 1] + binary_image[row][col - 1] >=3))
//                {
//                    binary_image[row][col]=1;
//                }
//            }
//        }
//    }
//}

/***********************************************
* @brief : 搜索底边左右边线
* @param : uint8 imageInput[MT9V03X_H][MT9V03X_H], uint8 Row, uint8 Col, uint8 Bottonline
* @return: void
* @date  : 2025/10/20
* @author: LDL
************************************************/
//void Search_Bottom_Line_OTSU(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline)
//{
//    for (int Xsite = Col / 2-2; Xsite > 1; Xsite--)//左边
//    {
//        if (imageInput[Bottonline][Xsite] == 1 && imageInput[Bottonline][Xsite - 1] == 0)
//        {
//            ImageDeal[Bottonline].LeftBoundary = Xsite;//获取底边左边线
//            break;
//        }
//    }
//    for (int Xsite = Col / 2+2; Xsite < MT9V03X_W-1; Xsite++)
//    {
//        if (imageInput[Bottonline][Xsite] == 1 && imageInput[Bottonline][Xsite + 1] == 0)
//        {
//            ImageDeal[Bottonline].RightBoundary = Xsite;//获取底边右边线
//            break;
//        }
//    }
//}

/***********************************************
* @brief : 搜索边线 上交大开源
* @param : uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline
* @return: void
* @date  : 2025/10/20
* @author: 开源
************************************************/
//void Search_Left_and_Right_Lines(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline)
//{
//    /*寻左线坐标规则*/
//    uint8 Left_Rule[2][8] = {
//                                  {0,-1,1,0,0,1,-1,0 },//{0,-1},{1,0},{0,1},{-1,0},  (x,y )
//                                  {-1,-1,1,-1,1,1,-1,1} //{-1,-1},{1,-1},{1,1},{-1,1}
//    };
//    /*寻右线坐标规则*/
//    int Right_Rule[2][8] = {
//                              {0,-1,1,0,0,1,-1,0 },//{0,-1},{1,0},{0,1},{-1,0},
//                              {1,-1,1,1,-1,1,-1,-1} //{1,-1},{1,1},{-1,1},{-1,-1}
//    };
//      int num=0;
//    uint8 Left_Ysite = Bottonline;
//    uint8 Left_Xsite = (uint8) ImageDeal[Bottonline].LeftBoundary;
//    uint8 Left_Rirection = 0;//左边方向
//    uint8 Pixel_Left_Ysite = Bottonline;
//    uint8 Pixel_Left_Xsite = 0;
//
//    uint8 Right_Ysite = Bottonline;
//    uint8 Right_Xsite = (uint8) ImageDeal[Bottonline].RightBoundary;
//    uint8 Right_Rirection = 0;//右边方向
//    uint8 Pixel_Right_Ysite = Bottonline;
//    uint8 Pixel_Right_Xsite = 0;
//    uint8 Ysite = Bottonline;
//    ImageStatus.OFFLineBoundary = 5;
//    while (1)
//    {
//        num++;
//        if(num > (MT9V03X_W * MT9V03X_H))
//        {
//            ImageStatus.OFFLineBoundary = Ysite;
//            break;
//        }
//        if (Ysite >= Pixel_Left_Ysite && Ysite >= Pixel_Right_Ysite)
//        {
//            if (Ysite < ImageStatus.OFFLineBoundary)
//            {
//                ImageStatus.OFFLineBoundary = Ysite;
//                break;
//            }
//            else
//            {
//                Ysite--;
//            }
//        }
//        /*********左边巡线*******/
//        if ((Pixel_Left_Ysite > Ysite) || Ysite == ImageStatus.OFFLineBoundary)//右边扫线
//        {
//            /*计算前方坐标*/
//            Pixel_Left_Ysite = Left_Ysite + Left_Rule[0][2 * Left_Rirection + 1];
//            Pixel_Left_Xsite = Left_Xsite + Left_Rule[0][2 * Left_Rirection];
//
//            if (imageInput[Pixel_Left_Ysite][Pixel_Left_Xsite] == 0)//前方是黑色
//            {
//                //顺时针旋转90
//                if (Left_Rirection == 3)
//                    Left_Rirection = 0;
//                else
//                    Left_Rirection++;
//            }
//            else//前方是白色
//            {
//                /*计算左前方坐标*/
//                Pixel_Left_Ysite = Left_Ysite + Left_Rule[1][2 * Left_Rirection + 1];
//                Pixel_Left_Xsite = Left_Xsite + Left_Rule[1][2 * Left_Rirection];
//
//                if (imageInput[Pixel_Left_Ysite][Pixel_Left_Xsite] == 0)//左前方为黑色
//                {
//                    //方向不变  Left_Rirection
//                    Left_Ysite = Left_Ysite + Left_Rule[0][2 * Left_Rirection + 1];
//                    Left_Xsite = Left_Xsite + Left_Rule[0][2 * Left_Rirection];
//                    if (ImageDeal[Left_Ysite].LeftBoundary_First == 0){
//                        ImageDeal[Left_Ysite].LeftBoundary_First = Left_Xsite;
//                        ImageDeal[Left_Ysite].LeftBoundary = Left_Xsite;
//                    }
//                }
//                else//左前方为白色
//                {
//                    // 方向发生改变 Left_Rirection  逆时针90度
//                    Left_Ysite = Left_Ysite + Left_Rule[1][2 * Left_Rirection + 1];
//                    Left_Xsite = Left_Xsite + Left_Rule[1][2 * Left_Rirection];
//                    if (ImageDeal[Left_Ysite].LeftBoundary_First == 0 )
//                        ImageDeal[Left_Ysite].LeftBoundary_First = Left_Xsite;
//                    ImageDeal[Left_Ysite].LeftBoundary = Left_Xsite;
//                    if (Left_Rirection == 0)
//                        Left_Rirection = 3;
//                    else
//                        Left_Rirection--;
//                }
//
//            }
//        }
//        /*********右边巡线*******/
//        if ((Pixel_Right_Ysite > Ysite) || Ysite == ImageStatus.OFFLineBoundary)//右边扫线
//        {
//            /*计算前方坐标*/
//            Pixel_Right_Ysite = Right_Ysite + Right_Rule[0][2 * Right_Rirection + 1];
//            Pixel_Right_Xsite = Right_Xsite + Right_Rule[0][2 * Right_Rirection];
//
//            if (imageInput[Pixel_Right_Ysite][Pixel_Right_Xsite] == 0)//前方是黑色
//            {
//                //逆时针旋转90
//                if (Right_Rirection == 0)
//                    Right_Rirection = 3;
//                else
//                    Right_Rirection--;
//            }
//            else//前方是白色
//            {
//                /*计算右前方坐标*/
//                Pixel_Right_Ysite = Right_Ysite + Right_Rule[1][2 * Right_Rirection + 1];
//                Pixel_Right_Xsite = Right_Xsite + Right_Rule[1][2 * Right_Rirection];
//
//                if (imageInput[Pixel_Right_Ysite][Pixel_Right_Xsite] == 0)//左前方为黑色
//                {
//                    //方向不变  Right_Rirection
//                    Right_Ysite = Right_Ysite + Right_Rule[0][2 * Right_Rirection + 1];
//                    Right_Xsite = Right_Xsite + Right_Rule[0][2 * Right_Rirection];
//                    if (ImageDeal[Right_Ysite].RightBoundary_First == (MT9V03X_W - 1))
//                        ImageDeal[Right_Ysite].RightBoundary_First = Right_Xsite;
//                    ImageDeal[Right_Ysite].RightBoundary = Right_Xsite;
//                }
//                else//左前方为白色
//                {
//                    // 方向发生改变 Right_Rirection  逆时针90度
//                    Right_Ysite = Right_Ysite + Right_Rule[1][2 * Right_Rirection + 1];
//                    Right_Xsite = Right_Xsite + Right_Rule[1][2 * Right_Rirection];
//                    if (ImageDeal[Right_Ysite].RightBoundary_First == (MT9V03X_W - 1))
//                        ImageDeal[Right_Ysite].RightBoundary_First = Right_Xsite;
//                    ImageDeal[Right_Ysite].RightBoundary = Right_Xsite;
//                    if (Right_Rirection == 3)
//                        Right_Rirection = 0;
//                    else
//                        Right_Rirection++;
//                }
//
//            }
//        }
//
//        if (abs(Pixel_Right_Xsite - Pixel_Left_Xsite) < 3)//Ysite<80是为了放在底部是斑马线扫描结束  3 && Ysite < 30
//        {
//
//            ImageStatus.OFFLineBoundary = Ysite;
//            break;
//        }
//    }
//}

/***********************************************
* @brief : 获取边线
* @param : uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline
* @return: void
* @date  : 2025/10/20
* @author: LDL
************************************************/
//void Search_Border_OTSU(uint8 imageInput[MT9V03X_H][MT9V03X_W], uint8 Row, uint8 Col, uint8 Bottonline)
//{
//        ImageStatus.WhiteLine_L = 0;
//        ImageStatus.WhiteLine_R = 0;
//        //ImageStatus.OFFLine = 1;
//        /*封上下边界处理*/
//        for (int Xsite = 0; Xsite < MT9V03X_W; Xsite++)
//        {
//            imageInput[0][Xsite] = 0;
//            imageInput[Bottonline + 1][Xsite] = 0;
//        }
//        /*封左右边界处理*/
//        for (int Ysite = 0; Ysite < MT9V03X_H; Ysite++)
//        {
//                ImageDeal[Ysite].LeftBoundary_First = 0;
//                ImageDeal[Ysite].RightBoundary_First = 79;
//
//                imageInput[Ysite][0] = 0;
//                imageInput[Ysite][MT9V03X_W - 1] = 0;
//        }
//        /********获取底部边线*********/
//        Search_Bottom_Line_OTSU(imageInput, Row, Col, Bottonline);
//        /********获取左右边线*********/
//        Search_Left_and_Right_Lines(imageInput, Row, Col, Bottonline);
//
//
//
//        for (int Ysite = Bottonline; Ysite > ImageStatus.OFFLineBoundary + 1; Ysite--)
//        {
//            if (ImageDeal[Ysite].LeftBoundary < 3)
//            {
//                ImageStatus.WhiteLine_L++;
//            }
//            if (ImageDeal[Ysite].RightBoundary > MT9V03X_W - 3)
//            {
//                ImageStatus.WhiteLine_R++;
//            }
//        }
//}

/***********************************************
* @brief : 中线滤波
* @param : void
* @return: void
* @date  : 2025/10/20
* @author: LDL
************************************************/
//void RouteFilter(void)
//{
//    // MODIFIED: 循环的起始和结束值应相对于图像高度
//     for (Ysite = MT9V03X_H - 2; Ysite >= (ImageStatus.OFFLine + 5); Ysite--)
//     {
//         // NOTE: 45 这个值也需要根据新图像内容进行调整
//         if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite].IsRightFind == 'W' && Ysite <= (MT9V03X_H * 0.75))
//         {
//             ytemp = Ysite;
//             while (ytemp >= (ImageStatus.OFFLine + 5))
//             {
//                 ytemp--;
//                 if (ImageDeal[ytemp].IsLeftFind == 'T' && ImageDeal[ytemp].IsRightFind == 'T')
//                 {
//                     DetR = (float)(ImageDeal[ytemp - 1].Center - ImageDeal[Ysite + 2].Center) / (float)(ytemp - 1 - Ysite - 2);
//                     int CenterTemp = ImageDeal[Ysite + 2].Center;
//                     int LineTemp = Ysite + 2;
//                     while (Ysite >= ytemp) {
//                         ImageDeal[Ysite].Center = (int)(CenterTemp + DetR * (float)(Ysite - LineTemp));
//                         Ysite--;
//                     }
//                     break;
//                 }
//             }
//         }
//         //滑动平均滤波
//         if (Ysite > 0)
//         {
//             ImageDeal[Ysite].Center = (ImageDeal[Ysite - 1].Center + 2 * ImageDeal[Ysite].Center) / 3;
//         }
//     }
//}

///***********************************************
//* @brief : 计算中线
//* @param : void
//* @return: void
//* @date  : 2025/10/20
//* @author: LDL
//************************************************/
//void Calculate_Centerline(void)
//{
//    uint16 i;
//    for (i = hightest; i < image_h-1; i++)
//    {
//       center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
//       //求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
//       //当然也有多组边线的找法，但是个人感觉很繁琐，不建议
//    }
//}

/***********************************************
* @brief : 图像处理
* @param : void
* @return: void
* @date  : 2025/10/20
* @author: LDL
************************************************/
//void ImageProcess(void)
//{
//    Get_Camera_Image();
////    Copy_Zip_Image();
////    ImageStatus.OFFLine = 2;  //这个值根据真实距离得到，必须进行限制
////    ImageStatus.WhiteLine = 0;
////    for (Ysite = MT9V03X_H - 1; Ysite >= ImageStatus.OFFLine; Ysite--)
////    {
////      ImageDeal[Ysite].IsLeftFind = 'F';
////      ImageDeal[Ysite].IsRightFind = 'F';
////      ImageDeal[Ysite].LeftBorder = 0;
////      ImageDeal[Ysite].RightBorder = MT9V03X_W - 1;
////      ImageDeal[Ysite].LeftTemp = 0;
////      ImageDeal[Ysite].RightTemp = MT9V03X_W - 1;
////
////      ImageDeal[Ysite].close_LeftBorder = 0;
////      ImageDeal[Ysite].close_RightBorder = MT9V03X_W - 1;
////    }
//    Binaryzation();
////    Pixel_Filter();
//
////    // 定义底行为图像高度-2
////    uint8 bottom_row = MT9V03X_H - 2;
////
////    // 搜索边界
////    Search_Border_OTSU(binary_image, MT9V03X_H, MT9V03X_W, bottom_row);
////
////    // ImageStatus.OFFLineBoundary 是由八领域搜索确定的有效最高行
////    Calculate_Centerline(bottom_row,(uint8) ImageStatus.OFFLineBoundary);
////
////    // 对计算出的中线进行滤波
////    RouteFilter();
//}

////int16 TowPoint
//float Get_Error(void)  //获取平均误差
//{
//    int i;
//    float error_sum = 0;
//    int start_row = ImageStatus.TowPoint;
//    int end_row = start_row + 5; // 计算5行的平均值
//
////    if (start_row < ImageStatus.OFFLineBoundary) start_row = ImageStatus.OFFLineBoundary;
////    if (end_row >= MT9V03X_H) end_row = MT9V03X_H - 1;
////    if (start_row >= end_row) return 0.0f; // 如果范围无效，返回0
//
//    for (i = start_row; i < end_row; i++)
//    {
//        error_sum += (MT9V03X_W/2 - center_line[i]);
//    }
//
//    float error = error_sum / (end_row - start_row);
//    return error;
//}

/*************************************************************************
 *  函数名称：short GetOTSU (unsigned char tmImage[MT9V03X_H][MT9V03X_W])
 *  功能说明：大津法求阈值大小
 *  参数说明：tmImage ： 图像数据
 *  函数返回：无
*************************************************************************/
short GetOTSU (unsigned char tmImage[MT9V03X_H][MT9V03X_W])
{
    signed short i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned short HistoGram[256];     //原先为unsigned char ，但是同一个灰度值的像素点可能会超过255个，因此造成溢出，扩大数据范围，感谢评论区指正。

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < MT9V03X_H; j++)
    {
        for (i = 0; i < MT9V03X_W; i++)
        {
            HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0; MaxValue--); //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  像素总数

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        Pixelshortegral += HistoGram[j] * j;        //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];     //前景像素点数
        PixelFore = Amount - PixelBack;           //背景像素点数
        OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
        OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
        PixelshortegralBack += HistoGram[j] * j;  //前景灰度值
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
        MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //返回最佳阈值;
}

/*
函数名称：void get_start_point(uint8 start_row)
功能说明：寻找两个边界的边界点作为八邻域循环的起始点
参数说明：输入任意行数
函数返回：无
修改时间：2022年9月8日
备    注：
example：  get_start_point(image_h-2)
 */
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值
//uint8 get_start_point(uint8 start_row)
//{
//    uint8 rows_checked = 0;
//    uint8 l_found = 0, r_found = 0;
//    int current_row;
//
//    if (start_row >= image_h)
//    {
//        start_row = image_h - 2;
//    }
//
//    current_row = (int)start_row;
//
//    while (rows_checked < 20 && current_row >= 1)
//    {
//        start_point_l[0] = 0;
//        start_point_l[1] = 0;
//        start_point_r[0] = 0;
//        start_point_r[1] = 0;
//
//        l_found = 0;
//        r_found = 0;
//
//        for (uint8 i = (uint8)(image_w / 2); i > border_min; i--)
//        {
//            if (binary_image[current_row][i] == 255 && binary_image[current_row][i - 1] == 0)
//            {
//                start_point_l[0] = i;
//                start_point_l[1] = (uint8)current_row;
//                l_found = 1;
//                break;
//            }
//        }
//
//        for (uint8 i = (uint8)(image_w / 2); i < border_max; i++)
//        {
//            if (binary_image[current_row][i] == 255 && binary_image[current_row][i + 1] == 0)
//            {
//                start_point_r[0] = i;
//                start_point_r[1] = (uint8)current_row;
//                r_found = 1;
//                break;
//            }
//        }
//
//        if (l_found && r_found)
//        {
//            return 1;
//        }
//
//        current_row--;
//        rows_checked++;
//    }
//
//    start_point_l[0] = 0;
//    start_point_l[1] = 0;
//    start_point_r[0] = 0;
//    start_point_r[1] = 0;
//
//    return 0;
//}

uint8 get_start_point(uint8 start_row)
{
    uint8 i = 0,l_found = 0,r_found = 0;
    //清零
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y

    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y

        //从中间往左边，先找起点
    for (i = image_w / 2; i > border_min; i--)
    {
        start_point_l[0] = i;//x
        start_point_l[1] = start_row;//y
        if (binary_image[start_row][i] == 255 && binary_image[start_row][i - 1] == 0)
        {
            //printf("找到左边起点image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }

    for (i = image_w / 2; i < border_max; i++)
    {
        start_point_r[0] = i;//x
        start_point_r[1] = start_row;//y
        if (binary_image[start_row][i] == 255 && binary_image[start_row][i + 1] == 0)
        {
            //printf("找到右边起点image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }

    if(l_found&&r_found)return 1;
    else {
        //printf("未找到起点\n");
        return 0;
    }
}

/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
                            uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r            ：最多需要循环的次数
(*image)[image_w]       ：需要进行找点的图像数组，必须是二值图,填入数组名称即可
                       特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic              ：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic              ：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x               ：左边起点横坐标
l_start_y               ：左边起点纵坐标
r_start_x               ：右边起点横坐标
r_start_y               ：右边起点纵坐标
hightest                ：循环结束所得到的最高高度
函数返回：无
修改时间：2022年9月25日
备    注：
example：
    search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
                start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
 */
#define USE_num image_h*3   //定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

 //存放点的x，y坐标
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线
uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint8 hightest = 0;//最高点
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
{
    uint8 i = 0, j = 0;

    //左边变量
    uint8 search_filds_l[8][2] = { {  0 } };
    uint8 index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = {  0 };
    uint16 l_data_statics;//统计左边
    //定义八个邻域
    static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是顺时针

    //右边变量
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//中心坐标点
    uint8 index_r = 0;//索引下标
    uint8 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//统计右边
    //定义八个邻域
    static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是逆时针

    l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

        //开启邻域循环
    while (break_flag--)
    {

        //左边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//索引加一

        //右边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        index_l = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//先清零，后使用
            temp_l[i][1] = 0;//先清零，后使用
        }

        //左边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                temp_l[index_l][0] = search_filds_l[(i)][0];
                temp_l[index_l][1] = search_filds_l[(i)][1];
                index_l++;
                dir_l[l_data_statics - 1] = (i);//记录生长方向
            }

            if (index_l)
            {
                //更新坐标点
                center_point_l[0] = temp_l[0][0];//x
                center_point_l[1] = temp_l[0][1];//y
                for (j = 0; j < index_l; j++)
                {
                    if (center_point_l[1] > temp_l[j][1])
                    {
                        center_point_l[0] = temp_l[j][0];//x
                        center_point_l[1] = temp_l[j][1];//y
                    }
                }
            }

        }
        if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
        {
            //printf("三次进入同一个点，退出\n");
            break;
        }
        if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
            )
        {
            //printf("\n左右相遇退出\n");
            *hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
            //printf("\n在y=%d处退出\n",*hightest);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
            //printf("\n如果左边比右边高了，左边等待右边\n");
            continue;//如果左边比右边高了，左边等待右边
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
        {
            //printf("\n左边开始向下了，等待右边，等待中... \n");
            center_point_l[0] = (uint8) points_l[l_data_statics - 1][0];//x
            center_point_l[1] = (uint8) points_l[l_data_statics - 1][1];//y
            l_data_statics--;
        }
        r_data_statics++;//索引加一

        index_r = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_r[i][0] = 0;//先清零，后使用
            temp_r[i][1] = 0;//先清零，后使用
        }

        //右边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//索引加一
                dir_r[r_data_statics - 1] = (i);//记录生长方向
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {

                //更新坐标点
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }

            }
        }


    }
    //取出循环次数
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;
}

/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_L ：找到的点的总数
函数返回：无
修改时间：2022年9月25日
备    注：
example： get_left(data_stastics_l );
 */
uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组
void get_left(uint16 total_L)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    //初始化
    for (i = 0;i<image_h;i++)
    {
        l_border[i] = border_min;
    }
    h = image_h - 2;
    //左边
    for (j = 0; j < total_L; j++)
    {
        //printf("%d\n", j);
        if (points_l[j][1] == h)
        {
            l_border[h] = points_l[j][0]+1;
        }
        else continue; //每行只取一个点，没到下一行就不记录
        h--;
        if (h == 0)
        {
            break;//到最后一行退出
        }
    }
}

/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
函数返回：无
修改时间：2022年9月25日
备    注：
example：get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    for (i = 0; i < image_h; i++)
    {
        r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
    }
    h = image_h - 2;
    //右边
    for (j = 0; j < total_R; j++)
    {
        if (points_r[j][1] == h)
        {
            r_border[h] = points_r[j][0] - 1;
        }
        else continue;//每行只取一个点，没到下一行就不记录
        h--;
        if (h == 0)break;//到最后一行退出
    }
}

#define threshold_max   255*5//此参数可根据自己的需求调节
#define threshold_min   255*2//此参数可根据自己的需求调节
void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
    uint16 i, j;
    uint32 num = 0;


    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //统计八个方向的像素值
            num =
                bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
                + bin_image[i][j - 1] + bin_image[i][j + 1]
                + bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


            if (num >= threshold_max && bin_image[i][j] == 0)
            {

                bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

            }
            if (num <= threshold_min && bin_image[i][j] == 255)
            {

                bin_image[i][j] = 0;//黑

            }

        }
    }

}
/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8(*image)[image_w] 图像首地址
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[image_w])
{

    uint8 i = 0;
    for (i = 0; i < image_h; i++)
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;

    }
    for (i = 0; i < image_w; i++)
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;

    }
}

/***********************************************
* @brief : 计算中线
* @param : void
* @return: void
* @date  : 2025/10/20
* @author: LDL
************************************************/
void Calculate_Centerline(void)
{
    uint16 i;
    for (i = hightest; i < image_h-1; i++)
    {
       center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
       //求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
    }
}
/*
函数名称：void image_process(void)
功能说明：最终处理函数
参数说明：无
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_process();
 */
uint8 Break_flag_l = 0,Break_flag_r = 0;
int16 is_left_continuous = 0;
int16 is_right_continuous = 0;
uint8 cnt1 = 0;
uint16 is_long_straight = 0;
void image_process(void)
{
    //uint16 i;
    uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
    Get_Camera_Image();
    Binaryzation();
//    mt9v03x_finish_flag = 0;
    /*提取赛道边界*/
    image_filter(binary_image);
    image_draw_rectan(binary_image);//预处理
    //清零
    data_stastics_l = 0;
    data_stastics_r = 0;
    if (get_start_point(image_h - 2)) //找到起点了，再执行八领域，没找到就一直找
    {
//        Beep_Stop();
        cnt1 = 0;
//        printf("正在开始八领域\n");
        search_l_r((uint16)USE_num, binary_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
//        printf("八邻域已结束\n");
        // 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
        get_left(data_stastics_l);
        get_right(data_stastics_r);
//        printf("%d, %d\r\n", data_stastics_l, data_stastics_r);
//        Detect_Right_Roundabout_ringpoint();
//        Detect_Right_Roundabout_DownCorner();
//        Detect_Right_Roundabout_UpCorner();
//        Detect_Right_Roundabout_leftdown();
//        Detect_Right_Roundabout_Outring();
//        Detect_Left_Roundabout_UpCorner();
//        if (R_round_status == status_R_round_on)
//        {
//          Detect_Right_Roundabout();
//        }
//        if (L_round_status == status_L_round_on)
//        {
//          Detect_Left_Roundabout();
//        }
//        if (cross_status == status_cross_on)
//        {
//           Tilted_Cross_process();
//        }
//        if (Detect_zebraline() == 1)
//        {
//            Beep_Start();
//        }
//        else
//        {
//            Beep_Stop();
//        }
        Zebraline_Stop_Control();
        if (L_round_status != status_L_round_on)
         {
             // 清空左圆环状态和拐点
             L_roundabout_state = 0;
             L_roundabout_a_point_x = 0;
             L_roundabout_a_point_y = 0;
             L_roundabout_p_point_x = 0;
             L_roundabout_p_point_y = 0;
             L_roundabout_v_point_x = 0;
             L_roundabout_v_point_y = 0;
             L_roundabout_out_point_x = 0;
             L_roundabout_out_point_y = 0;
         }

         // --- 检查十字开关 ---
         if (cross_status != status_cross_on)
         {
             // 清空十字状态和拐点
             cross_state = 0;
             l_cross_a_point_x = 0;
             l_cross_a_point_y = 0;
             l_cross_v_point_x = 0;
             l_cross_v_point_y = 0;
             r_cross_a_point_x = 0;
             r_cross_a_point_y = 0;
             r_cross_v_point_x = 0;
             r_cross_v_point_y = 0;
         }

        uint8 is_cross_active = 0;

         if (cross_status == status_cross_on)
         {
             Tilted_Cross_process();
             if (cross_state != 0)
             {
                 is_cross_active = 1;
             }
         }

         if (is_cross_active)
         {
             // 十字运行中，关闭圆环
             L_roundabout_state = 0;
             R_roundabout_state = 0;
             // 清空拐点...
             L_roundabout_a_point_x = 0;
             L_roundabout_a_point_y = 0;
             L_roundabout_p_point_x = 0;
             L_roundabout_p_point_y = 0;
             L_roundabout_v_point_x = 0;
             L_roundabout_v_point_y = 0;
             L_roundabout_out_point_x = 0;
             L_roundabout_out_point_y = 0;

             R_roundabout_a_point_x = 0;
             R_roundabout_a_point_y = 0;
             R_roundabout_p_point_x = 0;
             R_roundabout_p_point_y = 0;
             R_roundabout_v_point_x = 0;
             R_roundabout_v_point_y = 0;
             R_roundabout_out_point_x = 0;
             R_roundabout_out_point_y = 0;
         }
         else
         {
             // 正常执行圆环
             if (R_round_status == status_R_round_on)
             {
                 Detect_Right_Roundabout();
             }
             if (L_round_status == status_L_round_on)
             {
                 Detect_Left_Roundabout();
             }
         }
         Calculate_Centerline();
         is_long_straight = Straight_Judgement(image_h - 20, 30, 0.5f);
//         if(is_long_straight)
//          {
//              Beep_Start();
//          }
//          else
//          {
//              Beep_Stop();
//          }
    }
    else
    {
//        Beep_Start();
        cnt1++;
        if (cnt1 >= 5)
        {
            Car_Stop();
        }
    }
    mt9v03x_finish_flag = 0;
}

//显示二值化图像
void Bin_Image_Show(void)
{
    uint16 i;
//    memcpy(image_buffer, binary_image, sizeof(image_buffer));
//    Convert_Gray8_To_RGB565(binary_image, display_image);
    ips200_show_gray_image(0, 0, binary_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
//    Draw_Fitting_Lines();
    for (i = 0; i < data_stastics_l; i++)
    {
        ips200_draw_point(points_l[i][0]+2, points_l[i][1], RGB565_RED);//显示起点
    }
    for (i = 0; i < data_stastics_r; i++)
    {
        ips200_draw_point(points_r[i][0]-2, points_r[i][1], RGB565_BLUE);//显示起点
    }

    for (i = hightest; i < image_h-1; i++)
    {
        ips200_draw_point(center_line[i], i, RGB565_GREEN);//显示起点 显示中线
        ips200_draw_point(l_border[i], i, RGB565_RED);//显示起点 显示左边线
        ips200_draw_point(r_border[i], i, RGB565_BLUE);//显示起点 显示右边线
    }
//    for (i = 0; i < MT9V03X_W; i++)
//    {
//        ips200_draw_point(i ,ImageStatus.TowPoint, RGB565_RED);
//        ips200_draw_point(i ,(ImageStatus.TowPoint) + 5, RGB565_RED);
//    }
//    Draw_Cross(x,y);
//    for (i = hightest; i < image_h - 1; i++)
//    {
//         if(center_line[i] > 0 && center_line[i] < image_w)
//             display_image[i][center_line[i]] = RGB565_GREEN; // 画中线
//         if(l_border[i] > 0 && l_border[i] < image_w)
//             display_image[i][l_border[i]] = RGB565_RED;      // 画左边界
//         if(r_border[i] > 0 && r_border[i] < image_w)
//             display_image[i][r_border[i]] = RGB565_BLUE;     // 画右边界
//    }

//    for (i = 0; i < image_w; i++)
//    {
//        if(ImageStatus.TowPoint >= 0 && ImageStatus.TowPoint < image_h)
//            display_image[ImageStatus.TowPoint][i] = RGB565_YELLOW;
//        if((ImageStatus.TowPoint + 5) >= 0 && (ImageStatus.TowPoint + 5) < image_h)
//            display_image[ImageStatus.TowPoint + 5][i] = RGB565_YELLOW;
//    }
//    if (roundabout_a_point_y > 0)
//    {
//        Draw_Cross_Marker(display_image, roundabout_a_point_x, roundabout_a_point_y, 5, RGB565_BLUE);
//    }
//    if (roundabout_p_point_y > 0)
//        Draw_Cross_Marker(display_image, roundabout_p_point_x, roundabout_p_point_y, 5, RGB565_PURPLE);
//    if (roundabout_v_point_y > 0)
//        Draw_Cross_Marker(display_image, roundabout_v_point_x, roundabout_v_point_y, 5, RGB565_RED);
    if (L_roundabout_a_point_y > 0)
        Draw_Cross_Marker(L_roundabout_a_point_x, L_roundabout_a_point_y, 5, RGB565_BLUE);
    if (L_roundabout_p_point_y > 0)
        Draw_Cross_Marker(L_roundabout_p_point_x, L_roundabout_p_point_y, 5, RGB565_PURPLE);
    if (L_roundabout_v_point_y > 0)
        Draw_Cross_Marker(L_roundabout_v_point_x, L_roundabout_v_point_y, 5, RGB565_RED);
//    if (roundabout_ld_point_y > 0)
//        Draw_Cross_Marker(roundabout_ld_point_x, roundabout_ld_point_y, 5, RGB565_PINK);
    if (L_roundabout_out_point_y > 0)
        Draw_Cross_Marker(L_roundabout_out_point_x, L_roundabout_out_point_y, 5, RGB565_BROWN);

    if (R_roundabout_a_point_y > 0)
        Draw_Cross_Marker(R_roundabout_a_point_x, R_roundabout_a_point_y, 5, RGB565_BLUE);
    if (R_roundabout_p_point_y > 0)
        Draw_Cross_Marker(R_roundabout_p_point_x, R_roundabout_p_point_y, 5, RGB565_PURPLE);
    if (R_roundabout_v_point_y > 0)
        Draw_Cross_Marker(R_roundabout_v_point_x, R_roundabout_v_point_y, 5, RGB565_RED);
    if (R_roundabout_out_point_y > 0)
        Draw_Cross_Marker(R_roundabout_out_point_x, R_roundabout_out_point_y, 5, RGB565_BROWN);

    // 左下拐点
    if (l_cross_a_point_y > 0)
        Draw_Cross_Marker(l_cross_a_point_x, l_cross_a_point_y, 5, RGB565_WHITE);

    // 左上拐点
    if (l_cross_v_point_y > 0)
        Draw_Cross_Marker(l_cross_v_point_x, l_cross_v_point_y, 5, RGB565_GREEN);

    // 右下拐点
    if (r_cross_a_point_y > 0)
        Draw_Cross_Marker(r_cross_a_point_x, r_cross_a_point_y, 5, RGB565_CYAN);

    // 右上拐点
    if (r_cross_v_point_y > 0)
        Draw_Cross_Marker(r_cross_v_point_x, r_cross_v_point_y, 5, RGB565_PINK);
//    ips200_show_rgb565_image(0, 0, display_image[0],MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
}

float Get_Steer_Error(void)  //获取舵机平均误差
{
     int i;
     float error_sum = 0;
     int start_row = ImageStatus.Steer_TowPoint;
     int end_row = start_row + 5; // 计算5行的平均值

     for (i = start_row; i < end_row; i++)
     {
         error_sum += (center_line[i] - MT9V03X_W/2);
     }

     ImageStatus.Steer_Center_error = error_sum / (end_row - start_row);
     return ImageStatus.Steer_Center_error;
//    int i;
//    float error_sum = 0;
//    int start_row = ImageStatus.TowPoint;
//    int end_row = start_row + 5;
//
//    if (start_row < hightest) start_row = hightest;
//    if (start_row < 0) start_row = 0;
//    if (end_row >= image_h) end_row = image_h - 1;
//    if (start_row >= end_row) return ImageStatus.Center_error;
//
//    for (i = start_row; i < end_row; i++)
//        error_sum += (center_line[i] - image_w/2);
//
//    float raw_err = error_sum / (end_row - start_row);
//
//    static float err_lp = 0.0f;
//    const float alpha = 0.3f;  // 低通
//    err_lp = alpha * raw_err + (1.0f - alpha) * err_lp;
//
//    ImageStatus.Center_error = err_lp;
//    return ImageStatus.Center_error;
}

float Get_Motor_Error(void)    //获取电机偏差
{
    int i;
    float error_sum = 0;
    int start_row = ImageStatus.Motor_TowPoint;
    int end_row = start_row + 5; // 计算5行的平均值

    for (i = start_row; i < end_row; i++)
    {
        error_sum += (center_line[i] - MT9V03X_W/2);
    }

    ImageStatus.Motor_Center_error = error_sum / (end_row - start_row);
    return ImageStatus.Motor_Center_error;
}

// --- 全局状态变量 ---
uint16 L_roundabout_state = 0;       // 环岛状态机，默认为无环岛状态
uint16 R_roundabout_state = 0;
uint8 roundabout_type  = 0;                  // 环岛类型，默认为无
uint8 roundabout_p_point_row = 0;            // 用于在状态间传递P点的行号

// P点和V点坐标记录, 初始化为0表示无效
//uint16 roundabout_a_point_x = 0, roundabout_a_point_y = 0;
//uint16 roundabout_p_point_x = 0, roundabout_p_point_y = 0;
//uint16 roundabout_v_point_x = 0, roundabout_v_point_y = 0;
//uint16 roundabout_ld_point_x = 0,roundabout_ld_point_y = 0;
//uint16 roundabout_rd_point_x = 0,roundabout_rd_point_y = 0;
//uint16 roundabout_out_point_x = 0,roundabout_out_point_y = 0;

// 右圆环拐点
uint16 R_roundabout_a_point_x = 0, R_roundabout_a_point_y = 0;  // 右圆环下拐点
uint16 R_roundabout_p_point_x = 0, R_roundabout_p_point_y = 0;  // 右圆环环岛点
uint16 R_roundabout_v_point_x = 0, R_roundabout_v_point_y = 0;  // 右圆环上拐点
uint16 R_roundabout_out_point_x = 0, R_roundabout_out_point_y = 0;  // 右圆环出环点

// 左圆环拐点
uint16 L_roundabout_a_point_x = 0, L_roundabout_a_point_y = 0;  // 左圆环下拐点
uint16 L_roundabout_p_point_x = 0, L_roundabout_p_point_y = 0;  // 左圆环环岛点
uint16 L_roundabout_v_point_x = 0, L_roundabout_v_point_y = 0;  // 左圆环上拐点
uint16 L_roundabout_out_point_x = 0, L_roundabout_out_point_y = 0;  // 左圆环出环点

uint8 break_row;

uint16 l_cross_v_point_x = 0,l_cross_v_point_y = 0;
uint16 l_cross_a_point_x = 0,l_cross_a_point_y = 0;

uint8 Find_Border_Break(uint8 *border, uint8 start_row, uint8 end_row, uint8 *break_row)
{
    const uint8 JUMP_THRESHOLD = 15;  // 跳变阈值，根据实际调整
    const uint8 MIN_CHECK_ROWS = 3;   // 最少检查行数

    if (start_row <= end_row || start_row >= image_h || end_row < MIN_CHECK_ROWS)
    {
        return 0;
    }

    for (int16 row = start_row - 1; row >= end_row + 1; row--)
    {
        uint8 current_val = border[row];
        uint8 prev_val = border[row + 1];
        uint8 next_val = border[row - 1];

        uint8 jump = (uint8)my_abs(current_val - prev_val);

        if (jump > JUMP_THRESHOLD)
        {
            uint8 jump_next = (uint8)my_abs(next_val - current_val);

            if (jump_next > JUMP_THRESHOLD / 2)
            {
                *break_row = (uint8)row;
                return 1;
            }
        }
    }

    return 0;
}

float Compute_Angle_Between_Lines(
           uint16 x1, uint16 y1,
           uint16 x2, uint16 y2,
           uint16 x3, uint16 y3,
           uint16 x4, uint16 y4)
{
    const float EPS = 2.0f;            // 向量最小长度阈值（像素），可调整为 1~3
    const float PI_F = 3.14159265358979323846f;

    float v1x = (float)x2 - (float)x1;
    float v1y = (float)y2 - (float)y1;
    float v2x = (float)x4 - (float)x3;
    float v2y = (float)y4 - (float)y3;

    float mag1 = sqrtf(v1x * v1x + v1y * v1y);
    float mag2 = sqrtf(v2x * v2x + v2y * v2y);

    // 向量太短，角度不可用
    if (mag1 < EPS || mag2 < EPS) return -1.0f;

    float a1 = atan2f(v1y, v1x);
    float a2 = atan2f(v2y, v2x);

    float diff = fabsf(a1 - a2);
    if (diff > PI_F) diff = 2.0f * PI_F - diff; // 归一到 [0, PI]

    return diff * 180.0f / PI_F;
}

float Compute_Angle(
           const uint16 (*points)[2],
           int seg1_start,
           int seg1_len,
           int seg2_start,
           int seg2_len,
           float* avg_residual)
{
    const float EPS = 1e-3f;
    const float PI_F = 3.14159265358979323846f;
    float sumY, sumX, sumYY, sumYX;
    float k1, b1, err1, k2, b2, err2;
    int i;

    if (seg1_len < 2 || seg2_len < 2 || seg1_start < 0 || seg2_start < 0)
        return -1.0f;

    sumY = sumX = sumYY = sumYX = 0.0f;
    for (i = 0; i < seg1_len; i++)
    {
        int idx = seg1_start + i;
        float y = (float)points[idx][1];
        float x = (float)points[idx][0];
        sumY  += y;
        sumX  += x;
        sumYY += y * y;
        sumYX += y * x;
    }
    float denom = (float)seg1_len * sumYY - sumY * sumY;
    if (fabsf(denom) < EPS) return -1.0f;
    k1 = ((float)seg1_len * sumYX - sumY * sumX) / denom;
    b1 = (sumX - k1 * sumY) / (float)seg1_len;
    err1 = 0.0f;
    for (i = 0; i < seg1_len; i++)
    {
        int idx = seg1_start + i;
        float y = (float)points[idx][1];
        float x = (float)points[idx][0];
        err1 += fabsf(k1 * y + b1 - x);
    }
    err1 /= (float)seg1_len;

    sumY = sumX = sumYY = sumYX = 0.0f;
    for (i = 0; i < seg2_len; i++)
    {
        int idx = seg2_start + i;
        float y = (float)points[idx][1];
        float x = (float)points[idx][0];
        sumY  += y;
        sumX  += x;
        sumYY += y * y;
        sumYX += y * x;
    }
    denom = (float)seg2_len * sumYY - sumY * sumY;
    if (fabsf(denom) < EPS) return -1.0f;
    k2 = ((float)seg2_len * sumYX - sumY * sumX) / denom;
    b2 = (sumX - k2 * sumY) / (float)seg2_len;
    err2 = 0.0f;
    for (i = 0; i < seg2_len; i++)
    {
        int idx = seg2_start + i;
        float y = (float)points[idx][1];
        float x = (float)points[idx][0];
        err2 += fabsf(k2 * y + b2 - x);
    }
    err2 /= (float)seg2_len;

    if (avg_residual)
        *avg_residual = 0.5f * (err1 + err2);

    float tan_theta = fabsf((k1 - k2) / (1.0f + k1 * k2));
    float theta = atanf(tan_theta);
    if (theta > (PI_F * 0.5f)) theta = PI_F - theta;

    return theta * 180.0f / PI_F;
}

#define A_POINT_FILTER_ALPHA 0.4f // Filter for the lower corner (A-point)
#define P_POINT_FILTER_ALPHA 0.4f // Filter for the convex point (P-point)
#define V_POINT_FILTER_ALPHA 0.4f // Filter for the upper corner (V-point)

void Extend_Border_By_Slope(uint8* border, uint16 a_point_x, uint16 a_point_y,
                            uint8 fit_length, uint8 extend_to_row)  //根据斜率补线
{
    // 安全检查
    if (a_point_y >= image_h - 2 || a_point_y < extend_to_row + 5)
        return;

    // 从A点往下取 fit_length 个点进行线性拟合
    float sum_x = 0.0f;
    float sum_y = 0.0f;
    float sum_xy = 0.0f;
    float sum_yy = 0.0f;
    int n = 0;

    int start_row = (int)a_point_y;
    int end_row = start_row + fit_length;
    if (end_row >= image_h - 2)
        end_row = image_h - 2;

    for (int row = start_row; row < end_row; row++)
    {
        if (border[row] > border_min && border[row] < border_max)
        {
            float y = (float)row;
            float x = (float)border[row];

            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_yy += y * y;
            n++;
        }
    }

    if (n < 3) return; // 有效点太少

    // 计算斜率 k 和截距 b: x = k*y + b
    float denominator = n * sum_yy - sum_y * sum_y;
    if (fabsf(denominator) < 1e-6f) return;

    float k = (n * sum_xy - sum_x * sum_y) / denominator;
    float b = (sum_x - k * sum_y) / n;

    // 🔧 关键: 从A点向上延伸补线
    for (int row = (int)a_point_y; row >= extend_to_row; row--)
    {
        float x_pred = k * (float)row + b;

        // 限幅
        int x_new = (int)(x_pred + 0.5f); // 四舍五入
        x_new = limit_a_b((int16)x_new, border_min, border_max);

        border[row] = (uint8)x_new;
    }
}

// uint8 Detect_Right_Roundabout_DownCorner(void) //右圆环下拐点检测
// {
//     const uint8 gap = 5;               // 点间隔（可调）
//     const float angle_threshold = 120;  // 夹角阈值

//     for (uint16 i = 10; i + 4 * gap < data_stastics_r / 2; i++)
//     {
//         // 取 5 个间隔点
//         uint16 idx1 = i;
//         uint16 idx2 = i + gap;
//         uint16 idx3 = i + 2 * gap;
//         uint16 idx4 = i + 3 * gap;
//         uint16 idx5 = i + 4 * gap;

//         uint8 d1 = (uint8) dir_r[idx1];
//         uint8 d2 = (uint8) dir_r[idx2];
//         uint8 d3 = (uint8) dir_r[idx3];
//         uint8 d4 = (uint8) dir_r[idx4];
//         uint8 d5 = (uint8) dir_r[idx5];

//         if (((d1 == 4) || (d1 == 5) ) &&
//                 ((d2 == 4) || (d2 == 5) ) &&
//             ((d3 == 1) || (d3 == 2)) &&
//             ((d4 == 1) || (d4 == 2)) && ((d5 == 1) || (d5 == 2)))
//         {
//             // 计算夹角: 直线(1,2) 和 (4,5)
//             float angle = Compute_Angle_Between_Lines(
//                 points_r[idx1][0], points_r[idx1][1],
//                 points_r[idx2][0], points_r[idx2][1],
//                 points_r[idx4][0], points_r[idx4][1],
//                 points_r[idx5][0], points_r[idx5][1]
//             );
// //            printf("%f\r\n", angle);
//             if (angle > angle_threshold && points_r[idx3][1] >= 60)
//             {
//                 // 第3个点为拐点
//                 roundabout_a_point_x = points_r[idx3][0];
//                 roundabout_a_point_y = points_r[idx3][1];
//                 return 1;
//                 break;  // 只记录第一个检测到的拐点
//             }
//         }
//     }
//     roundabout_a_point_x = 0;
//     roundabout_a_point_y = 0;
//     return 0;
// }

uint8 Detect_Right_Roundabout_DownCorner(void) //右圆环下拐点检测
{
    static uint16 last_a_point_x = 0;
    static uint16 last_a_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_a_point = 0;

    const uint8 gap = 5;               // 点间隔（可调）
    const float angle_threshold = 120;  // 夹角阈值

    for (uint16 i = 10; i + 4 * gap < data_stastics_r / 2; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_r[idx1];
        uint8 d2 = (uint8) dir_r[idx2];
        uint8 d3 = (uint8) dir_r[idx3];
        uint8 d4 = (uint8) dir_r[idx4];
        uint8 d5 = (uint8) dir_r[idx5];

        if (((d1 == 4) || (d1 == 5) ) &&
                ((d2 == 4) || (d2 == 5) ) &&
            ((d3 == 1) || (d3 == 2)) &&
            ((d4 == 1) || (d4 == 2)) && ((d5 == 1) || (d5 == 2)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_r[idx1][0], points_r[idx1][1],
                points_r[idx2][0], points_r[idx2][1],
                points_r[idx4][0], points_r[idx4][1],
                points_r[idx5][0], points_r[idx5][1]
            );

            if (angle > angle_threshold && points_r[idx3][1] >= 60)
            {
                R_roundabout_a_point_x = points_r[idx3][0];
                R_roundabout_a_point_y = points_r[idx3][1];
                last_a_point_x = R_roundabout_a_point_x;
                last_a_point_y = R_roundabout_a_point_y;
                has_last_a_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_a_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            R_roundabout_a_point_x = last_a_point_x;
            R_roundabout_a_point_y = last_a_point_y;
            return 1;
        }
    }

    has_last_a_point = 0;
    miss_frame_count = 0;
    last_a_point_x = 0;
    last_a_point_y = 0;
    R_roundabout_a_point_x = 0;
    R_roundabout_a_point_y = 0;
    return 0;
}



// uint8 Detect_Right_Roundabout_UpCorner(void)  //右圆环上拐点检测
// {
//     const uint8 gap = 5;              // 取点间隔
//     const float angle_threshold = 50; // 夹角阈值（度）

//     for (uint16 i = data_stastics_r / 4; i + 4 * gap < data_stastics_r; i++)
//     {
//         uint16 idx1 = i;
//         uint16 idx2 = i + gap;
//         uint16 idx3 = i + 2 * gap;
//         uint16 idx4 = i + 3 * gap;
//         uint16 idx5 = i + 4 * gap;

//         uint8 d1 = (uint8)dir_r[idx1];
//         uint8 d2 = (uint8)dir_r[idx2];
//         uint8 d3 = (uint8)dir_r[idx3];
//         uint8 d4 = (uint8)dir_r[idx4];
//         uint8 d5 = (uint8)dir_r[idx5];

//         if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
//                 ((d1 == 0) || (d2 == 6) || (d2 == 7)) &&
//                 ((d3 == 4) || (d3 == 5)) &&
//                 ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
//                 ((d5 == 4) || (d5 == 5) || (d5 == 6)))
//         {
// //            float angle = Compute_Angle_Between_Lines(
// //                points_r[idx1][0], points_r[idx1][1],
// //                points_r[idx2][0], points_r[idx2][1],
// //                points_r[idx4][0], points_r[idx4][1],
// //                points_r[idx5][0], points_r[idx5][1]
// //            );
//             float angle = Compute_Angle_Between_Lines(
//                 points_r[idx5][0], points_r[idx5][1],
//                 points_r[idx4][0], points_r[idx4][1],
//                 points_r[idx2][0], points_r[idx2][1],
//                 points_r[idx1][0], points_r[idx1][1]
//             );

//             if (angle > angle_threshold)
//             {
// //                printf("%f\r\n", angle);
//                 roundabout_v_point_x = points_r[idx3][0];
//                 roundabout_v_point_y = points_r[idx3][1];
//                 return 1;
//             }
//         }
//     }
//     roundabout_v_point_x = 0;
//     roundabout_v_point_y = 0;
//     return 0;
// }

uint8 Detect_Right_Roundabout_UpCorner(void)  //右圆环上拐点检测
{
    static uint16 last_v_point_x = 0;
    static uint16 last_v_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_v_point = 0;

    const uint8 gap = 5;              // 取点间隔
    const float angle_threshold = 45; // 夹角阈值（度）

    for (uint16 i = data_stastics_r / 4; i + 4 * gap < data_stastics_r; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8)dir_r[idx1];
        uint8 d2 = (uint8)dir_r[idx2];
        uint8 d3 = (uint8)dir_r[idx3];
        uint8 d4 = (uint8)dir_r[idx4];
        uint8 d5 = (uint8)dir_r[idx5];

        if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
                ((d1 == 0) || (d2 == 6) || (d2 == 7)) &&
                ((d3 == 4) || (d3 == 5)) &&
                ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
                ((d5 == 4) || (d5 == 5) || (d5 == 6)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_r[idx5][0], points_r[idx5][1],
                points_r[idx4][0], points_r[idx4][1],
                points_r[idx2][0], points_r[idx2][1],
                points_r[idx1][0], points_r[idx1][1]
            );
            uint8 p_point_exists = (R_roundabout_p_point_y > 0);
            uint8 v_point_valid = !p_point_exists || (points_r[idx3][1] < R_roundabout_p_point_y);

            if (angle > angle_threshold && points_r[idx3][1] >= 20 && v_point_valid)
            {
//                printf("%f\r\n",angle);
                R_roundabout_v_point_x = points_r[idx3][0];
                R_roundabout_v_point_y = points_r[idx3][1];
                last_v_point_x = R_roundabout_v_point_x;
                last_v_point_y = R_roundabout_v_point_y;
                has_last_v_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_v_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            R_roundabout_v_point_x = last_v_point_x;
            R_roundabout_v_point_y = last_v_point_y;
            return 1;
        }
    }

    has_last_v_point = 0;
    miss_frame_count = 0;
    last_v_point_x = 0;
    last_v_point_y = 0;
    R_roundabout_v_point_x = 0;
    R_roundabout_v_point_y = 0;
    return 0;
}

//uint8 Detect_Right_Roundabout_leftdown(void)  //右圆环左下拐点检测
//{
//    const uint8 gap = 5;              // 取点间隔
//    const float angle_threshold = 70; // 夹角阈值（度）
//
//     for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
//     {
//         uint16 idx1 = i;
//         uint16 idx2 = i + gap;
//         uint16 idx3 = i + 2 * gap;
//         uint16 idx4 = i + 3 * gap;
//         uint16 idx5 = i + 4 * gap;
//
//         uint8 d1 = (uint8) dir_l[idx1];
//         uint8 d2 = (uint8) dir_l[idx2];
//         uint8 d3 = (uint8) dir_l[idx3];
//         uint8 d4 = (uint8) dir_l[idx4];
//         uint8 d5 = (uint8) dir_l[idx5];
//
//         if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
//                 ((d2 == 0) || (d2 == 6) || (d2 == 7)) &&
//             ((d3 == 4) || (d3 == 5) || (d3 == 6)) &&
//             ((d4 == 4) || (d4 == 5) || (d4 == 6)) && ((d5 == 4) || (d5 == 5) || (d5 == 6)))
//         {
//             // 计算两条直线(1,2)与(4,5)的夹角
//             float angle = Compute_Angle_Between_Lines(
//                 points_l[idx1][0], points_l[idx1][1],
//                 points_l[idx2][0], points_l[idx2][1],
//                 points_l[idx4][0], points_l[idx4][1],
//                 points_l[idx5][0], points_l[idx5][1]
//             );
//
//             if (angle > angle_threshold)
//             {
////                 printf("%f\r\n",angle);
//                 R_roundabout_ld_point_x = points_l[idx3][0];
//                 R_roundabout_ld_point_y = points_l[idx3][1];
//                 return 1;
//                 break;
//             }
//         }
//     }
//     R_roundabout_ld_point_x = 0;
//     R_roundabout_ld_point_y = 0;
//     return 0;
//}

uint8 Detect_Right_Roundabout_Outring(void)  //右圆环出环拐点检测
{
    static uint16 last_out_point_x = 0;
    static uint16 last_out_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_out_point = 0;

    const uint8 gap = 5;              // 取点间隔
    const float angle_threshold = 120; // 夹角阈值（度）

    for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_l[idx1];
        uint8 d2 = (uint8) dir_l[idx2];
        uint8 d3 = (uint8) dir_l[idx3];
        uint8 d4 = (uint8) dir_l[idx4];
        uint8 d5 = (uint8) dir_l[idx5];

        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
                ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
            ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
            ((d4 == 2) || (d4 == 3) || (d4 == 4)) && 
            ((d5 == 2) || (d5 == 3) || (d5 == 4)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_l[idx1][0], points_l[idx1][1],
                points_l[idx2][0], points_l[idx2][1],
                points_l[idx4][0], points_l[idx4][1],
                points_l[idx5][0], points_l[idx5][1]
            );

            if (angle > angle_threshold && points_l[idx3][1] > 40)
            {
//                printf("%f\r\n",angle);
                // 检测成功，更新坐标和记忆
                R_roundabout_out_point_x = points_l[idx3][0];
                R_roundabout_out_point_y = points_l[idx3][1];
                last_out_point_x = R_roundabout_out_point_x;
                last_out_point_y = R_roundabout_out_point_y;
                has_last_out_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    // 当前帧未检测到，使用上一帧坐标（最多保持5帧）
    if (has_last_out_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            R_roundabout_out_point_x = last_out_point_x;
            R_roundabout_out_point_y = last_out_point_y;
            return 1;
        }
    }

    // 连续5帧未检测到，清空所有记录
    has_last_out_point = 0;
    miss_frame_count = 0;
    last_out_point_x = 0;
    last_out_point_y = 0;
    R_roundabout_out_point_x = 0;
    R_roundabout_out_point_y = 0;
    return 0;
}

//uint8 Detect_cross_l_v(void)  //十字左下拐点检测
//{
//    static uint16 last_crossl_point_x = 0;
//    static uint16 last_crossl_point_y = 0;
//    static uint8 miss_frame_count = 0;
//    static uint8 has_last_crossl_point = 0;
//
//    const uint8 gap = 5;              // 取点间隔
//    const float angle_threshold = 120; // 夹角阈值（度）
//
//    for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
//    {
//        uint16 idx1 = i;
//        uint16 idx2 = i + gap;
//        uint16 idx3 = i + 2 * gap;
//        uint16 idx4 = i + 3 * gap;
//        uint16 idx5 = i + 4 * gap;
//
//        uint8 d1 = (uint8) dir_l[idx1];
//        uint8 d2 = (uint8) dir_l[idx2];
//        uint8 d3 = (uint8) dir_l[idx3];
//        uint8 d4 = (uint8) dir_l[idx4];
//        uint8 d5 = (uint8) dir_l[idx5];
//
//        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
//                ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
//            ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
//            ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
//            ((d5 == 2) || (d5 == 3) || (d5 == 4)))
//        {
//            float angle = Compute_Angle_Between_Lines(
//                points_l[idx1][0], points_l[idx1][1],
//                points_l[idx2][0], points_l[idx2][1],
//                points_l[idx4][0], points_l[idx4][1],
//                points_l[idx5][0], points_l[idx5][1]
//            );
//
//            if (angle > angle_threshold && points_l[idx3][1] > 40)
//            {
////                printf("%f\r\n",angle);
//                // 检测成功，更新坐标和记忆
//                l_cross_v_point_x = points_l[idx3][0];
//                l_cross_v_point_y = points_l[idx3][1];
//                last_crossl_point_x = l_cross_v_point_x;
//                last_crossl_point_y = l_cross_v_point_y;
//                has_last_crossl_point = 1;
//                miss_frame_count = 0;
//                return 1;
//            }
//        }
//    }
//
//    // 当前帧未检测到，使用上一帧坐标（最多保持5帧）
//    if (has_last_crossl_point && miss_frame_count < 5)
//    {
//        miss_frame_count++;
//        if (miss_frame_count < 5)
//        {
//            l_cross_v_point_x = last_crossl_point_x;
//            l_cross_v_point_y = last_crossl_point_y;
//            return 1;
//        }
//    }
//
//    // 连续5帧未检测到，清空所有记录
//    has_last_crossl_point = 0;
//    miss_frame_count = 0;
//    last_crossl_point_x = 0;
//    last_crossl_point_y = 0;
//    l_cross_v_point_x = 0;
//    l_cross_v_point_y = 0;
//    return 0;
//}

//float roundabout_v_angle_filtered = 0.0f;
//uint8 Detect_Right_Roundabout_UpCorner(void)  //右圆环上拐点检测
//{
//    const uint8 gap = 5;
//    const uint8 seg_len = 5;
//    const float angle_threshold = 20.0f;
//    const float roi_radius_sq = 28.0f * 28.0f;
//    const float alpha_iir = 0.3f;
//    const float resid_weight = 0.5f;
//
//    static uint8  has_last = 0;
//    static uint16 last_x = 0, last_y = 0;
//    static float  angle_iir = 0.0f;
//    static float  angle_hist[3] = {0};
//    static uint8  hist_cnt = 0;
//
//    float best_score = -1000.0f;
//    float best_angle = 0.0f;
//    uint16 best_idx = 0;
//
//    for (uint16 i = data_stastics_r / 4; i + 2 * gap + seg_len < data_stastics_r; i++)
//    {
//        uint16 idx1 = i;
//        uint16 idx2 = i + gap;
//        uint16 idx3 = i + 2 * gap;
//        uint16 idx4 = i + 3 * gap;
//        uint16 idx5 = i + 4 * gap;
//
//        if ((idx1 + seg_len > data_stastics_r) || (idx3 + seg_len > data_stastics_r))
//            continue;
//
//        uint8 d1 = (uint8)dir_r[idx1];
//        uint8 d2 = (uint8)dir_r[idx2];
//        uint8 d3 = (uint8)dir_r[idx3];
//        uint8 d4 = (uint8)dir_r[idx4];
//        uint8 d5 = (uint8)dir_r[idx5];
//
//        if (!((d1 == 6) || (d1 == 7)) ||
//            !((d2 == 6) || (d2 == 7)) ||
//            !((d3 == 4) || (d3 == 5)) ||
//            !((d4 == 4) || (d4 == 5) || (d4 == 6)) ||
//            !((d5 == 4) || (d5 == 5) || (d5 == 6)))
//        {
//            continue;
//        }
//
//        if (has_last)
//        {
//            float dx = (float)points_r[idx3][0] - (float)last_x;
//            float dy = (float)points_r[idx3][1] - (float)last_y;
//            if ((dx * dx + dy * dy) > roi_radius_sq)
//                continue;
//        }
//
//        float avg_resid = 0.0f;
//        float angle = Compute_Angle(points_r,
//                                   (int)idx1,
//                                    seg_len,
//                                    (int)idx3,
//                                     seg_len,
//                                   &avg_resid);
//        if (angle < angle_threshold)
//            continue;
//
//        float score = angle - resid_weight * avg_resid;
//        if (score > best_score)
//        {
//            best_score = score;
//            best_angle = angle;
//            best_idx = idx3;
//        }
//    }
//
//    if (best_score > 0.0f)
//    {
//        if (hist_cnt < 3)
//        {
//            angle_hist[hist_cnt++] = best_angle;
//        }
//        else
//        {
//            angle_hist[0] = angle_hist[1];
//            angle_hist[1] = angle_hist[2];
//            angle_hist[2] = best_angle;
//        }
//
//        float med;
//        if (hist_cnt == 1) med = angle_hist[0];
//        else if (hist_cnt == 2) med = 0.5f * (angle_hist[0] + angle_hist[1]);
//        else
//        {
//            float a = angle_hist[0], b = angle_hist[1], c = angle_hist[2];
//            if (a > b) { float t = a; a = b; b = t; }
//            if (b > c) { float t = b; b = c; c = t; }
//            if (a > b) { float t = a; a = b; b = t; }
//            med = b;
//        }
//
//        angle_iir = alpha_iir * med + (1.0f - alpha_iir) * angle_iir;
//        roundabout_v_angle_filtered = angle_iir;
//        printf("%f\r\n",roundabout_v_angle_filtered);
//
//        roundabout_v_point_x = points_r[best_idx][0];
//        roundabout_v_point_y = points_r[best_idx][1];
//        last_x = roundabout_v_point_x;
//        last_y = roundabout_v_point_y;
//        has_last = 1;
//
//        return 1;
//    }
//
//    roundabout_v_point_x = 0;
//    roundabout_v_point_y = 0;
//    return 0;
//}

// uint8 Detect_Right_Roundabout_ringpoint(void)  //右圆环凸点检测
// {
//     const uint8 gap = 5;              // 取点间隔
//     const float angle_threshold = 70; // 夹角阈值（度）

//     for (uint16 i = data_stastics_r / 4; i + 4 * gap < data_stastics_r; i++)
//     {
//         uint16 idx1 = i;
//         uint16 idx2 = i + gap;
//         uint16 idx3 = i + 2 * gap;
//         uint16 idx4 = i + 3 * gap;
//         uint16 idx5 = i + 4 * gap;

//         uint8 d1 = (uint8)dir_r[idx1];
//         uint8 d2 = (uint8)dir_r[idx2];
//         uint8 d3 = (uint8)dir_r[idx3];
//         uint8 d4 = (uint8)dir_r[idx4];
//         uint8 d5 = (uint8)dir_r[idx5];

//         if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
//                 ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
//                 ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
//                 ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
//                 ((d5 == 2) || (d5 == 3) || (d5 == 4)))
//         {
//             float angle = Compute_Angle_Between_Lines(
//                 points_r[idx1][0], points_r[idx1][1],
//                 points_r[idx2][0], points_r[idx2][1],
//                 points_r[idx4][0], points_r[idx4][1],
//                 points_r[idx5][0], points_r[idx5][1]
//             );

//             if (angle > angle_threshold)
//             {
// //                printf("%f\r\n",angle);
//                 roundabout_p_point_x = points_r[idx3][0];
//                 roundabout_p_point_y = points_r[idx3][1];
//                 return 1;
//             }
//         }
//     }
//     roundabout_p_point_x = 0;
//     roundabout_p_point_y = 0;
//     return 0;
// }

uint8 Detect_Right_Roundabout_ringpoint(void)  //右圆环凸点检测
{
    static uint16 last_p_point_x = 0;
    static uint16 last_p_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_p_point = 0;

    const uint8 gap = 5;              // 取点间隔
    const float angle_threshold = 20; // 夹角阈值（度）

    for (uint16 i = data_stastics_r / 4; i + 4 * gap < data_stastics_r; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8)dir_r[idx1];
        uint8 d2 = (uint8)dir_r[idx2];
        uint8 d3 = (uint8)dir_r[idx3];
        uint8 d4 = (uint8)dir_r[idx4];
        uint8 d5 = (uint8)dir_r[idx5];

        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
                ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
                ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
                ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
                ((d5 == 2) || (d5 == 3) || (d5 == 4)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_r[idx1][0], points_r[idx1][1],
                points_r[idx2][0], points_r[idx2][1],
                points_r[idx4][0], points_r[idx4][1],
                points_r[idx5][0], points_r[idx5][1]
            );

            if ((angle > angle_threshold) && (points_r[idx3][1] > 50) && (points_r[idx3][0] >= 90))
            {
                printf("%f\r\n",angle);
                R_roundabout_p_point_x = points_r[idx3][0];
                R_roundabout_p_point_y = points_r[idx3][1];
                last_p_point_x = R_roundabout_p_point_x;
                last_p_point_y = R_roundabout_p_point_y;
                has_last_p_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_p_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            R_roundabout_p_point_x = last_p_point_x;
            R_roundabout_p_point_y = last_p_point_y;
            return 1;
        }
    }

    has_last_p_point = 0;
    miss_frame_count = 0;
    last_p_point_x = 0;
    last_p_point_y = 0;
    R_roundabout_p_point_x = 0;
    R_roundabout_p_point_y = 0;
    return 0;
}

//uint8 Detect_Left_Roundabout_DownCorner(void)  //左环岛下拐点检测
//{
//    const uint8 gap = 5;              // 取点间隔
//    const float angle_threshold = 80; // 夹角阈值（度）
//
//    for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
//    {
//        uint16 idx1 = i;
//        uint16 idx2 = i + gap;
//        uint16 idx3 = i + 2 * gap;
//        uint16 idx4 = i + 3 * gap;
//        uint16 idx5 = i + 4 * gap;
//
//        uint8 d1 = (uint8) dir_l[idx1];
//        uint8 d2 = (uint8) dir_l[idx2];
//        uint8 d3 = (uint8) dir_l[idx3];
//        uint8 d4 = (uint8) dir_l[idx4];
//        uint8 d5 = (uint8) dir_l[idx5];
//
//        if (((d1 == 4) || (d1 == 5)) &&
//                ((d2 == 4) || (d2 == 5)) &&
//            ((d3 == 0) || (d3 == 1) || (d3 == 2)) &&
//            ((d4 == 0) || (d4 == 1) || (d4 == 2)) && ((d5 == 0) || (d5 == 1) || (d5 == 2)))
//        {
//            // 计算两条直线(1,2)与(4,5)的夹角
//            float angle = Compute_Angle_Between_Lines(
//                points_l[idx1][0], points_l[idx1][1],
//                points_l[idx2][0], points_l[idx2][1],
//                points_l[idx4][0], points_l[idx4][1],
//                points_l[idx5][0], points_l[idx5][1]
//            );
//
//            if (angle > angle_threshold)
//            {
//                // 第3个点为拐点
//                roundabout_a_point_x = points_l[idx3][0];
//                roundabout_a_point_y = points_l[idx3][1];
//                return 1;
//                break;
//            }
//        }
//    }
//    return 0;
//}

uint8 Detect_Left_Roundabout_DownCorner(void)   //左环岛下拐点检测
{
    static uint16 last_a_point_x = 0;
    static uint16 last_a_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_a_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 80;

    for (uint16 i = 0; i + 4 * gap < data_stastics_l / 2; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_l[idx1];
        uint8 d2 = (uint8) dir_l[idx2];
        uint8 d3 = (uint8) dir_l[idx3];
        uint8 d4 = (uint8) dir_l[idx4];
        uint8 d5 = (uint8) dir_l[idx5];

        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
            ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
            ((d3 == 0) || (d3 == 1) || (d3 == 2)) &&
            ((d4 == 0) || (d4 == 1) || (d4 == 2)) &&
            ((d5 == 0) || (d5 == 1) || (d5 == 2)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_l[idx1][0], points_l[idx1][1],
                points_l[idx2][0], points_l[idx2][1],
                points_l[idx4][0], points_l[idx4][1],
                points_l[idx5][0], points_l[idx5][1]
            );

            if (angle > angle_threshold && points_l[idx3][1] >= 60)
            {
                L_roundabout_a_point_x = points_l[idx3][0];
                L_roundabout_a_point_y = points_l[idx3][1];
                last_a_point_x = L_roundabout_a_point_x;
                last_a_point_y = L_roundabout_a_point_y;
                has_last_a_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_a_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            L_roundabout_a_point_x = last_a_point_x;
            L_roundabout_a_point_y = last_a_point_y;
            return 1;
        }
    }

    has_last_a_point = 0;
    miss_frame_count = 0;
    last_a_point_x = 0;
    last_a_point_y = 0;
    L_roundabout_a_point_x = 0;
    L_roundabout_a_point_y = 0;
    return 0;
}

//uint8 Detect_Left_Roundabout_UpCorner(void)  //左圆环上拐点检测
//{
//    const uint8 gap = 5;              // 取点间隔
//    const float angle_threshold = 90; // 夹角阈值（度）
//
//    for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
//    {
//        uint16 idx1 = i;
//        uint16 idx2 = i + gap;
//        uint16 idx3 = i + 2 * gap;
//        uint16 idx4 = i + 3 * gap;
//        uint16 idx5 = i + 4 * gap;
//
//        uint8 d1 = (uint8)dir_l[idx1];
//        uint8 d2 = (uint8)dir_l[idx2];
//        uint8 d3 = (uint8)dir_l[idx3];
//        uint8 d4 = (uint8)dir_l[idx4];
//        uint8 d5 = (uint8)dir_l[idx5];
//
//        // 生长方向模式: 左→左上→上→右上/右
//        if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
//                ((d2 == 0) || (d2 == 6) || (d2 == 7)) &&
//                ((d3 == 4) || (d3 == 5) || (d3 == 6)) &&
//                ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
//                ((d5 == 4) || (d5 == 5) || (d5 == 6)))
//        {
//            float angle = Compute_Angle_Between_Lines(
//                points_l[idx1][0], points_l[idx1][1],
//                points_l[idx2][0], points_l[idx2][1],
//                points_l[idx4][0], points_l[idx4][1],
//                points_l[idx5][0], points_l[idx5][1]
//            );
//
//            if (angle > angle_threshold)
//            {
////                printf("%f\r\n",angle);
//                roundabout_v_point_x = points_l[idx3][0];
//                roundabout_v_point_y = points_l[idx3][1];
//                return 1;
//            }
//        }
//    }
//    return 0;
//}

uint8 Detect_Left_Roundabout_UpCorner(void) //左圆环上拐点检测
{
    static uint16 last_v_point_x = 0;
    static uint16 last_v_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_v_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 50;

    for (uint16 i = data_stastics_l / 4; i + 4 * gap < data_stastics_l; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8)dir_l[idx1];
        uint8 d2 = (uint8)dir_l[idx2];
        uint8 d3 = (uint8)dir_l[idx3];
        uint8 d4 = (uint8)dir_l[idx4];
        uint8 d5 = (uint8)dir_l[idx5];

        if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
            ((d2 == 0) || (d2 == 6) || (d2 == 7)) &&
            ((d3 == 4) || (d3 == 5) ) &&
            ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
            ((d5 == 4) || (d5 == 5) || (d5 == 6)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_l[idx5][0], points_l[idx5][1],
                points_l[idx4][0], points_l[idx4][1],
                points_l[idx2][0], points_l[idx2][1],
                points_l[idx1][0], points_l[idx1][1]
            );

            // P点存在性检查 (如果有P点,V点y值必须小于P点)
            uint8 p_point_exists = (L_roundabout_p_point_y > 0);
            uint8 v_point_valid = !p_point_exists || (points_l[idx3][1] < L_roundabout_p_point_y);

            if (angle > angle_threshold && v_point_valid && points_l[idx3][1] > 30)
            {
//                printf("%f\r\n",angle);
                L_roundabout_v_point_x = points_l[idx3][0];
                L_roundabout_v_point_y = points_l[idx3][1];
                last_v_point_x = L_roundabout_v_point_x;
                last_v_point_y = L_roundabout_v_point_y;
                has_last_v_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_v_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            L_roundabout_v_point_x = last_v_point_x;
            L_roundabout_v_point_y = last_v_point_y;
            return 1;
        }
    }

    has_last_v_point = 0;
    miss_frame_count = 0;
    last_v_point_x = 0;
    last_v_point_y = 0;
    L_roundabout_v_point_x = 0;
    L_roundabout_v_point_y = 0;
    return 0;
}

//uint8 Detect_Left_Roundabout_ringpoint(void)  //左圆环凸点检测
//{
//    const uint8 gap = 2;              // 取点间隔
//    const float angle_threshold = 10; // 夹角阈值（度）
//
//    for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
//    {
//        uint16 idx1 = i;
//        uint16 idx2 = i + gap;
//        uint16 idx3 = i + 2 * gap;
//        uint16 idx4 = i + 3 * gap;
//        uint16 idx5 = i + 4 * gap;
//
//        uint8 d1 = (uint8)dir_l[idx1];
//        uint8 d2 = (uint8)dir_l[idx2];
//        uint8 d3 = (uint8)dir_l[idx3];
//        uint8 d4 = (uint8)dir_l[idx4];
//        uint8 d5 = (uint8)dir_l[idx5];
//
//        // 生长方向模式: 左→左上→上→右上/右
//        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
//                ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
//                ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
//                ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
//                ((d5 == 2) || (d5 == 3) || (d5 == 4)))
//        {
//            float angle = Compute_Angle_Between_Lines(
//                points_l[idx1][0], points_l[idx1][1],
//                points_l[idx2][0], points_l[idx2][1],
//                points_l[idx4][0], points_l[idx4][1],
//                points_l[idx5][0], points_l[idx5][1]
//            );
//
//            if (angle > angle_threshold)
//            {
//                roundabout_p_point_x = points_l[idx3][0];
//                roundabout_p_point_y = points_l[idx3][1];
//                return 1;
//            }
//        }
//    }
//    return 0;
//}

uint8 Detect_Left_Roundabout_ringpoint(void)    //左圆环环岛点检测
{
    static uint16 last_p_point_x = 0;
    static uint16 last_p_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_p_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 5;

    for (uint16 i = data_stastics_l / 4; i + 4 * gap < data_stastics_l; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8)dir_l[idx1];
        uint8 d2 = (uint8)dir_l[idx2];
        uint8 d3 = (uint8)dir_l[idx3];
        uint8 d4 = (uint8)dir_l[idx4];
        uint8 d5 = (uint8)dir_l[idx5];

        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
            ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
            ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
            ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
            ((d5 == 2) || (d5 == 3) || (d5 == 4)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_l[idx1][0], points_l[idx1][1],
                points_l[idx2][0], points_l[idx2][1],
                points_l[idx4][0], points_l[idx4][1],
                points_l[idx5][0], points_l[idx5][1]
            );

            // P点位置约束: y > 40 且 x <= 图像中线 (镜像右圆环的 x >= 90)
            if ((angle > angle_threshold) &&
                (points_l[idx3][1] > 40) &&
                (points_l[idx3][0] <= (image_w / 2)))
            {
                L_roundabout_p_point_x = points_l[idx3][0];
                L_roundabout_p_point_y = points_l[idx3][1];
                last_p_point_x = L_roundabout_p_point_x;
                last_p_point_y = L_roundabout_p_point_y;
                has_last_p_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_p_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            L_roundabout_p_point_x = last_p_point_x;
            L_roundabout_p_point_y = last_p_point_y;
            return 1;
        }
    }

    has_last_p_point = 0;
    miss_frame_count = 0;
    last_p_point_x = 0;
    last_p_point_y = 0;
    L_roundabout_p_point_x = 0;
    L_roundabout_p_point_y = 0;
    return 0;
}

uint8 Detect_Left_Roundabout_Outring(void)  //左圆环出环点检测
{
    static uint16 last_out_point_x = 0;
    static uint16 last_out_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_out_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 120;

    // 在右边界搜索 (镜像右圆环的左边界)
    for (uint16 i = 0; i + 4 * gap < data_stastics_r; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_r[idx1];
        uint8 d2 = (uint8) dir_r[idx2];
        uint8 d3 = (uint8) dir_r[idx3];
        uint8 d4 = (uint8) dir_r[idx4];
        uint8 d5 = (uint8) dir_r[idx5];

        // 方向: 下→下左→左 (镜像右圆环出环点)
        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
            ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
            ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
            ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
            ((d5 == 2) || (d5 == 3) || (d5 == 4)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_r[idx1][0], points_r[idx1][1],
                points_r[idx2][0], points_r[idx2][1],
                points_r[idx4][0], points_r[idx4][1],
                points_r[idx5][0], points_r[idx5][1]
            );

            if (angle > angle_threshold && points_r[idx3][1] > 40)
            {
                L_roundabout_out_point_x = points_r[idx3][0];
                L_roundabout_out_point_y = points_r[idx3][1];
                last_out_point_x = L_roundabout_out_point_x;
                last_out_point_y = L_roundabout_out_point_y;
                has_last_out_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_out_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            L_roundabout_out_point_x = last_out_point_x;
            L_roundabout_out_point_y = last_out_point_y;
            return 1;
        }
    }

    has_last_out_point = 0;
    miss_frame_count = 0;
    last_out_point_x = 0;
    last_out_point_y = 0;
    L_roundabout_out_point_x = 0;
    L_roundabout_out_point_y = 0;
    return 0;
}


//void Draw_Cross_Marker(uint16 (*image_buffer)[image_w], uint16 x, uint16 y, uint8 size, uint16 color)
//{
//    for(int i = -size; i <= size; i++)
//    {
//        // 确保坐标在有效范围内
//        if(y + i >= 0 && y + i < image_h && x + i >= 0 && x + i < image_w)
//            image_buffer[y + i][x + i] = color; // 直接在内存数组中写入颜色值
//        if(y - i >= 0 && y - i < image_h && x + i >= 0 && x + i < image_w)
//            image_buffer[y - i][x + i] = color; // 直接在内存数组中写入颜色值
//    }
//}

void Draw_Cross_Marker(uint16 x, uint16 y, uint8 size, uint16 color)
{
    for(int i = -size; i <= size; i++)
    {
        if(x + i >= 0 && x + i < image_w && y + i >= 0 && y + i < image_h)
            ips200_draw_point(x + i, y + i, color);
        if(x + i >= 0 && x + i < image_w && y - i >= 0 && y - i < image_h)
            ips200_draw_point(x + i, y - i, color);
    }
}

void Connect_Border_Points(uint8* border, int x1, int y1, int x2, int y2)  //补线函数
{
    //保证 y1 < y2，方便后续的 for 循环从上往下遍历
    if (y1 > y2)
    {
        // 如果y1比y2大，则交换两个点的所有坐标
        int temp_x = x1; int temp_y = y1;
        x1 = x2; y1 = y2;
        x2 = temp_x; y2 = temp_y;
    }

    //处理边界情况，如果两个点在同一行，则无需补线
    if (y1 == y2) return;

    //遍历两个点之间的每一行
    for (int y = y1; y <= y2; y++)
    {
        // 确保当前行号在图像的有效范围内
        if (y >= 0 && y < image_h)
        {
            //计算当前行y对应的x坐标
            int x = x1 + (int)((float)(x2 - x1) * (float)(y - y1) / (float)(y2 - y1));

            //写入边界数组，防越界
            border[y] = (uint8)limit_a_b((int16) x, border_min, border_max);
        }
    }
}

float r_squared = 0;

uint8 Is_Border_Continue(uint8* border, uint8 start_row, uint8 end_row, uint8 border_type, float r2_threshold) //1:左边界  0:右边界
{
//    if (start_row <= end_row) return 0;
//
//    const int SAMPLE_STEP = 5;
//    const int MIN_SAMPLES = 10;      // 最少样本数
//    const float R2_THRESHOLD = 0.98f; // R^2 判定阈值
//
//    float sum_x = 0.0f;     // Σx
//    float sum_y = 0.0f;     // Σy
//    float sum_xy = 0.0f;    // Σ(xy)
//    float sum_xx = 0.0f;    // Σ(x²)
//    float sum_yy = 0.0f;    // Σ(y²)
//    int n = 0;              // 有效点计数
//
//    int sample_index = 0;
//    for (int row = (int)start_row; row > (int)end_row; row -= SAMPLE_STEP)
//    {
//        uint8 border_val = border[row];
//
//        if ((border_type == 1 && border_val == border_min) ||
//            (border_type == 0 && border_val == border_max))
//        {
//            continue;
//        }
//
//        float x = (float)sample_index; // 使用样本索引作为 x
//        float y = (float)border_val;
//
//        sum_x += x;
//        sum_y += y;
//        sum_xy += x * y;
//        sum_xx += x * x;
//        sum_yy += y * y;
//
//        n++;
//        sample_index++;
//    }
//
//    // 如果有效点数太少，无法进行拟合
//    if (n < MIN_SAMPLES) return 0;
//
//    // 计算相关系数 R²
//    float numerator = n * sum_xy - sum_x * sum_y;
//    float denom_x = n * sum_xx - sum_x * sum_x;
//    float denom_y = n * sum_yy - sum_y * sum_y;
//
//    if (denom_x <= 0.0f || denom_y <= 0.0f) return 0;
//
//    float denominator = sqrtf(denom_x * denom_y);
//    if (denominator < 1e-6f) return 0;
//
//    float r = numerator / denominator;
//    r_squared = r * r;
//
//    return (r_squared > R2_THRESHOLD) ? 1 : 0;
    // 安全检查
    if (start_row <= end_row || start_row >= image_h || end_row < 1)
        return 0;

    // 参数限制
    if (r2_threshold < 0.0f) r2_threshold = 0.0f;
    if (r2_threshold > 1.0f) r2_threshold = 1.0f;

    const int SAMPLE_STEP = 5;        // 采样步长
    const int MIN_VALID_SAMPLES = 5;  // 最少有效样本数

    // 🔧 步骤1: 收集有效点
    uint8 valid_x[64];  // 存储有效点的x坐标(边界值)
    uint8 valid_y[64];  // 存储有效点的y坐标(行号)
    int valid_count = 0;

    for (int row = start_row; row > end_row && valid_count < 64; row -= SAMPLE_STEP)
    {
        uint8 border_val = border[row];

        // 🔧 关键: 判断并丢弃无效点
        uint8 is_valid = 0;

        if (border_type == 1)  // 左边界
        {
            // 左边界有效条件: 不是最小值(border_min)且在合理范围内
            if (border_val > border_min && border_val < border_max)
                is_valid = 1;
        }
        else  // 右边界
        {
            // 右边界有效条件: 不是最大值(border_max)且在合理范围内
            if (border_val < border_max && border_val > border_min)
                is_valid = 1;
        }

        // 只保留有效点
        if (is_valid)
        {
            valid_x[valid_count] = border_val;
            valid_y[valid_count] = (uint8)row;
            valid_count++;
        }
    }
//    printf("%d\r\n",valid_count);

    // 🔧 步骤2: 检查有效点数量
    if (valid_count < MIN_VALID_SAMPLES)
    {
        return 0;  // 有效点太少,认为不连续
    }

    // 🔧 步骤3: 用有效点计算R² (以y为自变量, x=ky+b)
    float sum_x = 0.0f;
    float sum_y = 0.0f;
    float sum_xy = 0.0f;
    float sum_yy = 0.0f;
    float sum_xx = 0.0f;

    for (int i = 0; i < valid_count; i++)
    {
        float x = (float)valid_x[i];
        float y = (float)valid_y[i];

        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_xx += x * x;
        sum_yy += y * y;
    }

    // 🔧 步骤4: 计算相关系数R
    float n = (float)valid_count;
    float numerator = n * sum_xy - sum_x * sum_y;
    float denom_x = n * sum_xx - sum_x * sum_x;
    float denom_y = n * sum_yy - sum_y * sum_y;

    // 防止除零
    if (denom_x <= 0.0f || denom_y <= 0.0f)
        return 0;

    float denominator = sqrtf(denom_x * denom_y);
    if (denominator < 1e-6f)
        return 0;

    float r = numerator / denominator;
    r_squared = r * r;

    // 🔧 步骤5: 与阈值比较
    return (r_squared > r2_threshold) ? 1 : 0;
}

//void Test_Right_Roundabout_Entry(void)
//{
//    // 静态变量，用于“记忆”上一次成功检测到的V点坐标
//     static uint8  has_seen_v_point = 0;      // 标志位，表示是否至少成功检测过一次V点
//     static uint16 last_known_v_point_x = 0;  // 上一次有效V点的X坐标
//     static uint16 last_known_v_point_y = 0;  // 上一次有效V点的Y坐标
//
//     // --- 逻辑核心 ---
//
//     // 步骤 1: 尝试在当前帧中检测上拐点 (V点)
//     uint8 is_up_corner_detected = Detect_Right_Roundabout_UpCorner();
//
//     // 步骤 2: 如果在当前帧成功检测到V点
//     if (is_up_corner_detected)
//     {
//         // ...就用最新的（经过滤波的）坐标更新我们的“记忆”
//         last_known_v_point_x = roundabout_v_point_x;
//         last_known_v_point_y = roundabout_v_point_y;
//
//         // 同时，设置标志位，表明我们现在有了一组有效的坐标可供使用
//         has_seen_v_point = 1;
//     }
//
//     // 步骤 3: 只要我们曾经成功检测过V点...
//     if (has_seen_v_point)
//     {
//         // ...就始终执行补线操作。
//         // 如果本帧检测成功，这里使用的是更新后的坐标。
//         // 如果本帧检测失败，这里使用的是“记忆”中上一帧的坐标。
//         Connect_Border_Points(l_border,
//                               last_known_v_point_x,
//                               last_known_v_point_y,
//                               l_border[image_h - 2],
//                               image_h - 2);
//     }
//}

uint8 cnt = 0;
uint8 flag1 = 0;
//void Detect_Right_Roundabout(void)
//{
//        uint8 is_a_detected = Detect_Right_Roundabout_DownCorner();
//        uint8 is_p_detected = Detect_Right_Roundabout_ringpoint();
//        uint8 is_v_detected = Detect_Right_Roundabout_UpCorner();
//        uint8 is_out_detected = Detect_Right_Roundabout_Outring();
////        uint8 is_ld_detected = Detect_Right_Roundabout_leftdown();
//
//        static uint8 has_last_out_point = 0;
//        static uint16 last_out_x = 0;
//        static uint16 last_out_y = 0;
//
//        // 状态 0: 正常巡线，检测入口
//        if (roundabout_state == 0)
//        {
//            // 入口条件：当前帧检测到A点 且 左边界连续
//            if (is_a_detected && is_left_continuous)
//            {
//                roundabout_state = 1;
////                roundabout_type = 2;
//            }
//        }
//        else if (roundabout_state == 1)
//        {
//            if (is_p_detected && is_a_detected)
//            {
//                Connect_Border_Points(r_border,
//                                      roundabout_a_point_x, roundabout_a_point_y,
//                                      roundabout_p_point_x, roundabout_p_point_y);
//            }
//            else if (is_p_detected && !is_a_detected)
//            {
//                Connect_Border_Points(r_border,
//                                      r_border[image_h - 2], image_h - 2,
//                                      roundabout_p_point_x, roundabout_p_point_y);
//            }
//            else if (is_v_detected)
//            {
//                roundabout_state = 2;
//
//            }
//
//
//
//        }
//        else if (roundabout_state == 2)
//        {
//            if (is_v_detected)
//            {
//                if (roundabout_v_point_y < 60)
//                {
//                        Connect_Border_Points(l_border,
//                             roundabout_v_point_x, roundabout_v_point_y,
//                               0, 80);
//                        flag1 = 1;
//                }
//                else
//                {
//                        Connect_Border_Points(l_border,
//                          roundabout_v_point_x, roundabout_v_point_y,
//                            5, image_h - 2);
//                        Connect_Border_Points(l_border,
//                          roundabout_v_point_x, roundabout_v_point_y,
//                            image_w - 2, 20);
//                        flag2 = 1;
//                }
//
//            }
////            else if (!is_v_detected && is_ld_detected)
////            {
////                Connect_Border_Points(l_border,
////                                      roundabout_ld_point_x, roundabout_ld_point_y,
////                                      0, image_h - 2);
////            }
//            else if (!is_v_detected && flag1 == 1 && flag2 == 1)
//            {
//                roundabout_state = 3;
//                flag1 = 0;
//                flag2 = 0;
//            }
//
//        }
//        else if (roundabout_state == 3)
//        {
//            if (is_out_detected)
//            {
//                last_out_x = roundabout_out_point_x;
//                last_out_y = roundabout_out_point_y;
//                has_last_out_point = 1;
//            }
//            // 只要有过有效出环点,就持续补线
//            if (has_last_out_point)
//            {
//                Connect_Border_Points(l_border,
//                                      last_out_x, last_out_y,
//                                      image_w - 2, 0);
//            }
//            // 退出条件：左边界恢复连续
//            if (is_left_continuous)
//            {
//                roundabout_state = 0;  // 回到正常巡线
//                has_last_out_point = 0;
//                last_out_x = 0;
//                last_out_y = 0;
//            }
//        }
//}

uint16 Vy = 30;
uint16 P_line = 90;

void Detect_Right_Roundabout(void)  //右圆环状态机
{
        uint8 is_a_detected = Detect_Right_Roundabout_DownCorner();
        uint8 is_p_detected = Detect_Right_Roundabout_ringpoint();
        uint8 is_v_detected = Detect_Right_Roundabout_UpCorner();
        uint8 is_out_detected = Detect_Right_Roundabout_Outring();
        is_left_continuous = Is_Border_Continue(l_border, image_h - 20, 30, 1, 0.98f);

        static uint8  has_last_out_point = 0;
        static uint16 last_out_x = 0;
        static uint16 last_out_y = 0;

        static uint8 v_miss_cnt = 0;   
        static uint8 v_found_cnt = 0;
        static uint8 exit_cont_cnt = 0; 
        static uint8 out_recent_cnt = 0; 


//        static uint8 v_stable_cnt = 0;   // V点稳定出现计数
//        static uint16 last_v_y = 0;      // 记录上一次V点的y坐标

        if (R_roundabout_state == 0)
        {
            if ((is_a_detected && is_left_continuous))
            {
                R_roundabout_state = 1;
            }
        }
        else if (R_roundabout_state  == 1)
        {
//            set_speed = 80;
            if (is_p_detected && is_a_detected)
            {
                Connect_Border_Points(r_border,
                        R_roundabout_a_point_x, R_roundabout_a_point_y,
                        R_roundabout_p_point_x, R_roundabout_p_point_y);
            }
            else if (!is_p_detected && is_a_detected)
            {
                Extend_Border_By_Slope(r_border, R_roundabout_a_point_x, R_roundabout_a_point_y, 15, 25);
            }
            else if (is_p_detected && !is_a_detected)
            {
                Connect_Border_Points(r_border,
                                      r_border[image_h - 2], image_h - 2,
                                      R_roundabout_p_point_x, R_roundabout_p_point_y);
            }
            else if (is_v_detected)
            {
                R_roundabout_state = 2;
                v_miss_cnt = 0;
                flag1 = 0;
            }
        }
        else if (R_roundabout_state  == 2)
        {
//            set_speed = 80;
            if (is_v_detected)
            {
                v_miss_cnt = 0;

                if (R_roundabout_v_point_y > Vy)
                {
                    Connect_Border_Points(l_border,
                            R_roundabout_v_point_x, R_roundabout_v_point_y,
                        0, P_line);
                    Connect_Border_Points(l_border,
                            R_roundabout_v_point_x, R_roundabout_v_point_y,
                         image_w - 2, 30);
                    flag1 = 1;

                }
//                else
//                {
//                    Connect_Border_Points(l_border,
//                        roundabout_v_point_x, roundabout_v_point_y,
//                        5, image_h - 2);
//                    Connect_Border_Points(l_border,
//                        roundabout_v_point_x, roundabout_v_point_y,
//                        image_w - 2, 50);
//                    flag2 = 1;
//                }
            }
            else
            {
                if (flag1)
                {
                    if (++v_miss_cnt >= 20)  // 连续20帧未见V才进入出环
                    {
                        R_roundabout_state = 3;
                        flag1 = 0;
                        has_last_out_point = 0;
                        last_out_x = last_out_y = 0;
                        exit_cont_cnt = 0;
                        out_recent_cnt = 0;
                    }
                }
            }
        }
        else if (R_roundabout_state == 3)
        {
//            set_speed = 90;
            if (is_out_detected)
            {
                last_out_x = R_roundabout_out_point_x;
                last_out_y = R_roundabout_out_point_y;
                has_last_out_point = 1;
                out_recent_cnt = 10; // 维持10帧“最近有效”
            }
            if (out_recent_cnt) out_recent_cnt--;

//            // 只要有过有效出环点，就持续补线到右上角
//            if (has_last_out_point)
//            {
//                Connect_Border_Points(l_border,
//                                      last_out_x, last_out_y,
//                                      image_w - 2, 0);
//            }
//            if (is_v_detected)
//             {
//                 Connect_Border_Points(r_border,
//                                       R_roundabout_v_point_x, R_roundabout_v_point_y,
//                                       r_border[image_h - 2], image_h - 2);
//             }
            if (is_v_detected && R_roundabout_v_point_y > 30)
            {
                Connect_Border_Points(r_border,
                                      R_roundabout_v_point_x, R_roundabout_v_point_y,
                                      r_border[image_h - 2], image_h - 2);

            }
            else if (has_last_out_point && out_recent_cnt > 0)
            {
                Connect_Border_Points(l_border,
                                      last_out_x, last_out_y,
                                      image_w - 2, 0);
            }

           if (is_v_detected && !is_out_detected)
           {
//               Connect_Border_Points(r_border,
//                       R_roundabout_v_point_x, R_roundabout_v_point_y,
//                                      r_border[image_h - 2], image_h - 2);
                if (++v_found_cnt >= 2 && R_roundabout_v_point_y > 70)
                {
                     //退出环岛
                     R_roundabout_state  = 0;
                     has_last_out_point = 0;
                     last_out_x = last_out_y = 0;
                     exit_cont_cnt = 0;
                     v_miss_cnt = 0;
                     v_found_cnt = 0;
                }
           }
           else
           {
                v_found_cnt = 0;
           }
       }
//        else if (roundabout_state == 3)
//        {
//            set_speed = 100;
//
//            // 🔧 修改点1: 出环点补线逻辑
//            if (is_out_detected)
//            {
//                last_out_x = roundabout_out_point_x;
//                last_out_y = roundabout_out_point_y;
//                has_last_out_point = 1;
//                out_recent_cnt = 15; // 增加衰减时间
//            }
//            if (out_recent_cnt) out_recent_cnt--;
//
//            // 持续补线
//            if (has_last_out_point && out_recent_cnt > 0)
//            {
//                Connect_Border_Points(l_border,
//                                      last_out_x, last_out_y,
//                                      image_w - 2, 0);
//            }
//
//            // 🔧 修改点2: V点检测与右边界补线
//            if (is_v_detected)
//            {
//                // 补右边界
//                Connect_Border_Points(r_border,
//                                      roundabout_v_point_x, roundabout_v_point_y,
//                                      r_border[image_h - 2], image_h - 2);
//
//                // 🔧 判断V点是否稳定下降(y值增大表示下移)
//                if (last_v_y > 0)
//                {
//                    // V点在下降(y坐标增大)
//                    if (roundabout_v_point_y > last_v_y)
//                    {
//                        v_stable_cnt++;
//                    }
//                    else
//                    {
//                        v_stable_cnt = 0; // 重置计数
//                    }
//                }
//                last_v_y = roundabout_v_point_y;
//
//                v_found_cnt = 0; // 重置未检测到计数
//            }
//            else
//            {
//                v_found_cnt++;
//
//                // 🔧 修改点3: 同时满足多个条件才退出
//                // 条件1: V点连续稳定下降超过8帧
//                // 条件2: V点消失超过3帧
//                // 条件3: 左边界连续
//                // 条件4: V点最后位置足够低(y > 80)
//                if (v_stable_cnt >= 8 &&
//                    v_found_cnt >= 3 &&
//                    last_v_y > 80)
//                {
//                    // 退出环岛
//                    roundabout_state = 0;
//                    has_last_out_point = 0;
//                    last_out_x = last_out_y = 0;
//                    exit_cont_cnt = 0;
//                    v_miss_cnt = 0;
//                    v_found_cnt = 0;
//                    v_stable_cnt = 0;
//                    last_v_y = 0;
//                    set_speed = 100;
//                }
//            }
//        }
}

//void Detect_Left_Roundabout(void)  //左圆环状态机
//{
//    uint8 is_a_detected = Detect_Left_Roundabout_DownCorner();
//    uint8 is_p_detected = Detect_Left_Roundabout_ringpoint();
//    uint8 is_v_detected = Detect_Left_Roundabout_UpCorner();
//    uint8 is_out_detected = Detect_Left_Roundabout_Outring();
//    is_right_continuous = Is_Border_Continue(r_border, image_h - 20, 30, 0, 0.90f);
//
//    static uint8  has_last_out_point = 0;
//    static uint16 last_out_x = 0;
//    static uint16 last_out_y = 0;
//
//    static uint8 v_miss_cnt = 0;     // 连续未见V计数
//    static uint8 v_found_cnt = 0;
//    static uint8 exit_cont_cnt = 0;  // 连续"右边界连续"计数
//    static uint8 out_recent_cnt = 0; // 出环点最近有效计数
//
//    static uint8 slope_fix_frames = 0;  // 斜率补线持续帧数
//    const uint8 MAX_SLOPE_FIX_FRAMES = 70; // 最多补线帧
//
//    // ==================== 状态 0: 正常巡线，检测入口 ====================
//    if (roundabout_state == 0)
//    {
//        // 🔧 入口条件: 检测到A点(下拐点) 且 右边界连续
//        if (is_a_detected && is_right_continuous)
//        {
//            roundabout_state = 1;
//            slope_fix_frames = 0;
//        }
//    }
//    // ==================== 状态 1: 入环阶段，补左边界 A→P ====================
//    else if (roundabout_state == 1)
//    {
////        set_speed = 60;
//        if (is_a_detected && slope_fix_frames < MAX_SLOPE_FIX_FRAMES)
//         {
//             // 参数说明:
//             // - fit_length=15: 用A点下方15个点拟合斜率
//             // - extend_to_row=25: 向上补到第25行
//             Extend_Border_By_Slope(l_border,
//                                    roundabout_a_point_x,
//                                    roundabout_a_point_y,
//                                    15,  // 拟合长度
//                                    25); // 延伸到第25行
//             slope_fix_frames++;
//         }
//        // 🔧 补线策略1: 同时有A点和P点，直接连接
//        else if (is_p_detected && is_a_detected)
//        {
//            Connect_Border_Points(l_border,
//                                  roundabout_a_point_x, roundabout_a_point_y,
//                                  roundabout_p_point_x, roundabout_p_point_y);
//        }
//        // 🔧 补线策略2: 只有P点，从底部补到P点
//        else if (is_p_detected && !is_a_detected)
//        {
//            Connect_Border_Points(l_border,
//                                  l_border[image_h - 2], image_h - 2,
//                                  roundabout_p_point_x, roundabout_p_point_y);
//        }
//
//        // 🔧 状态转换: 检测到V点(上拐点)，进入环内阶段
//        if (is_v_detected)
//        {
//            roundabout_state = 2;
//            v_miss_cnt = 0;
//            flag1 = 0;
//        }
//    }
//    // ==================== 状态 2: 环内阶段，补右边界 V点 ====================
//    else if (roundabout_state == 2)
//    {
//        set_speed = 60;
//
//        if (is_v_detected)
//        {
//            v_miss_cnt = 0;
//
//            // 🔧 判断V点位置，选择补线策略
//            if (roundabout_v_point_y > 40)  // V点位置较低
//            {
////                Beep_Start();
//                // 从V点补到左上角和左侧
//                Connect_Border_Points(r_border,
//                                      roundabout_v_point_x, roundabout_v_point_y,
//                                      image_w - 1, P_line);
//                Connect_Border_Points(r_border,
//                                      roundabout_v_point_x, roundabout_v_point_y,
//                                      0, 30);
//                flag1 = 1;
//            }
//        }
//        else
//        {
//            // 🔧 V点消失处理
//            if (flag1)
//            {
//                if (++v_miss_cnt >= 20)  // 连续20帧未见V，进入出环阶段
//                {
//
//                    roundabout_state = 3;
//                    flag1 = 0;
//                    has_last_out_point = 0;
//                    last_out_x = last_out_y = 0;
//                    exit_cont_cnt = 0;
//                    out_recent_cnt = 0;
//                }
//            }
//        }
//    }
//    // ==================== 状态 3: 出环阶段 ====================
//    else if (roundabout_state == 3)
//    {
//        set_speed = 60;
//
//        // 🔧 记录"最近有效出环点"
//        if (is_out_detected)
//        {
//            last_out_x = roundabout_out_point_x;
//            last_out_y = roundabout_out_point_y;
//            has_last_out_point = 1;
//            out_recent_cnt = 10; // 维持10帧"最近有效"
//        }
//        if (out_recent_cnt) out_recent_cnt--;
//
//        // 🔧 持续补线右边界到左上角
//        if (has_last_out_point)
//        {
//            Connect_Border_Points(r_border,
//                                  last_out_x, last_out_y,
//                                  0, 0);  // 补到左上角
//        }
//
//        // 🔧 退出条件: 检测到V点且没有出环点
//        if (is_v_detected && !is_out_detected)
//        {
//            // 补左边界: V点到底部
////            Connect_Border_Points(l_border,
////                                  roundabout_v_point_x, roundabout_v_point_y,
////                                  l_border[image_h - 2], image_h - 2);
//
//            if (++v_found_cnt >= 5)  // 连续5帧检测到V点
//            {
//                // 退出环岛
//                roundabout_state = 0;
//                has_last_out_point = 0;
//                last_out_x = last_out_y = 0;
//                exit_cont_cnt = 0;
//                v_miss_cnt = 0;
//                v_found_cnt = 0;
////                set_speed = 100;
//            }
//        }
//        else
//        {
//            v_found_cnt = 0; // 未检测到V点，计数清零
//        }
//    }
//}

void Detect_Left_Roundabout(void)  //左圆环状态机
{
    if (cross_state != 0)
    {
        L_roundabout_state = 0;
        L_roundabout_a_point_x = 0;
        L_roundabout_a_point_y = 0;
        L_roundabout_p_point_x = 0;
        L_roundabout_p_point_y = 0;
        L_roundabout_v_point_x = 0;
        L_roundabout_v_point_y = 0;
        L_roundabout_out_point_x = 0;
        L_roundabout_out_point_y = 0;
        return;
    }
    uint8 is_a_detected   = Detect_Left_Roundabout_DownCorner();
    uint8 is_p_detected   = Detect_Left_Roundabout_ringpoint();
    uint8 is_v_detected   = Detect_Left_Roundabout_UpCorner();
    uint8 is_out_detected = Detect_Left_Roundabout_Outring();

    uint8 is_right_weak_continuous = Is_Border_Continue(r_border, image_h - 20, 30, 0, 0.50f);
    is_right_continuous = Is_Border_Continue(r_border, image_h - 30, 50, 0, 0.90f);

    uint8 has_right_corner = 0;

    static uint8  has_last_out_point = 0;
    static uint16 last_out_x = 0;
    static uint16 last_out_y = 0;

    static uint8 v_miss_cnt    = 0;  
    static uint8 v_found_cnt   = 0;  
    static uint8 out_recent_cnt = 0;

    static uint8 a_miss_cnt = 0;
    static uint16 last_a_point_x = 0;
    static uint16 last_a_point_y = 0;
    static uint8 extend_after_a_lost = 0;
    static uint8 has_valid_a_point = 0; 
    const uint8 MAX_EXTEND_FRAMES = 20;

    // 斜率补线持续帧数
//    static uint8 slope_fix_frames = 0;
//    const  uint8 MAX_SLOPE_FIX_FRAMES = 40; //补线帧数

    if (Detect_cross_r_v())
    {
        if (r_cross_v_point_y >= 60 && r_cross_v_point_y <= image_h - 20)
        {
            has_right_corner = 1;
        }
    }

    if (Detect_cross_r_a())
    {
        if (r_cross_a_point_y >= 60 && r_cross_a_point_y <= image_h - 20)
        {
            has_right_corner = 1;
        }
    }

    // ==================== 状态 0: 正常巡线，检测入口 ====================
    if (L_roundabout_state == 0)
    {
//        Speed_Goal = set_speed;
        if (is_a_detected && L_roundabout_a_point_y > 60 && L_roundabout_a_point_x < 94 && is_right_weak_continuous)
        {
            L_roundabout_state = 1;   // 进入摆正+入环阶段
//            slope_fix_frames   = 0;
            v_miss_cnt         = 0;
            has_last_out_point = 0;
            out_recent_cnt     = 0;
            // set_speed = 60;
            last_a_point_x = L_roundabout_a_point_x;
            last_a_point_y = L_roundabout_a_point_y;
            has_valid_a_point = 1;
        }
    }
    // ==================== 状态 1: 入环+摆正阶段，主要修正左边界 ====================
    else if (L_roundabout_state == 1)
    {
//        Speed_Goal = L_ring_speed_slow;
//         set_speed = 70;   // 如需更平稳可打开

        // 优先：利用 A 点前一段左线的斜率向上补线，把车头摆正
//        if (is_a_detected && slope_fix_frames < MAX_SLOPE_FIX_FRAMES)
//        {
//            Extend_Border_By_Slope(l_border,
//                    L_roundabout_a_point_x,
//                    L_roundabout_a_point_y,
//                                   15,   // 拟合长度：A 点下方 15 行
//                                   25);  // 向上补到第 25 行
//            slope_fix_frames++;
//        }
//        // 斜率补线结束后，fallback 到 A-P 直接连线逻辑
//        if (is_p_detected && is_a_detected)
//        {
//            Connect_Border_Points(l_border,
//                    L_roundabout_a_point_x, L_roundabout_a_point_y,
//                    L_roundabout_p_point_x, L_roundabout_p_point_y);
//        }
//        if (is_p_detected && !is_a_detected)
//        {
//            Connect_Border_Points(l_border,
//                                  l_border[image_h - 2], image_h - 2,
//                                  L_roundabout_p_point_x, L_roundabout_p_point_y);
//        }
//        if (!is_a_detected || slope_fix_frames >= MAX_SLOPE_FIX_FRAMES)
//        {
//            if (!is_right_continuous || has_right_corner)
//            {
//                L_roundabout_state = 0;
//                slope_fix_frames = 0;
//            }
//            else if (is_v_detected && is_right_continuous)
//            {
//                L_roundabout_state = 2;
//                v_miss_cnt       = 0;
//                flag1            = 0;
//    //             set_speed = 60;
//            }
//        }
        if (is_a_detected)
        {
            if (is_a_detected)
             {
                 last_a_point_x = L_roundabout_a_point_x;
                 last_a_point_y = L_roundabout_a_point_y;
                 a_miss_cnt = 0;
                 extend_after_a_lost = 0;
                 has_valid_a_point = 1;

                 Extend_Border_By_Slope(l_border,
                                        L_roundabout_a_point_x,
                                        L_roundabout_a_point_y,
                                        15, 25);
             }
             else
             {
                 a_miss_cnt++;

                 if (has_valid_a_point &&
                     extend_after_a_lost < MAX_EXTEND_FRAMES &&
                     last_a_point_y > 0)
                 {
                     Extend_Border_By_Slope(l_border,
                                            last_a_point_x,
                                            last_a_point_y,
                                            15, 25);
                     extend_after_a_lost++;
                 }
                 else if (is_p_detected)
                 {
                     Connect_Border_Points(l_border,
                                           l_border[image_h - 2], image_h - 2,
                                           L_roundabout_p_point_x, L_roundabout_p_point_y);
                 }
             }
        }

        if (!is_a_detected)
        {
            a_miss_cnt++;
        }

        uint8 should_check_transition = 0;

//        if (slope_fix_frames >= MAX_SLOPE_FIX_FRAMES)
//        {
//            should_check_transition = 1;
//        }
        if (a_miss_cnt >= 15)
        {
            should_check_transition = 1;
        }
        if (should_check_transition)
        {
            if (has_right_corner || !is_right_continuous)
            {
                L_roundabout_state = 0;
                a_miss_cnt = 0;
                extend_after_a_lost = 0;
                has_valid_a_point = 0;
                last_a_point_x = 0;
                last_a_point_y = 0;
            }
            
            else if (is_v_detected)
            {
                L_roundabout_state = 2;
                a_miss_cnt = 0;
                extend_after_a_lost = 0;
                has_valid_a_point = 0;
                last_a_point_x = 0;
                last_a_point_y = 0;
                v_miss_cnt = 0;
                flag1 = 0;
            }
        }


//        if (is_v_detected && is_right_continuous)
//        {
//            L_roundabout_state = 2;
//            v_miss_cnt       = 0;
//            flag1            = 0;
////             set_speed = 60;
//        }
    }
    // ==================== 状态 2: 环内阶段，主要操作右边界 V 点补线 ====================
    else if (L_roundabout_state == 2)
    {
//        Speed_Goal = L_ring_speed_slow;
//        set_speed = 60;

        if (is_v_detected)
        {
            v_miss_cnt = 0;

            if (L_roundabout_v_point_y > 40)  // V 点位置较低
            {
                Beep_Start();
                Connect_Border_Points(r_border,
                        L_roundabout_v_point_x, L_roundabout_v_point_y,
                                      image_w - 1, P_line);
                Connect_Border_Points(r_border,
                        L_roundabout_v_point_x, L_roundabout_v_point_y,
                                      0, 30);
                flag1 = 1;
            }
        }
        else
        {
            if (flag1)
            {
                if (++v_miss_cnt >= 20)  
                {
                    Beep_Stop();
                    L_roundabout_state = 3;
                    flag1              = 0;
                    has_last_out_point = 0;
                    last_out_x = last_out_y = 0;
                    out_recent_cnt     = 0;
                }
            }
        }
    }
    // ==================== 状态 3: 出环阶段 ====================
    else if (L_roundabout_state == 3)
    {
//        Speed_Goal = L_ring_speed_fast;
//        set_speed = 70;

        if (is_out_detected)
        {
            last_out_x         = L_roundabout_out_point_x;
            last_out_y         = L_roundabout_out_point_y;
            has_last_out_point = 1;
            out_recent_cnt     = 10; // 维持10帧“最近有效”
        }
        if (out_recent_cnt) out_recent_cnt--;
//        if (has_last_out_point)
//        {
//            Beep_Start();
//            Connect_Border_Points(r_border,
//                                  last_out_x, last_out_y,
//                                  0, 0);  // 补到左上角
//        }
        if (is_v_detected && L_roundabout_v_point_y > 40)
        {
            Connect_Border_Points(l_border,
                                  L_roundabout_v_point_x, L_roundabout_v_point_y,
                                  l_border[image_h - 2], image_h - 2);

        }
        else if (has_last_out_point && out_recent_cnt > 0)
        {
            Beep_Start();
            Connect_Border_Points(r_border,
                                  last_out_x, last_out_y,
                                  0, 0);
        }

        if (is_v_detected && !is_out_detected)
        {
            if (++v_found_cnt >= 1 && L_roundabout_v_point_y > 70)
            {
                Beep_Stop();
                // 退出环岛
                L_roundabout_state = 0;
                has_last_out_point = 0;
                last_out_x = last_out_y = 0;
                v_miss_cnt  = 0;
                v_found_cnt = 0;
                // set_speed = 80; 
            }
        }
        else
        {
            v_found_cnt = 0; 
        }
    }
}

uint8 Detect_cross_l_v(void)  //十字左上拐点检测
{
    static uint16 last_crossl_point_x = 0;
    static uint16 last_crossl_point_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_crossl_point = 0;

    const uint8 gap = 5;              // 取点间隔
    const float angle_threshold = 50; // 夹角阈值（度）

    for (uint16 i = 0; i + 4 * gap < data_stastics_l; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_l[idx1];
        uint8 d2 = (uint8) dir_l[idx2];
        uint8 d3 = (uint8) dir_l[idx3];
        uint8 d4 = (uint8) dir_l[idx4];
        uint8 d5 = (uint8) dir_l[idx5];

//        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
//                ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
//            ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
//            ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
//            ((d5 == 2) || (d5 == 3) || (d5 == 4)))
//        {
        if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
                ((d2 == 0) || (d2 == 6) || (d2 == 7)) &&
            ((d3 == 4) || (d3 == 5)) &&
            ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
            ((d5 == 4) || (d5 == 5) || (d5 == 6)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_l[idx1][0], points_l[idx1][1],
                points_l[idx2][0], points_l[idx2][1],
                points_l[idx4][0], points_l[idx4][1],
                points_l[idx5][0], points_l[idx5][1]
            );

            if (angle > angle_threshold && points_l[idx3][1] > 20 && points_l[idx3][1] < 60 && points_l[idx3][0] < 94)
            {
//                printf("%f\r\n",angle);
                // 检测成功，更新坐标和记忆
                l_cross_v_point_x = points_l[idx3][0];
                l_cross_v_point_y = points_l[idx3][1];
                last_crossl_point_x = l_cross_v_point_x;
                last_crossl_point_y = l_cross_v_point_y;
                has_last_crossl_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    // 当前帧未检测到，使用上一帧坐标（最多保持5帧）
    if (has_last_crossl_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            l_cross_v_point_x = last_crossl_point_x;
            l_cross_v_point_y = last_crossl_point_y;
            return 1;
        }
    }

    // 连续5帧未检测到，清空所有记录
    has_last_crossl_point = 0;
    miss_frame_count = 0;
    last_crossl_point_x = 0;
    last_crossl_point_y = 0;
    l_cross_v_point_x = 0;
    l_cross_v_point_y = 0;
    return 0;
}

uint8 Detect_cross_l_a(void)    //十字左下拐点检测
{
    static uint16 last_crossl_a_x = 0;
    static uint16 last_crossl_a_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 120;

    for (uint16 i = 0; i + 4 * gap < data_stastics_l / 2; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_l[idx1];
        uint8 d2 = (uint8) dir_l[idx2];
        uint8 d3 = (uint8) dir_l[idx3];
        uint8 d4 = (uint8) dir_l[idx4];
        uint8 d5 = (uint8) dir_l[idx5];

//        if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
//                ((d2 == 0) || (d2 == 6) || (d2 == 7)) &&
//            ((d3 == 4) || (d3 == 5) || (d3 == 6)) &&
//            ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
//            ((d5 == 4) || (d5 == 5) || (d5 == 6)))
//        {
          if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
              ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
              ((d3 == 1) || (d3 == 2) || (d3 == 3)) &&
              ((d4 == 1) || (d4 == 2) || (d4 == 3)) &&
              ((d5 == 1) || (d5 == 2) || (d5 == 3)))
          {
            float angle = Compute_Angle_Between_Lines(
                points_l[idx1][0], points_l[idx1][1],
                points_l[idx2][0], points_l[idx2][1],
                points_l[idx4][0], points_l[idx4][1],
                points_l[idx5][0], points_l[idx5][1]
            );

            if (angle > angle_threshold && points_l[idx3][1] > 60 && points_l[idx3][0] < 94)
            {
//                printf("%f\r\n",angle);
                l_cross_a_point_x = points_l[idx3][0];
                l_cross_a_point_y = points_l[idx3][1];
                last_crossl_a_x = l_cross_a_point_x;
                last_crossl_a_y = l_cross_a_point_y;
                has_last_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            l_cross_a_point_x = last_crossl_a_x;
            l_cross_a_point_y = last_crossl_a_y;
            return 1;
        }
    }

    has_last_point = 0;
    miss_frame_count = 0;
    last_crossl_a_x = 0;
    last_crossl_a_y = 0;
    l_cross_a_point_x = 0;
    l_cross_a_point_y = 0;
    return 0;
}

uint16 r_cross_v_point_x = 0, r_cross_v_point_y = 0;

uint8 Detect_cross_r_v(void)  //十字右上拐点检测
{
    static uint16 last_crossr_v_x = 0;
    static uint16 last_crossr_v_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 45;

    for (uint16 i = 0; i + 4 * gap < data_stastics_r; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_r[idx1];
        uint8 d2 = (uint8) dir_r[idx2];
        uint8 d3 = (uint8) dir_r[idx3];
        uint8 d4 = (uint8) dir_r[idx4];
        uint8 d5 = (uint8) dir_r[idx5];

//        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
//                ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
//            ((d3 == 2) || (d3 == 3) || (d3 == 4)) &&
//            ((d4 == 2) || (d4 == 3) || (d4 == 4)) &&
//            ((d5 == 2) || (d5 == 3) || (d5 == 4)))
//        {
        if (((d1 == 0) || (d1 == 6) || (d1 == 7)) &&
                 ((d1 == 0) || (d2 == 6) || (d2 == 7)) &&
                 ((d3 == 4) || (d3 == 5)) &&
                 ((d4 == 4) || (d4 == 5) || (d4 == 6)) &&
                 ((d5 == 4) || (d5 == 5) || (d5 == 6)))
         {
            float angle = Compute_Angle_Between_Lines(
                points_r[idx1][0], points_r[idx1][1],
                points_r[idx2][0], points_r[idx2][1],
                points_r[idx4][0], points_r[idx4][1],
                points_r[idx5][0], points_r[idx5][1]
            );

            if (angle > angle_threshold && points_r[idx3][1] > 20 && points_r[idx3][0] > 94)
            {
//                printf("%f\r\n",angle);
                r_cross_v_point_x = points_r[idx3][0];
                r_cross_v_point_y = points_r[idx3][1];
                last_crossr_v_x = r_cross_v_point_x;
                last_crossr_v_y = r_cross_v_point_y;
                has_last_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            r_cross_v_point_x = last_crossr_v_x;
            r_cross_v_point_y = last_crossr_v_y;
            return 1;
        }
    }

    has_last_point = 0;
    miss_frame_count = 0;
    last_crossr_v_x = 0;
    last_crossr_v_y = 0;
    r_cross_v_point_x = 0;
    r_cross_v_point_y = 0;
    return 0;
}

uint16 r_cross_a_point_x = 0, r_cross_a_point_y = 0;

uint8 Detect_cross_r_a(void)  //十字右下拐点检测
{
    static uint16 last_crossr_a_x = 0;
    static uint16 last_crossr_a_y = 0;
    static uint8 miss_frame_count = 0;
    static uint8 has_last_point = 0;

    const uint8 gap = 5;
    const float angle_threshold = 110;

    for (uint16 i = 0; i + 4 * gap < data_stastics_r; i++)
    {
        uint16 idx1 = i;
        uint16 idx2 = i + gap;
        uint16 idx3 = i + 2 * gap;
        uint16 idx4 = i + 3 * gap;
        uint16 idx5 = i + 4 * gap;

        uint8 d1 = (uint8) dir_r[idx1];
        uint8 d2 = (uint8) dir_r[idx2];
        uint8 d3 = (uint8) dir_r[idx3];
        uint8 d4 = (uint8) dir_r[idx4];
        uint8 d5 = (uint8) dir_r[idx5];

        if (((d1 == 4) || (d1 == 5) || (d1 == 6)) &&
            ((d2 == 4) || (d2 == 5) || (d2 == 6)) &&
            ((d3 == 1) || (d3 == 2) || (d3 == 3)) &&
            ((d4 == 1) || (d4 == 2) || (d4 == 3)) &&
            ((d5 == 1) || (d5 == 2) || (d5 == 3)))
        {
            float angle = Compute_Angle_Between_Lines(
                points_r[idx1][0], points_r[idx1][1],
                points_r[idx2][0], points_r[idx2][1],
                points_r[idx4][0], points_r[idx4][1],
                points_r[idx5][0], points_r[idx5][1]
            );

            if (angle > angle_threshold && points_r[idx3][1] > 40 && points_r[idx3][0] > 94)
            {
//                printf("%f\r\n",angle);
                r_cross_a_point_x = points_r[idx3][0];
                r_cross_a_point_y = points_r[idx3][1];
                last_crossr_a_x = r_cross_a_point_x;
                last_crossr_a_y = r_cross_a_point_y;
                has_last_point = 1;
                miss_frame_count = 0;
                return 1;
            }
        }
    }

    if (has_last_point && miss_frame_count < 5)
    {
        miss_frame_count++;
        if (miss_frame_count < 5)
        {
            r_cross_a_point_x = last_crossr_a_x;
            r_cross_a_point_y = last_crossr_a_y;
            return 1;
        }
    }

    has_last_point = 0;
    miss_frame_count = 0;
    last_crossr_a_x = 0;
    last_crossr_a_y = 0;
    r_cross_a_point_x = 0;
    r_cross_a_point_y = 0;
    return 0;
}

float lost_ratio;
uint8 Is_Right_Border_Lost(uint8* r_border, uint8 start_row, uint8 end_row, uint8 lost_threshold)   //检测右边界丢线
{
    if (start_row <= end_row || end_row < 10)
        return 0;

    uint8 lost_count = 0;
    uint8 total_count = 0;

    for (int row = start_row; row > end_row; row -= 3) // 每隔3行检测
    {
        total_count++;
        if (r_border[row] == border_max)
        {
            lost_count++;
        }
    }

    lost_ratio = (float)lost_count / (float)total_count;

    return (lost_ratio > (lost_threshold / 100.0f)) ? 1 : 0;
}

uint8 lost_threshold = 60;
uint16 cross_state = 0;
void Tilted_Cross_process(void)
{
    static uint8 cnt = 0;
    static uint8 exit_cnt = 0;           // 退出计数
    static uint8 inside_frames = 0;      // 在十字内的帧数

    uint8 is_leftdown_detected = Detect_cross_l_a();
    uint8 is_leftup_detected = Detect_cross_l_v();
    uint8 is_rightdown_detected = Detect_cross_r_a();
    uint8 is_rightup_detected = Detect_cross_r_v();
//    uint8 is_right_border_lost = Is_Right_Border_Lost(r_border, image_h - 15, 20, lost_threshold);

    if (cross_state == 0)
    {
         if ((is_leftdown_detected && is_rightdown_detected) || (is_leftdown_detected && is_leftup_detected))
//                 || (is_leftup_detected && is_rightup_detected))
         {
             cnt++;
             if (cnt >= 3)
             {
                 cross_state = 1;
             }
         }
         else
         {
             cnt = 0;
         }
    }
    else if (cross_state == 1)
    {
        inside_frames++;
        if (is_leftup_detected && is_leftdown_detected)
        {
            Connect_Border_Points(l_border, l_cross_v_point_x, l_cross_v_point_y, l_cross_a_point_x, l_cross_a_point_y);
        }
        else if (!is_leftup_detected && is_leftdown_detected)
        {
            Extend_Border_By_Slope(l_border, l_cross_a_point_x, l_cross_a_point_y, 15, 25);
        }
        else if (is_leftup_detected && !is_leftdown_detected)
        {
            Connect_Border_Points(l_border, l_cross_v_point_x, l_cross_v_point_y, l_border[image_h - 2], image_h - 2);
        }

//        if (is_rightup_detected)
//        {
//            if(!is_rightdown_detected)
//            {
//                Connect_Border_Points(r_border, r_cross_v_point_x, r_cross_v_point_y, r_border[image_h - 2], image_h - 2);
//            }
//            else
//            {
//                Connect_Border_Points(r_border, r_cross_v_point_x, r_cross_v_point_y, r_cross_a_point_x, r_cross_a_point_y);
//            }
//
//        }

        if (is_rightdown_detected && is_rightup_detected)
        {
            Connect_Border_Points(r_border, r_cross_v_point_x, r_cross_v_point_y, r_cross_a_point_x, r_cross_a_point_y);
        }
        else if (is_rightdown_detected && !is_rightup_detected)
        {
            Extend_Border_By_Slope(r_border, r_cross_a_point_x, r_cross_a_point_y, 15, 25);
        }
        else if (!is_rightdown_detected && is_rightup_detected)
        {
            Connect_Border_Points(r_border, r_cross_v_point_x, r_cross_v_point_y, r_border[image_h - 2], image_h - 2);
        }
        if (inside_frames > 20)
        {
            if (!is_leftup_detected && !is_rightup_detected)
            {
                exit_cnt++;
                if (exit_cnt >= 5)  // 连续5帧确认退出
                {
                    L_roundabout_state = 0;
                    R_roundabout_state = 0;
                    cross_state = 0;
                    exit_cnt = 0;
                    inside_frames = 0;
                    cnt = 0;
                }
            }
            else
            {
                exit_cnt = 0;  // 重置退出计数
            }
        }

        if (inside_frames > 80)  // 在十字内超过80帧强制退出
        {
            cross_state = 0;
            exit_cnt = 0;
            inside_frames = 0;
            cnt = 0;
        }

    }

}

float variance = 0;
uint8 Straight_Judgement(uint8 start_row, uint8 end_row, float variance_threshold)
{
    if (start_row <= end_row || start_row >= image_h || end_row < 1)
        return 0;

    const uint8 SAMPLE_STEP = 3;      // 采样间隔
    const uint8 MIN_SAMPLES = 10;     // 最少采样点数

    float samples[64];
    uint8 sample_count = 0;

    for (int row = start_row; row > end_row && sample_count < 64; row -= SAMPLE_STEP)
    {
        uint8 center_val = center_line[row];

        if (center_val > border_min && center_val < border_max)
        {
            samples[sample_count] = (float)center_val;
            sample_count++;
        }
    }

    if (sample_count < MIN_SAMPLES)
    {
        return 0;  // 样本太少，无法判断
    }

    float sum = 0.0f;
    for (uint8 i = 0; i < sample_count; i++)
    {
        sum += samples[i];
    }
    float mean = sum / (float)sample_count;

    float variance_sum = 0.0f;
    for (uint8 i = 0; i < sample_count; i++)
    {
        float diff = samples[i] - mean;
        variance_sum += diff * diff;
    }
    variance = variance_sum / (float)sample_count;

    return (variance < variance_threshold) ? 1 : 0;
}

#define ZEBRA_SCAN_START_ROW 100    // 扫描起始行（从下往上）
#define ZEBRA_SCAN_END_ROW 60       // 扫描结束行
#define ZEBRA_MIN_JUMPS 12          // 单行最少跳变次数（9条斑马线 → 至少12次跳变）
#define ZEBRA_MAX_JUMPS 25          // 单行最多跳变次数（防止噪声）
#define ZEBRA_MIN_FEATURE_ROWS 6    // 最少特征行数（至少6行满足条件）

uint8 zebra_detected_frames = 0;    // 连续检测到斑马线的帧数
const uint8 ZEBRA_STABLE_FRAMES = 3;  // 需要连续检测到的帧数

uint8 Detect_zebraline(void)
{
    uint8 feature_row_count = 0;  // 具有斑马线特征的行数

    for (int row = ZEBRA_SCAN_START_ROW; row >= ZEBRA_SCAN_END_ROW; row--)
    {
        uint8 jump_count = 0;  // 黑白跳变次数
        uint8 last_pixel = binary_image[row][border_min];

        for (int col = border_min + 1; col < border_max; col++)
        {
            uint8 current_pixel = binary_image[row][col];

            if (current_pixel != last_pixel)
            {
                jump_count++;
                last_pixel = current_pixel;
            }
        }

        if (jump_count >= ZEBRA_MIN_JUMPS && jump_count <= ZEBRA_MAX_JUMPS)
        {
            feature_row_count++;
        }
    }
    return (feature_row_count >= ZEBRA_MIN_FEATURE_ROWS) ? 1 : 0;
}

static uint8 zebra_stop_state = 0;           // 斑马线停车状态机
static uint8 zebra_lost_frames = 0;          // 斑马线消失后的帧数
static uint8 zebra_detect_count = 0;         // 检测到斑马线的次数

const uint8 ZEBRA_LOST_STOP_FRAMES = 3;     // 第二次斑马线消失后停车帧数
const uint16 MIN_INTERVAL_FRAMES = 200;      // 两次斑马线之间最少间隔帧数

void Zebraline_Stop_Control(void)
{
    static uint16 interval_cnt = 0;  // 离开第一次斑马线的帧数
    uint8 current_detected = Detect_zebraline();  // 当前帧是否检测到斑马线

    // ==================== 状态 0: 等待第一次斑马线 ====================
    if (zebra_stop_state == 0)
    {
        if (current_detected)
        {
            zebra_detect_count = 1;   
            zebra_stop_state = 1;     
            zebra_detected_frames = 0;
            zebra_lost_frames = 0;
            interval_cnt = 0;
        }
    }
    // ==================== 状态 1: 第一次斑马线已过，等待第二次 ====================
    else if (zebra_stop_state == 1)
    {
        interval_cnt++;  // 累计离开第一次斑马线的帧数

        if (current_detected)
        {
            if (interval_cnt >= MIN_INTERVAL_FRAMES)
            {
                zebra_detect_count = 2; 
                zebra_stop_state = 2;     
                zebra_detected_frames = 0;
                zebra_lost_frames = 0;

//                Beep_Start();  // 开始蜂鸣提示
            }
            else
            {
                zebra_detected_frames = 0;
            }
        }
    }
    // ==================== 状态 2: 第二次斑马线检测中 ====================
    else if (zebra_stop_state == 2)
    {
        if (current_detected)
        {
            zebra_lost_frames = 0;  
        }
        else
        {
            zebra_lost_frames++;

            if (zebra_lost_frames >= ZEBRA_LOST_STOP_FRAMES)
            {
                zebra_stop_state = 3;
            }
        }
    }
    // ==================== 状态 3: 停车 ====================
    else if (zebra_stop_state == 3)
    {
//        Beep_Stop();
        Car_Stop(); 

    }
}

#pragma section all restore
