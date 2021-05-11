# Documentation of ICP project 2020/2021
- Authors:
  - Ilya Doroshenko (xdoros01)
  - David Černý (xcerny74)

## About
The project implements a **heavily modified** version of the 2. project variant
(editor and interpreter of structured blocks), it instead implements a texture editor that is similar to texture
editors in 3D modeling software.

## Requirements
- *Qt* version **5.12**+
- *cmake* version **3.16**+
- *conan* package manager
    - Install using `pip3 install conan`
- *fmt* library version **7.1.3**+ (will be installed automatically by conan during cmake build)
- *OpenGL* version **4.2**

## Features
- Project window for choosing recent projects, creating new projects and opening existing projects
- Canvas for manipulating texture nodes
  - **Input nodes** - emit a constant texture (like a geometric shape)
  - **Operation nodes** - modify input textures and output modified textures
  - **Output nodes** - can be exported as images
- Projects can be saved to **.vtproj** files
- Node library stored in JSON format

## Limitations
- Nodes can not be renamed
- Composite nodes were not implemented
- The node library can't be edited from the program itself (only in the library file)

## Used sources
- TODO
