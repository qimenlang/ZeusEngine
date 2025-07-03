#pragma once
#include <string>
#include <vector>

class Component;

class Object {
public:
  Object(/* args */);
  ~Object();

  size_t getId() const { return m_id; }

  void setName(std::string name) { m_name = name; }
  const std::string &getName() const { return m_name; }

  void tick(float delta_time);
  bool load(const std::string &path);

private:
  /* data */
  size_t m_id;
  std::string m_name;
  std::string m_res_path;

  std::vector<Component> m_components;
};
