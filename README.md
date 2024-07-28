# RayTracingInAWeekend
This is my implementation of a raytracer based on the book by Peter Shirley, Raytracing in One Weekend, available here: 
https://raytracing.github.io/books/RayTracingInOneWeekend.html

Differently from his approach (printing the canvas into a .PPM file), I'm using the Win GDI+ for a more interactive approach, the image is rendered using GDI with a rate of 1 sample per frame and accumulated overtime.
The default approach uses all available cores but you can disable it by uncommenting the SINGLE_THREADED define in Raytracer.h.

# Build
Only windows libraries were used -> gdi32.lib; user32.lib
Open up the .sln file and compile it

# Images
1280x768 Fuzz = 0.15 metallic sphere
![Captura de tela 2024-07-27 221719](https://github.com/user-attachments/assets/3a5728c4-7fb2-40e4-adcb-fac4e5cbf283)

1280x768 Fuzz = 0 metallic sphere
![image](https://github.com/user-attachments/assets/7d8101b0-bf59-429a-a525-9b16b4aa14ac)
