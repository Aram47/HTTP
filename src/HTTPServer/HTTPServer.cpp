#include <HTTPServer.hpp>

using Router = HTTPServer::__Router;

HTTPServer::HTTPServer():
  server_address{},
  client_address{},
  server_socket{0},
  client_socket{0},
  __use_table{},
  __get_table{},
  __post_table{},
  __delete_table{}
  // __put_table{},
  // __patch_table{},
  // __head_table{},
  // __options_table{}
{}

HTTPServer::~HTTPServer()
{
  close(this->client_socket);
  close(this->server_socket);
}

void HTTPServer::ListenHelper(std::size_t PORT, const std::string& HOST, std::function<void()> cb) {
  if (PORT > 65535) {
    std::cerr << "Invalid PORT" << std::endl;
    return;
  }
	this->PORT = PORT;

  /*
    will check inputed HOST
    and after that will initialize this->HOST
  */
  if (HOST.empty() /*and some checks too*/) {
    std::cerr << "Invalid HOST" << std::endl;
    return;
  }
  this->HOST = HOST;

  /*
    AF_INET       = using IPv4
    SOCK_STREAM   = using TCP protocol
    0             = default protocol for using TCP

    if function socket will be return -1 it's meen
    we have an error
  */
  this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->server_socket == -1) {
    std::cerr << "Error creating socket!" << std::endl;
    return;
  }

  //
  /*
    AF_INET = We use IPv4;
    htons(PORT) = converts a port from host format (regular number) to
                  network format (big-endian)
    INADDR_ANY = means that the server will accept connections to
                  all available IP addresses.
  */
 	int opt = 1;
	setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  this->server_address.sin_family = AF_INET;
  this->server_address.sin_port = htons(PORT);
  this->server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(this->server_socket,
           (sockaddr*)&this->server_address,
           sizeof(this->server_address)) == -1)
  {
    std::cerr << "Error binding socket!" << std::endl;
    return;
  }

  /*
    SOMAXCONN = the maximum number of pending connections in the queue.
    SOMAXCONN = 4096
  */
	#ifdef __APPLE__
  	const int BACKLOG = 128;  // macOS may have lower SOMAXCONN
	#else
    const int BACKLOG = SOMAXCONN;
	#endif
  if (listen(this->server_socket, BACKLOG) == -1) {
    std::cerr << "Listening error!" << std::endl;
    return;
  }

  socklen_t client_address_size = sizeof(this->client_address);
  cb();

  this->client_socket = accept(this->server_socket,
                            (sockaddr*)&this->client_address,
                            &client_address_size);

  if (this->client_socket == -1) {
      std::cerr << "Error accepting connection!" << std::endl;
      return;
  }

  // cb();
}

void HTTPServer::Listen(std::size_t PORT, std::function<void()> cb) {
  this->ListenHelper(PORT, "localhost", cb);
};

void HTTPServer::Listen(std::size_t PORT, const std::string& HOST, std::function<void()> cb) {
  this->ListenHelper(PORT, HOST, cb);
}

// not finished yet

void HTTPServer::Use(const std::vector<HTTPServer::req_handler>& __req_handlers) {
  this->Use("/", this->_Router().Use(__req_handlers));
};

void HTTPServer::Use(const std::string& __path, 
                     const std::vector<HTTPServer::req_handler>& __req_handlers) 
{
  this->Use(__path, this->_Router().Use(__path, __req_handlers));
};

void HTTPServer::Use(const std::string& __path, const HTTPServer::__Router& __router) {
  
  std::cout << __path << std::endl;
  __Router rt = __router;
}

void HTTPServer::Get(const std::string& __path, 
                     const std::vector<HTTPServer::req_handler>& __req_handlers) 
{
  if (this->__get_table.find(__path) != this->__get_table.end()) {
    throw new CustomError("GET method already exists");
  }

  if (this->__use_table.find("/") != this->__use_table.end()) {
    for (auto it : __use_table["/"]) {
      this->__get_table[__path].push_back(it);
    }
  }

  for (auto it : __req_handlers) {
    this->__get_table[__path].push_back(it);
  }
};

void HTTPServer::Post(const std::string& __path, 
                      const std::vector<HTTPServer::req_handler>& __req_handlers)
{
  if (this->__post_table.find(__path) != this->__post_table.end()) {
    throw new CustomError("POST method already exists");
  }

  if (this->__use_table.find("/") != this->__use_table.end()) {
    for (auto it : __use_table["/"]) {
      this->__post_table[__path].push_back(it);
    }
  }

  for (auto it : __req_handlers) {
    this->__post_table[__path].push_back(it);
  }
};

void HTTPServer::Delete(const std::string& __path, 
                        const std::vector<HTTPServer::req_handler>& __req_handlers) 
{
  if (this->__delete_table.find(__path) != this->__delete_table.end()) {
    throw new CustomError("DELETE method already exists");
  }

  if (this->__use_table.find("/") != this->__use_table.end()) {
    for (auto it : __use_table["/"]) {
      this->__delete_table[__path].push_back(it);
    }
  }

  for (auto it : __req_handlers) {
    this->__delete_table[__path].push_back(it);
  }
};

