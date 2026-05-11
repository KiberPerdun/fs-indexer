//
// Created by KiberPerdun on 11.05.2026.
//

#include "server.h"

void
MediaServer::run (const std::string &host, const int32_t port)
{
  svr.Get ("/media_files",
           [this] (const httplib::Request &, httplib::Response &res)
             {
               std::string data_to_send;
               {
                 std::lock_guard<std::mutex> lock (mtx);
                 data_to_send = data;
               }
               res.set_content (data_to_send, "application/json");
             });

  svr.listen (host, port);
}
