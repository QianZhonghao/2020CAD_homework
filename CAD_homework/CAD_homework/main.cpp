//CAD homework

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "halfedge_struct.h"
#include "operation.h"
using namespace std;

/*-----���ڴ洢ɨ�ɲ���������-----*/
#define max_inner_loop_in_face 5
#define max_vertex_in_a_loop 10
int outer_loop_vertex_num;
int inner_loop_num;
int inner_loop_vertex_num;
coordinate outer_loop_vertex[max_vertex_in_a_loop];
coordinate inner_loop_vertex[max_vertex_in_a_loop];
/*-----���ڴ洢ɨ�ɲ���������-----*/

//��������������е��棬���Լ������Ϣ
void print_info(solid *s)
{
	ofstream output;
	output.open("result.txt");
	output.clear();
	if (s == nullptr) return;
	face *f = s->solid_face;
	while (f != nullptr)
	{
		output << "face_id: " << f->id << endl;
		output << '{' << endl;
		loop *l = f->face_loop;
		while (l != nullptr)
		{
			if (l->loop_face->id != f->id)
			{
				output << "face&loop error! " << f->id << endl << endl;
			}
			output << "	loop_id: " << l->id <<  endl;
			output << "	[" << endl;
			half_edge *he = l->loop_half_edge;
			half_edge *first = he;
			while (he != nullptr)
			{
				//output << "		half_edge_id: " << he->id << endl;
				if (he->half_edge_loop->id != l->id)
				{
					cout << "loop&half_edge error!" << f->id << ' ' << l->id << endl << endl;
				}
				output << "		" << he->start_vertex->id << "->" << he->end_vertex->id << "	start vertex coordinate: ";
				output << "		" << he->start_vertex->position.x << ' ' << he->start_vertex->position.y << ' ' << he->start_vertex->position.z << endl;
				//output << "		" << he->end_vertex->position.x << ' ' << he->end_vertex->position.y << ' ' << he->end_vertex->position.z << endl;
				he = he->next;
				if (he == first)
					break;
			}
			output << "	]" << endl;
			l = l->next;
		}
		l = f->face_inner;
		while (l != nullptr)
		{
			if (l->loop_face->id != f->id)
			{
				cout << "face&loop error! " << f->id << endl << endl;
			}
			output << "	inner_loop_id: " << l->id << endl;
			output << "	[" << endl;
			half_edge *he = l->loop_half_edge;
			half_edge *first = he;
			while (he != nullptr)
			{
				//output << "		half_edge_id: " << he->id << endl;
				if (he->half_edge_loop->id != l->id)
				{
					cout << "loop&half_edge error!" << f->id << ' ' << l->id << endl << endl;
				}
				output << "		" << he->start_vertex->id << "->" << he->end_vertex->id << "	start vertex coordinate: ";
				output << "		" << he->start_vertex->position.x << ' ' << he->start_vertex->position.y << ' ' << he->start_vertex->position.z << endl;
				//output << "		" << he->end_vertex->position.x << ' ' << he->end_vertex->position.y << ' ' << he->end_vertex->position.z << endl;
				he = he->next;
				if (he == first)
					break;
			}
			output << "	]" << endl;
			l = l->next;
		}
		output << '}' << endl;
		f = f->next;
	}
	output.close();
}

//��ŷ���������������壬���ڲ���mvfs(),mev(),mef()
solid *cube_by_euler()
{
	coordinate v0(0, 0, 0);
	coordinate v1(1, 0, 0);
	coordinate v2(0, 1, 0);
	coordinate v3(1, 1, 0);
	coordinate v4(0, 0, 1);
	coordinate v5(1, 0, 1);
	coordinate v6(0, 1, 1);
	coordinate v7(1, 1, 1);
	solid *s = mvfs(v0);
	loop *l = s->solid_face->face_loop;
	mev(v0, v1, l);
	mev(v0, v2, l);
	mev(v1, v3, l);
	mef(v2, v3, l);
	mev(v0, v4, l);
	mev(v1, v5, l);
	mef(v5, v4, l);
	mev(v2, v6, l);
	mef(v4, v6, l);
	mev(v3, v7, l);
	mef(v7, v5, l);
	mef(v6, v7, l);
	return s;
}

