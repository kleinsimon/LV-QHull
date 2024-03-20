LV-QHull
--------
A wrapper allowing to use some functions of the qhull library in LabVIEW:
- Calculation of the convex hull in N-D
- Delaunay Triangulation in N-D

Usage
-----
Included is the wrapper library written in LV2023-Q3 as well as dlls for Windows-X64. Other systems and LV-versions may be added in future.

Compilation
-----------
Because of the complex qh structure and memory issues, the qhull library could not be importet directly in labview. An additional C-wrapper is needed (LV-QHull.dll).
The sources reside in the "Wrapper" folder. You may use them to build shared libraries for other systems. It is linked against QHull-2020.2.
