//define half_edge data structure
#pragma once

//坐标
class coordinate;

//半边数据结构各拓扑元素
class solid;
class face;
class loop;
class half_edge;
class vertex;
class edge;

class coordinate
{
public:
	float x;
	float y;
	float z;

public:
	coordinate();
	coordinate(float x_init, float y_init, float z_init);
	//coordinate& operator= (const coordinate &c);
};

class solid
{
public:
	int id;
	solid *prev;
	solid *next;
	face *solid_face;
	//表示物体只需一个edge链表和一个vertex链表，在solid中存储其头结点以备遍历
	edge *solid_edge;
	vertex *solid_vertex;

public:
	solid();
	void add_face(face *f);
	void add_edge(edge *e);
	void add_vertex(vertex *v);

};

class face 
{
public:
	int id;
	solid *face_solid;
	face *prev;
	face *next;
	loop *face_loop;
	loop *face_inner;

public:
	face();
	void add_loop(loop *l);
	void add_inner(loop *l);
};

class loop
{
public:
	int id;
	face *loop_face;
	loop *prev;
	loop *next;
	half_edge *loop_half_edge;

public:
	loop();
};

class half_edge
{
public:
	int id;
	half_edge *prev;
	half_edge *next;
	half_edge *partner;

	loop *half_edge_loop;
	edge *half_edge_edge;
	vertex *start_vertex;
	vertex *end_vertex;

public:
	half_edge();

};

class vertex
{
public:
	int id;
	coordinate position;
	vertex *prev;
	vertex *next;

public:
	vertex();

};

class edge
{
public:
	int id;
	edge *prev;
	edge *next;
	half_edge *he1;
	half_edge *he2;

public:
	edge();

};