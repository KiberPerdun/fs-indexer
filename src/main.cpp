//
// Created by KiberPerdun on 10.05.2026.
//

#include "json.hpp"
#include "scanner.h"
#include "server.h"
#include <getopt.h>
#include <pwd.h>
#include <unistd.h>

[[noreturn]] void
fsindexer_worker (MediaServer &server, const std::filesystem::path &path,
                  int32_t interval)
{
  FsIndexer indexer;
  for (;;)
    {
      auto res = indexer.scan (path);

      nlohmann::json j = { { "audio", res.audioFiles },
                           { "video", res.videoFiles },
                           { "images", res.imageFiles } };

      /*
       * Во время тестов было выявлена проблема с парсом названия файлов в
       * ломанной кодировке
       */
      server.setData (
          j.dump (-1, ' ', false, nlohmann::json::error_handler_t::replace));
      std::this_thread::sleep_for (std::chrono::seconds (interval));
    }
}

int
main (int argc, char **argv)
{
  MediaServer server;
  uid_t uid = geteuid ();
  struct passwd *pw = getpwuid (uid);
  std::filesystem::path path;
  int32_t interval = 10;

  if (!pw)
    {
      std::cerr << "Failed to get user home directory" << std::endl;
      return -1;
    }

  path = pw->pw_dir;

  static struct option options[]{ { "path", required_argument, nullptr, 'p' },
                                  { "interval", required_argument, nullptr,
                                    'i' },
                                  { nullptr, 0, nullptr, 0 } };

  int32_t opt;
  int32_t option_index = 0;
  while ((opt = getopt_long (argc, argv, "p:i:", options, &option_index))
         != -1)
    {
      switch (opt)
        {
        case 'p':
          path = optarg;
          break;
        case 'i':
          try
            {
              interval = std::stoi (optarg);
            }
          catch (...)
            {
              std::cerr << "Invalid interval value\n";
              return -1;
            }
          break;
        default:
          return -1;
        }
    }

  if (!std::filesystem::exists (path))
    {
      std::cerr << "Invalid path:" << path << std::endl;
      return -1;
    }

  std::thread worker (fsindexer_worker, std::ref (server), path, interval);
  worker.detach ();
  server.run ("0.0.0.0", 1234);

  return 0;
}
