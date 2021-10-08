#pragma once
#ifndef __COLOR_H_
#define __COLOR_H_
#include <algorithm>
#include "../optype.h"
#define WORD_BKCOLOR 0
#define WORD_COLOR 1
//#include "../Tool.h"
#include <math.h>
#define color2uint(color) (*(uint*)&color)
template<typename T>
constexpr T OP_ABS(T x) {
	return x < 0 ? -x : x;
}
template<typename T>
constexpr bool IN_RANGE(T lhs, T rhs, T df) { 
	return OP_ABS(lhs.b-rhs.b)<=df.b
		&&OP_ABS(lhs.g-rhs.g)<=df.g
		&&OP_ABS(lhs.r-rhs.r)<=df.r;
}
//��ɫ�ṹ

//#pragma pack(push)
#pragma pack(1)
struct color_t {
	//b is in low address ,alpha is in high address
	uchar b, g, r, alpha;
	color_t() :b(0), g(0), r(0), alpha(0) {}
	color_t(int b_, int g_, int r_) :b(b_), g(g_), r(r_),alpha(0xffu) {}
	//color_t(uint c) :b((c >> 24) & 0xff), g((c >> 16) & 0xff), r((c >> 8) & 0xff) {}
	//absolute val
	/*color_t operator-(const color_t& rhs) {
		color_t c;
		c.b = b - rhs.b;
		c.g = g - rhs.g;
		c.r = r - rhs.r;
		return c;
	}*/
	//bool operator<=(const color_t& rhs)const {
	//	return b <= rhs.b&&g <= rhs.g&&r <= rhs.r;
	//}
	//bool operator>(const color_t& rhs)const {
	//	return b > rhs.b || g > rhs.g || r > rhs.r;
	//}
	/*bool operator==(const color_t& rhs)const {
		return b == rhs.b&&g == rhs.g&&r == rhs.r;
	}*/
	color_t& str2color(const std::wstring& s) {
		int r = 0, g = 0, b = 0;
		std::wstring ss = s; 
		std::transform(ss.begin(), ss.end(), ss.begin(), ::towupper);
		int cnt = swscanf(ss.c_str(), L"%02X%02X%02X", &r, &g, &b);
		this->b = b; this->r = r; this->g = g;
		return *this;
	}
	color_t& str2color(const std::string&s) {
		int r, g, b;
		std::string ss = s;
		std::transform(ss.begin(), ss.end(), ss.begin(), ::toupper);
		int cnt = sscanf(ss.c_str(), "%02X%02X%02X", &r, &g, &b);
		this->b = b; this->r = r; this->g = g;
		return *this;
	}
	std::string tostr() {
		char buff[10];
		sprintf(buff, "%02X%02X%02X", r, g, b);
		return buff;
	}
	std::wstring towstr() {
		wchar_t buff[10];
		wsprintf(buff, L"%02X%02X%02X", r, g, b);
		return buff;
	}
};
#pragma pack()
//��ɫ-ƫɫ�ṹ
struct color_df_t {
	//��ɫ
	color_t color;
	//ƫɫ
	color_t df;
};
//����-��ɫ-ƫɫ�ṹ
struct pt_cr_df_t {
	int x, y;
	std::vector<color_df_t> crdfs;
};

#endif
