# RayTracingInAWeekend
This is my implementation of a raytracer based on the book by Peter Shirley, Raytracing in One Weekend, available here: 
https://raytracing.github.io/books/RayTracingInOneWeekend.html

Differently from his approach (printing the canvas into a .PPM file), I'm using the Win GDI+ for a more interactive approach, the image is rendered using GDI with a rate of 1 sample per frame and accumulated overtime.
The default approach uses all available cores but you can disable it by uncommenting the SINGLE_THREADED define in Raytracer.h.

# Build
Only windows libraries were used -> gdi32.lib; user32.lib
Open up the .sln file and compile it