// void HTTPServer::Put(const std::string& __path, 
//                      const std::vector<HTTPServer::req_handler>& __req_handlers) 
// {
  
// };

// void HTTPServer::Patch(const std::string& __path, 
//                        const std::vector<HTTPServer::req_handler>& __req_handlers) 
// {
  
// };

// void HTTPServer::Head(const std::string& __path, 
//                       const std::vector<HTTPServer::req_handler>& __req_handlers) 
// {
  
// };

// void HTTPServer::Options(const std::string& __path, 
//                          const std::vector<HTTPServer::req_handler>& __req_handlers) 
// {
  
// };

Router HTTPServer::_Router() {
  return Router();
};

Router HTTPServer::__Router::Use(const std::vector<HTTPServer::req_handler>& __req_handlers) {
  if (this->__use_table.find("/") == this->__use_table.end()) {
    this->__use_table["/"] = __req_handlers;
  } else {
    for (auto it : __req_handlers) {
      this->__use_table["/"].push_back(it);
    }
  }
  return *this;
};

Router HTTPServer::__Router::Use(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {
  if (__path == "/") {
    this->Use(__req_handlers);
    return *this;
  }
    // will check is valid path or not
  if (this->__use_table.find(__path) == this->__use_table.end()) {
    this->__use_table[__path] = __req_handlers;
  } else {
    for (auto it : __req_handlers) {
      this->__use_table[__path].push_back(it);
    }
  }
  return *this;
};

Router HTTPServer::__Router::Get(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {
  if (this->__get_table.find(__path) != this->__get_table.end()) {
    throw new CustomError("GET method already exists");
  }

  if (this->__use_table.find("/") != this->__use_table.end()) {
    for (auto it : __use_table["/"]) {
      this->__get_table[__path].push_back(it);
    }
  }

  for (auto it : __req_handlers) {
    this->__get_table[__path].push_back(it);
  }

  return *this;
};

Router HTTPServer::__Router::Post(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {
  if (this->__post_table.find(__path) != this->__post_table.end()) {
    throw new CustomError("POST method already exists");
  }

  if (this->__use_table.find("/") != this->__use_table.end()) {
    for (auto it : __use_table["/"]) {
      this->__post_table[__path].push_back(it);
    }
  }

  for (auto it : __req_handlers) {
    this->__post_table[__path].push_back(it);
  }

  return *this;
};

Router HTTPServer::__Router::Delete(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {
  if (this->__delete_table.find(__path) != this->__delete_table.end()) {
    throw new CustomError("Delete method already exists");
  }

  if (this->__use_table.find("/") != this->__use_table.end()) {
    for (auto it : __use_table["/"]) {
      this->__delete_table[__path].push_back(it);
    }
  }

  for (auto it : __req_handlers) {
    this->__delete_table[__path].push_back(it);
  }

  return *this;
};

// Router HTTPServer::__Router::Put(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {

// };

// Router HTTPServer::__Router::Patch(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {

// };

// Router HTTPServer::__Router::Head(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {

// };

// Router HTTPServer::__Router::Options(const std::string& __path, const std::vector<HTTPServer::req_handler>& __req_handlers) {

// };











// void HTTPServer::HandleRequest() {
//   char buffer[1024] = {0};
//   ssize_t valread = read(this->client_socket, buffer, 1024);
//   if (valread == -1) {
//     std::cerr << "Error reading from socket!" << std::endl;
//     return;
//   }

//   std::string request(buffer);
//   std::string method = request.substr(0, request.find(' '));
//   std::string path = request.substr(request.find(' ') + 1, request.find(' ', request.find(' ') + 1) - request.find(' ') - 1);
//   std::string body = request.substr(request.find("\r\n\r\n") + 4);

//   Streamable req;
//   req.method = method;
//   req.path = path;
//   req.body = body;

//   Streamable res;
//   res.method = method;
//   res.path = path;
//   res.body = body;

//   Next next;
//   next.__use_table = this->__use_table;
//   next.__get_table = this->__get_table;
//   next.__post_table = this->__post_table;
//   next.__delete_table = this->__delete_table;
//   next.__put_table = this->__put_table;
//   next.__patch_table = this->__patch_table;
//   next.__head_table = this->__head_table;
//   next.__options_table = this->__options_table;

//   next.__use_table[path] = this->__use_table[path];
//   next.__get_table[path] = this->__get_table[path];
//   next.__post_table[path] = this->__post_table[path];
//   next.__delete_table[path] = this->__delete_table[path];
//   next.__put_table[path] = this->__put_table[path];
//   next.__patch_table[path] = this->__patch_table[path];
//   next.__head_table[path] = this->__head_table[path];
//   next.__options_table[path] = this->__options_table[path];

//   next.__use_table["/"] = this->__use_table["/"];
//   next.__get_table["/"] = this->__get_table["/"];
//   next.__post_table["/"] = this->__post_table["/"];
//   next.__delete_table["/"] = this->__delete_table["/"];
//   next.__put_table["/"] = this->__put_table["/"];
//   next.__patch_table["/"] = this->__patch_table["/"];
//   next.__head_table
//   ["/"] = this->__head_table["/"];
//   next.__options_table["/"] = this->__options_table["/"];
// };