//��ŷ��������������������壬���ڲ���kemr(),kfmrh()
solid *cube_with_one_handle_by_euler()
{
	coordinate v0(0, 0, 0);
	coordinate v1(3, 0, 0);
	coordinate v2(3, 3, 0);
	coordinate v3(0, 3, 0);
	coordinate v4(0, 0, 3);
	coordinate v5(3, 0, 3);
	coordinate v6(3, 3, 3);
	coordinate v7(0, 3, 3);
	coordinate v8(2, 1, 3);
	coordinate v9(1, 1, 3);
	coordinate v10(1, 2, 3);
	coordinate v11(2, 2, 3);
	coordinate v12(2, 1, 0);
	coordinate v13(1, 1, 0);
	coordinate v14(1, 2, 0);
	coordinate v15(2, 2, 0);
	solid *s = mvfs(v0);
	loop *l = s->solid_face->face_loop;
	mev(v0, v1, l);
	mev(v1, v2, l);
	mev(v2, v3, l);
	loop *base = mef(v0, v3, l);
	mev(v0, v4, l);
	mev(v1, v5, l);
	mef(v5, v4, l);
	mev(v2, v6, l);
	mef(v6, v5, l);
	mev(v3, v7, l);
	mef(v7, v6, l);
	mef(v4, v7, l);
	mev(v5, v8, l);
	l = kemr(v5, v8, l);
	mev(v8, v9, l);
	mev(v9, v10, l);
	mev(v10, v11, l);
	l = mef(v8, v11, l);
	mev(v8, v12, l);
	mev(v9, v13, l);
	mef(v12, v13, l);
	mev(v10, v14, l);
	mef(v13, v14, l);
	mev(v11, v15, l);
	mef(v14, v15, l);
	mef(v15, v12, l);
	kfmrh(l, base);
	return s;
}

//��ɨ�ɲ������������壬���ڲ���sweep_operation()
solid *cube_by_sweep()
{
	coordinate v0(0, 0, 0);
	coordinate v1(1, 0, 0);
	coordinate v2(1, 1, 0);
	coordinate v3(0, 1, 0);
	solid *s = mvfs(v0);
	loop *l = s->solid_face->face_loop;
	mev(v0, v1, l);
	mev(v1, v2, l);
	mev(v2, v3, l);
	loop *l_base = mef(v0, v3, l);
	face *f_base = l_base->loop_face;
	coordinate direction(0, 0, 1);
	sweep_operation(direction, f_base);
	return s;
}

//��ɨ�ɲ�����������������壬���ڲ���sweep_operation()
solid *cube_with_one_handle_by_sweep()
{
	coordinate v0(0, 0, 0);
	coordinate v1(3, 0, 0);
	coordinate v2(3, 3, 0);
	coordinate v3(0, 3, 0);
	coordinate v4(1, 1, 0);
	coordinate v5(2, 1, 0);
	coordinate v6(2, 2, 0);
	coordinate v7(1, 2, 0);
	solid *s = mvfs(v0);
	loop *l = s->solid_face->face_loop;
	mev(v0, v1, l);
	mev(v1, v2, l);
	mev(v2, v3, l);
	loop *l_base = mef(v0, v3, l);
	face *f_base = l_base->loop_face;
	l = l_base;
	mev(v0, v4, l);
	l = kemr(v0, v4, l);
	mev(v4, v5, l);
	mev(v5, v6, l);
	mev(v6, v7, l);
	mef(v4, v7, l);
	coordinate direction(0, 0, 3);
	sweep_operation(direction, f_base);

	return s;
}

