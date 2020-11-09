//define operations
#include "halfedge_struct.h"

#pragma once

//5个欧拉操作
solid *mvfs(const coordinate &c);
half_edge *mev(const coordinate &c_target, const coordinate &c_new, loop *l);
loop *mef(const coordinate &c_start, const coordinate &c_end, loop *l);
loop *kemr(const coordinate &c_start, const coordinate &c_end, loop *l);
void kfmrh(loop *inner_loop, loop *outer_loop);

//扫成操作
solid *sweep_operation(const coordinate &sweep_direction, face *f);