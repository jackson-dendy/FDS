# 🔥 FDS Solver Framework

This project is a **Finite Difference Scheme (FDS)** solver designed for flexible simulation of partial differential equations (PDEs) such as heat conduction, diffusion, and wave equations. It is written in **C++ for high performance** and integrates seamlessly with **Python** via bindings, allowing rapid experimentation, visualization, and post-processing.

## 🚀 Features

- Modular support for multiple PDEs (e.g., 1D/2D/3D transient heat equations, advection-diffusion, etc.)
- Written in modern C++ for speed and clarity
- Exposed to Python for easy simulation setup, visualization, and automation
- Built-in mesh generators and solvers
- Compatible with `matplotlib` for animation and plotting

---

## 🛠️ Setup Instructions

### 1. Build C++ Core

```bash
cd build
cmake .. 
cd ..
cmake --build build --target ALL_BUILD --config Release