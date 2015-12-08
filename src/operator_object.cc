#include "operator_object.h"

using namespace std;

Nan::Persistent<v8::Function> Operator::constructor;

NAN_MODULE_INIT(Operator::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Operator").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  Nan::SetPrototypeMethod(tpl, "add", Add);
  Nan::SetPrototypeMethod(tpl, "mul", Mul);
  Nan::SetPrototypeMethod(tpl, "fullAdd", FullAdd);
  Nan::SetPrototypeMethod(tpl, "halfAdd", HalfAdd);
  Nan::SetPrototypeMethod(tpl, "recrypt", Recrypt);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Operator").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Operator::Operator(v8::Local<v8::Object> o) {
	fhe_pk_init(pk);

	v8::Local<v8::Value> p = o->Get(Nan::New("p").ToLocalChecked());
	v8::Local<v8::Value> alpha = o->Get(Nan::New("alpha").ToLocalChecked());

	if (!p->IsString() || !alpha->IsString()) {
		Nan::ThrowTypeError(Nan::New("Wrong argument").ToLocalChecked());
        return;
	}

	v8::String::Utf8Value _p(p->ToString());
	v8::String::Utf8Value _alpha(alpha->ToString());

	v8::Local<v8::Value> B = o->Get(Nan::New("B").ToLocalChecked());
	v8::Local<v8::Value> c = o->Get(Nan::New("c").ToLocalChecked());

	if (!B->IsArray() || !c->IsArray()) {
        Nan::ThrowTypeError(Nan::New("Wrong argument").ToLocalChecked());
        return;
    }

    v8::Local<v8::Array> _B = v8::Local<v8::Array>::Cast(B);
    v8::Local<v8::Array> _c = v8::Local<v8::Array>::Cast(c);

    if (_B->Length() != S1 || _c->Length() != S1) {
        Nan::ThrowTypeError(Nan::New("Wrong argument").ToLocalChecked());
        return;
    }

	mpz_set_str(pk->p, *_p, EXPORT_BASE);
    mpz_set_str(pk->alpha, *_alpha, EXPORT_BASE);

    int i;
    for (i = 0; i < S1; i++) {
        v8::String::Utf8Value _Bi(_B->Get(i)->ToString());
        v8::String::Utf8Value _ci(_c->Get(i)->ToString());

        mpz_set_str(pk->B[i], *_Bi, EXPORT_BASE);
        mpz_set_str(pk->c[i], *_ci, EXPORT_BASE);
    }
}

Operator::~Operator() {
	fhe_pk_clear(pk);
}

NAN_METHOD(Operator::New) {
    if (info.Length() < 1) {
		Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
        return;
	}

	if (!info[0]->IsObject()) {
        Nan::ThrowTypeError(Nan::New("Argument must be a plain object").ToLocalChecked());
        return;
    }

    if (info.IsConstructCall()) {
        Operator *obj = new Operator(info[0]->ToObject());
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        v8::Local<v8::Value> array = info.Data();
        v8::Local<v8::Function> cons = Nan::New(constructor);
        info.GetReturnValue().Set(cons->NewInstance(info.Length(), &array));
    }
}

NAN_METHOD(Operator::Add) {
    if (info.Length() < 2) {
        Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
        return;
    }

    if (!info[0]->IsString() || !info[1]->IsString()) {
        Nan::ThrowTypeError(Nan::New("Wrong arguments").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value a(info[0]->ToString());
    v8::String::Utf8Value b(info[1]->ToString());

	mpz_t _a, _b, _res;
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_res);

    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

	Operator* obj = Nan::ObjectWrap::Unwrap<Operator>(info.This());

	fhe_add(_res, _a, _b, obj->pk);

	char* s = mpz_get_str(NULL, EXPORT_BASE, _res);

	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_res);

	info.GetReturnValue().Set(Nan::New(s).ToLocalChecked());
}

