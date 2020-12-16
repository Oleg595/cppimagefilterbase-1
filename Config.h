#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

enum Fil_Name {
	BLACK_WHITE,
	RED,
	THRESHOLD,
	BLUR,
	EDGE,
	ERROR
};

struct Filter {
	enum Fil_Name f_n;
	int u;
	int l;
	int b;
	int r;
};

class Config {
public:
	Config(string conf);
	vector<Filter>* Fil();
private:
	vector<Filter>* filters;
	string Read(string str, int* i);
	enum Fil_Name Name(string str);
};
