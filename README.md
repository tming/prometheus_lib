# prometheus_lib
prometheus clinet for c++, very simple demo

it's a simple prometheus encode code with c++, it will output the Text format example of https://prometheus.io/docs/instrumenting/exposition_formats/

TODO:
1. it dose not support Nan, +Inf, and -Inf
2. it is not thread-safe, if you want use it in multi-thread env, you should change it to fit, such as notify the thead with pipe, or just with mutex lock
3. it is not flexible, for example, it will be more flexible with json for labels
