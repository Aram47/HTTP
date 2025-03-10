#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <functional>

#ifdef __linux__
#include <bits/c++config.h>
#endif

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <CustomError.hpp>
#include <Request.hpp>
#include <Response.hpp> 
#include <Next.hpp>


class HTTPServer {
public:
  using req_handler = std::function<void(const Request&, const Response&, const Next&)>;
public:
  class __Router {
    public:
      using table = std::unordered_map<std::string, std::vector<req_handler>>;
    public:
      __Router Use(const std::vector<req_handler>&);
      __Router Use(const std::string&, const std::vector<req_handler>&);
  
      __Router Get(const std::string&, const std::vector<req_handler>&);
      __Router Post(const std::string&, const std::vector<req_handler>&);
      __Router Delete(const std::string&, const std::vector<req_handler>&);
      // __Router Put(const std::string&, const std::vector<req_handler>&);
      // __Router Patch(const std::string&, const std::vector<req_handler>&);
      // __Router Head(const std::string&, const std::vector<req_handler>&);
      // __Router Options(const std::string&, const std::vector<req_handler>&);
    private:
      table __use_table;
      table __get_table;
      table __post_table;
      table __delete_table;
      // table __put_table;
      // table __patch_table;
      // table __head_table;
      // table __options_table;
    };
private:
  using table = std::unordered_map<std::string, std::vector<req_handler>>;
public:
    using Router = HTTPServer::__Router;
public:
  /*
    constructors and destructor
  */
  HTTPServer();
  ~HTTPServer();
  HTTPServer(const HTTPServer&) = delete;
  HTTPServer(HTTPServer&&) = delete;
  HTTPServer& operator=(const HTTPServer&) = delete;
  HTTPServer& operator=(HTTPServer&&) = delete;
public:
  /*
    public methods for working with HTTPServer
  */
  void Listen(std::size_t, std::function<void()>);
  void Listen(std::size_t, const std::string&, std::function<void()>);

  void Use(const std::vector<req_handler>&);
  void Use(const std::string&, const std::vector<req_handler>&);
  void Use(const std::string&, const __Router&);
  
  void Get(const std::string&, const std::vector<req_handler>&);
  void Post(const std::string&, const std::vector<req_handler>&);
  void Delete(const std::string&, const std::vector<req_handler>&);
  // void Put(const std::string&, const std::vector<req_handler>&);
  // void Patch(const std::string&, const std::vector<req_handler>&);
  // void Head(const std::string&, const std::vector<req_handler>&);
  // void Options(const std::string&, const std::vector<req_handler>&);

  Router _Router();

private:
  void ListenHelper(std::size_t, const std::string&, std::function<void()>);
private:
  /*
    sockaddr_in = a Struct that stores address and port information
  */
  sockaddr_in server_address;
  sockaddr_in client_address;
  int server_socket;
  int client_socket;
  std::size_t PORT;
  std::string HOST;
  table __use_table;
  table __get_table;
  table __post_table;
  table __delete_table;
  // table __put_table;
  // table __patch_table;
  // table __head_table;
  // table __options_table;
};

#define REQ_HANDLER(__cb) static_cast<HTTPServer::req_handler>(__cb)

#endif // __HTTP_SERVER__
