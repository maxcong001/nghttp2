#pragma once

#include <string_view>
#include <string>
#include <locale>
#include <codecvt>
#include <map>
using namespace std::string_view_literals;
namespace nghttp2 {
namespace asio_http2 {
inline static std::string_view trim(std::string_view v) {
  v.remove_prefix((std::min)(v.find_first_not_of(" "), v.size()));
  v.remove_suffix((std::min)(v.size() - v.find_last_not_of(" ") - 1, v.size()));
  return v;
}
// input should be like:
// test=test&testa=testa
inline static std::map<std::string_view, std::string_view>
parse_query(std::string_view str) {
  std::map<std::string_view, std::string_view> query;
  std::string_view key;
  std::string_view val;
  size_t pos = 0;
  size_t length = str.length();
  for (size_t i = 0; i < length; i++) {
    char c = str[i];
    if (c == '=') {
      key = {&str[pos], i - pos};
      key = trim(key);
      pos = i + 1;
    } else if (c == '&') {
      val = {&str[pos], i - pos};
      val = trim(val);
      pos = i + 1;
      // if (is_form_url_encode(key)) {
      //	auto s = form_urldecode(key);
      //}
      query.emplace(key, val);
    }
  }

  if (pos == 0) {
    return {};
  }

  if ((length - pos) > 0) {
    val = {&str[pos], length - pos};
    val = trim(val);
    query.emplace(key, val);
  } else if ((length - pos) == 0) {
    query.emplace(key, "");
  }

  return query;
}

inline static std::string url_encode(const std::string &value) noexcept {
  static auto hex_chars = "0123456789ABCDEF";

  std::string result;
  result.reserve(value.size()); // Minimum size of result

  for (auto &chr : value) {
    if (!((chr >= '0' && chr <= '9') || (chr >= 'A' && chr <= 'Z') ||
          (chr >= 'a' && chr <= 'z') || chr == '-' || chr == '.' ||
          chr == '_' || chr == '~'))
      result += std::string("%") +
                hex_chars[static_cast<unsigned char>(chr) >> 4] +
                hex_chars[static_cast<unsigned char>(chr) & 15];
    else
      result += chr;
  }

  return result;
}

inline static std::string url_decode(const std::string &value) noexcept {
  std::string result;
  result.reserve(value.size() / 3 +
                 (value.size() % 3)); // Minimum size of result

  for (std::size_t i = 0; i < value.size(); ++i) {
    auto &chr = value[i];
    if (chr == '%' && i + 2 < value.size()) {
      auto hex = value.substr(i + 1, 2);
      auto decoded_chr =
          static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
      result += decoded_chr;
      i += 2;
    } else if (chr == '+')
      result += ' ';
    else
      result += chr;
  }

  return result;
}

inline static std::string u8wstring_to_string(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(wstr);
}

inline static std::wstring u8string_to_wstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(str);
}

inline static std::string get_string_by_urldecode(std::string_view content) {
  return url_decode(std::string(content.data(), content.size()));
}

inline static bool is_url_encode(std::string_view str) {
  return str.find("%") != std::string_view::npos ||
         str.find("+") != std::string_view::npos;
}
} // namespace asio_http2
} // namespace nghttp2
