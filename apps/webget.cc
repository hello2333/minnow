#include "address.hh"
#include "debug.hh"
#include "socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

using namespace std;

namespace {
void get_URL( const string& host, const string& path )
{
  debug( "Function called: get_URL( \"{}\", \"{}\" )", host, path );
  // Use TCPSocket and Address to send an format of HTTP request to the server
  // Create a TCPSocket: which address
  Address local_address("0.0.0.0", 8081);
  // 使用connect的时候，这里传host和host+path有区别吗？
  Address peer_address(host, "http");

  TCPSocket tcp_socket;
  // listen: should client call "listen"?
  // tcp_socket.listen();
  // accept: should client call "accept"?
  //TCPSocket connected_socket = tcp_socket.accept();
  tcp_socket.bind(local_address);
  tcp_socket.connect(peer_address);

  // send: how to send the request, which function
  // how to close the connection
  std::vector<std::string> request = {
    "GET " + path + " HTTP/1.1\r\n",
    "Host: " + host + "\r\n",
    "Connection: close\r\n",
  };
  tcp_socket.write(std::move(request));

  // rev from server
  // how to judge if reach the EOF of the response?
  std::string response;
  while (not tcp_socket.eof()) {
    tcp_socket.read(response);
    if (not response.empty()) {
      debug("response: {}", response);
      response.clear();
    }
  }
  // debug( "get_URL() function not yet implemented" );
  debug( "get_URL() function done" );
}
} // namespace

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
