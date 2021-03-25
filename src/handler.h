#pragma once

#include <aws/lambda-runtime/runtime.h>

aws::lambda_runtime::invocation_response my_handler(aws::lambda_runtime::invocation_request const& req);
