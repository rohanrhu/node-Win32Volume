/*
* https://github.com/rohanrhu/node-Win32Volume
*
* Win32Volume is a node.js module for setting volume level in Windows platform
*
* Copyright (C) 2017 Oğuzhan Eroğlu <rohanrhu2@gmail.com>
* Licensed under The MIT License (MIT)
*
*/

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include "node.h"
#include "uv.h"
#include <objbase.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "Winmm.lib")

struct AsyncWorker {
	uv_work_t request;
	v8::Persistent<v8::Function> callback;
	bool is_callback;
	bool is_mute;
	double volume_level;
	HRESULT result;
};

IAudioEndpointVolume* OpenAudioDevice() {
	HRESULT hr;

	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;

	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(LPVOID *)&deviceEnumerator
	);

	if (!SUCCEEDED(hr)) return NULL;

	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();

	if (!SUCCEEDED(hr)) return NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();

	if (!SUCCEEDED(hr)) return NULL;

	return endpointVolume;
}

void SetVolume(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	double volume_level = v8::Local<v8::Integer>::Cast(args[0])->NumberValue();

	v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[1]);
	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc];

	IAudioEndpointVolume* dev = OpenAudioDevice();

	HRESULT result;
	
	result = dev->SetMasterVolumeLevelScalar(volume_level, NULL);
	dev->Release();

	args.GetReturnValue().Set(v8::Boolean::New(isolate, (result == S_OK) ? TRUE : FALSE));
}

void SetVolume__AsyncWorker(uv_work_t* request) {
	AsyncWorker* data = static_cast<AsyncWorker *>(request->data);

	IAudioEndpointVolume* dev = OpenAudioDevice();

	data->result = dev->SetMasterVolumeLevelScalar(data->volume_level, NULL);
	dev->Release();
}

void SetVolume__AsyncWorkerCb(uv_work_t* request, int status) {
	AsyncWorker* data = static_cast<AsyncWorker *>(request->data);

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc];

	argv[0] = v8::Boolean::New(
		isolate,
		(data->result == S_OK) ? TRUE : FALSE
	);

	if (data->is_callback)
	v8::Local<v8::Function>::New(isolate, data->callback)
		->Call(
			isolate->GetCurrentContext()->Global(),
			argc,
			argv
		);
}

void SetVolume__Async(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	double volume_level = v8::Local<v8::Boolean>::Cast(args[0])->NumberValue();

	bool is_callback;
	v8::Local<v8::Function> cb;

	if (is_callback = !args[1]->IsUndefined()) {
		cb = v8::Local<v8::Function>::Cast(args[1]);
	}

	AsyncWorker* worker = new AsyncWorker;
	worker->request.data = worker;
	worker->is_callback = is_callback;
	if (is_callback) worker->callback.Reset(isolate, cb);
	worker->volume_level = volume_level;

	uv_queue_work(
		uv_default_loop(),
		&worker->request,
		SetVolume__AsyncWorker,
		SetVolume__AsyncWorkerCb
	);
}

void SetMute(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	bool is_mute = v8::Local<v8::Boolean>::Cast(args[0])->NumberValue();

	v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[1]);
	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc];

	IAudioEndpointVolume* dev = OpenAudioDevice();

	HRESULT result;

	result = dev->SetMute(is_mute, NULL);
	dev->Release();

	args.GetReturnValue().Set(v8::Boolean::New(isolate, (result == S_OK) ? TRUE : FALSE));
}

void SetMute__AsyncWorker(uv_work_t* request) {
	AsyncWorker* data = static_cast<AsyncWorker *>(request->data);

	IAudioEndpointVolume* dev = OpenAudioDevice();

	data->result = dev->SetMute(data->is_mute, NULL);
	dev->Release();
}

void SetMute__AsyncWorkerCb(uv_work_t* request, int status) {
	AsyncWorker* data = static_cast<AsyncWorker *>(request->data);

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc];

	argv[0] = v8::Boolean::New(
		isolate,
		(data->result == S_OK) ? TRUE : FALSE
	);

	if (data->is_callback)
	v8::Local<v8::Function>::New(isolate, data->callback)
		->Call(
			isolate->GetCurrentContext()->Global(),
			argc,
			argv
		);
}

void SetMute__Async(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	bool is_mute = v8::Local<v8::Boolean>::Cast(args[0])->NumberValue();

	bool is_callback;
	v8::Local<v8::Function> cb;

	if (is_callback = !args[1]->IsUndefined()) {
		cb = v8::Local<v8::Function>::Cast(args[1]);
	}
	
	AsyncWorker* worker = new AsyncWorker;
	worker->request.data = worker;
	worker->is_callback = is_callback;
	if (is_callback) worker->callback.Reset(isolate, cb);
	worker->is_mute = is_mute;
	
	uv_queue_work(
		uv_default_loop(),
		&worker->request,
		SetMute__AsyncWorker,
		SetMute__AsyncWorkerCb
	);
}

void Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
	NODE_SET_METHOD(exports, "setVolume", SetVolume__Async);
	NODE_SET_METHOD(exports, "setVolumeSync", SetVolume);
	NODE_SET_METHOD(exports, "setMute", SetMute__Async);
	NODE_SET_METHOD(exports, "setMuteSync", SetMute);
}

NODE_MODULE(win32_volume, Init)