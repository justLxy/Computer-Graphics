# The Whispering Mills

## Personal Information
- Name: Xuanyi Lyu
- UtorID: lyuxuany
- Student Number: 1009343266
- Based on assignments: A1 (Raster Images), A2 (Ray Casting), A3 (Ray Tracing), A4 (Bounding Volume Hierarchy), A6 (Shader Pipeline)

## Overview

This project is an 8-second ray-traced animation of a pastoral scene featuring two papercraft windmills. I started with a static 3D model and built a complete rendering pipeline around it. The final video shows spinning windmill blades, drifting clouds, realistic lighting with shadows, and a slow camera dolly. The animation runs at 1920×1080 resolution and 60 fps, with background music.

## How to Build and Run

**Build** (macOS, requires CMake ≥ 3.20):
```bash
cd "Showcase/Code"
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
```

**Run** the renderer:
```bash
./showcase
```
This writes 480 frames to `Showcase/Piece/frames/frame_XXXX.ppm`.

**Encode** the video (requires ffmpeg; install with `brew install ffmpeg` if needed):
```bash
ffmpeg -y -framerate 60 -i ../../Piece/frames/frame_%04d.ppm -pix_fmt yuv420p ../../Piece/piece.mp4
```

**Add background music** (optional):
```bash
ffmpeg -y -framerate 60 -i ../../Piece/frames/frame_%04d.ppm \
  -i ../source/Five_Hundred_Miles.mp3 \
  -c:v libx264 -crf 18 -pix_fmt yuv420p -c:a aac -b:a 192k -shortest \
  ../../Piece/piece.mp4
```

The first 2.5 seconds of the video show a title card with the work title, my name, and "CSC317 Fall 2025".

## What I Built

The core challenge was taking a static mesh and bringing it to life. I implemented several major features to make this happen:

### 1. Mesh Loading and Rendering
I wrote a custom OBJ/MTL loader (`src/obj_loader.{h,cpp}`) that handles vertex positions, normals, texture coordinates, materials, and texture images. The loader supports polygon triangulation and resolves texture paths relative to the model directory. For rendering, I built a ray tracer that samples textures (nearest-neighbor interpolation) and computes Blinn-Phong shading with directional lighting.

### 2. BVH Acceleration
To make ray tracing practical for a high-poly model (11197 triangles), I implemented a bounding volume hierarchy (`src/bvh.{h,cpp}`). The BVH uses median splits along the longest axis and iterative traversal. This brought render times down from minutes per frame to a few seconds.

### 3. Windmill Animation
The windmill blades needed to rotate realistically without drifting or clipping through the structure. I grouped triangles by their texture names (e.g., "wing") and computed a separate pivot point and rotation axis for each fan. The pivot is the centroid of the fan's vertices, and the axis is estimated using PCA (the eigenvector with smallest variance). Each frame, I apply a Rodrigues rotation to the blade vertices and normals, then rebuild the BVH so shadows and intersections stay correct. This logic is in `src/main.cpp`.

### 4. Animated Sky
The sky is rendered as an emissive background plane. To make the clouds drift, I scroll the texture UV coordinates over time (`emissive_uv_offset` in `src/scene.{h,cpp}`). I also added separate exposure and gain controls for emissive materials to make the sky bright without overexposing the ground.

### 5. Lighting and Shadows
I implemented a directional sun with hard shadows using shadow rays through the BVH. To avoid overly dark shadows, I added a small ambient term and an optional "sky fill" that adds soft blue light to upward-facing surfaces. The final image goes through a filmic tone mapper (Hable/Uncharted 2 curve) to compress the dynamic range. All of this is in `src/scene.cpp`.

### 6. Camera and Output
The camera performs a slow dolly-in over the 8-second animation. I render at 1920×1080 and 60 fps, writing each frame as a PPM file. A simple bitmap font renderer (`src/image.{h,cpp}`) draws the title card overlay for the first 2.5 seconds. At the end of the run, the program prints the ffmpeg command to encode the frames into an MP4.

To help find the right camera angles, I also built an interactive preview tool (`preview.cpp`) that uses GLFW to open a window where I can adjust all camera parameters with keyboard controls and see the result in real time.

### 7. Performance
Rendering is multithreaded (one thread per row chunk). On my machine, each frame takes a few seconds. The BVH is rebuilt every frame to account for the moving windmill blades, but the cost is acceptable thanks to the simple median-split construction.

## What I Contributed

The 3D model and textures are from an external source (see Acknowledgements), but all the code and animation logic are my own work:
- Windmill blade rotation with automatic pivot/axis detection
- Cloud drift via UV animation
- Lighting system (sun direction lock, sky fill, shadows, tone mapping)
- Camera path and title overlay
- Interactive preview tool for camera tuning
- OBJ/MTL/texture loader with robust path handling
- BVH construction and per-frame rebuild
- Multithreaded rendering loop

## Optional Controls

If you want to tweak the animation, you can set environment variables before running `./showcase`:
- **Windmills**: `WIND_RPM` (rotation speed), `WIND_ENABLE` (toggle on/off)
- **Sun**: `SUN_LOCK_AT_START=1` (lock sun to initial camera view), `SUN_INTENSITY` (brightness)
- **Sky**: `SKY_FILL` (ambient fill from above), `CLOUD_SPD_U` (cloud drift speed)
- **Camera**: `CAM_HEIGHT_ABS`, `CAM_FOV`, `FPS`

See `src/main.cpp` for the full list.

## Acknowledgements

- **3D Model**: Papercraft Windmills (OBJ/MTL/textures) by Ruslan Sokolovsky. Free/open asset. Author's X profile: [https://x.com/Ruslans3d](https://x.com/Ruslans3d). The model is completely static; all animations are implemented by me.
- **Music**: "Five Hundred Miles" performed by Carey Mulligan, Justin Timberlake, and Stark Sands.
- **Libraries**: 
  - Image loading uses macOS system frameworks (ImageIO/CoreGraphics) via a custom shim in `src/stb_image.h`.
  - GLFW (optional, only for the preview tool; not required for the main renderer).
  - Linear algebra (3×3 eigendecomposition) is implemented from scratch in `src/main.cpp`.
- **Course Material**: I used concepts from the course assignments on Raster Images, Ray Casting, Ray Tracing, Bounding Volume Hierarchy, and Shader Pipeline.
