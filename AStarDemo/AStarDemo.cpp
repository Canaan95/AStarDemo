// AStarDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

#define BEGIN_POS_VALUE 2
#define END_POS_VALUE 3
#define OBSTACLE_VALUE 1
#define NORMAL_VALUE 0

#define MAP_MAX_ROW 5
#define MAP_MAX_COL 7

static std::vector<std::vector<int>> map_info = { 
													{ 0, 0, 0, 0, 0, 0, 0} ,
													{ 0, 0, 0, 1, 0, 0, 0} ,
													{ 0, 2, 0, 1, 0, 3, 0} ,
													{ 0, 0, 0, 1, 0, 0, 0} ,
													{ 0, 0, 0, 0, 0, 0, 0} ,
												};


class NavigatePoint
{
public:
	NavigatePoint(){}
	~NavigatePoint(){}

	bool operator<(const NavigatePoint& p)
	{
		return this->f < p.f;
	}

	int row = -1;
	int col = -1;
	int parent_index = -1;
	int f = 0;
	int g = 0;
	int h = 0;
};

static std::vector<std::vector<NavigatePoint>> map_data(map_info.size(), std::vector<NavigatePoint>());
static std::list<NavigatePoint> open_list;
static std::list<NavigatePoint> closed_list;

int find_position_by_value(int value)
{
	for (int row = 0; row < map_info.size(); ++row)
	{
		for (int col = 0; col < map_info[row].size(); ++col)
		{
			if (map_info[row][col] == value)
				return row * 10 + col;
		}
	}

	return -1;
}

void init_map_data()
{
	for (int row = 0; row < map_data.size(); ++row)
		map_data[row].resize(MAP_MAX_COL);

	for (int row = 0; row < map_info.size(); ++row)
	{
		NavigatePoint p;
		p.row = row;
		for (int col = 0; col < map_info[row].size(); ++col)
		{
			p.col = col;
			p.f = p.g = p.h = 0;
			p.parent_index = -1;
			
			map_data[row][col] = p;
		}
	}
}

bool check_point_in_open_list(const NavigatePoint& point)
{
	const auto find_it = find_if(open_list.begin(), open_list.end(), [&](const NavigatePoint& p) {
		return point.col == p.col && point.row == p.row;
	});

	return find_it != open_list.end();
}

void calc_f_value(NavigatePoint& point, int begin_pos, int end_pos)
{
	int begin_row = begin_pos / 10;
	int begin_col = begin_pos % 10;
	int end_row = end_pos / 10;
	int end_col = end_pos % 10;

	int parent_row = point.parent_index / 10;
	int parent_col = point.parent_index % 10;
	if (point.parent_index == -1)
		point.g = 0;
	else
	{
		const NavigatePoint& parent_point = map_data[parent_row][parent_col];
		point.g = parent_point.g + 10;
	}

	point.h = (abs(point.row - begin_row) + abs(point.col - begin_col)) * 10;
	point.f = point.g + point.h;
	
	printf("calc_f_value, (%d, %d), g:%d, f:%d, h:%d\n", point.row, point.col, point.g, point.f, point.h);
}

void update_nearby_point_info(int cur_index, int begin_pos, int end_pos)
{
	auto _update = [&](int _row, int _col)
	{
		NavigatePoint p = map_data[_row][_col];
		if (!check_point_in_open_list(p))
		{
			calc_f_value(p, begin_pos, end_pos);
			p.parent_index = cur_index;
			open_list.emplace_back(p);
		}
	};

	int row = cur_index / 10;
	int col = cur_index % 10;

	// top
	int next_row = row - 1;
	if (next_row)
		_update(next_row, col);

	// bottom
	next_row = row + 1;
	if (next_row < map_data.size())
		_update(next_row, col);

	// left
	int next_col = col - 1;
	if (next_col)
		_update(row, next_col);

	// right
	next_col = col + 1;
	if (next_col < MAP_MAX_COL)
		_update(row, next_col);
}

NavigatePoint select_nearby_min_f_point()
{
	open_list.sort();
	//sort(open_list.begin(), open_list.end());

	NavigatePoint p = *(open_list.begin());
	open_list.pop_front();
	return p;
}

const list<NavigatePoint>& find_path(int begin_pos, int end_pos)
{
	int begin_row = begin_pos / 10;
	int begion_col = begin_pos % 10;
	int end_row = end_pos / 10;
	int end_col = end_pos % 10;

	NavigatePoint p;
	open_list.emplace_back(p);

	while (!open_list.empty())
	{
		NavigatePoint select_point = select_nearby_min_f_point();
		closed_list.emplace_back(select_point);
		if (select_point.col == end_col && select_point.row == end_row)
			break;
		update_nearby_point_info(select_point.row * 10 + select_point.col, begin_pos, end_pos);
	}

	return closed_list;
}

int main()
{
	/*
	int begion_pos = find_position_by_value(BEGIN_POS_VALUE);
	int end_pos = find_position_by_value(END_POS_VALUE);
	
	cout << "begin_pos: " << begion_pos << " ,end_pos: " << end_pos << endl;

	init_map_data();
	const auto& path = find_path(begion_pos, end_pos);
	for (const auto& it : path)
	{
		cout << it.parent_index << endl;
	}
	*/
	return 0;
}
