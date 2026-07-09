#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <types.hpp>

class Registry;

class Geometry {
    static uint16_t polyx;

    static MATRIX cameraMtx;
    static VECTOR  cameraPos;  
    static SVECTOR cameraRot;

    static Registry* registry;


    static void upd_cam();
    static void set_matrix(VECTOR* pos, SVECTOR* rot);

    public:
    static int Initialize();

    static void SetRegistry(Registry* reg);
    static void SetCamera(VECTOR cam_pos, SVECTOR cam_rot);

    static void Update();
};

#endif