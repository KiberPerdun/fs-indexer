//
// Created by KiberPerdun on 10.05.2026.
//

#include "json.hpp"
#include "scanner.h"
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <unistd.h>

int
main ()
{
  FsIndexer indexer;
  uid_t uid = geteuid ();
  struct passwd *pw = getpwuid (uid);

  if (!pw)
    return -1;

  std::filesystem::path homePath = pw->pw_dir;

  FsIndexerScanResult res = indexer.scan (homePath);
  homePath /= ".media_files";
  std::ofstream stream (homePath);
  stream << nlohmann::json{
    { "audio", res.audioFiles },
    { "video", res.videoFiles },
    { "images", res.imageFiles }
  }.dump (2);

  return 0;
}
