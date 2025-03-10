#include <iostream>
#include <HTTP.hpp>

int main() {
    HTTPServer server;
    server.Get("/", {REQ_HANDLER([](const Request& req, const Response& res, const Next& next) {
      req.request();
      res.response();
      next.next();
    })});
    server.Listen(2489, []() {
        std::cout << "Server is running on port 2489" << std::endl;
    });
    return 0;
}