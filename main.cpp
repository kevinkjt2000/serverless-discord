#include <cstdlib>
#include <iostream>

#include <aws/lambda-runtime/runtime.h>
#include <nlohmann/json.hpp>

using namespace aws::lambda_runtime;
using json = nlohmann::json;

std::string getEnvVar(std::string const & key)
{
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}

static invocation_response my_handler(invocation_request const& req)
{
    std::cout << "env(foo): " << getEnvVar("foo") << std::endl;
    auto parsed_payload = json::parse(req.payload);
    if (parsed_payload.contains("type") && parsed_payload["type"] == 1) {
        auto response = R"(
          {
            "type": 1
          }
        )"_json;
        return invocation_response::success(response.dump(), "application/json");
    }

    return invocation_response::failure("unexpected non-ping payload received",
                                        "error type here");
}

int main()
{
    run_handler(my_handler);
    return 0;
}
