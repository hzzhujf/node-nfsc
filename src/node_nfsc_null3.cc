/*
 * Copyright 2017 Scality
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @authors:
 *    Guillaume Gimenez <ggim@scality.com>
 */
#include "node_nfsc.h"
#include "node_nfsc_null3.h"
#include "node_nfsc_fattr3.h"

// ( callback(err) )
NAN_METHOD(NFS::Client::Null3) {
    bool typeError = true;
    if ( info.Length() != 1) {
        Nan::ThrowTypeError("Must be called with 1 parameters");
        return;
    }
    if (!info[0]->IsFunction())
        Nan::ThrowTypeError("Parameter 1, callback must be a function");
    else
        typeError = false;
    if (typeError)
        return;
    NFS::Client* obj = ObjectWrap::Unwrap<NFS::Client>(info.Holder());
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new NFS::Null3Worker(obj, callback));
}

NFS::Null3Worker::Null3Worker(NFS::Client *client_,
                                Nan::Callback *callback)
    : Procedure3Worker(client_, 0, callback)
{}

void NFS::Null3Worker::procSuccess()
{
    v8::Local<v8::Value> argv[] = {
        Nan::Null()
    };
    callback->Call(sizeof(argv)/sizeof(*argv), argv);
}

void NFS::Null3Worker::procFailure()
{
    v8::Local<v8::Value> argv[] = {
        Nan::New(error?error:NFSC_UNKNOWN_ERROR).ToLocalChecked()
    };
    callback->Call(1, argv);
}

