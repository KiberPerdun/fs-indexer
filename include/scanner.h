//
// Created by KiberPerdun on 10.05.2026.
//

#ifndef FS_INDEXER_SCANNER_H
#define FS_INDEXER_SCANNER_H

#include <filesystem>
#include <vector>

struct FsIndexerScanResult
{
  std::vector<std::string> audioFiles;
  std::vector<std::string> videoFiles;
  std::vector<std::string> imageFiles;
};

class FsIndexer
{
  public:
  FsIndexerScanResult scan (const std::filesystem::path &path);
};

#endif //FS_INDEXER_SCANNER_H