/*
*   Async addon syste provided by paulhauner https://github.com/paulhauner
*   License: MIT
*   Tested in node.js v4.4.2 LTS in Ubuntu Linux
*/
#include <node.h>
#include <uv.h>
#include <iostream>
#include <unistd.h>
#include <string>

#include "XinputWatcher.cpp"


using namespace std;

namespace XinputAddon {
  
  using v8::Function;
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Persistent;

  /**
  * Work structure is be used to pass the callback function and data 
  * from the initiating function to the function which triggers the callback.
  */
  struct Work {
    uv_work_t  request;
    Persistent<Function> callback;
    string result;
    XinputWatcher watcher;
  };
  
  /**
  * WorkAsync function is the "middle" function which does the work.
  * After the WorkAsync function is called, the WorkAsyncComplete function
  * is called.
  */
  static void WorkAsync(uv_work_t *req) {
    Work *work = static_cast<Work *>(req->data);
    XinputWatcher watcher = work->watcher;
    work->result = watcher.print_events();
  }
  
  /**
  * WorkAsyncComplete function is called once we are ready to trigger the callback
  * function in JS.
  */
  static void WorkAsyncComplete(uv_work_t *req,int status)
  {
    Isolate * isolate = Isolate::GetCurrent();

    v8::HandleScope handleScope(isolate);

    Work *work = static_cast<Work *>(req->data);
    
    const char *result = work->result.c_str();
    Local<Value> argv[1] = { String::NewFromUtf8(isolate, result) };
    
    // https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);
  }
  
  /**
  * DoTaskAsync is the initial function called from JS. This function returns
  * immediately, however starts a uv task which later calls the callback function
  */
  void DoTaskAsync(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate(); 
    
    // Check the number of arguments passed.
    if (args.Length() < 2) {
      // Throw an Error that is passed back to JavaScript
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Wrong number of arguments")));
      return;
    }

    // Check the argument types
    if (!args[0]->IsFunction() || !args[1]->IsString()) {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Wrong arguments")));
      return;
    }
    
    Work * work = new Work();
    work->request.data = work;

    v8::String::Utf8Value param1(args[1]->ToString());
    std::string inputId = std::string(*param1);

    work->watcher.watch(inputId);

    
    // args[0] is where we pick the callback function out of the JS function params.
    // Because we chose args[0], we must supply the callback fn as the first parameter
    Local<Function> callback = Local<Function>::Cast(args[0]);
    work->callback.Reset(isolate, callback);
    
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);
    
    args.GetReturnValue().Set(Undefined(isolate));  
  }
  
  
  /**
  * init function declares what we will make visible to node
  */
  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "watch", DoTaskAsync);
  }

  NODE_MODULE(XinputAddon, init)

}  
