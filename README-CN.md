# Graffiti Engine

[![badge](https://img.shields.io/badge/GitHub-181717?style=plastic&logo=github&logoColor=white)](https://github.com/htl309/Graffiti )[![Gitee](https://img.shields.io/badge/Gitee-FF3B3F?style=plastic&logo=gitee&logoColor=white)](https://gitee.com/htl309/Graffiti)

![Graffiti](/resource/logo/LOGO_INVERT.png "Graffiti")

### API与平台

- [x] 支持Opengl API
- [x] 支持Vulkan API
- [x] 支持Windows系统
- [ ] 支持Android系统

### 图形算法 

- [x] 三角形							（2025.6.4）
- [x] 网格模式                        （2025.6.10）
- [x] 相机                                （2025.6.10）
- [x] Bling-phong光照          （2025.6.13）
- [x] 几何着色器                    （2025.6.18）
- [x] 纹理                                （2025.7.1）
- [ ] 材质与PBR渲染
- [ ] 阴影
- [ ] 延迟渲染与多光源
- [ ] 网格着色器
- [ ] PCB混合管线

### 环境

- GLAD   4.6  (OpenGL API)
- Vulkan 1.3  (Vulkan API)
- GLFW          (Windows窗口)
- glm           （数学、数学、数学)
- ImGui         (交互)
- spdlog        (日志系统)
- stb              (纹理加载)
- tinygltf       (模型加载)

### 构建

1. 在Window系统上使用Cmake工具进行构建。项目中应该已经包含了一切，直接构建就可以运行项目。
2. 需要注意的是，必须构建Cmake在根目录下新建的Build文件，因为我Cmake的一些路径直接写了build。
3. 当然Graffiti引擎本体并不需要设置第二部的build，直接编译就可以得到Graffiti.lib，可以被其他程序使用。

### 说明

- 有一些算法用到的架构安卓不支持，等之后在说。


- 代码行数统计：在Windows中项目根目录终端，运行如下命令，可以看到代码行数，现在应该有7000行左右吧

  ```powershell
  Get-ChildItem "Graffiti" -Recurse -File -Include *.cpp,*.h,*.hpp | Get-Content | Measure-Object -Line
  ```


### 预览

![image-20250702111725925](/resource/pic/Guide00-1.png)

![image-20250702111725925](/resource/pic/Readme-1.png)

![image-20250702112341003](/resource/pic/Readme-2.png)

![image-20250702111751034](/resource/pic/Readme-3.png)
