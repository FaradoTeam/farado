// test_token_expiration.cpp
#include <boost/test/unit_test.hpp>
#include <thread>
#include <chrono>

#include "vault-server/src/api/middleware/auth_middleware.h"

#include "test_server_fixture.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace farado::server::test
{

BOOST_FIXTURE_TEST_CASE(test_token_expiration, TestServerFixture)
{
    auto client = createClient();
    
    // Генерируем токен с коротким сроком действия (2 секунды)
    AuthMiddleware auth(TEST_SECRET_KEY);
    std::string shortLivedToken = auth.generateToken("test_user", 2);
    
    // Сразу после генерации токен должен работать
    {
        http_request request { methods::GET };
        request.set_request_uri("/api/items");
        request.headers().add("Authorization", "Bearer " + shortLivedToken);
        
        auto response = client.request(request).get();
        BOOST_CHECK_EQUAL(response.status_code(), status_codes::OK);
    }
    
    // Ждем истечения срока действия
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    // Токен больше не должен работать
    {
        http_request request { methods::GET };
        request.set_request_uri("/api/items");
        request.headers().add("Authorization", "Bearer " + shortLivedToken);
        
        auto response = client.request(request).get();
        BOOST_CHECK_EQUAL(response.status_code(), status_codes::Unauthorized);
    }
}

} // namespace farado::server::test
