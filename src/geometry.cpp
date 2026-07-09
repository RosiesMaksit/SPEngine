#include <game.hpp>
#include <render.hpp>
#include <geometry.hpp>
#include <registry/registry.hpp>

uint16_t Geometry  ::  polyx;

 MATRIX  Geometry::cameraMtx;
 VECTOR  Geometry::cameraPos;
SVECTOR  Geometry::cameraRot;

Registry* Geometry::registry;

int Geometry::Initialize() {
    InitGeom();

    gte_SetGeomOffset(WIDTH / 2, HEIGHT / 2);
    gte_SetGeomScreen(LENGTH_OT);

    return 0;
}

void Geometry::SetRegistry(Registry* reg) {
    registry = reg;
}

void Geometry::Update() {
    upd_cam();

    polyx = 0;

    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!(registry->flags[i] & (VISUAL | MESH))) continue;
        VECTOR pos  = registry->positions[i];
        SVECTOR rot = registry->rotations[i];
        CVECTOR c   = registry->colors[i];

        set_matrix(&pos, &rot);

        Mesh mesh = registry->meshes[i];
        int count = mesh.count_triangles;

        for (int f = 0; f < count; f++) {
            if (polyx >= 512) continue;

            POLY_F3* poly = &Render::db[Render::dbx].polys[polyx++];

            int otz, p;

            Polygon* face = &mesh.triangles[f];

            SVECTOR* v0 = &face->points[0]; 
            SVECTOR* v1 = &face->points[1];
            SVECTOR* v2 = &face->points[2];

            gte_ldv3(v0, v1, v2);
            
            gte_rtpt();
            gte_nclip();
            gte_stopz(&p);
            if (p <= 0) continue;

            gte_avsz3();
            gte_stotz(&otz);

            if (otz <= 1 || otz >= 4096) continue;

            setPolyF3(poly);
            setRGB0(poly, c.r, c.g, c.b);

            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);

            addPrim(&Render::db[Render::dbx].ot[otz], poly);

        }
    }

    /* примерно так:
    upd_camera();
    for (objects) {
        set_mtx();
    }

    set_mtx() {
        for (polygons) {
            POLY_F3* poly = &db[dbx].polys[polyx++];
        }
    } сделать
    */
}



void Geometry::upd_cam() {
    VECTOR invPos;
    invPos = {-cameraPos.vx, -cameraPos.vy, -cameraPos.vz};

    RotMatrix(&cameraRot, &cameraMtx);

    ApplyMatrixLV(&cameraMtx, &invPos, &invPos);
    TransMatrix(&cameraMtx, &invPos);
}

void Geometry::set_matrix(VECTOR* pos, SVECTOR* rot) {
    MATRIX oMtx, mtx;
    
    RotMatrix(rot, &oMtx);
    TransMatrix(&oMtx, pos);

    CompMatrixLV(&cameraMtx, &oMtx, &mtx);

    gte_SetRotMatrix(&mtx);
    gte_SetTransMatrix(&mtx);
}

void Geometry::SetCamera(VECTOR cam_pos, SVECTOR cam_rot) {
    cameraPos = cam_pos;
    cameraRot = cam_rot;
}