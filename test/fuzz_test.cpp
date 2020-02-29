#include "shared_types.h"
#include <string>
#include <meta_json/from_json.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::string s((const char*)data, size);
  try {
  } catch (...) {
    auto f = meta_json::from_json<Flat>(s);
  }
  return 0;
}
