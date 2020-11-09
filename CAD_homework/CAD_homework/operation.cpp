#include "operation.h"

//mvfs操作，构造一个体，一个面，一个外环，一个点
solid *mvfs(const coordinate &c)
{
	solid *s = new solid;
	face *f = new face;
	loop *l = new loop;
	vertex *v = new vertex;

	s->add_face(f);
	s->add_vertex(v);
	
	f->add_loop(l);

	v->position.x = c.x;
	v->position.y = c.y;
	v->position.z = c.z;

	return s;
}

//mev操作，构造一个新点，同时构造一条连接新点与另一给定点的边
half_edge *mev(const coordinate &c_target, const coordinate &c_new, loop *l)
{
	vertex *v_new = new vertex;
	v_new->position.x = c_new.x;
	v_new->position.y = c_new.y;
	v_new->position.z = c_new.z;

	solid *s = l->loop_face->face_solid;

	vertex *target = s->solid_vertex;
	while (target != nullptr)
	{
		if (target->position.x == c_target.x && target->position.y == c_target.y && target->position.z == c_target.z)
			break;
		target = target->next;
	}

	edge *e_new = new edge;
	half_edge *he_new1 = new half_edge;
	half_edge *he_new2 = new half_edge;

	s->add_edge(e_new);
	s->add_vertex(v_new);

	e_new->he1 = he_new1;
	e_new->he2 = he_new2;

	he_new1->half_edge_edge = e_new;
	he_new1->half_edge_loop = l;
	he_new1->start_vertex = target;
	he_new1->end_vertex = v_new;
	he_new1->partner = he_new2;

	he_new2->half_edge_edge = e_new;
	he_new2->half_edge_loop = l;
	he_new2->start_vertex = v_new;
	he_new2->end_vertex = target;
	he_new2->partner = he_new1;

	he_new1->next = he_new2;
	he_new2->prev = he_new1;

	//当loop为空时的情况
	if (l->loop_half_edge == nullptr)
	{
		l->loop_half_edge = he_new1;
		he_new1->prev = he_new2;
		he_new2->next = he_new1;
	}

	//当loop非空时的情况
	else
	{
		half_edge *temp = l->loop_half_edge;
		while (temp->end_vertex != target)
			temp = temp->next;

		he_new2->next = temp->next;
		temp->next->prev = he_new2;
		temp->next = he_new1;
		he_new1->prev = temp;
	}
	return he_new1;
}

//mef操作，在两给定点间构造一条新边，同时构造一个新环，一个新面
//新面包含以c_start为起点的半边，原来的面包含以c_end为起点的半边
//输入应保证生成环的唯一性和几何有效性
loop *mef(const coordinate &c_start, const coordinate &c_end, loop *l)
{
	face *f_new = new face;
	loop *l_new = new loop;
	edge *e_new = new edge;
	half_edge *he_new1 = new half_edge;
	half_edge *he_new2 = new half_edge;

	solid *s = l->loop_face->face_solid;
	s->add_face(f_new);
	s->add_edge(e_new);
	f_new->add_loop(l_new);
	e_new->he1 = he_new1;
	e_new->he2 = he_new2;

	/*-----定位起点和终点-----*/
	vertex *v_start;
	vertex *v_end;
	vertex *current = s->solid_vertex;
	while (current != nullptr)
	{
		if (current->position.x == c_start.x && current->position.y == c_start.y && current->position.z == c_start.z)
			break;
		current = current->next;
	}
	v_start = current;
	current = s->solid_vertex;
	while (current != nullptr)
	{
		if (current->position.x == c_end.x && current->position.y == c_end.y && current->position.z == c_end.z)
			break;
		current = current->next;
	}
	v_end = current;
	/*-----定位起始点和终点-----*/

	he_new1->partner = he_new2;
	he_new1->half_edge_loop = l_new;
	he_new1->half_edge_edge = e_new;
	he_new1->start_vertex = v_start;
	he_new1->end_vertex = v_end;

	he_new2->partner = he_new1;
	he_new2->half_edge_loop = l;
	he_new2->half_edge_edge = e_new;
	he_new2->start_vertex = v_end;
	he_new2->end_vertex = v_start;

	half_edge *he_before_newloop = l->loop_half_edge;
	half_edge *he_after_newloop = l->loop_half_edge;
	half_edge *he_before_oldloop = l->loop_half_edge;
	half_edge *he_after_oldloop = l->loop_half_edge;

	/*定位loop中与新边相邻的四条半边*/
	while (he_before_newloop != nullptr)
	{
		if (he_before_newloop->end_vertex == v_start)
			break;
		he_before_newloop = he_before_newloop->next;
	}
	while (he_after_newloop != nullptr)
	{
		if (he_after_newloop->start_vertex == v_end)
			break;
		he_after_newloop = he_after_newloop->next;
	}
	while (he_before_oldloop != nullptr)
	{
		if (he_before_oldloop->end_vertex == v_end)
			break;
		he_before_oldloop = he_before_oldloop->next;
	}
	while (he_after_oldloop != nullptr)
	{
		if (he_after_oldloop->start_vertex == v_start)
			break;
		he_after_oldloop = he_after_oldloop->next;
	}
	/*定位loop中与新边相邻的四条半边*/

	he_new1->next = he_after_newloop;
	he_new1->prev = he_before_newloop;
	he_after_newloop->prev = he_new1;
	he_before_newloop->next = he_new1;
	l_new->loop_half_edge = he_new1;
	half_edge *update = he_new1->next;
	while (update != he_new1)
	{
		update->half_edge_loop = l_new;
		update = update->next;
	}

	he_new2->next = he_after_oldloop;
	he_new2->prev = he_before_oldloop;
	he_after_oldloop->prev = he_new2;
	he_before_oldloop->next = he_new2;
	l->loop_half_edge = he_new2;
	return l_new;
}

