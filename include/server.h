//
// Created by KiberPerdun on 10.05.2026.
//

#ifndef FS_INDEXER_SERVER_H
#define FS_INDEXER_SERVER_H

#include "httplib.h"
#include <cstdint>
#include <mutex>

class MediaServer
{
  private:
  httplib::Server svr;
  std::mutex mtx;
  std::string data;

  public:
  void
  setData (const std::string &data_)
  {
    std::lock_guard<std::mutex> lock (mtx);
    data = data_;
  }
  void run (const std::string &host, int32_t port);
};

#endif //FS_INDEXER_SERVER_H