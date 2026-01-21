# 3D Graphics Engine from Scratch (C/OpenGL)

This project is a 3D graphics engine implementation built from scratch using C and Legacy OpenGL (GLUT). 

Instead of relying on built-in OpenGL transformation functions (like `gluLookAt` or `glRotate`), this project implements the core **3D projection logic, matrix rotations, and camera mathematics manually**.

![3D Engine Demo](screenshots/3D_demo1.png)

## 🚀 Features

* **Manual 3D Mathematics:** All 3D-to-2D projection and coordinate transformations (Rotation X/Y/Z) are calculated manually using trigonometric functions.
* **Custom Camera System:** First-person camera implementation with pitch/yaw control controlled by mouse and keyboard.
* **Physics Basics:** Simple gravity, jumping logic, and crouch mechanics.
* **Wireframe & Polygon Rendering:** Rendering logic for 3D primitives.
* **Collision Detection:** Basic floor collision handling.

## 🛠️ Technologies Used

* **Language:** C
* **Library:** OpenGL / GLUT (freeglut)
* **IDE:** Visual Studio

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **W, A, S, D** | Move Camera (Walk) |
| **Mouse** | Look Around (Pitch/Yaw) |
| **Space** | Jump |
| **C** | Crouch |
| **ESC** | Exit Application |

## 🧮 Code Highlight: Manual Projection

One of the key aspects of this project is avoiding the black-box of OpenGL's internal matrix stack for understanding the underlying math. For example, the rotation logic is implemented manually:

```c
coord rotateXZ(coord p, float angle) {
    coord temp;
    float c = cos(angle);
    float s = sin(angle);
    temp.x = p.x * c - p.z * s;
    temp.z = p.x * s + p.z * c;
    temp.y = p.y;
    return temp;
}
```
📥 How to Run

Go to the Releases section on the right.

Download the latest .zip file.

Extract the folder.

Run 3D_World.exe (Ensure glut32.dll is in the same folder).

Developed by Burak Ayberk Caglar - Bilkent University CTIS major
