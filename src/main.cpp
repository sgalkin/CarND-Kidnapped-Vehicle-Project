#include <iostream>
#include <memory>

#include <uWS/uWS.h>
#include "ProgramOptions.hxx"

#include "map.h"
#include "application.h"
#include "protocol.h"
#include "parameters.h"

namespace {
  po::parser parser() {
    po::parser parser;
    parser["help"]
      .abbreviation('?')
      .description("print this help screen")
      .callback([&parser]{
          std::cout << parser << '\n';
        });
    parser["map"]
      .abbreviation('m')
      .description("map file")
      .type(po::string);
    return parser;
  }
}

int main(int argc, char* argv[])
{  
  using App = Application<WSProtocol, Parameters, std::default_random_engine>;
  
  auto parser = ::parser();
  if(!parser(argc, argv) ||
     !parser["map"].available()) {
    std::cerr << parser << "\n";
    return -1;
  }

  static std::random_device rnd;
  std::default_random_engine engine{rnd()};

  const auto map = read_map(parser["map"].get().string);
  
  uWS::Hub h;
  std::unique_ptr<App> app;

  h.onMessage([&app](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode) {
    try {
      auto message = std::string(data, length);
      auto response = app->ProcessMessage(std::move(message));
      ws.send(response.data(), response.length(), uWS::OpCode::TEXT);
    } catch(std::runtime_error& e) {
      std::cerr << "Error while processing message: " << e.what() << std::endl;
    }
  });

  h.onConnection([&h,&app,&map,&engine](uWS::WebSocket<uWS::SERVER>, uWS::HttpRequest) {
    std::cout << "Connected" << std::endl;
    app.reset(new App(map, engine));
  });

  h.onDisconnection([&h,&app](uWS::WebSocket<uWS::SERVER>, int, char *, size_t) {
    std::cout << "Disconnected" << std::endl;
    app.reset();
  });

  constexpr int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << port << std::endl;
    return -1;
  }
  h.run();
  return 0;
}