NAN_METHOD(Operator::Mul) {
    if (info.Length() < 2) {
        Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
        return;
    }

    if (!info[0]->IsString() || !info[1]->IsString()) {
        Nan::ThrowTypeError(Nan::New("Wrong arguments").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value a(info[0]->ToString());
    v8::String::Utf8Value b(info[1]->ToString());

	mpz_t _a, _b, _res;
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_res);

    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

	Operator* obj = Nan::ObjectWrap::Unwrap<Operator>(info.This());

	fhe_mul(_res, _a, _b, obj->pk);

	char* s = mpz_get_str(NULL, EXPORT_BASE, _res);

	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_res);

	info.GetReturnValue().Set(Nan::New(s).ToLocalChecked());
}

NAN_METHOD(Operator::FullAdd) {
    if (info.Length() < 3) {
        Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
        return;
    }

    if (!info[0]->IsString() || !info[1]->IsString() ||
        !info[2]->IsString()) {
        Nan::ThrowTypeError(Nan::New("Wrong arguments").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value c_in(info[0]->ToString());
    v8::String::Utf8Value a(info[1]->ToString());
    v8::String::Utf8Value b(info[2]->ToString());

	mpz_t _a, _b, _c_in, _c_out, _res;
    mpz_init(_c_in);
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_c_out);
    mpz_init(_res);

    mpz_set_str(_c_in, *c_in, EXPORT_BASE);
    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

	Operator* obj = Nan::ObjectWrap::Unwrap<Operator>(info.This());

	fhe_fulladd(_res, _c_out, _a, _b, _c_in, obj->pk);

	char* s1 = mpz_get_str(NULL, EXPORT_BASE, _res);
	char* s2 = mpz_get_str(NULL, EXPORT_BASE, _c_out);

	mpz_clear(_c_in);
	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_c_out);
	mpz_clear(_res);

    v8::Isolate* isolate = info.GetIsolate();
	v8::Local<v8::Array> array = v8::Array::New(isolate, 2);

	array->Set(0, Nan::New(s1).ToLocalChecked());
	array->Set(1, Nan::New(s2).ToLocalChecked());

	info.GetReturnValue().Set(array);
}


NAN_METHOD(Operator::HalfAdd) {
    if (info.Length() < 2) {
        Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
        return;
    }

    if (!info[0]->IsString() || !info[1]->IsString()) {
        Nan::ThrowTypeError(Nan::New("Wrong arguments").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value a(info[0]->ToString());
    v8::String::Utf8Value b(info[1]->ToString());

	mpz_t _a, _b, _c_out, _res;
    mpz_init(_a);
    mpz_init(_b);
    mpz_init(_c_out);
    mpz_init(_res);

    mpz_set_str(_a, *a, EXPORT_BASE);
    mpz_set_str(_b, *b, EXPORT_BASE);

	Operator* obj = Nan::ObjectWrap::Unwrap<Operator>(info.This());

	fhe_halfadd(_res, _c_out, _a, _b, obj->pk);

	char* s1 = mpz_get_str(NULL, EXPORT_BASE, _res);
	char* s2 = mpz_get_str(NULL, EXPORT_BASE, _c_out);

	mpz_clear(_a);
	mpz_clear(_b);
	mpz_clear(_c_out);
	mpz_clear(_res);

    v8::Isolate* isolate = info.GetIsolate();
	v8::Local<v8::Array> array = v8::Array::New(isolate, 2);

	array->Set(0, Nan::New(s1).ToLocalChecked());
    array->Set(1, Nan::New(s2).ToLocalChecked());

	info.GetReturnValue().Set(array);
}

NAN_METHOD(Operator::Recrypt) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError(Nan::New("Argument must be a string").ToLocalChecked());
		return;
	}

	v8::String::Utf8Value str(info[0]->ToString());

	Operator* obj = Nan::ObjectWrap::Unwrap<Operator>(info.This());

	mpz_t c;
	mpz_init(c);

	mpz_set_str(c, *str, EXPORT_BASE);

	fhe_recrypt(c, obj->pk);

	char* s = mpz_get_str(NULL, EXPORT_BASE, c);

	mpz_clear(c);

    info.GetReturnValue().Set(Nan::New(s).ToLocalChecked());
}