int main()
{
	int operation;
	cout << "����������ѡ�����Ĳ���" << endl;
	cout << "����1: �����ŷ�����������Ĭ��������" << endl;
	cout << "����2: �����ŷ�����������Ĭ�ϴ�һ������������" << endl;
	cout << "����3: �����ɨ�ɲ��������Ĭ��������" << endl;
	cout << "����4: �����ɨ�ɲ��������Ĭ�ϴ�һ������������" << endl;
	cout << "����5���Զ���ɨ�ɲ���" << endl;

	cin >> operation;
	solid *s = nullptr;
	if (operation == 1)
		s = cube_by_euler();
	else if (operation == 2)
		s = cube_with_one_handle_by_euler();
	else if (operation == 3)
		s = cube_by_sweep();
	else if (operation == 4)
		s = cube_with_one_handle_by_sweep();
	else if (operation == 5)//�����û��������Ϣ�����Զ���ɨ����
	{
		/*-----����ɨ�ɷ���-----*/
		coordinate direction;
		float delta_x, delta_y, delta_z;
		cout << "������ɨ�ɲ����ķ��� delta_x  delta_y  delta_z" << endl;
		cin >> delta_x >> delta_y >> delta_z;
		direction.x = delta_x;
		direction.y = delta_y;
		direction.z = delta_z;

		/*-----�����⻷��Ϣ-----*/
		cout << "�������⻷�ж���ĸ���(����Ϊ3�Ҳ�����10)" << endl;
		cin >> outer_loop_vertex_num;
		cout << "�밴���ַ���˳������ɨ�����⻷�и���������� x y z" << endl;
		float c_x, c_y, c_z;
		for (int i = 0; i < outer_loop_vertex_num; i++)
		{
			cin >> c_x >> c_y >> c_z;
			outer_loop_vertex[i].x = c_x;
			outer_loop_vertex[i].y = c_y;
			outer_loop_vertex[i].z = c_z;
		}

		/*------�����⻷------*/
		s = mvfs(outer_loop_vertex[0]);
		loop *l_current, *l_base;
		face *f_base;
		l_current = s->solid_face->face_loop;
		for (int i = 1; i < outer_loop_vertex_num; i++)
			mev(outer_loop_vertex[i - 1], outer_loop_vertex[i], l_current);
		l_base = mef(outer_loop_vertex[outer_loop_vertex_num - 1], outer_loop_vertex[0], l_current);
		f_base = l_base->loop_face;
		l_current = l_base;

		cout << "�������ڻ�����(������5)" << endl;
		cin >> inner_loop_num;
		
		for (int i = 0; i < inner_loop_num; i++)
		{
			//-----�����ڻ���Ϣ-----//
			cout << "��������ڻ��ж���ĸ���(����Ϊ3�Ҳ�����10)" << endl;
			cin >> inner_loop_vertex_num;
			cout << "�밴���ַ���˳��������ڻ��и���������� x y z (�ڻ��������⻷�෴)" << endl;
			for (int j = 0; j < inner_loop_vertex_num; j++)
			{
				cin >> c_x >> c_y >> c_z;
				inner_loop_vertex[j].x = c_x;
				inner_loop_vertex[j].y = c_y;
				inner_loop_vertex[j].z = c_z;
			}
			
			//------�����ڻ�------//
			mev(outer_loop_vertex[0], inner_loop_vertex[0], l_current);
			l_current = kemr(outer_loop_vertex[0], inner_loop_vertex[0], l_current);
			for (int j = 1; j < inner_loop_vertex_num; j++)
				mev(inner_loop_vertex[j - 1], inner_loop_vertex[j], l_current);
			mef(inner_loop_vertex[0], inner_loop_vertex[inner_loop_vertex_num - 1], l_current);
			l_current = l_base;
		}
		
		/*------ɨ�ɲ���------*/
		sweep_operation(direction, f_base);
	}
	else
		cout << "invalid operation!" << endl;
	cout << "������ɣ���Ϊ��Ч���룬����������result.txt�ļ�" << endl;
	print_info(s);

	system("pause");
	return 0;
}