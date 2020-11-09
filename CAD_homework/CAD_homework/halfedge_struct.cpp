#include "halfedge_struct.h"

//各拓扑结构的编号，用于调试
static int solid_id = 0;
static int face_id = 0;
static int loop_id = 0;
static int half_edge_id = 0;
static int vertex_id = 0;
static int edge_id = 0;


//coordinate functions
coordinate::coordinate()
{
	x = 0;
	y = 0;
	z = 0;
}

coordinate::coordinate(float x_init, float y_init, float z_init)
{
	x = x_init;
	y = y_init;
	z = z_init;
}


//solid functions
solid::solid()
{
	id = solid_id++;
	prev = nullptr;
	next = nullptr;
	solid_face = nullptr;
	solid_vertex = nullptr;

}

void solid::add_face(face *f)
{
	if (solid_face == nullptr)
		solid_face = f;
	else
	{
		face *current = solid_face;
		while (current->next)
			current = current->next;
		current->next = f;
		f->prev = current;
	}
	f->face_solid = this;
}

void solid::add_edge(edge *e)
{
	if (solid_edge == nullptr)
		solid_edge = e;
	else
	{
		edge *current = solid_edge;
		while (current->next)
			current = current->next;
		current->next = e;
		e->prev = current;
	}
}

void solid::add_vertex(vertex *v)
{
	if (solid_vertex == nullptr)
		solid_vertex = v;
	else
	{
		vertex *current = solid_vertex;
		while (current->next)
			current = current->next;
		current->next = v;
		v->prev = current;
	}
}

//face functions
face::face()
{
	id = face_id++;
	face_solid = nullptr;
	prev = nullptr;
	next = nullptr;
	face_loop = nullptr;
	face_inner = nullptr;
}

void face::add_loop(loop *l)
{
	if (face_loop == nullptr)
		face_loop = l;
	else
	{
		loop *current = face_loop;
		while (current->next)
			current = current->next;
		current->next = l;
		l->prev = current;
	}
	l->loop_face = this;
}

void face::add_inner(loop *l)
{
	if (face_inner == nullptr)
		face_inner = l;
	else
	{
		loop *current = face_inner;
		while (current->next)
			current = current->next;
		current->next = l;
		l->prev = current;
	}
	l->loop_face = this;
}


//loop functions
loop::loop()
{
	id = loop_id++;
	loop_face = nullptr;
	prev = nullptr;
	next = nullptr;
	loop_half_edge = nullptr;
}


//half_edge functions
half_edge::half_edge()
{
	id = half_edge_id++;
	prev = nullptr;
	next = nullptr;
	partner = nullptr;

	half_edge_loop = nullptr;
	half_edge_edge = nullptr;
	start_vertex = nullptr;
	end_vertex = nullptr;
}


//vertex functions
vertex::vertex()
{
	id = vertex_id++;
	prev = nullptr;
	next = nullptr;
}

//edge functions
edge::edge()
{
	id = edge_id++;
	prev = nullptr;
	next = nullptr;
	he1 = nullptr;
	he2 = nullptr;
}
