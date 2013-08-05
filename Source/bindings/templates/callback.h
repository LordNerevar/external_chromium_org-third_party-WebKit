/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// This file has been auto-generated by CodeGeneratorV8.pm. DO NOT MODIFY!

#ifndef {{v8_class_name}}_h
#define {{v8_class_name}}_h

{% if conditional_string %}
#if {{conditional_string}}
{% endif %}
{% for filename in header_includes %}
#include "{{filename}}"
{% endfor %}

namespace WebCore {

class ScriptExecutionContext;

class {{v8_class_name}} : public {{cpp_class_name}}, public ActiveDOMCallback {
public:
    static PassRefPtr<{{v8_class_name}}> create(v8::Handle<v8::Value> value, ScriptExecutionContext* context)
    {
        ASSERT(value->IsObject());
        ASSERT(context);
        return adoptRef(new {{v8_class_name}}(v8::Handle<v8::Object>::Cast(value), context));
    }

    virtual ~{{v8_class_name}}();

{% for method in methods %}
    virtual {{method.return_type}} {{method.name}}({{method.argument_declaration}});
{% endfor %}

    virtual ScriptExecutionContext* scriptExecutionContext() const { return ContextLifecycleObserver::scriptExecutionContext(); }

private:
    {{v8_class_name}}(v8::Handle<v8::Object>, ScriptExecutionContext*);

    ScopedPersistent<v8::Object> m_callback;
    RefPtr<DOMWrapperWorld> m_world;
};

}

{% if conditional_string %}
#endif // {{conditional_string}}
{% endif %}
#endif // {{v8_class_name}}_h
