#pragma once

#include <exception>
#include <string>


namespace hmr::xcpt
{

////////////////////////////////////////////////////////////
class base : public std::exception
{
protected:
  std::string const err_msg;

public:
  base(std::string const &msg) : err_msg(msg) {}

  const char *what() const noexcept override
  {
    return err_msg.c_str();
  }
};


namespace format
{
  ////////////////////////////////////////////////////////////
  class need_more_data : public base
  {
  public:
    need_more_data(std::string const &msg) : base(msg) {}
  };

} // namespace format


namespace hex
{
  ////////////////////////////////////////////////////////////
  class invalid_input : public base
  {
  public:
    invalid_input(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class need_more_data : public base
  {
  public:
    need_more_data(std::string const &msg) : base(msg) {}
  };

} // namespace hex


namespace binary
{
  ////////////////////////////////////////////////////////////
  class invalid_input : public base
  {
  public:
    invalid_input(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class need_more_data : public base
  {
  public:
    need_more_data(std::string const &msg) : base(msg) {}
  };

} // namespace binary


namespace base64
{
  ////////////////////////////////////////////////////////////
  class invalid_alphabet : public base
  {
  public:
    invalid_alphabet(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class invalid_input : public base
  {
  public:
    invalid_input(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class need_more_data : public base
  {
  public:
    need_more_data(std::string const &msg) : base(msg) {}
  };

} // namespace base64


namespace url
{
  ////////////////////////////////////////////////////////////
  class invalid_input : public base
  {
  public:
    invalid_input(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class need_more_data : public base
  {
  public:
    need_more_data(std::string const &msg) : base(msg) {}
  };

} // namespace url


namespace kvp
{
  ////////////////////////////////////////////////////////////
  class parse_error : public base
  {
  public:
    parse_error(std::string const &msg) : base(msg) {}
  };

} // namespace kvp


namespace crypto
{
  ////////////////////////////////////////////////////////////
  class invalid_input : public base
  {
  public:
    invalid_input(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class invalid_key_length : public base
  {
  public:
    invalid_key_length(std::string const &msg) : base(msg) {}
  };

  ////////////////////////////////////////////////////////////
  class invalid_iv_length : public base
  {
  public:
    invalid_iv_length(std::string const &msg) : base(msg) {}
  };

} // namespace crypto


} // namespace hmr::xcpt