//kemr操作，删去环中的一条边，并构造一个内环
//其中v_start属于外环，v_end属于新构造的内环
//输入应保证构建内环的唯一性
loop *kemr(const coordinate &c_start, const coordinate &c_end, loop *l)
{
	solid *s = l->loop_face->face_solid;
	face *f = l->loop_face;
	loop *l_new = new loop;
	f->add_inner(l_new);

	/*-----定位起点和终点-----*/
	vertex *v_start;
	vertex *v_end;
	vertex *current = s->solid_vertex;
	while (current != nullptr)
	{
		if (current->position.x == c_start.x && current->position.y == c_start.y && current->position.z == c_start.z)
			break;
		current = current->next;
	}
	v_start = current;
	current = s->solid_vertex;
	while (current != nullptr)
	{
		if (current->position.x == c_end.x && current->position.y == c_end.y && current->position.z == c_end.z)
			break;
		current = current->next;
	}
	v_end = current;
	/*-----定位起始点和终点-----*/

	/*-----定位待删除的半边-----*/
	half_edge *target_start_end = l->loop_half_edge;
	while (target_start_end)
	{
		if (target_start_end->start_vertex == v_start && target_start_end->end_vertex == v_end)
			break;
		target_start_end = target_start_end->next;
	}
	half_edge *target_end_start = target_start_end->partner;
	/*-----定位待删除的半边-----*/

	/*-----内环不为空时构造内环-----*/
	if (target_start_end->next != target_end_start)
	{
		l_new->loop_half_edge = target_start_end->next;
		target_start_end->next->prev = target_end_start->prev;
		target_end_start->prev->next = target_start_end->next;
		half_edge *current = l_new->loop_half_edge;
		current->half_edge_loop = l_new;
		current = current->next;
		while (current != l_new->loop_half_edge)
		{
			current->half_edge_loop = l_new;
			current = current->next;
		}
	}
	/*-----内环不为空时构造内环-----*/

	/*-----更新外环信息-----*/
	target_start_end->prev->next = target_end_start->next;
	target_end_start->next->prev = target_start_end->prev;
	l->loop_half_edge = target_start_end->prev;
	/*-----更新外环信息-----*/

	/*-----删除边对应的数据结构*/
	edge *target_e = target_start_end->half_edge_edge;
	if (target_e->prev == nullptr)
	{
		s->solid_edge = target_e->next;
		if(target_e->next != nullptr)
			target_e->next->prev = nullptr;
	}//待删除边在链表头
	else
	{
		target_e->prev->next = target_e->next;
		if(target_e->next != nullptr)
			target_e->next->prev = target_e->prev;
	}

	/*-----删除对应边的数据结构-----*/
	delete target_e;
	delete target_start_end;
	delete target_end_start;
	/*-----删除对应边的数据结构-----*/
	return l_new;
}

//kfmrh操作，删去一个面，并将其定义为一个内环，进而在体中构建一个柄
//没有考虑合并两个物体的情况，输入应保证几何有效性
void kfmrh(loop *inner_loop, loop *outer_loop)
{
	face *f_to_del = inner_loop->loop_face;
	face *f_to_update = outer_loop->loop_face;

	/*-----更新数据结构-----*/
	f_to_update->add_inner(inner_loop);
	inner_loop->loop_face = f_to_update;
	/*-----更新数据结构-----*/

	if (f_to_del->prev == nullptr)
	{
		f_to_del->face_solid->solid_face = f_to_del->next;
		if (f_to_del->next != nullptr)
			f_to_del->next->prev = nullptr;
	}//待删除面在链表头
	else
	{
		f_to_del->prev->next = f_to_del->next;
		if (f_to_del->next != nullptr)
		{
			f_to_del->next->prev = f_to_del->prev;
		}
	}//待删除面不在链表头

	delete f_to_del;
	return;
}

