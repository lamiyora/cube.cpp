# ASCII 3D 旋轉立方體

這是一個使用 **C++ 在終端機中繪製並旋轉 3D 立方體**的範例程式。
透過簡單的數學運算（3D 旋轉 + 透視投影），將立方體即時顯示為 ASCII 圖形。

## 執行效果

在終端機中會看到一個由 `#` 組成、不斷旋轉的立方體。


## 程式架構

### 1️⃣ 資料結構

```cpp
struct point3Dpos { float x, y, z; };
struct point2Dpos { int x, y; };
```

* `point3Dpos`：表示 3D 空間中的點
* `point2Dpos`：表示投影到螢幕上的 2D 點



### 2️⃣ 3D 旋轉（核心數學）
這個過程背後的數學基礎是**旋轉矩陣**的概念。
<img width="365" height="412" alt="image" src="https://github.com/user-attachments/assets/e3788081-91d0-4a80-8850-6c46dcde2b43" />


程式提供三個旋轉函式：

* `rotateX()`：繞 X 軸旋轉
  ```
  y' = y·cosθ − z·sinθ
  z' = y·sinθ + z·cosθ
  ```
  
* `rotateY()`：繞 Y 軸旋轉
  ```
  x' = x·cosθ + z·sinθ
  z' = −x·sinθ + z·cosθ
  ```
  
* `rotateZ()`：繞 Z 軸旋轉
  ```
  x' = x·cosθ − y·sinθ
  y' = x·sinθ + y·cosθ
  ```

每一幀都會對立方體的 8 個頂點進行旋轉，產生動畫效果。



### 3️⃣ 3D → 2D 透視投影

```cpp
point2D.x = (x * FOCAL_LENGTH) / (z + DISTANCE_FROM_CAM);
point2D.y = (y * FOCAL_LENGTH) / (z + DISTANCE_FROM_CAM);
```

這一步將 3D 點「壓平」成 2D，模擬遠近效果：

* 物體越遠（z 越大），顯示越小
* `FOCAL_LENGTH` 控制視角
* `DISTANCE_FROM_CAM` 避免除以 0



### 4️⃣ 畫布與繪圖

* 使用 `char canva[HEIGHT][WIDTH]` 當作畫布
* `drawPoint()`：在畫布上畫點
* `drawLine()`：使用類似 Bresenham 演算法畫線
* 將立方體的邊一條條連起來



### 5️⃣ 主迴圈（動畫關鍵）

```cpp
while (true) {
    旋轉
    投影
    清空畫面
    繪製
    輸出
}
```

每一圈：

1. 增加旋轉角度
2. 重新計算座標
3. 清除畫面並重畫
4. `Sleep(100)` 控制刷新速度

## 可修改參數

* `ROTATE_ANGLE`：旋轉速度
* `WIDTH / HEIGHT`：畫面大小
* `PAINT`：繪製立方體的字元（如 `*`, `@`）
* 移除某個旋轉軸，觀察效果差異

# TODO
1. Z-buffer（深度緩衝）
2. 互動式調整旋轉與控制
3. 面（Polygon）渲染
4. 點/線/面繪製
