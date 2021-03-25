#pragma once

#include <aws/lambda-runtime/runtime.h>

#include "ed25519.h"

aws::lambda_runtime::invocation_response my_handler(aws::lambda_runtime::invocation_request const& req, SignatureInterface const& verifier);
aws::lambda_runtime::invocation_response my_handler_shim(aws::lambda_runtime::invocation_request const& req);