//扫成操作,根据指定扫成方向和一个由欧拉操作生成的面构造实体
solid *sweep_operation(const coordinate &sweep_direction, face *f)
{
	float delta_x = sweep_direction.x;
	float delta_y = sweep_direction.y;
	float delta_z = sweep_direction.z;
	solid *s = f->face_solid;

	loop *l_current;
	loop *outer_loop;
	vertex *v_first;
	vertex *v_current, *first_up;
	
	half_edge *he_current;
	half_edge *he_new;

	coordinate c_current_vertex, c_new_vertex, c_prev_up;

	/*----------外环扫成----------*/
	l_current = f->face_loop->loop_half_edge->partner->half_edge_loop; //基于与外环反向的环进行欧拉操作
	outer_loop = l_current;
	v_first = l_current->loop_half_edge->start_vertex;
	he_current = l_current->loop_half_edge;

	c_current_vertex.x = v_first->position.x;
	c_current_vertex.y = v_first->position.y;
	c_current_vertex.z = v_first->position.z;
	c_new_vertex.x = v_first->position.x + delta_x;
	c_new_vertex.y = v_first->position.y + delta_y;
	c_new_vertex.z = v_first->position.z + delta_z;

	he_new = mev(c_current_vertex, c_new_vertex, l_current);
	first_up = he_new->end_vertex;
	c_prev_up.x = first_up->position.x;
	c_prev_up.y = first_up->position.y;
	c_prev_up.z = first_up->position.z;

	while (he_current->end_vertex != v_first)
	{
		v_current = he_current->end_vertex;
	
		c_current_vertex.x = v_current->position.x;
		c_current_vertex.y = v_current->position.y;
		c_current_vertex.z = v_current->position.z;
		c_new_vertex.x = v_current->position.x + delta_x;
		c_new_vertex.y = v_current->position.y + delta_y;
		c_new_vertex.z = v_current->position.z + delta_z;

		he_current = he_current->next;

		mev(c_current_vertex, c_new_vertex, l_current);
		mef(c_new_vertex, c_prev_up, l_current);

		c_prev_up.x = c_new_vertex.x;
		c_prev_up.y = c_new_vertex.y;
		c_prev_up.z = c_new_vertex.z;
	}

	c_new_vertex.x = first_up->position.x;
	c_new_vertex.y = first_up->position.y;
	c_new_vertex.z = first_up->position.z;
	mef(c_new_vertex, c_prev_up, l_current);
	/*----------外环扫成----------*/

	/*----------内环扫成----------*/
	//与外环的扫成操作类似，只需添加kfmrh()操作
	loop *l_op = f->face_inner;
	while (l_op != nullptr)
	{
		l_current = l_op->loop_half_edge->partner->half_edge_loop;
		v_first = l_current->loop_half_edge->start_vertex;
		he_current = l_current->loop_half_edge;

		c_current_vertex.x = v_first->position.x;
		c_current_vertex.y = v_first->position.y;
		c_current_vertex.z = v_first->position.z;
		c_new_vertex.x = v_first->position.x + delta_x;
		c_new_vertex.y = v_first->position.y + delta_y;
		c_new_vertex.z = v_first->position.z + delta_z;

		he_new = mev(c_current_vertex, c_new_vertex, l_current);
		first_up = he_new->end_vertex;
		c_prev_up.x = first_up->position.x;
		c_prev_up.y = first_up->position.y;
		c_prev_up.z = first_up->position.z;

		while (he_current->end_vertex != v_first)
		{
			v_current = he_current->end_vertex;

			c_current_vertex.x = v_current->position.x;
			c_current_vertex.y = v_current->position.y;
			c_current_vertex.z = v_current->position.z;
			c_new_vertex.x = v_current->position.x + delta_x;
			c_new_vertex.y = v_current->position.y + delta_y;
			c_new_vertex.z = v_current->position.z + delta_z;

			he_current = he_current->next;

			mev(c_current_vertex, c_new_vertex, l_current);
			mef(c_new_vertex, c_prev_up, l_current);

			c_prev_up.x = c_new_vertex.x;
			c_prev_up.y = c_new_vertex.y;
			c_prev_up.z = c_new_vertex.z;
		}

		c_new_vertex.x = first_up->position.x;
		c_new_vertex.y = first_up->position.y;
		c_new_vertex.z = first_up->position.z;
		mef(c_new_vertex, c_prev_up, l_current);
		kfmrh(l_current, outer_loop);

		l_op = l_op->next;
	}
	/*----------内环扫成----------*/

	return s;
}