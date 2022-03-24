#pragma once
#include "pch.h"

class User
{
public:
	int id;
	std::string first_name;
	std::optional<std::string> last_name;
	std::optional<std::string> username;
};

template<>
struct nlohmann::adl_serializer<User> {
	static void from_json(const json& j, User& u) {
		j.at("id").get_to(u.id);
		j.at("first_name").get_to(u.first_name);

		if (auto i = j.find("last_name"); i != j.end())
			u.last_name.emplace(i->get<std::string>());
		if (auto i = j.find("username"); i != j.end())
			u.username.emplace(i->get<std::string>());
	}
};
