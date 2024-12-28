//
// Created by Admin on 28/12/2024.
//


#include <MyDP/Reflection/Reflection.h>
#include <MyDP/Visitor/MultiVisitor.h>

#include "nameof.hpp"

#include <string>
#include <iostream>

using namespace std;
using namespace My;

struct Figure { virtual ~Figure() = default; };
struct Sphere : Figure { int radius{ 0 }; };
struct Square : Figure { float sideLength{ 0.f }; };

struct Cosmetics { virtual ~Cosmetics() = default; };
struct Lipstick : Cosmetics { string name{ "mac" }; };
struct Lipglaze : Cosmetics { float color{ 0.f }; };

template<typename Obj>
class MemVarSerialize : public RawPtrVisitor<MemVarSerialize<Obj>, MemVar<void* Obj::*>> {
public:
	using Base = RawPtrVisitor<MemVarSerialize<Obj>, MemVar<void* Obj::*>>;

	MemVarSerialize() : obj{ nullptr } {
		Base::template Regist<
			MemVar<float Obj::*>,
			MemVar<int Obj::*>,
			MemVar<string Obj::*>>();
	}

	template<typename T>
	void ImplVisit(MemVar<T Obj::*>* memvar) {
		cout << memvar->Of(*obj);
	}

	void ImplVisit(MemVar<string Obj::*>* memvar) {
		cout << "\"" << memvar->Of(*obj) << "\"";
	}

	Obj* obj;
};

struct Serialize : RawPtrMultiVisitor<Serialize, Figure, Cosmetics> {
	Serialize() {
		Regist<Sphere, Square, Lipstick, Lipglaze>();
	}

	template<typename T>
	void ImplVisit(T* e) {
		static MemVarSerialize<T> ms;
		ms.obj = e;
		cout << "{" << endl;
		cout << "\"type\": \"" << Reflection<T>::Instance().GetName() << "\"" << endl;
		for (auto nv : Reflection<T>::Instance().Vars()) {
			cout << nv.first << ": ";
			ms.Visit(nv.second);
			cout << endl;
		}
		cout << "}" << endl;
	}
};

int main() {
	Reflection<Sphere>::Instance()
		.SetName(nameof::nameof_type<Sphere>().data())
		.Regist(&Sphere::radius, NAMEOF(&Sphere::radius).data());

	Reflection<Square>::Instance()
		.SetName(nameof::nameof_type<Square>().data())
		.Regist(&Square::sideLength, NAMEOF(&Square::sideLength).data());

	Reflection<Lipstick>::Instance()
		.SetName(nameof::nameof_type<Lipstick>().data())
		.Regist(&Lipstick::name, NAMEOF(&Lipstick::name).data());

	Reflection<Lipglaze>::Instance()
		.SetName(nameof::nameof_type<Lipglaze>().data())
		.Regist(&Lipglaze::color, NAMEOF(&Lipglaze::color).data());

	Serialize serialize;
	Sphere a;
	Square b;
	Figure* figures[2] = { &a,&b };
	Lipstick c;
	Lipglaze d;
	Cosmetics* cosmetics[2] = { &c,&d };

	for (auto f : figures)
		serialize.Visit(f);
	for (auto c : cosmetics)
		serialize.Visit(c);
}
