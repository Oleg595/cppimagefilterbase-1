#include"Config.h"
#include<fstream>

Config::Config(string conf) {
	ifstream file(conf);
	string str;
	filters = new vector<Filter>;
	while (getline(file, str)) {
		string* s = new string[5];
		int j = 0;
		for (int i = 0; i < 5; i++) {
			s[i] = Read(str, &j);
		}
		Filter filter;
		filter.f_n = Name(s[0]);
		filter.u = atoi(s[1].c_str());
		filter.l = atoi(s[2].c_str());
		filter.b = atoi(s[3].c_str());
		filter.r = atoi(s[4].c_str());
		//filters->insert(filters->end(), filters->capacity(), filter);
		filters->push_back(filter);
	}
}

string Config::Read(string str, int* i) {
	int j = *i;
	string s;
	char ch = str[j];
	while (ch != ' ' && ch != '\0') {
		s = s + ch;
		j++;
		ch = str[j];
	}
	j++;
	*i = j;
	return s;
}

enum Fil_Name Config::Name(string str) {
	if (str == "BLACKWHITE") {
		return BLACK_WHITE;
	}
	if (str == "Red") {
		return RED;
	}
	if (str == "Threshold") {
		return THRESHOLD;
	}
	if (str == "Edge") {
		return EDGE;
	}
	if (str == "Blur") {
		return BLUR;
	}
	return ERROR;
}

vector<Filter>* Config::Fil() {
	return filters;
}
