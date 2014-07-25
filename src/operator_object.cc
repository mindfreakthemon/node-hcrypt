#include "operator_object.h"

using namespace v8;
using namespace std;

Persistent<Function> Operator::constructor;

Operator::Operator(Handle<Object> o) {
	fhe_pk_init(pk);

	Local<Value> p = o->Get(String::New("p"));
//	Local<Value> alpha = o->Get(String::New("alpha"));

	if (!p->IsString()/* || !alpha->IsString()*/) {
		ThrowException(Exception::TypeError(String::New("Wrong argument")));
        return;
	}

	String::Utf8Value _p(p->ToString());
//	String::Utf8Value _alpha(alpha->ToString());
//
//	Local<Value> B = o->Get(String::New("B"));
//	Local<Value> c = o->Get(String::New("c"));
//
//	if (!B->IsArray() || !c->IsArray()) {
//        ThrowException(Exception::TypeError(String::New("Wrong argument")));
//        return;
//    }
//
//    Local<Array> _B = Local<Array>::Cast(B);
//    Local<Array> _c = Local<Array>::Cast(c);
//
//    if (_B->Length() != S1 || _c->Length() != S1) {
//        ThrowException(Exception::TypeError(String::New("Wrong argument")));
//        return;
//    }

	mpz_set_str(pk->p, *_p, EXPORT_BASE);
//    mpz_set_str(pk->alpha, *_alpha, EXPORT_BASE);
//
//    int i;
//    for (i = 0; i < S1; i++) {
//        String::Utf8Value _Bi(_B->Get(i)->ToString());
//        String::Utf8Value _ci(_c->Get(i)->ToString());
//
//        mpz_set_str(pk->B[i], *_Bi, EXPORT_BASE);
//        mpz_set_str(pk->c[i], *_ci, EXPORT_BASE);
//    }
}

Operator::~Operator() {
	fhe_pk_clear(pk);
}

void Operator::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);

	tpl->SetClassName(String::NewSymbol("Operator"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("add"),
		FunctionTemplate::New(Add)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("mul"),
        FunctionTemplate::New(Mul)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("fullAdd"),
        FunctionTemplate::New(FullAdd)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("halfAdd"),
        FunctionTemplate::New(HalfAdd)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Operator"), constructor);
}

Handle<Value> Operator::New(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
	}

	if (!args[0]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Argument must be a plain object")));
        return scope.Close(Undefined());
    }

	if (args.IsConstructCall()) {
        // Invoked as constructor: `new Operator(...)`
        Operator* obj = new Operator(args[0]->ToObject());
        obj->Wrap(args.This());
        return args.This();
    } else {
        // Invoked as plain function `Operator(...)`, turn into construct call.
        Local<Value> array = args.Data();
        return scope.Close(constructor->NewInstance(args.Length(), &array));
    }
}

Handle<Value> Operator::Add(const Arguments& args) {
	HandleScope scope;

    if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    String::Utf8Value a(args[0]->ToString());
    String::Utf8Value b(args[1]->ToString());

	mpz_t _a, _b, _res;
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_res);

    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

    Operator* obj = ObjectWrap::Unwrap<Operator>(args.This());

	fhe_add(_res, _a, _b, obj->pk);

	char* s = mpz_get_str(NULL, EXPORT_BASE, _res);

	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_res);

	return scope.Close(String::New(s));
}

Handle<Value> Operator::Mul(const Arguments& args) {
	HandleScope scope;

    if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    String::Utf8Value a(args[0]->ToString());
    String::Utf8Value b(args[1]->ToString());

	mpz_t _a, _b, _res;
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_res);

    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

    Operator* obj = ObjectWrap::Unwrap<Operator>(args.This());

	fhe_mul(_res, _a, _b, obj->pk);

	char* s = mpz_get_str(NULL, EXPORT_BASE, _res);

	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_res);

	return scope.Close(String::New(s));
}

Handle<Value> Operator::FullAdd(const Arguments& args) {
	HandleScope scope;

    if (args.Length() < 3) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString() ||
        !args[2]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    String::Utf8Value c_in(args[0]->ToString());
    String::Utf8Value a(args[1]->ToString());
    String::Utf8Value b(args[2]->ToString());

	mpz_t _a, _b, _c_in, _c_out, _res;
    mpz_init(_c_in);
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_c_out);
    mpz_init(_res);

    mpz_set_str(_c_in, *c_in, EXPORT_BASE);
    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

    Operator* obj = ObjectWrap::Unwrap<Operator>(args.This());

	fhe_fulladd(_res, _c_out, _a, _b, _c_in, obj->pk);

	char* s1 = mpz_get_str(NULL, EXPORT_BASE, _res);
	char* s2 = mpz_get_str(NULL, EXPORT_BASE, _c_out);

	mpz_clear(_c_in);
	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_c_out);
	mpz_clear(_res);

	Local<Array> array = Array::New(2);

	array->Set(0, String::New(s1));
	array->Set(1, String::New(s2));

	return scope.Close(array);
}


Handle<Value> Operator::HalfAdd(const Arguments& args) {
	HandleScope scope;

    if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    String::Utf8Value a(args[0]->ToString());
    String::Utf8Value b(args[1]->ToString());

	mpz_t _a, _b, _c_out, _res;
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_c_out);
    mpz_init(_res);

    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

    Operator* obj = ObjectWrap::Unwrap<Operator>(args.This());

	fhe_halfadd(_res, _c_out, _a, _b, obj->pk);

	char* s1 = mpz_get_str(NULL, EXPORT_BASE, _res);
	char* s2 = mpz_get_str(NULL, EXPORT_BASE, _c_out);

	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_c_out);
	mpz_clear(_res);

	Local<Array> array = Array::New(2);

	array->Set(0, String::New(s1));
	array->Set(1, String::New(s2));

	return scope.Close(array);
}
