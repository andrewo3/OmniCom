@echo off
"%VULKAN_SDK%\Bin\glslc.exe" .\res\shaders\main.vert -o .\res\shaders\main.vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" .\res\shaders\main.frag -o .\res\shaders\main.frag.spv