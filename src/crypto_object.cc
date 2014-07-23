#include "crypto_object.h"

using namespace v8;
using namespace std;

Persistent<Function> Crypto::constructor;

Crypto::Crypto() {
	fhe_pk_init(pk);
    fhe_sk_init(sk);
	fhe_keygen(pk, sk);
}

Crypto::~Crypto() {
	fhe_pk_clear(pk);
	fhe_sk_clear(sk);
}

void Crypto::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);

	tpl->SetClassName(String::NewSymbol("Crypto"));
	tpl->InstanceTemplate()->SetInternalFieldCount(2);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("encrypt"),
		FunctionTemplate::New(Encrypt)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("decrypt"),
		FunctionTemplate::New(Decrypt)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("export"),
		FunctionTemplate::New(Export)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Crypto"), constructor);
}

Handle<Value> Crypto::New(const Arguments& args) {
	HandleScope scope;

	if (args.IsConstructCall()) {
		// Invoked as constructor: `new Crypto(...)`
		Crypto* obj = new Crypto();
		obj->Wrap(args.This());
		return args.This();
	} else {
		// Invoked as plain function `Crypto(...)`, turn into construct call.
		Local<Value> array = args.Data();
		return scope.Close(constructor->NewInstance(args.Length(), &array));
	}
}

Handle<Value> Crypto::Export(const Arguments& args) {
	HandleScope scope;

	char* s;
	int i;

	Crypto* obj = ObjectWrap::Unwrap<Crypto>(args.This());

	Local<Object> result = Object::New();

	Local<Array> B = Array::New(S1);
	Local<Array> c = Array::New(S1);

	s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->p);
	result->Set(String::New("p"), String::New(s));

	s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->alpha);
    result->Set(String::New("alpha"), String::New(s));

    for (i = 0; i < S1; i++) {
        s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->B[i]);
		B->Set(i, String::New(s));

		s = mpz_get_str(NULL, EXPORT_BASE, obj->pk->c[i]);
		c->Set(i, String::New(s));
    }

	result->Set(String::New("B"), B);
	result->Set(String::New("c"), c);

	return scope.Close(result);
}

Handle<Value> Crypto::Decrypt(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}

	if (!args[0]->IsString()) {
		ThrowException(Exception::TypeError(String::New("Argument must be a string")));
		return scope.Close(Undefined());
	}

	String::Utf8Value str(args[0]->ToString());

	Crypto* obj = ObjectWrap::Unwrap<Crypto>(args.This());

	mpz_t c;
	mpz_init(c);

	mpz_set_str(c, *str, EXPORT_BASE);

	int m = fhe_decrypt(c, obj->sk);

	mpz_clear(c);

	return scope.Close(Number::New(m));
}

Handle<Value> Crypto::Encrypt(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("Argument must be number")));
    return scope.Close(Undefined());
  }

  int m = args[0]->NumberValue();

  if ((m != 0) && (m != 1)) {
    ThrowException(Exception::TypeError(String::New("Argument must be either 0 or 1")));
    return scope.Close(Undefined());
  }

  Crypto* obj = ObjectWrap::Unwrap<Crypto>(args.This());

  mpz_t c;
  mpz_init(c);

  fhe_encrypt(c, obj->pk, m);

  char* s = mpz_get_str(NULL, EXPORT_BASE, c);

  mpz_clear(c);

  return scope.Close(String::New(s));
}
