#include <registry/registry.hpp>
#include <render.hpp>

   DB     Render::db[2];
 uint8_t  Render::dbx;
 uint16_t Render::tilex;
 uint8_t  Render::tpagex;
 uint16_t Render::timx;

Registry* Render::registry;

int Render::Initialize() {
    dbx = 0;

    ResetGraph(0);

    SetDefDispEnv(&db[0].disp, 0, 0, WIDTH, HEIGHT);
    SetDefDrawEnv(&db[0].draw, 0, HEIGHT, WIDTH, HEIGHT);
    SetDefDispEnv(&db[1].disp, 0, HEIGHT, WIDTH, HEIGHT);
    SetDefDrawEnv(&db[1].draw, 0, 0, WIDTH, HEIGHT);

    SetVideoMode(MODE_NTSC);

    for (int i = 0; i < 2; i++) {
        db[i].draw.isbg = 1;
        setRGB0(&db[i].draw, 50, 50, 50);
    }

    SetDispMask(1);

    return 0;
}

void Render::Present() {
    tilex = 0;
    tpagex = 0;
    timx = 0;
    flip();

    DrawSync(0);
    VSync(0);

    PutDispEnv(&db[dbx].disp);
    PutDrawEnv(&db[dbx].draw);

    DrawOTag(db[dbx].ot + LENGTH_OT - 1);

    dbx ^= 1;

    ClearOTagR(db[dbx].ot, LENGTH_OT);

}

void Render::SetRegistry(Registry* reg) {
    registry = reg;
}

void Render::flip() {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!(registry->flags[i] & VISUAL) || (registry->flags[i] & MESH)) continue;

        Position pos  = registry->positions[i];
        Size     size = registry->sizes    [i];
        CVECTOR  c    = registry->flags[i] & COLOR ? registry->colors[i] : CVECTOR{128, 128, 128};
        uint8_t abr = (registry->flags[i] & TRANS) ? ((registry->flags[i] & ABR_MASK) >> 6) : 0;

        if (registry->flags[i] & PRIMITIVE) {
            if (!(tilex < 512)) continue;
            POLY_F4* poly = &db[dbx].tiles[tilex++];

            setPolyF4(poly);

            setXY4(poly,
                pos.vx,                   pos.vy,
                pos.vx + size.w,          pos.vy,
                pos.vx,     pos.vy   +    size.h,
                pos.vx + size.w, pos.vy + size.h
            );

            setRGB0(poly, c.r, c.g, c.b);

            if (registry->flags[i] & TRANS) {
                setSemiTrans(poly, 1);

                DR_TPAGE* tpage = &db[dbx].dr_tpages[tpagex++];

                uint16_t tval = getTPage(0, abr, 0, 0);

                setDrawTPage(tpage, 0, 0, tval);

                addPrim(db[dbx].ot[pos.vz], poly);
                addPrim(db[dbx].ot[pos.vz], tpage);
                
                continue;
            }


            addPrim(db[dbx].ot[pos.vz], poly);

        } else {
            if (!(timx < 512)) continue;
            Texture* tex = &registry->textures[i];

            POLY_FT4* poly = &db[dbx].tims[timx++];

            setPolyFT4(poly);

            if (registry->flags[i] & TRANS) setSemiTrans(poly, 1); else abr = tex->tpage_mode;

            setRGB0(poly, c.r, c.g, c.b);
            setXY4(poly,
                pos.vx,                   pos.vy,
                pos.vx + size.w,          pos.vy,
                pos.vx,     pos.vy   +    size.h,
                pos.vx + size.w, pos.vy + size.h
            );


            uint16_t u = tex->u_offset;
            uint16_t v = tex->v_offset;
            setUV4(poly,
                u,                   v,
                u + size.w,          v,
                u,     v    +   size.h,
                u + size.w, v + size.h
            );
            setClut(poly, tex->crect.x, tex->crect.y);

            poly->tpage = getTPage(tex->tim_mode & 0x3, abr, tex->prect.x, tex->prect.y);

            addPrim(db[dbx].ot[pos.vz], poly);
        }
    }
}