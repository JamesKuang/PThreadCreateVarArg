PThreadCreateVarArg
===================

A light wrapper around the C pthread_create function to support variable number of arguments.

User calls thread_var_arg directly. Sample usage:

include "PTWrapper.hpp"
#include <iostream>

using std::cout;
using std::endl;

struct A {
	void method1(int a, double b, int *c) {
		cout << "method1: " << a << ", " << b << ", " << *c << endl;
	}

	void method2(const char *c, std::string s) {
		cout << "method2: " << c << ", " << s << endl;
	}

	void method3() {
		cout << "method3: no arguments" << endl;
	}
};

int main() {
	A a;

	int i = 3;
	pthread_t tid = cs540::NewThread(&a, &A::method1, 1337, 3.14, &i);
	pthread_join(tid, nullptr);

	const char *c = "this is a char";
	pthread_t tid2 = cs540::NewThread(&a, &A::method2, c, std::string("hello string"));
	pthread_join(tid2, nullptr);

	pthread_t tid3 = cs540::NewThread(&a, &A::method3);
	pthread_join(tid3, nullptr);
}

