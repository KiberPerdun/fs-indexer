//
// Created by KiberPerdun on 10.05.2026.
//

#include "scanner.h"
#include <iostream>

#include <algorithm>
#include <unordered_set>

namespace fs = std::filesystem;

FsIndexerScanResult
FsIndexer::scan (const std::filesystem::path &path)
{
  FsIndexerScanResult res;
  std::error_code err;

  /* В случае удаления папки для сканирования, возвращаем пустой результат,
   * я считаю это нормальным поведением, потому что если папки нет - значит
   * и файлов в ней нет. Мы уже имеем проверку того существует ли путь
   * при запуске приложения, поэтому если папка удалится или доступ к ней
   * будет утрачен, просто будем возвращать пустой массив, что бы не прерывать
   * выполнение программы.
   */
  if (!fs::exists (path, err) || !fs::is_directory (path, err))
    return res;

  res.audioFiles.reserve (1024);
  res.imageFiles.reserve (1024);
  res.videoFiles.reserve (1024);

  auto options = fs::directory_options::skip_permission_denied;

  try
    {
      for (const auto &entry :
           fs::recursive_directory_iterator (path, options))
        {
          if (!entry.is_regular_file (err))
            continue;

          const fs::path &file = entry.path ();

          std::string extension = file.extension ().string ();
          std::string filename = file.filename ().string ();
          std::transform (extension.begin (), extension.end (),
                          extension.begin (), ::tolower);

          static const std::unordered_set<std::string> audioExts
              = { ".mp3", ".wav", ".flac", ".ogg", ".m4a" };
          static const std::unordered_set<std::string> videoExts
              = { ".mp4", ".mkv", ".avi", ".mov", ".wmv" };
          static const std::unordered_set<std::string> imageExts
              = { ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp" };

          if (audioExts.count (extension))
            res.audioFiles.push_back (filename);

          else if (videoExts.count (extension))
            res.videoFiles.push_back (filename);

          else if (imageExts.count (extension))
            res.imageFiles.push_back (filename);
        }
    }
  catch (const fs::filesystem_error &e)
    {
      std::cerr << e.what () << std::endl;
    }

  return res;
}
