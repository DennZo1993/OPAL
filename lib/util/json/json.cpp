#include "json.h"
#include "fs/File.h"


namespace util {

Json JsonFromString(const std::string &inputStr)
{
  std::string error = "";
  Json result = json11::Json::parse(inputStr, error);

  if (error != "")
    throw JsonParseException("Error while parsing JSON: " + error);

  return result;
}


Json JsonFromFile(const std::string &fileName)
{
  std::string contents = ReadFileToString(fileName);
  return JsonFromString(contents);
}


Json JsonFromFile(const char *fileName)
{
  return JsonFromFile(std::string(fileName));
}


Json JsonFromFile(std::ifstream &inputStream)
{
  std::string contents = ReadFileToString(inputStream);
  return JsonFromString(contents);
}

} // namespace util

