#ifndef __hello_c_

#include <stdio.h>
#include <string.h>
#include "include/dart_api.h"
#include "include/dart_native_api.h"

extern void main_init();
extern void main_loop();
extern int isQuit();

Dart_Handle HandleError(Dart_Handle handle) {
	if (Dart_IsError(handle)) Dart_PropagateError(handle);
	return handle;
}

Dart_NativeFunction ResolveName(
	Dart_Handle name,
	int argc,
	bool* auto_setup_scope);

DART_EXPORT Dart_Handle hello_Init(
	Dart_Handle parent_library
) {
	printf("called hello_Init %p", parent_library);
	Dart_Handle result_code = Dart_SetNativeResolver(
		parent_library, ResolveName, NULL
	);
	if (Dart_IsError(result_code)) {
		return result_code;
	}
	return Dart_Null();
}


void lib_enterFrame(Dart_NativeArguments arguments) {
	//printf("call enterFrame\r\n");
	Dart_EnterScope();
	main_loop();
	Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(1)));
	Dart_ExitScope();
}

void lib_init(Dart_NativeArguments arguments) {
	printf("call init\r\n");
	Dart_EnterScope();
	main_init();
	Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(1)));
	Dart_ExitScope();
}

void lib_isQuit(Dart_NativeArguments arguments) {
	printf("call isQuit\r\n");
	Dart_EnterScope();
	int v = isQuit();
	Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(v)));
	Dart_ExitScope();
}

Dart_NativeFunction ResolveName(
	Dart_Handle name,
	int argc,
	bool* auto_setup_scope) {
	printf("call ResolveName\r\n");
	Dart_NativeFunction result = NULL;
	const char *cname;
	Dart_EnterScope();
	Dart_StringToCString(name, &cname);
	//
	printf("called capp_enterFrame %s\r\n", cname);
	if (strcmp("capp_enterFrame", cname) == 0) {
		*auto_setup_scope = false;
		result = lib_enterFrame;
	}
	else if (strcmp("capp_init", cname) == 0) {
		printf("call ResolveName ed\r\n");
		*auto_setup_scope = false;
		result = lib_init;
	}
	else if (strcmp("capp_isQuit", cname) == 0) {
		*auto_setup_scope = false;
		result = lib_isQuit;
	}

	Dart_ExitScope();
	return result;
}
#endif
