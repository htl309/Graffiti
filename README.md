# Graffiti Engine

[![badge](https://img.shields.io/badge/GitHub-181717?style=plastic&logo=github&logoColor=white)](https://github.com/htl309/Graffiti ) [![Gitee](https://img.shields.io/badge/Gitee-FF3B3F?style=plastic&logo=gitee&logoColor=white)](https://gitee.com/htl309/Graffiti)

![Graffiti](resource/logo/Graffiti_LOGO.png "Graffiti")

### API and Platforms

- [x] OpenGL API
- [x] Vulkan API
- [x] Windows OS
- [ ] Android OS

### Graphics Algorithms

- [x] Triangle						
- [x] WireframeMode       
- [x] Camera                   
- [x] Blinn-Phong Lighting    
- [x] Geometry Shader  
- [x] Texture            
- [x] Materials and PBR
- [ ] Shadows
- [ ] Deferred Rendering
- [x] Mesh Shader
- [x] [PCB Hybrid Pipeline](https://github.com/htl309/pcbbuilder)

### Environment

- GLAD   4.6  (OpenGL API)
- Vulkan 1.3  (Vulkan API)
- GLFW          (Windows)
- glm              (Math、Math、Math)
- ImGui         (UI interaction)
- spdlog        (Logging System)
- stb              (Texture Loading)
- tinygltf       (Model Loading)
- meshoptimizer       (Mesh Grouping)

### Build

1. Use CMake on Windows to build the project. Everything needed should already be included, so you can build and run the project directly.
2. Note that you must build the CMake `build` folder created in the root directory, because some paths in my CMake files are hardcoded to `build`.
3. Of course, the Graffiti engine core itself does not require the second step’s build folder setup — you can compile it directly to get `Graffiti.lib`, which can be used by other programs.

### Notes

- Some algorithms use architectures not supported on Android; we will address that later.

- Code line count: In the project root directory terminal on Windows, run the following command to see the code line count. Currently, it should be around 7,000 lines.

  ```powershell
  Get-ChildItem "Graffiti" -Recurse -File -Include *.cpp,*.h,*.hpp | Get-Content | Measure-Object -Line
  ```

### Preview

#### 00_Hello_Triangle

![image-20250702111725925](resource/pic/Guide00-1.png)

#### 01_Bunny

![image-20250702111725925](resource/pic/Readme-1.png)

#### 02_GeometryShader

![image-20250702112341003](resource/pic/Readme-2.png)

#### 03_Texture

![image-20250702111751034](resource/pic/Readme-3.png)

#### 04_PBR

![image-20250714152102126](resource/pic/Readme-4.png)

#### 05_MeshShader

![image-20250722232904935](resource/pic/Readme-5.png)

### [PCB Hybrid PipeLine](https://github.com/htl309/pcbbuilder)

![t4](resource/pic/t4.png)
