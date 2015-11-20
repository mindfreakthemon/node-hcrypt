#include "crypto_object.h"

using namespace std;

Nan::Persistent<v8::Function> Crypto::constructor;

NAN_MODULE_INIT(Crypto::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Crypto").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  Nan::SetPrototypeMethod(tpl, "encrypt", Encrypt);
  Nan::SetPrototypeMethod(tpl, "decrypt", Decrypt);
  Nan::SetPrototypeMethod(tpl, "recrypt", Recrypt);
  Nan::SetPrototypeMethod(tpl, "export", Export);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Crypto").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Crypto::Crypto() {
	fhe_pk_init(pk);
    fhe_sk_init(sk);
	fhe_keygen(pk, sk);
}

Crypto::~Crypto() {
	fhe_pk_clear(pk);
	fhe_sk_clear(sk);
}

NAN_METHOD(Crypto::New) {
    if (info.IsConstructCall()) {
        Crypto *obj = new Crypto();
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        v8::Local<v8::Function> cons = Nan::New(constructor);
        info.GetReturnValue().Set(cons->NewInstance());
    }
}

NAN_METHOD(Crypto::Export) {
	char* s;
	int i;

	Crypto* obj = Nan::ObjectWrap::Unwrap<Crypto>(info.This());

    v8::Isolate* isolate = info.GetIsolate();
	v8::Local<v8::Object> result = v8::Object::New(isolate);

	v8::Local<v8::Array> B = v8::Array::New(isolate, S1);
	v8::Local<v8::Array> c = v8::Array::New(isolate, S1);

	s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->p);
	result->Set(Nan::New("p").ToLocalChecked(), Nan::New(s).ToLocalChecked());

	s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->alpha);
    result->Set(Nan::New("alpha").ToLocalChecked(), Nan::New(s).ToLocalChecked());

    for (i = 0; i < S1; i++) {
        s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->B[i]);
		B->Set(i, Nan::New(s).ToLocalChecked());

		s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->c[i]);
		c->Set(i, Nan::New(s).ToLocalChecked());
    }

	result->Set(Nan::New("B").ToLocalChecked(), B);
	result->Set(Nan::New("c").ToLocalChecked(), c);

	info.GetReturnValue().Set(result);
}

NAN_METHOD(Crypto::Decrypt) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError(Nan::New("Argument must be a string").ToLocalChecked());
		return;
	}

	v8::String::Utf8Value str(info[0]->ToString());

	Crypto* obj = Nan::ObjectWrap::Unwrap<Crypto>(info.This());

	mpz_t c;
	mpz_init(c);

	mpz_set_str(c, *str, EXPORT_BASE);

	int m = fhe_decrypt(c, obj->sk);

	mpz_clear(c);

    info.GetReturnValue().Set(m);
}

NAN_METHOD(Crypto::Recrypt) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError(Nan::New("Argument must be a string").ToLocalChecked());
		return;
	}

	v8::String::Utf8Value str(info[0]->ToString());

	Crypto* obj = Nan::ObjectWrap::Unwrap<Crypto>(info.This());

	mpz_t c;
	mpz_init(c);

	mpz_set_str(c, *str, EXPORT_BASE);

	fhe_recrypt(c, obj->pk);

	char* s = mpz_get_str(NULL, EXPORT_BASE, c);

	mpz_clear(c);

    info.GetReturnValue().Set(Nan::New(s).ToLocalChecked());
}

NAN_METHOD(Crypto::Encrypt) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError(Nan::New("Wrong number of arguments").ToLocalChecked());
		return;
	}

	if (!info[0]->IsNumber()) {
		Nan::ThrowTypeError(Nan::New("Argument must be number").ToLocalChecked());
		return;
	}

	int m = info[0]->NumberValue();

	if ((m != 0) && (m != 1)) {
		Nan::ThrowTypeError(Nan::New("Argument must be either 0 or 1").ToLocalChecked());
		return;
	}

	Crypto* obj = Nan::ObjectWrap::Unwrap<Crypto>(info.This());

	mpz_t c;
	mpz_init(c);

	fhe_encrypt(c, obj->pk, m);

//	// HACK
//	mpz_t c0;
//	mpz_init(c0);
//	fhe_encrypt(c0, obj->pk, 0);
//
//	srand(time(NULL));
//	int i, r = rand() % 2;
//
//	for (i = 0; i < r; i++) {
//		fhe_add(c, c, c0, obj->pk);
//		fhe_recrypt(c, obj->pk);
//	}

	char* s = mpz_get_str(NULL, EXPORT_BASE, c);

	mpz_clear(c);

    info.GetReturnValue().Set(Nan::New(s).ToLocalChecked());
}
