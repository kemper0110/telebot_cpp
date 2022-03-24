#pragma once

#include "pch.h"

#include "User.h"

class Chat
{
public:
	int id;
	enum class Type { Private, Group, Supergroup, Channel } type;
	std::optional<std::string> title;
	std::optional<std::string> username;
	std::optional<std::string> first_name;
	std::optional<std::string> last_name;
	//bool all_members_are_administrators;
};

template<>
struct nlohmann::adl_serializer<Chat> {
	static void from_json(const json& j, Chat& c) {
		j.at("id").get_to(c.id);
		const static std::unordered_map<std::string, Chat::Type> toType{
			{"private", Chat::Type::Private},
			{"group", Chat::Type::Group},
			{"supergroup", Chat::Type::Supergroup},
			{"channel", Chat::Type::Channel}
		};
		c.type = toType.at(j.at("type").get<std::string>());

		if (auto i = j.find("title"); i != j.end())
			c.title.emplace(i->get<std::string>());
		if (auto i = j.find("username"); i != j.end())
			c.username.emplace(i->get<std::string>());
		if (auto i = j.find("first_name"); i != j.end())
			c.first_name.emplace(i->get<std::string>());
		if (auto i = j.find("last_name"); i != j.end())
			c.last_name.emplace(i->get<std::string>());
	}
};