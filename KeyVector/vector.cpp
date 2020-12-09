#include <string>
#include <cassert>
#include "vector.h"

struct MyStruct
{
	int i;
};


int main()
{
	Vector<int> vinit{ 2 };
	vinit.pushBack(100);
	vinit.pushBack(784);
	vinit.pushBack(7);
	vinit.pushBack(8);
	vinit.pushBack(84);
	vinit.pushBack(40);
	vinit.print();

	for (int i = 0; i < 1000; ++i)
	{
		vinit.pushBack(i);
	}

	std::cout << '\n';
	for (const auto &cai : vinit)
	{
		std::cout << cai << ' ';
	}
	std::cout << '\n';
	vinit.popBack();

	Vector<int> v;
	v.reserve(64); // 64 initial capacity
	v.pushBack(5437);
	std::cout << v[0] << '\n';
	std::size_t capacity = v.getCapacity();
	std::cout << "capacity=" << capacity << '\n';
	assert(capacity == 64);
	v.print();

	Vector<std::wstring> v1(2);
	assert(v1.getSize() == 0);
	v1.pushBack(L"hi");
	assert(v1[0] == L"hi");
	assert(v1.getSize() == 1);
	v1.print();

	Vector<int> v2;
	std::cout << "capacity=" << v2.getCapacity() << " size=" << v2.getSize() << '\n';
	v2.pushBack(100);
	assert(v2[0] == 100);
	v2.pushBack(101);
	assert(v2.getSize() == 2);
	assert(v2[1] == 101);
	v2.pushBack(101);
	std::cout << '\n';
	v2.print();
	std::cout << "capacity=" << v2.getCapacity() << " size=" << v2.getSize() << '\n';
	std::cout << '\n';

	Vector<std::wstring> v3(2, L"hello");
	assert(v3.getSize() == 2);
	assert(v3.getCapacity() == 2);
	assert(v3[0] == L"hello");
	assert(v3[1] == L"hello");
	v3.print();
	std::cout << '\n';
	v3.resize(1);
	assert(v3.getSize() == 1);
	assert(v3[0] == L"hello");
	v3.print();
	std::cout << '\n'
		<< '\n';

	Vector<std::wstring> v4 = v3;
	assert(v4.getSize() == 1);
	assert(v4[0] == v3[0]);
	try
	{
		v3.at(0) = L"test";
		v4.at(0) = L"helloflower";
	}
	catch (Exception &ex)
	{
		ex.what();
	}
	assert(v4[0] != v3[0]);
	assert(v3[0] == L"test");
	assert(v4[0] == L"helloflower");
	v3.popBack();
	assert(v3.getSize() == 0);
	v4.print();
	std::cout << '\n';

	Vector<std::wstring> v5;
	v5 = v4;
	v5.print();

	Vector<int> v6(20, 9);
	Vector<int>::iterator it = v6.begin();
	while (it != v6.end())
	{
		assert(*it == 9);
		++it;
	}
	v6.print();
	std::cout << "v7 coming up.." << '\n';
	std::cout << '\n';

	Vector<int> v7{ v6 };
	v7.print();
	std::cout << '\n';

	std::cout << std::boolalpha;
	bool b = v7 == v6;
	std::cout << "Are v7 and v6 equal? " << b << '\n';

	Vector<int> v8{ std::move(v6) };
	v8.print();

	std::cout << '\n';
	std::cout << "v6 must be cleared now.." << '\n';
	v6.print();
	std::cout << "yup\n";
	std::cout << '\n';

	Vector<int> v9;
	v9 = std::move(v8);
	v9.print();

	std::cout << '\n';
	std::cout << "v8 must be cleared now.." << '\n';
	v8.print();
	std::cout << "yup\n";
	std::cout << '\n';

	//std::cout << "v9.begin() = " << v9.begin() << "   v9.end() = " << v9.end() << '\n';
	//std::cout << "distanceIter(v9.begin(),v9.end())=" << v9.distanceIter(v9.begin(), v9.end()) << '\n';

	std::cout << "v10 with pointers - iterators" << '\n';
	Vector<int> v10(v9.begin(), v9.end());
	v10.print();
	std::cout << "yup\n";

	Vector<MyStruct> v11(8, MyStruct{ 354 });
	std::cout << "Size=" << v11.getSize() << '\n';
	std::cout << "Capacity=" << v11.getCapacity() << '\n';
	for (const auto &i : v11)
	{
		std::cout << i.i << '\n';
	}
	std::cout << '\n';

	struct Complex
	{
		float m_re;
		float m_im;
		Complex(float re, float im)
			: m_re(re),
			m_im(im)
		{}
	};

	Complex complexity{ 54.984, 98.356 };
	std::cout << "emplacing" << '\n';
	Vector<Complex> v12;
	v12.emplaceBack(543.87f, 890.341f);
	v12.emplaceBack(complexity.m_re, complexity.m_im);
	v12.emplaceBack(complexity.m_re + 98543.80f, complexity.m_im + 45.0);
	v12.emplaceBack(543.87, 890.341);
	v12.emplaceBack(543.87, 890.341);
	v12.emplaceBack(903.1, 120.02f);
	for (const auto &i : v12)
	{
		std::cout << i.m_re << ' ' << i.m_im << '\n';
	}

	std::system( "pause" );
}