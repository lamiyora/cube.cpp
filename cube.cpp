#include <stdio.h>
#include <math.h>
#include <windows.h>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 50;
const float ROTATE_ANGLE = 0.05;
const int FOCAL_LENGTH = 30;
const int DISTANCE_FROM_CAM = 5;
const char PAINT = '#';

struct point3Dpos
{
    float x;
    float y;
    float z;
};

struct point2Dpos
{
    int x;
    int y;
};

//X 軸旋轉
point3Dpos rotateX(point3Dpos point, float angle) 
{
    float x = point.x, y = point.y, z = point.z; //保存x,y,z舊值
    point.y = y * cos(angle) - z * sin(angle);
    point.z = y * sin(angle) + z * cos(angle);
    return {point.x, point.y, point.z};
}

//Y 軸旋轉
point3Dpos rotateY(point3Dpos point, float angle) 
{
    float x = point.x, y = point.y, z = point.z; //保存x,y,z舊值
    point.x = x * cos(angle) + z * sin(angle);
    point.z = -x * sin(angle) + z * cos(angle);
    return {point.x, point.y, point.z};
}

point3Dpos rotateZ(point3Dpos point, float angle) 
{
    float x = point.x, y = point.y, z = point.z; //保存x,y,z舊值
    point.x = x * cos(angle) - y * sin(angle);
    point.y = x * sin(angle) + y * cos(angle);
    return {point.x, point.y, point.z};
}

point2Dpos flatten(point3Dpos point3D)
{
    point2Dpos point2D;
    point2D.x = (point3D.x * FOCAL_LENGTH) / (point3D.z + DISTANCE_FROM_CAM);
    point2D.y = (point3D.y * FOCAL_LENGTH) / (point3D.z + DISTANCE_FROM_CAM);

    // 邊界檢查
    point2D.x += WIDTH / 2;
    point2D.y += HEIGHT / 2;
    return point2D;
}

void initCanva(char canva[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            canva[y][x] = ' ';
}

void drawPoint(point2Dpos point, char canva[HEIGHT][WIDTH])
{
    if (point.x >= 0 && point.x < WIDTH && point.y >= 0 && point.y < HEIGHT)
        canva[point.y][point.x] = PAINT;
}

void drawLine(point2Dpos p1, point2Dpos p2, char canva[HEIGHT][WIDTH])
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int error = 0; //誤差值

    int sx = (p1.x < p2.x ? 1 : -1);
    int sy = (p1.y < p2.y ? 1 : -1);

    if (dx > dy)
    {
        while (p1.x != p2.x)
        {
            error += dy;
            if (error >= dx)
            {
                p1.y += sy;
                error -= dx;
            }
            drawPoint(p1, canva);
            p1.x += sx;
        }
    }
    else
    {
        while (p1.y != p2.y)
        {
            error += dx;
            if (error >= dy)
            {
                p1.x += sx;
                error -= dy;
            }
            drawPoint(p1, canva);
            p1.y += sy;
        }
    }
}

int main() 
{
    point3Dpos pointArr[8] = {
        {1,1,1}, {1,1,-1}, {1,-1,1},
        {1,-1,-1}, {-1,1,1}, {-1,1,-1},
        {-1,-1,1}, {-1,-1,-1}
    };
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;

    //createCanva
    char canva[HEIGHT][WIDTH];

    while(true)
    {
        // 把 旋轉後的點 存到新的陣列
        point3Dpos rotatedPoint[8] = {};
        for (int i = 0; i < 8; i++)
        {
            rotatedPoint[i] = pointArr[i];
            rotatedPoint[i] = rotateX(rotatedPoint[i], angleX);
            rotatedPoint[i] = rotateY(rotatedPoint[i], angleY);
            rotatedPoint[i] = rotateZ(rotatedPoint[i], angleZ);
        }
        
        // 投影到 2D
        point2Dpos projectedPoint[8] = {};
        for (int i = 0; i < 8; i++)
        {
            projectedPoint[i] = flatten(rotatedPoint[i]);
        }
        
        // Draw
        printf("\x1b[H"); // 回到左上角 // 清除
        initCanva(canva); 
        drawLine(projectedPoint[0], projectedPoint[1],canva);
        drawLine(projectedPoint[0], projectedPoint[2],canva);
        drawLine(projectedPoint[0], projectedPoint[4],canva);
        drawLine(projectedPoint[3], projectedPoint[1],canva);
        drawLine(projectedPoint[3], projectedPoint[2],canva);
        drawLine(projectedPoint[3], projectedPoint[7],canva);
        drawLine(projectedPoint[5], projectedPoint[1],canva);
        drawLine(projectedPoint[5], projectedPoint[4],canva);
        drawLine(projectedPoint[5], projectedPoint[7],canva);
        drawLine(projectedPoint[6], projectedPoint[2],canva);
        drawLine(projectedPoint[6], projectedPoint[4],canva);
        drawLine(projectedPoint[6], projectedPoint[7],canva);

        for (int row = 0; row < HEIGHT; row++)
        {
            for (int col = 0; col < WIDTH; col++)
            {
                printf("%c", canva[row][col]);
            }
            printf("\n");
        }

        angleX += ROTATE_ANGLE;
        angleY += ROTATE_ANGLE;
        angleZ += ROTATE_ANGLE;
        Sleep(100); // 休眠避免刷新過快
    }
}
